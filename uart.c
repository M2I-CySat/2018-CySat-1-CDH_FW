
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pic24.h"

#include "serial.h"
#include "uart.h"
#include "testing.h"

/* The UART we use for the console (Dev Board: 1, Explorer16: 2) */
#define uartCONSOLE_UART     2

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


static void prvUartInit (void)
{
    vSetupUart1( uartBAUD, uartQUEUE_SIZE );
    vSetupUart2( uartBAUD, uartQUEUE_SIZE );
}

void vStartUartTask( void )
{
    /* Initialise the hardware. */
    prvUartInit();

    xTaskCreate( vUart1RxTask, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
    xTaskCreate( vUart2RxTask, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
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

void vConsolePuts( char *pcString )
{
#if uartCONSOLE_UART == 1
    vUart1Puts( pcString );
#else
    vUart2Puts( pcString );
#endif
}

void vConsolePutsError( char *pcString )
{
    // TODO: Add more useful info
    vConsolePuts( uartERROR_TEXT "[ERROR] " );
    vConsolePuts( pcString );
    vConsolePuts( uartDEFAULT_TEXT );
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
        vConsolePuts("(OVERFLOW!)\r\n");
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
        interpretTestingCommand( pcCommandBuffer );
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
        if( xUart1GetChar( NULL, &cRxByte, uartRX_BLOCK_TIME ) )
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
        if( xUart2GetChar( NULL, &cRxByte, uartRX_BLOCK_TIME ) )
        {
#if uartCONSOLE_UART != 1
            prvConsoleRx(cRxByte);
#else
            vUart1Putc(cRxByte);
#endif
        }
    }
}

