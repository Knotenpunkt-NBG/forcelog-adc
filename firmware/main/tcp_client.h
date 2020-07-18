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

#include "queues.h"
#include "events.h"
#include "defines.h"

struct sockaddr_in str_serverAddressConf;
struct sockaddr_in str_serverAddressMes;

uint64_t ul_zeroTime;

void t_tcpInit(void *arg);
char* readTcpString(char* out, int i_maxNumChars, int sock);
int readTcpCmdlet(int sock);
void sendAck (int sock);

void t_tcpSendMes (void * param);
void t_tcpSendConf (void * param);
void t_tcpRecvConf (void * param);
void t_tcpIdle (void* param);

#endif /* MAIN_TCP_CLIENT_H_ */
