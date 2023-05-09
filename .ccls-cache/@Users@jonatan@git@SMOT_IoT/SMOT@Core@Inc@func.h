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
float getWaterLevel(void);
void updateLED();
void setMoisture(uint8_t data);
void setMoistLed(uint8_t *LED, uint8_t data);
uint8_t getMoistLed(uint8_t *LED);

void runPump (uint8_t time);
void stopPump ();
void calibrateUS();
uint8_t getWaterPercent();
void turnOnLed(uint8_t LED);


#endif /* INC_FUNC_H_ */
