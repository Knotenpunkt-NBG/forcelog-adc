/*
 * uart.h
 *
 *  Created on: Oct 5, 2020
 *      Author: lor
 */

#ifndef MAIN_COM_SERIAL_H_
#define MAIN_COM_SERIAL_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "config.h"

#include "globals.h"
#include "defines.h"



void tserialRun(void *arg);

void fserialInit(void);
void	freadUartString	(char* out, int i_maxNumChars);
int		freadUartCmdlet	(int *i_cmdlet);

//extern uart_config_t uart_config;

#endif /* MAIN_COM_SERIAL_H_ */
