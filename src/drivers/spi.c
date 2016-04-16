/* CySat SPI driver */

#include <drivers/spi.h>
#include <signals.h>
#include <error.h>
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_dma.h"
#include "cmsis_os.h"

osMutexId mem_mutex_id;
osMutexDef(mem_mutex);

osMutexId sysalt_mutex_id;
osMutexDef(sysalt_mutex);

static osThreadId mem_blocked_thread;
static osThreadId sysalt_blocked_thread;

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

#define SPI_MEM_RX_DMA_CHANNEL DMA_Channel0
#define SPI_MEM_TX_DMA_CHANNEL DMA_Channel0

#define SPI_SYSALT_RX_DMA_STREAM DMA2_Stream2
#define SPI_SYSALT_TX_DMA_STREAM DMA2_Stream3

#define SPI_SYSALT_RX_DMA_CHANNEL DMA_Channel3
#define SPI_SYSALT_TX_DMA_CHANNEL DMA_Channel3

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
static void storeSPIThreadId(SPI_HandleTypeDef * hspi);
static osThreadId getSPIThreadId(SPI_HandleTypeDef * hspi);

int SPI_Initialize()
{
	mem_handle.Init.SPI_BaudRatePrescalar =SPI_BaudRatePrescalar_4;
	mem_handle.Init.SPI_CPOL = SPI_CPOL_LOW;
	mem_handle.Init.SPI_CPHA = SPI_CPHA_1Edge;
	mem_handle.Init.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_set;
	mem_handle.Init.SPI_DataSize = SPI_DataSize_8b;
	mem_handle.Init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	mem_handle.Init.SPI_FirstBit = SPI_FirstBit_MSB;
	mem_handle.Init.SPI_Mode = SPI_Mode_Master;

	mem_handle.instance = SPI_MEM;
	
	HAL_SPI_Init(SPI_MEM);

	mem_mutex_id = osMutexCreate(osMutex(mem_mutex));
	if (mem_mutex_id == NULL) {
		ERROR_MiscRTOS("Failed to create spi mutex");
	}

	sysalt_mutex_id = osMutexCreate(osMutex(sysalt_mutex));
	if (sysalt_mutex_id == NULL) {
		ERROR_MiscRTOS("Failed to create spi mutex");
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

	/* set thread ID in a static variable */
	storeSPIThreadId(instance);

	/* wait_signal SPI_COMPLETE */
	// osSignalWait();

	/* Release mutex */
	if (osMutexRelease(mutex) != osOK) {
		Error_MiscRTOS("Mutex was not released successfully");
	}
	
	/* Return success */
	return 0;
}

int SPI_Receive(SPI_HandleTypeDef * hspi, uint8_t * buffer, size_t len)
{
	/* Lock mutex for this particular SPI instance */
	
	/* 1. Retrieve the DMA stream associated with transmit of instance
	 * 2. Make sure the DMA is disabled
	 * 3. Set up the DMA
	 * 	a. PeripheralBaseAddr = instance->DR
	 * 	b. MemoryBaseAddr = buffer
	 * 	c. Direction = MemoryToPeripheral
	 * 	d. MemoryInc = yes
	 * 	e. PeripheralInc = no
	 * 	f. Set channel (retrieve channel associated with instance)
	 *	g. Other options (datasize, mode, priority)
	 */

	/* Initialiaze DMA */

	/* Go! */

	/* set thread ID in a static variable */

	/* wait_signal SPI_COMPLETE */

	/* Tear down DMA */

	/* Release mutex */
	
	/* Return success */
	return 0;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	if (hspi == mem_handle) {
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
		mem_dma_tx_handle.Init.DMA_PeriperalBaseAddr = (uint32_t) &(hspi->DR);
		mem_dma_tx_handle.Init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		mem_dma_tx_handle.Init.DMA_BufferSize = len;
		mem_dma_tx_handle.Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		mem_dma_tx_handle.Init.DMA_MemoryInc = DMA_MemoryInc_Enable;
		mem_dma_tx_handle.Init.DMA_PheriperalDataSize = DMA_PeripheralDataSize_Byte;
		mem_dma_tx_handle.Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		mem_dma_tx_handle.Init.DMA_Mode = DMA_Mode_Normal;
		mem_dma_tx_handle.Init.DMA_Priority - DMA_Priority_VeryHigh;
		mem_dma_tx_handle.Init.DMA_Channel SPI_MEM_TX_DMA_CHANNEL;

		/* Set instance */
		mem_dma_tx_handle.instance = SPI_MEM_TX_DMA_STREAM;
		/* TODO: Error callback */

		/* Callbacks */
		mem_dma_tx_handle.XferCptlCallback = mem_tx_cptl_cb;

		/* Configure */
		HAL_DMA_INIT(&mem_dma_tx_handle);

		/* RX */
		SPI_MEM_DMA_RX_CLK_ENABLE();
		
		/* Configure DMA parameters */
		mem_dma_rx_handle.Init.DMA_PeriperalBaseAddr = (uint32_t) &(hspi->DR);
		mem_dma_rx_handle.Init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
		mem_dma_rx_handle.Init.DMA_BufferSize = len;
		mem_dma_rx_handle.Init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		mem_dma_rx_handle.Init.DMA_MemoryInc = DMA_MemoryInc_Enable;
		mem_dma_rx_handle.Init.DMA_PheriperalDataSize = DMA_PeripheralDataSize_Byte;
		mem_dma_rx_handle.Init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		mem_dma_rx_handle.Init.DMA_Mode = DMA_Mode_Normal;
		mem_dma_rx_handle.Init.DMA_Priority - DMA_Priority_VeryHigh;
		mem_dma_rx_handle.Init.DMA_Channel SPI_MEM_RX_DMA_CHANNEL;

		/* Set instance */
		mem_dma_rx_handle.instance = SPI_MEM_RX_DMA_STREAM;
		/* TODO: Error callback */

		/* Callbacks */
		mem_dma_rx_handle.XferCptlCallback = mem_rx_cptl_cb;

		/* Configure */
		HAL_DMA_INIT(&mem_dma_rx_handle);

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

static void storeSPIThreadId(SPI_HandleTypeDef* hspi) {
	if (hspi == &mem_handle) {
		mem_blocked_thread = osGetThreadId();
	} if (hspi == &sysalt_handle) {
		sysalt_blocked_thread = osGetThreadId();
	} else {
		ERROR_NotImplemented("Bad SPI hspi");
	}
}

static osThreadId getSPIThreadId(SPI_HandleTypeDef* hspi) {
	if (hspi == &mem_handle) {
		return mem_blocked_thread;
	} if (hspi == &sysalt_handle) {
		return sysalt_blocked_thread;
	} else {
		ERROR_NotImplemented("Bad SPI hspi");
	}
	return NULL;
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

/* Callbacks */
static void mem_tx_cplt_cb()
{
	/* todo: tx complete */
}
