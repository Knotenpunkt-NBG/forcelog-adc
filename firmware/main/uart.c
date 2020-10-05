/*
 * uart.c
 *
 *  Created on: Oct 5, 2020
 *      Author: lor
 */

#include "uart.h"


void serial_config(void *arg) //TODO:
{
	while(1) {
		sendRgbLedStatus(LEDC_STATUS_WAITING_SERIAL);
		printf("ack\n");
		printf("Entering configuration mode. Send man for manual\n");
		sendRgbLedStatus(LEDC_STATUS_WAITING_SERVER);

		while (1){
			uint32_t i_cmdlet = 0;
			while ((i_cmdlet = readUartCmdlet()) == 0);
			switch(i_cmdlet)
			{
			case ('m' << 8 * 3) + ('a' << 8 * 2) + ('n' << 8 * 1) + (0xFF << 8 * 0):
								printf("ack\n");
			printf("man\n");
			break;

			case ('s' << 8 * 3) + ('s' << 8 * 2) + ('i' << 8 * 1) + ('d' << 8 * 0):
			printf("ack\n");
			printf("Enter SSID:\n");
			readUartString((char *)sta_config.sta.ssid, 33);
			printf("ack\n");
			printf("SSID:%s\n", (char*) sta_config.sta.ssid);
			break;

			case ('p' << 8 * 3) + ('a' << 8 * 2) + ('s' << 8 * 1) + ('s' << 8 * 0):
			printf("ack\n");
			printf("Enter password:\n");
			readUartString((char *)sta_config.sta.password, 64);
			printf("ack\n");
			break;

			case ('s' << 8 * 3) + ('v' << 8 * 2) + ('i' << 8 * 1) + ('p' << 8 * 0):
			case ('i' << 8 * 3) + ('p' << 8 * 2) + (0xFF << 8 * 1) + (0xFF << 8 * 0):
			printf("ack\n");
			printf("Enter server IP:\n");
			char* s_ip[16] = {};
			readUartString((char *)s_ip, 16);
			inet_pton(AF_INET, (char*)s_ip, &str_serverAddressMes.sin_addr.s_addr);
			inet_pton(AF_INET, (char*)s_ip, &str_serverAddressConf.sin_addr.s_addr);
			printf("ack\n");
			break;

			case ('p' << 8 * 3) + ('c' << 8 * 2) + ('o' << 8 * 1) + ('n' << 8 * 0):
			printf("ack\n");
			printf("Enter server port for config:\n");
			char* s_portConf[16] = {};
			readUartString((char*)s_portConf, 16);
			str_serverAddressConf.sin_port = htons(atoi((char*)s_portConf));
			printf("ack\n");
			printf("Config Port:%d\n", str_serverAddressConf.sin_port);
			break;

			case ('p' << 8 * 3) + ('m' << 8 * 2) + ('e' << 8 * 1) + ('s' << 8 * 0):
			printf("ack\n");
			printf("Enter server port for measurements:\n");
			char* s_portMes[16] = {};
			readUartString((char*)s_portMes, 16);
			str_serverAddressMes.sin_port = htons(atoi((char*)s_portMes));
			printf("ack\n");
			printf("Measurements Port:%d\n", str_serverAddressMes.sin_port);
			break;

			case ('s' << 8 * 3) + ('a' << 8 * 2) + ('v' << 8 * 1) + ('e' << 8 * 0):
			case ('s' << 8 * 3) + ('a' << 8 * 2) + ('v' << 8 * 1) + (0xFF << 8 * 0):
				printf("ack\n");
				printf("Saving network config to non volative memory...\n");
				str_serverAddressConf.sin_port = 37648;
				printf("ConfPort:%d\n", str_serverAddressConf.sin_port);
				nvs_open("storage", NVS_READWRITE, &handle_nvs);
				nvs_set_u16(handle_nvs, "k_serverPortMes", str_serverAddressMes.sin_port);
				nvs_set_u16(handle_nvs, "k_srvPortConf", str_serverAddressConf.sin_port);
				nvs_set_u32(handle_nvs, "k_serverIp", str_serverAddressMes.sin_addr.s_addr);
				nvs_set_u32(handle_nvs, "k_serverIp", str_serverAddressConf.sin_addr.s_addr);
				nvs_set_str(handle_nvs, "k_ssid", (char*) sta_config.sta.ssid);
				nvs_set_str(handle_nvs, "k_pass", (char*) sta_config.sta.password);
				nvs_commit(handle_nvs);
				nvs_close(handle_nvs);
				printf("ack\n");
				printf("Saved. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
				printf("Saved Mes. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
				printf("Saved Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);
			break;

			case ('l' << 8 * 3) + ('o' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0):
				nvs_open("storage", NVS_READWRITE, &handle_nvs);
				nvs_get_u16(handle_nvs, "k_serverPortMes", &str_serverAddressMes.sin_port);
				nvs_get_u16(handle_nvs, "k_srvPortConf", &str_serverAddressConf.sin_port);
				nvs_get_u32(handle_nvs, "k_serverIp", &str_serverAddressMes.sin_addr.s_addr);
				nvs_get_u32(handle_nvs, "k_serverIp", &str_serverAddressConf.sin_addr.s_addr);
				size_t required_size = 0;
				nvs_get_str(handle_nvs, "k_ssid", NULL, &required_size);
				nvs_get_str(handle_nvs, "k_ssid", (char*) sta_config.sta.ssid, &required_size);
				nvs_get_str(handle_nvs, "k_pass", NULL, &required_size);
				nvs_get_str(handle_nvs, "k_pass", (char*) sta_config.sta.password, &required_size);
				nvs_close(handle_nvs);
				printf("Load. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
				printf("Load Mes. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
				printf("Load Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);
			break;

			case ('s' << 8 * 3) + ('t' << 8 * 2) + ('r' << 8 * 1) + ('t' << 8 * 0):
				printf("ack\n");
				xTaskCreate(connectWifi, "connectWifi", 4096, NULL, 10, NULL);
				xEventGroupSetBits(eg_uart, BIT_CONFIG_FINISHED);
				vTaskDelete(NULL);
			break;

			default:
				printf("inv\n");
				break;
			}
		}
	}
}

void serial_idle(void *arg)	//TODO: add automatic UART Check
{
	/*printf("conf\n");
	vTaskDelay(100 / portTICK_PERIOD_MS);
	if (readUartCmdlet() == ('a' << 8 * 3) + ('c' << 8 * 2) + ('k' << 8 * 1) + (0xFF << 8 * 0)) //code:ack
	{
		xTaskCreate(serial_config, "serial_config", 2048, NULL, 10, NULL);
		xEventGroupWaitBits(eg_uart,BIT_CONFIG_FINISHED,true,true,portMAX_DELAY);
	}*/
	while(1)
	{
		while (
				(readUartCmdlet() != (('c' << 8 * 3) + ('o' << 8 * 2) + ('s' << 8 * 1) + ('e' << 8 * 0))) //code: cose
		)
		{
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
		xTaskCreate(serial_config, "serial_config", 2048, NULL, 10, NULL);
		xEventGroupWaitBits(eg_uart,BIT_CONFIG_FINISHED,true,true,portMAX_DELAY);
	}
}


char* readUartString(char* out, int i_maxNumChars) {
	uint8_t *data = (uint8_t *) malloc(i_maxNumChars);
	while(1)
	{
		int i_len = uart_read_bytes(UART_NUM_0, data, i_maxNumChars, 20 / portTICK_RATE_MS);
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
	free(data);
	return out;
}

int readUartCmdlet()
{
	int i_cmdlet = 0;
	uint8_t *data = (uint8_t *) malloc(6);
	while (uart_read_bytes(UART_NUM_0, data, 6, 20 / portTICK_RATE_MS) == 0)
		vTaskDelay(1 / portTICK_PERIOD_MS);
	for(int i = 0; i<4; i++)
	{
		i_cmdlet += (data[i] << (8*(3-i)));
	}
	free(data);
	return i_cmdlet;
}
