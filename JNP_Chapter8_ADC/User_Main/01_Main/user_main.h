/*
 * user_main.h
 *
 *  Created on: Aug 16, 2025
 *      Author: me
 */

#ifndef MAIN_USER_MAIN_H_
#define MAIN_USER_MAIN_H_

#include "IO_Port.h"
#include "interface.h"

struct count_structure
{
	uint8_t timer_500ms_flag;
	uint8_t timer_count;
};

struct interface_structure
{
	uint8_t uart_rx_data;
	uint8_t uart_500ms_flag;
};

struct SPI_structure
{
	uint8_t TX_Data;
	uint8_t RX_Data;
	uint8_t SPI_Send_Flag;
};
struct analog_structure
{
	uint16_t dac_value;
	uint32_t adc_value;
	// adc와 dac는 2byte값가지므로 16bit
	float volt;
	uint8_t adc_flag;
};

struct mcu_structure
{
	struct count_structure count;
	struct interface_structure interface;
	struct SPI_structure SPI;
	struct analog_structure analog;
};

void user_main(void);
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart3;
extern SPI_HandleTypeDef hspi1;
extern DAC_HandleTypeDef hdac;
extern ADC_HandleTypeDef hadc1;

extern struct mcu_structure mcu;

#endif /* 01_MAIN_USER_MAIN_H_ */
