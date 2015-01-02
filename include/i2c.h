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

#ifdef  __cplusplus
extern "C" {
#endif

  void initializeSPI();
  
  void dmaI2CRead(uint8_t * buffer, uint8_t address, uint16_t length);
  void dmaI2CWrite(uint8_t * buffer, uint8_t address, uint16_t length);


#ifdef  __cplusplus
}
#endif

#endif  /* SPI_H */

