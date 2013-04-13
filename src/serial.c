
#include "serial.h"


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
*	08-Oct-2011	 0.2a	TRL - Added serial port 2, error counters
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

/* Standard include file. */
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* Serial include files. */
#include "serial.h"

#define serMAX_PORTS	((unsigned short) 2 )	// max number of serial ports we are supporting

/*-----------------------------------------------------------*/
/*  static configuration of serial port from these defines */

#define serINTERRUPT_ENABLE     1
#define serINTERRUPT_DISABLE    0
#define serCLEAR_FLAG           0
#define serSET_FLAG             1

#define serNO_BLOCK             ( ( portTickType ) 0 )

//#define COMQUEUE						// add queues to registry for debug
/*-----------------------------------------------------------*/
//typedef enum
//{
//	serCOM1, 		// only port 1 and 2 defined at this time
//	serCOM2,
//	serCOM3,
//	serCOM4,
//	serCOM5,
//	serCOM6,
//	serCOM7,
//	serCOM8
//} eCOMPort;
//
//typedef enum
//{
//	serNO_PARITY,
//	serEVEN_PARITY,
//	serODD_PARITY
//} eParity;
//
//typedef enum
//{
//	serSTOP_1,
//	serSTOP_2
//} eStopBits;
//
//typedef enum
//{
//	serBITS_8, 		// only 8 or 9 bits supported by dsPIC
//	serBITS_9
//} eDataBits;


/*-----------------------------------------------------------*/

/* The queues used to communicate between tasks and ISRs. */

static xQueueHandle xRxedChars  __attribute__ ((unused)); 	// prevents warnings
static xQueueHandle xCharsForTx	__attribute__ ((unused)); 	// prevents warnings

static portBASE_TYPE xTxHasEnded;

/*-----------------------------------------------------------*/ 


// Port structure
static	xComPort xPorts[serMAX_PORTS] =
{
	{0, NULL, NULL, 0,0,0 },	// Serial Port 1, xRxedChars handle, xCharsForTx handle, Party, Framing, Overrun
	{1, NULL, NULL, 0,0,0 }		// Serial Port 2, xRxedChars handle, xCharsForTx handle, Party, Framing, Overrun
};

//typedef xComPort * xComPortHandle;

/*-----------------------------------------------------------*/
/* Prototype for public functions defined here */
//xComPortHandle xSerialPortInit( eCOMPort ePort, unsigned long eWantedBaud, eParity eWantedParity, eDataBits eWantedDataBits, eStopBits eWantedStopBits, unsigned portBASE_TYPE uxTxBufferLength, unsigned portBASE_TYPE uxRxBufferLength );
//void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength );
//signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime );
//signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime );
//void vSerialClose( xComPortHandle xPort );

/* These functions are NOT RTOS safe, use with care */
//void uart1_puts(char *str);
//void uart1_putc(char c);
//void uart1_put(char *str, int len);
//
//void uart2_puts(char *str);
//void uart2_putc(char c);
//void uart2_put(char *str, int len);

/* Prototype for private functions defined here */
void prvSerialPort1Init( unsigned long eWantedBaud, eParity eWantedParity, eDataBits eWantedDataBits, eStopBits eWantedStopBits);
void prvSerialPort2Init( unsigned long eWantedBaud, eParity eWantedParity, eDataBits eWantedDataBits, eStopBits eWantedStopBits);


/*-----------------------------------------------------------*/
xComPortHandle xSerialPortInit( eCOMPort ePort, unsigned long eWantedBaud, eParity eWantedParity, eDataBits eWantedDataBits, eStopBits eWantedStopBits, unsigned portBASE_TYPE uxTxBufferLength, unsigned portBASE_TYPE uxRxBufferLength )
{
unsigned short usPort;
xComPortHandle pxPort = NULL;

	portENTER_CRITICAL();		// for now we will make sure we have control
	{
	usPort = (unsigned short) ePort;
	if (usPort < serMAX_PORTS)
		{
		// lets select the correct serial port
		switch (usPort)
			{
			case 0:
				prvSerialPort1Init( eWantedBaud, eWantedParity, eWantedDataBits, eWantedStopBits);
				break;
			case 1:
				prvSerialPort2Init( eWantedBaud, eWantedParity, eWantedDataBits, eWantedStopBits);
				break;
			}

		pxPort = (xComPortHandle) (&(xPorts[usPort]));

		// lets build the queue's for this device
    	((xComPort*)pxPort)->xRxedChars  = xQueueCreate(uxRxBufferLength, (unsigned portBASE_TYPE) sizeof(signed portCHAR));
    	((xComPort*)pxPort)->xCharsForTx = xQueueCreate(uxTxBufferLength, (unsigned portBASE_TYPE) sizeof(signed portCHAR));

#if defined (COMQUEUE)		// used only for debug
		if (usPort == 0)
			{
			vQueueAddToRegistry (pxPort->xRxedChars,  "Rx1 Queue" );
			vQueueAddToRegistry (pxPort->xCharsForTx, "Tx1 Queue" );
			}
		else
			{
			vQueueAddToRegistry (pxPort->xRxedChars,  "Rx2 Queue" );
			vQueueAddToRegistry (pxPort->xCharsForTx, "Tx2 Queue" );
			}
#endif

		}
	}
	portEXIT_CRITICAL();
return pxPort;
}

