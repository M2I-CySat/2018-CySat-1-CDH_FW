/* serial.h 
 * 
 * USART driver for FreeRTOS
 * 
 * API:
 *  Initialization: serialInit();
 *  GetChar:        xSerialGetChar(USARTx, &byte, blocktime);
 *  PutChar:        xSerialPutChar(USARTx, byte, blocktime);
 * 
 * Get and Put return success/fail
 * 
 */

#ifndef SERIAL_H
#define SERIAL_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"

void serialInit();
int xSerialGetChar(USART_TypeDef, unsigned char *, portTickType);
int xSerialPutChar(USART_TypeDef, unsigned char, portTickType);


#define serialUSART2_BAUDRATE 9600

#endif