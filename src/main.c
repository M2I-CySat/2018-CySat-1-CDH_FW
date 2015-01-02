
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
/* Application includes */

/*Init system config*/

#define BURN_DELAY          15 /*Seconds until antenna burn*/


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
    
    char i2cbuffer[10];
    
    for(;;)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);
        
        /* Test I2C */
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {} /*TODO: Timeout*/
        I2C_GenerateSTART(I2C1, ENABLE);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
        }
        I2C_Send7bitAddress(I2C1, 0x3C, I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
        
        I2C_SendData(I2C1, 10);
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET) {}
        
        
        I2C_GenerateSTART(I2C1, ENABLE);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}
        I2C_Send7bitAddress(I2C1, 0x3C, I2C_Direction_Receiver);
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET) {}
        
        I2C_AcknowledgeConfig(I2C1, DISABLE);
        
        /*voodoo read */
        (void)I2C1->SR2;
        I2C_GenerateSTOP(I2C1, ENABLE);
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET) {}
        i2cbuffer[0] = I2C_ReceiveData(I2C1);
        while(I2C1->CR1 & I2C_CR1_STOP) {}
        
        I2C_AcknowledgeConfig(I2C1, ENABLE);
        
        vConsolePrintf("I2C Test Byte: %x\r\n", i2cbuffer[0]);
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
