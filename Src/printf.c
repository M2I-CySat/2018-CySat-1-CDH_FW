/* CySat printf and friends */

#include <stdint.h>
#include <string.h>

#include <printf.h>
#include <error.h>

/* Eventually move this functionality to a uart.h */
#include <main.h>

/* The static printf() buffer */

static char printf_buffer[PRINTF_BUFFER_SIZE];

/* Static functions */
static int lockBuffer();
static int releaseBuffer();
static void uartPuts(USART_TypeDef *uart, char *s);

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
	/* Determine which UART to use based on a number of important and highly variable factors */
	USART_TypeDef *dbgUart = USART2;
	
	/* Print severity and thread information */
	uartPuts(dbgUart, "[DEBUG] ");
	
	/* Print the buffer */
	uartPuts(dbgUart, printf_buffer);
	
	va_list args;
	va_start(args, format_string);
	
	int retval;
	retval = vuprintf(dbgUart, format_string, args);
	
	va_end(args);

	
	return retval;
}

int vuprintf(USART_TypeDef *uart, const char *format_string, va_list args)
{
	if (!lockBuffer()){
		ERROR_ResourceFrozen("Unable to obtain printf buffer mutex");
	}
	
	/* Format into the buffer */
	int retval;
	retval = vsnprintf(printf_buffer, PRINTF_BUFFER_SIZE, format_string, args);
	
	/* Print the buffer */
	uartPuts(uart, printf_buffer);
	
	if (!releaseBuffer()){
		ERROR_MiscRTOS("Unable to release printf buffer mutex");
	}
	
	return retval;
}

static void uartPuts(USART_TypeDef *uart, char *s)
{
	UART_HandleTypeDef *uartHandle;
	uartHandle = UART_GetHandle(uart);
	
	HAL_UART_Transmit(uartHandle, (uint8_t *)s, strlen(s), 0xFFFF);
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