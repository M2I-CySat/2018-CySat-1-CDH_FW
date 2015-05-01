
#include "nichrome.h"
#include "stm32f4xx_gpio.h"
#include "pinmapping.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "system.h"

#define nichromeON  1
#define nichromeOFF 0

#define BURN_TIME 4000

#define CTRL_1_GPIO	IO_25_GPIO
#define CTRL_2_GPIO	IO_27_GPIO
#define CTRL_3_GPIO	IO_29_GPIO
#define CTRL_4_GPIO	IO_37_GPIO
#define CTRL_5_GPIO	IO_39_GPIO
#define CTRL_6_GPIO	IO_41_GPIO

#define CTRL_1_PIN	IO_25_PIN
#define CTRL_2_PIN	IO_27_PIN
#define CTRL_3_PIN	IO_29_PIN
#define CTRL_4_PIN	IO_37_PIN
#define CTRL_5_PIN	IO_39_PIN
#define CTRL_6_PIN	IO_41_PIN

//number of seconds before burn
//used for an automated burn after deployment
#define BURN_INITIAL_DELAY 0

/* Print messages to Console? */
#define nichromeLOCAL_PRINT

#ifdef nichromeLOCAL_PRINT
#include "uart.h"
#endif

/**
 * Turn on Nichrome Burn Enable for specified amount of time
 * @param ctrl Which nichrome wire to burn (1-6)
 * @param ms Time in milliseconds to burn
 */
static void vNichromeBurn(char ctrl, unsigned ms);

/**
 * Initialize all nichrome control pins to OFF
 */
static void vNichromeInit();


/**
 * Set a nichrome control
 * @param ctrl
 * @param val
 */
inline static void vNichromeSet(char ctrl, unsigned val);

/**
 * Print an error for a bad control
 * @param ctrl
 */
inline static void vBadCtrlError( char ctrl );


/* Implementation*/


inline static void vBadCtrlError( char ctrl )
{
#ifdef nichromeLOCAL_PRINT
	vConsoleErrorPrintf( "Bad Nichrome Control (expected 1-6, got %d)\r\n", ctrl );
#endif
}

static void vNichromeInitializeCtrl(uint8_t ctrl)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	switch(ctrl)
	{
		case 1:
			GPIO_InitStructure.GPIO_Pin = CTRL_1_PIN;
			GPIO_Init(CTRL_1_GPIO, &GPIO_InitStructure);
			break;
		case 2:
			GPIO_InitStructure.GPIO_Pin = CTRL_2_PIN;
			GPIO_Init(CTRL_2_GPIO, &GPIO_InitStructure);
			break;
		case 3:
			GPIO_InitStructure.GPIO_Pin = CTRL_3_PIN;
			GPIO_Init(CTRL_3_GPIO, &GPIO_InitStructure);
			break;
		case 4:
			GPIO_InitStructure.GPIO_Pin = CTRL_4_PIN;
			GPIO_Init(CTRL_4_GPIO, &GPIO_InitStructure);
			break;
		case 5:
			GPIO_InitStructure.GPIO_Pin = CTRL_5_PIN;
			GPIO_Init(CTRL_5_GPIO, &GPIO_InitStructure);
			break;
		case 6:
			GPIO_InitStructure.GPIO_Pin = CTRL_6_PIN;
			GPIO_Init(CTRL_6_GPIO, &GPIO_InitStructure);
			break;
	}
}

inline static void vNichromeSet(char ctrl, unsigned val)
{
	if (val == nichromeON)
	{
		switch (ctrl)
		{
			case 1: GPIO_SetBits(CTRL_1_GPIO, CTRL_1_PIN); return;
			case 2: GPIO_SetBits(CTRL_2_GPIO, CTRL_2_PIN); return;
			case 3: GPIO_SetBits(CTRL_3_GPIO, CTRL_3_PIN); return;
			case 4: GPIO_SetBits(CTRL_4_GPIO, CTRL_4_PIN); return;
			case 5: GPIO_SetBits(CTRL_5_GPIO, CTRL_5_PIN); return;
			case 6: GPIO_SetBits(CTRL_6_GPIO, CTRL_6_PIN); return;
		}
	}
	else if (val == nichromeOFF)
	{
		switch (ctrl)
		{
			case 1: GPIO_ResetBits(CTRL_1_GPIO, CTRL_1_PIN); return;
			case 2: GPIO_ResetBits(CTRL_2_GPIO, CTRL_2_PIN); return;
			case 3: GPIO_ResetBits(CTRL_3_GPIO, CTRL_3_PIN); return;
			case 4: GPIO_ResetBits(CTRL_4_GPIO, CTRL_4_PIN); return;
			case 5: GPIO_ResetBits(CTRL_5_GPIO, CTRL_5_PIN); return;
			case 6: GPIO_ResetBits(CTRL_6_GPIO, CTRL_6_PIN); return;
		}
	}
	vBadCtrlError(ctrl);
}

static void vNichromeInit()
{
	char ctrl;

	/* TODO: These probably don't ALL need to be enabled */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	for (ctrl = 1; ctrl <= 6; ++ctrl)
	{
		vNichromeInitializeCtrl(ctrl); /* Initialize GPIO */
		vNichromeSet(ctrl, nichromeOFF); /* Turn off */
	}
}

static void vNichromeBurn(char ctrl, unsigned ms)
{
#ifdef nichromeLOCAL_PRINT
	vConsolePrintf("Burn #%d (%d ms) start...", ctrl, ms);
#endif
	vNichromeSet(ctrl, nichromeON);
	vTaskDelay(ms);
	vNichromeSet(ctrl, nichromeOFF);
#ifdef nichromeLOCAL_PRINT
	vConsolePrint("Done.\r\n");
#endif
}

void vNichromeStartTask()
{
	vNichromeInit();
	xTaskCreate( vNichromeTask, NULL, configMINIMAL_STACK_SIZE + 200, NULL, systemPRIORITY_NICHROME, NULL );
}

void vNichromeTask( void *pvParameters )
{
	char ctrl;
	vConsolePrint("Delayed burn initialized...\r\n");

	int i = 0;
	for(i = 0; i < BURN_INITIAL_DELAY; i++) {
		vTaskDelay(1000);
	}

#ifdef nichromeLOCAL_PRINT
	vTaskDelay(1000);
	vConsolePrint("\r\nBurn in 3...\r");
	vTaskDelay(1000);
	vConsolePrint("Burn in 2...\r");
	vTaskDelay(1000);
	vConsolePrint("Burn in 1...\r");
	vTaskDelay(1000);
#else
	vTaskDelay(4000);
#endif

	/* Burn all 6 wires, 1 at a time */
	for (ctrl = 1; ctrl <= 6; ++ctrl)
	{
		vNichromeBurn(ctrl, BURN_TIME);
	}

	for( ;; )
	{
		vTaskDelay(10000);
	}
}
