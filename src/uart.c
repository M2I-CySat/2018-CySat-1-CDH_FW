#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#include "stm32f4xx_usart.h"

#include "uart.h"

#include <stdio.h>

/*
 * NOTE: This file serves to create all of the put/print stuff as well as 
 * establish a "console" uart (vConsolePrintf). Currently, uart1 is usart2,
 * which is console, and uart2 is usart6 (probably radio)
 */


/* We should find that each character can be queued for Tx immediately and we
don't have to block to send. */
#define uartNO_BLOCK        ( ( portTickType ) 0 )

/* The Rx task will block on the Rx queue for a long period. */
#define uartRX_BLOCK_TIME   ( ( portTickType ) 0xffff )

/* The length of the queue used to send messages to the UART queue. */
#define uartQUEUE_SIZE      ( (unsigned portBASE_TYPE) 320 )

#define uartBAUD            9600

static void prvUartInit (void);

/* Static variables */

/* Implementations */

static void prvUartInit (void)
{
    
    /* TODO: Call a function in serial.c/serial.h that does hardware init */
    /* Also todo - implement that function to be called */
    /* Then call functions that create and start the serial queues and tasks */
    
}

void vUartStartTask( void )
{
    /* Initialise the hardware. */
    prvUartInit();
}

inline static portBASE_TYPE prvUartPut( USART_TypeDef & usart, char c )
{
    //TODO: put character into tx queue
}

inline static portBASE_TYPE prvUartPrint( USART_TypeDef & usart, char s[])
{
    int i;
    for( i = 0; s[i]; ++i )
    {
        if( pdFAIL == prvUartPut( usart, s[i] ) )
        {
            return pdFAIL;
        }
    }

    return pdPASS;
}

inline static portBASE_TYPE prvUartVprintf(  USART_TypeDef & usart, const char *fmt, va_list ap )
{
    static char pcPrintfBuffer[uartSPRINTF_BUFFER_SIZE+1];
    portBASE_TYPE out;

    vsnprintf( pcPrintfBuffer, uartSPRINTF_BUFFER_SIZE, fmt, ap );

    out = prvUartPrint( usart, pcPrintfBuffer );

    return out;
}

void vUart1Put( char c )
{
    prvUartPut( USART2, c );
}

void vUart1Print( char *s )
{
    prvUartPrint( USART2, s );
}

void vUart1Printf( const char *fmt, ... )
{
    va_list ap;
    va_start(ap, fmt);
    prvUartVprintf( USART2, fmt, ap );
    va_end(ap);
}

void vUart2Put( char c )
{
    prvUartPut( USART6, c );
}

void vUart2Print( char *s )
{
    prvUartPrint( USART6, s );
}

void vUart2Printf( const char *fmt, ... )
{
    va_list ap;
    va_start(ap, fmt);
    prvUartVprintf( USART6, fmt, ap );
    va_end(ap);
}

void vConsolePut( char c )
{
    prvUartPut( USART2, c );
}

void vConsolePrint( char *s )
{
    prvUartPrint( USART2, s );
}

void vConsolePrintf( const char *fmt, ... )
{
    va_list ap;
    va_start(ap, fmt);
    prvUartVprintf( USART2, fmt, ap );
    va_end(ap);
}

void vConsoleErrorPrintf( const char *fmt, ... )
{
    va_list ap;
    vConsolePrint( uartERROR_TEXT "[ERROR] " );
    va_start(ap, fmt);
    prvUartVprintf( USART2, fmt, ap );
    va_end(ap);
    vConsolePrint( uartDEFAULT_TEXT );
}