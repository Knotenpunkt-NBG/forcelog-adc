/*
 * tcp_client.c
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#include "com_tcp.h"

//TODO: add sockets in server function if in AP Mode

void ftcpInit()
{
	ESP_LOGI(TAG_TCP, "STARTING TCP\n");
	stu_serverAddressMes.sin_family			=	AF_INET;
	stu_serverAddressConf.sin_family		=	AF_INET;
	stu_serverAddressUdp.sin_family			=	AF_INET;
	stu_serverAddressUdp.sin_addr.s_addr	=	htonl(INADDR_ANY);

	xTaskCreate	(t_tcpSend,		"t_tcpSendMes",		4096,	NULL	,	10,		NULL);
	xTaskCreate	(t_tcpConf,		"t_tcpConf",		4096,	NULL	,	10,		NULL);
	xEventGroupWaitBits(eg_tcp,	BIT_TCPSENDMES_READY,	true,true,portMAX_DELAY);
	xEventGroupWaitBits(eg_tcp,	BIT_TCPSENDCONF_READY,	true,true,portMAX_DELAY);
}

void t_tcpSend (void* param)
{
	int sock = 0;
	uint8_t *data[64] = {};
	uint64_t time = 0;
	double measurement = 0.0;
	float f_temperature = 0.0;
	xEventGroupSetBits(eg_tcp, BIT_TCPSENDMES_READY);
	sock = connSockMes();
	ul_zeroTime = 0;
	while(1)
	{
		xEventGroupWaitBits(eg_tcp,BIT_TCPSEND,true,true,TICKS_RECON_MES);
		//Sends measurements
		if(xQueuePeekFromISR(q_time_mes, &time))
		{
			xQueueReceive(q_measurement, &measurement, portMAX_DELAY);
			xQueueReceive(q_time_mes, &time, portMAX_DELAY);
			time -= ul_zeroTime;
			sprintf((char*)data, "|meas|%.*f|%llu|\n", 2, measurement,time);
			if (send(sock, (char *)data, strlen((const char*)data), 0) == -1)
			{
				close(sock);
				sock = connSockMes();
			}
			ESP_LOGV(TAG_TCP, "SENDING MEASUREMENT\n");
		}
		//sends blink time
		else if(xQueuePeekFromISR(q_time_blink, &time))
		{
			xQueueReceive(q_time_blink, &time, portMAX_DELAY);
			time -= ul_zeroTime;
			sprintf((char*)data, "|blk|%llu|\n", time);
			if (send(sock, (char *)data, strlen((const char*)data), 0) == -1)
			{
				close(sock);
				sock = connSockMes();
			}
			ESP_LOGV(TAG_TCP, "SENDING BLINK\n");
		}
		//send temperature
		else if(xQueuePeekFromISR(q_time_temp, &time))
		{
			xQueueReceive(q_temperature, &f_temperature, portMAX_DELAY);
			xQueueReceive(q_time_temp, &time, portMAX_DELAY);
			time -= ul_zeroTime;
			sprintf((char*)data, "|temp|%.*f|%llu|\n",2, f_temperature, time);
			if (send(sock, (char *)data, strlen((const char*)data), 0) == -1)
			{
				close(sock);
				sock = connSockMes();
			}
			ESP_LOGV(TAG_TCP, "SENDING Temperature\n");
		}
		else
		{

			if (sendKeepAlive(sock) == -1)
			{
				ESP_LOGV(TAG_TCP, "KEEPALIVE MES FAILED\n");
				close(sock);
				sock = connSockMes();
			}
		}
	}
}

void t_tcpConf (void* param)
{

	int i_cycles = 0;
	int sock = 0;
	int i_cmdlet = 0;
	int i_flag = 0;
	int i_flagConfig = 0;
	char rx_buffer[128];
	char* pc_txMessage;
	xEventGroupSetBits(eg_tcp, BIT_TCPSENDCONF_READY);
	sock = connSockConf();
	while(1)
	{
		xEventGroupWaitBits(eg_tcp, BIT_TCPQUEUE_READY, false, true, TICKS_TIMEOUT_CONF);
		if (xEventGroupGetBits(eg_tcp) & BIT_TCPQUEUE_READY)
		{
			xQueueReceive(q_tcpConf, &pc_txMessage, portMAX_DELAY);
			send(sock, pc_txMessage, strlen((const char*)pc_txMessage), 0);
			xEventGroupClearBits(eg_tcp, BIT_TCPQUEUE_READY);
		}

		i_flag = readTcpCmdlet(sock, &i_cmdlet);
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
						sendAck(sock);
					readTcpString((char*)rx_buffer, 50, sock);
				}
				i_flagConfig = fConfig(i_cmdlet, rx_buffer);
				if (i_flagConfig)
				{
					ESP_LOGD(TAG_TCP, "CONFIG FLAG:%d\n", i_flagConfig);
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
				if (sendKeepAlive(sock) < 0)
				{
					ESP_LOGW(TAG_TCP, "Keepalive port config failed errno:%d\n", errno);
					sock = connSockConf();
				}
				i_cycles = 0;
			}
		}
	}
}



char* readTcpString(char* out, int i_maxNumChars, int sock)
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

int readTcpCmdlet(int sock, int *i_cmdlet)
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

void sendAck (int sock)
{
	send(sock, (const char*) "|ack|\n", strlen((const char*)"|ack|\n"), 0);
}

int sendKeepAlive (int sock)
{
	return send(sock, (const char*) "​\u200B", strlen((const char*)"\u200B"),0);
}

int connSockConf (void)
{
	char str[16] = "";
	int sock = 0;
	int i = 0;
	receiving_timeout.tv_sec = 0;
	receiving_timeout.tv_usec = 5000;
	inet_ntop(AF_INET, &stu_serverAddressConf.sin_addr.s_addr, (char*)str, sizeof(str));
	ESP_LOGI(TAG_TCP, "Connecting to Socket Conf at  %s:%d\n", str , ntohs(stu_serverAddressConf.sin_port));
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while (connect(sock, (struct sockaddr *)&stu_serverAddressConf, sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (i == 100)
		{
			ESP_LOGW(TAG_TCP, "Could not connect to config Socket at %s:%d. errno = %d\n",
					str , ntohs(stu_serverAddressConf.sin_port), errno);
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

int connSockMes (void)
{
	char str[16] = "";
	int sock = 0;
	int i = 0;
	inet_ntop(AF_INET, &stu_serverAddressMes.sin_addr.s_addr, (char*)str, sizeof(str));
	ESP_LOGI(TAG_TCP, "Connecting to measurement socket at  %s:%d\n", str , ntohs(stu_serverAddressMes.sin_port));
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while (connect(sock, (struct sockaddr *)&stu_serverAddressMes, sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (i == 100)
		{
			ESP_LOGW(TAG_TCP, "Could not connect to measurement Socket at %s:%d. errno = %d\n",
					str , ntohs(stu_serverAddressMes.sin_port), errno);
			i = 0;
		}
		i++;
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	return sock;
}
