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

	xTaskCreate	(ttcpMes,		"t_tcpSendMes",		4096,	NULL	,	10,		&ht_tcpMes);
	xTaskCreate	(ttcpConf,		"t_tcpConf",		4096,	NULL	,	10,		&ht_tcpConf);
}

//TODO: add bundling of measurements for higher throughput with less overhead max size of buffer:1400B
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
		xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		fconfigTcp(ui_cmdlet, &adcConfig_mom, &serverAddr_mom);
		vTaskDelay(1);
	}
	while(1)
	{
		ESP_LOGD(TAG_TCP,"STARTING SOCK MES\n");
		while((ui_cmdlet != CMD_strt) || (ui_cmdlet != CMD_peek))
		{
			if(sock == 0)
				sock = fconnSock(&serverAddr_mom);
			if(xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, 100 / portTICK_PERIOD_MS) != 0)
				fconfigTcp(ui_cmdlet, &adcConfig_mom, &serverAddr_mom);
		}

		if(ui_cmdlet == CMD_strt)
		{
			//write configuration message
		}
		ui_cmdlet = 0;
		ul_zeroTime = 0;
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
					sock = fconnSockMes(&serverAddr_mom);
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
					sock = fconnSockMes(&serverAddr_mom);
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
					sock = fconnSockMes(&serverAddr_mom);
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
					sock = fconnSockMes(&serverAddr_mom);
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
	int i_cmdlet = 0;
	int i_flag = 0;
	int i_flagConfig = 0;
	char rx_buffer[128];
	char* pc_txMessage;
	while(ui_cmdlet != CMD_conn)
	{
		xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		fconfigTcp(ui_cmdlet, NULL, &stu_serverAddressConf);
		vTaskDelay(1);
	}
	sock = fconnSockConf(&stu_serverAddressConf);
	while(1)
	{
		xEventGroupWaitBits(eg_tcp, BIT_TCPQUEUE_READY, false, true, TICKS_TIMEOUT_CONF);
		if (xEventGroupGetBits(eg_tcp) & BIT_TCPQUEUE_READY)
		{
			xQueueReceive(q_tcpConf, &pc_txMessage, portMAX_DELAY);
			send(sock, pc_txMessage, strlen((const char*)pc_txMessage), 0);
			free(pc_txMessage);
			xEventGroupClearBits(eg_tcp, BIT_TCPQUEUE_READY);
		}

		i_flag = freadTcpCmdlet(sock, &i_cmdlet);
		if (i_cmdlet != 0)
		{
			ESP_LOGD(TAG_TCP, "CMDlet:%#08x\n", i_cmdlet);
			switch (i_cmdlet)
			{
			case CMD_man: //inquires manual
				send(sock, (const char*) MANUAL, strlen((const char*)MANUAL), 0);
				break;
			case CMD_info: //inquires all the information
				//send(sock, (const char*) INQUIRE_MESSAGE, strlen((const char*)INQUIRE_MESSAGE), 0);
				break;

			default:
				if (fcmdCheck(i_cmdlet))
				{
					if (i_flag == 0)
						fsendAck(sock);
					freadTcpString((char*)rx_buffer, 50, sock);
				}
				i_flagConfig = fConfig(i_cmdlet, rx_buffer);
				if (i_flagConfig)
				{
					send(sock, (char *)rx_buffer, strlen((const char*)rx_buffer), 0);
				}
				i_cmdlet = 0;
				break;
			}
		}
		else if (i_cmdlet == 0)
		{
			i_cycles++;
			if (i_cycles == CYCLES_RECON_CONF)
			{
				ESP_LOGV(TAG_TCP, "Sending Keepalive\n");
				if (fsendKeepAlive(sock) < 0)
				{
					ESP_LOGW(TAG_TCP, "Keepalive port config failed errno:%d\n", errno);
					sock = fconnSockConf(&stu_serverAddressConf);
				}
				i_cycles = 0;
			}
		}
	}
}

char* freadTcpString(char* out, int i_maxNumChars, int sock)
{
	uint8_t *data = (uint8_t *) malloc(i_maxNumChars);
	while(1)
	{
		int i_len = recv(sock, data, i_maxNumChars, 0);
		for (int i = 0; i < i_len; i++)
		{
			if((data[i]== '\r') || (data[i] == '\n') || (data[i] == '\0'))
			{
				out[i] = '\0';
			}
			else
			{
				out[i] = data[i];
			}
		}
		out[i_len+1] = '\0';
		if(out[0] != '\0')
			break;
	}
	return out;
}