void prvSerialPort1Init( unsigned long ulWantedBaud, eParity Parity, eDataBits DataBits, eStopBits StopBits)
{

char cChar;

if ( DataBits == serBITS_9) DataBits = 0x03;	// 9-bit data, no parity

// Setup UART 1
// USART Mode register

U1MODEbits.UARTEN = 0;		//UART1 Enable bit	// We need to set this after all the bits are set
												// 1 = UART1 is enabled; all UART1 pins are controlled by UARTx as
												//	defined by UEN<1:0>
												// 0 = UART1 is disabled; all UART1 pins are controlled by PORT latches;
												//	UART1 power consumption minimal
U1MODEbits.USIDL = 0; 		//Stop in Idle Mode bit
												// 1 = Discontinue module operation when device enters Idle mode
												// 0 = Continue module operation in Idle mode
U1MODEbits.IREN = 0;		//IrDA Encoder and Decoder Enable bit(1)
												// 1 = IrDA encoder and decoder enabled
												// 0 = IrDA encoder and decoder disabled
U1MODEbits.RTSMD = 1;		//Mode Selection for UxRTS Pin bit
												// 1 = UxRTS pin in Simplex mode
												// 0 = UxRTS pin in Flow Control mode
U1MODEbits.UEN1 = 0;		//UARTx Enable bits
U1MODEbits.UEN0 = 0;							// 11 = UxTX, UxRX and BCLKx pins are enabled and used; UxCTS pin controlled by PORT latches
												// 10 = UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
												// 01 = UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin controlled by PORT latches
												// 00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx pins controlled by PORT latches
U1MODEbits.WAKE = 0;		//Wake-up on Start bit Detect During Sleep Mode Enable bit
												// 1 = UARTx will continue to sample the UxRX pin; interrupt generated on falling edge, bit cleared in
												//	hardware on following rising edge
												// 0 = No wake-up enabled
U1MODEbits.LPBACK = 0;		//UARTx Loopback Mode Select bit
												// 1 = Enable Loopback mode
												// 0 = Loopback mode is disabled
U1MODEbits.ABAUD = 0;		//Auto-Baud Enable bit
												// 1 = Enable baud rate measurement on the next character ? requires reception
												//	of a Sync field (55h); cleared in hardware upon completion
												// 0 = Baud rate measurement disabled or completed
//U1MODEbits.URXINV = 0;		//Receive Polarity Inversion bit
												// 1 = UxRX Idle state is ?0?
												// 0 = UxRX Idle state is ?1?
U1MODEbits.BRGH = 0;		//High Baud Rate Enable bit
												// 1 = BRG generates 4 clocks per bit period (4x Baud Clock, High-Speed mode)
												// 0 = BRG generates 16 clocks per bit period (16x Baud Clock, Standard mode)
							//Parity and Data Selection bits
U1MODEbits.PDSEL = ((Parity || DataBits) & 0x03);
//U1MODEbits.PDSEL1 = 0;
//U1MODEbits.PDSEL0 = 0;						// 11 = 9-bit data, no parity
												// 10 = 8-bit data, odd parity
												// 01 = 8-bit data, even parity
												// 00 = 8-bit data, no parity
U1MODEbits.STSEL = StopBits;//Stop Bit Selection bit
												// 1 = Two Stop bits
												// 0 = One Stop bit

// USART Status and Control register

U1STAbits.UTXISEL1 = 1;		//Transmission Interrupt Mode Selection bits
U1STAbits.UTXISEL0 = 0;							// 11 = Reserved; do not use
												// 10 = Interrupt when a character is transferred to the Transmit Shift
												//	Register and as a result, the transmit buffer becomes empty
												// 01 = Interrupt when the last character is shifted out of the Transmit
												//	Shift Register; all transmit operations are completed
												// 00 = Interrupt when a character is transferred to the Transmit Shift Register (this implies there is at least
												//	one character open in the transmit buffer)
U1STAbits.UTXINV	= 0;	//Transmit Polarity Inversion bit
												// If IREN = 0:
												// 	1 = TX Idle state is ?0?
												// 	0 = TX Idle state is ?1? Normal
												// If IREN = 1:
												// 	1 = IrDA encoded TX Idle state is ?1?
												// 	0 = IrDA encoded TX Idle state is ?0?
U1STAbits.UTXBRK = 0;		//Transmit Break bit
												// 1 = Send Sync Break on next transmission ? Start bit, followed by twelve ?0? bits, followed by Stop bit;
												//	cleared by hardware upon completion
												// 0 = Sync Break transmission disabled or completed
U1STAbits.UTXEN = 0;		//Transmit Enable bit
												// 1 = Transmit enabled, UxTX pin controlled by UARTx
												// 0 = Transmit disabled, any pending transmission is aborted and buffer is reset. UxTX pin controlled by PORT.
U1STAbits.UTXBF;			//Transmit Buffer Full Status bit (read-only)
												// 1 = Transmit buffer is full
												// 0 = Transmit buffer is not full, at least one more character can be written
U1STAbits.TRMT;				//Transmit Shift Register Empty bit (read-only)
												// 1 = Transmit Shift Register is empty and transmit buffer is empty (the last transmission has completed)
												// 0 = Transmit Shift Register is not empty, a transmission is in progress or queued
U1STAbits.URXISEL1 = 0;		//Receive Interrupt Mode Selection bits
U1STAbits.URXISEL0 = 0;							// 11 = Interrupt is set on RSR transfer, making the receive buffer full (i.e., has 4 data characters)
												// 10 = Interrupt is set on RSR transfer, making the receive buffer 3/4 full (i.e., has 3 data characters)
												// 0x = Interrupt is set when any character is received and transferred from the RSR to the receive buffer;
												//	receive buffer has one or more characters
U1STAbits.ADDEN = 0;		//Address Character Detect bit (bit 8 of received data = 1)
												// 1 = Address Detect mode enabled. If 9-bit mode is not selected, this does not take effect.
												// 0 = Address Detect mode disabled
U1STAbits.RIDLE;			//Receiver Idle bit (read-only)
												// 1 = Receiver is Idle
												// 0 = Receiver is active
U1STAbits.PERR;				//Parity Error Status bit (read-only)
												// 1 = Parity error has been detected for the current character (character at the top of the receive FIFO)
												// 0 = Parity error has not been detected
U1STAbits.FERR;				//Framing Error Status bit (read-only)
												// 1 = Framing error has been detected for the current character (character at the top of the receive FIFO)
												// 0 = Framing error has not been detected
U1STAbits.OERR;				//Receive Buffer Overrun Error Status bit (clear/read-only)
												// 1 = Receive buffer has overflowed
												// 0 = Receive buffer has not overflowed (clearing a previously set OERR bit (1 ? 0 transition) will reset
												//	the receiver buffer and the RSR to the empty state)
U1STAbits.URXDA;			//Receive Buffer Data Available bit (read-only)
												// 1 = Receive buffer has data, at least one more character can be read
												// 0 = Receive buffer is empty
	U1BRG = (unsigned short)(( (float)configCPU_CLOCK_HZ / ( (float)16 * (float)ulWantedBaud ) ) - (float)0.5);	// BRGH = 0
//	U1BRG = (unsigned short)(( (float)configCPU_CLOCK_HZ / ( (float)4 * (float)ulWantedBaud ) ) - (float)0.5);	// BRGH = 1


	/* It is assumed that this function is called prior to the scheduler being
	started.  Therefore interrupts must not be allowed to occur yet as they
	may attempt to perform a context switch.
	*/

	portDISABLE_INTERRUPTS();

	U1MODEbits.UARTEN = 1;		// Enable the UART
	U1STAbits.UTXEN = 1;		// Transmit Enable

	IFS0bits.U1RXIF = serCLEAR_FLAG;
	IFS0bits.U1TXIF = serCLEAR_FLAG;
	IPC2bits.U1RXIP = configKERNEL_INTERRUPT_PRIORITY;
	IPC3bits.U1TXIP = configKERNEL_INTERRUPT_PRIORITY;
	IEC0bits.U1TXIE = serINTERRUPT_ENABLE;
	IEC0bits.U1RXIE = serINTERRUPT_ENABLE;

	/* Clear the Rx buffer. */
	while( U1STAbits.URXDA == serSET_FLAG )
	{
		cChar = U1RXREG;
	}

	xTxHasEnded = pdTRUE;
}

