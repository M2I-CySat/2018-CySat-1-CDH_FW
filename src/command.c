/*
 * command.c
 *
 *  Created on: Dec 28, 2013
 *      Author: jake
 */



#include "uart.h"
#include "serial.h"

#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>
#include <croutine.h>

#include <command.h>

#include <stdio.h>
#include <string.h>

#define uartRX_BLOCK_TIME   ( ( portTickType ) 0xffff )
#define MAX_COMMAND_LENGTH 100

/* Command Format
 *
 * For now, commands are formatted as follows:
 *
 * '!' is the start character, and '$' is the end character
 *
 *
 * Several cases can occur:
 *  - MAX_COMMAND_LENGTH exceeded - clear array, byteIndex = 0, store character
 *  - ! - clear array, byteIndex = 0, store character
 *  - $ - Store character, Handle the command
 *  - any other character - store and increment index
 *
 *  In this way, it will always be storing strings since the last start character. If
 *  command[0] is not ! (i.e. an overflow occurred some time or whatever) the command is invalid and
 *  will be rejected by HandleCommand
 *
 */

static void prvConsoleHandleCommand(char * command) {
	vConsolePrintf("Command received: %s\r\n", command);
}

static char rxByte() {
	char byte = 0;

	while (!xSerialGetChar(xUart2Handle, &byte, uartRX_BLOCK_TIME));
	return byte;
}

static void vUart1RXTask( void * params) {
	char commandBuffer[MAX_COMMAND_LENGTH];
	memset(commandBuffer, 0, MAX_COMMAND_LENGTH);

	int byteIndex = 0;
	char byte;

	for (;;) {
		byte = rxByte();
		if ((byteIndex == MAX_COMMAND_LENGTH) || (byte == '!')) {
			byteIndex = 0;
			memset(commandBuffer, 0, MAX_COMMAND_LENGTH);
		}

		commandBuffer[byteIndex] = byte;
		byteIndex ++;

		if (byte == '$') {
			prvConsoleHandleCommand(commandBuffer);
		}
	}
/*	for(;;) {
		if (xSerialGetChar(xUart2Handle, &rxByte, uartRX_BLOCK_TIME)) {
			commandBuffer[i] = rxByte;
			i++;
			if (rxByte == '\n') {
				prvConsoleHandleCommand(commandBuffer);
				memset(commandBuffer, 0, 100);
				i = 0;
			}
		}
	} */
}

void xStartUart1CommandHandling() {
    xTaskCreate( vUart1RXTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_UART1, NULL );
}
