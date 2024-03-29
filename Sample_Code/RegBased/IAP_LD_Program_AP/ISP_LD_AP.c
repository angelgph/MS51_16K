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
//  File Function: MS51 LDROM program APROM demo code
//***********************************************************************************************************
#include "MS51.h"

//***********************************************************************************************************
//  Before using this demo code, please remind below CONFOGs setting, thanks.
//                 CONFIG0.7 --> CBS=0
//                 CONFIG1 = 0xFC, because this demo code size is 2495 Byte
//***********************************************************************************************************

#define     CID_READ            0x0B
#define     DID_READ            0x0C
#define     CFG_READ            0xC0
#define     CFG_ERASE           0xE2

#define     PAGE_ERASE_AP       0x22
#define     BYTE_READ_AP        0x00
#define     BYTE_PROGRAM_AP     0x21

#define     BYTE_PROGRAM_CFG    0xE1
#define     BYTE_READ_CFG       0xC0

#define     PAGE_SIZE           128

#define     APROM_SIZE          8192   
#define     APROM_START_ADDR    0x0000

#define     ERASE_FAIL          0x70
#define     PROGRAM_FAIL        0x71
#define     IAPFF_FAIL          0x72
#define     IAP_PASS            0x00

/*-----------------------------------------------------------------------------------------------------------*/
void Trigger_IAP(void)
{   
    set_IAPGO;                                  //trigger IAP
    if((CHPCON&SET_BIT6)==SET_BIT6)             //check IAPFF (CHPCON.6), if flag = 1, the ISP process is fail.
    {
        while(1)
        {
          P12 = 0;
          Timer0_Delay(16000000,200,1000);
          P12 = 1;
          Timer0_Delay(16000000,200,1000);
        }
    }
}                  

/*-----------------------------------------------------------------------------------------------------------*/
void Erase_CONFIG(void)
{   
    set_IAPEN;
    
    IAPAL = 0x00;
    IAPAH = 0x00;
    IAPFD = 0xFF;
    IAPCN = CFG_ERASE;
    
    set_CFUEN;
    Trigger_IAP();
    clr_CFUEN;
    
    clr_IAPEN;
}
/*-----------------------------------------------------------------------------------------------------------*/
void Erase_APROM(void)
{   
    UINT16 u16Count;

    set_IAPEN;
    set_APUEN;                        // To erase or program APROM must enable APUEN
  
    IAPFD = 0xFF;                      // MUST setting IAPFD = 0xFF when do erase process.
    IAPCN = PAGE_ERASE_AP;

    for(u16Count=0x0000;u16Count<APROM_SIZE/PAGE_SIZE;u16Count++)
    {        
        IAPAL = LOBYTE(u16Count*PAGE_SIZE + APROM_START_ADDR);
        IAPAH = HIBYTE(u16Count*PAGE_SIZE + APROM_START_ADDR);
        Trigger_IAP(); 
    } 
    
    clr_APUEN;
    clr_IAPEN;
}
/*-----------------------------------------------------------------------------------------------------------*/
BIT Erase_APROM_Verify(void)
{   
    UINT16 u16Count;
    BIT    Error_Flag = 0;

    set_IAPEN;
    
    IAPAL = LOBYTE(APROM_START_ADDR);
    IAPAH = HIBYTE(APROM_START_ADDR);
    IAPCN = BYTE_READ_AP;

    for(u16Count=0;u16Count<APROM_SIZE/PAGE_SIZE;u16Count++)
    {   
        IAPFD = 0x00;    
        Trigger_IAP();

        if(IAPFD != 0xFF)
        {
            Error_Flag = 1;
            break;
        }

        IAPAL++;
        if(IAPAL == 0x00)
        {
            IAPAH++;
        }
    } 
    
    clr_IAPEN;
    
    if(Error_Flag == 1)
        return FAIL;
    else
        return PASS;
}
/*-----------------------------------------------------------------------------------------------------------*/
void Program_APROM(void)
{   
    UINT16 u16Count;

    set_IAPEN;
    set_APUEN;   
  
    IAPAL = LOBYTE(APROM_START_ADDR);
    IAPAH = HIBYTE(APROM_START_ADDR);
    IAPFD = 0xFF;
    IAPCN = BYTE_PROGRAM_AP;

    for(u16Count=0;u16Count<APROM_SIZE;u16Count++)
    {   
        IAPFD++;     
        Trigger_IAP();
       
        IAPAL++;
        if(IAPAL == 0)
        {
            IAPAH++;
        }
    } 
    
    clr_APUEN;
    clr_IAPEN;
}
/*-----------------------------------------------------------------------------------------------------------*/
BIT Program_APROM_Verify(void)
{   
    UINT16 u16Count;
    UINT8  u8Read_Data;
    BIT    Error_Flag = 0;

    set_IAPEN;
    
    IAPAL = LOBYTE(APROM_START_ADDR);
    IAPAH = HIBYTE(APROM_START_ADDR);
    IAPCN = BYTE_READ_AP;

    u8Read_Data = 0x00;

    for(u16Count=0;u16Count<APROM_SIZE;u16Count++)
    {   
        Trigger_IAP();
        if(IAPFD != u8Read_Data)
        {
            Error_Flag = 1;
            break;
        }

        IAPAL++;
        if(IAPAL == 0)
        {
            IAPAH++;
        }
        u8Read_Data ++;
    } 

    clr_IAPEN;
     
    if(Error_Flag == 1)
        return FAIL;
    else
        return PASS;
}
/*-----------------------------------------------------------------------------------------------------------*/
void Program_CONFIG(UINT8 u8Address,UINT8 u8Data)
{   
    set_IAPEN;
    
    IAPAL = u8Address;
    IAPAH = 0x00;
    IAPFD = u8Data;
    IAPCN = BYTE_PROGRAM_CFG;
    
    set_CFUEN;
    Trigger_IAP();
    clr_CFUEN;
    
    clr_IAPEN;
}

/*-----------------------------------------------------------------------------------------------------------*/
void main (void) 
{
    ALL_GPIO_QUASI_MODE;

    Erase_APROM();
    Program_APROM();

    Timer0_Delay(16000000,200,1000);
    while(1);
}

