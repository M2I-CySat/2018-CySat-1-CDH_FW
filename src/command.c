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

#include <power.h>
#include <stdio.h>
#include <system.h>
#include <string.h>
#include <power.h>
#include <helium.h>
#include <nichrome.h>


#define uartRX_BLOCK_TIME   ( ( portTickType ) 0xffff )
#define MAX_COMMAND_LENGTH 75
#define MAX_QUERY_SUBTYPE_LENGTH 12
#define MAX_TYPE_LENGTH 12

#define UART1_ECHO 1
#define UART2_ECHO 0

#define CHECKSUM_MESSAGES 0
#define CONSOLE_MESSAGES 1
#define RADIO_MESSAGES 0


/* FUNCTION PROTOTYPES */

static void sendMessage(char *);
static void prvHandleQuery(char **, int);
static void prvHandleCommand(char **, int);
static void prvHandleMessage(char *);
static void prvHandlePowerPanelQuery(char **, int);


/* FUNCTION BODIES */

static inline void nackType() {
    sendMessage("NACK_ERROR,TYPE");
}

static inline void nackSubtype() {
    sendMessage("NACK_ERROR,SUBTYPE");;
}

static inline void nackLength() {
    sendMessage("NACK_ERROR,LENGTH");
}

static void sendMessage(char * body) {
    char * checksum = "a1";
#if CHECKSUM_MESSAGES
    //calculate an actual checksum
#endif
#if CONSOLE_MESSAGES
    vConsolePrintf("!%s,%s$", body, checksum);
#endif
#if RADIO_MESSAGES
    //TODO Sprintf the message, then heliumSendData
    //vHeliumSendData("!%s,%s$", body, checksum);
#endif
}

/*Message subtype handling*/
void prvHandlePowerPanelQuery(char** fields, int fieldCount) {
    powerData * housekeeping;
    char resultBuffer[50];
    memset(resultBuffer, 0, 50);
    if (fields[2][0] == 'X') {
        vPowerPollHousekeepingData();
        housekeeping = xPowerGetHousekeepingData();
        sprintf(resultBuffer, "RESULT,POW_PANEL,X,%04X,%04X,%04X",housekeeping->XVoltage, housekeeping->XCurrent0, housekeeping->XCurrent1);
        sendMessage(resultBuffer);
    }
    else if (fields[2][0] == 'Y') {
        vPowerPollHousekeepingData();
        housekeeping = xPowerGetHousekeepingData();
        sprintf(resultBuffer, "RESULT,POW_PANEL,Y,%04X,%04X,%04X", housekeeping->YVoltage, housekeeping->YCurrent0, housekeeping->YCurrent1);
        sendMessage(resultBuffer);
    }
    else if (fields[2][0] == 'Z') {
        vPowerPollHousekeepingData();
        housekeeping = xPowerGetHousekeepingData();
        sprintf(resultBuffer, "RESULT,POW_PANEL,Z,%04X,%04X,%04X", housekeeping->ZVoltage, housekeeping->ZCurrent0, housekeeping->ZCurrent1);
        sendMessage(resultBuffer);
    }
    else
        sendMessage("NACK_ERROR,PARAM,Invalid Axis");
}

/*Message Type Handling*/
static void prvHandleQuery(char * fields[], int fieldCount) {
    if(strncmp("HELLO", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3)
            sendMessage("RESULT,HELLO,Hello World");
        else
            nackLength();
    }
    else if (strncmp("POW_PANEL", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 4)
            prvHandlePowerPanelQuery(fields, fieldCount);
        else
            nackLength();
    }
    else
        nackSubtype();
}

static void prvHandleCommand(char * fields[], int fieldCount) {
    if(strncmp("BURN", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
            vNichromeStartTask();
        } else
            nackLength();
    }
    else if(strncmp("POW_PRINT", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
            vPowerPollHousekeepingData();
            vPowerPrintHousekeepingData();
        }
        else
            nackLength();
    }
    else
        nackSubtype();
}

static void prvHandleMessage(char * command) {
    if (command[0] != '!') return; //invalid command

#if CHECKSUM_MESSAGES
    //VALIDATE CHECKSUM HERE -- ANY LATER AND IT WILL GET TRASHED BY PARSING
#endif

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

    if (strncmp("QUERY", fields[0], MAX_TYPE_LENGTH) == 0)
        prvHandleQuery(fields, fieldCount);
    //TODO: THIS MUST BE MADE TO FIT THE PROTOCOL DOCUMENT.
    else if (strncmp("COMMAND", fields[0], MAX_TYPE_LENGTH) == 0)
        prvHandleCommand(fields, fieldCount);
    else
        nackType();
}

static inline char rxByte1() {
    signed char byte = 0;

    while (!xSerialGetChar(xUart1Handle, &byte, uartRX_BLOCK_TIME));
    return byte;
}

static inline char rxByte2() {
    signed char byte = 0;

    while (!xSerialGetChar(xUart2Handle, &byte, uartRX_BLOCK_TIME));
    return byte;
}


static void vUart1RXTask( void * params) {
    char commandBuffer[MAX_COMMAND_LENGTH];
    memset(commandBuffer, 0, MAX_COMMAND_LENGTH);

    int byteIndex = 0;
    char byte;

    for (;;) {
        byte = rxByte1();

#if UART1_ECHO
        vConsolePut(byte);
#endif

        if ((byteIndex == MAX_COMMAND_LENGTH) || (byte == '!')) {
            byteIndex = 0;
            memset(commandBuffer, 0, MAX_COMMAND_LENGTH);
        }

        commandBuffer[byteIndex] = byte;
        byteIndex ++;

        if (byte == '$') {
            prvHandleMessage(commandBuffer);
        }
    }
}
static void vUart2RXTask( void * params) {
    char commandBuffer[MAX_COMMAND_LENGTH];
    memset(commandBuffer, 0, MAX_COMMAND_LENGTH);

    int byteIndex = 0;
    char byte;

    for (;;) {
        byte = rxByte2();

#if UART2_ECHO
        vConsolePut(byte);
#endif

        if ((byteIndex == MAX_COMMAND_LENGTH) || (byte == '!')) {
            byteIndex = 0;
            memset(commandBuffer, 0, MAX_COMMAND_LENGTH);
        }

        commandBuffer[byteIndex] = byte;
        byteIndex ++;

        if (byte == '$') {
            prvHandleMessage(commandBuffer);
        }
    }
}

void xStartUart1CommandHandling() {
    vConsolePrint("Starting command handling on UART1\r\n");
    xTaskCreate( vUart1RXTask, NULL, configMINIMAL_STACK_SIZE + 200, NULL, systemPRIORITY_UART1, NULL );
}

void xStartUart2CommandHandling() {
    vConsolePrint("Starting command handling on UART2\r\n");
    xTaskCreate( vUart2RXTask, NULL, configMINIMAL_STACK_SIZE + 200, NULL, systemPRIORITY_UART2, NULL );
}
