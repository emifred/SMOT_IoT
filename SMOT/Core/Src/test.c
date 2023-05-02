/*
 * test.c
 *
 *  Created on: Apr 18, 2023
 *      Author: emil
 */


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

bool testWaterLevelSensor()
{
    float moisture = readSoil();
    if(moisture > 0 && moisture < 5000)
    {
        return true;
    }else{
        return false;
    }
}
