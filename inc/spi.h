#ifndef __SPI_H
#define __SPI_H

/* CySat DMA'd SPI driver
 */

#include <stdint.h>

int SPI_Transmit(SPI_TypeDef instance, uint8_t * buffer, size_t len);
int SPI_Receive(SPI_TypeDef instance, uint8_t * buffer, size_t len);
int SPI_FullDuplex(SPI_TypeDef instance, uint8_t * rxBuffer, uint8_t * txBuffer, size_t len);

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


/* DMA Associations */
#define SPI_MEM_RX_DMA_STREAM DMA1_Stream3
#define SPI_MEM_TX_DMA_STREAM DMA1_Stream4

#define SPI_MEM_RX_DMA_CHANNEL DMA_Channel0
#define SPI_MEM_tX_DMA_CHANNEL DMA_Channel0

#define SPI_SYSALT_RX_DMA_STREAM DMA2_Stream2
#define SPI_SYSALT_TX_DMA_STREAM DMA2_Stream3

#define SPI_SYSALT_RX_DMA_CHANNEL DMA_Channel3
#define SPI_SYSALT_TX_DMA_CHANNEL DMA_Channel3

#endif
