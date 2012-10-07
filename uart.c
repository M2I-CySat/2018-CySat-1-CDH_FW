
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


//static xQueueHandle xUartTxQueue;
//static xQueueHandle xUartRxQueue;



static void prvUartInit (void)
{
    xSerialPortInitMinimal( uartBAUD, uartQUEUE_SIZE );
}

void vStartUartTask( void )
{
    /* Initialise the hardware. */
    prvUartInit();

    /* Create the queue used by the UART task.  Messages for display on the UART
    are received via this queue. */
//    xUartTxQueue = xQueueCreate( uartQUEUE_SIZE, sizeof( signed char ) );
//    xUartRxQueue = xQueueCreate( uartQUEUE_SIZE, sizeof( signed char ) );

//    xTaskCreate( vUartTxTask, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
    xTaskCreate( vUartRxTask, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
}


//static char prvUartGetc( void )
//{
//    while (!U2STAbits.URXDA);   // wait for a new character to arrive
//    return (char) U2RXREG;      // read the character from the receive buffer
//}
//
//static void prvUartGets( char *s, int len )
//{
//    char *p = s;
//    int i;
//    for (i=0; i<len-1; ++i) {
//        *s = prvUartGetc();
//        prvUartPutc(*s);
//        if ((*s==BACKSPACE)&&(s>p)) {
//            prvUartPutc(' ');     // overwrite last character
//            prvUartPutc(BACKSPACE);
//            ++len;
//            --s;
//            continue;
//        }
//        if (*s=='\r' || *s=='\n')
//            break;
//        ++s;
//    }
//    *s = '\0';
//}

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

static void vUartRxTask( void *pvParameters )
{
    char buffer[lcdSTRING_LENGTH];
    unsigned short i = 0;

    signed char cRxByte;

    for( ;; )
    {
        /* Block on the queue that contains received bytes until a byte is
        available. */
        if( xSerialGetChar( NULL, &cRxByte, uartRX_BLOCK_TIME ) )
        {
            /* echo */
            vUartPutc(cRxByte);

            /* print to LCD */
            buffer[i] = cRxByte;
            buffer[i+1] = '\0';
            i = (i+1)%(lcdSTRING_LENGTH - 1);
            
            vLcdPuts(buffer);
        }
    }
}

