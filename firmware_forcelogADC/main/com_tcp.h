/*
 * tcp_client.h
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#ifndef MAIN_COM_TCP_H_
#define MAIN_COM_TCP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include <lwip/sockets.h>
#include "esp_wifi.h"
#include <esp_event.h>
#include "driver/uart.h"

#include "defines.h"
#include "globals.h"
#include "config.h"

struct timeval receiving_timeout;



void ftcpInit();
char* readTcpString(char* out, int i_maxNumChars, int sock);
int readTcpCmdlet(int sock, int *i_cmdlet);
void sendAck (int sock);
int sendKeepAlive (int sock);
int connSockConf(void);
int connSockMes	(void);

void t_tcpSend (void * param);
void t_tcpConf (void * param);


#endif /* MAIN_COM_TCP_H_ */
