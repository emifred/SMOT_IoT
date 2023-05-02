/*
 * func.h
 *
 *  Created on: Apr 18, 2023
 *      Author: emil
 */

#ifndef INC_FUNC_H_
#define INC_FUNC_H_
#include <stm32l4xx.h>
#include <main.h>
uint16_t getSoil(ADC_HandleTypeDef *hadc);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
uint16_t getWaterLevel(void);
void updateLED();
void setMoisture(uint8_t data);

#endif /* INC_FUNC_H_ */