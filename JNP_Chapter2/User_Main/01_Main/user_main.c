#include "main.h"

void user_main(void)
{
	RCC->AHB1ENR |= 1<<1;// GPIOB에 Clock 공급
	GPIOB -> MODER |= (1<<(7*2)) | (1<<(14*2));  // GPIOB MODER General Purpose Output Mode
	GPIOB -> OSPEEDR |=  (3<<(7*2)) | (3<<(14*2)); // Very High Speed
	while(1)
	{
		GPIOB -> ODR |= (1<<7) | (1<<14); // GPIOB 7,GPIOB 14 ON
		HAL_Delay(500);
		GPIOB -> ODR ^= (1<<7) | (1<<14);// GPIOB 7,GPIOB 14 OFF
		HAL_Delay(500);
	}
}
