/*
 * tcp_client.c
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#include "com_tcp.h"

struct timeval receiving_timeout;

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
	stu_mesCell dataPoint;
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
				ui_length += sprintf((char*)data + ui_length,
						"\1|\2%s\3|\2%llu\3|\2%s\3|\4\n",
						dataPoint.str_type, dataPoint.ul_time, dataPoint.str_data);
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
								&& (pstu_adcConfig->uc_txMode == TXMODE_TCP_PLUS_SDBACK))
						{
							//TODO:implement backup routine if connection is lost
							ESP_LOGD(TAG_TCP,
									"NOTIFYING SD.");
						}
						ESP_LOGD(TAG_TCP,
								"MES SOCKET SEND FAILED. RECONNECTING.");
						fconfigTcp(CMD_come,
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
									&& (pstu_adcConfig->uc_txMode == TXMODE_TCP_PLUS_SDBACK))
							{
								//TODO:implement backup routine if connection is lost
								ESP_LOGD(TAG_TCP,
										"NOTIFYING SD.");
							}
							ESP_LOGD(TAG_TCP,
									"MES SOCKET SEND FAILED. RECONNECTING.");
							fconfigTcp(CMD_come,
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
						if (!fsendKeepAlive(sock))
						{
							if((xEventGroupGetBits(eg_status) & STATUS_RECORDING)
									&& (pstu_adcConfig->uc_txMode == TXMODE_TCP_PLUS_SDBACK))
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
							sock = fconnSock(&pstu_hotspotConfig->portMes);
						}
						else
						{
							sock = fconnSock(&pstu_staConfig->portMes);
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
	int sock = 0;
	char* pc_txMessage = NULL;
	char* pc_configIn = malloc(128);
	receiving_timeout.tv_sec = 0;
	receiving_timeout.tv_usec = 5000;
	stu_configMessage stu_response;

	while(1)
	{
		if(xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, 10 / portTICK_PERIOD_MS) != 0)
		{
			if (ui_cmdlet == CMD_fire)
			{
				xQueueReceive(	q_tcpMessages,
								&pc_txMessage,
								portMAX_DELAY);
				if(sock)
				{
					if (0 > send(	sock,
									pc_txMessage,
									strlen((const char*)pc_txMessage),
									0))
					{
						//TODO: RECONNECT
						xQueueSend(	q_tcpMessages,
									&pc_txMessage,
									portMAX_DELAY);
					}
					else
					{
						free(pc_txMessage);
					}
				}

			}
			else
			{
				fconfigTcp(	ui_cmdlet,
							&sock);
			}
		}
		if(sock)
		{
			if (fgetMessage(sock, pc_configIn, 0))
			{
				xSemaphoreTake(hs_configCom,
						portMAX_DELAY);
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				xQueueReceive(q_pconfigOut,
						&stu_response,
						portMAX_DELAY);

				while (stu_response.pc_response)// as long as no NULL pointer is sent from task keep comm blocked
				{
					if (0 > send(	sock,
									(const char*) stu_response.pc_response,
									strlen((const char*)stu_response.pc_response),
									0))
					{
						sock = 0;
						if(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
							{
								for( int i = 0; (i < 5) & !sock; i++)
								{
									if(xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
									{
										sock = fconnSock(&pstu_hotspotConfig->portConf);
									}
									else
									{
										sock = fconnSock(&pstu_staConfig->portConf);
									}
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
					else
					{
						free(stu_response.pc_response);
						stu_response.pc_response = 0;
					}
					while (stu_response.uc_numResponses) //check if task expects response
					{
						if (fgetMessage(sock, pc_configIn, 0))
						{
							xQueueSend(q_pconfigIn,
									&pc_configIn,
									portMAX_DELAY);
							stu_response.uc_numResponses--;
						}

					}
					xQueueReceive(q_pconfigOut,
							&stu_response,
							portMAX_DELAY);
				}

				xSemaphoreGive(hs_configCom);
			}
			else
			{
				if(!fsendKeepAlive(sock))
				{
					close(sock);
					sock = 0;
					if(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
						{
							if(xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
							{
								sock = fconnSock(&pstu_hotspotConfig->portConf);
							}
							else
							{
								sock = fconnSock(&pstu_staConfigMom->portConf);
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
						sock = fconnSock(&pstu_hotspotConfig->portConf);
					}
					else
					{
						sock = fconnSock(&pstu_staConfigMom->portConf);
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
			else
			{
				xEventGroupClearBits(eg_status, STATUS_TCPCONF_CONN);
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
	int i_flag = 0;
	if (0 < send(sock,
			(const char*) "​\u200B",
			strlen((const char*)"\u200B"),
			0))
		i_flag = 1;
	return i_flag;
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
				{
					sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
					if (sock < 0)
					{
						if (errno != err_old)
						{
							ESP_LOGE(TAG_TCP, "Unable to accept connection at port:%d: errno %d",*port, errno);
							err_old = errno;
						}
						sock = 0;
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
				int* sock)
{
	char* pc_value = 0;
	char* pc_configOut = 0;
	char* pc_configIn = 0;

	switch(ui_cmdlet)
	{
	case 0:
		break;
	case CMD_wait:
		//send momentary config to sock mes
		break;

	case CMD_stpm:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tEnter port.\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);

		if(pc_value != 0)
		{
			pc_configOut = malloc(64);
			unsigned int ui_port = (uint32_t)strtol(pc_value, NULL, 10);
			if(ui_port <= 65353)
			{
				pc_configOut = malloc(64);
				portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
				portENTER_CRITICAL(&mux);
				pstu_staConfigMom->portMes = ui_port;
				portEXIT_CRITICAL(&mux);

				sprintf(pc_configOut,
						"|stpc|%d|\t\tOK, port of measurement server set to %d|\n",
						ui_port, ui_port);
				fsendResponse(0, 1, pc_configOut);
			}
			xEventGroupSync( eg_sync,
					BIT_TCPMES_SYNC,
					BIT_TCPMES_SYNC | BIT_CONFIG_SYNC,
					portMAX_DELAY );
		}
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


uint32_t fgetMessage(int sock, char* pc_configIn, uint32_t ui_timeout)
{
	uint32_t ui_return = 0;
	int i_cycles = 0;
	int i_flag = 0;
	uint64_t ul_timeBeginning = esp_timer_get_time();

	i_flag = freadTcpString(pc_configIn,
			128,
			sock);
	if (i_flag == 0) //timeout occurred no message present
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
	else if (i_flag < 0 ) //message did not have an delimiter or was too large for buffer
	{
		fsendMessage(sock, (char*)MESS_INVALID);
	}

	else
	{
		ui_return = 1;
	}

	return ui_return;
}


uint32_t	fsendMessage	(int sock,
							char* pc_configOut)
{
	while (send(sock,
			pc_configOut,
			strlen(pc_configOut),
			0) < 0)
	{
		if(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
		{
			if(xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
			{
				sock = fconnSock(&pstu_hotspotConfig->portConf);
			}
			else
			{
				sock = fconnSock(&pstu_staConfig->portConf);
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
		else
		{
			while (!(xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN))
			{
				vTaskDelay(10 / portTICK_PERIOD_MS);
			}
		}
	}
	return 0;
}
