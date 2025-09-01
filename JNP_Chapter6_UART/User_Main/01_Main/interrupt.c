#include "main.h"

uint16_t RX_Data = 0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		mcu.count.timer_count++;
		if(mcu.count.timer_count == 100)
		{
			// mcu.count.timer_count == 100 is equal to 500ms
			mcu.count.timer_count = 0;
			mcu.count.timer_500ms_flag += 1;
			mcu.interface.uart_500ms_flag = 1;
		}
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart3.Instance)
	{
		HAL_UART_Receive_IT(&huart3,&mcu.interface.uart_rx_data , 1);
		RX_Data = mcu.interface.uart_rx_data;
	}
}
