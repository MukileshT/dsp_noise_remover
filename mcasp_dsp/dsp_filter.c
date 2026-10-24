#include <math.h>
#include <string.h>
#include "dsp_filter.h"

#define PI_D  3.14159265358979323846
#define M     (NTAPS - 1)      /* 128 */
#define HALF  (M / 2)          /* 64: center tap index */

static float h[HALF + 1];      /* h[0..64], symmetric: h[M-i] == h[i] */
static float buf[2 * NTAPS];   /* duplicated circular buffer */
static int   pos;

/* Hamming-windowed sinc low-pass, unity gain at DC. */
void dsp_init(void)
{
    const double wc = 2.0 * PI_D * CUTOFF_HZ / FS_HZ;
    double t[NTAPS], sum = 0.0;
    int n;

    for (n = 0; n < NTAPS; n++)
    {
        double m = n - HALF;
        double ideal = (n == HALF) ? wc / PI_D : sin(wc * m) / (PI_D * m);
        t[n] = ideal * (0.54 - 0.46 * cos(2.0 * PI_D * n / M));
        sum += t[n];
    }
    for (n = 0; n <= HALF; n++)
        h[n] = (float)(t[n] / sum);

    memset(buf, 0, sizeof(buf));
    pos = 0;
}

/* Force optimization for this function even if the project is built -O0. */
#ifdef _TMS320C6X
#pragma FUNCTION_OPTIONS(dsp_voice_filter, "-o3")
#endif
float dsp_voice_filter(float x)
{
    const float *restrict p;
    const float *restrict hh = h;
    float a0 = 0.0f, a1 = 0.0f;
    int i;

    pos = (pos == 0) ? (NTAPS - 1) : (pos - 1);
    buf[pos] = x;
    buf[pos + NTAPS] = x;
    p = &buf[pos];

    /* Symmetric FIR: 64 pair-multiplies instead of 129, two accumulators. */
    #pragma MUST_ITERATE(32, 32)
    for (i = 0; i < HALF; i += 2)
    {
        a0 += hh[i]     * (p[i]     + p[M - i]);
        a1 += hh[i + 1] * (p[i + 1] + p[M - i - 1]);
    }
    return a0 + a1 + hh[HALF] * p[HALF];
}