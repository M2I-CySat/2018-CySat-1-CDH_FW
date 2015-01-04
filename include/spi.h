/* 
 * File:   mem.h
 * Author: Jake Drahos
 *
 * DMA'd SPI implementation
 * Usage: Initialize with initializeSPI(); Take the mutex for SPI and call dmaSPITransfer();
 * Manually control /CS outside of this API.
 */

#ifndef SPI_H
#define	SPI_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>

#ifdef	__cplusplus
extern "C" {
#endif

	void initializeSPI();
	
	int16_t SPI1Transfer(uint8_t * txBuffer, uint8_t * rxBuffer, uint16_t length, TickType_t blocktime);
    int16_t SPI1_TakeMutex(TickType_t blocktime);
    int16_t SPI1_ReleaseMutex();
    
/* SPI1: Unknown GPIOs, DMA2 Stream 2 Channel 3(RX)/Stream 5 Channel 3(TX) */
#define SPI1_DMA_CLK RCC_AHB1Periph_DMAA2
#define SPI1_DMA_STREAM_RX DMA2_Stream2
#define SPI1_DMA_STREAM_TX DMA2_Stream5
#define SPI1_DMA_CHANNEL_RX DMA_Channel_3
#define SPI1_DMA_CHANNEL_TX DMA_Channel_3
#define SPI1_DMA_RX_FLAG_TCIF DMA_FLAG_TCIF2
#define SPI1_DMA_RX_FLAG_DMEIF DMA_FLAG_DMEIF2
#define SPI1_DMA_RX_FLAG_FEIF DMA_FLAG_FEIF2
#define SPI1_DMA_RX_FLAG_HTIF DMA_FLAG_HTIF2
#define SPI1_DMA_RX_FLAG_TEIF DMA_FLAG_TEIF2
#define SPI1_DMA_TX_FLAG_TCIF DMA_FLAG_TCIF5
#define SPI1_DMA_TX_FLAG_DMEIF DMA_FLAG_DMEIF5
#define SPI1_DMA_TX_FLAG_FEIF DMA_FLAG_FEIF5
#define SPI1_DMA_TX_FLAG_HTIF DMA_FLAG_HTIF5
#define SPI1_DMA_TX_FLAG_TEIF DMA_FLAG_TEIF5
#define SPI1_DMA_PREPRIO configLIBRARY_KERNEL_INTERRUPT_PRIORITY
#define SPI1_DMA_SUBPRIO 0
#define SPI1_DMA_RX_IRQn DMA2_Stream2_IRQn
#define SPI1_DMA_TX_IRQn DMA2_Stream5_IRQn

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

