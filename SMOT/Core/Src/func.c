/*
 * func.c
 *
 *  Created on: Apr 18, 2023
 *      Author: emil
 */
#include <string.h>
#include <stdio.h>
#include "main.h"
#include <stm32l4xx_hal_conf.h>
void delay_us(uint16_t us);


float read_Soil(ADC_HandleTypeDef *hadc){
	// Poll ADC1 Perihperal & TimeOut = 1mSec
	float value_Soil;
	uint16_t return_Value;
	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc,100);
	value_Soil = (HAL_ADC_GetValue(hadc));
	if(value_Soil > 4023){
		value_Soil = 4023;
	}
	value_Soil = value_Soil/4023;
	if(value_Soil == 0){
		return_Value = 100;
		return return_Value;
	}
	else
		return_Value = (1 - value_Soil)*100;
		return return_Value;
}



uint16_t korv(){
	 uint16_t count=0;
	 HAL_GPIO_WritePin(GPIOC, trig_Pin, GPIO_PIN_SET);
	 //delay_us(10);
	 HAL_GPIO_WritePin(GPIOC, trig_Pin, GPIO_PIN_RESET);
	 while (HAL_GPIO_ReadPin(GPIOC, echo_Pin)){
		 count++;
	 }
	 return count;
}


