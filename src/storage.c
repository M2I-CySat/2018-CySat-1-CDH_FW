/*
 * File:   storage.c
 * Author: Jake Drahos
 *
 * Created on September 20, 2014, 1:22 PM
 *
 * Contains abstract data storage routines. This includes the data heap
 * as well as config storage. Once the data storage task is started,
 * it is illegal to directly call the mem read/write functions from any other
 * task.
 */

#include "storage.h"
#include "queue.h"
#include "task.h"
#include "system.h"
#include "mem.h"
#include "serial.h"
#include "uart.h"

#define STORAGE_STACK_SIZE (configMINIMAL_STACK_SIZE + 256)

static xTaskHandle driverTask;
static xQueueHandle messageQueue;
static storageDriverCommand messageBuffer;

static void prvStorageTask();
static void prvDoRead(unsigned char *, unsigned int, unsigned int, unsigned char *);
static void prvDoWrite(unsigned char *, unsigned int, unsigned int, unsigned char *);

void startStorageDriverTask()
{
    messageQueue = xQueueCreate(16, sizeof(storageDriverCommand));
    vConsolePrintf("Starting storage stuff\r\n");
//    xTaskCreate(prvStorageTask, NULL, STORAGE_STACK_SIZE, NULL, systemPRIORITY_STORAGE, &driverTask);
//    xTaskCreate(storageTestTask, NULL, STORAGE_STACK_SIZE, NULL, systemPRIORITY_STORAGE + 1, NULL);
}

void storageTestTask()
{
    char * testString = "Successful test";
    char testBuffer[16];
    char flag = pdFALSE;
    memset(testBuffer, 0, 16);

    vConsolePrintf("Storage test task started...\r\n");

    writeConfig(testString, 44, strlen(testString), &flag);

    while (flag == pdFALSE){}
    flag == pdFALSE;

    vConsolePrintf("Reading back...\r\n");
    readConfig(testBuffer, 44, strlen(testString), &flag);

    while (flag == pdFALSE){}

    vConsolePrintf("Flag set to true. Operation complete. Buffer:\r\n%s\r\n", testBuffer);

    for(;;);
}

int sendStorageDriverCommand(
        storageDriverCommandType type,
        unsigned char * operand,
        unsigned int offset,
        unsigned int length,
        unsigned char * flag)
{
    storageDriverCommand message;
    message.command = type;
    message.operand = operand;
    message.offset = offset;
    message.length = length;
    message.completed = flag;
    return xQueueSend(messageQueue, &message, portMAX_DELAY);
}

static void prvStorageTask()
{
    for(;;)
    {
        if(xQueueReceive(messageQueue, &messageBuffer, portMAX_DELAY) == pdTRUE)
        {
            switch (messageBuffer.command) {
                case(READ_CONFIG):
                    prvDoRead(messageBuffer.operand, messageBuffer.offset, messageBuffer.length, messageBuffer.completed);
                    break;
                case(WRITE_CONFIG):
                    prvDoWrite(messageBuffer.operand, messageBuffer.offset, messageBuffer.length, messageBuffer.completed);
                    break;
            }
        }
    }
}

static void prvDoRead(unsigned char * dst, unsigned int offset, unsigned int length, unsigned char * completed)
{
    vFRAMRead(offset, length, dst);
    *completed = pdTRUE;
}
static void prvDoWrite(unsigned char * src, unsigned int offset, unsigned int length, unsigned char * completed)
{
    vFRAMWrite(offset, length, src);
    *completed = pdTRUE;
}