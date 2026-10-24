
#ifndef DSP_FILTER_H
#define DSP_FILTER_H

#define FS_HZ        16000.0f
#define NTAPS        101
#define VOICE_LO_HZ  300.0f
#define VOICE_HI_HZ  3400.0f
#define NOISE_LO_HZ  500.0f
#define NOISE_HI_HZ  4000.0f
#define TARGET_SNR_DB 5.0f

void dsp_init(void);
float dsp_add_test_noise(float x);
float dsp_voice_filter(float x);

void dsp_process_block(
    const float *in,
    float *out,
    int n,
    int add_noise
);

#endif
