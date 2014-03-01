#include "clock.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"



long currentTime;
static void prvClockTask();
static void zeroRTC();
static void startRTC();
static void readRTC();

#define CLOCK_ADDR 0xD0

long getCurrentTime() {
    return currentTime;
}

static void prvClockTask() {
    /*Read current time from RTC
      Loop and update time    */

    currentTime = 0;

    vConsolePrintf("ClockTask started\r\n");

    zeroRTC();
    startRTC();
    for(;;) {
        readRTC();
        //vConsolePrintf("Current time: %d\r\n");
        vTaskDelay(1000);
    }
}

static void readRTC(){
    char buffer[8];
    char addr = 0;

    cWireWrite(wireBUS5, CLOCK_ADDR, &addr, 1);
    cWireRead( wireBUS5, CLOCK_ADDR, buffer, 4 );

    vConsolePrintf("Seconds field: %x\r\n", (buffer[2] & 0x00ff));

    int seconds = buffer[1] & 0x0f;

    currentTime = seconds;
}

static void startRTC() {
    char writeBuffer[2] = {0x0c, 0};
    cWireWrite(wireBUS5, CLOCK_ADDR, writeBuffer, 1);
    cWireRead(wireBUS5, CLOCK_ADDR, (writeBuffer + 1), 1);
    writeBuffer[1] = writeBuffer[1] & (~(1 << 6));
    cWireWrite(wireBUS5, CLOCK_ADDR, writeBuffer, 2);
}

static void zeroRTC() {
    char writeBuffer[10] = {0x0c, 0,0,0,0,0,0,0,0,0};
    cWireWrite(wireBUS5, CLOCK_ADDR, writeBuffer, 9);
}

void vStartClockTask() {
    xTaskCreate( prvClockTask, NULL, configMINIMAL_STACK_SIZE + 16, NULL, systemPRIORITY_UART2, NULL );
}