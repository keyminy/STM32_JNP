#include "main.h"

void user_main(void)
{
	while(1)
	{
		HAL_GPIO_WritePin(GPIOB,LD2_Pin, SIGNAL_HIGH);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOB,LD2_Pin, SIGNAL_LOW);
		HAL_Delay(100);
	}
}
