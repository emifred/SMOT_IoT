/*
 * automatic.c
 *
 *  Created on: 2 May 2023
 *      Author: alexander
 */

//i task code senare
/*
//initializing some variables first time :)
float iTerm = 0;
int previous_error = 0;
int current_moisture = 0;
int moisture_target; //inställningen som bestämmer hur mycket fukt det skall vara
float pK = 1; //inställing som bestämmer hur mycket P värdet påverkar slutvärdet
float iK = 0,05; //inställning som bestämmer hur mycket I värdet påverkar slutvärdet
float scalar = 0.0001 //konverteringsfaktor för att konvertera från output till sekunder pumpande
float iMax = 300 //i_max värde så att den inte gör något knäppt
int time_between_waterings = 900000; //15 min in microseconds
while(true) {
 	Task_sleep(time_between_waterings) (15 min) later
	waterPlant();
}
*/
/*
#include <stdio.h>
#include <stdbool.h>
#include <func.h>
#include <stm32l4xx.h>
void waterPlant() {
	int output = 0;
	if (current_moisture >= moisture_target) {
			iTerm = 0;
	}
	if(getWaterLevel() < 4 && current_moisture < moisture_target) { //if water level is below 4 cm and moisture is below target
		int error = moisture_target - current_moisture;
		int pTerm = pK * error;

		iTerm += ((previous_error + error) * 0.5f * time_between_waterings) * iK;

		if(iTerm > iMax)
		{
			iTerm = iMax;
		}
		
		output = (pTerm + iTerm) * scalar;

		if(output <= 0 || output > 20) { //kan inte köra pumpen negativa sekunder
			output = 0
		}

		runPump(output);

		previous_error = error;
	}
}
*/
