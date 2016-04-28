/* CySat SPI driver */

#include <drivers/spi.h>
#include <error.h>
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_rcc.h"
#include "cmsis_os.h"

osMutexId mem_mutex_id;
osMutexDef(mem_mutex);

osMutexId sysalt_mutex_id;
osMutexDef(sysalt_mutex);

osSemaphoreId mem_semph_id;
osSemaphoreDef(mem_semph);

osSemaphoreId sysalt_semph_id;
osSemaphoreDef(sysalt_semph);

static SPI_HandleTypeDef mem_handle;
static SPI_HandleTypeDef sysalt_handle;

static DMA_HandleTypeDef mem_dma_tx_handle;
static DMA_HandleTypeDef mem_dma_rx_handle;

static DMA_HandleTypeDef sysalt_dma_tx_handle;
static DMA_HandleTypeDef sysalt_dma_rx_handle;

/* MSP conifguration defines */


/* DMA Associations */
#define SPI_MEM_RX_DMA_STREAM DMA1_Stream3
#define SPI_MEM_TX_DMA_STREAM DMA1_Stream4

#define SPI_MEM_RX_DMA_CHANNEL DMA_CHANNEL_0
#define SPI_MEM_TX_DMA_CHANNEL DMA_CHANNEL_0

#define SPI_SYSALT_RX_DMA_STREAM DMA2_Stream2
#define SPI_SYSALT_TX_DMA_STREAM DMA2_Stream3

#define SPI_SYSALT_RX_DMA_CHANNEL DMA_CHANNEL_3
#define SPI_SYSALT_TX_DMA_CHANNEL DMA_CHANNEL_3

#define SPI_MEM_MOSI_GPIO GPIOB
#define SPI_MEM_MISO_GPIO GPIOB
#define SPI_MEM_CLK_GPIO GPIOB
#define SPI_MEM_CS0_GPIO GPIOB
#define SPI_MEM_CS1_GPIO GPIOB
#define SPI_MEM_CS2_GPIO GPIOB
#define SPI_MEM_CS3_GPIO GPIOB

#define SPI_MEM_MOSI_PIN GPIO_Pin15
#define SPI_MEM_MISO_PIN GPIO_Pin14
#define SPI_MEM_SCK_PIN GPIO_Pin13
#define SPI_MEM_CS0_PIN GPIO_Pin12
#define SPI_MEM_CS1_PIN GPIO_Pin8
#define SPI_MEM_CS2_PIN GPIO_Pin1
#define SPI_MEM_CS3_PIN GPIO_Pin0

/* SPI1 Clocks */
#define SPI_MEM_CLK_ENABLE() __HAL_RCC_SPI2_CLK_ENABLE()
#define SPI_MEM_MOSI_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_MEM_MISO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_MEM_SCK_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_MEM_CS0_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_MEM_CS1_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_MEM_CS2_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_MEM_CS3_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI_MEM_DMA_TX_CLK_ENABLE() __HAL_RCC_DMA1_CLK_ENABLE()
#define SPI_MEM_DMA_RX_CLK_ENABLE() __HAL_RCC_DMA1_CLK_ENABLE()

static void mem_tx_cplt_cb();
static void mem_rx_cplt_cb();

static void sysalt_tx_cplt_cb();
static void sysalt_rx_cplt_cb();

static osMutexId getSPIMutex(SPI_HandleTypeDef * hspi);
static osSemaphoreId getSPISemaphore(SPI_HandleTypeDef * hspi);

int SPI_Initialize()
{
	mem_handle.Init.Mode = SPI_MODE_MASTER;
	mem_handle.Init.Direction = SPI_DIRECTION_2LINES;
	mem_handle.Init.DataSize = SPI_DATASIZE_8BIT;
	mem_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
	mem_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	mem_handle.Init.NSS = SPI_NSS_SOFT;
	mem_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	mem_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	mem_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	mem_handle.Init.CRCPolynomial = 0;

	mem_handle.Instance = SPI_MEM;
	
	HAL_SPI_Init(&mem_handle);

	mem_mutex_id = osMutexCreate(osMutex(mem_mutex));
	if (mem_mutex_id == NULL) {
		ERROR_MiscRTOS("Failed to create spi mutex");
	}

	sysalt_mutex_id = osMutexCreate(osMutex(sysalt_mutex));
	if (sysalt_mutex_id == NULL) {
		ERROR_MiscRTOS("Failed to create spi mutex");
	}

	/* Create semaphores */
	mem_semph_id = osSemaphoreCreate(osSemaphore(mem_semph), 0);
	if (mem_semph_id == NULL) {
		ERROR_MiscRTOS("Failed to create spi semaphore");
	}

	sysalt_semph_id = osSemaphoreCreate(osSemaphore(sysalt_semph), 0);
	if (sysalt_semph_id == NULL) {
		ERROR_MiscRTOS("Failed to create spi semaphore");
	}

	return 0;
}

