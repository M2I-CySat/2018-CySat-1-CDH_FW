
/* Standard includes. */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"

#include <system.h>
#include <stdio.h>

/* Hardware Includes */
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

#include <i2c.h>
#include <spi.h>
/* Application includes */

/*Init system config*/

#define BURN_DELAY          5 /*Seconds until antenna burn*/


static void initTask(void * params);
static void init();

static void lowLevelHardwareInit()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
}

static void initializeBackupRegisters()
{  
    int i;
    
    bitSet(PWR->CR, PWR_CR_DBP);   
    for (i = 0; i < 2; i++) {} /* Delay for bus write */ 
   
    /* RTC_BKP0R (Status Flags)
      Bit 0: Antenna Status (0 undeployed, 1 deployed)
    */
    RTC->BKP0R = 0x00000000;
    
    bitClear(PWR->CR, PWR_CR_DBP);
}

int main( void )
{
    lowLevelHardwareInit();
    xTaskCreate(initTask, NULL, systemDEFAULT_STACK_SIZE, NULL, systemPRIORITY_INIT, NULL);
    vTaskStartScheduler();

    /* Will only reach here if there is insufficient heap available to start
    the scheduler. */
    return 0;
}

void initTask(void * params)
{
    /* Test Heartbeat Initialization */
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* End test heartbeat initialization */
    
    /* Begin actual init */
    unsigned char buffer;
    int i;
    
    vUartStartTask();
    vConsolePrintf("\r\n\r\n==================== BOOT ====================\r\n");
    vConsolePrintf("Init task started\r\n");

    vConsolePrintf("Checking RTC initialization status...\r\n");
    if (!(RTC->ISR & RTC_ISR_INITS)) { /*This is first boot*/
        vConsolePrintf("RTC Not Initialized. Performing first boot routine.\r\n");
        vConsolePrintf("Initializing RTC...");
        startRTC();
        vConsolePrintf("Done.\r\n");
        vConsolePrintf("Delaying for RTC synchronization...");
        vTaskDelay(2500);
        vConsolePrintf("Done\r\n");

        vConsolePrintf("Initializing Backup Register...");
        initializeBackupRegisters();
        vConsolePrintf("Done\r\n");
        
        vConsolePrintf("First boot routine complete.\r\n");
    }
    else
    {
        vConsolePrintf("RTC already initialized.\r\n");
    }

    char timeBuffer[15];
    sprintf(timeBuffer, "Mission Time: %ld\r\n", getMissionTime());
    vConsolePrintf(timeBuffer);
    vConsolePrintf("Waiting for deployment delay...\r\n");

    while(getMissionTime() <= BURN_DELAY) {
        vTaskDelay(500);
    }
    
    vConsolePrintf("Deployment delay reached - checking status...\r\n");
    if (!(RTC->BKP0R & ANTENNA_STATUS)) {
        vConsolePrintf("Deploying Antennas...");
//TODO: Call antenna deployment function
        vConsolePrintf("Done\r\n");
        
        vConsolePrintf("Setting antenna status flag...");
        bitSet(PWR->CR, PWR_CR_DBP);     
        for (i = 0; i < 2; i++) {} /* Delay for bus write */ 
        bitSet(RTC->BKP0R, ANTENNA_STATUS);
        bitSet(PWR->CR, PWR_CR_DBP);        
        vConsolePrintf("Done\r\n");
    }
    else
    {
      vConsolePrintf("Antennas already deployed!\r\n");
    }

    vConsolePrintf("Starting command handling on console...");
    xStartUart1CommandHandling();
    vConsolePrintf("Done\r\n");

//    vConsolePrintf("Starting storage driver task\r\n");
//TODO: Port    startStorageDriverTask();
    
    vConsolePrintf("Init finished!\r\n");
    
    initializeI2C();
    initializeSPI();
    
    uint8_t i2cbuffer[1000];
    uint8_t spitxbuffer[100];
    uint8_t spirxbuffer[100];
    
    for(;;)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);
        
        /* I2C test */
        I2C1_TakeMutex(portMAX_DELAY);
        i2cbuffer[0] = 10;
        I2C1Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        I2C1Read(i2cbuffer, 0x3C, 3, portMAX_DELAY);
        
        vConsolePrintf("I2C Test Bytes: %x %x %x\r\n", i2cbuffer[0], i2cbuffer[1], i2cbuffer[2]);
        
        i2cbuffer[0] = 0x00;
        I2C1Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        I2C1Read(i2cbuffer, 0x3C, 3, portMAX_DELAY);
        vConsolePrintf("I2C Pre-Write Bytes: %x %x %x\r\n", i2cbuffer[0], i2cbuffer[1], i2cbuffer[2]);
        
        i2cbuffer[0] = 0x02;
        I2C1Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        
        i2cbuffer[1] = 0x00;
        I2C1Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        
        i2cbuffer[0] = 0x00;
        I2C1Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
        I2C1Read(i2cbuffer, 0x3C, 3, portMAX_DELAY);
        
        I2C1_ReleaseMutex();
        
        vConsolePrintf("I2C Readback Bytes: %x %x %x\r\n", i2cbuffer[0], i2cbuffer[1], i2cbuffer[2]);
       /* 
        vConsolePrintf("Beginning long read. (100 1KB reads).  Should take 8 seconds\r\n");
        for(i = 0; i < 100; i++)
        {
            I2C1Write(i2cbuffer, 0x3C, 1, portMAX_DELAY);
            I2C1Read(i2cbuffer, 0x3c, 1000, portMAX_DELAY);
        }
        vConsolePrintf("Long read done");
        */
       
        vConsolePrintf("Testing SPI transfer...");
        SPI1_TakeMutex();
        SPI1Transfer(spitxbuffer, spirxbuffer, 100);
        SPI1_ReleaseMutex();
        vConsolePrintf("Done\r\n");
    }
}

void vApplicationStackOverflowHook()
{
  for(;;) {}
}

void vApplicationMallocFailedHook()
{
  for(;;) {}
}
