
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pic24.h"

#include "system.h"
#include "iomapping.h"
#include "serial.h"
#include "uart.h"
#include "testing.h"

/* The UART we use for the console (Dev Board: 1, Explorer16: 2) */
#define uartCONSOLE_UART     1

/* We should find that each character can be queued for Tx immediately and we
don't have to block to send. */
#define uartNO_BLOCK         ( ( portTickType ) 0 )

/* The Rx task will block on the Rx queue for a long period. */
#define uartRX_BLOCK_TIME    ( ( portTickType ) 0xffff )

/* The length of the queue used to send messages to the UART queue. */
#define uartQUEUE_SIZE       320

#define uartBAUD             9600

#define uartESCAPE_CHARACTER "\033"
#define uartDEFAULT_TEXT     uartESCAPE_CHARACTER "[0;37m"
#define uartERROR_TEXT       uartESCAPE_CHARACTER "[0;31m"


/*
 * The UART is written to by more than one task so is controlled by this
 * 'gatekeeper' task.  This is the only task that is actually permitted to
 * access the UART directly.  Other tasks wanting to display a message send
 * the message to the gatekeeper.
 */
//static void vUartTxTask( void *pvParameters );
static void vUart1RxTask( void *pvParameters );
static void vUart2RxTask( void *pvParameters );

static void prvUartInit (void);

static char pcBuffer[uartBUFFER_SIZE];

/* Static variables */

static int isUartRelay;

/* Implementations */

static void prvUartInit (void)
{
    vIoMap();

    RPINR19bits.U2RXR = 10;
    RPOR8bits.RP17R = 5;
    vSetupUart2( uartBAUD, uartQUEUE_SIZE );
    
//    RPINR19bits.U2RXR = 10;
//    RPOR8bits.RP17R = 5;
//    U2STA= 0x0000;
//    U2MODE  = 0x0000; // initialize the UART module
//    U2MODEbits.UARTEN = 1;
//    U2STAbits.UTXEN=1;
//    IFS1bits.U2RXIF = 0;

    vUart2ChannelSelect(HEM);

    RPINR18bits.U1RXR=30;
    RPOR8bits.RP16R = 3;
    vSetupUart1( uartBAUD, uartQUEUE_SIZE );

//    RPINR18bits.U1RXR=30;
//    RPOR8bits.RP16R = 3;
//    U1STA= 0x0000;
//    U1MODE  = 0x0000; // initialize the UART module
//    U1MODEbits.UARTEN = 1;
//    U1STAbits.UTXEN=1;
//    IFS0bits.U1RXIF = 0;
}

void vUartStartTask( void )
{
    /* Initialise the hardware. */
    prvUartInit();

    xTaskCreate( vUart2RxTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_UART2, NULL );
    xTaskCreate( vUart1RxTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_UART1, NULL );

    /* TODO (Bug?): Figure out why U1TXREG must be set for UART to work. */
    /* UxTXREG can be set to anything, as long as it's written*/
//    while(U1STAbits.UTXBF == 1);
    U1TXREG = 0;
//    while(U2STAbits.UTXBF == 1);
    U2TXREG = 0;
}

void vUart1Putc( char cChar )
{
    xUart1PutChar( NULL, (signed char) cChar, uartNO_BLOCK );
}
void vUart2Putc( char cChar )
{
    xUart2PutChar( NULL, (signed char) cChar, uartNO_BLOCK );
}

void vUart1Puts( char *pcString )
{
    while (*pcString)
    {
        vUart1Putc(*pcString++);
    }
}
void vUart2Puts( char *pcString )
{
    while (*pcString)
    {
        vUart2Putc(*pcString++);
    }
}

void vConsolePutc( char cChar )
{
#if uartCONSOLE_UART == 1
    vUart1Putc( cChar );
#else
    vUart2Putc( cChar );
#endif
}

void vConsolePrint( char *pcString )
{
#if uartCONSOLE_UART == 1
    vUart1Puts( pcString );
#else
    vUart2Puts( pcString );
#endif
}

void vConsolePuts( char *pcString )
{
    vConsolePrint( pcString );
    vConsolePrint( "\r\n" );
}

void vConsolePutsError( char *pcString )
{
    // TODO: Add more useful info
    vConsolePrint( uartERROR_TEXT "[ERROR] " );
    vConsolePrint( pcString );
    vConsolePuts( uartDEFAULT_TEXT );
}

void vUartRelayMode( int mode )
{
    isUartRelay = mode;
}

static unsigned short prvConsoleGetc( char c, char* pcCommandBuffer )
{
    static unsigned int i = 0;
    unsigned short usEnter = 0;

    switch(c) {
        case -1:    // Yes, it happens.
            break;
        case 1:     // ^A
        case 2:     // ^B
        case 3:     // ^C
            // TODO: Interrupt command
            break;
        case 8:     // Backspace
        case 127:   // Delete
            if (i > 0) {
                vConsolePutc(c);
                --i;
            }
            break;
        case 10:    // Line feed
        case 11:    // Vertical tab
        case 13:    // Carriage return
            vConsolePutc('\r');
            vConsolePutc('\n');
            i = 0;
            sprintf( pcCommandBuffer, "%s", pcBuffer );
            usEnter = 1;
            break;
        case 27:    // Escape
            break;
        default:
            vConsolePutc(c);
            pcBuffer[i] = c;
            ++i;
    }

    if (i >= uartBUFFER_SIZE) {
        vConsolePuts("(OVERFLOW!)");
        i = 0;
    }

    pcBuffer[i] = '\0';
    
    return usEnter;
}

static void prvConsoleRx( signed char cRxByte )
{
    static char pcCommandBuffer[uartBUFFER_SIZE];
    
    if( prvConsoleGetc( cRxByte, pcCommandBuffer ) )
    {
        // User pressed enter
        vInterpretTestingCommand( pcCommandBuffer );
    }
    else
    {
        // User entered key other than enter
    }
}

static void vUart1RxTask( void *pvParameters )
{

    signed char cRxByte;

    for( ;; )
    {
        /* Block on the queue that contains received bytes until a byte is
        available. */
        U1TXREG = 0; /* TODO (bug?) why is this needed? */
        if( xUart1GetChar( NULL, &cRxByte, uartRX_BLOCK_TIME ) )
        {
//            vConsolePuts( "Uart1Rx!" );
            if( isUartRelay )
            {
                vUart2Putc(cRxByte);
            }
            else
            {
#if uartCONSOLE_UART == 1
                prvConsoleRx(cRxByte);
#endif
            }
        }
    }
}

static void vUart2RxTask( void *pvParameters )
{
    signed char cRxByte;

    for( ;; )
    {
        /* Block on the queue that contains received bytes until a byte is
        available. */
//        U2TXREG = 0; /* TODO (bug?) why is this needed? */
        if( xUart2GetChar( NULL, &cRxByte, uartRX_BLOCK_TIME ) )
        {
//            vConsolePuts( "Uart2Rx!" );
            if( isUartRelay )
            {
                vUart1Putc(cRxByte);
            }
            else
            {
#if uartCONSOLE_UART != 1
                prvConsoleRx(cRxByte);
#else
                vUart1Putc(cRxByte);
#endif
            }
        }
    }
}

