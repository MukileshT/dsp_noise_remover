



#ifndef _CODECIF_H_
#define _CODECIF_H_



#define CODEC_INTERFACE_I2C


extern void I2CCodecIfInit(unsigned int baseAddr, unsigned int intCh,
                           unsigned int slaveAddr);

extern void CodecRegWrite(unsigned int baseAddr, unsigned char regAddr,
                          unsigned char regData);
extern void CodecRegBitSet(unsigned int baseAddr, unsigned char regAddr,
                           unsigned char bitMask);
extern void CodecRegBitClr(unsigned int baseAddr, unsigned char regAddr,
                           unsigned char bitMask);
extern unsigned char CodecRegRead(unsigned int baseAddr,
                                  unsigned char regAddr);

#endif
