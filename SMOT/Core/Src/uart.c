#include <uart.h>
#include <func.h>
#include <main.h>


uint8_t* rxData[3];

void uartTransmit(uint8_t* uartDataToSend)
{
    //put data to send in array in correct order


    HAL_UART_Transmit(&huart1, uartDataToSend, 7, 200);
	//HAL_UART_Transmit_IT(&huart1, uartDataToSend, 7);

    //send water level sensor

}

uint8_t* uartRecieve(uint8_t* uartRecievedData)
{
	HAL_UART_Receive_IT(&huart1, uartRecievedData, 3);
	//HAL_UART_Receive(&huart1, uartRecievedData, 3, 200);
}


