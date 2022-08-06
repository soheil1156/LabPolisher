//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2009 Code Red Technologies Ltd. 
//
// UART example project for RDB1768 development board
//
// Software License Agreement
// 
// The software is owned by Code Red Technologies and/or its suppliers, and is 
// protected under applicable copyright laws.  All rights are reserved.  Any 
// use in violation of the foregoing restrictions may subject the user to criminal 
// sanctions under applicable laws, as well as to civil liability for the breach 
// of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD. 
//
//*****************************************************************************


#include <lpc17xx.h>
#include "uart.h"

// PCUART0
#define PCUART0_POWERON (1 << 3)
#define PCUART1_POWERON (1 << 4)
#define PCUART2_POWERON (1 << 24)
#define PCUART3_POWERON (1 << 25)

#define PCLK_UART0 6
#define PCLK_UART1 8
#define PCLK_UART2 16
#define PCLK_UART3 18
#define PCLK_UART0_MASK (3 << 6)
#define PCLK_UART1_MASK (3 << 8)
#define PCLK_UART2_MASK (3 << 16)
#define PCLK_UART3_MASK (3 << 18)

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80


// volatile uint32_t UART0Status, UART1Status;
// volatile uint8_t UART0TxEmpty = 1;
// volatile uint8_t UART1TxEmpty = 1;
// volatile uint8_t UART0Buffer[BUFSIZE], UART1Buffer[BUFSIZE];
// volatile uint32_t UART0Count = 0, UART1Count = 0;


/*****************************************************************************
** Function name:		UART0_IRQHandler
**
** Descriptions:		UART0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
// void UART0_IRQHandler (void) 
// {
//   uint8_t IIRValue, LSRValue;
//   uint8_t Dummy = Dummy;
// 	
//   IIRValue = LPC_UART0->IIR;
//     
//   IIRValue >>= 1;			/* skip pending bit in IIR */
//   IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
//   if ( IIRValue == IIR_RLS )		/* Receive Line Status */
//   {
// 	LSRValue = LPC_UART0->LSR;
// 	/* Receive Line Status */
// 	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
// 	{
// 	  /* There are errors or break interrupt */
// 	  /* Read LSR will clear the interrupt */
// 	  UART0Status = LSRValue;
// 	  Dummy = LPC_UART0->RBR;		/* Dummy read on RX to clear 
// 							interrupt, then bail out */
// 	  return;
// 	}
// 	if ( LSRValue & LSR_RDR )	/* Receive Data Ready */			
// 	{
// 	  /* If no error on RLS, normal ready, save into the data buffer. */
// 	  /* Note: read RBR will clear the interrupt */
// 	  UART0Buffer[UART0Count] = LPC_UART0->RBR;
// 	  UART0Count++;
// 	  if ( UART0Count == BUFSIZE )
// 	  {
// 		UART0Count = 0;		/* buffer overflow */
// 	  }	
// 	}
//   }
//   else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
//   {
// 	/* Receive Data Available */
// 	UART0Buffer[UART0Count] = LPC_UART0->RBR;
// 	UART0Count++;
// 	if ( UART0Count == BUFSIZE )
// 	{
// 	  UART0Count = 0;		/* buffer overflow */
// 	}
//   }
//   else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
//   {
// 	/* Character Time-out indicator */
// 	UART0Status |= 0x100;		/* Bit 9 as the CTI error */
//   }
//   else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
//   {
// 	/* THRE interrupt */
// 	LSRValue = LPC_UART0->LSR;		/* Check status in the LSR to see if
// 									valid data in U0THR or not */
// 	if ( LSRValue & LSR_THRE )
// 	{
// 	  UART0TxEmpty = 1;
// 	}
// 	else
// 	{
// 	  UART0TxEmpty = 0;
// 	}
//   }
//     
// }

