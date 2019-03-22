/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jan/21/2019
//***********************************************************************************************************
#include "MS51.h"


/*****************************************************************************/
/*The main C function.  Program execution starts                             */
/*here after stack initialization.                                           */
/*****************************************************************************/
void main (void) 
{
    unsigned char temp;
    P12_QUASI_MODE;
    ENABLE_ADC_BANDGAP;
#if 0
  while(1)
    {
      clr_ADCCON0_ADCF;
      set_ADCCON0_ADCS;                                  /*Each time ADC start trig signal*/
      while(!(ADCCON0&0x80));                            /* wait ADCF to 1 */
      temp = ADCRH;
      P12 = 0 ;
      Timer0_Delay(24000000,200,1000);
      P12 = 1;
      Timer0_Delay(24000000,200,1000);
    }

#else
    MODIFY_HIRC_24();
    UART_Open(24000000,UART0_Timer3,115200);
    ENABLE_UART0_PRINTF;
    while(1)
     {
      clr_ADCCON0_ADCF;
      set_ADCCON0_ADCS;                                  // Each time ADC start trig signal
       while(!(ADCCON0&0x80));
      temp = ADCRH;
      printf ("\n Value = 0x%bx",ADCRH);
      printf ("\n Value = 0x%bx",ADCRL);
      Timer0_Delay(24000000,200,1000);
    }
#endif
}

