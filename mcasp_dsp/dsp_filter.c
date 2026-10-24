#include <math.h>
#include <string.h>
#include "dsp_filter.h"

#define PI_D 3.14159265358979323846

static float h[NTAPS];
static float buf[2 * NTAPS];   /* duplicated circular buffer */
static int   pos;

/* Hamming-windowed sinc low-pass, unity gain at DC. */
void dsp_init(void)
{
    const int M = NTAPS - 1;
    const double wc = 2.0 * PI_D * CUTOFF_HZ / FS_HZ;
    double t[NTAPS], sum = 0.0;
    int n;

    for (n = 0; n < NTAPS; n++)
    {
        double m = n - M / 2.0;
        double ideal = (m == 0.0) ? wc / PI_D : sin(wc * m) / (PI_D * m);
        t[n] = ideal * (0.54 - 0.46 * cos(2.0 * PI_D * n / M));
        sum += t[n];
    }
    for (n = 0; n < NTAPS; n++)
        h[n] = (float)(t[n] / sum);

    memset(buf, 0, sizeof(buf));
    pos = 0;
}

float dsp_voice_filter(float x)
{
    const float *p;
    float acc = 0.0f;
    int i;

    pos = (pos == 0) ? (NTAPS - 1) : (pos - 1);
    buf[pos] = x;
    buf[pos + NTAPS] = x;
    p = &buf[pos];

    for (i = 0; i < NTAPS; i++)
        acc += h[i] * p[i];
    return acc;
}