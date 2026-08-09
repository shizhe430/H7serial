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
#include "stm32h7xx_hal.h"

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
#define AS608_WAK_Pin GPIO_PIN_6
#define AS608_WAK_GPIO_Port GPIOE
#define AMBIENT_LED2_Pin GPIO_PIN_2
#define AMBIENT_LED2_GPIO_Port GPIOH
#define AMBIENT_LED1_Pin GPIO_PIN_5
#define AMBIENT_LED1_GPIO_Port GPIOH
#define LCD_BL_Pin GPIO_PIN_5
#define LCD_BL_GPIO_Port GPIOB
#define CAMERA_LIGHT_Pin GPIO_PIN_1
#define CAMERA_LIGHT_GPIO_Port GPIOA
#define DS18B20_DQ_Pin GPIO_PIN_12
#define DS18B20_DQ_GPIO_Port GPIOB
#define PUMP_PWM_Pin GPIO_PIN_0
#define PUMP_PWM_GPIO_Port GPIOI
#define TANK_LEVEL_Pin GPIO_PIN_1
#define TANK_LEVEL_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */
#ifndef CAMERA_FINGERPRINT_ENABLE
#define CAMERA_FINGERPRINT_ENABLE 0U
#endif

#define SDRAM_OFFICIAL_CLOCK_DIAG 1U
#ifndef SDRAM_50MHZ_DIAG
#define SDRAM_50MHZ_DIAG 1U
#endif

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
