/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define nNMI_Pin GPIO_PIN_2
#define nNMI_GPIO_Port GPIOE
#define nIRQ_Pin GPIO_PIN_3
#define nIRQ_GPIO_Port GPIOE
#define nFIRQ_Pin GPIO_PIN_4
#define nFIRQ_GPIO_Port GPIOE
#define MRDY_Pin GPIO_PIN_5
#define MRDY_GPIO_Port GPIOE
#define nHALT_Pin GPIO_PIN_13
#define nHALT_GPIO_Port GPIOC
#define D0_Pin GPIO_PIN_0
#define D0_GPIO_Port GPIOC
#define D1_Pin GPIO_PIN_1
#define D1_GPIO_Port GPIOC
#define D2_Pin GPIO_PIN_2
#define D2_GPIO_Port GPIOC
#define D3_Pin GPIO_PIN_3
#define D3_GPIO_Port GPIOC
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOC
#define D5_Pin GPIO_PIN_5
#define D5_GPIO_Port GPIOC
#define A8_Pin GPIO_PIN_8
#define A8_GPIO_Port GPIOD
#define A9_Pin GPIO_PIN_9
#define A9_GPIO_Port GPIOD
#define A10_Pin GPIO_PIN_10
#define A10_GPIO_Port GPIOD
#define A11_Pin GPIO_PIN_11
#define A11_GPIO_Port GPIOD
#define A12_Pin GPIO_PIN_12
#define A12_GPIO_Port GPIOD
#define A13_Pin GPIO_PIN_13
#define A13_GPIO_Port GPIOD
#define A14_Pin GPIO_PIN_14
#define A14_GPIO_Port GPIOD
#define A15_Pin GPIO_PIN_15
#define A15_GPIO_Port GPIOD
#define D6_Pin GPIO_PIN_6
#define D6_GPIO_Port GPIOC
#define D7_Pin GPIO_PIN_7
#define D7_GPIO_Port GPIOC
#define RnW_Pin GPIO_PIN_8
#define RnW_GPIO_Port GPIOC
#define E_Pin GPIO_PIN_9
#define E_GPIO_Port GPIOC
#define Q_Pin GPIO_PIN_10
#define Q_GPIO_Port GPIOC
#define BS_Pin GPIO_PIN_11
#define BS_GPIO_Port GPIOC
#define BA_Pin GPIO_PIN_12
#define BA_GPIO_Port GPIOC
#define A0_Pin GPIO_PIN_0
#define A0_GPIO_Port GPIOD
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOD
#define A2_Pin GPIO_PIN_2
#define A2_GPIO_Port GPIOD
#define A3_Pin GPIO_PIN_3
#define A3_GPIO_Port GPIOD
#define A4_Pin GPIO_PIN_4
#define A4_GPIO_Port GPIOD
#define A5_Pin GPIO_PIN_5
#define A5_GPIO_Port GPIOD
#define A6_Pin GPIO_PIN_6
#define A6_GPIO_Port GPIOD
#define A7_Pin GPIO_PIN_7
#define A7_GPIO_Port GPIOD
#define nDMA_Pin GPIO_PIN_0
#define nDMA_GPIO_Port GPIOE
#define nRESET_Pin GPIO_PIN_1
#define nRESET_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
