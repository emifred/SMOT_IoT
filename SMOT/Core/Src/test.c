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
uint8_t testSoil;
/* bool testWaterLevelFunction(); */
/* bool testMoistureFunction(); */

void Test_Program(){
    bool waterLevelTestVar = false;
    bool moistureTestVar = false;
    bool uartTestVar = false;


    uint8_t waterLevel1 = getWaterLevel();
    waterLevelTestVar = testWaterLevelFunction();
    moistureTestVar = testMoistureLevelFunction();
    for(int i = 0; i < 4; i++)
    {
    uartTestVar = testUartSend();
    HAL_Delay(1000);
}

    uint8_t testSoil = 255;
    while(1)
    {
        testSoil = getSoil(&hadc1);
        HAL_Delay(100);
        ledTest();
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

bool testMoistureLevelFunction()
{
    uint16_t moisture = getSoil(&hadc1);
    if(moisture > 0 && moisture < 5000)
    {
        return true;
    }else{
        return false;
    }
}


bool testUartSend()
{
    uint8_t dataToSend[5];
    for(int i = 0; i<5; i++)
    {
        dataToSend[i] = i+1;
    }
    if(HAL_UART_Transmit(&huart1, dataToSend, 5, 2000) == 0x00)
    {
        return true;
    }else{
        return false;
    }

    void ledTest(){
    	turnOnLed(1);
    	HAL_Delay(1000);
    	turnOnLed(2);
    	HAL_Delay(1000);
    	turnOnLed(3);
    	HAL_Delay(1000);
    }
