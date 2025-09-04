#include "main.h"

struct mcu_structure mcu;

void user_main(void)
{
	HAL_UART_Receive_DMA(&huart3, &mcu.interface.uart_rx_data, 1);
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, &mcu.analog.adc_value, 1);
	while(1)
	{
		if(mcu.interface.tx_flag == 1)
		{
			mcu.interface.tx_flag = 0;
			// CR1 레지스터 송신완료 인터럽트 활성화
			__HAL_UART_ENABLE_IT(&huart3,UART_IT_TC);
		}
		if(mcu.analog.dac_flag==1)
		{
			// DAC 제어
			HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, mcu.analog.dac_value);
		}
		if(mcu.analog.adc_flag == 1)
		{
			mcu.analog.adc_flag = 0;
			mcu.analog.volt = ((3300*mcu.analog.dac_value)/4095); //ADC 변환 (3250?)
		}
	}
}
