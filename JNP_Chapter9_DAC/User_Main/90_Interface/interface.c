#include "main.h"

uint8_t TX_Data[10] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};
uint8_t TX_Count = 0;

void system_interfcae_tx(void)
{
	HAL_UART_Transmit_IT(&huart3, &mcu.interface.tx_data[mcu.interface.tx_count],1);
	mcu.interface.tx_count++;
	if(mcu.interface.tx_count > 3)
	{
		mcu.interface.tx_count = 0;
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_TXE); // CR1 레지스터 TXE 인터럽트 비활성화
	}
}

void respond_interface_rx(void)
{
	uint8_t current_state;
	current_state = mcu.interface.rx_data[0]; // 첫번째 데이터의 state 데이터 수신

	// rx 버퍼 카운트 초기화
	if(mcu.interface.rx_count >= 3)
	{
		mcu.interface.rx_count = 0;
	}

	switch (current_state) {
		case 1: // led on
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
			current_state = 0;
			break;
		case 2: // led off
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
			current_state = 0;
			break;
		case 3: // dac control
			// byte조합을 통하여 dac value생성
			// rx_data[1] << 8하면 8개 이동한 빈 자리에 rx_data[2]가 들어옴
			// 1byte를 2byte로 조합하는 방법임
			mcu.analog.dac_value = (mcu.interface.rx_data[1] << 8) + mcu.interface.rx_data[2];
			mcu.analog.dac_flag = 1; // dac control flag enable
			break;
		default:
			break;
	}
}
