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
#include "freertos/semphr.h"

#include "defines.h"
#include "globals.h"
#include "config.h"

struct timeval receiving_timeout;



void ftcpInit();
//Reads string from tcp socket
//RETURN:
//1 if string has been read
//0 if no string has been found
//-1 if string has been found but without delimiter (\n, \0 or \r)
int freadTcpString(char* out, int i_maxNumChars, int sock);
void fsendAck (int sock);
int fsendKeepAlive (int sock);
int fconnSock (struct sockaddr_in* serverAddr);
void fconfigTcp(uint32_t ui_cmdlet,struct stu_adcConfig* p_adcConfig_mom, struct sockaddr_in* serverAddress_mom);

void ttcpMes (void* param);
void ttcpConf (void* param);


#endif /* MAIN_COM_TCP_H_ */
