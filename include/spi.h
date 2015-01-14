/*!
 * \File   spi.h
 * \Author Jake Drahos
 *
 * \addtogroup CySat
 * \{
 * \defgroup CySat_SPI
 * \{
 * 
 * \brief CySat SPI API
 * 
 * The CySat SPI API uses DMA and RTOS-wrapping to allow
 * large SPI transactions to occur without blocking
 * other tasks from running. In order to use any
 * functionality of this API, SPI_Initialize() must first
 * be called.
 * 
 * Acces is determined by a mutex for each SPI peripheral.
 * The mutex must be obtained before any task can safely use
 * the peripheral. This mutex must also be relased once the
 * transaction is complete.
 * 
 * SPI access is abstracted into a single function: SPIxTransfer.
 * This function takes two buffers, a Tx and an Rx buffer. Data
 * is transferred in full-duplex mode, transmitted to the
 * slave device from the Tx buffer while filling the Rx buffer
 * with data received. Appropriately sized buffers must be used
 * for any transaction, even if it is conceptually half-duplex, since
 * the SPIxTransfer function operates in full duplex at all times.
 * 
 * The SPIxTransfer function does not handle the assertion
 * or deassertion of the Chip Select lines. CS lines must be handled
 * outside of this API.
 * 
 * The following is an example of a hypothetical 4-byte write-readback
 * operation from an external memory device. First, a one-byte
 * opcode (0xAA for write, 0xBB for read) is written, followed by a three-byte address
 * (0x00, 0x00, 0x40), followed by 4 data bytes (0x1A, 0x2B, 0x3C, 0x4D).
 * This results in a total transaction size of 8 bytes.
\code{.c}
    uint8_t txbuffer[8] = {0xAA, 0x00, 0x00, 0x40, 0x1A, 0x2B, 0x3C, 0x4D};
    uint8_t rxbuffer[8] = {0};
    
    if(SPI1_TakeMutex(portMAX_DELAY))
    {
        ASSERT_CHIP_SELECT();
        SPI1_Transfer(txbuffer, rxbuffer, 8, portMAX_DELAY);
        DEASSERT_CHIP_SELECT();
\endcode
 * The 4 bytes have been written to the device,
 * and rxbuffer is full of junk data from the device.
 * The contents of txbuffer[4] through txbuffer[7] do
 * not matter past this point 
\code{.c}
        txbuffer[0] = 0xBB;
    
        ASSERT_CHIP_SELECT();
        SPI1_Transfer(txbuffer, rxbuffer, 8, portMAX_DELAY);
        DEASSERT_CHIP_SELECT();
    
        SPI1_ReleaseMutex();
    }
\endcode
    
 * rxbuffer now contains the data read back from
 * the device. Note that the data is in rxbuffer[4]
 * through rxbuffer[8], since the first 4 bytes
 * are filled with junk data received during
 * transmission of the opcode and address bytes
 */

#ifndef SPI_H
#define	SPI_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>

#ifdef	__cplusplus
extern "C" {
#endif
    /*! Perform a full-duplex SPI transfer
     * 
     * Both buffers must be of suffient size to complete
     * the transaction, even if one is largely full of
     * dummy bytes or junk data. Do not call this function without first 
     * obtaining the mutex with SPI1_TakeMutex(). This function
     * blocks until the operation is complete, or if the timeout
     * is reached. During blocking, other threads are free 
     * to run. If the timeout is reached, FALSE will 
     * be returned. This should be treated as an extreme
     * error condition such as hardware failure, so
     * set a sufficient blocktime to avoid this
     * happening unintentionally.
     * 
     * Chip Select handling must be handled by the application,
     * outside of this API.
     * 
     * \param txBuffer Buffer containing bytes to be transmitted
     * \param rxBuffer Destination buffer for received bytes
     * \param length Number of bytes to transmit and receive
     * \param blocktime Amount of time to block before returning false
     * 
     * \return pdTRUE when the transfer completes, or pdFALSE
     * if the blocktime was reached.
     */
	int16_t SPI1_Transfer(uint8_t * txBuffer, uint8_t * rxBuffer, uint16_t length, TickType_t blocktime);
	
	/*! Acqire Mutex for SPI1
     * 
     * This mutex must be acquired before the SPI1_Transfer()
     * can be used. Release the mutex with SPI1_ReleaseMutex() once
     * the transaction is complete.
     * 
     * \param blocktime Maximum amount of time to block
     * 
     * \return pdTRUE if the mutex was acquired, or pdFALSE if
     * the blocktime was reached.
     */
    int16_t SPI1_TakeMutex(TickType_t blocktime);
    
    /*! Release Mutex for SPi1
     * 
     * Call this function once the transaction is complete. Do
     * not call this function without first acquiring the mutex with SPI1_TakeMutex()
     * 
     * \return pdTRUE is the mutex was successfully released. Should
     * only return pdFALSE if an error occurred (the mutex was not held
     * prior to calling this function)
     */
    int16_t SPI1_ReleaseMutex();
    
    /*!
     * \}
     * \addtogroup CySat_Private
     * \{
     */
    
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

    /*! Initialize all SPI peripherals.
     * 
     * No other functions from the SPI API may be used until
     * SPI_Initialize() has been called.
     * 
     * This function is called early during the satellite 
     * init process.
     */
    void SPI_Initialize();
    

#ifdef	__cplusplus
}
#endif

/*!
 * \}
 * \}
 */

#endif	/* SPI_H */

