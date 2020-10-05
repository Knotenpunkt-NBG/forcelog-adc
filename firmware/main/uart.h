/*
 * uart.h
 *
 *  Created on: Oct 5, 2020
 *      Author: lor
 */

#ifndef MAIN_UART_H_
#define MAIN_UART_H_

#include "globals.h"
#include "defines.h"
#include <lwip/sockets.h>
#include "driver/uart.h"

void serial_config(void *arg);
void serial_idle(void *arg);

char*	readUartString(char* out, int i_maxNumChars);
int		readUartCmdlet();

#endif /* MAIN_UART_H_ */
