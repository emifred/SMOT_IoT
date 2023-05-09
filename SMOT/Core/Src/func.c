/*
 * func.c
 *
 *  Created on: Apr 18, 2023
 *      Author: emil
 */
#include <string.h>
#include <stdio.h>
#include <main.h>
#include <math.h>
#include <tim.h>
#include <stm32l4xx.h>
void delay_us(uint16_t us);


uint16_t Distance;
uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured = 0;  // is the first value captured ?
uint8_t setMoist;
uint8_t waterWarning;
uint8_t emptyWater = 7;
uint8_t motorRunning = 0;


#define TRIG_PIN GPIO_PIN_9
#define TRIG_PORT GPIOA
#define maxWaterLevelCM 2
uint8_t LOW_LED = 20;
uint8_t MED_LED = 40;
uint8_t HIGH_LED = 60;


void setMoisture(uint8_t data)
{
	setMoist = data;
}

void setMoistLed(uint8_t *LED, uint8_t data)
{
	*LED = data;
}

uint8_t getMoistLed(uint8_t *LED)
{
	return *LED;
}
float value_Soil;
uint8_t getSoil(ADC_HandleTypeDef *hadc){
	// Poll ADC1 Perihperal & TimeOut = 1mSec

	uint8_t return_Value;
	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc,100);
	value_Soil = (HAL_ADC_GetValue(hadc));
	if(value_Soil > 4023){
		value_Soil = 4023;
	}
	//value_Soil = value_Soil/4023;
	value_Soil = value_Soil - 1600;
	value_Soil *= (-0.11);
	value_Soil += 108;
	if(value_Soil == 0){
		return_Value = 100;
		return return_Value;
	}
	else{
		//return_Value = (uint8_t) floor((1 - value_Soil)*100);
		return_Value = value_Soil;
		return return_Value;
	}
}
uint16_t controlTime;
void delay (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);

	while (__HAL_TIM_GET_COUNTER (&htim1) < time){
		controlTime = __HAL_TIM_GET_COUNTER (&htim1);
	}
}


// Let's write the callback function

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
	{
		if (Is_First_Captured==0) // if the first value is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured==1)   // if the first is already captured
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffff - IC_Val1) + IC_Val2;
			}

			Distance = Difference * .034/2;
			Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
}

float getWaterLevel(void)
	{
		HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
		delay(10);  // wait for 10 us
		HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

		__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);

		return Distance;
	}

uint8_t getWaterPercent()
{
	float waterLevelPercentage;
	waterLevelPercentage = (1 - ((getWaterLevel()/emptyWater)))*100;
	return waterLevelPercentage;

}
void turnOnLed(uint8_t LED)
{
	switch(LED)
		{
		case 1:
			HAL_GPIO_WritePin(LOW_LED_GPIO_Port, LOW_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(MED_LED_GPIO_Port, MED_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(HIGH_LED_GPIO_Port, HIGH_LED_Pin, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(MED_LED_GPIO_Port, MED_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LOW_LED_GPIO_Port, LOW_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(HIGH_LED_GPIO_Port, HIGH_LED_Pin, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(HIGH_LED_GPIO_Port, HIGH_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(MED_LED_GPIO_Port, MED_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LOW_LED_GPIO_Port, LOW_LED_Pin, GPIO_PIN_RESET);
			break;

		}
}

uint8_t ledCase;
void updateLED()
{
	uint8_t soilValue = getSoil(&hadc1);
	if(soilValue < getMoistLed(&LOW_LED)){
		ledCase = 1;
	}
	if(soilValue < getMoistLed(&MED_LED)){
			ledCase = 2;
		}
	if(soilValue < getMoistLed(&HIGH_LED)){
			ledCase = 3;
		}
	turnOnLed(ledCase);
}

void runPump (uint8_t time)
{
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PIN_SET);
    if(manualWatering == 1)
    {
	    pumpSeconds = time;
    }else if(automaticWatering == 1)
    {
	    pumpSecondsAutomatic = time;
    }
	motorRunning = 1;

}
void stopPump ()
{
	HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PIN_RESET);
	motorRunning = 0;

}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//}


void calibrateUS()
{

	emptyWater = getWaterLevel() - maxWaterLevelCM;

}
