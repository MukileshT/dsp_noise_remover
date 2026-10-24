/* dsp_filter.h - FIR band-pass (300-3400 Hz) + optional test-noise generator
 * Target: TMS320C6748 (floating point). Pure C, no OS/driver calls. */
#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#define FS_HZ        16000.0     /* MUST equal the codec sampling rate */
#define NTAPS        129         /* fir1 order 512 -> 513 taps         */
#define VOICE_LO_HZ  300.0
#define VOICE_HI_HZ  3400.0
#define NOISE_LO_HZ  4500.0
#define NOISE_HI_HZ  7000.0
#define TARGET_SNR_DB 5.0

void  dsp_init(void);                       /* call once at start-up           */
float dsp_add_test_noise(float x);          /* clean sample -> noisy sample    */
float dsp_voice_filter(float x);            /* noisy sample -> filtered sample */
void  dsp_process_block(const float *in, float *out, int n, int add_noise);

#endif
