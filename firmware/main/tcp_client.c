/*
 * tcp_client.c
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#include "tcp_client.h"



void t_tcpInit(void *arg)
{
	printf("WAITING WIFI WAITING WIFI WAITING WIFI\n");
	xEventGroupWaitBits(s_wifi_event_group,WIFI_CONNECTED_BIT,	true,true,portMAX_DELAY);
	printf("STARTING TCP STARTING TCP STARTING TCP\n");
	ul_zeroTime = 0;
	receiving_timeout.tv_sec = 0;
	receiving_timeout.tv_usec = 5000;
	str_serverAddressMes.sin_family			=	AF_INET;
	str_serverAddressConf.sin_family		=	AF_INET;
	str_serverAddressUdp.sin_family			=	AF_INET;
	str_serverAddressUdp.sin_port			=	htons(4244);
	str_serverAddressUdp.sin_addr.s_addr	=	htonl(INADDR_ANY);

	int sock_mes = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	printf("socket mes:  %d\n", sock_mes);

	int i = 0;
	while (connect(sock_mes, (struct sockaddr *)&str_serverAddressMes, sizeof(struct sockaddr_in)) == -1)
	{
		close(sock_mes);
		sock_mes = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (i == 100)
		{
			ESP_LOGW(TAG_TCP, "Could not connect to measurement Socket at %d:%d. errno = %d\n",
					str_serverAddressMes.sin_addr.s_addr,str_serverAddressMes.sin_port, errno);
			i = 0;
		}
		i++;
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	xTaskCreate	(t_tcpSendMes,	"t_tcpSendMes",		4096,	(void*)sock_mes,	10,		NULL);
	xTaskCreate	(t_tcpConf,		"t_tcpConf",		4096,	NULL	,			10,		NULL);
	xTaskCreate	(t_tcpIdle,		"t_tcpIdle",		1024,	(void*)sock_mes,	10,		h_t_tcpIdle);
	xEventGroupWaitBits(eg_tcp,	BIT_TCPSENDMES_READY,	true,true,portMAX_DELAY);
	xEventGroupWaitBits(eg_tcp,	BIT_TCPSENDCONF_READY,	true,true,portMAX_DELAY);
	xEventGroupWaitBits(eg_tcp,	BIT_TCPIDLE_READY,		true,true,portMAX_DELAY);

	vTaskDelete(NULL);
}

void t_tcpIdle (void* param)
{
	//int sock = (int) param;
	xEventGroupSetBits (eg_tcp,		BIT_TCPIDLE_READY);
	xEventGroupWaitBits(eg_tcpRun,	BIT_TCPRECON,	true,true,portMAX_DELAY);

	vTaskDelete(NULL);
}

void t_tcpSendMes (void* param)
{
	int sock = 0;
	uint8_t b_egBits = 0;
	uint8_t *data[64] = {};
	uint64_t time = 0;
	sock = (int) param;
	double measurement = 0.0;
	xEventGroupSetBits(eg_tcp, BIT_TCPSENDMES_READY);
	while(1)
	{
		xEventGroupWaitBits(eg_tcp,BIT_SENDMES | BIT_SENDBLINK,false,false,TICKS_RECON_MES);
		b_egBits = xEventGroupGetBits(eg_tcp) & 0x30;
		if (b_egBits == BIT_SENDMES)
		{
			xQueueReceive(q_measurement, &measurement, portMAX_DELAY);
			xQueueReceive(q_time_mes, &time, portMAX_DELAY);
			time -= ul_zeroTime;
			sprintf((char*)data, ":mes:%.*f:%llu:\n", 2, measurement,time);
			if (send(sock, (char *)data, strlen((const char*)data), 0) == -1)
			{
				int rc = 1;
				while(rc != 0)
				{
					close(sock);
					sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					rc = connect(sock, (struct sockaddr *)&str_serverAddressMes, sizeof(struct sockaddr_in));
					if (rc != 0)
					{
						ESP_LOGE(TAG_TCP, "error connectin to socket mes. errno:%d\n", errno);
					}
					vTaskDelay(TICKS_RECON_MES);
				}
			}
			xEventGroupClearBits(eg_tcp, BIT_SENDMES);
		}
		else if (b_egBits == BIT_SENDBLINK)
		{
			xQueueReceive(q_time_blink, &time, portMAX_DELAY);
			time -= ul_zeroTime;
			sprintf((char*)data, "blk:%llu\n", time);
			send(sock, (char *)data, strlen((const char*)data), 0);
			xEventGroupClearBits(eg_tcp, BIT_SENDBLINK);
		}
		else if (b_egBits == 0)
		{
			if (sendKeepAlive(sock) == -1)
			{
				int rc = 1;
				while(rc != 0)
				{
					close(sock);
					sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					rc = connect(sock, (struct sockaddr *)&str_serverAddressMes, sizeof(struct sockaddr_in));
					if (rc != 0)
					{
						ESP_LOGE(TAG_TCP, "error connecting to socket mes. errno:%d\n", errno);
					}
					vTaskDelay(TICKS_RECON_MES);
				}
			}
		}
		b_egBits = 0;
	}
}

void t_tcpConf (void* param)
{
	int i_cycles = 0;
	int sock = 0;
	int i_cmdlet = 0;
	int i_flag = 0;
	char rx_buffer[128];

	sock = connSockConf();

	xEventGroupSetBits(eg_tcp, BIT_TCPSENDCONF_READY);

	uint8_t *data[128] = {};
	while(1)
	{
		xEventGroupWaitBits(eg_tcp, BIT_TCPQUEUE_READY, false, true, TICKS_TIMEOUT_CONF);
		if (xEventGroupGetBits(eg_tcp) & BIT_TCPQUEUE_READY)
		{
			xQueueReceive(q_tcpConf, &data, portMAX_DELAY);
			ESP_LOGD(TAG_TCP, "sending:%s\n",(char*)data);
			//send(sock, (char *)data, strlen((const char*)data), 0);
			send(sock, (const char*) "TESTITEST", strlen((const char*)"TESTITEST"), 0);
			xEventGroupClearBits(eg_tcp, BIT_TCPQUEUE_READY);
		}

		i_flag = readTcpCmdlet(sock, &i_cmdlet);
		if (i_cmdlet != 0)
		{
			ESP_LOGV(TAG_TCP, "Config CMDlet:%d\n", i_cmdlet);
			switch (i_cmdlet)
			{
			case CMD_man: //inquires manual
				break;
			case CMD_info: //inquires all the information
				break;
			//SINGLE INSTRUCTION CMDLETS
			case CMD_tare: //tare
			case CMD_stop: //stops the sampling and kills the tasks
			case CMD_strt: //starts the adc measurements as soon as socket mes is connected without wating for an trigger (udp or pin)
			case CMD_benb: //enables blink
			case CMD_blnk: //single shot blink without TCP message
			case CMD_zero: //sets the momentary time to zero
			case CMD_bdis: //disables blink
			case CMD_save: //saves config of ADC and blink to non volatile storage
			case CMD_load: //load config of ADC and blink from volatile storage
			case CMD_loge: //Sets logging to error (highest)
			case CMD_logw: //Sets logging to warning
			case CMD_logi: //Sets logging to info
			case CMD_logd: //Sets logging to debug
			case CMD_logv: //Sets logging to verbose (lowest)
			case CMD_scal: //starts calibration once a known weight has been placed
			case CMD_conn: //connects to the access point
				fConfig(i_cmdlet, NULL);
				sendAck(sock);
				break;

				//Double Instruction CMDLETS
			case CMD_bbrt: //brightness of blink (0-1023)
			case CMD_bdur: //duration of blink
			case CMD_bper: //period of blink
			case CMD_bfrq: //sets frequency of blink (1-78125Hz)
			case CMD_mper: //sets the sample period in us
			case CMD_vcal: //reads calibration value from server and writes it to adc
			case CMD_rcal: //calibrates with an known weight and sends the factor back to the server
			case CMD_ssid: //SSID of access point
			case CMD_pass: //pass for access point
			case CMD_ipco: //IP for configuration server
			case CMD_ipme: //IP for logging server
			case CMD_poco: //port for configuration server
			case CMD_pome: //port for logging server
			case CMD_potr: //port for UDP trigger
				if (i_flag == 0)
				{
					sendAck(sock);
					readTcpString((char*)rx_buffer, 50, sock);
					fConfig (i_cmdlet, rx_buffer);
					sendAck(sock);
				}
				else if (i_flag == 1)
				{
					readTcpString((char*)rx_buffer, 50, sock);
					fConfig (i_cmdlet, rx_buffer);
					sendAck(sock);
				}
				break;

				//SPECIAL CASES
			case CMD_wait: //starts the adc in an easy to trigger status until trigger (udp or pin) is received
				fConfig(i_cmdlet, NULL);
				xTaskCreate (t_udpWait, "t_udpWait", 2048, NULL, 10, NULL);
				sendAck(sock);
				break;

			default:
				send(sock, (const char*) "inv\n", strlen((const char*)"inv\n"), 0);
				break;
			}
		}
		else if (i_cmdlet == 0)
		{
			i_cycles++;
			ESP_LOGV(TAG_TCP, "Cycles Keepalive config: %d\n", i_cycles);
			if (i_cycles == CYCLES_RECON_CONF)
			{
				ESP_LOGD(TAG_TCP, "Sending Keepalive\n");
				if (sendKeepAlive(sock) < 0)
				{
					ESP_LOGW(TAG_TCP, "Keepalive failed errno:%d\n", errno);
					connSockConf();
				}
				i_cycles = 0;
			}
		}
	}
}

void t_udpWait (void* param)
{
	ESP_LOGI(TAG_UDP, "Waiting for UDP trigger");
	int sock_udp = socket (AF_INET, SOCK_DGRAM, 0);
	int i_cmdlet = 0;
	int err = bind(sock_udp, (struct sockaddr *)&str_serverAddressUdp, sizeof(struct sockaddr_in));
	if (err < 0) {
		ESP_LOGI(TAG_UDP, "Socket unable to bind: errno %d", errno);
	}
	while (i_cmdlet != CMD_rec)
	{
		readTcpCmdlet(sock_udp, &i_cmdlet);
	}
	xEventGroupSetBits(eg_adc, BIT_ADC_WAIT);
	esp_timer_start_periodic(h_timerBlink, struct_blinkConfig.ui_blinkPeriod * 1000);
	ESP_LOGI(TAG_UDP, "UDP trigger received");
	shutdown(sock_udp, 0);
	close(sock_udp);
	vTaskDelete(NULL);
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
	char data = 0;
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
	send(sock, (const char*) "ack\n", strlen((const char*)"ack\n"), 0);
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
	inet_ntop(AF_INET, &str_serverAddressConf.sin_addr.s_addr, (char*)str, sizeof(str));
	ESP_LOGI(TAG_TCP, "Connecting to Socket Conf at  %s:%d\n", str , ntohs(str_serverAddressConf.sin_port));
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	while (connect(sock, (struct sockaddr *)&str_serverAddressConf, sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (i == 100)
		{
			ESP_LOGW(TAG_TCP, "Could not connect to config Socket at %s:%d. errno = %d\n",
					str , ntohs(str_serverAddressConf.sin_port), errno);
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
	int sock = 0;
	return sock;
}
