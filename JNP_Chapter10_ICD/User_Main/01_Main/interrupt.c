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
		system_uart.rx_buffer[system_uart.rx_index++] = mcu.interface.uart_rx_data;
		if(system_uart.rx_index == MAX_UART_BUFFER_SIZE)
		{
			system_uart.rx_index = 0;
		}
		system_interface_rx();

//		// 수신된 데이터를 rx버퍼에 저장
//		mcu.interface.rx_data[mcu.interface.rx_count++] = mcu.interface.uart_rx_data;
//		// 1byte 데이터가 수신되면 respond 함수로 이동
//		respond_interface_rx();
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
	// adc가 끝나면 adc값을 변환하는 플래그
	mcu.analog.adc_flag = 1;
}