/*-----------------------------------------------------------*/
void prvSerialPort2Init( unsigned long ulWantedBaud, eParity Parity, eDataBits DataBits, eStopBits StopBits)
{

char cChar;

if ( DataBits == serBITS_9) DataBits = 0x03;	// 9-bit data, no parity

// Setup UART 2
// USART 2 Mode register

U2MODEbits.UARTEN = 0;		//UARTx Enable bit  // We need to set this after all the other bits are set
												// 1 = UARTx is enabled; all UARTx pins are controlled by UARTx as
												//	defined by UEN<1:0>
												// 0 = UARTx is disabled; all UARTx pins are controlled by PORT latches;
												//	UARTx power consumption minimal
U2MODEbits.USIDL = 0; 		//Stop in Idle Mode bit
												// 1 = Discontinue module operation when device enters Idle mode
												// 0 = Continue module operation in Idle mode
U2MODEbits.IREN = 0;		//IrDA Encoder and Decoder Enable bit(1)
												// 1 = IrDA encoder and decoder enabled
												// 0 = IrDA encoder and decoder disabled
U2MODEbits.RTSMD = 1;		//Mode Selection for UxRTS Pin bit
												// 1 = UxRTS pin in Simplex mode
												// 0 = UxRTS pin in Flow Control mode
U2MODEbits.UEN1 = 0;		//UARTx Enable bits
U2MODEbits.UEN0 = 0;							// 11 = UxTX, UxRX and BCLKx pins are enabled and used; UxCTS pin controlled by PORT latches
												// 10 = UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
												// 01 = UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin controlled by PORT latches
												// 00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx pins controlled by PORT latches
U2MODEbits.WAKE = 0;		//Wake-up on Start bit Detect During Sleep Mode Enable bit
												// 1 = UARTx will continue to sample the UxRX pin; interrupt generated on falling edge, bit cleared in
												//	hardware on following rising edge
												// 0 = No wake-up enabled
U2MODEbits.LPBACK = 0;		//UARTx Loopback Mode Select bit
												// 1 = Enable Loopback mode
												// 0 = Loopback mode is disabled
U2MODEbits.ABAUD = 0;		//Auto-Baud Enable bit
												// 1 = Enable baud rate measurement on the next character ? requires reception
												//	of a Sync field (55h); cleared in hardware upon completion
												// 0 = Baud rate measurement disabled or completed
//U2MODEbits.URXINV = 0;		//Receive Polarity Inversion bit ( maybe be missing from some chip .h files)
												// 1 = UxRX Idle state is ?0?
												// 0 = UxRX Idle state is ?1?
U2MODEbits.BRGH = 0;		//High Baud Rate Enable bit
												// 1 = BRG generates 4 clocks per bit period  (4x Baud Clock, High-Speed mode)
												// 0 = BRG generates 16 clocks per bit period (16x Baud Clock, Standard mode)
							//Parity and Data Selection bits
U2MODEbits.PDSEL = ((Parity || DataBits) & 0x03);
//U2MODEbits.PDSEL1 = 0;
//U2MODEbits.PDSEL0 = 0;						// 11 = 9-bit data, no parity
												// 10 = 8-bit data, odd parity
												// 01 = 8-bit data, even parity
												// 00 = 8-bit data, no parity
U2MODEbits.STSEL = StopBits;//Stop Bit Selection bit
												// 1 = Two Stop bits
												// 0 = One Stop bit

// USART 2 Status and Control register
U2STAbits.UTXISEL1 = 1;		//Transmission Interrupt Mode Selection bits
U2STAbits.UTXISEL0 = 0;							// 11 = Reserved; do not use
												// 10 = Interrupt when a character is transferred to the Transmit Shift
												//	Register and as a result, the transmit buffer becomes empty
												// 01 = Interrupt when the last character is shifted out of the Transmit
												//	Shift Register; all transmit operations are completed
												// 00 = Interrupt when a character is transferred to the Transmit Shift Register
												//	 (this implies there is at least one character open in the transmit buffer)
U2STAbits.UTXINV	= 0;	//Transmit Polarity Inversion bit
												// If IREN = 0:
												// 1 = TX Idle state is ?0?
												// 0 = TX Idle state is ?1?
												// If IREN = 1:
												// 1 = IrDA encoded TX Idle state is ?1?
												// 0 = IrDA encoded TX Idle state is ?0?
U2STAbits.UTXBRK = 0;		//Transmit Break bit
												// 1 = Send Sync Break on next transmission ? Start bit, followed by twelve ?0? bits, followed by Stop bit;
												//	cleared by hardware upon completion
												// 0 = Sync Break transmission disabled or completed
U2STAbits.UTXEN = 0;		//Transmit Enable bit
												// 1 = Transmit enabled, UxTX pin controlled by UARTx
												// 0 = Transmit disabled, any pending transmission is aborted and buffer is reset. UxTX pin controlled by PORT.
U2STAbits.UTXBF;			//Transmit Buffer Full Status bit (read-only)
												// 1 = Transmit buffer is full
												// 0 = Transmit buffer is not full, at least one more character can be written
U2STAbits.TRMT;				//Transmit Shift Register Empty bit (read-only)
												// 1 = Transmit Shift Register is empty and transmit buffer is empty (the last transmission has completed)
												// 0 = Transmit Shift Register is not empty, a transmission is in progress or queued
U2STAbits.URXISEL1 = 0;		//Receive Interrupt Mode Selection bits
U2STAbits.URXISEL0 = 0;							// 11 = Interrupt is set on RSR transfer, making the receive buffer full (i.e., has 4 data characters)
												// 10 = Interrupt is set on RSR transfer, making the receive buffer 3/4 full (i.e., has 3 data characters)
												// 0x = Interrupt is set when any character is received and transferred from the RSR to the receive buffer;
												//	receive buffer has one or more characters
U2STAbits.ADDEN = 0;		//Address Character Detect bit (bit 8 of received data = 1)
												// 1 = Address Detect mode enabled. If 9-bit mode is not selected, this does not take effect.
												// 0 = Address Detect mode disabled
U2STAbits.RIDLE;			//Receiver Idle bit (read-only)
												// 1 = Receiver is Idle
												// 0 = Receiver is active
U2STAbits.PERR;				//Parity Error Status bit (read-only)
												// 1 = Parity error has been detected for the current character (character at the top of the receive FIFO)
												// 0 = Parity error has not been detected
U2STAbits.FERR;				//Framing Error Status bit (read-only)
												// 1 = Framing error has been detected for the current character (character at the top of the receive FIFO)
												// 0 = Framing error has not been detected
U2STAbits.OERR;				//Receive Buffer Overrun Error Status bit (clear/read-only)
												// 1 = Receive buffer has overflowed
												// 0 = Receive buffer has not overflowed (clearing a previously set OERR bit (1 ? 0 transition) will reset
												//	the receiver buffer and the RSR to the empty state)
U2STAbits.URXDA;			//Receive Buffer Data Available bit (read-only)
												// 1 = Receive buffer has data, at least one more character can be read
												// 0 = Receive buffer is empty
U2STAbits.UTXEN = 1;		//Transmit Enable bit, we will set this at the end in the TX section below
												// 1 = Transmit enabled, UxTX pin controlled by UARTx
												// 0 = Transmit disabled, any pending transmission is aborted and buffer is reset. UxTX pin controlled by PORT.

	U2BRG = (unsigned short)(( (float)configCPU_CLOCK_HZ / ( (float)16 * (float)ulWantedBaud ) ) - (float)0.5);	// BRGH = 0
//	U2BRG = (unsigned short)(( (float)configCPU_CLOCK_HZ / ( (float)4 * (float)ulWantedBaud ) ) - (float)0.5);	// BRGH = 1

	/* It is assumed that this function is called prior to the scheduler being
	started.  Therefore interrupts must not be allowed to occur yet as they
	may attempt to perform a context switch. */

	portDISABLE_INTERRUPTS();

	U2MODEbits.UARTEN = 1;		// Enable the UART
	U2STAbits.UTXEN = 1;		// Transmit Enable

	IFS1bits.U2RXIF = serCLEAR_FLAG;
	IFS1bits.U2TXIF = serCLEAR_FLAG;
	IPC7bits.U2RXIP = configKERNEL_INTERRUPT_PRIORITY;
	IPC7bits.U2TXIP = configKERNEL_INTERRUPT_PRIORITY;
	IEC1bits.U2TXIE = serINTERRUPT_ENABLE;
	IEC1bits.U2RXIE = serINTERRUPT_ENABLE;

	/* Clear the Rx buffer. */
	while( U2STAbits.URXDA == serSET_FLAG )
	{
		cChar = U2RXREG;
	}

	xTxHasEnded = pdTRUE;
}

