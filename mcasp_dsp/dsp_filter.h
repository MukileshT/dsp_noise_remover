
#ifndef DSP_FILTER_H
#define DSP_FILTER_H

// Sampling frequency of the input voice signal (16 kHz)
#define FS_HZ     16000.0f

// Number of FIR filter coefficients (129 taps)
// An odd number of taps is used for symmetrical coefficients
#define NTAPS     129

// Cutoff frequency of the low-pass filter (4 kHz)
// Frequencies above this value are attenuated
#define CUTOFF_HZ 4000.0f

// Function to initialize the FIR filter
// Calculates filter coefficients and clears the input buffer
void dsp_init(void);

// Function to process one input sample
// Input  : x (unfiltered voice sample)
// Output : Filtered voice sample
float dsp_voice_filter(float x);

#endif // DSP_FILTER_H
