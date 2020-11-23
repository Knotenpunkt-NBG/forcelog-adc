/*
 * tcp_client.c
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#include "com_tcp.h"

//TODO: add sockets as server if in AP Mode

void ftcpInit()
{
	ESP_LOGI(TAG_TCP, "STARTING TCP\n");
	//	stu_serverAddressMes.sin_family			=	AF_INET;
	//	stu_serverAddressConf.sin_family		=	AF_INET;
	//	stu_serverAddressUdp.sin_family			=	AF_INET;
	//	stu_serverAddressUdp.sin_addr.s_addr	=	htonl(INADDR_ANY);

	xTaskCreate	(ttcpMes,		"t_tcpSendMes",		4096,	NULL	,	8,		&ht_tcpMes);
	xTaskCreate	(ttcpConf,		"t_tcpConf",		4096,	NULL	,	8,		&ht_tcpConf);
}

//TODO: add bundling of measurements for higher throughput with less overhead max size of buffer:1400B
//TODO: combine all queues into one with size of a struct
void ttcpMes (void* param)
{
	vTaskSuspend(NULL);
	int sock = 0;
	struct stu_adcConfig adcConfig_mom;
	struct sockaddr_in serverAddr_mom;
	uint32_t ui_cmdlet = 0;
	uint8_t *data[64] = {};
	uint64_t time = 0;
	double measurement = 0.0;
	float f_temperature = 0.0;
	while(ui_cmdlet != CMD_conn)
	{
		ESP_LOGD(TAG_TCP, "MES CMDLET WAITING");
		xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		ESP_LOGD(TAG_TCP, "MES CMDLET RECEIVED");
		fconfigTcp(ui_cmdlet, &adcConfig_mom, &serverAddr_mom);
		vTaskDelay(1);
	}
	while(1)
	{
		ESP_LOGD(TAG_TCP,"STARTING SOCK MES\n");
		while((ui_cmdlet != CMD_wait) || (ui_cmdlet != CMD_peek))
		{
			if(sock == 0)
				sock = fconnSock(&serverAddr_mom);
			if(xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, 100 / portTICK_PERIOD_MS) != 0)
				fconfigTcp(ui_cmdlet, &adcConfig_mom, &serverAddr_mom);
		}

		if(ui_cmdlet == CMD_wait)
		{
			//write configuration message
			vTaskResume(ht_configRun);
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		}
		else
		{
			vTaskResume(ht_configRun);
		}
		ul_zeroTime = esp_timer_get_time();
		while(ui_cmdlet != CMD_stop)
		{
			//Sends measurements
			if(ui_cmdlet == CMD_MEAS_READY)
			{
				xQueueReceive(q_value_mes_tcp, &measurement, portMAX_DELAY);
				xQueueReceive(q_time_mes_tcp, &time, portMAX_DELAY);
				time -= ul_zeroTime;
				sprintf((char*)data, "|meas|%.*f|%llu|\n", adcConfig_mom.uc_numDecimals, measurement,time);
				if (send(sock, (char *)data, strlen((const char*)data), 0) == -1)
				{
					close(sock);
					sock = fconnSock(&serverAddr_mom);
				}
				ESP_LOGV(TAG_TCP, "SENDING MEASUREMENT\n");
				ui_cmdlet = 0;
			}
			//sends blink time
			else if(ui_cmdlet == CMD_BLINK_READY)
			{
				xQueueReceive(q_time_blink_tcp, &time, portMAX_DELAY);
				time -= ul_zeroTime;
				sprintf((char*)data, "|blk|%llu|\n", time);
				if (send(sock, (char *)data, strlen((const char*)data), 0) == -1)
				{
					close(sock);
					sock = fconnSock(&serverAddr_mom);
				}
				ESP_LOGV(TAG_TCP, "SENDING BLINK\n");
				ui_cmdlet = 0;
			}
			//send temperature
			else if(ui_cmdlet == CMD_TEMPINT_READY)
			{
				xQueueReceive(q_value_tempint_tcp, &f_temperature, portMAX_DELAY);
				xQueueReceive(q_time_tempint_tcp, &time, portMAX_DELAY);
				time -= ul_zeroTime;
				sprintf((char*)data, "|temp|%.*f|%llu|\n",2, f_temperature, time);
				if (send(sock, (char *)data, strlen((const char*)data), 0) == -1)
				{
					close(sock);
					sock = fconnSock(&serverAddr_mom);
				}
				ESP_LOGV(TAG_TCP, "SENDING Temperature\n");
				ui_cmdlet = 0;
			}
			else if(ui_cmdlet == 0)
			{
				if (fsendKeepAlive(sock) == -1)
				{
					ESP_LOGV(TAG_TCP, "KEEPALIVE MES FAILED\n");
					close(sock);
					sock = fconnSock(&serverAddr_mom);
				}
			}
			else
			{
				fconfigTcp(ui_cmdlet, &adcConfig_mom, &serverAddr_mom);
			}
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, TICKS_RECON_MES);
		}
		close(sock);
		sock = 0;
	}
}

void ttcpConf (void* param)
{
	vTaskSuspend(NULL);
	ESP_LOGD(TAG_TCP,"STARTING TCP CONFIG");
	uint32_t ui_cmdlet = 0;
	struct sockaddr_in stu_serverAddressConf;
	int i_cycles = 0;
	int sock = 0;
	int i_flag = 0;
	char* pc_txMessage;
	char* pc_configOut = NULL;
	char* pc_configIn = malloc(64);
	receiving_timeout.tv_sec = 0;
	receiving_timeout.tv_usec = 5000;

	while(1)
	{
		while(ui_cmdlet != CMD_conn)
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			fconfigTcp(ui_cmdlet, NULL, &stu_serverAddressConf);
			vTaskDelay(1);
		}
		sock = fconnSock(&stu_serverAddressConf);
		setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&receiving_timeout,sizeof(receiving_timeout));
		send(sock, (const char*) GREETING_MESSAGE, strlen((const char*)GREETING_MESSAGE), 0);
		while(1)
		{
			if(xQueueReceive(q_tcpMessages, &pc_txMessage, 1)>0)
			{
				send(sock, pc_txMessage, strlen((const char*)pc_txMessage), 0);
				free(pc_txMessage);
			}
			i_flag = freadTcpString(pc_configIn, 64, sock);

			if (i_flag <= 0)
			{
				i_cycles++;
				if (i_cycles == CYCLES_RECON_CONF)
				{
					ESP_LOGV(TAG_TCP, "Sending Keepalive\n");
					if (fsendKeepAlive(sock) < 0)
					{
						ESP_LOGW(TAG_TCP, "Keepalive port config failed errno:%d\n", errno);
						sock = fconnSock(&stu_serverAddressConf);
					}
					i_cycles = 0;
				}
			}
			else if (i_flag == 0 )
			{
				ESP_LOGD(TAG_TCP, "SENDING ERROR MESSAGE\n");
				send(sock, (const char*)MESS_INVALID, strlen((const char*)MESS_INVALID), 0);
			}
			else
			{
//				ESP_LOGD(TAG_TCP, "CMDLET:%s", pc_configIn);
				xSemaphoreTake(hs_configCom, portMAX_DELAY);
				xQueueSend(q_pconfigIn,&pc_configIn, portMAX_DELAY);
				xQueueReceive(q_pconfigOut, &pc_configOut, portMAX_DELAY);
				while (pc_configOut != 0)
				{
					send(sock, (const char*) pc_configOut, strlen((const char*)pc_configOut), 0);

					while(freadTcpString(pc_configIn, 64, sock) <= 0)
						vTaskDelay(100 / portTICK_PERIOD_MS);
					xQueueSend(q_pconfigIn,&pc_configIn, portMAX_DELAY);
					xQueueReceive(q_pconfigOut, &pc_configOut, portMAX_DELAY);
				}
				xQueueReceive(q_pconfigOut, &pc_configOut, portMAX_DELAY);
				send(sock, (const char*) pc_configOut, strlen((const char*)pc_configOut), 0);
				xSemaphoreGive(hs_configCom);
				vTaskResume(ht_configRun);
			}
		}
	}
}



int freadTcpString	(char* out,
					int i_maxNumChars,
					int sock)
{
	int i_flag = 0;
	uint8_t c_single = 0;
	for (int i = 0; i < i_maxNumChars; i++)
	{
		if(recv(sock, &c_single, 1, 0) > 0)
		{
			if((c_single== '\r') || (c_single == '\n') || (c_single == '\0'))
			{
				out[i] = '\0';
				i_flag = 1;
				break;
			}
			else
			{
				out[i] = c_single;
				if(i == i_maxNumChars-1)
					i_flag = 0;
			}
		}
		else
		{
			i_flag = -1;
			break;
		}
	}
	return i_flag;
}


void fsendAck (int sock)
{
	send(sock, (const char*) "|ack|\n", strlen((const char*)"|ack|\n"), 0);
}


int fsendKeepAlive (int sock)
{
	return send(sock, (const char*) "​\u200B", strlen((const char*)"\u200B"),0);
}


int fconnSock (struct sockaddr_in* serverAddr)
{
	int sock = 0;
	char str[16] = "";
	inet_ntop(AF_INET, &(serverAddr->sin_addr.s_addr), (char*)str, sizeof(str));
	ESP_LOGV(TAG_TCP, "Connecting to socket at  %s:%d\n", str , ntohs(serverAddr->sin_port));
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(sock, (struct sockaddr*)serverAddr, sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		sock = 0;
	}
	if(sock!=0)
		ESP_LOGI(TAG_TCP, "SOCKET ESTABLISHED ON %s:%d\n", str , ntohs(serverAddr->sin_port));
	return sock;
}


void fconfigTcp	(uint32_t ui_cmdlet,
				struct stu_adcConfig* p_adcConfig_mom,
				struct sockaddr_in* serverAddress_mom)
{
	switch(ui_cmdlet)
	{
	case 0:
		break;
	case CMD_poco:
	case CMD_pome:
	case CMD_ipco:
	case CMD_ipme:
	case CMD_ldtm:
	case CMD_ldtc:
	case CMD_svtm:
	case CMD_svtc:
		xQueueSend(q_pointer,&serverAddress_mom, portMAX_DELAY);
		vTaskSuspend(NULL);
		break;

	case CMD_ldad:
	case CMD_svad:
		xQueueSend(q_pointer,&p_adcConfig_mom, portMAX_DELAY);
		vTaskSuspend(NULL);
		break;
	default:
		ESP_LOGD(TAG_TCP, "WRONG CMDLET");
		break;
	}
}