/*-----------------------------------------------------------*/
signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime )
{
	/* Get the next character from the buffer.  Return false if no characters
	are available or arrive before xBlockTime expires. */
	if( xQueueReceive(((xComPort *)pxPort)->xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}

/*-----------------------------------------------------------*/
signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime )
{
	/* Return false if after the block time there is no room on the Tx queue. */

	if( xQueueSend(((xComPort *)pxPort)->xCharsForTx, &cOutChar, xBlockTime ) != pdPASS )
	{
		return pdFAIL;
	}
	/* A critical section should not be required as xTxHasEnded will not be
	written to by the ISR if it is already 0 (is this correct?) */

	if( xTxHasEnded )
	{
		xTxHasEnded = pdFALSE;
		IFS0bits.U1TXIF = serSET_FLAG;
	}
	return pdPASS;
}

/*-----------------------------------------------------------*/
void vSerialPutString( xComPortHandle pxPort, const signed char * const pcString, unsigned short usStringLength )
{
signed char *pxNext;

	/* A parameters that this port does not use. */
	( void ) usStringLength;

	/* Send each character in the string, one at a time. */
	pxNext = ( signed char * ) pcString;
	while( *pxNext )
	{
		xSerialPutChar( pxPort, *pxNext, serNO_BLOCK );
		pxNext++;
	}
}

/**********************************************************************
*
*	This allows you to use the generic C STDIO features to write to another port:
*	FILE *port = fopen("1","w");
*	fprintf(port,"This will be output through the serial port \n");
*
*	Also use by printf with a handle of 1
*
*	Added support for basic serial port 1 and 2 using FreeRTOS Queue's
*	case 3 will print to serial port 2, if it open
*	This replaces the standard write.c routine in the C30 lib
*
**********************************************************************/
/*
int write(int handle, void *buffer, unsigned int len) {
int i;
	switch (handle)
	{
		default:
	    case 0:				// stdin
	    case 1:				// stdout
	    case 2:				// stderr

			// send the data to serial port 1 via FreeRTOS
			if (!(xPorts[0].xCharsForTx == NULL))
			{
			for (i = len; i; --i)
				{
					xSerialPutChar( (xComPortHandle) (&(xPorts[0])), *(char*) buffer, 0 );
					break;
				}
			}
		break;
		case 3:				// serial port 2

			// send the data to serial port 2 via FreeRTOS
			if (!(xPorts[1].xCharsForTx == NULL))
			{
			for (i = len; i; --i)
				{
					xSerialPutChar( (xComPortHandle) (&(xPorts[1])), *(char*) buffer, 0 );
					break;
				}
			}
	    break;
	}
return(len);
}
*/

/*-----------------------------------------------------------*/
/**
 * Close serial port. Turn off the interrupts.
 */

void vSerialClose( xComPortHandle xPort )
{
unsigned short usPort;

	usPort = ((xComPort *)xPort)->sPort   ;

		switch (usPort)
			{
			case 0:
				IEC0bits.U1TXIE = serINTERRUPT_DISABLE;
				IEC0bits.U1RXIE = serINTERRUPT_DISABLE;
				break;
			case 1:
				IEC1bits.U2TXIE = serINTERRUPT_DISABLE;
				IEC1bits.U2RXIE = serINTERRUPT_DISABLE;
				break;
			default:			// do nothing if its not a valid port
				break;
			}
}

/*-----------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt( void )
{
char cChar;
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Get the character and post it on the queue of Rxed characters.
	If the post causes a task to wake, force a context switch as the woken task
	may have a higher priority than the task we have interrupted. */
	IFS0bits.U1RXIF = serCLEAR_FLAG;
	while( U1STAbits.URXDA )
	{
		cChar = U1RXREG;
		xQueueSendFromISR( xPorts[0].xRxedChars, &cChar, &xHigherPriorityTaskWoken );
	}
		// if the interrupt handler was disabled while data was being received,
		// data may have backed-up in the receiver circuitry, causing an overrun
		// condition. So let's check the OERR bit to see if this has happened
		// and if it has, we'll need to reset the serial port receiver circuitry
		// to get data flowing again.

		if(U1STAbits.OERR)
		{
			// reset by setting to zero
			U1STAbits.OERR = 0;

			// indicate that we've had an error
			xPorts[0].ucOverrunErr++;
		}

		// if incoming data gets misaligned and the receiver doesn't receive a
		// stop bit where it expects to detect it, the receiver circuitry will
		// set the FERR bit to indicate that it's received corrupted data. The
		// likely reason for this is an incorrectly set baud rate on either the
		// receiver or transmitter end.
		if(U1STAbits.FERR)
		{
			xPorts[0].ucFramingErr++;
		}

		// lets also check for an incoming party error

		if(U1STAbits.PERR)
		{
			xPorts[0].ucPartyErr++;
		}

	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD();
	}
}

