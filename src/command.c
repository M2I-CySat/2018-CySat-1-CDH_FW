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
#include <system.h>
#include <string.h>

#define uartRX_BLOCK_TIME   ( ( portTickType ) 0xffff )
#define MAX_COMMAND_LENGTH 100
#define MAX_QUERY_SUBTYPE_LENGTH 12
#define MAX_TYPE_LENGTH 12

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

static void prvDoQueryTime() {
        vConsolePrintf("!RESULT,TIME,It's been many seconds since 1970,C2D3$");
}

static void prvDoQueryMtime() {
        vConsolePrintf("!RESULT,MTIME,Long time in space...,E4F5$");
}

static void prvDoQueryHello() {
        vConsolePrintf("!RESULT,HELLO,Hello World,A0B1$");
}

static void prvConsoleHandleQuery(char * fields[], int fieldCount) {
        if((strncmp("HELLO", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) && (fieldCount == 3))
		prvDoQueryHello();
        if((strncmp("TIME", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) && (fieldCount == 3))
		prvDoQueryTime();
        if((strncmp("MTIME", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) && (fieldCount == 3))
		prvDoQueryMtime();
}

static void prvConsoleHandleCommand(char * command) {
	if (!command[0] == '!') return; //invalid command

	//VALIDATE CHECKSUM HERE -- ANY LATER AND IT WILL GET TRASHED BY PARSING


	int fieldCount = 1;
	int currentField = 0;
	int len = strlen(command);
	int i;
	for(i = 0; i < len; i++) {
		if (command[i] == ',') fieldCount++;
	}

	if (fieldCount < 3) return; //invalid command

	char * fields[fieldCount]; 	//pointer to beginning of each field

	fields[currentField] = command + 1; //first field begins at command[1]

	for(i = 0; i < len; i++) {
		if (command[i] == ',') {
			command[i] = 0; //set the , to an ascii null to let strcmp work
			fields[++currentField] = ((command + i) + 1); //the next field begins at comma + 1
		}
	}

	command[len - 1] = 0;  	// replace stop character with ascii null.
		// The command buffer now holds just \0 separated fields.
                //Each field is pointed to by fields[]

	if ((strncmp("QUERY", fields[0], MAX_TYPE_LENGTH) == 0) && (fieldCount >= 3))
		prvConsoleHandleQuery(fields, fieldCount);

}

static char rxByte() {
	signed char byte = 0;

	while (!xSerialGetChar(xUart1Handle, &byte, uartRX_BLOCK_TIME));
	return byte;
}

#define UART1_ECHO

static void vUart1RXTask( void * params) {
	char commandBuffer[MAX_COMMAND_LENGTH];
	memset(commandBuffer, 0, MAX_COMMAND_LENGTH);

        vConsolePrintf("Console RX Task Started!\r\n");

	int byteIndex = 0;
	char byte;

	for (;;) {
		byte = rxByte();

#ifdef UART1_ECHO
                vConsolePut(byte);
#endif

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
}

void xStartUart1CommandHandling() {
    xTaskCreate( vUart1RXTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_UART1, NULL );
}
