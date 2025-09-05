/*
 * interface.h
 *
 *  Created on: Sep 2, 2025
 *      Author: me
 */

#ifndef INTERFACE_INTERFACE_H_
#define INTERFACE_INTERFACE_H_

// ============== Command Code Definition ===================== //
#define SYSTEM_STATUS_COMMAND 0x10
#define LED_ON_COMMAND 	      0x20
#define LED_OFF_COMMAND		  0x21
#define CONTROL_COMMAND	      0x40
// ============== BASIC Code Definition ===================== //
#define START_CODE_1ST 		  0x90
#define START_CODE_2ND		  0x86
#define SOURCE_ADDR			  0x10
#define DESTINATION_ADDR	  0x21
#define END_CODE			  0xF8

// ============== Code Length Definition ===================== //
#define START_CODE_LENGTH	  2
#define ADDRESS_LENGTH		  1
#define COMMAND_LENGTH		  1
#define DATA_LENGTH			  2
#define SUB_DATA_LENGTH		  100
#define CRC_LENGTH			  2
#define END_CODE_LENGTH		  1
#define MAX_FRAME_DATA_LENGTH 100

// ============== Data Process State Definition ===================== //
#define START_STATE_1ST		  		0
#define START_STATE_2ND		  		1
#define SOURCE_ADDRESS_STATE		2
#define DESTINATION_ADDRESS_STATE	3
#define COMMAND_STATE		  		4
#define LENGTH_H_STATE		  		5
#define LENGTH_L_STATE		  		6
#define SUB_DATA_STATE		  		7
#define CRC_H_STATE		  			8
#define CRC_L_STATE		  			9
#define END_CODE_STATE		  		10

// ============== UART BUFFER Definition ===================== //
#define MAX_UART_BUFFER_SIZE		20

struct tx_frame_structure
{
	uint8_t start_code[START_CODE_LENGTH];
	uint8_t src_address[ADDRESS_LENGTH];
	uint8_t des_address[ADDRESS_LENGTH];
	uint8_t command_code[COMMAND_LENGTH];
	uint8_t data_length[DATA_LENGTH];
	uint8_t sub_data[SUB_DATA_LENGTH];
	uint8_t crc[CRC_LENGTH];
	uint8_t end_code[END_CODE_LENGTH];
};

// 시스템 인터페이스에서 사용하는 버퍼 및 플래그 구조체
struct system_interface_structure
{
	// ============== Transmit data process ===================== //
	uint16_t tx_index;
	uint16_t tx_frame_length;
	uint16_t tx_sub_data_length;
	// ============== Receive data process ===================== //
	uint16_t rx_index;
	uint16_t rx_buffer_index;
	uint8_t  rx_buffer[MAX_FRAME_DATA_LENGTH];
	uint8_t  rx_sub_data_buffer[MAX_FRAME_DATA_LENGTH];

	uint8_t rx_slave_address;
	uint8_t rx_master_address;
	uint8_t rx_command_code;
	uint16_t rx_length;
	uint16_t rx_sub_data_length;
	uint8_t current_state;
	uint16_t rx_sub_data_index;
	// ============== control flag ===================== //
	uint8_t tx_control_flag;
	uint8_t tx_send_flag;

	uint8_t control_flag;
	uint8_t led_on_flag;
	uint8_t led_off_flag;
	uint8_t dac_control_flag;
};

// ICD 문서 TAB3의 Status 구조체
struct system_status_structure
{
	uint8_t alarm_status;
	uint8_t firmware_version;
	uint8_t adc_voltage_h;
	uint8_t adc_voltage_l;
	uint8_t dac_voltage_h;
	uint8_t dac_voltage_l;
	//
	uint8_t adc_analog_h;
	uint8_t adc_analog_l;
	uint8_t dac_analog_h;
	uint8_t dac_analog_l;
};

// Tab4 Control structure
struct control_1_flag_structure
{
	uint8_t reserved_1 :1;
	uint8_t dac_ctl :1;
	uint8_t reserved_3 :1;
	uint8_t reserved_4 :1;
	uint8_t reserved_5 :1;
	uint8_t reserved_6 :1;
	uint8_t reserved_7 :1;
	uint8_t reserved_8 :1;
};
union control_1_flag_union
{
	struct control_1_flag_structure item;
	uint8_t all;
};

struct uart_control_structure
{
	union control_1_flag_union flag1;
	uint16_t dac_control_data;
};

void system_Init(void);
void system_interface_tx_frame(void);
void system_interfcae_tx(void);
void system_interface_rx(void);
void system_status_load(void);
void system_control_check(void);

#endif /* 90_INTERFACE_INTERFACE_H_ */
