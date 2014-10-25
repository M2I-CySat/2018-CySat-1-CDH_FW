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
static void prvDoHeapInsert(unsigned char *, unsigned char *);
static void prvDoHeapPop(unsigned char *, unsigned char *);

static void heapInsert(unsigned char *);
static void heapPop(unsigned char *);

/* These use heap indices */
/* Returns 1 if a comes before b, -1 if b comes before a, zero if equal*/
static int compareFootprintTimestamps(unsigned int, unsigned int);
static void swapFootprints(unsigned int, unsigned int);
static void upHeap(unsigned int);
static void downHeap(unsigned int);

/* Use raw offsets */
static long getFootprintTimestamp(unsigned int);

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
    unsigned char buffer[4];
    vFRAMWrite(HEAP_BEGIN + (heapSize * FOOTPRINT_DATA_SIZE), FOOTPRINT_DATA_SIZE, fp);
    cursor = heapSize++;

    upHeap(cursor);
    vFRAMRead(HEAP_BEGIN, 4, buffer);
}

static void downHeap(unsigned int cursor)
{
    register unsigned int l, r;
    l = (cursor * 2) + 1;
    r = (cursor * 2) + 2;

    if (r > heapSize) /* No children */
        return;
    else if (r == heapSize) /* Only has one child */
    {
        if (compareFootprintTimestamps(cursor, l) == -1)
            swapFootprints(cursor, l);
        return;
    }
    else
    {
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
}
static void heapPop(unsigned char * fp)
{
    unsigned int cursor = 0;
    vFRAMRead(HEAP_BEGIN, FOOTPRINT_DATA_SIZE, fp);

    swapFootprints(cursor, --heapSize);
    downHeap(cursor);
}

static long getFootprintTimestamp(unsigned int offest)
{
    long timestamp = 0;
    unsigned char buffer[4] = {0,0,0,0};
    vFRAMRead(offest, 4, buffer);

    timestamp += ((unsigned long) buffer[3]);
    timestamp += (((unsigned long) buffer[2]) << 8);
    timestamp += (((unsigned long) buffer[1]) << 16);
    timestamp += (((unsigned long) buffer[0]) << 24);
    
    return timestamp;
}

static int compareFootprintTimestamps(unsigned int a, unsigned int b)
{
    long timestampA = getFootprintTimestamp((a * FOOTPRINT_DATA_SIZE) + HEAP_BEGIN);
    long timestampB = getFootprintTimestamp((b * FOOTPRINT_DATA_SIZE) + HEAP_BEGIN);

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
    char testBuffer[FOOTPRINT_DATA_SIZE];
    char testValues[10] = {5,3,2,8,7,1,9,5,6,4};
    int i;

    vConsolePrintf("Storage sanity check started...\r\n");
    memset(testBuffer, 0, FOOTPRINT_DATA_SIZE);

    /* Direct write sanity check */
    vConsolePrintf("Writing ``%s''\r\n", testString);
    writeConfigBlocking(testString, 0x20, strlen(testString));
    vConsolePrintf("Reading back...\r\n");
    readConfigBlocking(testBuffer, 0x20, strlen(testString));
    vConsolePrintf("Read back: '%s'\r\n\r\n", testBuffer);
    /* End direct write sanity check*/
    
    /* Heap sanity check */
    memset(testBuffer, 0, FOOTPRINT_DATA_SIZE);
    vConsolePrintf("Heap sanity check.\r\nWriting test footprints: ");
    for (i = 0; i < 10; i++)
    {
        testBuffer[3] = testValues[i];
        vConsolePrintf("%d ", testValues[i]);
        pushFootprintBlocking(testBuffer);
    }

    memset(testBuffer, 0, FOOTPRINT_DATA_SIZE);
    vConsolePrintf("\r\nReading test footprints: ");
    for (i = 0; i < 10; i++)
    {
        popFootprintBlocking(testBuffer);
        vConsolePrintf("%d ", testBuffer[3]);
    }
    vConsolePrintf("\r\nHeap sanity check complete\r\n");
    /* End heap sanity check */

    vConsolePrintf("Beginning heap torture test...\r\nPushing Values: ");
    for (i = 1; i <= 100; i++)
    {
        vConsolePrintf("%d ", i);
        testBuffer[3] = i;
        pushFootprintBlocking(testBuffer);
    }
    vConsolePrintf("\r\nPopping Values: ");
    for (i = 1; i <= 100; i++)
    {
        popFootprintBlocking(testBuffer);
        vConsolePrintf("%d ", testBuffer[3]);
    }
    vConsolePrintf("\r\n");

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
                case(PUSH_HEAP):
                    prvDoHeapInsert(messageBuffer.operand, messageBuffer.completed);
                    break;
                case(POP_HEAP):
                    prvDoHeapPop(messageBuffer.operand, messageBuffer.completed);
                    break;
            }
        }
    }
}

static void prvDoRead(unsigned char * dst, unsigned int offset, unsigned int length, unsigned char * completed)
{
    vFRAMRead(offset, length, dst);
    if (completed != 0)
        *completed = 7;
}
static void prvDoWrite(unsigned char * src, unsigned int offset, unsigned int length, unsigned char * completed)
{
    vFRAMWrite(offset, length, src);
    if (completed != 0)
        *completed = pdTRUE;
}
static void prvDoHeapInsert(unsigned char * src, unsigned char * completed)
{
    heapInsert(src);
    if (completed != 0)
        *completed = pdTRUE;
}
static void prvDoHeapPop(unsigned char * dst, unsigned char * completed)
{
    heapPop(dst);
    if (completed != 0)
        *completed = pdTRUE;
}

void pushFootprintBlocking(unsigned char * fp)
{
    unsigned char flag = pdFALSE;
    pushFootprint(fp, &flag);
    while (flag == pdFALSE) {}
}
void popFootprintBlocking(unsigned char * fp)
{
    unsigned char flag = pdFALSE;
    popFootprint(fp, &flag);
    while (flag == pdFALSE) {}
}
void writeConfigBlocking(unsigned char * src, unsigned int offset, unsigned int length)
{
    unsigned char flag = pdFALSE;
    writeConfig(src, offset, length, &flag);
    while (flag == pdFALSE) {}
}
void readConfigBlocking(unsigned char * dst, unsigned int offset, unsigned int length)
{
    unsigned char flag = pdFALSE;
    readConfig(dst, offset, length, &flag);
    while (flag == pdFALSE) {}
}