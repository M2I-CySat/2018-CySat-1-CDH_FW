/* serial.c
 * 
 * USART driver for FreeRTOS
 * 
 */

#include "serial.h"

#include "stm32f4xx_usart.h"
#include "queue.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

static xQueueHandle usart2TxChars;

void serialInit()
{
    usart2TxChars = xQueueCreate(serialTX_QUEUE_SIZE, 1);
    
    USART_InitTypeDef usartConfig;
    GPIO_InitTypeDef gpioConfig;
    NVIC_InitTypeDef nvicConfig;
    
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
    
    /* Enable desired interrupts */
    nvicConfig.NVIC_IRQChannel = USART2_IRQn;
    nvicConfig.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
    nvicConfig.NVIC_IRQChannelSubPriority = 0;
    nvicConfig.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicConfig);
    /* Hardware configuration for USART2 complete */

}

int xSerialPutChar(USART_TypeDef * usart, unsigned char a, portTickType blocktime)
{
    if (usart == USART2)
    {
        USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
        return xQueueSend(usart2TxChars, &a, blocktime);
    }
    return pdFALSE;
}

void USART2_IRQHandler()
{
    BaseType_t taskWoken = pdFALSE;
    char c;
    if (USART_GetITStatus( USART2, USART_IT_TXE) == SET)
    {
        if (xQueueReceiveFromISR(usart2TxChars, &c, &taskWoken) == pdTRUE)
        {
            USART_SendData(USART2, c);
        }
        else
        {
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        }
    }
    portEND_SWITCHING_ISR(taskWoken);
}