// /*****************************************************************************
// ** Function name:		UART1_IRQHandler
// **
// ** Descriptions:		UART1 interrupt handler
// **
// ** parameters:			None
// ** Returned value:		None
// ** 
// *****************************************************************************/
// void UART1_IRQHandler (void) 
// {
//   uint8_t IIRValue, LSRValue;
//   uint8_t Dummy = Dummy;
// 	
//   IIRValue = LPC_UART1->IIR;
//     
//   IIRValue >>= 1;			/* skip pending bit in IIR */
//   IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
//   if ( IIRValue == IIR_RLS )		/* Receive Line Status */
//   {
// 	LSRValue = LPC_UART1->LSR;
// 	/* Receive Line Status */
// 	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
// 	{
// 	  /* There are errors or break interrupt */
// 	  /* Read LSR will clear the interrupt */
// 	  UART1Status = LSRValue;
// 	  Dummy = LPC_UART1->RBR;		/* Dummy read on RX to clear 
// 								interrupt, then bail out */
// 	  return;
// 	}
// 	if ( LSRValue & LSR_RDR )	/* Receive Data Ready */			
// 	{
// 	  /* If no error on RLS, normal ready, save into the data buffer. */
// 	  /* Note: read RBR will clear the interrupt */
// 	  UART1Buffer[UART1Count] = LPC_UART1->RBR;
// 	  UART1Count++;
// 	  if ( UART1Count == BUFSIZE )
// 	  {
// 		UART1Count = 0;		/* buffer overflow */
// 	  }	
// 	}
//   }
//   else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
//   {
// 	/* Receive Data Available */
// 	UART1Buffer[UART1Count] = LPC_UART1->RBR;
// 	UART1Count++;
// 	if ( UART1Count == BUFSIZE )
// 	{
// 	  UART1Count = 0;		/* buffer overflow */
// 	}3 v                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               vv
//   }
//   else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
//   {
// 	/* Character Time-out indicator */
// 	UART1Status |= 0x100;		/* Bit 9 as the CTI error */
//   }
//   else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
//   {
// 	/* THRE interrupt */
// 	LSRValue = LPC_UART1->LSR;		/* Check status in the LSR to see if
// 								valid data in U0THR or not */
// 	if ( LSRValue & LSR_THRE )
// 	{
// 	  UART1TxEmpty = 1;
// 	}
// 	else
// 	{
// 	  UART1TxEmpty = 0;
// 	}
//   }

// ***********************
// Function to set up UART
void UART0_Init(int baudrate)
{
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;	
	
	// Turn on power to UART0
	LPC_SC->PCONP |=  PCUART0_POWERON;
		
	// Turn on UART0 peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_UART0_MASK);
	LPC_SC->PCLKSEL0 |=  (0 << PCLK_UART0);		// PCLK_periph = CCLK/4
	
	// Set PINSEL0 so that P0.2 = TXD0, P0.3 = RXD0
	LPC_PINCON->PINSEL0 &= ~0xf0;
	LPC_PINCON->PINSEL0 |= ((1 << 4) | (1 << 6));
	
	LPC_UART0->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
    Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
    LPC_UART0->DLM = Fdiv / 256;							
    LPC_UART0->DLL = Fdiv % 256;
    LPC_UART0->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
    LPC_UART0->FCR = 0x07;		// Enable and reset TX and RX FIFONVIC_EnableIRQ(UART0_IRQn);
		

  
}

void UART1_Init(int baudrate)
{
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART0
	LPC_SC->PCONP |=  PCUART1_POWERON;

	// Turn on UART0 peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_UART1_MASK);
	LPC_SC->PCLKSEL0 |=  (0 << PCLK_UART1);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.2 = TXD0, P0.3 = RXD0
	LPC_PINCON->PINSEL4 &= ~0xF;
	LPC_PINCON->PINSEL4 |= (0xA << 0);
	//LPC_PINCON->PINSEL1 &= ~0xc;
	//LPC_PINCON->PINSEL1 |= (1 << 0);

	LPC_UART1->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
    Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
    LPC_UART1->FCR = 0x07;		// Enable and reset TX and RX FIFO
// 		NVIC_EnableIRQ(UART1_IRQn);

//     LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */
}