/*-----------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt( void )
{
signed char cChar;
portBASE_TYPE xTaskWoken = pdFALSE;

	/* If the transmit buffer is full we cannot get the next character.
	Another interrupt will occur the next time there is space so this does
	not matter. */
	IFS0bits.U1TXIF = serCLEAR_FLAG;
	while( !( U1STAbits.UTXBF ) )
	{
		if( xQueueReceiveFromISR( xPorts[0].xCharsForTx, &cChar, &xTaskWoken ) == pdTRUE )
		{
			/* Send the next character queued for Tx. */
			U1TXREG = cChar;
		}
		else
		{
			/* Queue empty, nothing to send. */
			xTxHasEnded = pdTRUE;
			break;
		}
	}
	if( xTaskWoken != pdFALSE )
	{
		taskYIELD();
	}
}

/*-----------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt( void )
{
char cChar;
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Get the character and post it on the queue of Rxed characters.
	If the post causes a task to wake force a context switch as the woken task
	may have a higher priority than the task we have interrupted. */
	IFS1bits.U2RXIF = serCLEAR_FLAG;
	while( U2STAbits.URXDA )
	{
		cChar = U2RXREG;
		xQueueSendFromISR( xPorts[1].xRxedChars, &cChar, &xHigherPriorityTaskWoken );
	}
		// if the interrupt handler was disabled while data was being received,
		// data may have backed-up in the receiver circuitry, causing an overrun
		// condition. So let's check the OERR bit to see if this has happened
		// and if it has, we'll need to reset the serial port receiver circuitry
		// to get data flowing again.

		if(U2STAbits.OERR)
		{
			// reset by setting to zero
			U2STAbits.OERR = 0;

			// indicate that we've had an error
			xPorts[1].ucOverrunErr++;
		}

		// if incoming data gets misaligned and the receiver doesn't receive a
		// stop bit where it expects to detect it, the receiver circuitry will
		// set the FERR bit to indicate that it's received corrupted data. The
		// likely reason for this is an incorrectly set baud rate on either the
		// receiver or transmitter end.
		if(U2STAbits.FERR)
		{
			xPorts[1].ucFramingErr++;
		}

		// lets also check for an incoming party error

		if(U2STAbits.PERR)
		{
			xPorts[1].ucPartyErr++;
		}

	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD();
	}
}

