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

}

bool testWaterLevelSensor()
{

	uint8_t waterLevel = HCSR04_Read();

    if(waterLevel > 0 && waterLevel < 1000)
    {
        return true;
    }else{
        return false;
    }
}

bool testWaterMoistureSensor()
{
	uint16_t moisture = getSoil(&hadc1);
    if(moisture > 0 && moisture < 5000)
    {
        return true;
    }else{
        return false;
    }
}
