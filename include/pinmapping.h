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

#define IO_41_GPIO  GPIOC

#define IO_39_GPIO  GPIOC

#define IO_37_GPIO  GPIOC

#define IO_29_GPIO  GPIOA

#define IO_27_GPIO  GPIOA

#define IO_25_GPIO  GPIOE




/*! Mapping for the GPIO Pin Number
 * */
#define IO_47_PIN		GPIO_Pin_3
#define IO_46_PIN		GPIO_Pin_2

#define IO_41_PIN   GPIO_Pin_7

#define IO_39_PIN   GPIO_Pin_0

#define IO_37_PIN   GPIO_Pin_2

#define IO_29_PIN   GPIO_Pin_5

#define IO_27_PIN   GPIO_Pin_9

#define IO_25_PIN   GPIO_Pin_1


#endif
