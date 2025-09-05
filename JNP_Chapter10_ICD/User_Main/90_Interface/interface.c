#include "main.h"

struct tx_frame_structure 				system_tx_frame;
struct tx_frame_structure 				rx_respond_frame;
struct system_interface_structure 		system_uart;
struct uart_control_structure			system_control;
struct system_status_structure 			system_status;

uint8_t* tx_data_buffer;

void system_Init(void)
	// tx 프레임 초기화(고정된 값)
{
	system_uart.tx_index = 1;
	system_tx_frame.start_code[0] 	= 	START_STATE_1ST;
	system_tx_frame.start_code[1] 	= 	START_STATE_2ND;
	system_tx_frame.src_address[0] 	= 	SOURCE_ADDR;
	system_tx_frame.des_address[0] 	= 	DESTINATION_ADDR;
	system_tx_frame.end_code[0]   	=	END_CODE;
}

void system_interfcae_tx(void)
{
	if(system_uart.tx_index == 1)
	{
		// TX Buffer에 tx data전달하는 주소값 전달
		tx_data_buffer = system_tx_frame.start_code;
	}
	else if(system_uart.tx_index == system_uart.tx_frame_length)
	{
		// TX Buffer에 tx data 전달 마지막 데이터
		tx_data_buffer = system_tx_frame.end_code;
	}
	// tx data 1byte 전송
	HAL_UART_Transmit_IT(&huart3, (uint8_t*)tx_data_buffer, 1);
	if(system_uart.tx_index > system_uart.tx_frame_length)
	{
		system_uart.tx_index = 1;
		system_uart.tx_frame_length = 0;
		// CR1 레지스터 TXE 인터럽트 비활성화
		__HAL_UART_DISABLE_IT(&huart3,UART_IT_TXE);
	}
	else
	{
		 tx_data_buffer++;
		 system_uart.tx_index++;
	}

}

void system_interface_tx_frame(void)
{
	// tx 프레임 생성
	system_uart.tx_frame_length = 10; // 전체 프레임 기본 길이 데이터값은 10
	switch (system_uart.tx_control_flag)
	{
		case 0:
			// status frame생성
			system_tx_frame.command_code[0] = SYSTEM_STATUS_COMMAND;
			system_uart.tx_sub_data_length = 0;
			break;
		case 1:
			// led on frame 생성
			system_tx_frame.command_code[0] = LED_ON_COMMAND;
			system_uart.tx_sub_data_length = 0;
			break;
		case 2:
			// led off frame 생성
			system_tx_frame.command_code[0] = LED_OFF_COMMAND;
			system_uart.tx_sub_data_length = 0;
			break;
		case 3:
			// dac control data frame
			system_tx_frame.command_code[0] = CONTROL_COMMAND;
			system_uart.tx_sub_data_length = 3;
			system_tx_frame.sub_data[0] = 0x02; // dac control flag
			system_tx_frame.sub_data[1] = ((mcu.analog.dac_value >> 8) & 0xFF); // dac data flag
			system_tx_frame.sub_data[2] = (mcu.analog.dac_value & 0xFF); // dac data flag
			break;
	}
	system_uart.tx_send_flag = 1;
	// sub data의 length 넣음
	system_tx_frame.data_length[0] = (system_uart.tx_sub_data_length >> 8); // SUB DATA 길이
	system_tx_frame.data_length[1] = system_uart.tx_sub_data_length; // SUB DATA 길이
	system_tx_frame.crc[0] = 0x00;
	system_tx_frame.crc[1] = 0x00;
	// 전체 프레임 길이 + 서브 데이터 길이
	system_uart.tx_frame_length += system_uart.tx_sub_data_length;

	if(system_uart.tx_send_flag == 1)
	{
		system_uart.tx_send_flag = 0;
		// enable TX interrupt
		__HAL_UART_ENABLE_IT(&huart3, UART_IT_TC);
	}
}

