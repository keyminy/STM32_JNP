#include "main.h"

uint8_t RX_Data[10] = {0};
uint16_t RX_Count = 0;


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
//		HAL_UART_Receive_IT(&huart3,&mcu.interface.uart_rx_data , 1);
//		RX_Data = mcu.interface.uart_rx_data;

		RX_Data[RX_Count++] = mcu.interface.uart_rx_data;
		if(RX_Count >= 10)
		{
			RX_Count = 0;
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart3.Instance)
	{
		system_interfcae_tx();
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	mcu.analog.adc_flag = 1;
}
