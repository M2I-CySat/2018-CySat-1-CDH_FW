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

static DMA_InitTypeDef I2C1_DMA_InitStructure;

#define DIR_TX 1
#define DIR_RX 2
static void I2C1_DMAConfig(uint32_t buffer, uint32_t bufferSize, uint8_t dir,
                      DMA_Stream_TypeDef * stream)
{
  if (dir == DIR_TX)
  {
    /* Configure the DMA Tx Stream with the buffer address and the buffer size */
    I2C1_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer;
    I2C1_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;    
    I2C1_DMA_InitStructure.DMA_BufferSize = (uint32_t)bufferSize;  
    DMA_Init(stream, &I2C1_DMA_InitStructure);  
  }
  else
  { 
    /* Configure the DMA Rx Stream with the buffer address and the buffer size */
    I2C1_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer;
    I2C1_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    I2C1_DMA_InitStructure.DMA_BufferSize = (uint32_t)bufferSize;      
    DMA_Init(stream, &I2C1_DMA_InitStructure);    
  }
}

static void initializeI2C1()
{
  I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  RCC_AHB1PeriphClockCmd(I2C1_DMA_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHB1PeriphClockCmd(I2C1_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
  
  I2C_StructInit(&I2C_InitStructure);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_OWN_ADDRESS;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;

  I2C_Cmd(I2C1, ENABLE);
  I2C_Init(I2C1, &I2C_InitStructure);
  
  /* Initialize I2C1 on PB8 and PB9 */
  
  GPIO_PinAFConfig(I2C1_SCL_PORT, I2C1_SCL_PIN_SOURCE, GPIO_AF_I2C1);
  GPIO_PinAFConfig(I2C1_SDA_PORT, I2C1_SDA_PIN_SOURCE, GPIO_AF_I2C1);
  
  GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  DMA_ClearFlag(I2C1_DMA_STREAM_RX, I2C1_DMA_RX_FLAG_FEIF | I2C1_DMA_RX_FLAG_DMEIF | I2C1_DMA_RX_FLAG_TEIF |
                              I2C1_DMA_RX_FLAG_HTIF | I2C1_DMA_RX_FLAG_TCIF);
  DMA_Cmd(I2C1_DMA_STREAM_RX, DISABLE);
  DMA_DeInit(I2C1_DMA_STREAM_RX);
  DMA_StructInit(&I2C1_DMA_InitStructure);
  I2C1_DMA_InitStructure.DMA_Channel = I2C1_DMA_CHANNEL_RX;
  I2C1_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(I2C1->DR);
  I2C1_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;    /* This parameter will be configured durig communication */;
  I2C1_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; /* This parameter will be configured durig communication */
  I2C1_DMA_InitStructure.DMA_BufferSize = 0xFFFF;              /* This parameter will be configured durig communication */
  I2C1_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  I2C1_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  I2C1_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  I2C1_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  I2C1_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  I2C1_DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  I2C1_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  I2C1_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  I2C1_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  I2C1_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(I2C1_DMA_STREAM_RX, &I2C1_DMA_InitStructure);
  
  DMA_ClearFlag(I2C1_DMA_STREAM_TX, I2C1_DMA_TX_FLAG_FEIF | I2C1_DMA_TX_FLAG_DMEIF | I2C1_DMA_TX_FLAG_TEIF |
                              I2C1_DMA_TX_FLAG_HTIF | I2C1_DMA_TX_FLAG_TCIF);
  DMA_Cmd(I2C1_DMA_STREAM_TX, DISABLE);
  
  DMA_DeInit(I2C1_DMA_STREAM_TX);
  I2C1_DMA_InitStructure.DMA_Channel = I2C1_DMA_CHANNEL_TX;
  DMA_Init(I2C1_DMA_STREAM_TX, &I2C1_DMA_InitStructure);
  
  /* Setup interrupts */
  
}

void dmaI2C1Write(uint8_t * buffer, uint8_t address, uint16_t length)
{
    uint16_t bytes_sent = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {} /*TODO: Timeout*/
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}
    I2C_Send7bitAddress(I2C1, address, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
    
    if (length < 2)
    {
        while (bytes_sent < length)
        {
            I2C_SendData(I2C1, buffer[bytes_sent]);
            while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET) {}
            bytes_sent++;
        }
        I2C_GenerateSTOP(I2C1, ENABLE);
        while(I2C1->CR1 & I2C_CR1_STOP) {}
    }
    else
    {
        /* TODO: DMA WRITE */
    }
}

void dmaI2C1Read(uint8_t * buffer, uint8_t address, uint16_t length)
{
    uint16_t bytes_received = 0;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) {} /*TODO: Timeout*/
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {}
    I2C_Send7bitAddress(I2C1, address, I2C_Direction_Receiver);
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET) {}
    
    if (length < 2)
    {
        I2C_AcknowledgeConfig(I2C1, DISABLE);
        
        (void)I2C1->SR2;
        I2C_GenerateSTOP(I2C1, ENABLE);
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET) {}
        if (length > 0)
        {
            buffer[bytes_received] = I2C_ReceiveData(I2C1);
        }
        while(I2C1->CR1 & I2C_CR1_STOP) {}
        
        I2C_AcknowledgeConfig(I2C1, ENABLE);
    }
    else
    {
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}
        I2C1_DMAConfig((uint32_t)buffer, length, DIR_RX, I2C1_DMA_STREAM_RX);
        I2C_DMALastTransferCmd(I2C1, ENABLE);
        DMA_Cmd(I2C1_DMA_STREAM_RX, ENABLE);
        I2C_DMACmd(I2C1, ENABLE);
        
        /*TODO: Move EOT handling to interrupt/RTOS wrapping*/
        while(!DMA_GetFlagStatus(I2C1_DMA_STREAM_RX, I2C1_DMA_RX_FLAG_TCIF)) {}
        
        I2C_GenerateSTOP(I2C1, ENABLE);
        DMA_Cmd(I2C1_DMA_STREAM_RX, DISABLE);
        DMA_ClearFlag(I2C1_DMA_STREAM_RX, I2C1_DMA_RX_FLAG_TCIF);
    }
}

void initializeI2C()
{
  initializeI2C1();
}