/*-----------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _U2TXInterrupt( void )
{
signed char cChar;
portBASE_TYPE xTaskWoken = pdFALSE;

	/* If the transmit buffer is full we cannot get the next character.
	Another interrupt will occur the next time there is space so this does
	not matter. */
	IFS1bits.U2TXIF = serCLEAR_FLAG;
	while( !( U2STAbits.UTXBF ) )
	{
		if( xQueueReceiveFromISR( xPorts[1].xCharsForTx, &cChar, &xTaskWoken ) == pdTRUE )
		{
			/* Send the next character queued for Tx. */
			U2TXREG = cChar;
		}
		else
		{
			/* Queue empty, nothing to send. */
			xTxHasEnded = pdTRUE;
			break;
		}
	}

	if( xTaskWoken != pdFALSE )
	{
		taskYIELD();
	}
}

/*-----------------------------------------------------------*/
/*  These routines are NOT RTOS safe... use with care!
*	 required that the serial port be setup prior to any calls */

void uart1_puts(char *str)
{
	while(*str != '\0')
	{
		while(U1STAbits.UTXBF)
			;  /* wait if the buffer is full */
		U1TXREG = *str++;   /* transfer data byte to TX reg */
	}
}

/*-----------------------------------------------------------*/
void uart1_put(char *str, int len)
{
        while(len-- > 0)
        {
                while(U1STAbits.UTXBF)
                        ;  /* wait if the buffer is full */
                U1TXREG = *str++;   /* transfer data byte to TX reg */
        }
}

/*-----------------------------------------------------------*/
void uart1_putc(char c)
{
	while(U1STAbits.UTXBF)
			;
	U1TXREG = c;
}


/*-----------------------------------------------------------*/
void uart2_puts(char *str)
{
	while(*str != '\0')
	{
		while(U2STAbits.UTXBF)
			;  /* wait if the buffer is full */
		U2TXREG = *str++;   /* transfer data byte to TX reg */
	}
}

/*-----------------------------------------------------------*/
void uart2_put(char *str, int len)
{
        while(len-- > 0)
        {
                while(U2STAbits.UTXBF)
                        ;  /* wait if the buffer is full */
                U2TXREG = *str++;   /* transfer data byte to TX reg */
        }
}

/*-----------------------------------------------------------*/
void uart2_putc(char c)
{
	while(U2STAbits.UTXBF)
			;
	U2TXREG = c;
}
/* ****************************************************** */



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


/* BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER. 

NOTE:  This driver is primarily to test the scheduler functionality.  It does
not effectively use the buffers or DMA and is therefore not intended to be
an example of an efficient driver. */

/* Standard include file. */
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* Demo app include files. */
#include "serial.h"

/* Hardware setup. */
#define serOUTPUT						0
#define serINPUT						1
#define serLOW_SPEED					0
#define serONE_STOP_BIT					0
#define serEIGHT_DATA_BITS_NO_PARITY	0
#define serNORMAL_IDLE_STATE			0
#define serAUTO_BAUD_OFF				0
#define serLOOPBACK_OFF					0
#define serWAKE_UP_DISABLE				0
#define serNO_HARDWARE_FLOW_CONTROL		0
#define serSTANDARD_IO					0
#define serNO_IRDA						0
#define serCONTINUE_IN_IDLE_MODE		0
#define serUART_ENABLED					1
#define serINTERRUPT_ON_SINGLE_CHAR		0
#define serTX_ENABLE					1
#define serINTERRUPT_ENABLE				1
#define serINTERRUPT_DISABLE			0
#define serCLEAR_FLAG					0
#define serSET_FLAG						1


