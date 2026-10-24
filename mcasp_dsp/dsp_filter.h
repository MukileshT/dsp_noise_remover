#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#define FS_HZ     16000.0f
#define NTAPS     129          /* odd, symmetric (linear-phase) FIR */
#define CUTOFF_HZ 4000.0f      /* -6 dB point; stopband starts ~4.2 kHz */

void  dsp_init(void);
float dsp_voice_filter(float x);   /* one sample in, one sample out */

#endif