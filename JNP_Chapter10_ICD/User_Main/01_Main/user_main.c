#include "main.h"

struct mcu_structure mcu;

void user_main(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_UART_Receive_DMA(&huart3, &mcu.interface.uart_rx_data, 1);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, &mcu.analog.adc_value, 1);
	system_Init();
	while(1)
	{
		if(mcu.analog.adc_flag == 1)
		{
			mcu.analog.adc_flag = 0;
			mcu.analog.volt = ((3300*mcu.analog.adc_value)/4095); //ADC 변환 (3250?)
			mcu.analog.int_volt = (mcu.analog.volt * 256);
		}
		if(mcu.interface.uart_500ms_flag)
		{
			mcu.interface.uart_500ms_flag = 0;
			system_interface_tx_frame();
		}
		system_status_load();
		system_control_check();
	}
}
