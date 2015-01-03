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
  
  void dmaI2C1Read(uint8_t * buffer, uint8_t address, uint16_t length);
  void dmaI2C1Write(uint8_t * buffer, uint8_t address, uint16_t length);
#define DIR_TX 1
#define DIR_RX 2
  
/* I2C1: SCL: PB8, SDA: PB9, DMA1 Stream 5 Channel 1(RX)/Stream 6 Channel 1(TX) */
/* Defines for portability */
#define I2C1_SCL_PORT GPIOB
#define I2C1_SDA_PORT GPIOB
#define I2C1_SCL_PIN GPIO_Pin_8
#define I2C1_SDA_PIN GPIO_Pin_9
#define I2C1_SCL_PIN_SOURCE GPIO_PinSource8
#define I2C1_SDA_PIN_SOURCE GPIO_PinSource9
#define I2C1_DMA_STREAM_RX DMA1_Stream5
#define I2C1_DMA_STREAM_TX DMA1_Stream6
#define I2C1_DMA_CHANNEL_RX DMA_Channel_1
#define I2C1_DMA_CHANNEL_TX DMA_Channel_1
#define I2C1_DMA_RX_FLAG_TCIF DMA_FLAG_TCIF5
#define I2C1_DMA_RX_FLAG_DMEIF DMA_FLAG_DMEIF5
#define I2C1_DMA_RX_FLAG_FEIF DMA_FLAG_FEIF5
#define I2C1_DMA_RX_FLAG_HTIF DMA_FLAG_HTIF5
#define I2C1_DMA_RX_FLAG_TEIF DMA_FLAG_TEIF5
#define I2C1_DMA_TX_FLAG_TCIF DMA_FLAG_TCIF6
#define I2C1_DMA_TX_FLAG_DMEIF DMA_FLAG_DMEIF6
#define I2C1_DMA_TX_FLAG_FEIF DMA_FLAG_FEIF6
#define I2C1_DMA_TX_FLAG_HTIF DMA_FLAG_HTIF6
#define I2C1_DMA_TX_FLAG_TEIF DMA_FLAG_TEIF6
#define I2C1_DMA_CLK RCC_AHB1Periph_DMA1
#define I2C1_GPIO_CLK RCC_AHB1Periph_GPIOB
#define I2C1_OWN_ADDRESS 0x34


#ifdef  __cplusplus
}
#endif

#endif  /* SPI_H */

