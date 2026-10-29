



#include "codecif.h"
#include "aic31.h"



#define AIC31_P0_REG0               (0)
#define AIC31_P0_REG1               (1)
#define AIC31_P0_REG2               (2)
#define AIC31_P0_REG3               (3)
#define AIC31_P0_REG4               (4)
#define AIC31_P0_REG5               (5)
#define AIC31_P0_REG6               (6)
#define AIC31_P0_REG7               (7)
#define AIC31_P0_REG8               (8)
#define AIC31_P0_REG9               (9)
#define AIC31_P0_REG10              (10)
#define AIC31_P0_REG11              (11)
#define AIC31_P0_REG12              (12)
#define AIC31_P0_REG13              (13)
#define AIC31_P0_REG14              (14)
#define AIC31_P0_REG15              (15)
#define AIC31_P0_REG16              (16)
#define AIC31_P0_REG17              (17)
#define AIC31_P0_REG18              (18)
#define AIC31_P0_REG19              (19)
#define AIC31_P0_REG20              (20)
#define AIC31_P0_REG21              (21)
#define AIC31_P0_REG22              (22)
#define AIC31_P0_REG23              (23)
#define AIC31_P0_REG24              (24)
#define AIC31_P0_REG25              (25)
#define AIC31_P0_REG26              (26)
#define AIC31_P0_REG27              (27)
#define AIC31_P0_REG28              (28)
#define AIC31_P0_REG29              (29)
#define AIC31_P0_REG30              (30)
#define AIC31_P0_REG31              (31)
#define AIC31_P0_REG32              (32)
#define AIC31_P0_REG33              (33)
#define AIC31_P0_REG34              (34)
#define AIC31_P0_REG35              (35)
#define AIC31_P0_REG36              (36)
#define AIC31_P0_REG37              (37)
#define AIC31_P0_REG38              (38)
#define AIC31_P0_REG40              (40)
#define AIC31_P0_REG41              (41)
#define AIC31_P0_REG42              (42)
#define AIC31_P0_REG43              (43)
#define AIC31_P0_REG44              (44)
#define AIC31_P0_REG45              (45)
#define AIC31_P0_REG46              (46)
#define AIC31_P0_REG47              (47)
#define AIC31_P0_REG48              (48)
#define AIC31_P0_REG49              (49)
#define AIC31_P0_REG50              (50)
#define AIC31_P0_REG51              (51)
#define AIC31_P0_REG52              (52)
#define AIC31_P0_REG53              (53)
#define AIC31_P0_REG54              (54)
#define AIC31_P0_REG55              (55)
#define AIC31_P0_REG56              (56)
#define AIC31_P0_REG57              (57)
#define AIC31_P0_REG58              (58)
#define AIC31_P0_REG59              (59)
#define AIC31_P0_REG60              (60)
#define AIC31_P0_REG61              (61)
#define AIC31_P0_REG62              (62)
#define AIC31_P0_REG63              (63)
#define AIC31_P0_REG64              (64)
#define AIC31_P0_REG65              (65)
#define AIC31_P0_REG66              (66)
#define AIC31_P0_REG67              (67)
#define AIC31_P0_REG68              (68)
#define AIC31_P0_REG69              (69)
#define AIC31_P0_REG70              (70)
#define AIC31_P0_REG71              (71)
#define AIC31_P0_REG72              (72)
#define AIC31_P0_REG73              (73)
#define AIC31_P0_REG74              (74)
#define AIC31_P0_REG75              (75)
#define AIC31_P0_REG76              (76)
#define AIC31_P0_REG77              (77)
#define AIC31_P0_REG78              (78)
#define AIC31_P0_REG79              (79)
#define AIC31_P0_REG80              (80)
#define AIC31_P0_REG81              (81)
#define AIC31_P0_REG82              (82)
#define AIC31_P0_REG83              (83)
#define AIC31_P0_REG84              (84)
#define AIC31_P0_REG85              (85)
#define AIC31_P0_REG86              (86)
#define AIC31_P0_REG87              (87)
#define AIC31_P0_REG88              (88)
#define AIC31_P0_REG89              (89)
#define AIC31_P0_REG90              (90)
#define AIC31_P0_REG91              (91)
#define AIC31_P0_REG92              (92)
#define AIC31_P0_REG93              (93)
#define AIC31_P0_REG94              (94)
#define AIC31_P0_REG95              (95)
#define AIC31_P0_REG96              (96)
#define AIC31_P0_REG97              (97)
#define AIC31_P0_REG98              (98)
#define AIC31_P0_REG99              (99)
#define AIC31_P0_REG100             (100)
#define AIC31_P0_REG101             (101)
#define AIC31_P0_REG102             (102)

#define AIC31_RESET                 (0x80)

#define AIC31_SLOT_WIDTH_16         (0u << 4u)
#define AIC31_SLOT_WIDTH_20         (1u << 4u)
#define AIC31_SLOT_WIDTH_24         (2u << 4u)
#define AIC31_SLOT_WIDTH_32         (3u << 4u)



