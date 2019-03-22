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
//  File Function: MS51 IAP erase / program / read verify demo code
//***********************************************************************************************************
#include "MS51.h"

//---------------------------------------------------------------
// Following define by customer
// Please confirm the start addresss not over your code size
//---------------------------------------------------------------
#define     DATA_SIZE           4096   
#define     DATA_START_ADDR     0x3800           


void IAP_ERROR_LED(void)
{
  while (1)
  {
    P12 = 0;
    Timer0_Delay(16000000,100,1000);
    P12 = 1;
    Timer0_Delay(16000000,100,1000);
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
void Erase_APROM(void)
{   
    UINT16 u16Count;

    set_CHPCON_IAPEN;                          // Enable IAP function
    IAPFD = 0xFF;                        // IMPORTANT !! To erase function must setting IAPFD = 0xFF 
    IAPCN = PAGE_ERASE_APROM;
    set_IAPUEN_APUEN;                           //  APROM modify Enable
  
    for(u16Count=0x0000;u16Count<DATA_SIZE/PAGE_SIZE;u16Count++)    //
    {        
        IAPAL = LOBYTE(u16Count*PAGE_SIZE + DATA_START_ADDR);
        IAPAH = HIBYTE(u16Count*PAGE_SIZE + DATA_START_ADDR);
        Trigger_IAP(); 
    } 
    clr_IAPUEN_APUEN;
    clr_CHPCON_IAPEN;
}
//-----------------------------------------------------------------------------------------------------------
void Erase_APROM_Verify(void)
{   
    UINT16 u16Count;
    set_CHPCON_IAPEN;
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_READ_APROM;

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
void Program_APROM(void)
{   
    UINT16 u16Count;

    set_CHPCON_IAPEN;
    set_IAPUEN_APUEN;    
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_PROGRAM_APROM;
    
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
    
    clr_IAPUEN_APUEN;
    clr_CHPCON_IAPEN;
}
//-----------------------------------------------------------------------------------------------------------
void Program_APROM_Verify(void)
{   
    UINT16 u16Count;
    UINT8  u8Read_Data;

    set_CHPCON_IAPEN;
    IAPAL = LOBYTE(DATA_START_ADDR);
    IAPAH = HIBYTE(DATA_START_ADDR);
    IAPCN = BYTE_READ_APROM;
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

    Erase_APROM();
    Erase_APROM_Verify();
    Program_APROM();
    Program_APROM_Verify();

    while(1);
}
//-----------------------------------------------------------------------------------------------------------