void system_interface_rx(void)
{
	uint8_t uart_rx_data;

	if(system_uart.rx_index == system_uart.rx_buffer_index)
		return;

	uart_rx_data = system_uart.rx_buffer[system_uart.rx_index++];

	if(system_uart.rx_buffer_index > (MAX_UART_BUFFER_SIZE-1))
		system_uart.rx_buffer_index = 0;

	switch (system_uart.current_state)
	{
		case START_STATE_1ST:
			if(uart_rx_data == START_CODE_1ST)
			{
				// 2번째 상태로 넘어간다
				system_uart.current_state = START_STATE_2ND;
				rx_respond_frame.start_code[0] = uart_rx_data;
			}
			else
			{
				system_uart.current_state = START_STATE_1ST;
			}
			break;
		case START_STATE_2ND:
			if(uart_rx_data == START_STATE_2ND)
			{
				// 2번째 상태로 넘어간다
				system_uart.current_state = SOURCE_ADDRESS_STATE;
				rx_respond_frame.start_code[1] = uart_rx_data;
			}
			else
			{
				system_uart.current_state = START_STATE_1ST;
			}
			break;
		case SOURCE_ADDRESS_STATE:
			system_uart.current_state = DESTINATION_ADDRESS_STATE;
			rx_respond_frame.des_address[0] = uart_rx_data;
			break;
		case DESTINATION_ADDRESS_STATE:
			system_uart.current_state = COMMAND_STATE;
			rx_respond_frame.src_address[0] = uart_rx_data;
			break;
		case COMMAND_STATE:
			system_uart.current_state = LENGTH_H_STATE;
			rx_respond_frame.command_code[0] = uart_rx_data;
			system_uart.rx_command_code = uart_rx_data;
			break;
		case LENGTH_H_STATE:
			system_uart.current_state = LENGTH_L_STATE;
			system_uart.rx_length = uart_rx_data;
			break;
		case LENGTH_L_STATE:
			system_uart.current_state = SUB_DATA_STATE;
			// subdata length는 2byte이기때문에 합쳐준다.
			system_uart.rx_length <<= 8;
			system_uart.rx_length += uart_rx_data;
			// 길이 비교
			if(system_uart.rx_length > MAX_FRAME_DATA_LENGTH)
			{
				system_uart.current_state = START_STATE_1ST;
			}
			else
			{
				if(system_uart.rx_length == 0)
					system_uart.current_state = CRC_H_STATE; // skip SUB_DATA_STATE
				else
					system_uart.current_state = SUB_DATA_STATE;
			}
			break;
		case SUB_DATA_STATE:
			system_uart.rx_sub_data_buffer[system_uart.rx_sub_data_index] = uart_rx_data;
			system_uart.rx_sub_data_index++;

			// subindex 다 받으면 다음 상태인 CRC_H로
			if(system_uart.rx_sub_data_index == system_uart.rx_length)
				system_uart.current_state = CRC_H_STATE;
			break;
		case CRC_H_STATE:
			system_uart.current_state = CRC_L_STATE;
			break;
		case CRC_L_STATE:
			system_uart.current_state = END_CODE_STATE;
			break;
		case END_CODE_STATE:
			if(uart_rx_data != END_CODE)
			{
				// initialize status
				system_uart.current_state = START_STATE_1ST;
				return;
			}

			// callback마다 함수 실행되므로, 데이터만 분석하고 flag만 셋하고 main문에서 데이터 처리!!
			switch (system_uart.rx_command_code) {
				case SYSTEM_STATUS_COMMAND:
					// sub data의 길이는 구조체의 전체 길이를 전달
					system_uart.rx_sub_data_length = sizeof(system_status);
					memcpy(rx_respond_frame.sub_data, &system_status, system_uart.rx_sub_data_length);
					// void* memcpy(void* destination, const void* source, size_t num);
					break;
				case LED_ON_COMMAND:
					system_uart.control_flag = 1;
					system_uart.led_on_flag = 1;
					break;
				case LED_OFF_COMMAND:
					system_uart.control_flag = 1;
					system_uart.led_off_flag = 1;
					break;
				case CONTROL_COMMAND:
					system_uart.control_flag = 1;
					system_control.flag1.all = system_uart.rx_sub_data_buffer[0];
					// 2byte로 만들기
					system_control.dac_control_data = (system_uart.rx_sub_data_buffer[1] << 8) + system_uart.rx_sub_data_buffer[2];
					break;
			}
			rx_respond_frame.data_length[0] = (system_uart.rx_sub_data_length >> 8);
			rx_respond_frame.data_length[1] = (system_uart.rx_sub_data_length);
			break;
	}
}

// memcpy했던 구조체의 system status data만들기
void system_status_load(void)
{
	system_status.alarm_status = 0;
	system_status.firmware_version = 0x01;
	system_status.adc_voltage_h = (mcu.analog.int_volt >> 8);
	system_status.adc_voltage_l = (mcu.analog.int_volt);
	system_status.dac_voltage_h = 0;
	system_status.dac_voltage_l = 0;
	system_status.adc_analog_h = (mcu.analog.adc_value >> 8);
	system_status.adc_analog_l = (mcu.analog.adc_value);
	system_status.dac_analog_h = (mcu.analog.dac_value >> 8);
	system_status.dac_analog_l = (mcu.analog.dac_value);

	system_status.alarm_status = 0;
}

//각 flag를 보며 control하는 함수
void system_control_check(void)
{
	if(system_uart.control_flag == 1)
	{
		system_uart.control_flag = 0;
		if(system_uart.led_off_flag == 1)
		{
			system_uart.led_off_flag = 0;
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		}
		if(system_uart.led_on_flag == 1){
			system_uart.led_on_flag = 0;
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		}
		if(system_control.flag1.item.dac_ctl == 1)
		{
			system_control.flag1.item.dac_ctl = 0;
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, system_control.dac_control_data);
		}
	}
}
