#ifndef __PINMAPPING_H
#define __PINMAPPING_H

/*! Pin mapping from IO.X to STM32 GPIO and GPIO_Pin numbers
 * 
 * Example: IO.47 -> PD3, so IO_47_GPIO -> GPIOD
 * and IO_47_PIN -> GPIO_Pin_3
 * */

#include <stm32f4xx_gpio.h>

/*! Mapping for the GPIO peripheral
 * */
#define IO_47_GPIO	GPIOD
#define IO_46_GPIO	GPIOD


/*! Mapping for the GPIO Pin Number
 * */
#define IO_47_PIN		GPIO_Pin_3
#define IO_46_PIN		GPIO_Pin_2

#endif
