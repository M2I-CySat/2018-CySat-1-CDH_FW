

#include <stdlib.h>
#include <stdarg.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pic24.h"
#include "myprintf.h"

#include "system.h"
#include "iomapping.h"
#include "serial.h"
#include "testing.h"

#include "helium.h"
#include "command.h"
#include "uart.h"

/* The UART we use for the console (Dev Board: 1, Explorer16: 2) */
#define uartCONSOLE_UART    1

#if uartCONSOLE_UART == 1
#define xConsoleUartHandle xUart1Handle
#elif uartCONSOLE_UART == 2
#define xConsoleUartHandle xUart2Handle
#else
#error "Unknown uartCONSOLE_UART"
#endif


/* We should find that each character can be queued for Tx immediately and we
don't have to block to send. */
#define uartNO_BLOCK        ( ( portTickType ) 0 )

/* The Rx task will block on the Rx queue for a long period. */
#define uartRX_BLOCK_TIME   ( ( portTickType ) 0xffff )

/* The length of the queue used to send messages to the UART queue. */
#define uartQUEUE_SIZE      ( (unsigned portBASE_TYPE) 320 )

#define uartBAUD            9600ul
#define uartPARITY          serNO_PARITY
#define uartDATA_BITS       serBITS_8
#define uartSTOP_BITS       serSTOP_1

#define uart1COM            serCOM1
#define uart1BAUD           uartBAUD
#define uart1PARITY         uartPARITY
#define uart1DATA_BITS      uartDATA_BITS
#define uart1STOP_BITS      uartSTOP_BITS
#define uart1TX_QUEUE_SIZE  uartQUEUE_SIZE
#define uart1RX_QUEUE_SIZE  uartQUEUE_SIZE

#define uart2COM            serCOM2
#define uart2BAUD           uartBAUD
#define uart2PARITY         uartPARITY
#define uart2DATA_BITS      uartDATA_BITS
#define uart2STOP_BITS      uartSTOP_BITS
#define uart2TX_QUEUE_SIZE  uartQUEUE_SIZE
#define uart2RX_QUEUE_SIZE  uartQUEUE_SIZE


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

/* Static variables */

#ifdef serialALTERNATE_IMPLEMENTATION
xComPortHandle xUart1Handle;
xComPortHandle xUart2Handle;
#endif

/* Implementations */

static void prvUartInit (void)
{
    vIoMap();

    RPINR19bits.U2RXR = 10;
    RPOR8bits.RP17R = 5;

#ifdef serialALTERNATE_IMPLEMENTATION
    xUart2Handle = xSerialPortInit( uart2COM, uart2BAUD, uart2PARITY, uart2DATA_BITS, uart2STOP_BITS, uart2TX_QUEUE_SIZE, uart2RX_QUEUE_SIZE );
#else
    vSetupUart2( uart2BAUD, uartQUEUE_SIZE );
#endif
    
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
    
#ifdef serialALTERNATE_IMPLEMENTATION
    xUart1Handle = xSerialPortInit( uart1COM, uart1BAUD, uart1PARITY, uart1DATA_BITS, uart1STOP_BITS, uart1TX_QUEUE_SIZE, uart1RX_QUEUE_SIZE );
#else
    vSetupUart1( uart1BAUD, uartQUEUE_SIZE );
#endif

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

    //old private tasks
    //xTaskCreate( vUart2RxTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_UART2, NULL );
    //xTaskCreate( vUart1RxTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_UART1, NULL );

    //mine have a capital X, so what
    //xTaskCreate( vUart2RXTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_UART2, NULL );
    xTaskCreate( vUart1RXTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_UART1, NULL );

    /* TO/DO (Bug?): Figure out why U1TXREG must be set for UART to work. */
    /* UxTXREG can be set to anything, as long as it's written*/
//    while(U1STAbits.UTXBF == 1);
    U1TXREG = 0;
//    while(U2STAbits.UTXBF == 1);
    U2TXREG = 0;
}

/*
 * These functions assume serialALTERNATE_IMPLEMENTATION is defined!
 * If you find a new serial library and removed the original definition, go 
 * ahead and remove this check (but make sure prvUart* functions still work)
 */
#ifndef serialALTERNATE_IMPLEMENTATION
#error "serialALTERNATE_IMPLEMENTATION needs to be used for prvUart* functions"
#endif

inline static portBASE_TYPE prvUartPut( xComPortHandle pxPort, char c )
{
//    uart1_putc(c);
//    return 1;
    return xSerialPutChar( pxPort, c, uartNO_BLOCK );
}

inline static portBASE_TYPE prvUartPrint( xComPortHandle pxPort, char s[])
{
    int i;
    for( i = 0; s[i]; ++i )
    {
        if( pdFAIL == prvUartPut( pxPort, s[i] ) )
        {
            return pdFAIL;
        }
    }

    return pdPASS;
}

inline static portBASE_TYPE prvUartVprintf( xComPortHandle pxPort, const char *fmt, va_list ap )
{
    static char pcPrintfBuffer[uartSPRINTF_BUFFER_SIZE+1];
    portBASE_TYPE out;

//    vsnprintf( pcPrintfBuffer, uartSPRINTF_BUFFER_SIZE, fmt, ap );
    myvsnprintf( pcPrintfBuffer, uartSPRINTF_BUFFER_SIZE, fmt, ap );
//    pcPrintfBuffer[uartSPRINTF_BUFFER_SIZE] = 0;

    out = prvUartPrint( pxPort, pcPrintfBuffer );

    return out;
}

/* TODO Add error checking */

void vUart1Put( char c )
{
    prvUartPut( xUart1Handle, c );
}

