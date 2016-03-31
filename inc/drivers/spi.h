#ifndef __SPI_H
#define __SPI_H

/* CySat DMA'd SPI driver
 */

#include <stdint.h>

int SPI_Transmit(SPI_TypeDef instance, uint8_t * buffer, size_t len);
int SPI_Receive(SPI_TypeDef instance, uint8_t * buffer, size_t len);
int SPI_FullDuplex(SPI_TypeDef instance, uint8_t * rxBuffer, uint8_t * txBuffer, size_t len);

/* Defines wrapping SPI functions for convenience */
#define MEM_SPI SPI2
#define MEM_Transmit(buffer, len) SPI_Transmit(MEM_SPI, buffer, len)
#define MEM_Receive(buffer, len) SPI_Transmit(MEM_SPI, buffer, len)
#define MEM_FullDuplex(rxBuffer, txBuffer, len) SPI_Transmit(MEM_SPI, rxBuffer, txBuffer, len)

#endif
