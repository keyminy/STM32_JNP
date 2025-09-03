#include "main.h"

struct mcu_structure mcu;

void user_main(void)
{
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, &mcu.analog.adc_value, 1);
	while(1)
	{
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, mcu.analog.dac_value);
		if(mcu.analog.adc_flag  == 1)
		{
			mcu.analog.adc_flag  = 0;
			mcu.analog.volt = ((3250 * mcu.analog.adc_value)/ 4095);
		}

//		HAL_ADC_Start(&hadc1);
//		mcu.analog.adc_value = HAL_ADC_GetValue(&hadc1);
//		mcu.analog.volt = ((3250 * mcu.analog.adc_value)/ 4095);
//		mcu.analog.volt = mcu.analog.volt/1000;
	}
}
