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