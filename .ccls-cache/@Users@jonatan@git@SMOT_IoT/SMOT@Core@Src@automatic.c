/*
 * automatic.c
 *
 *  Created on: 2 May 2023
 *      Author: alexander
 */


/*
#include <stdio.h>
#include <stdbool.h>
#include <func.h>
#include <stm32l4xx.h>


//maybe use mutex :(

#include "cmsis_os2.h"

osMutexId_t global_mutex_id;
const osMutexAttr_t Thread_Mutex_attr = {
  "myThreadMutex",     // human readable mutex name
  osMutexPrioInherit,  // attr_bits
  NULL,                // memory for control block
  0U                   // size for control block
};
global_mutex_id = osMutexNew(&Thread_Mutex_attr);




//trying to make thread code :)


osThreadId_t waterPlantHandle;
const osThreadAttr_t waterPlant_attributes = {
  .name = "waterPlant",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
//change prio here??
void waterPlant(void *argument);
waterPlantHandle = osThreadNew(waterPlant, NULL, &waterPlant_attributes);
void waterPlant(void *argument)
{
	osDelay(5); //waiting abit before starting just in case. :)
	//initializing some variables
	float iTerm = 0;
	int previous_error = 0;
	//int current_moisture = 0; trying to use global instead
	int moisture_target; //inställningen som bestämmer hur mycket fukt det skall vara
	float pK = 1.0; //inställing som bestämmer hur mycket P värdet påverkar slutvärdet
	float iK = 0.05; //inställning som bestämmer hur mycket I värdet påverkar slutvärdet
	float iMax = 30; //i_max värde så att den inte gör något knäppt
	float scalar = 0.04; //konverteringsfaktor för att konvertera från output till sekunder pumpande
	int output = 0;
	// int time_between_waterings = 900000; 15 min in microseconds
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 900000; //time between waterings = 15 min in miliseconds
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
  	{
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		//writing global variables into locals
		osMutexAcquire(global_mutex_id, osWaitForever);
		int curMoist = currentMoistLevel;
		int waterLevel = currentWaterLevel;
		//get moisture target from somewhere here to...
		osMutexRelease(global_mutex_id);
		waterPlantHelper(curMoist, waterLevel);
		osDelay(1);
	}



void waterPlantHelper(int curMoist, int waterLevel) {
	if (curMoist >= moisture_target) { //reset iTerm when target reached as it is not needed.
			iTerm = 0;
	}
	if(waterLevel < 4 && curMoist < moisture_target) { //if water level is below 4 cm and moisture is below target
		int error = moisture_target - curMoist;
		int pTerm = pK * error;
		iTerm += ((previous_error + error) * 0.5f * time_between_waterings) * iK;
		if(iTerm > iMax)
		{
			iTerm = iMax;
		}
		output = (pTerm + iTerm) * scalar;
		if(output <= 0 || output > 20) { //kan inte köra pumpen negativa sekunder och inte för många heller
			output = 0;
		}
		runPump(output);
		previous_error = error;
	}
}
*/
