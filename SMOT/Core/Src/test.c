/*
 * test.c
 *
 *  Created on: Apr 18, 2023
 *      Author: emil
 */


Test_Program(){
	bool testWaterLevel = testWaterLevelSensor();
	bool moistureLvel = testMoistureSensor();


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
    uint16_t moisture = readSoil();
    if(moisture > 0 && moisture < 5000)
    {
        return true;
    }else{
        return false;
    }
}
