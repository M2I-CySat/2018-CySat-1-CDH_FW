/* uart.h
 * 
 * Manages UARTs and holds on to handles
 */
 
#include <stm32f4xx_hal.h>
#include <stdint.h>
 
UART_HandleTypeDef * UART_GetHandle(USART_TypeDef * usart);

/* Initialize all UARTs. Returns 0 upon success. -1 upon failure. */
int UART_Initialize();

/* UART Transmit Wrapper */
ssize_t UART_Write(USART_TypeDef * usart, uint8_t * data, uint16_t size);

/* Get the debug UART */
USART_TypeDef * UART_GetDebug(); 

/* Get handle for debug UART */
inline UART_HandleTypeDef * UART_GetDebugHandle()
{
	return UART_GetHandle(UART_GetDebug());
}

/*----------------- MSP Configuration --------------------*/
/* USART2 MSP Configuration */
#define USART2_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USART2_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USART2_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USART2_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USART2 Pins */
#define USART2_TX_PIN                    GPIO_PIN_2
#define USART2_TX_GPIO_PORT              GPIOA  
#define USART2_TX_AF                     GPIO_AF7_USART2
#define USART2_RX_PIN                    GPIO_PIN_3
#define USART2_RX_GPIO_PORT              GPIOA 
#define USART2_RX_AF                     GPIO_AF7_USART2