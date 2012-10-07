
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pic24.h"

#include "serial.h"
#include "uart.h"
#include "lcd.h"


/* We should find that each character can be queued for Tx immediately and we
don't have to block to send. */
#define uartNO_BLOCK         ( ( portTickType ) 0 )

/* The Rx task will block on the Rx queue for a long period. */
#define uartRX_BLOCK_TIME    ( ( portTickType ) 0xffff )

/* The length of the queue used to send messages to the UART queue. */
#define uartQUEUE_SIZE       320

#define uartBAUD             9600

#define uartBUFFER_SIZE      lcdSTRING_LENGTH

/*
 * The UART is written to by more than one task so is controlled by this
 * 'gatekeeper' task.  This is the only task that is actually permitted to
 * access the UART directly.  Other tasks wanting to display a message send
 * the message to the gatekeeper.
 */
//static void vUartTxTask( void *pvParameters );
static void vUartRxTask( void *pvParameters );

/*
 * Setup UART2.
 */
static void prvUartInit (void);

static char pcBuffer[uartBUFFER_SIZE];

char pcUartCommand[uartBUFFER_SIZE];


static void prvUartInit (void)
{
    xSerialPortInitMinimal( uartBAUD, uartQUEUE_SIZE );
}

void vStartUartTask( void )
{
    /* Initialise the hardware. */
    prvUartInit();

    xTaskCreate( vUartRxTask, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
}

void vUartPutc( char cChar )
{
    xSerialPutChar( NULL, (signed char) cChar, uartNO_BLOCK );
}

void vUartPuts( char *pcString )
{
    while (*pcString)
    {
        vUartPutc(*pcString++);
    }
}

//static void vUartTxTask( void *pvParameters )
//{
//    char cChar;
//
//    /* Welcome message. */
////    vUartPuts( "I'm a Uart; Hello World!\r\n" );
//
//    for( ;; )
//    {
//
////        vUartPuts( "I'm still a UART!\r\n" );
//        /* Wait for a message to arrive that requires displaying. */
//        while( xQueueReceive( xUartTxQueue, &cChar, portMAX_DELAY ) != pdPASS );
//        vUartPutc( cChar );
//
//    }
//}

static void prvUartCopyCommandBuffer()
{
    static unsigned int i = 0;
    for( i = 0; i < uartBUFFER_SIZE; ++i )
    {
        pcUartCommand[i] = pcBuffer[i];

        if( pcUartCommand[i] == '\0' )
        {
            break;
        }
    }
}

static unsigned short prvUartGetc( char c )
{
    static unsigned int i = 0;
    unsigned short usEnter = 0;

    switch(c) {
        case -1:    // Yes, it happens.
            break;
        case 1:     // ^A
        case 2:     // ^B
        case 3:     // ^C
            break;
        case 8:     // Backspace
        case 127:   // Delete
            if (i > 0) {
                vUartPutc(c);
                --i;
            }
            break;
        case 10:    // Line feed
        case 11:    // Vertical tab
        case 13:    // Carriage return
            vUartPutc('\r');
            vUartPutc('\n');
            i = 0;
            prvUartCopyCommandBuffer();
            usEnter = 1;
            break;
        case 27:    // Escape
            break;
        default:
            vUartPutc(c);
            pcBuffer[i] = c;
            ++i;
    }

    if (i >= uartBUFFER_SIZE) {
        vUartPuts("(OVERFLOW!)\r\n");
        i = 0;
    }

    pcBuffer[i] = '\0';
    
    return usEnter;
}

static void vUartRxTask( void *pvParameters )
{
    signed char cRxByte;
    char pcEchoCommand[uartBUFFER_SIZE + 40];

    for( ;; )
    {
        /* Block on the queue that contains received bytes until a byte is
        available. */
        if( xSerialGetChar( NULL, &cRxByte, uartRX_BLOCK_TIME ) )
        {
            if( prvUartGetc( cRxByte ) )
            {
                sprintf( pcEchoCommand, "You told me '%s'\r\n", pcUartCommand );
                vUartPuts(pcEchoCommand);
            }
            else
            {
                vLcdPuts( pcBuffer );
            }
        }
    }
}