void UART2_Init(int baudrate)
{
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART2 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC->PCONP |=  PCUART0_POWERON;

	// Turn on UART2 peripheral clock
	LPC_SC->PCLKSEL1 &= ~(PCLK_UART2_MASK);
	LPC_SC->PCLKSEL1 |=  (0 << PCLK_UART2);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.10 = TXD2, P0.11 = RXD2
	LPC_PINCON->PINSEL0 &= ~0xf00000;
	LPC_PINCON->PINSEL0 |= ((1 << 20) | (1 << 22));

	LPC_UART2->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
    Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
    LPC_UART2->DLM = Fdiv / 256;
    LPC_UART2->DLL = Fdiv % 256;
    LPC_UART2->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
    LPC_UART2->FCR = 0x07;		// Enable and reset TX and RX FIFO
}

void UART3_Init(int baudrate)
{
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART2 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC->PCONP |=  PCUART3_POWERON;

	// Turn on UART2 peripheral clock
	LPC_SC->PCLKSEL1 &= ~(PCLK_UART3_MASK);
	LPC_SC->PCLKSEL1 |=  (0 << PCLK_UART3);		// PCLK_periph = CCLK/4

	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	//////////////////MODIFIED/////////////////////////
	// Set PINSEL0 so that P0.26 = TXD3, P0.25 = RXD3
	LPC_PINCON->PINSEL1 &= ~(0xf<<18);
	LPC_PINCON->PINSEL1 |= ((3 << 18) | (3 << 20));

	LPC_UART3->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
    Fdiv = ( pclk / 16 ) / baudrate ;	// Set baud rate
    LPC_UART3->DLM = Fdiv / 256;
    LPC_UART3->DLL = Fdiv % 256;
    LPC_UART3->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
    LPC_UART3->FCR = 0x07;		// Enable and reset TX and RX FIFO
}
// ***********************
// Function to send character over UART
void UART0_Sendchar(char c)
{
	while( (LPC_UART0->LSR & LSR_THRE) == 0 );	// Block until tx empty
	
	LPC_UART0->THR = c;
}

void UART1_Sendchar(char c)
{
	while( (LPC_UART1->LSR & LSR_THRE) == 0 );	// Block until tx empty

	LPC_UART1->THR = c;
}

void UART2_Sendchar(char c)
{
	while( (LPC_UART2->LSR & LSR_THRE) == 0 );	// Block until tx empty

	LPC_UART2->THR = c;
}

void UART3_Sendchar(char c)
{
	while( (LPC_UART3->LSR & LSR_THRE) == 0 );	// Block until tx empty

	LPC_UART3->THR = c;
}

// ***********************
// Function to get character from UART
char UART0_Getchar()
{
	char c;
	while( (LPC_UART0->LSR & LSR_RDR) == 0 );  // Nothing received so just block 	
	c = LPC_UART0->RBR; // Read Receiver buffer register
	return c;
}

char UART1_Getchar()
{
	char c;
	while( (LPC_UART1->LSR & LSR_RDR) == 0 );  // Nothing received so just block
	c = LPC_UART1->RBR; // Read Receiver buffer register
	return c;
}

char UART2_Getchar()
{
	char c;
	while( (LPC_UART2->LSR & LSR_RDR) == 0 );  // Nothing received so just block
	c = LPC_UART2->RBR; // Read Receiver buffer register
	return c;
}

char UART3_Getchar()
{
	char c;
	while( (LPC_UART3->LSR & LSR_RDR) == 0 );  // Nothing received so just block
	c = LPC_UART3->RBR; // Read Receiver buffer register
	return c;
}

// ***********************
// Function to prints the string out over the UART
void UART0_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART0_Sendchar(pcString[i]); // print each character
		i++;
	}
}

void UART1_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART1_Sendchar(pcString[i]); // print each character
		i++;
	}
}

void UART2_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART2_Sendchar(pcString[i]); // print each character
		i++;
	}
}

void UART3_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART3_Sendchar(pcString[i]); // print each character
		i++;
	}
}
  

