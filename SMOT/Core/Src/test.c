/*
 * test.c
 *
 *  Created on: Apr 18, 2023
 *      Author: emil
 */
#include <stdio.h>
#include <stdbool.h>
#include <func.h>
#include <stm32l4xx.h>

#include <test.h>


void Test_Program(){
<<<<<<< HEAD
bool waterLevelFunction = false;
bool moistureFunction = true;

waterLevelFunction = testWaterLevelFunction();
moistureFunction = testMoistureFunction();
while(1)
	{
		testSoil = getSoil(&hadc1);
		HAL_Delay(100);
	}

}

bool testWaterLevelFunction()
{

	uint8_t waterLevel = getWaterLevel();

    if(waterLevel > 0 && waterLevel < 1000)
    {
        return true;
    }else{
        return false;
    }
}

bool testMoistureFunction()
{
	uint16_t moisture = getSoil(&hadc1);
    if(moisture > 0 && moisture < 5000)
    {
        return true;
    }else{
        return false;
    }
}
