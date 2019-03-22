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
/*  File Function: MS51 Watchdog reset disable by modify CONFIG setting demo                                */
/************************************************************************************************************/
#include "MS51.h"

/***********************************************************************
  WDT CONFIG enable 
  warning : this macro is only when ICP not enable CONFIG WDT function
  copy this marco code to you code to enable WDT reset.
************************************************************************/
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
/***********************************************************************
  WDT CONFIG disable 
  warning : this macro is for CONFIG already enable WDT Reset to disable.
  Since erase CONFIG is full page 
  step 1. storage CONFIG value in RAM
  step 2. modify RAM CONFIG4 WDT value to disable 
  step 3. erase CONFIG page
  step 4. re writer CONFIG.
************************************************************************/
void Disable_WDT_Reset_Config(void)
{
  UINT8 cf0,cf1,cf2,cf3,cf4;
  
    set_CHPCON_IAPEN;
    IAPAL = 0x00;
    IAPAH = 0x00;
    IAPCN = BYTE_READ_CONFIG;
    set_IAPTRG_IAPGO;                                  //Storage CONFIG0 data
    cf0 = IAPFD;
    IAPAL = 0x01;
    set_IAPTRG_IAPGO;                                  //Storage CONFIG1 data
    cf1 = IAPFD;
    IAPAL = 0x02;
    set_IAPTRG_IAPGO;                                  //Storage CONFIG2 data
    cf2 = IAPFD;
    IAPAL = 0x03;
    set_IAPTRG_IAPGO;                                  //Storage CONFIG3 data
    cf3 = IAPFD;
    IAPAL = 0x04;
    set_IAPTRG_IAPGO;                                  //Storage CONFIG4 data
    cf4 = IAPFD;
    cf4 |= 0xF0;                                      //Moidfy Storage CONFIG4 data disable WDT reset
    
    set_IAPUEN_CFUEN;  
    IAPCN = PAGE_ERASE_CONFIG;                        //Erase CONFIG all
    IAPAH = 0x00;
    IAPAL = 0x00;
    IAPFD = 0xFF;
    set_IAPTRG_IAPGO;
    
    IAPCN = BYTE_PROGRAM_CONFIG;                    //Write CONFIG
    IAPFD = cf0;
    set_IAPTRG_IAPGO;
    IAPAL = 0x01;
    IAPFD = cf1;
    set_IAPTRG_IAPGO;
    IAPAL = 0x02;
    IAPFD = cf2;
    set_IAPTRG_IAPGO;
    IAPAL = 0x03;
    IAPFD = cf3;
    set_IAPTRG_IAPGO;
    IAPAL = 0x04;
    IAPFD = cf4;
    set_IAPTRG_IAPGO;

    set_IAPUEN_CFUEN;
    clr_CHPCON_IAPEN;
}

/***********************************************************************************************************/
/*    Main function                                                                                        */
/************************************************************************************************************/
void main (void)
{

    ALL_GPIO_QUASI_MODE;

    P12 = 0;
    Timer0_Delay(16000000,50,1000);          //toggle I/O to show MCU Reset
    P12 = 1;
    Timer0_Delay(16000000,50,1000); 
    P12 = 0;
    Timer0_Delay(16000000,50,1000); 
    P12 = 1;
    Timer0_Delay(16000000,50,1000); 

  
/*----------------------------------------------------------------------------------------------*/
/* WDT Init !!! ENABLE CONFIG WDT FIRST !!!                                                     */
/* Warning:                                                                                     */
/* Always check CONFIG WDT enable first, CONFIG not enable, SFR can't enable WDT reset          */
/* Please call Enable_WDT_Reset_Config() function to enable CONFIG WDT reset                    */
/*----------------------------------------------------------------------------------------------*/

    Enable_WDT_Reset_Config();                  //Software enable WDT reset CONFIG setting
    TA=0xAA;TA=0x55;WDCON|=0x07;                //Setting WDT prescale 
    set_WDCON_WDCLR;                            //Clear WDT timer
    while((WDCON|~SET_BIT6)==0xFF);             //confirm WDT clear is ok before into power down mode

    set_WDCON_WDTR;                             //WDT run
    Disable_WDT_Reset_Config();               //Software disable WDT reset CONFIG setting

    while (1)
    {
      P12 = 0;
      Timer0_Delay(16000000,100,1000); 
      P12 = 1;
      Timer0_Delay(16000000,100,1000);
      P12 = 0;
      Timer0_Delay(16000000,100,1000);
      P12 = 1;
      Timer0_Delay(16000000,100,1000);
      P12 = 0;
      set_PCON_PD;
    }
}