/* The queues used to communicate between tasks and ISR's. */
static xQueueHandle xUart1RxQueue;
static xQueueHandle xUart1TxQueue;
static xQueueHandle xUart2RxQueue;
static xQueueHandle xUart2TxQueue;

static portBASE_TYPE xU1TxHasEnded;
static portBASE_TYPE xU2TxHasEnded;
/*-----------------------------------------------------------*/

void vSetupUart1( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
    char cChar;

    /* Create the queues used by the com test task. */
    xUart1RxQueue = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
    xUart1TxQueue = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );

    /* Setup the UART. */
    U1MODEbits.BRGH		= serLOW_SPEED;
    U1MODEbits.STSEL	= serONE_STOP_BIT;
    U1MODEbits.PDSEL	= serEIGHT_DATA_BITS_NO_PARITY;
    U1MODEbits.RXINV	= serNORMAL_IDLE_STATE;
    U1MODEbits.ABAUD	= serAUTO_BAUD_OFF;
    U1MODEbits.LPBACK	= serLOOPBACK_OFF;
    U1MODEbits.WAKE		= serWAKE_UP_DISABLE;
    U1MODEbits.UEN		= serNO_HARDWARE_FLOW_CONTROL;
    U1MODEbits.IREN		= serNO_IRDA;
    U1MODEbits.USIDL	= serCONTINUE_IN_IDLE_MODE;
    U1MODEbits.UARTEN	= serUART_ENABLED;

    U1BRG = (unsigned short)(( (float)configCPU_CLOCK_HZ / ( (float)16 * (float)ulWantedBaud ) ) - (float)0.5);
//        U2BRG = (unsigned short)(( (float)2900000 / ( (float)16 * (float)ulWantedBaud ) ) - (float)0.5); // TO/DO: fix this

    U1STAbits.URXISEL	= serINTERRUPT_ON_SINGLE_CHAR;
    U1STAbits.UTXEN		= serTX_ENABLE;
    U1STAbits.UTXINV	= serNORMAL_IDLE_STATE;
    U1STAbits.UTXISEL0	= serINTERRUPT_ON_SINGLE_CHAR;
    U1STAbits.UTXISEL1	= serINTERRUPT_ON_SINGLE_CHAR;

    /* It is assumed that this function is called prior to the scheduler being
    started.  Therefore interrupts must not be allowed to occur yet as they
    may attempt to perform a context switch. */
    portDISABLE_INTERRUPTS();

    _U1RXIF = serCLEAR_FLAG;
    _U1TXIF = serCLEAR_FLAG;
    _U1RXIP = configKERNEL_INTERRUPT_PRIORITY;
    _U1TXIP = configKERNEL_INTERRUPT_PRIORITY;
    _U1TXIE = serINTERRUPT_ENABLE;
    _U1RXIE = serINTERRUPT_ENABLE;

    /* Clear the Rx buffer. */
    while( U1STAbits.URXDA == serSET_FLAG )
    {
            cChar = U1RXREG;
    }

    xU1TxHasEnded = pdTRUE;
}
void vSetupUart2( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
    char cChar;

    /* Create the queues used by the com test task. */
    xUart2RxQueue = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
    xUart2TxQueue = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );

    /* Setup the UART. */
    U2MODEbits.BRGH		= serLOW_SPEED;
    U2MODEbits.STSEL	= serONE_STOP_BIT;
    U2MODEbits.PDSEL	= serEIGHT_DATA_BITS_NO_PARITY;
    U2MODEbits.RXINV	= serNORMAL_IDLE_STATE;
    U2MODEbits.ABAUD	= serAUTO_BAUD_OFF;
    U2MODEbits.LPBACK	= serLOOPBACK_OFF;
    U2MODEbits.WAKE		= serWAKE_UP_DISABLE;
    U2MODEbits.UEN		= serNO_HARDWARE_FLOW_CONTROL;
    U2MODEbits.IREN		= serNO_IRDA;
    U2MODEbits.USIDL	= serCONTINUE_IN_IDLE_MODE;
    U2MODEbits.UARTEN	= serUART_ENABLED;

    U2BRG = (unsigned short)(( (float)configCPU_CLOCK_HZ / ( (float)16 * (float)ulWantedBaud ) ) - (float)0.5);
//        U2BRG = (unsigned short)(( (float)2900000 / ( (float)16 * (float)ulWantedBaud ) ) - (float)0.5); // TO/DO: fix this

    U2STAbits.URXISEL	= serINTERRUPT_ON_SINGLE_CHAR;
    U2STAbits.UTXEN		= serTX_ENABLE;
    U2STAbits.UTXINV	= serNORMAL_IDLE_STATE;
    U2STAbits.UTXISEL0	= serINTERRUPT_ON_SINGLE_CHAR;
    U2STAbits.UTXISEL1	= serINTERRUPT_ON_SINGLE_CHAR;

    /* It is assumed that this function is called prior to the scheduler being
    started.  Therefore interrupts must not be allowed to occur yet as they
    may attempt to perform a context switch. */
    portDISABLE_INTERRUPTS();

    _U2RXIF = serCLEAR_FLAG;
    _U2TXIF = serCLEAR_FLAG;
    _U2RXIP = configKERNEL_INTERRUPT_PRIORITY;
    _U2TXIP = configKERNEL_INTERRUPT_PRIORITY;
    _U2TXIE = serINTERRUPT_ENABLE;
    _U2RXIE = serINTERRUPT_ENABLE;

    /* Clear the Rx buffer. */
    while( U2STAbits.URXDA == serSET_FLAG )
    {
            cChar = U2RXREG;
    }

    xU2TxHasEnded = pdTRUE;
}

xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
    vSetupUart1( ulWantedBaud, uxQueueLength );
    vSetupUart2( ulWantedBaud, uxQueueLength );
    return NULL;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xUart1GetChar( xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime )
{
    /* Only one port is supported. */
    ( void ) pxPort;

    /* Get the next character from the buffer.  Return false if no characters
    are available or arrive before xBlockTime expires. */
    if( xQueueReceive( xUart1RxQueue, pcRxedChar, xBlockTime ) )
    {
        return pdTRUE;
    }
    else
    {
        return pdFALSE;
    }
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xUart1PutChar( xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime )
{
    /* Only one port is supported. */
    ( void ) pxPort;

    /* Return false if after the block time there is no room on the Tx queue. */
    if( xQueueSend( xUart1TxQueue, &cOutChar, xBlockTime ) != pdPASS )
    {
        return pdFAIL;
    }

    /* A critical section should not be required as xTxHasEnded will not be
    written to by the ISR if it is already 0 (is this correct?). */
    if( xU1TxHasEnded )
    {
        xU1TxHasEnded = pdFALSE;
        _U2TXIF = serSET_FLAG;
    }

    return pdPASS;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xUart2GetChar( xComPortHandle pxPort, signed char *pcRxedChar, portTickType xBlockTime )
{
    /* Only one port is supported. */
    ( void ) pxPort;

    /* Get the next character from the buffer.  Return false if no characters
    are available or arrive before xBlockTime expires. */
    if( xQueueReceive( xUart2RxQueue, pcRxedChar, xBlockTime ) )
    {
        return pdTRUE;
    }
    else
    {
        return pdFALSE;
    }
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xUart2PutChar( xComPortHandle pxPort, signed char cOutChar, portTickType xBlockTime )
{
	/* Only one port is supported. */
	( void ) pxPort;

	/* Return false if after the block time there is no room on the Tx queue. */
	if( xQueueSend( xUart2TxQueue, &cOutChar, xBlockTime ) != pdPASS )
	{
            return pdFAIL;
	}

	/* A critical section should not be required as xTxHasEnded will not be
	written to by the ISR if it is already 0 (is this correct?). */
	if( xU2TxHasEnded )
	{
            xU2TxHasEnded = pdFALSE;
            _U2TXIF = serSET_FLAG;
	}

	return pdPASS;
}
/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
}
/*-----------------------------------------------------------*/

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt( void )
{
    char cChar;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* Get the character and post it on the queue of Rxed characters.
    If the post causes a task to wake force a context switch as the woken task
    may have a higher priority than the task we have interrupted. */
    _U1RXIF = serCLEAR_FLAG;
    while( U1STAbits.URXDA )
    {
        cChar = U1RXREG;
        xQueueSendFromISR( xUart1RxQueue, &cChar, &xHigherPriorityTaskWoken );
    }

    if( xHigherPriorityTaskWoken != pdFALSE )
    {
        taskYIELD();
    }
}
/*-----------------------------------------------------------*/

void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt( void )
{
    signed char cChar;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* If the transmit buffer is full we cannot get the next character.
    Another interrupt will occur the next time there is space so this does
    not matter. */
    _U1TXIF = serCLEAR_FLAG;
    while( !( U1STAbits.UTXBF ) )
    {
        if( xQueueReceiveFromISR( xUart1TxQueue, &cChar, &xHigherPriorityTaskWoken ) == pdTRUE )
        {
            /* Send the next character queued for Tx. */
            U1TXREG = cChar;
        }
        else
        {
            /* Queue empty, nothing to send. */
            xU1TxHasEnded = pdTRUE;
            break;
        }
    }

    if( xHigherPriorityTaskWoken != pdFALSE )
    {
        taskYIELD();
    }
}
/*-----------------------------------------------------------*/

void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt( void )
{
    char cChar;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* Get the character and post it on the queue of Rxed characters.
    If the post causes a task to wake force a context switch as the woken task
    may have a higher priority than the task we have interrupted. */
    _U2RXIF = serCLEAR_FLAG;
    while( U2STAbits.URXDA )
    {
        cChar = U2RXREG;
        xQueueSendFromISR( xUart2RxQueue, &cChar, &xHigherPriorityTaskWoken );
    }

    if( xHigherPriorityTaskWoken != pdFALSE )
    {
        taskYIELD();
    }
}
/*-----------------------------------------------------------*/

void __attribute__((__interrupt__, auto_psv)) _U2TXInterrupt( void )
{
    signed char cChar;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* If the transmit buffer is full we cannot get the next character.
    Another interrupt will occur the next time there is space so this does
    not matter. */
    _U2TXIF = serCLEAR_FLAG;
    while( !( U2STAbits.UTXBF ) )
    {
        if( xQueueReceiveFromISR( xUart2TxQueue, &cChar, &xHigherPriorityTaskWoken ) == pdTRUE )
        {
            /* Send the next character queued for Tx. */
            U2TXREG = cChar;
        }
        else
        {
            /* Queue empty, nothing to send. */
            xU2TxHasEnded = pdTRUE;
            break;
        }
    }

    if( xHigherPriorityTaskWoken != pdFALSE )
    {
        taskYIELD();
    }
}

#endif
