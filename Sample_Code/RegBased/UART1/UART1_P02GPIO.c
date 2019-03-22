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
/*  File Function: MS51 UART1 with interrupt demo                                                           */
/************************************************************************************************************/
#include "MS51.h"

unsigned char  UART_BUFFER;
bit riflag;


void SerialPort1_ISR(void) interrupt 15 
{
    if (RI_1==1) 
    {                                       /* if reception occur */
        clr_SCON_1_RI_1;                             /* clear reception flag for next reception */
        UART_BUFFER = SBUF_1;
        riflag =1;
    }
    if(TI_1==1)
    {
        clr_SCON_1_TI_1;                             /* if emission occur */
    }
}


/****************************************************************************************************************
 * FUNCTION_PURPOSE: Main function 
  
 !!! N76E003 UART1 pin also occupied by debug pin, 
 please remove Nu-link or not in debug mode to test UART1 function.

 External UART1 connect also disturb debug download

 ***************************************************************************************************************/
void main (void)
{
    ALL_GPIO_QUASI_MODE;
    P12_PUSHPULL_MODE;    // For I/O toggle display
/* Modify HIRC to 24MHz for UART baud rate deviation not over 1%*/
    MODIFY_HIRC_24();
    UART_Open(24000000,UART1_Timer3,115200);
    ENABLE_UART1_INTERRUPT;                 //For interrupt enable
    ENABLE_GLOBAL_INTERRUPT;                //ENABLE_GLOBAL_INTERRUPT

    while(1)
    {
      if (riflag)
      {
          P12 = ~ P12;      //Receive each byte P12 toggle, never work under debug mode
          DISABLE_UART1_INTERRUPT;
          UART_Send_Data(UART1,UART_BUFFER);
          riflag = 0;
          ENABLE_UART1_INTERRUPT;
      }
    }
}
  
