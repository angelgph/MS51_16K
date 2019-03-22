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

/***********************************************************************************************************
//  File Function: MS51 UART0 receive and transmit loop test
/***********************************************************************************************************/
#include "MS51.h"

unsigned char uart_receive_data;
bit receiveFlag,bufOverFlag;

/************************************************************************************************************/
/*  UART interrupt subroutine                                                                               */
/************************************************************************************************************/
void Serial_ISR (void) interrupt 4 
{
    if (RI)
    {   
      receiveFlag = 1;
      uart_receive_data = SBUF;
      clr_SCON_RI;                                         // Clear RI (Receive Interrupt).
    }
    if (TI)
    {       
        clr_SCON_TI;                                       // Clear TI (Transmit Interrupt).
    }

}

/************************************************************************************************************/
/*  Main function                                                                                           */
/************************************************************************************************************/
 void main(void)
{
  P12_PUSHPULL_MODE;
/* Modify HIRC to 24MHz for UART printf function only */
    MODIFY_HIRC_24();
    UART_Open(24000000,UART0_Timer3,115200);
    set_IE_ES;                                  /* Enable UART0 interrupt */
    set_IE_EA;                                  /* Global interrupt enable */
  
/* while receive data from RXD, send this data to TXD */
  while(1)
  {
    if (receiveFlag)
    {
      receiveFlag = 0;
      clr_IE_ES;
      UART_Send_Data(UART0,uart_receive_data);
      set_IE_ES;
    }
  }
}