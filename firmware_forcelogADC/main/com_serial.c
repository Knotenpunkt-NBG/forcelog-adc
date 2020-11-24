/*
 * uart.c
 *
 *  Created on: Oct 5, 2020
 *      Author: lor
 */

#include "com_serial.h"


void fserialInit	(void)
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
	xTaskCreate(tserialRun,	"serial_config",	4096, NULL, 8, &ht_serialRun);
}

//TODO: timeout causes guru meditation error
void tserialRun	(void* param)
{
	vTaskSuspend(NULL);
	ESP_LOGD(TAG_UART, "STARTING SERIAL");
	char* pc_configOut = NULL;
	char* pc_configIn = malloc(64);
	int i_flag = 0;
	while (1)
	{
		i_flag = freadUartString(pc_configIn, 64);
//		ESP_LOGD(TAG_UART, "i_flag:%d", i_flag);
		if (i_flag < 0 )
		{
			ESP_LOGD(TAG_UART, "SENDING ERROR MESSAGE\n");
			printf(MESS_INVALID);
		}
		else if (i_flag > 0)
		{
			xSemaphoreTake(hs_configCom, portMAX_DELAY);
			xQueueSend(q_pconfigIn,&pc_configIn, portMAX_DELAY);
			xQueueReceive(q_pconfigOut, &pc_configOut, portMAX_DELAY);
			if (pc_configOut == 0) //Config said it does not need more info
			{
				xQueueReceive(q_pconfigOut, &pc_configOut, portMAX_DELAY);
				printf("%s", pc_configOut);
				xEventGroupSync( eg_sync, BIT_CONFIG_FIN, BIT_STORAGE_FIN | BIT_COMM_FIN | BIT_CONFIG_FIN, portMAX_DELAY );
			}
			else //config requested more info
			{
				int i_result = 0;
				while (pc_configOut != 0)
				{
					//sending response from config
					printf("%s", pc_configOut);
					//checking uart for more strings with timeout
					for(int i = 0; (i < COM_CYCLES_TIMEOUT) && (i_result == 0); i++)
					{
						i_result = freadUartString(pc_configIn, 64);
						vTaskDelay(100 / portTICK_PERIOD_MS);
					}
					//success
					if(i_result > 0)
					{
						xQueueSend(q_pconfigIn,&pc_configIn, portMAX_DELAY);
						xQueueReceive(q_pconfigOut, &pc_configOut, portMAX_DELAY);
						if (pc_configOut == 0)
						{
							xQueueReceive(q_pconfigOut, &pc_configOut, portMAX_DELAY);
						}
						printf("%s", pc_configOut);
						pc_configOut = 0;
						xEventGroupSync( eg_sync, BIT_CONFIG_FIN, BIT_STORAGE_FIN | BIT_COMM_FIN | BIT_CONFIG_FIN, portMAX_DELAY );

					}
					//timeout
					else if (i_result == 0)
					{
						xQueueSend(q_pconfigIn,&i_result, portMAX_DELAY);
						pc_configOut = 0;
						printf("timeout ocurred\n");
					}
					//error
					else
					{
						printf(MESS_INVALID);
						pc_configOut = 0;
					}
				}
			}
			xSemaphoreGive(hs_configCom);
		}
		else
		{
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
	}
}

int freadUartString	(char* out,
					int i_maxNumChars)
{
	int i_flag = 0;
	uint8_t c_single = 0;
	for (int i = 0; i < i_maxNumChars; i++)
	{
		if(uart_read_bytes(0, &c_single, 1, 5 / portTICK_RATE_MS) > 0)
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
