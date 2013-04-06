
#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

/*
 * Use Tom Lafleur's library instead of the FreeRTOS demo
 * 
 * Use this library unless you want to take the time to fix the entire
 * source code. The original implementation is only for reference.
 */
#define serialALTERNATE_IMPLEMENTATION

#ifdef serialALTERNATE_IMPLEMENTATION


/*
*	TITLE		Queued serial drivers for FreeRTOS using dsPIC33 and PIC24
*
*	VERSION:	0.2a (Beta)
*
*	DATE:		08-Oct-2011
*
*	AUTHOR:		Tom Lafleur
*				lafleur@lafleur.us
*
*	COMMENTS:	You are free to use this source code for any non-commercial
*				use. Please do not make copies of this source code, modified
*				or un-modified, publicly available on the internet or
*				elsewhere without permission. Thanks.
*
*				Copyright ©2005-2011 R.  . All rights are reserved.
*
********************************************************************************
*
*	CHANGE LOG:
*
*	DATE         REV  DESCRIPTION
*	-----------  ---  ----------------------------------------------------------
*	12-Jun-2009  0.1	TRL - Original code for N2K60 under FreeRTOS
*	01-Oct-2011	 0.2a	TRL - Added serial port 2, write function, error counters
*				 0.3	TRL -
*
*
*	Notes:		1)	Tested with FreeRTOS ver 7.0.1 PIC24/33, N2k60 board
*				2)  Support for two serial ports, can easily be expanded for more ports
*				3)  Added Rx error counters to the data structures
*				4)	If using processors with Peripheral pin select, you must define
*					 pin's prior to using this driver
*				5)  Based on FreeRTOS demo code and my 2007 robotics serial driver
*
*	ToDo:		1)  Check if code is atomic and thread safe
*				2)  Make Rx error counters avilable global
*				3)  Make Tx use DMA if avilable, if so we will use ring buffers
*					 and give a full buffer to DMA system. Queue's are too slow.
*				4)
*
* 	My Notes:
*
*
*
*/

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

typedef enum
{
	serCOM1,
	serCOM2,
	serCOM3,
	serCOM4,
	serCOM5,
	serCOM6,
	serCOM7,
	serCOM8
} eCOMPort;

typedef enum
{
	serNO_PARITY,
	serEVEN_PARITY,
	serODD_PARITY
} eParity;

typedef enum
{
	serSTOP_1,
	serSTOP_2
} eStopBits;

typedef enum
{
	serBITS_8,
	serBITS_9
} eDataBits;


typedef struct xCOM_PORT
{
    eCOMPort sPort;                         // Comm port address 0, 1, not needed, but it here
    xQueueHandle xRxedChars;                // Read buffer
    xQueueHandle xCharsForTx;               // Write buffer
    volatile unsigned char ucPartyErr;      // Party error counter
    volatile unsigned char ucFramingErr;    // Framing error counter
    volatile unsigned char ucOverrunErr;    // Overrun error counter
} volatile xComPort;

typedef void * xComPortHandle;
//typedef xComPort * xComPortHandle;

/*-----------------------------------------------------------*/
xComPortHandle xSerialPortInit( eCOMPort ePort, unsigned long eWantedBaud, eParity eWantedParity, eDataBits eWantedDataBits, eStopBits eWantedStopBits, unsigned portBASE_TYPE uxTxBufferLength, unsigned portBASE_TYPE uxRxBufferLength );
void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength );
signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime );
signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime );
void vSerialClose( xComPortHandle xPort );

// These are NOT RTOS safe, use with care!

/* These functions are NOT RTOS safe, use with care */
void uart1_puts(char *str);
void uart1_putc(char c);
void uart1_put(char *str, int len);
void uart2_puts(char *str);
void uart2_putc(char c);
void uart2_put(char *str, int len);


#else

/*
    FreeRTOS V7.2.0 - Copyright (C) 2012 Real Time Engineers Ltd.


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?                                      *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************


    http://www.FreeRTOS.org - Documentation, training, latest information,
    license and contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool.

    Real Time Engineers ltd license FreeRTOS to High Integrity Systems, who sell
    the code with commercial support, indemnification, and middleware, under
    the OpenRTOS brand: http://www.OpenRTOS.com.  High Integrity Systems also
    provide a safety engineered and independently SIL3 certified version under
    the SafeRTOS brand: http://www.SafeRTOS.com.
*/

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

typedef void * xComPortHandle;

typedef enum
{ 
	serCOM1, 
	serCOM2, 
	serCOM3, 
	serCOM4, 
	serCOM5, 
	serCOM6, 
	serCOM7, 
	serCOM8 
} eCOMPort;

typedef enum 
{ 
	serNO_PARITY, 
	serODD_PARITY, 
	serEVEN_PARITY, 
	serMARK_PARITY, 
	serSPACE_PARITY 
} eParity;

typedef enum 
{ 
	serSTOP_1, 
	serSTOP_2 
} eStopBits;

typedef enum 
{ 
	serBITS_5, 
	serBITS_6, 
	serBITS_7, 
	serBITS_8 
} eDataBits;

typedef enum 
{ 
	ser50,		
	ser75,		
	ser110,		
	ser134,		
	ser150,    
	ser200,
	ser300,		
	ser600,		
	ser1200,	
	ser1800,	
	ser2400,   
	ser4800,
	ser9600,		
	ser19200,	
	ser38400,	
	ser57600,	
	ser115200
} eBaud;

void vSetupUart1( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength );
void vSetupUart2( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength );
xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength );
xComPortHandle xSerialPortInit( eCOMPort ePort, eBaud eWantedBaud, eParity eWantedParity, eDataBits eWantedDataBits, eStopBits eWantedStopBits, unsigned portBASE_TYPE uxBufferLength );
void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength );
signed portBASE_TYPE xUart1GetChar( xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime );
signed portBASE_TYPE xUart1PutChar( xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime );
signed portBASE_TYPE xUart2GetChar( xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime );
signed portBASE_TYPE xUart2PutChar( xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime );
portBASE_TYPE xSerialWaitForSemaphore( xComPortHandle xPort );
void vSerialClose( xComPortHandle xPort );


#endif

#endif /* SERIAL_H */

