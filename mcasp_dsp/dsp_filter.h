#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#define FS_HZ     16000.0f
#define NTAPS     129
#define CUTOFF_HZ 4000.0f

void  dsp_init(void);
float dsp_voice_filter(float x);

#endif