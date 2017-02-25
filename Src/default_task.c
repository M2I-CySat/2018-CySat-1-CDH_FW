#include "uart2.h"
#include "cmsis_os.h"
#include "mutexes.h"
#include "gpio.h"

static void InitializeEverything(void);

void DefaultTask(const void * arguments)
{
	Debug_Printf("Default task started");
	InitializeEverything();
	Debug_Printf("One-time initialization completed");
	for(;;) {
		osDelay(2000);
		Debug_Printf("Default");
	}
}

static void InitializeEverything()
{
	/* Down semaphores */
	osSemaphoreWait(mem_semaphoreHandle, 0);
	osSemaphoreWait(uart2_txSemaphoreHandle, 0);

	/* GPIOA */
	HAL_GPIO_WritePin(GPIOA, NYIELD_P_Pin | NRST_S_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, SYS_RESET_Pin, GPIO_PIN_RESET);

	/* GPIOB */
	HAL_GPIO_WritePin(
			GPIOB,
			MEM_CS0_Pin | MEM_CS1_Pin | MEM_CS2_Pin | MEM_CS3_Pin |
				MEM_NWP_Pin | MEM_NHOLD_Pin,
			GPIO_PIN_SET);

}
