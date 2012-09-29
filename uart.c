
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pic24.h"

#include "uart.h"

/*
 * The UART is written to by more than one task so is controlled by this
 * 'gatekeeper' task.  This is the only task that is actually permitted to
 * access the UART directly.  Other tasks wanting to display a message send
 * the message to the gatekeeper.
 */
static void vUartTxTask( void *pvParameters );
static void vUartRxTask( void *pvParameters );

/*
 * Setup UART2.
 */
static void prvUartInit (void);

/*
 * Write a string of text to the UART
 */
static void prvUartPuts( char *s );

static void prvUartPutc( char c );

static char prvUartGetc( void );

static void prvUartGets( char *s, int len );


/* The length of the queue used to send messages to the UART gatekeeper task. */
#define uartQUEUE_SIZE      3

#define uartBUFFER_SIZE     80


/* The queue used to send messages to the LCD task. */
static xQueueHandle xUartTxQueue;
static xQueueHandle xUartRxQueue;

void vStartUartTask( void )
{
    /* Initialise the hardware. */
    prvUartInit();

    /* Create the queue used by the UART task.  Messages for display on the UART
    are received via this queue. */
    xUartTxQueue = xQueueCreate( uartQUEUE_SIZE, sizeof( xUartMessage ) );
    xUartRxQueue = xQueueCreate( uartQUEUE_SIZE, sizeof( xUartMessage ) );

    xTaskCreate( vUartTxTask, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
//    xTaskCreate( vUartRxTask, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
}

void vUartPuts(char* message)
{
    static char cUartBuffer[ uartBUFFER_SIZE ];
    static xUartMessage xMessage = { cUartBuffer };

    sprintf(cUartBuffer, message);

    xQueueSend( xUartTxQueue, &xMessage, portMAX_DELAY );
}


static void prvUartInit (void)
{
    /* init the serial port (UART2, 9600@32MHz, 8, N, 1, CTS/RTS) */
    U2BRG   = BRATE;    // initialize the baud rate generator
    U2MODE  = U_ENABLE; // initialize the UART module
    U2STA   = U_TX;     // enable the Transmitter
}

static void prvUartPutc( char c )
{
    while (U2STAbits.UTXBF);    // wait while Tx buffer full
    U2TXREG = (unsigned int) c;
}

static void prvUartPuts( char *s )
{
    while (*s)
    {
        prvUartPutc(*s++);
    }
}

static char prvUartGetc( void )
{
    while (!U2STAbits.URXDA);   // wait for a new character to arrive
    return (char) U2RXREG;      // read the character from the receive buffer
}

static void prvUartGets( char *s, int len )
{
    char *p = s;
    int i;
    for (i=0; i<len-1; ++i) {
        *s = prvUartGetc();
        prvUartPutc(*s);
        if ((*s==BACKSPACE)&&(s>p)) {
            prvUartPutc(' ');     // overwrite last character
            prvUartPutc(BACKSPACE);
            ++len;
            --s;
            continue;
        }
        if (*s=='\r' || *s=='\n')
            break;
        ++s;
    }
    *s = '\0';
}

static void vUartTxTask( void *pvParameters )
{
    xUartMessage xMessage;

    /* Welcome message. */
    prvUartPuts( "I'm a Uart; Hello World!\r\n" );

    for( ;; )
    {

//        prvUartPuts( "I'm still a UART!\r\n" );
        /* Wait for a message to arrive that requires displaying. */
        while( xQueueReceive( xUartTxQueue, &xMessage, portMAX_DELAY ) != pdPASS );
        prvUartPuts( xMessage.pcMessage );

    }
}

static void vUartRxTask( void *pvParameters )
{
}
