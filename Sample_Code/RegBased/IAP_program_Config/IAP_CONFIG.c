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

//***********************************************************************************************************/
/*  File Function: MS51 ADC / Bandgap input demo code                                                       */
/************************************************************************************************************/
#include "MS51.h"
 
/*****************************************************************************************************************
 ONLY Write CONFIG to enable WDT Reset Function:
******************************************************************************************************************/
 unsigned char IAPCFBuf[5];

void Enable_WDT_Reset_Config(void)
{
    set_CHPCON_IAPEN;                                  // Enable IAP function
    IAPCN = BYTE_PROGRAM_CONFIG;
    set_IAPUEN_CFUEN;                                  // Enable CONFIG writer bit
    IAPAL = 0x04;
    IAPAH = 0x00;
    IAPFD = 0x0F;
    set_IAPTRG_IAPGO;                                  //trigger IAP
    clr_IAPUEN_CFUEN;
    clr_CHPCON_IAPEN;
}
/*****************************************************************************************************************
Whole CONFIG modify Function. Include erase CONFIG, be careful use and always desable interrupt.
******************************************************************************************************************/    
void Modify_CONFIG(unsigned char u8CF0,unsigned char u8CF1,unsigned char u8CF2,unsigned char u8CF3,unsigned char u8CF4)
{   
    unsigned char u8Count;
    DISABLE_GLOBAL_INTERRUPT;
/* Loop save original CONFIG data in XRAM  */
    set_CHPCON_IAPEN;                    // Enable IAP function
    IAPCN = BYTE_READ_CONFIG;            
    IAPAH = 0x00;
    for(u8Count=0;u8Count<5;u8Count++)  
    {        
        IAPAL = u8Count;
        set_IAPTRG_IAPGO; 
        IAPCFBuf[u8Count] = IAPFD;
    } 
/* Erase CONFIG setting    */
    set_IAPUEN_CFUEN;                    // APROM modify Enable
    IAPFD = 0xFF;                        // IMPORTANT !! To erase function must setting IAPFD = 0xFF 
    IAPCN = PAGE_ERASE_CONFIG;
    IAPAL = 0x00;
    set_IAPTRG_IAPGO;
/* Modify CONFIG setting as customer define */
    IAPCN = BYTE_PROGRAM_CONFIG;
    IAPFD = u8CF0;
    set_IAPTRG_IAPGO;
    IAPAL++;
    IAPFD = u8CF1;
    set_IAPTRG_IAPGO;
    IAPAL++;
    IAPFD = u8CF2;
    set_IAPTRG_IAPGO;
    IAPAL++;
    IAPFD = u8CF3;
    set_IAPTRG_IAPGO;
    IAPAL++;
    IAPFD = u8CF4;
    set_IAPTRG_IAPGO;
    clr_IAPUEN_CFUEN;
/* Check programed data, if not match, program the storaged before data.  */
    IAPCN = BYTE_READ_CONFIG;
    IAPAL = 0x00;
    set_IAPTRG_IAPGO; 
    if (IAPFD != u8CF0)
      goto MDCFEND;
    IAPAL++;
    set_IAPTRG_IAPGO; 
    if (IAPFD != u8CF1)
      goto MDCFEND;
    IAPAL++;
    set_IAPTRG_IAPGO; 
    if (IAPFD != u8CF2)
      goto MDCFEND;
    IAPAL++;
    set_IAPTRG_IAPGO; 
    if (IAPFD != u8CF3)
      goto MDCFEND;
    IAPAL++;
    set_IAPTRG_IAPGO; 
    if (IAPFD != u8CF4)
      goto MDCFEND;
    goto CFCLOSE;
MDCFEND:
    set_IAPUEN_CFUEN;                      // APROM modify Enable
    for(u8Count=0;u8Count<5;u8Count++)    // Loop page erase APROM special define address area.
    {        
        IAPAL = u8Count;
        IAPFD = IAPCFBuf[u8Count];
        set_IAPTRG_IAPGO; 
    } 
CFCLOSE:
    clr_IAPUEN_CFUEN;                    // Disable APROM modify 
    clr_CHPCON_IAPEN;                    // Disable IAP
    
}

/*-----------------------------------------------------------------------------------------------*/
void main (void) 
{

  ALL_GPIO_QUASI_MODE;
/* If first time after reset, To modify P2.0 as GPIO input, not reset pin */
  if ((PCON&SET_BIT4)==SET_BIT4)        
      Modify_CONFIG(0xEB,0xFF,0xFF,0xFF,0xFF);

/*only P2.0 to low , enable WDT reset */
  while(P12);
  if ((PCON&SET_BIT4)==SET_BIT4)        
  {
      Enable_WDT_Reset_Config();
      PCON&=CLR_BIT4;    /*clr POF */
  }
  while(1);
}


