#include <lpc214x.h>
#include "types.h"
#include "delay.h"
#include "adc.h"
#define FOSC 12000000  //Hz
#define CCLK (FOSC*5)  //Hz
#define PCLK (CCLK/4)  //Hz
#define ADCCLK 3000000 //Hz
#define CLKDIV ((PCLK/ADCCLK)-1)

//defines for ADCR sfr                                                                                                                                    +
#define CLKDIV_START_BIT 8 //bit 8 to 15
#define PDN_BIT          (1<<21)
#define ADC_START_BIT    24

//define for ADDR sfr
#define RESULT_START_BIT 6 //bit 6 to 15
#define DONE_BIT         31
#define CH_SEL 0x02
void Init_ADC(void)
{
    //def already ain0,ain1,ain2,ain3

    //confgd by start up code

    //cfg ADCR sfr
    PINSEL1 |= 0X15400000;
    AD0CR = PDN_BIT | CLKDIV | CH_SEL;
}

f32 Read_ADC(u32 ch)
{
    u32 adcDVal;
    f32 eAR;

    //masking channel No
    AD0CR &= 0xFFFFFF00;

    //update channel No & start adc conversion
    AD0CR |= (1 << ADC_START_BIT) | (1 << ch);

    //wait for > 2.44us
    delay_us(3);

    //check status of conversion complete
    while (((AD0GDR >> DONE_BIT) & 1) == 0);

    AD0CR &= ~(1 << ADC_START_BIT);

    adcDVal = ((AD0GDR >> RESULT_START_BIT) & 1023);

    eAR = (adcDVal * (3.3 / 1023));

    return eAR;
}
