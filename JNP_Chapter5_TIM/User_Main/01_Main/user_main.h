/*
 * user_main.h
 *
 *  Created on: Aug 16, 2025
 *      Author: me
 */

#ifndef MAIN_USER_MAIN_H_
#define MAIN_USER_MAIN_H_

#include "IO_Port.h"

struct count_structure
{
	uint8_t timer_500ms_flag;
	uint8_t timer_count;
};

struct mcu_structure
{
	struct count_structure count;
};

void user_main(void);
extern TIM_HandleTypeDef htim2;
extern struct mcu_structure mcu;

#endif /* 01_MAIN_USER_MAIN_H_ */
