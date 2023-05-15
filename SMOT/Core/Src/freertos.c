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
#include <stdbool.h>
#include <func.h>
#include <uart.h>
#include <test.h>
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
uint8_t uartDataToSend[7];
const int BUF_SIZE = 12;
uint8_t uartRecievedDataBeforeFiltering[12];
uint8_t uartRecievedData[7];
uint8_t currentMoistLevel;
uint8_t currentWaterLevel;

uint8_t targetMoisture = 0;
// uint8_t pumpTime = 0;
uint8_t pumpTrigger = 0;
uint8_t test = 1;
uint8_t suspend = 0;

uint8_t LOW_LED = 20;
uint8_t MED_LED = 40;
uint8_t HIGH_LED = 60;
uint8_t ledCase = 1;

void waterPlantTask(void *argument);
void uartParser();


// uint8_t motorRunning = 0;
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
/* Definitions for readUartRXBuffe */
osThreadId_t readUartRXBuffeHandle;
const osThreadAttr_t readUartRXBuffe_attributes = {
  .name = "readUartRXBuffe",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for uartRecieveMutex */
osMutexId_t uartRecieveMutexHandle;
const osMutexAttr_t uartRecieveMutex_attributes = {
  .name = "uartRecieveMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osThreadId_t waterPlantHandle;

const osThreadAttr_t waterPlant_attributes = {
    .name = "waterPlant",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

osMutexId_t global_mutex_id;
const osMutexAttr_t Thread_Mutex_attr = {
    "myThreadMutex",     // human readable mutex name
    osMutexPrioInherit,  // attr_bits
    NULL,                // memory for control block
    0U                   // size for control block
};

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Suspend(void *argument);
void readSensor(void *argument);
void modeSelectButt(void *argument);
void readUartTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of uartRecieveMutex */
  uartRecieveMutexHandle = osMutexNew(&uartRecieveMutex_attributes);

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

  /* creation of readUartRXBuffe */
  readUartRXBuffeHandle = osThreadNew(readUartTask, NULL, &readUartRXBuffe_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    waterPlantHandle = osThreadNew(waterPlantTask, NULL, &waterPlant_attributes);
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

    uartDataToSend[0] = 254;

    uartDataToSend[6] = 127;
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 75;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    /* Infinite loop */
    for(;;)
    {
    	test = 2;
        // Wait for the next cycle.
        vTaskDelayUntil( &xLastWakeTime, xFrequency );

        osMutexAcquire(uartRecieveMutexHandle, osWaitForever);
        uartTransmit(uartDataToSend);
        osMutexRelease(uartRecieveMutexHandle);
        uartRecieve(uartRecievedDataBeforeFiltering);
        //uartRecieve(uartRecievedData);
        if(pumpTrigger == 1 && motorRunning == 0 && manualWatering == 1)
        {
            runPump(pumpSeconds);
            //reset all pumpTrigger values
            //pumpTrigger = 0;
            //uartRecievedData[2] = 0;
        }
        //osMutexAcquire(uartRecieveMutexHandle, osWaitForever);



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
    const TickType_t xFrequency = 10;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    /* Infinite loop */
    for(;;)
    {
    	test = 8;
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        /* currentMoistLevel = getSoil(&hadc1); */
        currentMoistLevel = 4;

        currentWaterLevel = getWaterPercent();


        motorRunning = (uint8_t) HAL_GPIO_ReadPin(PUMP_GPIO_Port, PUMP_Pin);

        osMutexAcquire(uartRecieveMutexHandle, osWaitForever);
        uartDataToSend[1] = getSoil(&hadc1);
        uartDataToSend[2] = currentWaterLevel;
        uartDataToSend[5] = motorRunning;
        osMutexRelease(uartRecieveMutexHandle);

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

/* USER CODE BEGIN Header_readUartTask */
/**
 * @brief Function implementing the readUartRXBuffe thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_readUartTask */
void readUartTask(void *argument)
{
  /* USER CODE BEGIN readUartTask */

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 5;
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    bool readyForTrigger = true;
    /* Infinite loop */
    for(;;)
    {
    	//test = 4;
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        //osMutexAcquire(uartRecieveMutexHandle, osWaitForever);
        uartParser();
        osMutexAcquire(global_mutex_id, osWaitForever);
        targetMoisture = uartRecievedData[0];

        pumpSeconds = uartRecievedData[1];

        pumpTrigger = uartRecievedData[2];
        osMutexRelease(global_mutex_id);
        if(pumpTrigger == 1 && readyForTrigger)
        {
            manualWatering = 1;
        }
        if(pumpTrigger == 1)
        {
            readyForTrigger = false;
        }else if(pumpTrigger == 0 && !motorRunning && manualWatering == 0)
        {
            readyForTrigger = true;
        }

        /* if(pumpTrigger == 1 && !motorRunning) */
        //osMutexRelease(uartRecieveMutexHandle);
        automaticWatering = uartRecievedData[3];

        osDelay(1);
    }
  /* USER CODE END readUartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //osMutexRelease(uartRecieveMutexHandle);
}

void waterPlantHelper(uint8_t curMoist, uint8_t waterLevel, int time_between_waterings, int targetMoistureCopy, float *iTerm, int* previous_error);

float iTerm = 0;
void waterPlantTask(void *argument)
{
  osDelay(5); // waiting abit before starting just in case. :)
  // initializing some variables

    int previous_error = 0;
    //int current_moisture = 0; trying to use global instead
    int moisture_target; //inställningen som bestämmer hur mycket fukt det skall vara

  int time_between_waterings = 10000; // 15 min in microseconds
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 10000; // time between waterings = 15 min in miliseconds
  xLastWakeTime = xTaskGetTickCount();
  //int previous_error;
  for (;;)
  {
  /* test = 77; */
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
    // writing global variables into locals
    osMutexAcquire(global_mutex_id, osWaitForever);
    uint8_t curMoist = getSoil(&hadc1);
    uint8_t waterLevel = currentWaterLevel;
    uint8_t targetMoistureCopy = targetMoisture;   
    osMutexRelease(global_mutex_id);

    if(automaticWatering && !manualWatering)
    {
        waterPlantHelper(curMoist, waterLevel, time_between_waterings, targetMoistureCopy, &iTerm, &previous_error);
    }
    osDelay(1);
  }
}

uint8_t output = 0;
void waterPlantHelper(uint8_t curMoist, uint8_t waterLevel, int time_between_waterings, int targetMoistureCopy, float *iTerm, int* previous_error)
{
  float pK = 1.0;      // inställing som bestämmer hur mycket P värdet påverkar slutvärdet
  float iK = 0.00000; // inställning som bestämmer hur mycket I värdet påverkar slutvärdet
  float iMax = 30;     // i_max värde så att den inte gör något knäppt
  float scalar = 0.6;  // konverteringsfaktor för att konvertera från output till sekunder pumpande

  // output;

  if (curMoist >= targetMoistureCopy)
  { // reset iTerm when target reached as it is not needed.
    *iTerm *= 0.8f;
  }

  /* if (waterLevel > 4 && curMoist < targetMoistureCopy) */
  if (curMoist < targetMoistureCopy)
  { // if water level is below 4 cm and moisture is below target
    int error = targetMoistureCopy - curMoist;
    int pTerm = pK * error;
    *iTerm += ((*previous_error + error) * 0.5f * time_between_waterings) * iK;
    if (*iTerm > iMax)
    {
      *iTerm = iMax;
    }
    output = (pTerm + *iTerm) * scalar;
    if (output <= 0 || output > 20000)
    { // kan inte köra pumpen negativa sekunder och inte för många heller
      output = 0;
    }
        runPump(output);
    *previous_error = error;
  }

}


uint8_t startBlock = 255;
/* uint8_t endBlock = 255; */
bool foundStart = false;
/* bool foundEnd = false; */
void uartParser()
{
    //uint8_t startBlock = 255;

foundStart = false;
/* foundEnd = false; */

    for(int i = 0; i < BUF_SIZE; i++)
    {
        if(uartRecievedDataBeforeFiltering[i] == 254 && !foundStart)
        {
            startBlock = i+1;
            foundStart = true;
            break;
        }
    }
    /* if(foundStart) */
    /* { */
    /*     for(int i = startBlock; i < BUF_SIZE; i++) */
    /*     { */
    /*         if(i == startBlock + 5 && uartRecievedDataBeforeFiltering[i] == 254) */
    /*         { */
    /*             endBlock = i; */
    /*             foundEnd = true; */
    /*             break; */
    /*         } */
    /*     } */
    /* } */

    if(foundStart)
    {
        for(int i = 0; i < BUF_SIZE; i++)
        {
            uartRecievedData[i] = uartRecievedDataBeforeFiltering[startBlock + i];
        }
    }
}

void updateLED()
{

	if(targetMoisture <= LOW_LED){
		ledCase = 1;
	}
	else if( LOW_LED <= targetMoisture && targetMoisture <= HIGH_LED){
			ledCase = 2;
		}
	else if(HIGH_LED <= targetMoisture){
			ledCase = 3;
		}
	turnOnLed(ledCase);
}

/* USER CODE END Application */

