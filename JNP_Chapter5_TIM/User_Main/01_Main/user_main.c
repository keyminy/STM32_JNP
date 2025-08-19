#include "main.h"

struct mcu_structure mcu;
uint8_t pulse_test = 50;


void user_main(void)
{
//	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	while(1)
	{
		htim2.Instance->CCR1 = pulse_test;
		/*
		if(mcu.count.timer_500ms_flag == 1)
		{
			HAL_GPIO_WritePin(GPIOB,LD2_Pin, SIGNAL_HIGH);
			HAL_GPIO_WritePin(GPIOB,LD3_Pin, SIGNAL_HIGH);
		}
		else if(mcu.count.timer_500ms_flag == 2)
		{
			HAL_GPIO_WritePin(GPIOB,LD2_Pin, SIGNAL_LOW);
			HAL_GPIO_WritePin(GPIOB,LD3_Pin, SIGNAL_LOW);
			mcu.count.timer_500ms_flag = 0;
		}
		*/
	}
}
