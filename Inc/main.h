/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define RADIO_TR_Pin GPIO_PIN_0
#define RADIO_TR_GPIO_Port GPIOC
#define RADIO_RR_Pin GPIO_PIN_1
#define RADIO_RR_GPIO_Port GPIOC
#define RADIO_TX_Pin GPIO_PIN_2
#define RADIO_TX_GPIO_Port GPIOA
#define RADIO_RX_Pin GPIO_PIN_3
#define RADIO_RX_GPIO_Port GPIOA
#define SYS_NSS_Pin GPIO_PIN_4
#define SYS_NSS_GPIO_Port GPIOA
#define SYS_SCK_Pin GPIO_PIN_5
#define SYS_SCK_GPIO_Port GPIOA
#define SYS_MISO_Pin GPIO_PIN_6
#define SYS_MISO_GPIO_Port GPIOA
#define SYS_MOSI_Pin GPIO_PIN_7
#define SYS_MOSI_GPIO_Port GPIOA
#define MEM_CS3_Pin GPIO_PIN_0
#define MEM_CS3_GPIO_Port GPIOB
#define MEM_CS2_Pin GPIO_PIN_1
#define MEM_CS2_GPIO_Port GPIOB
#define MEM_CS0_Pin GPIO_PIN_12
#define MEM_CS0_GPIO_Port GPIOB
#define MEM_SCK_Pin GPIO_PIN_13
#define MEM_SCK_GPIO_Port GPIOB
#define MEM_MISO_Pin GPIO_PIN_14
#define MEM_MISO_GPIO_Port GPIOB
#define MEM_MOSI_Pin GPIO_PIN_15
#define MEM_MOSI_GPIO_Port GPIOB
#define UART2_TX_Pin GPIO_PIN_6
#define UART2_TX_GPIO_Port GPIOC
#define UART2_RX_Pin GPIO_PIN_7
#define UART2_RX_GPIO_Port GPIOC
#define NYIELD_P_Pin GPIO_PIN_8
#define NYIELD_P_GPIO_Port GPIOA
#define PTX_SRX_Pin GPIO_PIN_9
#define PTX_SRX_GPIO_Port GPIOA
#define PRX_STX_Pin GPIO_PIN_10
#define PRX_STX_GPIO_Port GPIOA
#define SYS_RESET_Pin GPIO_PIN_11
#define SYS_RESET_GPIO_Port GPIOA
#define NRST_S_Pin GPIO_PIN_15
#define NRST_S_GPIO_Port GPIOA
#define MEM_NWP_Pin GPIO_PIN_4
#define MEM_NWP_GPIO_Port GPIOB
#define MEM_NHOLD_Pin GPIO_PIN_5
#define MEM_NHOLD_GPIO_Port GPIOB
#define SYS_SCL_Pin GPIO_PIN_6
#define SYS_SCL_GPIO_Port GPIOB
#define SYS_SDA_Pin GPIO_PIN_7
#define SYS_SDA_GPIO_Port GPIOB
#define MEM_CS1_Pin GPIO_PIN_8
#define MEM_CS1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
