/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/* Website: http://www.nuvoton.com                                                                         */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : Jan/21/2019                                                                                   */
/***********************************************************************************************************/

/************************************************************************************************************/
/*  File Function: MS51 Watchdog reset enable by modify CONFIG demo code                                    */
/************************************************************************************************************/
#include "MS51.h"

/****************************************************************************/
/*  WDT CONFIG enable                                                       */
/*  warning : this macro is only when ICP not enable CONFIG WDT function    */
/*  copy this marco code to you code to enable WDT reset.                   */
/****************************************************************************/
void Enable_WDT_Reset_Config(void)
{
    set_CHPCON_IAPEN;
    IAPAL = 0x04;
    IAPAH = 0x00;
    IAPFD = 0x0F;
    IAPCN = BYTE_PROGRAM_CONFIG;
    set_IAPUEN_CFUEN;
    set_IAPTRG_IAPGO;                                  //trigger IAP
    while((CHPCON&SET_BIT6)==SET_BIT6);          //check IAPFF (CHPCON.6)
    clr_IAPUEN_CFUEN;
    clr_CHPCON_IAPEN;
}

/************************************************************************************************************
*    Main function 
************************************************************************************************************/
void main (void)
{

    ALL_GPIO_QUASI_MODE;

    P12 = 0;
    Timer0_Delay(16000000,50,1000);          /*toggle I/O to show MCU Reset*/
    P12 = 1;
    Timer0_Delay(16000000,50,1000);
    P12 = 0;
    Timer0_Delay(16000000,50,1000);
    P12 = 1;
    Timer0_Delay(16000000,50,1000);

  
/*---------------------------------------------------------------------------------------------- */
/* WDT Init !!! ENABLE CONFIG WDT FIRST !!!                                                      */
/* Warning:                                                                                      */
/* Always check CONFIG WDT enable first, CONFIG not enable, SFR can't enable WDT reset           */
/* Please call Enable_WDT_Reset_Config() function to enable CONFIG WDT reset                     */
/*---------------------------------------------------------------------------------------------- */

    Enable_WDT_Reset_Config();                  //Software enable WDT reset CONFIG setting
    TA=0xAA;TA=0x55;WDCON=0x06;                 //Setting WDT prescale 
    set_WDCON_WDCLR;                            //Clear WDT timer
    while((WDCON|~SET_BIT6)==0xFF);             //confirm WDT clear is ok before into power down mode
    set_WDCON_WDTR;                             //WDT run

    while (1)
    {
      set_PCON_PD;
    }
}

