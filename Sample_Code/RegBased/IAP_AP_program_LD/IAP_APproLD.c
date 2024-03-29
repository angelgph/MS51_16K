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

//***********************************************************************************************************
//  File Function: MS51 IAP program LDROM demo code
//***********************************************************************************************************
#include "MS51.h"


/*---------------------------------------------------------------
// Following define by customer
// Please confirm the start addresss not over your code size
//---------------------------------------------------------------*/
#define     DATA_SIZE           1024   
#define     DATA_START_ADDR     0x0000           

/********************************************************************************************
 Following IAP command register is also define in SFR_Macro.h
 
  #define set_IAPEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON |= SET_BIT0 ;EA=BIT_TMP
  #define clr_IAPEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON &= ~SET_BIT0;EA=BIT_TMP
  #define set_APUEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPUEN |= SET_BIT0 ;EA=BIT_TMP
  #define clr_APUEN   BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPUEN &= ~SET_BIT0;EA=BIT_TMP
  
**********************************************************************************************/
void IAP_ERROR_LED(void)
{
  while (1)
  {
    P12 = 0;
    Timer0_Delay(16000000,200,1000);
    P12 = 1;
    Timer0_Delay(16000000,200,1000);
  }

}

//-----------------------------------------------------------------------------------------------------------/
void Trigger_IAP(void)
{   
    set_IAPTRG_IAPGO;                                  //trigger IAP
    if((CHPCON&SET_BIT6)==SET_BIT6)             // if fail flag is set, toggle error LED and IAP stop
    {
      clr_CHPCON_IAPFF;
      IAP_ERROR_LED();
    }
}
/*
    WARNING:
  No matter read or writer, when IAPFF is set 1, 
  this step process is fail. DATA should be ignore.
*/
//-----------------------------------------------------------------------------------------------------------/

/*****************************************************************************************************************
Erase APROM subroutine:
  

******************************************************************************************************************/    
void Erase_LDROM(void)
{   
    UINT16 u16Count;

    set_CHPCON_IAPEN;                          // Enable IAP function
    IAPFD = 0xFF;                        // IMPORTANT !! To erase function must setting IAPFD = 0xFF 
    IAPCN = PAGE_ERASE_LDROM;
    set_IAPUEN_LDUEN;                          //  APROM modify Enable
  
    for(u16Count=0x0000;u16Count<DATA_SIZE/PAGE_SIZE;u16Count++)    //
    {        
        IAPAL = LOBYTE(u16Count*PAGE_SIZE + DATA_START_ADDR);
        IAPAH = HIBYTE(u16Count*PAGE_SIZE + DATA_START_ADDR);
        Trigger_IAP(); 
    } 
    clr_IAPUEN_LDUEN;
    clr_CHPCON_IAPEN;
}
//-----------------------------------------------------------------------------------------------------------
void Erase_LDROM_Verify(void)
{   
    UINT16 u16Count;
    set_CHPCON_IAPEN;
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_READ_LDROM;

    for(u16Count=0;u16Count<DATA_SIZE;u16Count++)
    {   
        IAPFD = 0x00;    
        Trigger_IAP();
        if(IAPFD != 0xFF)
          IAP_ERROR_LED();
        IAPAL++;
        if(IAPAL == 0x00)
          IAPAH++;
    } 
    clr_CHPCON_IAPEN;
    
}
//-----------------------------------------------------------------------------------------------------------
void Program_LDROM(void)
{   
    UINT16 u16Count;

    set_CHPCON_IAPEN;
    set_IAPUEN_LDUEN;    
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_PROGRAM_LDROM;
  
    for(u16Count=0;u16Count<DATA_SIZE;u16Count++)
    {   
        IAPFD++;     
        Trigger_IAP();
       
        IAPAL++;
        if(IAPAL == 0)
        {
            IAPAH++;
        }
    } 
    
    clr_IAPUEN_LDUEN;
    clr_CHPCON_IAPEN;
}
//-----------------------------------------------------------------------------------------------------------
void Program_LDROM_Verify(void)
{   
    UINT16 u16Count;
    UINT8  u8Read_Data;

    set_CHPCON_IAPEN;
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_READ_LDROM;
    u8Read_Data = 0x00;

    for(u16Count=0;u16Count<DATA_SIZE;u16Count++)
    {   
        Trigger_IAP();
        if(IAPFD != u8Read_Data)
          IAP_ERROR_LED();
        IAPAL++;
        if(IAPAL == 0)
        {
            IAPAH++;
        }
        u8Read_Data ++;
    } 
    clr_CHPCON_IAPEN;
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
void main (void) 
{

  ALL_GPIO_QUASI_MODE;
  
    Erase_LDROM();
    Erase_LDROM_Verify();
    Program_LDROM();
    Program_LDROM_Verify();

    while(1);
}
//-----------------------------------------------------------------------------------------------------------
