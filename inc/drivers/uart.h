/* uart.h
 * 
 * Manages UARTs and holds on to handles
 */
#ifndef __UART_H
#define __UART_H
 
#include <stdint.h>
#include <unistd.h>

enum UART_Uart {
	UART_1,
	UART_2,
	UART_3
};
 

/* Initialize all UARTs. Returns 0 upon success. -1 upon failure. */
int UART_Initialize();

/* UART Transmit Wrapper */
ssize_t UART_Write(enum UART_Uart usart, uint8_t * data, uint16_t size);

/* Get the debug UART */
enum UART_Uart UART_GetDebug(); 

#endif