int freadTcpCmdlet(int sock, int *i_cmdlet)
{
	int i_flag = 0;
	*i_cmdlet = 0;
	char data = '\0';
	for (int i = 0; i<4 ; i++)
	{
		recv(sock, &data, 1, 0);
		if ((data== '\r') || (data == '\n') || (data == '\0'))
		{
			break;
		}
		*i_cmdlet += (data << (8*(3-i)));
	}
	recv(sock, &data, 1, MSG_PEEK);
	if (data == ':')
	{
		recv(sock, &data, 1, 0);
		i_flag = 1;
	}
	ESP_LOGV(TAG_TCP, "CMDlet received:%d\n",*i_cmdlet);
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


int fconnSockConf (struct sockaddr_in* stu_serverAddressConf)
{
	char str[16] = "";
	int sock = 0;
	int i = 0;
	receiving_timeout.tv_sec = 0;
	receiving_timeout.tv_usec = 5000;
	inet_ntop(AF_INET, &stu_serverAddressConf->sin_addr.s_addr, (char*)str, sizeof(str));
	ESP_LOGI(TAG_TCP, "Connecting to Socket Conf at  %s:%d\n", str , ntohs(stu_serverAddressConf->sin_port));
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while (connect(sock, (struct sockaddr*) stu_serverAddressConf, sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (i == 100)
		{
			ESP_LOGW(TAG_TCP, "Could not connect to config Socket at %s:%d. errno = %d\n",
					str , ntohs(stu_serverAddressConf->sin_port), errno);
			i = 0;
		}
		i++;
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	while (setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&receiving_timeout,sizeof(receiving_timeout)) < 0)
	{
		ESP_LOGE(TAG_TCP, "failed to set config timeout errno=%d\n", errno);
	}
	return sock;
}

int fconnSockMes (struct sockaddr_in* stu_serverAddressMes)
{
	char str[16] = "";
	int sock = 0;
	int i = 0;
	inet_ntop(AF_INET, &stu_serverAddressMes->sin_addr.s_addr, (char*)str, sizeof(str));
	ESP_LOGI(TAG_TCP, "Connecting to measurement socket at  %s:%d\n", str , ntohs(stu_serverAddressMes->sin_port));
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while (connect(sock, (struct sockaddr *)&stu_serverAddressMes, sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (i == 100)
		{
			ESP_LOGW(TAG_TCP, "Could not connect to measurement Socket at %s:%d. errno = %d\n",
					str , ntohs(stu_serverAddressMes->sin_port), errno);
			i = 0;
		}
		i++;
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	return sock;
}

void fconfigTcp(uint32_t ui_cmdlet,struct stu_adcConfig* p_adcConfig_mom, struct sockaddr_in* serverAddress_mom)
{
	struct stu_adcConfig* p_adcConfig_ext = NULL;
	struct sockaddr_in* serverAddress_ext = NULL;
	switch(ui_cmdlet)
	{
	case CMD_ldtm:
	case CMD_ldtc:
		xQueuePeek(q_pointer, &serverAddress_ext, portMAX_DELAY);
		serverAddress_mom->sin_addr.s_addr = serverAddress_ext->sin_addr.s_addr;
		serverAddress_mom->sin_port = serverAddress_ext->sin_port;
		serverAddress_mom->sin_family = serverAddress_ext->sin_family;
		free(serverAddress_ext);
		xQueueReceive(q_pointer, &serverAddress_ext, portMAX_DELAY);
		break;

	case CMD_ldad:
		xQueuePeek(q_pointer, &p_adcConfig_ext, portMAX_DELAY);
		p_adcConfig_mom->uc_numDecimals		= p_adcConfig_ext->uc_numDecimals;
		free(p_adcConfig_ext);
		xQueueReceive(q_pointer, &p_adcConfig_ext, portMAX_DELAY);
		break;

	case CMD_svtm:
	case CMD_svtc:
		xQueueSend(q_pointer,&serverAddress_mom, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));
		break;

	case CMD_svad:
		xQueueSend(q_pointer,&p_adcConfig_mom, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));
		break;
	case CMD_ipco:
	case CMD_ipme:
	{
		uint32_t ui_temp = 0;
		xQueuePeek(q_pointer, &ui_temp, portMAX_DELAY);
		serverAddress_mom->sin_addr.s_addr = ui_temp;
		xQueueReceive(q_pointer, &ui_temp, portMAX_DELAY);
		break;
	}
	case CMD_poco:
	case CMD_pome:
	{
		uint32_t ui_temp = 0;
		xQueuePeek(q_pointer, &ui_temp, portMAX_DELAY);
		serverAddress_mom->sin_port = ui_temp;
		xQueueReceive(q_pointer, &ui_temp, portMAX_DELAY);
		break;
	}


	default:
		break;
	}
}
