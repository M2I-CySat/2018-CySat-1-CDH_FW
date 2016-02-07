/* uart.h
 * 
 * Manages UARTs and holds on to handles
 */
 
#include <stm32f4xx_hal.h>
 
UART_HandleTypeDef * UART_GetHandle(USART_TypeDef * usart);

/* Initialize all UARTs. Returns 0 upon success. -1 upon failure. */
int UART_Initialize();

/* Get the debug UART */
USART_TypeDef * UART_GetDebug(); 

/* Get handle for debug UART */
inline UART_HandleTypeDef * UART_GetDebugHandle()
{
	return UART_GetHandle(UART_GetDebug());
}