#include "main.h"

struct mcu_structure mcu;

void user_main(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
//	HAL_UART_Receive_IT(&huart3,&mcu.interface.uart_rx_data , 1);
	HAL_UART_Receive_DMA(&huart3,&mcu.interface.uart_rx_data , 1);
	// UART TX 인터럽트 활성화
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_TC);
	while(1)
	{
//		if(mcu.interface.uart_500ms_flag == 1)
//		{
//			mcu.interface.uart_500ms_flag = 0;
//			HAL_UART_Transmit_IT(&huart3, &TX_Data, 1);
//		}
	}
}
