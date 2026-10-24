#include <math.h>
#include <stdint.h>
#include <string.h>
#include "dsp_filter.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float h[NTAPS];
    float buf[2 * NTAPS];   /* history stored twice -> no modulo in the MAC loop */
    int   pos;
} fir_t;

static fir_t voice_fir;     /* 300-3400 Hz, the actual filter               */
static fir_t noise_fir;     /* 4500-7000 Hz, shapes the test noise          */

static uint32_t rng_state = 2463534242u;
static float    ps_avg = 1e-6f, pn_avg = 1e-6f;   /* running power estimates   */
static float    pw_alpha;

/* ---------- fir1(512,[f1 f2]/(Fs/2),'bandpass') equivalent (Hamming) ---------- */
static void design_bandpass(float *h, double f1, double f2, double fs)
{
    const int    M  = NTAPS - 1;              /* order = 512 */
    const double w1 = 2.0 * M_PI * f1 / fs;
    const double w2 = 2.0 * M_PI * f2 / fs;
    double tmp[NTAPS];
    int n;

    for (n = 0; n < NTAPS; n++) {
        double m   = n - 0.5 * M;             /* symmetric about the centre   */
        double ideal = (fabs(m) < 1e-9) ? (w2 - w1) / M_PI
                                        : (sin(w2 * m) - sin(w1 * m)) / (M_PI * m);
        double win = 0.54 - 0.46 * cos(2.0 * M_PI * n / M);   /* hamming(513) */
        tmp[n] = ideal * win;
    }
    /* fir1 scaling: unity gain at the centre of the pass band */
    {
        double w0 = 0.5 * (w1 + w2), re = 0.0, im = 0.0;
        for (n = 0; n < NTAPS; n++) {
            re += tmp[n] * cos(w0 * n);
            im -= tmp[n] * sin(w0 * n);
        }
        double g = sqrt(re * re + im * im);
        for (n = 0; n < NTAPS; n++) h[n] = (float)(tmp[n] / g);
    }
}

/* ---------- FIR with circular buffer written twice ---------- */
static float fir_run(fir_t *f, float x)
{
    const float *restrict h = f->h;
    const float *restrict p;
    float acc = 0.0f;
    int i;

    f->pos = (f->pos == 0) ? (NTAPS - 1) : (f->pos - 1);
    f->buf[f->pos]          = x;
    f->buf[f->pos + NTAPS]  = x;
    p = &f->buf[f->pos];                      /* p[0] newest ... p[NTAPS-1] oldest */

#pragma MUST_ITERATE(NTAPS, NTAPS)
    for (i = 0; i < NTAPS; i++)
        acc += h[i] * p[i];
    return acc;
}

/* ---------- white Gaussian-ish noise (replacement for randn) ---------- */
static float rand_uniform(void)               /* xorshift32 -> [-1,1) */
{
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return (float)((int32_t)rng_state) * (1.0f / 2147483648.0f);
}
static float rand_gauss(void)                 /* sum of 12 uniforms, unit variance */
{
    float s = 0.0f; int i;
    for (i = 0; i < 12; i++) s += 0.5f * (rand_uniform() + 1.0f);
    return s - 6.0f;
}

/* ---------- public API ---------- */
void dsp_init(void)
{
    memset(&voice_fir, 0, sizeof voice_fir);
    memset(&noise_fir, 0, sizeof noise_fir);
    design_bandpass(voice_fir.h, VOICE_LO_HZ, VOICE_HI_HZ, FS_HZ);
    design_bandpass(noise_fir.h, NOISE_LO_HZ, NOISE_HI_HZ, FS_HZ);
    pw_alpha = (float)(1.0 - 1.0 / (0.5 * FS_HZ));   /* ~0.5 s power averaging */
    ps_avg = pn_avg = 1e-6f;
}

/* MATLAB: n = nh*sqrt(sum(s.^2)/(sum(nh.^2)*10^(snr/10))); noisy = s + n;
 * The whole-file sums become running averages, so it works sample by sample. */
float dsp_add_test_noise(float x)
{
    const float snr_lin = (float)pow(10.0, TARGET_SNR_DB / 10.0);
    float nh = fir_run(&noise_fir, rand_gauss());
    float gain;

    ps_avg = pw_alpha * ps_avg + (1.0f - pw_alpha) * x  * x;
    pn_avg = pw_alpha * pn_avg + (1.0f - pw_alpha) * nh * nh;
    gain   = sqrtf(ps_avg / (pn_avg * snr_lin + 1e-20f));
    return x + gain * nh;
}

float dsp_voice_filter(float x) { return fir_run(&voice_fir, x); }

void dsp_process_block(const float *in, float *out, int n, int add_noise)
{
    int i;
    for (i = 0; i < n; i++) {
        float v = add_noise ? dsp_add_test_noise(in[i]) : in[i];
        out[i]  = dsp_voice_filter(v);
    }
}
