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
//  File Function: MS51 read UID demo
//***********************************************************************************************************
#include "MS51.h"

/**********************************/
/* *only read one byte of UID    */
/*********************************/
unsigned char xdata UIDBuffer[9];

unsigned char UID_BYTE(unsigned char Addr)
{
    unsigned char DATATEMP;
    set_CHPCON_IAPEN;
    IAPAL = Addr;
    IAPAH = 0x00;
    IAPCN = READ_UID;
    set_IAPTRG_IAPGO;
    DATATEMP = IAPFD;
    clr_CHPCON_IAPEN;
    return DATATEMP;
}

/*********************************************/
/* read whole CID and storage into XRAM     */
/*********************************************/ 
void All_UID_Read(void)
{   
    unsigned char u8Count;

    set_CHPCON_IAPEN;
    IAPAL = 0x00;
    IAPAH = 0x00;
    IAPCN = READ_UID;
    for(u8Count=0;u8Count<9;u8Count++)
    {   
        IAPFD = 0x00;
        set_IAPTRG_IAPGO;
        UIDBuffer[u8Count] = IAPFD ;
        IAPAL++;
    } 
    clr_CHPCON_IAPEN;
}


void main(void)
{
  unsigned char READ1;
  ALL_GPIO_QUASI_MODE;

/* Modify HIRC to 24MHz for UART printf function only */
    MODIFY_HIRC_24();
    UART_Open(24000000,UART0_Timer3,115200);
    ENABLE_UART0_PRINTF;

  All_UID_Read();
  printf ("\n UID = ");
  for(READ1=0;READ1<0X09;READ1++)
  {
    printf (" 0x%bx",UIDBuffer[READ1]);
  }
  
  while(1);


}