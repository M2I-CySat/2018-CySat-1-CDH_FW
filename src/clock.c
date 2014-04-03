#include "clock.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"



long currentTime;
static void prvClockTask();
static void startRTC();
static void readRTC();

#define CLOCK_ADDR 0xD0

long getMissionTime() {
    return currentTime;
}

static void prvClockTask() {
    /*Read current time from RTC
      Loop and update time    */

    startRTC();
    readRTC();
    int i = 0;
    for(;;) {
        //vConsolePrintf("Current time: %d\r\n");
        vTaskDelay(1000);
        currentTime++;
        if (i == 15) {
            readRTC();
            i = 0;
        }
        i++;
    }
}

static void readRTC(){
    char buffer[8];
    char addr = 0;

    cWireWrite(wireBUS5, CLOCK_ADDR, &addr, 1);
    cWireRead( wireBUS5, CLOCK_ADDR, buffer, 4 );

    long seconds = buffer[1] & 0x0f;
    seconds += ((buffer[1] & 0x70) >> 4) * 10;
    seconds += (buffer[2] & 0x0f) * 60;
    seconds += ((buffer[2] & 0x70) >> 4) * 600;
    seconds += (buffer[3] & 0x0f) * 3600;
    seconds += ((buffer[3] & 0x30) >> 4) * 36000;

    currentTime = seconds;
}

static void startRTC() {
    char writeBuffer[2] = {0x0c, 0};
    cWireWrite(wireBUS5, CLOCK_ADDR, writeBuffer, 1);
    cWireRead(wireBUS5, CLOCK_ADDR, (writeBuffer + 1), 1);
    writeBuffer[1] = writeBuffer[1] & (~(1 << 6));
    cWireWrite(wireBUS5, CLOCK_ADDR, writeBuffer, 2);
}

void zeroRTC() {
    char writeBuffer[10] = {0x0c, 0,0,0,0,0,0,0,0,0};
    cWireWrite(wireBUS5, CLOCK_ADDR, writeBuffer, 9);
    cWireWrite(wireBUS5, CLOCK_ADDR, writeBuffer + 1, 8);
    readRTC();
}

void vStartClockTask() {
    xTaskCreate( prvClockTask, NULL, configMINIMAL_STACK_SIZE + 16, NULL, systemPRIORITY_UART2, NULL );
}