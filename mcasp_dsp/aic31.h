



#ifndef _AIC31_H_
#define _AIC31_H_



#define AIC31_DATATYPE_I2S             (0u << 6u)
#define AIC31_DATATYPE_DSP             (1u << 6u)
#define AIC31_DATATYPE_RIGHTJ          (2u << 6u)
#define AIC31_DATATYPE_LEFTJ           (3u << 6u)


#define AIC31_MODE_ADC                 (0xF0u)
#define AIC31_MODE_DAC                 (0x0Fu)
#define AIC31_MODE_BOTH                (0xFFu)


extern void AIC31Reset(unsigned int baseAddr);
extern void AIC31DataConfig(unsigned int baseAddr, unsigned char dataType,
                            unsigned char slotWidth, unsigned char dataOff);
extern void AIC31SampleRateConfig(unsigned int baseAddr, unsigned int mode,
                                  unsigned int sampleRate);
extern void AIC31ADCInit(unsigned int baseAddr);
extern void AIC31DACInit(unsigned int baseAddr);

#endif
