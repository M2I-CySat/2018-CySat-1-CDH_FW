/* CySat printf and friends */

#include <stdint.h>
#include <string.h>

#include <printf.h>
#include <error.h>

#include <drivers/uart.h>

/* The static printf() buffer */

static char printf_buffer[PRINTF_BUFFER_SIZE];

/* Static functions */
static int lockBuffer();
static int releaseBuffer();

int uprintf(USART_TypeDef *uart, const char *format_string, ...)
{
	va_list args;
	va_start(args, format_string);
	
	/* Format into the buffer */
	int retval;
	retval = vuprintf(uart, format_string, args);
	
	va_end(args);
	
	return retval;
}

int dbg_printf(const char *format_string, ...)
{
	USART_TypeDef * dbgUart = UART_GetDebug();
	
	/* Print severity and thread information */
	uputs("[DEBUG] ", dbgUart);
	
	va_list args;
	va_start(args, format_string);
	
	int retval;
	retval = vuprintf(dbgUart, format_string, args);
	
	va_end(args);
	
	uputs("\r\n", dbgUart);

	
	return retval;
}

int vuprintf(USART_TypeDef *uart, const char *format_string, va_list args)
{
	if (!lockBuffer()){
		ERROR_ResourceFrozen("Unable to obtain printf buffer mutex");
	}
	
	/* Format into the buffer */
	vsnprintf(printf_buffer, PRINTF_BUFFER_SIZE, format_string, args);
	
	/* Print the buffer */
	int retval;
	retval = uputs(printf_buffer, uart);
	
	if (!releaseBuffer()){
		ERROR_MiscRTOS("Unable to release printf buffer mutex");
	}
	
	return retval;
}

int uputs(const char * s, USART_TypeDef *uart)
{	
	return UART_Write(uart, (uint8_t *)s, strlen(s));
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