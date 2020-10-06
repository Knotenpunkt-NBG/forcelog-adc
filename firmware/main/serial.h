/*
 * uart.h
 *
 *  Created on: Oct 5, 2020
 *      Author: lor
 */

#ifndef MAIN_SERIAL_H_
#define MAIN_SERIAL_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "config.h"

#include "globals.h"
#include "defines.h"


void serial_config(void *arg);
void serial_idle(void *arg);

void serial_init();
void	readUartString	(char* out, int i_maxNumChars);
int		readUartCmdlet	(int *i_cmdlet);

extern uart_config_t uart_config;

#endif /* MAIN_SERIAL_H_ */
