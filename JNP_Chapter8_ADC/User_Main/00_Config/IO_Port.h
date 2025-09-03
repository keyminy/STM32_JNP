/*
 * IO_Port.h
 *
 *  Created on: Aug 16, 2025
 *      Author: me
 */

#ifndef CONFIG_IO_PORT_H_
#define CONFIG_IO_PORT_H_

#define SIGNAL_HIGH	GPIO_PIN_SET
#define SIGNAL_LOW	GPIO_PIN_RESET

#define SPI_ENABLE( level ) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,level)


#endif /* 00_CONFIG_IO_PORT_H_ */
