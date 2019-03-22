/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/*  Website: http://www.nuvoton.com                                                                        */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : Jan/21/2019                                                                                   */
/***********************************************************************************************************/

/**********************************************************************************************************/
/*  File Function: MS51 ADC multi channel input demo code                                                 */
/**********************************************************************************************************/
#include "MS51.h"

unsigned char xdata ADCdataAIN5H, ADCdataAIN5L;
unsigned char xdata ADCdataVBGH, ADCdataVBGL;

/*****************************************************************************/
/*The main C function.  Program execution starts                             */
/*here after stack initialization.                                           */
/*****************************************************************************/
void main (void) 
{
  /* UART0 settting for printf function */
    MODIFY_HIRC_24();
    UART_Open(24000000,UART0_Timer3,115200);
    ENABLE_UART0_PRINTF;
  
/*Enable channel 5 */ 
      ENABLE_ADC_AIN5;
      ADCCON1|=0X30;            /* clock divider */
      ADCCON2|=0X0E;            /* AQT time */
      AUXR1|=SET_BIT4;          /* ADC clock low speed */
      clr_ADCCON0_ADCF;
      set_ADCCON0_ADCS;                                
      while(ADCF == 0);
      ADCdataAIN5H = ADCRH;
      ADCdataAIN5L = ADCRL;
      DISABLE_ADC;
      printf("\n ADC channel 5 =0x%bx", ADCdataAIN5H);

/*Enable Bandgap */     
      ENABLE_ADC_BANDGAP;
      ADCCON1|=0X30;            /* clock divider */
      ADCCON2|=0X0E;            /* AQT time */
      AUXR1|=SET_BIT4;          /* ADC clock low speed */
      clr_ADCCON0_ADCF;
      set_ADCCON0_ADCS;                                
      while(ADCF == 0);
      ADCdataVBGH = ADCRH;
      ADCdataVBGL = ADCRL;
      DISABLE_ADC;
      printf("\n ADC channel bandgap =0x%bx", ADCdataVBGH);
      
    while(1);  
}