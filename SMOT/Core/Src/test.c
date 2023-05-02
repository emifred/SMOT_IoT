/*
 * test.c
 *
 *  Created on: Apr 18, 2023
 *      Author: emil
 */
#include <stdio.h>
#include <stdbool.h>
#include <func.h>
#include <test.h>
#include <stm32l4xx.h>



void Test_Program(){
	bool waterLevelTest = testWaterLevelFunction();
	bool moistureLevelTest = testMoistureLevelFunction();

}

bool testWaterLevelFunction()
{
	uint8_t waterLevel = HCSR04_Read();
    if(waterLevel > 0 && waterLevel < 1000)
    {
        return true;
    }else{
        return false;
    }
}

bool testMoistureLevelFunction()
{

	uint16_t moisture = readSoil(&hadc1);
    if(moisture > 0 && moisture < 5000)
    {
        return true;
    }else{
        return false;
    }
}
