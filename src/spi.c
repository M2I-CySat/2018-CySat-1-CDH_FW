/*
 * File:   mem.c
 * Author: Jake Drahos
 *
 *
 * Source file for memory management.
 *
 * These functions allow access to PPM flash and FRAM over SPI
 *
 * Created on November 11, 2013, 5:43 PM
 */

#include <spi.h>
#include <uart.h>
#include <system.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_rcc.h>

#include <semphr.h>

static DMA_InitTypeDef SPI1_DMA_InitStructure;

static volatile SemaphoreHandle_t SPI1_DMA_RX_Semaphore;

static SemaphoreHandle_t SPI1_Mutex;

/* SPI1: Unknown GPIOs, DMA2 Stream 2 Channel 3(RX)/Stream 5 Channel 3(TX) */

static void SPI_Initialize1()
{
    NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);
    
    SPI_Cmd(SPI1, ENABLE);
    
    /* Initialize DMA streams */
    DMA_StructInit(&SPI1_DMA_InitStructure);
    
    DMA_ClearFlag(SPI1_DMA_STREAM_RX, SPI1_DMA_RX_FLAG_FEIF | SPI1_DMA_RX_FLAG_DMEIF | SPI1_DMA_RX_FLAG_TEIF |
                              SPI1_DMA_RX_FLAG_HTIF | SPI1_DMA_RX_FLAG_TCIF);
    DMA_Cmd(SPI1_DMA_STREAM_RX, DISABLE);

    DMA_DeInit(SPI1_DMA_STREAM_RX);
    SPI1_DMA_InitStructure.DMA_Channel = SPI1_DMA_CHANNEL_RX;
    SPI1_DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(SPI1->DR);
    SPI1_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;    /* This parameter will be configured durig communication */;
    SPI1_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; /* This parameter will be configured durig communication */
    SPI1_DMA_InitStructure.DMA_BufferSize = 0xFFFF;              /* This parameter will be configured durig communication */
    SPI1_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    SPI1_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    SPI1_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    SPI1_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    SPI1_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    SPI1_DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    SPI1_DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    SPI1_DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    SPI1_DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    SPI1_DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(SPI1_DMA_STREAM_RX, &SPI1_DMA_InitStructure);
    
    DMA_ClearFlag(SPI1_DMA_STREAM_TX, SPI1_DMA_TX_FLAG_FEIF | SPI1_DMA_TX_FLAG_DMEIF | SPI1_DMA_TX_FLAG_TEIF |
                              SPI1_DMA_TX_FLAG_HTIF | SPI1_DMA_TX_FLAG_TCIF);
    DMA_Cmd(SPI1_DMA_STREAM_TX, DISABLE);
  
    DMA_DeInit(SPI1_DMA_STREAM_TX);
    SPI1_DMA_InitStructure.DMA_Channel = SPI1_DMA_CHANNEL_TX;
    DMA_Init(SPI1_DMA_STREAM_TX, &SPI1_DMA_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_DMA_RX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SPI1_DMA_PREPRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SPI1_DMA_SUBPRIO;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Only use RX interrupt */
    DMA_ITConfig(SPI1_DMA_STREAM_RX, DMA_IT_TC, ENABLE);      
  
    SPI1_DMA_RX_Semaphore = xSemaphoreCreateBinary();
    SPI1_Mutex = xSemaphoreCreateMutex();
}

void SPI_Initialize()
{
    SPI_Initialize1();
}

int16_t SPI1_Transfer(uint8_t * txBuffer, uint8_t * rxBuffer, uint16_t length, TickType_t blocktime)
{
    SPI1_DMA_InitStructure.DMA_BufferSize = length;
    
	SPI1_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) rxBuffer;
	SPI1_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_Init(SPI1_DMA_STREAM_RX, &SPI1_DMA_InitStructure);
	
	SPI1_DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) txBuffer;
	SPI1_DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_Init(SPI1_DMA_STREAM_TX, &SPI1_DMA_InitStructure);
	
	DMA_Cmd(SPI1_DMA_STREAM_RX, ENABLE);
	DMA_Cmd(SPI1_DMA_STREAM_TX, ENABLE);
    
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx | SPI_I2S_DMAReq_Rx, ENABLE);
    
    return xSemaphoreTake(SPI1_DMA_RX_Semaphore, blocktime);
}

int16_t SPI1_TakeMutex(TickType_t blocktime)
{
    return xSemaphoreTake(SPI1_Mutex, blocktime);
}

int16_t SPI1_ReleaseMutex()
{
    return xSemaphoreGive(SPI1_Mutex);
}

/* SPI1_DMA_RX_IRQHandler */
void DMA2_Stream2_IRQHandler()
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    NVIC_ClearPendingIRQ(DMA2_Stream2_IRQn);
    if(DMA_GetFlagStatus(SPI1_DMA_STREAM_RX, SPI1_DMA_RX_FLAG_TCIF) != RESET)
    {
        DMA_Cmd(SPI1_DMA_STREAM_RX, DISABLE);
        DMA_Cmd(SPI1_DMA_STREAM_TX, DISABLE);
        DMA_ClearFlag(SPI1_DMA_STREAM_RX, SPI1_DMA_RX_FLAG_TCIF);
        DMA_ClearFlag(SPI1_DMA_STREAM_TX, SPI1_DMA_TX_FLAG_TCIF);
        
        xSemaphoreGiveFromISR(SPI1_DMA_RX_Semaphore, &higherPriorityTaskWoken);
    }
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}