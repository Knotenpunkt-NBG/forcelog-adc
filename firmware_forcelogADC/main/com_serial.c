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

void tserialRun	(void* param)
{
	char* pc_configIn = malloc(128);
	int i_flag = 0;
	stu_configMessage stu_response;
	while (1)
	{
		i_flag = freadUartString(pc_configIn, 128);
//		ESP_LOGD(TAG_UART, "i_flag:%d", i_flag);
		if (i_flag < 0 )
		{
			ESP_LOGD(TAG_UART, "SENDING ERROR MESSAGE\n");
			printf(MESS_INVALID);
		}
		else if (i_flag > 0)
		{
			i_flag = 0;
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
				printf("%s", stu_response.pc_response);
				free(stu_response.pc_response);
				stu_response.pc_response = 0;

				while (stu_response.uc_numResponses > 0) //check if task expects response
				{
					while(i_flag <= 0)
					{
						i_flag = freadUartString(pc_configIn, 128);
						//		ESP_LOGD(TAG_UART, "i_flag:%d", i_flag);
						if (i_flag < 0 )
						{
							ESP_LOGD(TAG_UART, "SENDING ERROR MESSAGE\n");
							printf(MESS_INVALID);
						}
						else if (i_flag == 0)
						{
							vTaskDelay(100 / portTICK_PERIOD_MS);
						}
						else
						{
							xQueueSend(q_pconfigIn,
									&pc_configIn,
									portMAX_DELAY);
							stu_response.uc_numResponses--;
						}
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
