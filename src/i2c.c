/*
 * File:   i2c.c
 * Author: Jake Drahos
 *
 *
 * Created on November 11, 2013, 5:43 PM
 */

#include <i2c.h>
#include <uart.h>
#include <system.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_rcc.h>

static DMA_InitTypeDef DMA_InitStructure;

#define DIR_TX 1
#define DIR_RX 2
void dmaConfig(uint32_t buffer, uint32_t bufferSize, uint8_t dir,
                      DMA_Stream_TypeDef * stream)
{
  if (dir == DIR_TX)
  {
    /* Configure the DMA Tx Stream with the buffer address and the buffer size */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;    
    DMA_InitStructure.DMA_BufferSize = (uint32_t)bufferSize;  
    DMA_Init(stream, &DMA_InitStructure);  
  }
  else
  { 
    /* Configure the DMA Rx Stream with the buffer address and the buffer size */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)bufferSize;      
    DMA_Init(stream, &DMA_InitStructure);    
  }
}

/* I2C1: Unknown GPIOs, DMA1 Stream 5 Channel 1(RX)/Stream 6 Channel 1(TX) */
static void initializeI2C1()
{
  I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
  
  I2C_StructInit(&I2C_InitStructure);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x34;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;

  I2C_Cmd(I2C1, ENABLE);
  I2C_Init(I2C1, &I2C_InitStructure);
  
  /* Initialize I2C1 on PB8 and PB9 */
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Initialize I2C1 DMA1 (S5C1/S6C1)*/
  DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 |
                              DMA_FLAG_HTIF5 | DMA_FLAG_TCIF5);
  DMA_Cmd(DMA1_Stream5, DISABLE);
  DMA_DeInit(DMA1_Stream5);
  DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(I2C1->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;    /* This parameter will be configured durig communication */;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; /* This parameter will be configured durig communication */
  DMA_InitStructure.DMA_BufferSize = 0xFFFF;              /* This parameter will be configured durig communication */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream5, &DMA_InitStructure);
  
  DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_FEIF6 | DMA_FLAG_DMEIF6 | DMA_FLAG_TEIF6 |
                              DMA_FLAG_HTIF6 | DMA_FLAG_TCIF6);
  DMA_Cmd(DMA1_Stream6, DISABLE);
  
  DMA_DeInit(DMA1_Stream6);
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);
  
}

void initializeI2C()
{
  initializeI2C1();
}