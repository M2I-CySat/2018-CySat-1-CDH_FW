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

#include <string.h>
#include <stdint.h>

#define STORAGE_STACK_SIZE (configMINIMAL_STACK_SIZE + 256)

static xTaskHandle driverTask;
static xQueueHandle messageQueue;
static storageDriverCommand messageBuffer;

/* Used for swapping or other operations. Restrict to Task thread.
 * Made static to avoid needing a massive heap
 * */
static unsigned char heapBuffer1 [FOOTPRINT_DATA_SIZE];
static unsigned char heapBuffer2 [FOOTPRINT_DATA_SIZE];

static unsigned int heapSize = 0;

static void prvStorageTask();
static void prvDoRead(unsigned char *, unsigned int, unsigned int, unsigned char *);
static void prvDoWrite(unsigned char *, unsigned int, unsigned int, unsigned char *);

static void heapInsert(unsigned char *);
static void heapPop(unsigned char *);

/* These use heap indices */
/* Returns 1 if a comes before b, -1 if b comes before a, zero if equal*/
static int compareFootprintTimestamps(unsigned int, unsigned int);
static void swapFootprints(unsigned int, unsigned int);
static void upHeap(unsigned int);
static void downHeap(unsigned int);

/* Use raw offsets */
static uint32_t getFootprintTimestamp(unsigned int);

static void upHeap(unsigned int cursor)
{
    if (cursor == 0)
        return;

    register unsigned int parent = (cursor - 1) / 2;

    if (compareFootprintTimestamps(parent, cursor) == -1)
    {
        swapFootprints(parent, cursor);
        upHeap(parent);
    }
}
static void heapInsert(unsigned char * fp)
{
    unsigned int cursor = 0;
    vFRAMWrite(HEAP_BEGIN + (heapSize * FOOTPRINT_DATA_SIZE), FOOTPRINT_DATA_SIZE, fp);
    cursor = heapSize++;

    upHeap(cursor);
}

static void downHeap(unsigned int cursor)
{
    register unsigned int l, r;
    l = (cursor * 2) + 1;
    r = (cursor * 2) + 2;

    if ((l >= heapSize) || (r >= heapSize))
        return;

    register int cmp_l, cmp_r, cmp_c;
    cmp_l = compareFootprintTimestamps(cursor, l);
    cmp_r = compareFootprintTimestamps(cursor, r);

    if ((cmp_l != -1) && (cmp_r != -1))
        return; /* Cursor goes before both children. Heap is good */

    cmp_c = compareFootprintTimestamps(l, r);
    if (cmp_c == 1) /* Left goes before right */
    {
        swapFootprints(l, cursor); /* Continue with left */
        downHeap(l);
    }
    else
    {
        swapFootprints(r, cursor); /* Continue with right*/
        downHeap(r);
    }
}
static void heapPop(unsigned char * fp)
{
    unsigned int cursor = 0;
    vFRAMRead(HEAP_BEGIN, FOOTPRINT_DATA_SIZE, fp);

    swapFootprints(cursor, --heapSize);
    downHeap(cursor);
}

static uint32_t getFootprintTimestamp(unsigned int offest)
{
    uint32_t timestamp = 0;
    unsigned char buffer[4] = {0,0,0,0};
    
    vFRAMRead(offest, 4, buffer);
    timestamp |= buffer[3];
    timestamp |= (buffer[2] << 8);
    timestamp |= (buffer[1] << 16);
    timestamp |= (buffer[0] << 24);
    
    return timestamp;
}

static int compareFootprintTimestamps(unsigned int a, unsigned int b)
{
    uint32_t timestampA = getFootprintTimestamp((a * FOOTPRINT_DATA_SIZE) + HEAP_BEGIN);
    uint32_t timestampB = getFootprintTimestamp(b * FOOTPRINT_DATA_SIZE + HEAP_BEGIN);

    if (timestampA == timestampB)
        return 0;
    else if (timestampA < timestampB)
        return 1;
    else if (timestampA > timestampB)
        return -1;
    else
        return 0;
}

static void swapFootprints(unsigned int a, unsigned int b)
{
    unsigned int offsetA, offsetB;
    offsetA = (a * FOOTPRINT_DATA_SIZE) + HEAP_BEGIN;
    offsetB = (b * FOOTPRINT_DATA_SIZE) + HEAP_BEGIN;

    vFRAMRead(offsetA, FOOTPRINT_DATA_SIZE, heapBuffer1);
    vFRAMRead(offsetB, FOOTPRINT_DATA_SIZE, heapBuffer2);

    vFRAMWrite(offsetA, FOOTPRINT_DATA_SIZE, heapBuffer2);
    vFRAMWrite(offsetB, FOOTPRINT_DATA_SIZE, heapBuffer1);
}

void startStorageDriverTask()
{
    messageQueue = xQueueCreate(16, sizeof(storageDriverCommand));
    vConsolePrintf("Starting storage driver...");
    xTaskCreate(prvStorageTask, NULL, STORAGE_STACK_SIZE, NULL, systemPRIORITY_STORAGE, &driverTask);
    xTaskCreate(storageTestTask, NULL, STORAGE_STACK_SIZE - 200, NULL, systemPRIORITY_TEST, NULL);
}

void storageTestTask()
{
    char * testString = "Successful sanity check";
    char testBuffer[32];
    volatile char flag = pdFALSE;
    vConsolePrintf("Storage test task started...\r\n");
    
    memset(testBuffer, 0, 16);

    flag = pdFALSE;
    writeConfig(testString, 0x20, strlen(testString), &flag);
    while (flag == pdFALSE){}

    vConsolePrintf("Reading back...\r\n");

    flag = pdFALSE;
    readConfig(testBuffer, 0x40, strlen(testString), &flag);
    while (flag == pdFALSE){}

    vConsolePrintf("Flag set to true. Operation complete. Buffer: '%s'\r\n", testBuffer);

    for(;;) {
        vTaskDelay(10000);
    }
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
    *completed = 7;
}
static void prvDoWrite(unsigned char * src, unsigned int offset, unsigned int length, unsigned char * completed)
{
    vFRAMWrite(offset, length, src);
    *completed = pdTRUE;
}
