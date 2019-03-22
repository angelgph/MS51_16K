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
/* File Function: MS51 APROM program DATAFLASH as EEPROM way                                                */
/************************************************************************************************************/
#include "MS51.h"

#define ADDR_BASE 0x5700

#define FLASH_L_LOW      ADDR_BASE+1
#define FLASH_L_HIGH    ADDR_BASE+2
#define FLASH_H_LOW      ADDR_BASE+3
#define FLASH_H_HIGH    ADDR_BASE+4

volatile unsigned char xdata page_buffer[128];
UINT8 read_APROM_BYTE(UINT16 code *u16_addr);
/*****************************************************************************************************************
write_DATAFLASH_BYTE :
user can copy all this subroutine into project, then call this function in main.
******************************************************************************************************************/    
void write_DATAFLASH_BYTE(UINT16 u16_addr,UINT8 u8_data)
{
  UINT8 looptmp=0;
  UINT16 u16_addrl_r;

  UINT16 tmp=0;
//Check page start address
  u16_addrl_r=(u16_addr/128)*128;
//Save APROM data to XRAM0
  for(looptmp=0;looptmp<0x80;looptmp++)
  {
   tmp = read_APROM_BYTE((unsigned int code *)(u16_addrl_r+looptmp));
    page_buffer[looptmp]=tmp;
  }
// Modify customer data in XRAM
  page_buffer[u16_addr&0x7f] = u8_data;
  
//Erase APROM DATAFLASH page
    IAPAL = u16_addrl_r&0xff;
    IAPAH = (u16_addrl_r>>8)&0xff;
    IAPFD = 0xFF;
    set_CHPCON_IAPEN; 
    set_IAPUEN_APUEN;
    IAPCN = 0x22;     
     set_IAPTRG_IAPGO; 
    
//Save changed RAM data to APROM DATAFLASH
    
    set_CHPCON_IAPEN; 
    set_IAPUEN_APUEN;
    IAPCN = 0x21;
    for(looptmp=0;looptmp<0x80;looptmp++)
    {
      IAPAL = (u16_addrl_r&0xff)+looptmp;
      IAPAH = (u16_addrl_r>>8)&0xff;
      IAPFD = page_buffer[looptmp];
      set_IAPTRG_IAPGO;      
    }
    clr_IAPUEN_APUEN;
    clr_CHPCON_IAPEN;
}  
  
//-------------------------------------------------------------------------
UINT8 read_APROM_BYTE(UINT16 code *u16_addr)
{
  UINT8 rdata;
  rdata = *u16_addr>>8;
  return rdata;
}

/******************************************************************************************************************/  


void main (void) 
{
    UINT8 datatemp;
    UINT16 system16highsite;
  
    P12_QUASI_MODE;
    while(P12);
/* -------------------------------------------------------------------------*/
/*  Dataflash use APROM area, please ALWAYS care the address of you code    */
/*  APROM 0x3800~0x38FF demo as dataflash                                   */
/*   Please use Memory window key in C:0x3800 to check earse result         */
/* -------------------------------------------------------------------------*/
/*call write byte   */
    write_DATAFLASH_BYTE (0x3802,0x66);  
    write_DATAFLASH_BYTE (0x3802,0x65);  
    write_DATAFLASH_BYTE (0x3802,0x64);  
    write_DATAFLASH_BYTE (0x3802,0x63);  
    write_DATAFLASH_BYTE (0x3882,0x61);
    write_DATAFLASH_BYTE (FLASH_L_LOW,0x55);
    write_DATAFLASH_BYTE (FLASH_L_HIGH,0x77);
    write_DATAFLASH_BYTE (FLASH_H_LOW,0x88);
    write_DATAFLASH_BYTE (FLASH_H_HIGH,0x99);
/*call read byte   */
    datatemp = read_APROM_BYTE(0x3802);
    system16highsite = ((read_APROM_BYTE(FLASH_H_HIGH)<<8)+read_APROM_BYTE(FLASH_H_LOW));

    while(1);

}

