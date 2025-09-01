#include "main.h"

struct mcu_structure mcu;

void user_main(void)
{
	while(1)
	{
		if(mcu.SPI.SPI_Send_Flag == 1)
		{
			mcu.SPI.SPI_Send_Flag = 0;
			SPI_ENABLE(SIGNAL_LOW);
//			HAL_SPI_Transmit(&hspi1, &mcu.SPI.TX_Data, 1, 10);
			//TX가 RX받아지는지 확인
			HAL_SPI_TransmitReceive(&hspi1, &mcu.SPI.TX_Data,&mcu.SPI.RX_Data ,1, 10);
			SPI_ENABLE(SIGNAL_HIGH);
		}
	}
}
