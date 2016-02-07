/* uart.c
 *
 * Does initialization and holds UART handles
 */

#include <uart.h>
#include <error.h>
 
enum {
	INDEX_USART1,
	INDEX_USART2,
	INDEX_USART6
};

UART_HandleTypeDef handles[3];
int initialized[3];

static void MspInitUART2();

static void MspDeInitUART2();


UART_HandleTypeDef * UART_GetHandle(USART_TypeDef * usart)
{
	if (usart == USART1) {
		if (!initialized[INDEX_USART1]) {
			ERROR_Uninitialized("Usart 1 not initialized.");
		}
		return &handles[INDEX_USART1];
	} else if (usart == USART2) {
		if (!initialized[INDEX_USART2]) {
			ERROR_Uninitialized("Usart 2 not initialized.");
		}
		return &handles[INDEX_USART2];
	} else if (usart == USART6) {
		if (!initialized[INDEX_USART6]) {
			ERROR_Uninitialized("Usart 6 not initialized.");
		}
		return &handles[INDEX_USART6];
	} else {
		return NULL;
	}
}

USART_TypeDef * UART_GetDebug()
{
	/* Determine the debug uart */
	return USART2;
}

int UART_Initialize()
{
	/* UART2 configured as follow:
	  - Word Length = 8 Bits
	  - Stop Bit = One Stop bit
	  - Parity = Disabled
	  - BaudRate = 38400 baud
	  - Hardware flow control disabled (RTS and CTS signals) */
	handles[INDEX_USART2].Instance          = USART2;

	handles[INDEX_USART2].Init.BaudRate     = 38400;
	handles[INDEX_USART2].Init.WordLength   = UART_WORDLENGTH_8B;
	handles[INDEX_USART2].Init.StopBits     = UART_STOPBITS_1;
	handles[INDEX_USART2].Init.Parity       = UART_PARITY_NONE;
	handles[INDEX_USART2].Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	handles[INDEX_USART2].Init.Mode         = UART_MODE_TX_RX;
	handles[INDEX_USART2].Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(&handles[INDEX_USART2]) != HAL_OK) {
		return -1;
	}
	
	initialized[INDEX_USART2] = 1;
	
	return 0;
}

ssize_t UART_Write(USART_TypeDef *uart, uint8_t * data, uint16_t size)
{
	UART_HandleTypeDef *uartHandle;
	uartHandle = UART_GetHandle(uart);
	
	HAL_UART_Transmit(uartHandle, data, size, 0xFFFF);
	
	return size;
}

/*------------- MSP Initialization -------------------
 * Configure clock enable
 * Configure GPIO
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if (huart == &handles[INDEX_USART1]) {
		ERROR_NotImplemented("USART1 not implemented");
	} else if (huart == &handles[INDEX_USART2]) {
		MspInitUART2();
	} else if (huart == &handles[INDEX_USART6]) {
		ERROR_NotImplemented("USART6 not implemented");
	}
}

static void MspInitUART2()
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USART2_TX_GPIO_CLK_ENABLE();
  USART2_RX_GPIO_CLK_ENABLE();
  
  /* Enable USART2 clock */
  USART2_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART2_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USART2_TX_AF;
  
  HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USART2_RX_PIN;
  GPIO_InitStruct.Alternate = USART2_RX_AF;
    
  HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);
}

/* Disable the Peripheral's clock
 * Revert GPIO and NVIC configuration to their default state
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	if (huart == &handles[INDEX_USART1]) {
		ERROR_NotImplemented("USART1 not implemented");
	} else if (huart == &handles[INDEX_USART2]) {
		MspDeInitUART2();
	} else if (huart == &handles[INDEX_USART6]) {
		ERROR_NotImplemented("USART6 not implemented");
	}
}

static void MspDeInitUART2()
{
  /*##-1- Reset peripherals ##################################################*/
  USART2_FORCE_RESET();
  USART2_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USART2_TX_GPIO_PORT, USART2_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USART2_RX_GPIO_PORT, USART2_RX_PIN);

}
