/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <func.h>
#include <stm32l4xx.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t uartDataToSend[5];
uint8_t currentMoistLevel;
uint8_t currentWaterLevel;
uint8_t motorRunning = 0;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for suspendButton */
osThreadId_t suspendButtonHandle;
const osThreadAttr_t suspendButton_attributes = {
  .name = "suspendButton",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for readSensors */
osThreadId_t readSensorsHandle;
const osThreadAttr_t readSensors_attributes = {
  .name = "readSensors",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for modeSelect */
osThreadId_t modeSelectHandle;
const osThreadAttr_t modeSelect_attributes = {
  .name = "modeSelect",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Suspend(void *argument);
void readSensor(void *argument);
void modeSelectButt(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of suspendButton */
  suspendButtonHandle = osThreadNew(Suspend, NULL, &suspendButton_attributes);

  /* creation of readSensors */
  readSensorsHandle = osThreadNew(readSensor, NULL, &readSensors_attributes);

  /* creation of modeSelect */
  modeSelectHandle = osThreadNew(modeSelectButt, NULL, &modeSelect_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	 // Wait for the next cycle.
	  vTaskDelayUntil( &xLastWakeTime, xFrequency );
	  uartTransmit();
	  uartReceive();
	  updateLED();

    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Suspend */
/**
* @brief Function implementing the suspendButton thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Suspend */
void Suspend(void *argument)
{
  /* USER CODE BEGIN Suspend */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Suspend */
}

/* USER CODE BEGIN Header_readSensor */
/**
* @brief Function implementing the readSensors thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_readSensor */
void readSensor(void *argument)
{
  /* USER CODE BEGIN readSensor */
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  vTaskDelayUntil( &xLastWakeTime, xFrequency );
	  currentMoistLevel = getSoil(&hadc1);
	  currentWaterLevel = getWaterPercent();
	  motorRunning = HAL_GPIO_ReadPin(PUMP_GPIO_Port, PUMP_Pin);
    osDelay(1);
  }
  /* USER CODE END readSensor */
}

/* USER CODE BEGIN Header_modeSelectButt */
/**
* @brief Function implementing the modeSelect thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_modeSelectButt */
void modeSelectButt(void *argument)
{
  /* USER CODE BEGIN modeSelectButt */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END modeSelectButt */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void uartTransmit()
{

    //put data to send in array in correct order
    uartDataToSend[0] = currentMoistLevel;
    uartDataToSend[1] = currentWaterLevel;
    uartDataToSend[5] = motorRunning;


    //send water level sensor

}
void uartReceive()
{

}
/* USER CODE END Application */

