#include <uart.h>
#include <func.h>
#include <main.h>
/* #include <stdio.h> */

uint8_t uartDataToSend[5];

void uartTransmit()
{
    uint8_t motorRunning = 0;

    //put data to send in array in correct order
    uartDataToSend[0] = getSoil(&hadc1);
    uartDataToSend[1] = getWaterLevel();
    uartDataToSend[5] = motorRunning;

    HAL_Send
    //send water level sensor

}
