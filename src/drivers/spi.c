/* CySat SPI driver */

#include <spi.h>
#include <signals.h>
#include <error.h>

osMutexId = spi_mutex_id;
osMutexDef(spi_mutex_id);

static osThreadId spi_mem_thread;
static osThreadId spi_sysalt_thread;

int SPI_Initialize()
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_DMA2, ENABLE);

	SPI_InitTypeDef spi_init_structure;
	SPI_StructInit(&spi_init_structure);
	spi_init_structure.SPI_BaudRatePrescalar =SPI_BaudRatePrescalar_4;
	spi_init_structure.SPI_CPOL = SPI_CPOL_LOW;
	spi_init_structure.SPI_CPHA = SPI_CPHA_1Edge;
	spi_init_structure.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_set;
	spi_init_structure.SPI_DataSize = SPI_DataSize_8b;
	spi_init_structure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi_init_structure.SPI_FirstBit = SPI_FirstBit_MSB;
	spi_init_structure.SPI_Mode = SPI_Mode_Master;
	
	SPI_Init(SPI1, &spi_init_structure);

	spi_mutex_id = osMutexCreate(osMutex(spi_mutex_id));
	if (spi_mutex_id == NULL) {
		ERROR_MiscRTOS(Failed to create spi mutex);
	}
}

int SPI_Transmit(SPI_TypeDef* instance, uint8_t * buffer, size_t len)
{
	/* Lock mutex for this particular SPI instance */
	if (osMutexWait(spi_mutex_id, osWaitForever) != osOK) {
		ERROR_MiscRTOS("Mutex wait failed");
	}
	
	/* 1. Retrieve the DMA stream associated with transmit of instance
	 * 2. Make sure the DMA is disabled
	 * 3. Set up the DMA
	 * 	a. PeripheralBaseAddr = instance->DR
	 * 	b. MemoryBaseAddr = buffer
	 * 	c. Direction = PeripheralToMemory
	 * 	d. MemoryInc = yes
	 * 	e. PeripheralInc = no
	 * 	f. Set channel (retrieve channel associated with instance)
	 *	g. Other options (datasize, mode, priority)
	 */

	/* Initialiaze DMA */
	DMA_Cmd(getTxStream(instance), DISABLE);
	DMA_DeInit(getTxStream(instance));
	
	DMA_InitTypeDef dma_init_structure;
	DMA_StructInit(&dma_init_structure);
	
	dma_init_structure.DMA_PeriperalBaseAddr = (uint32_t) &(instance->DR);
	dma_init_structure.Memory0BaseAddr = (uint32_t) buffer;
	dma_init_structure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	dma_init_structure.DMA_BufferSize = len;
	dma_init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_init_structure.DMA_PheriperalDataSize = DMA_PeripheralDataSize_Byte;
	dma_init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_init_structure.DMA_Mode = DMA_Mode_Normal;
	dma_init_structure.DMA_Priority - DMA_Priority_VeryHigh;
	dma_init_structure - DMA_CHannel3 // Is this the right channel?
	
	DMA_Init(getTxStream(instance), &dma_init_structure);

	/* Go! */

	/* set thread ID in a static variable */
	storeThreadId(instance);

	/* wait_signal SPI_COMPLETE */
	// osSignalWait();

	/* Tear down DMA */
	DMA_DeInit(getTxStream(instance));
	
	/* Release mutex */
	if (osMutexRelease(getTxStream(instance)) != osOK) {
		Error_MiscRTOS("Mutex was not released successfully");
	}
	
	/* Return success */
	return 0;
}

int SPI_Receive(SPI_TypeDef instance, uint8_t * buffer, size_t len)
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
}

/* Interrupt Handler(s)
 * One per DMA stream?
 *
 * {
 * 	instance = figure out instance from stream + channel
 *
 * 	thread = get thread ID from static variables
 *
 * 	Signal_Send(thread, SPI_COMPLETE)
 * 	}
 */

static uint32_t getTxChannel (SPI_TypeDef* instance) {
	if (instance == SPI_MEM) {
		return SPI_MEM_TX_DMA_CHANNEL;
	} else {
		ERROR_NotImplemented("Bad SPI instance");
	}
}

static DMA_Stream_TypeDef getTxStream (SPI_TypeDef* instance) {
	if (instance == SPI_MEM) {
		return SPI_MEM_TX_DMA_STREAM;
	} else {
		ERROR_NotImplemented("Bad SPI instance");
	}
}

static void storeThreadId(SPI_TypeDef* instance) {
	if (instance == SPI_MEM) {
		spi_mem_thread = osGetThreadId();
	} else {
		ERROR_NotImplemented("Bad SPI instance");
	}
}
