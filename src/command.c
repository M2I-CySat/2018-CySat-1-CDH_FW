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
#include <clock.h>
#include "storage.h"
#include <string.h>


#define uartRX_BLOCK_TIME   ( ( portTickType ) 0xffff )
#define MAX_COMMAND_LENGTH 75
#define MAX_QUERY_SUBTYPE_LENGTH 12
#define MAX_TYPE_LENGTH 12

#define UART1_ECHO 1
#define UART2_ECHO 0

#define CHECKSUM_MESSAGES 0
#define CONSOLE_MESSAGES 1
#define RADIO_MESSAGES 0

//Enable usage of non-standard commands for raw memory access
//Very unsafe
#define RAW_MEM 1

/* FUNCTION PROTOTYPES */

static void sendMessage(char *);
static void prvHandleQuery(char **, int);
static void prvHandleCommand(char **, int);
static void prvHandleMessage(char *);
static void prvHandlePowerPanelQuery(char **, int);
static void prvHandlePowerBattQuery(char **, int);


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
//    powerData * housekeeping;

    char resultBuffer[50];
    memset(resultBuffer, 0, 50);
    if (fields[2][0] == 'X') {
//TODO: Port        vPowerPollHousekeepingData();
//TODO: Port         housekeeping = xPowerGetHousekeepingData();
//TODO: Port         sprintf(resultBuffer, "RESULT,POW_PANEL,X,%04X,%04X,%04X",housekeeping->XVoltage, housekeeping->XCurrent0, housekeeping->XCurrent1);
        sendMessage(resultBuffer);
    }
    else if (fields[2][0] == 'Y') {
//TODO: Port         vPowerPollHousekeepingData();
//TODO: Port         housekeeping = xPowerGetHousekeepingData();
//TODO: Port         sprintf(resultBuffer, "RESULT,POW_PANEL,Y,%04X,%04X,%04X", housekeeping->YVoltage, housekeeping->YCurrent0, housekeeping->YCurrent1);
        sendMessage(resultBuffer);
    }
    else if (fields[2][0] == 'Z') {
//TODO: Port         vPowerPollHousekeepingData();
//TODO: Port         housekeeping = xPowerGetHousekeepingData();
//TODO: Port         sprintf(resultBuffer, "RESULT,POW_PANEL,Z,%04X,%04X,%04X", housekeeping->ZVoltage, housekeeping->ZCurrent0, housekeeping->ZCurrent1);
        sendMessage(resultBuffer);
    }
    else
        sendMessage("NACK_ERROR,PARAM,Invalid Axis");
}

void prvHandleTimeQuery() {
    char resultBuffer[20];

    memset(resultBuffer, 0, 20);
    sprintf(resultBuffer, "RESULT,TIME,%ld", getMissionTime());
    sendMessage(resultBuffer);
}

void prvHandlePowerBattQuery(char ** fields, int fieldCount) {
    powerData * housekeeping = NULL;

    char resultBuffer[50];
    memset(resultBuffer, 0, 50);

    if (fields[2][0] == '0') {
//TODO: Port         vPowerPollHousekeepingData();
//TODO: Port         housekeeping = xPowerGetHousekeepingData();

        //housekeeping->Battery0Temperature = 0x02bc;

        char * dir;
        if (housekeeping->Battery0Direction)
            dir = "D";
        else
            dir = "C";

        sprintf(resultBuffer, "RESULT,POW_BATTERY,0,%04X,%04X,%s,%04X",
                housekeeping->Battery0Temperature,
                housekeeping->Battery0Voltage,
                dir,
                housekeeping->Battery0Current);
        sendMessage(resultBuffer);
    }
    else if (fields[2][0] == '1') {
//TODO: Port         vPowerPollHousekeepingData();
//TODO: Port         housekeeping = xPowerGetHousekeepingData();
        char * dir;
        if (housekeeping->Battery1Direction)
            dir = "D";
        else
            dir = "C";

        sprintf(resultBuffer, "RESULT,POW_BATTERY,1,%04X,%04X,%s,%04X",
                housekeeping->Battery1Temperature,
                housekeeping->Battery1Voltage,
                dir,
                housekeeping->Battery1Current);
        sendMessage(resultBuffer);
    }
    else
        sendMessage("NACK_ERROR,PARAM,Invalid battery");
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
    else if (strncmp("POW_BATTERY", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 4)
            prvHandlePowerBattQuery(fields, fieldCount);
        else
            nackLength();
    }
    else if (strncmp("TIME", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3)
            prvHandleTimeQuery();
        else
            nackLength();
    }
    else
        nackSubtype();
}

#if RAW_MEM
static void framRead(char ** fields) {
    unsigned char dataBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
    int length;
    unsigned long addr;
    sscanf(fields[3], "%d", &length);
    sscanf(fields[2], "%ld", &addr);

//TODO: Port     vFRAMRead(addr, length, dataBuffer);

    vConsolePrint(dataBuffer);
    vConsolePrint("\r\n");
}

static void framWrite(char ** fields) {
    int length;
    unsigned long addr;
    sscanf(fields[3], "%d", &length);
    sscanf(fields[2], "%ld", &addr);

//TODO: Port     vFRAMWrite(addr, length, fields[4]);
}
#endif

static void prvHandleCommand(char * fields[], int fieldCount) {
    if(strncmp("BURN", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
//TODO: Port             vNichromeStartTask();
        } else
            nackLength();
    }
    else if(strncmp("REBOOT", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
            vTaskDelay(2000);
//TODO: Port             __asm__ volatile ("reset");
        } else
            nackLength();
    }
    else if(strncmp("SET_FIRSTBOOT_0", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
//TODO: Port             unsigned char buffer = 0;
//TODO: Port             vFRAMWrite(0, 1, &buffer);
        } else
            nackLength();
    }
    else if(strncmp("SET_FIRSTBOOT_1", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
//TODO: Port             unsigned char buffer = 1;
//TODO: Port             vFRAMWrite(0, 1, &buffer);
        } else
            nackLength();
    }
    else if(strncmp("RESET_CLOCK", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
            startRTC();
        } else
            nackLength();
    }
    else if(strncmp("CLEAR_HEAP", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
//TODO: Port             clearHeap();
        } else
            nackLength();
    }
    else if(strncmp("POW_PRINT", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 3) {
            sendMessage("ACK_COMMAND");
//TODO: Port             vPowerPollHousekeepingData();
//TODO: Port             vPowerPrintHousekeepingData();
        }
        else
            nackLength();
    }
#if RAW_MEM
    else if(strncmp("READ", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 5) {
//TODO: Port             framRead(fields);
            sendMessage("ACK_COMMAND");
        }
        else
            nackLength();
    }
    else if(strncmp("WRITE", fields[1], MAX_QUERY_SUBTYPE_LENGTH) == 0) {
        if (fieldCount == 6) {
//TODO: Port             framWrite(fields);
            sendMessage("ACK_COMMAND");
        }
        else
            nackLength();
    }
#endif
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

    while (!xSerialGetChar(USART2, &byte, uartRX_BLOCK_TIME));
    return byte;
}

static inline char rxByte2() {
    signed char byte = 0;

//TODO: Port     while (!xSerialGetChar(xUart2Handle, &byte, uartRX_BLOCK_TIME));
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
    xTaskCreate( vUart1RXTask, NULL, configMINIMAL_STACK_SIZE + 200, NULL, systemPRIORITY_UART1, NULL );
}

void xStartUart2CommandHandling() {
    xTaskCreate( vUart2RXTask, NULL, configMINIMAL_STACK_SIZE + 200, NULL, systemPRIORITY_UART2, NULL );
}
