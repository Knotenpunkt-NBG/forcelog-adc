/*
 * tcp_client.c
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#include "com_tcp.h"


void ftcpInit()
{
	htim_tcpSend =
			xTimerCreate("tim_tcpSend",
					PERIOD_TCPMES_PACKET_MS / portTICK_PERIOD_MS,
					pdTRUE,NULL,
					callback_buffer);
	xTaskCreate	(ttcpMes,
			"t_tcpSendMes",
			4096,
			NULL,
			8,
			&ht_tcpMes);
	xTaskCreate	(ttcpConf,
			"t_tcpConf",
			4096,
			NULL,
			8,
			&ht_tcpConf);
}


void ttcpMes (void* param)
{
	int sock = 0;
	uint32_t ui_cmdlet = 0;
	uint8_t* data = malloc(1400);
	struct stu_mesCell dataPoint;
	uint32_t ui_length = 0;
	uint32_t ui_counterKeepalive = 0;

	while(1)
	{
			while ((xQueueReceiveFromISR(q_measurements,
					&dataPoint,
					NULL) == pdTRUE)
						&& sock)
			{
				dataPoint.ul_time -= ul_zeroTime;
				switch (dataPoint.b_type)
				{
				case TYPE_ADC:
					ui_length += sprintf((char*)data + ui_length,
							"|meas|%.*f|%llu|\n",
							gstu_config->adc.uc_numDecimals, dataPoint.d_measurement,dataPoint.ul_time);
					break;
				case TYPE_BLINK:
					ui_length += sprintf((char*)data + ui_length,
							"|blk|%llu|\n",
							dataPoint.ul_time);
					break;
				case TYPE_TEMP_INT:
					ui_length += sprintf((char*)data + ui_length,
							"|tpin|%.*f|%llu|\n",
							2, dataPoint.d_measurement, dataPoint.ul_time);
					break;
				case TYPE_TEMP_EXT:
					ui_length += sprintf((char*)data + ui_length,
							"|tpex|%.*f|%llu|\n",
							2, dataPoint.d_measurement, dataPoint.ul_time);
					break;
				}
				if (ui_length >= 1300)
				{
					xTimerReset(htim_tcpSend, portMAX_DELAY);
					if (-1 == send(sock,
							(char *)data,
							strlen((const char*)data),
							0))
					{
						sock = 0;
						if((xEventGroupGetBits(eg_status) & STATUS_RECORDING)
								&& (gstu_config->adc.uc_txMode == TXMODE_TCP_PLUS_SDBACK))
						{
							//TODO:implement backup routine if connection is lost
							ESP_LOGD(TAG_TCP,
									"NOTIFYING SD.");
						}
						ESP_LOGD(TAG_TCP,
								"MES SOCKET SEND FAILED. RECONNECTING.");
						fconfigTcp(CMD_come,
								gstu_config->apConfig.portMes,
								&sock);
					}
					ui_length = 0;
				}
				ui_cmdlet = 0;
			}
			if(pdTRUE == xTaskNotifyWait(false,	//check if cmdlets arrived
					ULONG_MAX,
					&ui_cmdlet,
					10 / portTICK_PERIOD_MS))
			{
				if (ui_cmdlet == CMD_zero)
				{
					ul_zeroTime = esp_timer_get_time();
				}
				else if (ui_cmdlet == CMD_fire)
				{
					if (ui_length > 0) //check if data to be send is present
					{
						if (-1 == send(sock,
								(char *)data,
								strlen((const char*)data),
								0))
						{
							if((xEventGroupGetBits(eg_status) & STATUS_RECORDING)
									&& (gstu_config->adc.uc_txMode == TXMODE_TCP_PLUS_SDBACK))
							{
								//TODO:implement backup routine if connection is lost
								ESP_LOGD(TAG_TCP,
										"NOTIFYING SD.");
							}
							ESP_LOGD(TAG_TCP,
									"MES SOCKET SEND FAILED. RECONNECTING.");
							fconfigTcp(CMD_come,
									gstu_config->apConfig.portMes,
									&sock);
						}
						ui_length = 0;
					}

				}
				else if (ui_cmdlet == CMD_strt)
				{
					char* pc_temp = 0;
					xQueueReceive(q_send, &pc_temp, portMAX_DELAY);
					send(sock,
							(char *)pc_temp,
							strlen((const char*)pc_temp),
							0);
					xEventGroupSync(eg_sync,
							BIT_TCPMES_SYNC,
							BIT_CONFIG_SYNC | BIT_TCPMES_SYNC,
							portMAX_DELAY);
				}
				else
				{
					fconfigTcp(ui_cmdlet,
							gstu_config->apConfig.portMes,
							&sock);
				}
				ui_cmdlet = 0;
			}
			else //otherwise send keepalive to check if socket is still open
			{
				if(sock)
				{
					ui_counterKeepalive ++;
					if (ui_counterKeepalive >= 10)
					{
						if (fsendKeepAlive(sock) < 0)
						{
							if((xEventGroupGetBits(eg_status) & STATUS_RECORDING)
									&& (gstu_config->adc.uc_txMode == TXMODE_TCP_PLUS_SDBACK))
							{
								//TODO:implement backup routine if connection is lost
								ESP_LOGD(TAG_TCP,
										"NOTIFYING SD.");
							}
							ESP_LOGW(TAG_TCP,
									"Keepalive socked mes failed. errno:%d\n",
									errno);
							sock = 0;
						}
						ui_counterKeepalive = 0;
					}
				}
				else
				{
					if(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
					{
						if(xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
						{
							sock = fconnSock(&gstu_config->apConfig.portMes);
						}
						else
						{
							sock = fconnSock(&gstu_config->staConfig.portMes);
						}
						if (sock)
						{
							xEventGroupSetBits(eg_status, STATUS_TCPMES_CONN);
						}
						else
						{
							xEventGroupClearBits(eg_status, STATUS_TCPMES_CONN);
						}
					}
				}
			}
	}
}


void ttcpConf (void* param)
{
	uint32_t ui_cmdlet = 0;
	int i_cycles = 0;
	int sock = 0;
	int i_flag = 0;
	char* pc_txMessage = NULL;
	char* pc_configOut = NULL;
	char* pc_configIn = malloc(64);
	receiving_timeout.tv_sec = 0;
	receiving_timeout.tv_usec = 5000;

	while(1)
	{
		if(xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, 10 / portTICK_PERIOD_MS) != 0)
		{
			if (ui_cmdlet == CMD_fire)
			{
				xQueueReceive(q_tcpMessages,
						&pc_txMessage,
						portMAX_DELAY);
				if(sock)
				{
					send(sock,
							pc_txMessage,
							strlen((const char*)pc_txMessage),
							0);
				}
				free(pc_txMessage);
			}
			else
			{
				fconfigTcp(ui_cmdlet,
						gstu_config->apConfig.portConf,
						&sock);
			}
			i_cycles = 0;
		}
		if(sock)
		{
			i_flag = freadTcpString(pc_configIn,
					64,
					sock);
			if (i_flag <= 0) //timeout occurred no message present
			{
				i_cycles++;
				if (i_cycles == CYCLES_RECON_CONF)
				{
					if (fsendKeepAlive(sock) < 0)
					{
						close(sock);
						sock = 0;
					}
					i_cycles = 0;
				}
			}
			else if (i_flag == 0 ) //message did not have an delimiter or was too large for buffer
			{
				if (send(sock,
						(const char*)MESS_INVALID,
						strlen((const char*)MESS_INVALID),
						0) < 0)
				{
					if(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
						{
							if(xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
							{
								sock = fconnSock(&gstu_config->apConfig.portConf);
							}
							else
							{
								sock = fconnSock(&gstu_config->staConfig.portConf);
							}
							if (sock)
							{
								xEventGroupSetBits(eg_status, STATUS_TCPCONF_CONN);
							}
							else
							{
								xEventGroupClearBits(eg_status, STATUS_TCPCONF_CONN);
							}
						}
				}
			}
			else //message received correctly
			{
				xSemaphoreTake(hs_configCom,
						portMAX_DELAY);
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				xQueueReceive(q_pconfigOut,
						&pc_configOut,
						portMAX_DELAY);
				while (pc_configOut != 0) //config requests more info
				{
					if (send(sock,
							(const char*) pc_configOut,
							strlen((const char*)pc_configOut),
							0) < 0)
					{
						if(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
							{
								if(xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
								{
									sock = fconnSock(&gstu_config->apConfig.portConf);
								}
								else
								{
									sock = fconnSock(&gstu_config->staConfig.portConf);
								}
								if (sock)
								{
									xEventGroupSetBits(eg_status, STATUS_TCPCONF_CONN);
								}
								else
								{
									xEventGroupClearBits(eg_status, STATUS_TCPCONF_CONN);
								}
							}
					}

					while(freadTcpString(pc_configIn, 64, sock) <= 0)
						vTaskDelay(100 / portTICK_PERIOD_MS);
					xQueueSend(q_pconfigIn,
							&pc_configIn,
							portMAX_DELAY);
					xQueueReceive(q_pconfigOut,
							&pc_configOut,
							portMAX_DELAY);
				}
				xQueueReceive(q_pconfigOut,
						&pc_configOut,
						portMAX_DELAY); //NULL pointer received send the last response

				if (send(sock,
						(const char*) pc_configOut,
						strlen((const char*)pc_configOut),
						0))
				{
					if(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
						{
							if(xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
							{
								sock = fconnSock(&gstu_config->apConfig.portConf);
							}
							else
							{
								sock = fconnSock(&gstu_config->staConfig.portConf);
							}
							if (sock)
							{
								xEventGroupSetBits(eg_status, STATUS_TCPCONF_CONN);
							}
							else
							{
								xEventGroupClearBits(eg_status, STATUS_TCPCONF_CONN);
							}
						}
				}
				xSemaphoreGive(hs_configCom);
				xEventGroupSync( eg_config,
						BIT_COMM_SYNC,
						BIT_COMM_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
		}
		else
		{
			if (xEventGroupGetBits(eg_status) & STATUS_TCPCONF_CONN)
			{
				ESP_LOGW(TAG_TCP, "CONFIG SOCKET LOST");
				xEventGroupClearBits(eg_status, STATUS_TCPCONF_CONN);
			}

			if(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
				{
					if(xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
					{
						sock = fconnSock(&gstu_config->apConfig.portConf);
					}
					else
					{
						sock = fconnSock(&gstu_config->staConfig.portConf);
					}
					if (sock)
					{
						xEventGroupSetBits(eg_status, STATUS_TCPCONF_CONN);
					}
					else
					{
						xEventGroupClearBits(eg_status, STATUS_TCPCONF_CONN);
					}
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
			if((c_single== '\r')
					|| (c_single == '\n')
					|| (c_single == '\0'))
			{
				out[i] = '\0';
				i_flag = 1;
				break;
			}
			else
			{
				out[i] = c_single;
				if(i == i_maxNumChars-1)
					i_flag = -1;
			}
		}
		else
		{
			i_flag = 0;
			break;
		}
	}
	return i_flag;
}


void fsendAck (int sock)
{
	send(sock,
			(const char*) "|ack|\n",
			strlen((const char*)"|ack|\n"),
			0);
}


int fsendKeepAlive (int sock)
{
	return send(sock,
			(const char*) "​\u200B",
			strlen((const char*)"\u200B"),
			0);
}


int fconnSock (in_port_t* port)
{
	char addr_str[128];
	struct sockaddr_in dest_addr;
	int sock = 0;
	int err_old = 0;
	dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(*port);

	int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (listen_sock < 0) {
		ESP_LOGE(TAG_TCP, "Unable to create socket at port:%d: errno %d",*port, errno);
	}
	else
	{
		int opt = 1;
		setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
		if (err != 0) {
			ESP_LOGE(TAG_TCP, "Socket unable to bind: errno %d", errno);
			ESP_LOGE(TAG_TCP, "IPPROTO: %d", AF_INET);
		}
		else
		{
			err = listen(listen_sock, 1);
			if (err != 0) {
				ESP_LOGE(TAG_TCP, "Error occurred during listen: errno %d", errno);
			}
			else
			{
				struct timeval tval_acceptTimeout;
				tval_acceptTimeout.tv_sec = 1;
				tval_acceptTimeout.tv_usec = 0;
				setsockopt(listen_sock,
						SOL_SOCKET,
						SO_RCVTIMEO,
						&tval_acceptTimeout,
						sizeof(tval_acceptTimeout));
				struct sockaddr_in source_addr;
				uint addr_len = sizeof(source_addr);
				while(sock <= 0)
				{
					sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
					if (sock < 0)
					{
						if (htons(*port) != dest_addr.sin_port)
						{
							ESP_LOGD(TAG_TCP, "CHANGE OF PORT NOTICED");
							close(sock);
							sock = 0;
							break;
						}
						if (errno != err_old)
						{
							ESP_LOGE(TAG_TCP, "Unable to accept connection at port:%d: errno %d",*port, errno);
							err_old = errno;
						}
					}
					else
					{
						ESP_LOGI(TAG_TCP, "Socket(%d) bound, port %d", sock, *port);
						inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);
						ESP_LOGI(TAG_TCP, "Socket accepted ip address: %s SOCKET:%d", addr_str, sock);
						setsockopt(sock,
								SOL_SOCKET,
								SO_RCVTIMEO,
								&receiving_timeout,
								sizeof(receiving_timeout));
						break;
					}
				}

			}
		}
	}
	if(listen_sock)
		close(listen_sock);

	return sock;
}


void fconfigTcp	(uint32_t ui_cmdlet,
				in_port_t port,
				int* sock)
{
	switch(ui_cmdlet)
	{
	case 0:
		break;
	case CMD_wait:
		//send momentary config to sock mes
		break;

	case CMD_ldad:
	case CMD_ldtm:
		xEventGroupSync( eg_sync,
				BIT_TCPMES_SYNC,
				BIT_TCPMES_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_config,
				BIT_TCPMES_SYNC,
				BIT_TCPMES_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_ldtc:
		xEventGroupSync( eg_sync,
				BIT_TCPCONF_SYNC,
				BIT_TCPCONF_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_config,
				BIT_TCPMES_SYNC,
				BIT_TCPMES_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_zero:
		break;
	default:
		ESP_LOGD(TAG_TCP,
				"WRONG CMDLET");
		break;
	}
}


void callback_buffer (void* param)
{
	while(pdPASS != xTaskNotify(ht_tcpMes,
			CMD_fire,
			eSetValueWithoutOverwrite) );
}
