/*
 * uart.c
 *
 *  Created on: Oct 5, 2020
 *      Author: lor
 */

#include "com_serial.h"


void fserialInit(void)
{
	uart_config_t uart_config =
	{
			.baud_rate 	= 	115200,
			.data_bits 	= 	UART_DATA_8_BITS,
			.parity		= 	UART_PARITY_DISABLE,
			.stop_bits	=	UART_STOP_BITS_1,
			.flow_ctrl	=	UART_HW_FLOWCTRL_DISABLE
	};

	uart_driver_install(0, 1024 * 2, 0, 0, NULL, 0);
	uart_param_config(0, &uart_config);
	xTaskCreate(tserialRun,	"serial_config",	4096, NULL, 10, NULL);
}

void tserialRun(void *arg)
{
	int i_cmdlet = 0;
	int i_flagConfig = 0;
	char rx_buffer[64];
	int i_flag = 0;
	while (1)
	{
		i_flag = readUartCmdlet(&i_cmdlet);
		ESP_LOGD(TAG_UART, "i_cmdlet:%d", i_cmdlet);

		switch (i_cmdlet)
		{
		case 0:
			break;
		case CMD_man: //inquires manual
			printf(MANUAL);
			break;
		case CMD_info: //inquires all the information
			//printf(INQUIRE_MESSAGE, gb_moduleID, FIRMWARE_VERSION, gul_adcPeriod);
			break;

		default:
			if (fcmdCheck(i_cmdlet))
			{
				if (i_flag == 0)
					printf(":ack:\n");
				readUartString((char*)rx_buffer, 50);
			}
			i_flagConfig = fConfig(i_cmdlet, rx_buffer);
			if (i_flagConfig)
			{
				ESP_LOGD(TAG_TCP, "CONFIG FLAG:%d\n", i_flagConfig);
				printf((char*)rx_buffer);
			}
			i_cmdlet = 0;
			break;
		}
	}
}

int readUartCmdlet(int *i_cmdlet)
{
	int i_flag = 0;
	*i_cmdlet = 0;
	uint8_t data = 0;
	int length = 0;
	while( length == 0)
	{
		uart_get_buffered_data_len(0, (size_t*)&length);
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}

	for (int i = 0; i<5 ; i++)
	{
		uart_read_bytes(0, &data, 1, portMAX_DELAY);
		if ((data == '\r') || (data == '\n') || (data == '\0'))
		{
			uart_flush(0);
			break;
		}
		if (data == ':')
		{
			i_flag = 1;
			break;
		}
		*i_cmdlet += (data << (8*(3-i)));
	}
	//ESP_LOGD(TAG_UART, "i_cmdlet:%d", *i_cmdlet);
	return i_flag;
}


void readUartString(char* out, int i_maxNumChars) {
	uint8_t *data = (uint8_t *) malloc(i_maxNumChars);
	while(1)
	{
		int i_len = uart_read_bytes(0, data, i_maxNumChars, 20 / portTICK_RATE_MS);
		for (int i = 0; i < i_len; i++)
		{
			if((data[i]== '\r') || (data[i] == '\n') || (data[i] == '\0'))
			{
				out[i] = '\0';
				break;
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
}
