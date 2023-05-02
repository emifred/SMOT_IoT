/*
 * func.h
 *
 *  Created on: Apr 18, 2023
 *      Author: emil
 */

#ifndef INC_FUNC_H_
#define INC_FUNC_H_
#include <stm32l4xx_hal_conf.h>
#include <main.h>
float read_Soil(ADC_HandleTypeDef *hadc);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
uint16_t HCSR04_Read(void);

#endif /* INC_FUNC_H_ */
