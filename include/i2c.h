/* 
 * File:   i2c.h
 * Author: Jake Drahos
 *
 * DMA'd i2c implementation
 * Usage: Initialize with initializeI2C(); Take the mutex for I2C and call dmaI2CRead() or dmaI2CWrite()
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <stm32f4xx_dma.h>

#ifdef  __cplusplus
extern "C" {
#endif

  void initializeSPI();
  
  void dmaI2CRead(uint8_t * buffer, uint8_t address, uint16_t length);
  void dmaI2CWrite(uint8_t * buffer, uint8_t address, uint16_t length);
  void dmaConfig(uint32_t, uint32_t, uint8_t, DMA_Stream_TypeDef *);
#define DIR_TX 1
#define DIR_RX 2


#ifdef  __cplusplus
}
#endif

#endif  /* SPI_H */

