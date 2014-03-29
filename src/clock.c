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


    vConsolePrintf("ClockTask started\r\n");

    startRTC();
    readRTC();
    int i = 0;
    for(;;) {
        //vConsolePrintf("Current time: %d\r\n");
        vTaskDelay(1000);
        currentTime++;
        if (i == 120) {
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

void zeroRTC() {
    char writeBuffer[10] = {0x0c, 0,0,0,0,0,0,0,0,0};
    cWireWrite(wireBUS5, CLOCK_ADDR, writeBuffer, 9);
    currentTime = 0;
}

void vStartClockTask() {
    xTaskCreate( prvClockTask, NULL, configMINIMAL_STACK_SIZE + 16, NULL, systemPRIORITY_UART2, NULL );
}