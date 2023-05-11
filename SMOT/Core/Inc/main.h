/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
extern uint8_t pumpSeconds;
extern uint8_t pumpSecondsAutomatic;
extern uint8_t pumpTimerCount;
extern uint8_t manualWatering;
extern uint8_t automaticWatering;
extern uint8_t motorRunning;
extern uint8_t targetMoisture;
extern uint8_t suspend;

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern ADC_HandleTypeDef hadc1;

extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart1;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delay_us (uint16_t us);
void updateLED();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MODE_Pin GPIO_PIN_0
#define MODE_GPIO_Port GPIOC
#define MODE_EXTI_IRQn EXTI0_IRQn
#define SUSPEND_Pin GPIO_PIN_1
#define SUSPEND_GPIO_Port GPIOC
#define SUSPEND_EXTI_IRQn EXTI1_IRQn
#define PUMP_Pin GPIO_PIN_2
#define PUMP_GPIO_Port GPIOC
#define TEMP_HUM_Pin GPIO_PIN_3
#define TEMP_HUM_GPIO_Port GPIOC
#define HIGH_LED_Pin GPIO_PIN_1
#define HIGH_LED_GPIO_Port GPIOA
#define MED_LED_Pin GPIO_PIN_4
#define MED_LED_GPIO_Port GPIOA
#define LOW_LED_Pin GPIO_PIN_0
#define LOW_LED_GPIO_Port GPIOB
#define GPIO_Output_Pin GPIO_PIN_9
#define GPIO_Output_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
