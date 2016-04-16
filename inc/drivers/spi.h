#ifndef __SPI_H
#define __SPI_H

/* CySat DMA'd SPI driver
 */

#include <stdint.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_spi.h>

int SPI_Transmit(SPI_HandleTypeDef * hspi, uint8_t * buffer, size_t len);
int SPI_Receive(SPI_HandleTypeDef * hspi, uint8_t * buffer, size_t len);
int SPI_FullDuplex(SPI_HandleTypeDef * hspi, uint8_t * rxBuffer, uint8_t * txBuffer, size_t len);

/* Defines wrapping SPI functions for convenience */
#define SPI_MEM SPI2

#define MEM_Transmit(buffer, len) SPI_Transmit(SPI_MEM, buffer, len)
#define MEM_Receive(buffer, len) SPI_Transmit(SPI_MEM, buffer, len)
#define MEM_FullDuplex(rxBuffer, txBuffer, len) SPI_Transmit(SPI_MEM, rxBuffer, txBuffer, len)

/* SYS on PRI, ALT on SEC */
#define SPI_SYSALT SPI1

#define SYSALT_Transmit(buffer, len) SPI_Transmit(SPI_SYSALT, buffer, len)
#define SYSALT_Receive(buffer, len) SPI_Receive(SPI_SYSALT, buffer, len)
#define SYSALT_FullDuplex(rxBuffer, txBuffer, len) SPI_FullDuplex(SPI_SYSALT, rxBuffer, txBuffer, len)

/* Handles */
static DMA_HandleTypeDef * SPI_MEM_DMA_TX_HANDLE;
static DMA_HandleTypeDef * SPI_MEM_DMA_RX_HANDLE;

static DMA_HandleTypeDef * SPI_SYSALT_DMA_TX_HANDLE;
static DMA_HandleTypeDef * SPI_SYSALT_DMA_RX_HANDLE;

#endif
