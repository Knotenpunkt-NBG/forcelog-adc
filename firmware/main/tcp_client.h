/*
 * tcp_client.h
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#ifndef MAIN_TCP_CLIENT_H_
#define MAIN_TCP_CLIENT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include <lwip/sockets.h>
#include "esp_wifi.h"
#include <esp_event.h>
#include "defines.h"
#include "globals.h"
#include "config.h"

struct timeval receiving_timeout;



void t_tcpInit(void *arg);
char* readTcpString(char* out, int i_maxNumChars, int sock);
int readTcpCmdlet(int sock, int *i_cmdlet);
void sendAck (int sock);
int sendKeepAlive (int sock);
int connSockConf(void);
int connSockMes	(void);

void t_tcpSendMes (void * param);
void t_tcpConf (void * param);
void t_tcpIdle (void* param);
void t_udpWait (void* param);

#endif /* MAIN_TCP_CLIENT_H_ */
