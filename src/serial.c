/* serial.c
 * 
 * USART driver for FreeRTOS
 * 
 */

#include "serial.h"

#include "stm32f4xx_usart.h"


void serialInit()
{
    USART_InitTypeDef usartConfig;
    GPIO_InitTypeDef gpioConfig;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    usartConfig.USART_WordLength = USART_WordLength_8b;
    usartConfig.USART_StopBits = USART_StopBits_2;
    usartConfig.USART_Parity = USART_Parity_No;
    usartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartConfig.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    /* Configure and Enable USART2 */
    usartConfig.USART_BaudRate = serialUSART2_BAUDRATE;
    
    USART_Init(USART2, &usartConfig);
    USART_Cmd(USART2, ENABLE);
    
    /* Configure GPIOs for USART2 (Use PA2 and PA3) */
    gpioConfig.GPIO_Speed = GPIO_Speed_100MHz;
    gpioConfig.GPIO_Mode = GPIO_Mode_AF;
    gpioConfig.GPIO_OType = GPIO_OType_PP;
    gpioConfig.GPIO_PuPd = GPIO_PuPd_UP;
    
    gpioConfig.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &gpioConfig);
    
    gpioConfig.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &gpioConfig);
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    
    for(;;)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, 'a');
    }
}