int SPI_Transmit(SPI_HandleTypeDef* hspi, uint8_t * buffer, size_t len)
{
	osMutexId mutex = getSPIMutex(hspi);

	/* Lock mutex for this particular SPI instance */
	if (osMutexWait(mutex, osWaitForever) != osOK) {
		ERROR_MiscRTOS("Mutex wait failed");
	}
	
	/* Go! */

	if (osSemaphoreWait(getSPISemaphore(hspi), osWaitForever) != osOK) {
		ERROR_MiscRTOS("Mutex was not released successfully");
	}

	/* Release mutex */
	if (osMutexRelease(mutex) != osOK) {
		ERROR_MiscRTOS("Mutex was not released successfully");
	}
	
	/* Return success */
	return 0;
}

int SPI_Receive(SPI_HandleTypeDef * hspi, uint8_t * buffer, size_t len)
{
	return 0;
}

int SPI_FullDuplex(SPI_HandleTypeDef * hspi, uint8_t * txBuffer, uint8_t * rxBuffer, size_t len) 
{
	return 0;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	if (hspi == &mem_handle) {
		/* Enable peripheral clocks */
		SPI_MEM_CLK_ENABLE();
		SPI_MEM_MOSI_CLK_ENABLE();
		SPI_MEM_MISO_CLK_ENABLE();
		SPI_MEM_CS0_CLK_ENABLE();
		SPI_MEM_CS1_CLK_ENABLE();
		SPI_MEM_CS2_CLK_ENABLE();
		SPI_MEM_CS3_CLK_ENABLE();

		/* TODO: Configure GPIO pins as AF push-pull */
		
		/* DMA configuration */
		SPI_MEM_DMA_TX_CLK_ENABLE();
		
		/* Configure DMA parameters */
		mem_dma_rx_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
		mem_dma_rx_handle.Init.PeriphInc = DMA_PINC_DISABLE;
		mem_dma_rx_handle.Init.MemInc = DMA_MINC_ENABLE;
		mem_dma_rx_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		mem_dma_rx_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		mem_dma_rx_handle.Init.Mode = DMA_NORMAL;
		mem_dma_rx_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		mem_dma_rx_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		mem_dma_rx_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
		mem_dma_rx_handle.Init.MemBurst = DMA_MBURST_SINGLE;
		mem_dma_rx_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
		mem_dma_tx_handle.Init.Channel = SPI_MEM_TX_DMA_CHANNEL;

		/* Set instance */
		mem_dma_tx_handle.Instance = SPI_MEM_TX_DMA_STREAM;
		/* TODO: Error callback */

		/* Callbacks */
		mem_dma_tx_handle.XferCpltCallback = mem_tx_cplt_cb;

		/* Configure */
		HAL_DMA_Init(&mem_dma_tx_handle);

		/* RX */
		SPI_MEM_DMA_RX_CLK_ENABLE();
		
		/* Configure DMA parameters */
		mem_dma_rx_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
		mem_dma_rx_handle.Init.PeriphInc = DMA_PINC_DISABLE;
		mem_dma_rx_handle.Init.MemInc = DMA_MINC_ENABLE;
		mem_dma_rx_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		mem_dma_rx_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		mem_dma_rx_handle.Init.Mode = DMA_NORMAL;
		mem_dma_rx_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		mem_dma_rx_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		mem_dma_rx_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
		mem_dma_rx_handle.Init.MemBurst = DMA_MBURST_SINGLE;
		mem_dma_rx_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
		mem_dma_rx_handle.Init.Channel = SPI_MEM_RX_DMA_CHANNEL;

		/* Set instance */
		mem_dma_rx_handle.Instance = SPI_MEM_RX_DMA_STREAM;
		/* TODO: Error callback */

		/* Callbacks */
		mem_dma_rx_handle.XferCpltCallback = mem_rx_cplt_cb;

		/* Configure */
		HAL_DMA_Init(&mem_dma_rx_handle);

		/* Associate DMA streams with SPI handle */
		hspi->hdmatx = &mem_dma_tx_handle;
		hspi->hdmarx = &mem_dma_rx_handle;

		/* Expose public handle pointers */
		SPI_MEM_DMA_TX_HANDLE = &mem_dma_tx_handle;
		SPI_MEM_DMA_RX_HANDLE = &mem_dma_rx_handle;

		/* TODO: Enable DMA interrupts */

	} else {
		ERROR_NotImplemented("Bad SPI handle");
	}
}

static osMutexId getSPIMutex(SPI_HandleTypeDef * hspi)
{
	if (hspi == &mem_handle) {
		return mem_mutex_id;
	} else if (hspi == &sysalt_handle) {
		return sysalt_mutex_id;
	} else {
		ERROR_NotImplemented("Bad SPI handle");
	}
	return NULL;
}

static osSemaphoreId getSPISemaphore(SPI_HandleTypeDef * hspi)
{
	if (hspi == &mem_handle) {
		return mem_semph_id;
	} else if (hspi == &sysalt_handle) {
		return sysalt_semph_id;
	} else {
		ERROR_NotImplemented("Bad SPI handle");
	}
	return NULL;
}

/* Callbacks */
static void mem_tx_cplt_cb()
{
	osSemaphoreRelease(mem_semph_id);
	return;
}

static void mem_rx_cplt_cb()
{
	osSemaphoreRelease(mem_semph_id);
	return;
}
