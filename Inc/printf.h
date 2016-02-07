/* CySat printf() and friends
 *
 * This printf() implementation
 * is RTOS-friendly and easy to 
 * use for debug purposes.
 */
 
#ifndef __PRINTF_H
#define __PRINTF_H

#include <stm32f4xx_hal.h>
#include <stdarg.h>

/* Call printf to a specific uart */
int uprintf(USART_TypeDef *uart, const char *format_string, ...);

/* Same but with args list */
int vuprintf(USART_TypeDef *uart, const char *format_string, va_list args);

/* Wraps uprintf() and prints to the debug UART with thread information if available */
int dbg_printf(const char *format_string, ...);

/* One shared buffer is used for all printf's across all tasks.
* This buffer is only used for formatting; it is atomically passed to
* the UART drivers.
*/
#define PRINTF_BUFFER_SIZE 128

#endif