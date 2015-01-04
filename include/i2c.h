/*! 
 * \File   i2c.h
 * \Author Jake Drahos
 *
 * \addtogroup CySat
 * \{
 * \defgroup CySat_I2C
 * \{
 * 
 * \brief CySat I2C API
 * 
 * The CySat I2C API uses DMA and RTOS-wrapping to allow
 * large I2C transactions to occur without blocking other
 * tasks from running. In order to use any functionality of this
 * API, initializeI2C() must first be called.
 * 
 * Access is determined by a mutex for each I2C peripheral. The mutex
 * must be obtained before a task can safely use the peripheral.
 * This mutex must also be released once the transaction is complete.
 * 
 * I2C access is abstracted into two functions: I2CxRead and I2CxWrite.
 * Each of these functions operates on a specified buffer and takes
 * an address of the slave device. I2CxRead fills the buffer with the
 * data from the slave device, and I2CxWrite writes the buffer contents
 * to the slave device. Here is an example of an I2C transaction. In
 * this example, a single byte (0x0A) will be written to the slave
 * device (Address 0x3C), then
 * three bytes will be read from the device. This represents a typical
 * I2C sensor in which the internal register address is written, then
 * data is read.
 * 
 *
    \code{.c}
    uint8_t buffer[3];
    buffer[0] = 0x0A;
  
    I2C1_TakeMutex(portMAX_DELAY);
    I2C1Write(buffer, 0x3C, 1, portMAX_DELAY);
    I2C1Read(buffer, 0x3C, 3, portMAX_DELAY);
    I2C1_ReleaseMutex();
    \endcode
 * 
 * The buffer now contains 3 bytes read from the internal
 * register 0x0A, 0x0B, and 0x0C of the slave device.
 */

#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stm32f4xx_dma.h>
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>

#ifdef  __cplusplus
extern "C" {
#endif

    
    /*! Perform an I2C Read
     * 
     * This function performs an I2C read with I2C1.
     * Do not call this function without first
     * obtaining the appropriate mutex via I2C1_TakeMutex().
     * This function contains
     * the entire I2C transaction, including START and STOP
     * conditions; nothing needs to be done outside of the
     * function call. This function blocks until the read
     * is complete, but allows other threads to run in the 
     * background. If the timeout is reached, pdFALSE will
     * be returned, but the transaction is likely still
     * in progress. This should be treated as an extreme
     * error condition such as a hardware failure. For this
     * reason, make sure to use a suitably generous blocktime.
     * 
     * \param buffer Buffer to which data will be written
     * \param address Address of the slave device
     * \param length Length of data to be read
     * \param blocktime Amount of time to delay
     * 
     * \return pdTRUE when transaction completes, otherwise
     * pdFALSE
     */
    
    /*!
     */
    int16_t I2C1Read(uint8_t * buffer, uint8_t address, uint16_t length, TickType_t blocktime);
    
    /*! Perform an I2C Write
     * 
     * This function performs an I2C write with I2C1.
     * Do not call this function without first
     * obtaining the appropriate mutex via I2C1_TakeMutex().
     * This function contains
     * the entire I2C transaction, including START and STOP
     * conditions; nothing needs to be done outside of the
     * function call. This function blocks until the write
     * is complete, but allows other threads to run in the 
     * background. If the timeout is reached, pdFALSE will
     * be returned, but the transaction is likely still
     * in progress. This should be treated as an extreme
     * error condition such as a hardware failure. For this
     * reason, make sure to use a suitably generous blocktime.
     * 
     * \param buffer Buffer from which data will be written to
     * the slave device
     * \param address Address of the slave device
     * \param length Length of data to be read
     * \param blocktime Amount of time to delay
     * 
     * \return pdTRUE when transaction completes, otherwise
     * pdFALSE
     */
    int16_t I2C1Write(uint8_t * buffer, uint8_t address, uint16_t length, TickType_t blocktime);
    
    /*! Obtain Mutex for I2C1
     * 
     * The mutex for I2C1 must be obtained prior to 
     * issuing reads or writes.
     * 
     * Release the mutex with I2C1_ReleaseMutex()
     * when the transaction is complete.
     * 
     * \param blocktime Amount of time to delay
     * 
     * \return pdTRUE if the mutex was obtained, or
     * pdFALSE if the blocktime was reached
     */
    int16_t I2C1_TakeMutex(TickType_t blocktime);
    
    /*! Release Mutex for I2C1
     * 
     * This function is called to release the mutex after
     * the transaction has been completed. Do not 
     * call this function without first obtaining
     * the mutex with I2C1_TakeMutex().
     * 
     * \return pdTRUE if the mutex was successfully released,
     * pdFALSE if an error occurred (the mutex was not taken prior
     * to the release)
     */
    int16_t I2C1_ReleaseMutex();

    /*!
     * \}
     * \addtogroup CySat_Private
     * \{
     * 
     * CySat_Private contains internal functions that
     * are not part of the official payload interface API.
     */
    
    /*! Initialize all I2C peripherals.
     * 
     * No other functions from the I2C API may be used until
     * initializeI2C() has been called.
     * 
     * This function is called early during the satellite 
     * init process.
     */
    void initializeI2C();
  
  
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
#define I2C1_DMA_PREPRIO configLIBRARY_KERNEL_INTERRUPT_PRIORITY
#define I2C1_DMA_SUBPRIO 0
#define I2C1_DMA_RX_IRQn DMA1_Stream5_IRQn
#define I2C1_DMA_TX_IRQn DMA1_Stream6_IRQn


#ifdef  __cplusplus
}
#endif

#endif  /* I2C_H */


/*!
 * \}
 * \}
 */