void AIC31Reset(unsigned int baseAddr)
{

    CodecRegWrite(baseAddr, AIC31_P0_REG0, 0);


    CodecRegWrite(baseAddr, AIC31_P0_REG1, AIC31_RESET);
}


void AIC31DataConfig(unsigned int baseAddr, unsigned char dataType,
                     unsigned char slotWidth, unsigned char dataOff)
{
    unsigned char slot;

    switch(slotWidth)
    {
        case 16:
            slot = AIC31_SLOT_WIDTH_16;
        break;

        case 20:
            slot = AIC31_SLOT_WIDTH_20;
        break;

        case 24:
            slot = AIC31_SLOT_WIDTH_24;
        break;

        case 32:
            slot = AIC31_SLOT_WIDTH_32;
        break;

        default:
            slot = AIC31_SLOT_WIDTH_16;
        break;
    }



    CodecRegWrite(baseAddr, AIC31_P0_REG9, (dataType | slot));

    CodecRegWrite(baseAddr, AIC31_P0_REG10, dataOff);
}


void AIC31SampleRateConfig(unsigned int baseAddr, unsigned int mode,
                           unsigned int sampleRate)
{
    unsigned char fs;
    unsigned char ref = 0x0Au;
    unsigned char temp;
    unsigned char pllPval = 4u;
    unsigned char pllRval = 1u;
    unsigned char pllJval = 16u;
    unsigned short pllDval = 0u;


    switch(sampleRate)
    {
        case 8000:
            fs = 0xAAu;
        break;

        case 11025:
            fs = 0x66u;
            ref = 0x8Au;
            pllJval = 14u;
            pllDval = 7000u;
        break;

        case 16000:
            fs = 0x44u;
        break;

        case 22050:
            fs = 0x22u;
            ref = 0x8Au;
            pllJval = 14u;
            pllDval = 7000u;
        break;

        case 24000:
            fs = 0x22u;
        break;

        case 32000:
            fs = 0x11u;
        break;

        case 44100:
            ref = 0x8Au;
            fs = 0x00u;
            pllJval = 14u;
            pllDval = 7000u;
        break;

        case 48000:
            fs = 0x00u;
        break;

        case 96000:
            ref = 0x6Au;
            fs = 0x00u;
        break;

        default:
            fs = 0x00u;
        break;
    }

    temp = (mode & fs);

    CodecRegWrite(baseAddr, AIC31_P0_REG2, temp);

    CodecRegWrite(baseAddr, AIC31_P0_REG3, 0x80 | pllPval);


    CodecRegWrite(baseAddr, AIC31_P0_REG102, 0x08);


    CodecRegBitClr(baseAddr, AIC31_P0_REG101, 0x01);


    CodecRegWrite(baseAddr, AIC31_P0_REG98, 0x20);

    temp = (pllJval << 2);
    CodecRegWrite(baseAddr, AIC31_P0_REG4, temp);


    CodecRegWrite(baseAddr, AIC31_P0_REG5, (pllDval >> 6) & 0xFF);
    CodecRegWrite(baseAddr, AIC31_P0_REG6, (pllDval & 0x3F) << 2);

    temp = pllRval;
    CodecRegWrite(baseAddr, AIC31_P0_REG11, temp);


    CodecRegWrite(baseAddr, AIC31_P0_REG8, 0xD0);

    CodecRegWrite(baseAddr, AIC31_P0_REG7, ref);
}


void AIC31ADCInit(unsigned int baseAddr)
{

    CodecRegWrite(baseAddr, AIC31_P0_REG15, 0x00);
    CodecRegWrite(baseAddr, AIC31_P0_REG16, 0x00);


    CodecRegWrite(baseAddr, AIC31_P0_REG17, 0x00);


    CodecRegWrite(baseAddr, AIC31_P0_REG18, 0x00);


    CodecRegWrite(baseAddr, AIC31_P0_REG25, 0x80);


    CodecRegWrite(baseAddr, AIC31_P0_REG19, 0x04);


    CodecRegWrite(baseAddr, AIC31_P0_REG22, 0x04);
}


void AIC31DACInit(unsigned int baseAddr)
{

    CodecRegWrite(baseAddr, AIC31_P0_REG37, 0xE0);


    CodecRegWrite(baseAddr, AIC31_P0_REG41, 0x02);
    CodecRegWrite(baseAddr, AIC31_P0_REG42, 0x6C);



    CodecRegWrite(baseAddr, AIC31_P0_REG47, 0x80);
    CodecRegWrite(baseAddr, AIC31_P0_REG51, 0x09);


    CodecRegWrite(baseAddr, AIC31_P0_REG64, 0x80);
    CodecRegWrite(baseAddr, AIC31_P0_REG65, 0x09);


    CodecRegWrite(baseAddr, AIC31_P0_REG82, 0x80);
    CodecRegWrite(baseAddr, AIC31_P0_REG86, 0x09);


    CodecRegWrite(baseAddr, AIC31_P0_REG92, 0x80);
    CodecRegWrite(baseAddr, AIC31_P0_REG93, 0x09);


    CodecRegWrite(baseAddr, AIC31_P0_REG43, 0x00);
    CodecRegWrite(baseAddr, AIC31_P0_REG44, 0x00);
}


