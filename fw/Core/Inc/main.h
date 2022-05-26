/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define BAT_DETEC_Pin GPIO_PIN_0
#define BAT_DETEC_GPIO_Port GPIOC
#define POWER_CONTROL_Pin GPIO_PIN_1
#define POWER_CONTROL_GPIO_Port GPIOC
#define RECORDING_STOP_Pin GPIO_PIN_2
#define RECORDING_STOP_GPIO_Port GPIOC
#define PLAY_Pin GPIO_PIN_3
#define PLAY_GPIO_Port GPIOC
#define FWD_1_Pin GPIO_PIN_0
#define FWD_1_GPIO_Port GPIOA
#define BWD_1_Pin GPIO_PIN_1
#define BWD_1_GPIO_Port GPIOA
#define FWD_2_Pin GPIO_PIN_2
#define FWD_2_GPIO_Port GPIOA
#define BWD_2_Pin GPIO_PIN_3
#define BWD_2_GPIO_Port GPIOA
#define SHUTDOWN_Pin GPIO_PIN_4
#define SHUTDOWN_GPIO_Port GPIOA
#define LEFT_Pin GPIO_PIN_5
#define LEFT_GPIO_Port GPIOA
#define PA6_TEMPERATURE_Pin GPIO_PIN_6
#define PA6_TEMPERATURE_GPIO_Port GPIOA
#define RIGHT_Pin GPIO_PIN_7
#define RIGHT_GPIO_Port GPIOA
#define UP_Pin GPIO_PIN_4
#define UP_GPIO_Port GPIOC
#define DOWN_Pin GPIO_PIN_5
#define DOWN_GPIO_Port GPIOC
#define ENTER_Pin GPIO_PIN_0
#define ENTER_GPIO_Port GPIOB
#define LCD_RESET_Pin GPIO_PIN_10
#define LCD_RESET_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_11
#define LCD_DC_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SCK_Pin GPIO_PIN_13
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_MISO_Pin GPIO_PIN_14
#define LCD_MISO_GPIO_Port GPIOB
#define LCD_MOSI_Pin GPIO_PIN_15
#define LCD_MOSI_GPIO_Port GPIOB
#define LCD_LED_Pin GPIO_PIN_6
#define LCD_LED_GPIO_Port GPIOC
#define S1_Pin GPIO_PIN_7
#define S1_GPIO_Port GPIOC
#define S2_Pin GPIO_PIN_8
#define S2_GPIO_Port GPIOC
#define S3_Pin GPIO_PIN_9
#define S3_GPIO_Port GPIOC
#define FWD_3_Pin GPIO_PIN_8
#define FWD_3_GPIO_Port GPIOA
#define BWD_3_Pin GPIO_PIN_9
#define BWD_3_GPIO_Port GPIOA
#define FWD_4_Pin GPIO_PIN_10
#define FWD_4_GPIO_Port GPIOA
#define BWD_4_Pin GPIO_PIN_11
#define BWD_4_GPIO_Port GPIOA
#define S4_Pin GPIO_PIN_12
#define S4_GPIO_Port GPIOA
#define S5_Pin GPIO_PIN_15
#define S5_GPIO_Port GPIOA
#define S6_Pin GPIO_PIN_10
#define S6_GPIO_Port GPIOC
#define S7_Pin GPIO_PIN_11
#define S7_GPIO_Port GPIOC
#define S8_Pin GPIO_PIN_12
#define S8_GPIO_Port GPIOC
#define RTC_SCL_Pin GPIO_PIN_6
#define RTC_SCL_GPIO_Port GPIOB
#define RTC_SDA_Pin GPIO_PIN_7
#define RTC_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
