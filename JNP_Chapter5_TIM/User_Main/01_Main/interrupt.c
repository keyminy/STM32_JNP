#include "main.h"


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
		}
	}
}