void vUart1Print( char *s )
{
    prvUartPrint( xUart1Handle, s );
}

void vUart1Printf( const char *fmt, ... )
{
    va_list ap;
    va_start(ap, fmt);
    prvUartVprintf( xUart1Handle, fmt, ap );
    va_end(ap);
}

void vUart2Put( char c )
{
    prvUartPut( xUart2Handle, c );
}

void vUart2Print( char *s )
{
    prvUartPrint( xUart2Handle, s );
}

void vUart2Printf( const char *fmt, ... )
{
    va_list ap;
    va_start(ap, fmt);
    prvUartVprintf( xUart2Handle, fmt, ap );
    va_end(ap);
}

void vConsolePut( char c )
{
    prvUartPut( xConsoleUartHandle, c );
}

void vConsolePrint( char *s )
{
    prvUartPrint( xConsoleUartHandle, s );
}

void vConsolePrintf( const char *fmt, ... )
{
    va_list ap;
    va_start(ap, fmt);
    prvUartVprintf( xConsoleUartHandle, fmt, ap );
    va_end(ap);
}

void vConsoleErrorPrintf( const char *fmt, ... )
{
    va_list ap;
    vConsolePrint( uartERROR_TEXT "[ERROR] " );
    va_start(ap, fmt);
    prvUartVprintf( xConsoleUartHandle, fmt, ap );
    va_end(ap);
    vConsolePrint( uartDEFAULT_TEXT );
}





static unsigned short prvConsoleGetc( char c, char* pcCommandBuffer )
{
    static char pcBuffer[uartRX_BUFFER_SIZE+1];
    static unsigned int i = 0;
    unsigned short usEnter = 0;

    /* Ensure pcBuffer is ALWAYS null-terminated */
    pcBuffer[uartRX_BUFFER_SIZE] = 0;

    switch(c) {
        case -1:    /* Yes, it happens. */
            break;
        case 1:     /* ^A */
        case 2:     /* ^B */
        case 3:     /* ^C */
            /* TODO: Interrupt command */
            break;
        case 8:     /* Backspace */
        case 127:   /* Delete */
            if (i > 0) {
                vConsolePut(c);
                --i;
            }
            break;
        case 10:    /* Line feed */
        case 11:    /* Vertical tab */
        case 13:    /* Carriage return */
            vConsolePut('\r');
            vConsolePut('\n');
            i = 0;
            mysnprintf( pcCommandBuffer, uartRX_BUFFER_SIZE, "%s", pcBuffer );
            usEnter = 1;
            break;
        case 27:    /* Escape */
            break;
        default:    /* All other chars */
            vConsolePut(c);
            pcBuffer[i] = c;
            ++i;
    }

    if (i >= uartRX_BUFFER_SIZE) {
        vConsolePrint("(OVERFLOW!)\n");
        i = 0;
    }

    pcBuffer[i] = 0;
    
    return usEnter;
}

static void prvConsoleRx( signed char cRxByte )
{
    static char pcCommandBuffer[uartRX_BUFFER_SIZE+1];
    
    if( prvConsoleGetc( cRxByte, pcCommandBuffer ) )
    {
        /* User pressed enter */

        /* Ensure buffer is ALWAYS null-terminated */
        pcCommandBuffer[uartRX_BUFFER_SIZE] = 0;
        vInterpretTestingCommand( pcCommandBuffer );
    }
    else
    {
        /* User entered key other than enter */
    }
}

static void vUart1RxTask( void *pvParameters )
{

    signed char cRxByte;

    for( ;; )
    {
        /* Block on the queue that contains received bytes until a byte is
        available. */
#ifdef serialALTERNATE_IMPLEMENTATION
        if( xSerialGetChar( xUart1Handle, &cRxByte, uartRX_BLOCK_TIME ) )
#else
        U1TXREG = 0; /* TO/DO (bug?) why is this needed? */
        if( xUart1GetChar( NULL, &cRxByte, uartRX_BLOCK_TIME ) )
#endif
        {
#if uartCONSOLE_UART == 1
            prvConsoleRx(cRxByte);
#endif
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
#ifdef serialALTERNATE_IMPLEMENTATION
        if( xSerialGetChar( xUart2Handle, &cRxByte, uartRX_BLOCK_TIME ) )
#else
//        U2TXREG = 0; /* TO/DO (bug?) why is this needed? */
        if( xUart2GetChar( NULL, &cRxByte, uartRX_BLOCK_TIME ) )
#endif
        {
#if uartCONSOLE_UART == 2
            prvConsoleRx(cRxByte);
#else
//            vUart1Putc(cRxByte);
            vHeliumUartRx(cRxByte);
#endif
        }
    }
}

#ifdef uartTEST_FUNCTIONS

static void vUartTestTask();

void vUartTest()
{
    static int i = 0;
#ifdef myprintfDOUBLE
    static double d = 0.0;
//    vConsolePrintf( "Test: %5d %04X %f %c\r\n", i, i, d, i%26+'a' );
    vConsolePrintf( "Test: %5d %+2.2f\r\n", i, 1.0-d);
    d += 0.05;
#else
    vConsolePrintf( "%s: %5d %04X %c\r\n", "Test", i, i, i%26+'a' );
#endif
    ++i;
}


void vUartStartTestTask( void )
{
    xTaskCreate( vUartTestTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_TEST, NULL );
}

static void vUartTestTask()
{
    vTaskDelay(500);
    vConsolePrintf( "sizeof(int) = %d\r\n", sizeof(int) );

    for( ;; )
    {
        vTaskDelay(400);
        vUartTest();
    }
}

#endif
