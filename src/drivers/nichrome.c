#include <drivers/nichrome.h>
#include <stm32f4xx_hal.h>
#include <error.h>
#include <printf.h>

static GPIO_TypeDef* get_GPIO_for_burner(enum NICHROME_Burner);
static int get_pin_for_burner(enum NICHROME_Burner);
static GPIO_InitTypeDef GPIO_InitStruct;

int NICHROME_Init() {
	/*
	 *TODO
	 * -GPIO Initialization via HAL
	 * -Create Handles
	 * -Initializes the perhiperal
	 */

	/* 
 	* Questions:
 	* 1) Does HAL_Init() need to be called here?
 	* 2)
 	*/

	/* Hacky RCC initialization - ideally handled with some
	 * defines
	 */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	dbg_printf("Initializing NICHROME BURNERS...");
	// TODO: Restore full burner init (currently avoids unimplmented errors with 2-6) BCSK
	// for (int burner = NICHROME_Burner_1; burner <= NICHROME_Burner_6; burner++) {
	for (int burner = NICHROME_Burner_1; burner == NICHROME_Burner_1; burner++) {
		/* Get Register and Pin for this burner */
		GPIO_TypeDef* GPIOx = get_GPIO_for_burner(burner);
		int pin = get_pin_for_burner(burner);

		/* Populate GPIO_InitStruct for this burner */
		GPIO_InitStruct.Pin = pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

		dbg_printf("Initializing Burner (%d)...", burner+1);
		/* Initialize the GPIO for this burner */
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}

	dbg_printf("NICHROME BURNERS Ready");
	return 1;
}

void NICHROME_On(enum NICHROME_Burner burner) {
    dbg_printf("Turning Nichrome on (%d)", burner+1);
	GPIO_TypeDef* gpiox =  get_GPIO_for_burner(burner);
    int pin = get_pin_for_burner(burner);
	HAL_GPIO_WritePin(gpiox, pin, GPIO_PIN_SET);
}

void NICHROME_Off (enum NICHROME_Burner burner) {
    dbg_printf("Turning Nichrome off (%d)", burner+1);
	GPIO_TypeDef* gpiox =  get_GPIO_for_burner(burner);
    int pin = get_pin_for_burner(burner);
	HAL_GPIO_WritePin(gpiox, pin, GPIO_PIN_RESET);
}

static GPIO_TypeDef* get_GPIO_for_burner(enum NICHROME_Burner burner) {
    switch (burner) {
        case NICHROME_Burner_1:
           return GPIOA;

	    case NICHROME_Burner_2:
			//TODO Return a register. For now, throw error
        	ERROR_NotImplemented("No GPIO_TypeDef is defined for NICHROME_Burner_2");
			break;
	
		case NICHROME_Burner_3:
			ERROR_NotImplemented("No GPIO_TypeDef is defined for NICHROME_Burner_3");
			break;

		case NICHROME_Burner_4:
			ERROR_NotImplemented("No GPIO_TypeDef is defined for NICHROME_Burner_4");
			break;

		case NICHROME_Burner_5:
			ERROR_NotImplemented("No GPIO_TypeDef is defined for NICHROME_Burner_5");
			break;

		case NICHROME_Burner_6:
			ERROR_NotImplemented("No GPIO_TypeDef is defined for NICHROME_Burner_6");
			break;
		
    }

	// This is temporary
	return GPIOA;

}

static int get_pin_for_burner(enum NICHROME_Burner burner) {
	switch(burner) {
		case NICHROME_Burner_1:
			return GPIO_PIN_5;

		case NICHROME_Burner_2:
			ERROR_NotImplemented("No GPIO Pin is defined");
			break;

		case NICHROME_Burner_3:
			ERROR_NotImplemented("No GPIO Pin is defined");
			break;

		case NICHROME_Burner_4:
			ERROR_NotImplemented("No GPIO Pin is defined");
			break;

		case NICHROME_Burner_5:
			ERROR_NotImplemented("No GPIO Pin is defined");
			break;

		case NICHROME_Burner_6:
			ERROR_NotImplemented("No GPIO Pin is defined");
			break;
		
	}
	
	// This is temporary
	return GPIO_PIN_5;

}
