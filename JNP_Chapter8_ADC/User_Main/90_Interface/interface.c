#include "main.h"

uint8_t TX_Data[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};
uint8_t TX_Count = 0;

void system_interfcae_tx(void)
{
	HAL_UART_Transmit_IT(&huart3, &TX_Data[TX_Count],1);
	TX_Count++;
	if(TX_Count >= 10)
	{
		TX_Count = 0;
	}
}
