/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.       */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/* Website: http://www.nuvoton.com                                                                         */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : Jan/21/2019                                                                                   */
/***********************************************************************************************************/


//***********************************************************************************************************
//  File Function: MS51 system clock enable ECLK demo code
//***********************************************************************************************************
#include "MS51.h"


//========================================================================
//  The test process:
//  1. Power on is run as default HIRC, show LED Fsys tickle faster
//   2. toggle P3.0 to GND.
//  2. call modify Fsys code to LIRC.
//  3. LED tickle speed slowly than before.
//========================================================================

void FSYS_ECLK_ENABLE(void)
{
    TA=0xAA;TA=0X55;CKEN|=0XC0;              //step1: enable ECLK clock source run
    while((CKSWT|CLR_BIT3)==CLR_BIT3);        //step2: check ready
    clr_CKSWT_OSC1;                          //step3: switching system clock source if needed
    set_CKSWT_OSC0;
    while((CKEN&SET_BIT0)==SET_BIT0);        //step4: check system clock switching OK or NG
}

void FSYS_HIRC_ENABLE(void)
{
    set_CKEN_HIRCEN;                        //step1: enable HIRC clock source run
    while((CKSWT&SET_BIT5)==0);        //step2: check ready
    TA=0xAA;TA=0x55;CKSWT&=0xF9;
    while((CKEN&SET_BIT0)==1);        //step4: check system clock switching OK or NG
    TA=0xAA;                          //step1: enable ECLK clock source run
    TA=0X55;
    CKEN&=0X3F;
}


void main(void)
{

/* Note
  MCU power on system clock is HIRC (16 MHz)
  Please keep P3.0 HIGH before you want to modify Fsys to LIRC
*/
  
    P11_QUASI_MODE;
    set_CKCON_CLOEN;                                  // Also can check P1.1 CLO pin for clock to find the Fsys change.
    FSYS_ECLK_ENABLE();
    Timer0_Delay(16000000,200,1000);;
    FSYS_HIRC_ENABLE();
    Timer0_Delay(16000000,2000,1000);;
    set_PCON_PD;
    while(1);

}

