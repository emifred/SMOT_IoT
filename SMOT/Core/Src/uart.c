#include <uart.h>
#include <func.h>
#include <main.h>
/* #include <stdio.h> */

uint8_t uartDataToSend[6];

void uartTransmit()
{
    //get soil moisture and split it into separate bytes
    uartDataToSend[0] = getSoil(&hadc1);
    uartDataToSend[1] = getWaterLevel();

    //send water level sensor

}
