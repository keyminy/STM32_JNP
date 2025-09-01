#include "main.h"

struct mcu_structure mcu;
uint8_t TX_Data = 0;


void user_main(void)
{
	HAL_UART_Receive_IT(&huart3,&mcu.interface.uart_rx_data , 1);

	while(1)
	{
		HAL_UART_Transmit_IT(&huart3, &TX_Data, 1);
	}
}
