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
#include "esp_system.h"

#include "config.h"

#include "globals.h"
#include "defines.h"



void	tserialRun(void* param);

void	fserialInit(void);

/**
 * @brief Read string from UART.
 * @param[in] out Pointer to initialised bus instance.
 * @param[in] i_maxNumChars Max Number of characters to be read befor timeout is thrown.
 * @return status
 */
int		freadUartString	(char* out, int i_maxNumChars);

int		freadUartCmdlet	(int *i_cmdlet);

#endif /* MAIN_COM_SERIAL_H_ */
