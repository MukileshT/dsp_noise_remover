#include <math.h>
#include <stdint.h>
#include <string.h>
#include "dsp_filter.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef TARGET_SNR_DB
#define TARGET_SNR_DB 5.0f
#endif

typedef struct
{
    float h[NTAPS];
    float buf[2 * NTAPS];
    int pos;
} fir_t;

static fir_t voice_fir;
static fir_t noise_fir;

static uint32_t rng_state = 2463534242u;
static float ps_avg = 1.0e-6f;
static float pn_avg = 1.0e-6f;
static float pw_alpha;
static float snr_linear;

/* Design an FIR band-pass filter using a Hamming window. */
static void design_bandpass(float *h, float f1, float f2, float fs)
{
    int n;
    const int M = NTAPS - 1;
    const double w1 = 2.0 * M_PI * f1 / fs;
    const double w2 = 2.0 * M_PI * f2 / fs;
    double temp[NTAPS];

    for (n = 0; n < NTAPS; n++)
    {
        double m = (double)n - (double)M / 2.0;
        double ideal;
        double window;

        if (fabs(m) < 1.0e-12)
        {
            ideal = (w2 - w1) / M_PI;
        }
        else
        {
            ideal = (sin(w2 * m) - sin(w1 * m)) / (M_PI * m);
        }

        window = 0.54 - 0.46 * cos(2.0 * M_PI * n / M);
        temp[n] = ideal * window;
    }

    /* Normalize gain at the centre of the passband. */
    {
        double w0 = 0.5 * (w1 + w2);
        double re = 0.0;
        double im = 0.0;
        double gain;

        for (n = 0; n < NTAPS; n++)
        {
            re += temp[n] * cos(w0 * n);
            im -= temp[n] * sin(w0 * n);
        }

        gain = sqrt(re * re + im * im);

        if (gain < 1.0e-12)
            gain = 1.0;

        for (n = 0; n < NTAPS; n++)
            h[n] = (float)(temp[n] / gain);
    }
}

/* Run the FIR using a duplicated circular buffer. */
static float fir_run(fir_t *f, float x)
{
    const float *restrict h = f->h;
    const float *restrict p;
    float acc = 0.0f;
    int i;

    f->pos = (f->pos == 0) ? (NTAPS - 1) : (f->pos - 1);

    f->buf[f->pos] = x;
    f->buf[f->pos + NTAPS] = x;

    p = &f->buf[f->pos];

    for (i = 0; i < NTAPS; i++)
        acc += h[i] * p[i];

    return acc;
}

/* Pseudo-random uniform noise in approximately [-1, 1). */
static float rand_uniform(void)
{
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;

    return (float)((int32_t)rng_state) *
           (1.0f / 2147483648.0f);
}

/* Approximate Gaussian noise. */
static float rand_gauss(void)
{
    float sum = 0.0f;
    int i;

    for (i = 0; i < 12; i++)
        sum += 0.5f * (rand_uniform() + 1.0f);

    return sum - 6.0f;
}

/* Call once before processing audio. */
void dsp_init(void)
{
    memset(&voice_fir, 0, sizeof(voice_fir));
    memset(&noise_fir, 0, sizeof(noise_fir));

    voice_fir.pos = 0;
    noise_fir.pos = 0;

    design_bandpass(
        voice_fir.h,
        VOICE_LO_HZ,
        VOICE_HI_HZ,
        FS_HZ
    );

    design_bandpass(
        noise_fir.h,
        NOISE_LO_HZ,
        NOISE_HI_HZ,
        FS_HZ
    );

    /* Approximately 0.5-second running power averaging. */
    pw_alpha = 1.0f - 1.0f / (0.5f * FS_HZ);

    ps_avg = 1.0e-6f;
    pn_avg = 1.0e-6f;

    rng_state = 2463534242u;
    snr_linear = (float)pow(10.0, (double)TARGET_SNR_DB / 10.0);
}

/* Optional test-noise generator. Do not use this on audio
   that already contains noise unless you want more noise. */
float dsp_add_test_noise(float x)
{
    float noise = fir_run(&noise_fir, rand_gauss());
    float gain;

    ps_avg = pw_alpha * ps_avg +
             (1.0f - pw_alpha) * x * x;

    pn_avg = pw_alpha * pn_avg +
             (1.0f - pw_alpha) * noise * noise;

    gain = sqrtf(ps_avg / (pn_avg * snr_linear + 1.0e-20f));

    return x + gain * noise;
}

/* Filter one input sample. Call dsp_init() first. */
float dsp_voice_filter(float x)
{
    return fir_run(&voice_fir, x);
}

/* Process a block of floating-point samples. */
void dsp_process_block(
    const float *in,
    float *out,
    int n,
    int add_noise
)
{
    int i;

    if (in == 0 || out == 0 || n <= 0)
        return;

    for (i = 0; i < n; i++)
    {
        float sample = in[i];

        if (add_noise)
            sample = dsp_add_test_noise(sample);

        out[i] = dsp_voice_filter(sample);
    }
}