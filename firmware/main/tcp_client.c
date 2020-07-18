/*
 * tcp_client.c
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#include "tcp_client.h"


void t_tcpInit(void *arg)
{
	ul_zeroTime = 0;
	str_serverAddressMes.sin_family = AF_INET;
	str_serverAddressConf.sin_family = AF_INET;
	xEventGroupWaitBits(s_wifi_event_group,WIFI_CONNECTED_BIT,true,true,portMAX_DELAY);
	printf("Connecting to Socket Mes. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
	printf("Connecting to Socket Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);
	int sock_mes = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int sock_conf = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	printf("socket mes:  %d\n", sock_mes);
	printf("socket conf:  %d\n", sock_conf);

	int rc_mes = connect(sock_mes, (struct sockaddr *)&str_serverAddressMes, sizeof(struct sockaddr_in));
	int rc_conf = connect(sock_conf, (struct sockaddr *)&str_serverAddressConf, sizeof(struct sockaddr_in));
	printf("connect rc mes: %d\n", rc_mes);
	printf("connect rc conf: %d\n", rc_conf);

	xTaskCreate(t_tcpSendMes,"t_tcpSendMes",2048,(void*)sock_mes,10,NULL);
	xTaskCreate(t_tcpRecvConf,"t_tcpRecvConf",2048,(void*)sock_conf,10,NULL);
	xTaskCreate (t_tcpSendConf, "t_tcpSendConf", 2048, (void*) sock_conf, 10, NULL);

	xEventGroupWaitBits(eg_tcp,BIT_TCPSENDMES_READY,true,true,portMAX_DELAY);
	xEventGroupWaitBits(eg_tcp,BIT_TCPSENDCONF_READY,true,true,portMAX_DELAY);
	xEventGroupWaitBits(eg_tcp,BIT_TCPRECVCONF_READY,true,true,portMAX_DELAY);

	//rc = close(sock);
	//printf("close: rc: %d\n", rc);

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
		xEventGroupWaitBits(eg_tcp,BIT_SENDMES | BIT_SENDBLINK,false,false,portMAX_DELAY);
		b_egBits = xEventGroupGetBits(eg_tcp) & 0x30;
		if (b_egBits == BIT_SENDMES)
		{
			xQueueReceive(q_measurement, &measurement, portMAX_DELAY);
			xQueueReceive(q_time, &time, portMAX_DELAY);
			time -= ul_zeroTime;
			sprintf((char*)data, "mes:%.*f:%llu\n", 2, measurement,time);
			send(sock, (char *)data, strlen((const char*)data), 0);
			xEventGroupClearBits(eg_tcp, BIT_SENDMES);
		}
		else if (b_egBits == BIT_SENDBLINK)
		{
			xQueueReceive(q_time, &time, portMAX_DELAY);
			time -= ul_zeroTime;
			sprintf((char*)data, "blk:%llu\n", time);
			send(sock, (char *)data, strlen((const char*)data), 0);
			xEventGroupClearBits(eg_tcp, BIT_SENDBLINK);
		}
		b_egBits = 0;
	}
}

void t_tcpSendConf (void* param)
{
	int sock = 0;
	sock = (int) param;
	xEventGroupSetBits(eg_tcp, BIT_TCPSENDCONF_READY);
	uint8_t *data[128] = {};
	while(1)
	{
		xEventGroupWaitBits(eg_tcp, BIT_TCPQUEUE_READY, false, true, portMAX_DELAY);
		xQueueReceive(q_tcpConf, &data, portMAX_DELAY);
		printf("sending:%s\n",(char*)data);
		//send(sock, (char *)data, strlen((const char*)data), 0);
		send(sock, (const char*) "TESTITEST", strlen((const char*)"TESTITEST"), 0);
		xEventGroupClearBits(eg_tcp, BIT_TCPQUEUE_READY);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void t_tcpRecvConf (void* param)
{
	int sock = 0;
	sock = (int) param;
	xEventGroupSetBits(eg_tcp, BIT_TCPRECVCONF_READY);

	while(1)
	{
		int i_cmdlet = 0;
		char rx_buffer[128];
		char  s_calValue[50] = {};
		while (i_cmdlet == 0)
		{
			i_cmdlet = readTcpCmdlet(sock);
		}
		switch (i_cmdlet)
		{
		case 0:
			//xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
			break;

		case CMD_tare: //tare
			xQueueSend(q_cmdlet, &i_cmdlet,  0);
			xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
			break;

		case CMD_cali: //calibrates with an known weight and sends the factor back
			xQueueSend(q_cmdlet, &i_cmdlet,  0);
			xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
			sendAck(sock);
			readTcpString((char*)s_calValue, 50, sock);
			double d_calValue = atof((char*)s_calValue);
			xQueueSend(q_conf, &d_calValue,  0);
			sendAck(sock);
			break;

		case CMD_wcal: //reads calibration value from server and writes it to adc
			xQueueSend(q_cmdlet, &i_cmdlet,  0);
			xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
			sendAck(sock);
			break;

		case CMD_strt:
			xQueueSend(q_cmdlet, &i_cmdlet,  0);
			xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
			xQueueSend(q_cmdlet, &i_cmdlet,  0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			break;

		case CMD_stop:
			xQueueSend(q_cmdlet, &i_cmdlet,  0);
			xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
			xQueueSend(q_cmdlet, &i_cmdlet,  0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			break;

		case CMD_mper: //samplerate
			xQueueSend(q_cmdlet, &i_cmdlet,  0);
			xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
			sendAck(sock);
			readTcpString((char*)rx_buffer, 50, sock);
			uint64_t ul_mesPeriod = atoll((char*)rx_buffer);
			xQueueSend(q_conf, &ul_mesPeriod,  0);
			sendAck(sock);
			break;

		case CMD_bper: //period of blink
			xQueueSend(q_cmdlet, &i_cmdlet, 0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			readTcpString((char*)rx_buffer, 50, sock);
			uint32_t ui_blinkPeriod = atoi((char*)rx_buffer);
			xQueueSend(q_conf, &ui_blinkPeriod, 0);
			break;

		case CMD_bdur: //duration of blink
			xQueueSend(q_cmdlet, &i_cmdlet, 0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			readTcpString((char*)rx_buffer, 50, sock);
			uint32_t ui_blinkDuration = atoi((char*)rx_buffer);
			xQueueSend(q_conf, &ui_blinkDuration, 0);
			break;

		case CMD_bbrt: //brightness of blink (0-1023)
			xQueueSend(q_cmdlet, &i_cmdlet, 0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			readTcpString((char*)rx_buffer, 50, sock);
			uint32_t ui_blinkBrightness = atoi((char*)rx_buffer);
			xQueueSend(q_conf, &ui_blinkBrightness, 0);
			sendAck(sock);
			break;

		case CMD_bfrq: //sets frequency of blink (1-78125Hz)
			xQueueSend(q_cmdlet, &i_cmdlet, 0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			readTcpString((char*)rx_buffer, 50, sock);
			uint32_t ui_blinkFrequency = atoi((char*)rx_buffer);
			xQueueSend(q_conf, &ui_blinkFrequency, 0);
			sendAck(sock);
			break;

		case CMD_benb: //enables blink
			xQueueSend(q_cmdlet, &i_cmdlet, 0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			break;

		case CMD_bdis: //disables blink
			xQueueSend(q_cmdlet, &i_cmdlet, 0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			break;

		case CMD_zero: //sets the momentary time to zero
			ul_zeroTime = esp_timer_get_time();
			sendAck(sock);
			break;

		case CMD_bsht: //single shot blink without tcp message
			xQueueSend(q_cmdlet, &i_cmdlet, 0);
			xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
			sendAck(sock);
			break;

		default:
			send(sock, (const char*) "inv\n", strlen((const char*)"inv\n"), 0);
			break;
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
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

int readTcpCmdlet(int sock)
{
	int i_cmdlet = 0;
	uint8_t *data = (uint8_t *) malloc(6);
	recv(sock, data, 5, 0);
	vTaskDelay(1 / portTICK_PERIOD_MS);
	for(int i = 0; i<4; i++)
	{
		if ((data[i]== '\r') || (data[i] == '\n') || (data[i] == '\0'))
		{
			data[i] = 0;
		}
		i_cmdlet += (data[i] << (8*(3-i)));
	}
	return i_cmdlet;
}

void t_tcpIdle (void* param)
{
	vTaskDelete(NULL);
}

void sendAck (int sock)
{
	send(sock, (const char*) "ack\n", strlen((const char*)"ack\n"), 0);
}
