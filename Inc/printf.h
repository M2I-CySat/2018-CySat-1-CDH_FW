/* CySat printf() and friends
 *
 * This printf() implementation
 * is RTOS-friendly and easy to 
 * use for debug purposes.
 */
 
#ifndef __PRINTF_H
#define __PRINTF_H

#include <drivers/uart.h>
#include <stdarg.h>
#include <cmsis_os.h>
#include <stdint.h>
#include <stdlib.h>

/* Initialization Function */
int PRINTF_Initialize();

/* Call printf to a specific uart */
int uprintf(USART_TypeDef *uart, const char *format_string, ...);

/* Same but with args list */
int vuprintf(USART_TypeDef *uart, const char *format_string, va_list args);

/* Wraps uprintf() and prints to the debug UART with thread information if available */
int dbg_printf(const char *format_string, ...);

/* Define a thread ID -> Name mapping for debug info */
size_t dbg_add_thread(osThreadId id, const char * name);

/* Retrieve a mapped thread name */
const char * dbg_thread_name(osThreadId id);

/* UART equivalent of fputs */
int uputs(const char * s, USART_TypeDef *uart);

/* One shared buffer is used for all printf's across all tasks.
* This buffer is only used for formatting; it is atomically passed to
* the UART drivers.
*/
#define PRINTF_BUFFER_SIZE 128

#endif