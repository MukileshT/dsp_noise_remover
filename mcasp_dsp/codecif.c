



#include "soc_C6748.h"
#include "interrupt.h"
#include "hw_syscfg0_C6748.h"
#include "i2c.h"
#include "codecif.h"


static void I2CCodecIsr(void);
static void I2CCodecIntSetup(unsigned int sysIntNum, unsigned int channel);
static void I2CCodecSendBlocking(unsigned int baseAddr, unsigned int dataCnt);
static void I2CCodecRcvBlocking(unsigned int baseAddr, unsigned int dataCnt);


volatile unsigned int dataIdx = 0;
volatile unsigned int txCompFlag = 1;
volatile unsigned int slaveData[3];
unsigned int savedBase;



static void I2CCodecIntSetup(unsigned int sysIntNum, unsigned int channel)
{
#ifdef _TMS320C6X
	IntRegister(C674X_MASK_INT4, I2CCodecIsr);
	IntEventMap(C674X_MASK_INT4, sysIntNum);
	IntEnable(C674X_MASK_INT4);
#else

    IntRegister(sysIntNum, I2CCodecIsr);
    IntChannelSet(sysIntNum, channel);
    IntSystemEnable(sysIntNum);
#endif
}


void I2CCodecIfInit(unsigned int baseAddr, unsigned int intCh,
                    unsigned int slaveAddr)
{
    unsigned int sysIntNum = 0;


    I2CMasterDisable(baseAddr);


    I2CMasterInitExpClk(baseAddr, 24000000, 8000000, 100000);


    I2CMasterSlaveAddrSet(baseAddr, slaveAddr);

    I2CMasterEnable(baseAddr);

    if(SOC_I2C_0_REGS == baseAddr)
    {
#ifdef _TMS320C6X
    	sysIntNum = SYS_INT_I2C0_INT;
#else
        sysIntNum = SYS_INT_I2CINT0;
#endif
    }

    I2CCodecIntSetup(sysIntNum, intCh);
}


static void I2CCodecSendBlocking(unsigned int baseAddr, unsigned int dataCnt)
{
    txCompFlag = 1;
    dataIdx = 0;
    savedBase = baseAddr;

    I2CSetDataCount(baseAddr, dataCnt);

    I2CMasterControl(baseAddr, I2C_CFG_MST_TX | I2C_CFG_STOP);

    I2CMasterIntEnableEx(baseAddr, I2C_INT_TRANSMIT_READY | I2C_INT_STOP_CONDITION);

    I2CMasterStart(baseAddr);

    while(txCompFlag);
}


static void I2CCodecRcvBlocking(unsigned int baseAddr, unsigned int dataCnt)
{
    txCompFlag = 1;
    dataIdx = 0;
    savedBase = baseAddr;

    I2CSetDataCount(baseAddr, dataCnt);

    I2CMasterControl(baseAddr, I2C_CFG_MST_RX | I2C_CFG_STOP);

    I2CMasterIntEnableEx(baseAddr, I2C_INT_DATA_READY | I2C_INT_STOP_CONDITION);

    I2CMasterStart(baseAddr);


    while(txCompFlag);
}


void I2CCodecIsr(void)
{
    unsigned int intCode = 0;
    unsigned int sysIntNum = 0;


    intCode = I2CInterruptVectorGet(savedBase);

    if(SOC_I2C_0_REGS == savedBase)
    {
#ifdef _TMS320C6X
    	sysIntNum = SYS_INT_I2C0_INT;
#else
         sysIntNum = SYS_INT_I2CINT0;
#endif
    }

    else
    {
         intCode = 0;
    }

    while(intCode!=0)
    {

#ifdef _TMS320C6X
    	IntEventClear(sysIntNum);
#else
        IntSystemStatusClear(sysIntNum);
#endif

         if (intCode == I2C_INTCODE_TX_READY)
         {
              I2CMasterDataPut(savedBase, slaveData[dataIdx]);
              dataIdx++;
         }

         if(intCode == I2C_INTCODE_RX_READY)
         {
              slaveData[dataIdx] = I2CMasterDataGet(savedBase);
              dataIdx++;
         }

         if (intCode == I2C_INTCODE_STOP)
         {

              I2CMasterIntDisableEx(savedBase, I2C_INT_TRANSMIT_READY
                                               | I2C_INT_DATA_READY);
              txCompFlag = 0;
         }

         intCode = I2CInterruptVectorGet(savedBase);
    }
}


void CodecRegWrite(unsigned int baseAddr, unsigned char regAddr,
                   unsigned char regData)
{
#ifdef CODEC_INTERFACE_I2C


    slaveData[0] = regAddr;
    slaveData[1] = regData;

    I2CCodecSendBlocking(baseAddr, 2);
#endif
}


unsigned char CodecRegRead(unsigned int baseAddr, unsigned char regAddr)
{
#ifdef CODEC_INTERFACE_I2C


    slaveData[0] = regAddr;
    I2CCodecSendBlocking(baseAddr, 1);


    I2CCodecRcvBlocking(baseAddr, 1);

#endif

    return (slaveData[0]);
}


void CodecRegBitSet(unsigned int baseAddr, unsigned char regAddr,
                    unsigned char bitMask)
{
#ifdef CODEC_INTERFACE_I2C


    slaveData[0] = regAddr;
    I2CCodecSendBlocking(baseAddr, 1);

    I2CCodecRcvBlocking(baseAddr, 1);

    slaveData[1] =  slaveData[0] | bitMask;
    slaveData[0] = regAddr;

    I2CCodecSendBlocking(baseAddr, 2);

#endif
}


void CodecRegBitClr(unsigned int baseAddr, unsigned char regAddr,
                    unsigned char bitMask)
{
#ifdef CODEC_INTERFACE_I2C


    slaveData[0] = regAddr;
    I2CCodecSendBlocking(baseAddr, 1);


    I2CCodecRcvBlocking(baseAddr, 1);

    slaveData[1] =  slaveData[0] & ~bitMask;
    slaveData[0] = regAddr;

    I2CCodecSendBlocking(baseAddr, 2);

#endif
}


