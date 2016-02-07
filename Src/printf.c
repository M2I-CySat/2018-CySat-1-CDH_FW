/* CySat printf and friends */

#include <stdarg.h>
#include "printf.h"

/* The static printf() buffer */

static char printf_buffer[PRINTF_BUFFER_SIZE];

/* Static functions */
static int lockBuffer();
static int releaseBuffer();
static int uartPuts(USART_TypeDef uart, char *s);

int uprintf(USART_TypeDef uart, const char *format_string, ...)
{
	if (!lockBuffer()){
		ERROR_ResourceFrozen("Unable to obtain printf buffer mutex");
	}
	
	va_list args;
	va_start(args, format_string);
	
	/* Format into the buffer */
	int retval;
	retval = vsnprintf(printf_buffer, PRINTF_BUFFER_SIZE, format_string, args);
	
	va_end(args);
	
	/* Print the buffer */
	uartPuts(uart, printf_buffer);
	
	if (!releaseBuffer()){
		ERRO_MiscRTOS("Unable to release printf buffer mutex");
	}
	
	return retval;
}

int dbg_printf(const char *format_string, ...)
{
	if (!lockBuffer()){
		ERROR_ResourceFrozen("Unable to obtain printf buffer mutex");
	}
	
	va_list args;
	va_start(args, format_string);
	
	int retval;
	retval = vsnprintf(printf_buffer, PRINTF_BUFFER_SIZE, format_string, args);
	
	va_end(args);
	
	/* Determine which UART to use based on a number of important and highly variable factors */
	USART_TypeDef dbgUart = USART2;
	
	/* Print severity and thread information */
	uartPuts(dbgUart, "[DEBUG] ");
	
	/* Print the buffer */
	uartPuts(dbgUart, printf_buffer);
	
	if (!releaseBuffer()){
		ERRO_MiscRTOS("Unable to release printf buffer mutex");
	}
	
	return retval;
}

static int lockBuffer()
{
	/* No RTOS for now */
	return 1;
}

static int releaseBuffer()
{
	/* No RTOS for now */
	return 1;
}