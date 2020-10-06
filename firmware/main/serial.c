/*
 * uart.c
 *
 *  Created on: Oct 5, 2020
 *      Author: lor
 */

#include "serial.h"


void serial_init()
{
	uart_config_t uart_config =
	{
			.baud_rate 	= 	115200,
			.data_bits 	= 	UART_DATA_8_BITS,
			.parity		= 	UART_PARITY_DISABLE,
			.stop_bits	=	UART_STOP_BITS_1,
			.flow_ctrl	=	UART_HW_FLOWCTRL_DISABLE
	};

	uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);
	uart_param_config(UART_NUM_0, &uart_config);

}

void serial_config(void *arg) //TODO:
{

	while(1) {
		sendRgbLedStatus(LEDC_STATUS_WAITING_SERIAL);
		printf("ack\n");
		printf("Entering configuration mode. Send man for manual\n");
		sendRgbLedStatus(LEDC_STATUS_WAITING_SERVER);
		int i_cmdlet = 0;
		char rx_buffer[128];
		int i_flag = 0;

		while (1){
			i_flag = readUartCmdlet(&i_cmdlet);

			switch(i_cmdlet)
			{
			case 0:
				vTaskDelay(10 / portTICK_PERIOD_MS);
				break;
			case CMD_man: //inquires manual
				break;

			case CMD_info: //inquires alle the information
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
				printf("ack\n");
				break;

				//Double Instruction CMDLETS
			case CMD_bbrt: //brightness of blink (0-1023)
			case CMD_bdur: //duration of blink
			case CMD_bper: //period of blink
			case CMD_bfrq: //sets frequency of blink (1-78125Hz)
			case CMD_mper: //sets the sampleperiod in us
			case CMD_vcal: //reads calibration value from server and writes it to adc
			case CMD_rcal: //calibrates with an known weight and sends the factor back to the server

			case CMD_ssid: //ssid of access point
			case CMD_pass: //pass for access point
			case CMD_ipco: //ip for configuration server
			case CMD_ipme: //ip for logging server
			case CMD_poco: //port for configuration server
			case CMD_pome: //port for logging server
			case CMD_potr: //port for UDP trigger

				if (i_flag == 0)
				{
					readUartString((char*)rx_buffer, 50);
					fConfig (i_cmdlet, rx_buffer);
				}
				else if (i_flag == 1)
				{
					readUartString((char*)rx_buffer, 50);
					fConfig (i_cmdlet, rx_buffer);
				}
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
		xTaskCreate(serial_config, "serial_config", 2048, NULL, 10, NULL);
		xEventGroupWaitBits(eg_uart,BIT_CONFIG_FINISHED,true,true,portMAX_DELAY);
	}
}




int readUartCmdlet(int *i_cmdlet)
{
	int i_flag = 0;
	*i_cmdlet = 0;
	uint8_t data = 0;
	int length = 0;
	uart_get_buffered_data_len(UART_NUM_0, (size_t*)&length);
	if (length == 0)
	{
		return i_flag;
	}

	for (int i = 0; i<4 ; i++)
	{
		uart_read_bytes(0, data, 1, 20 / portTICK_RATE_MS);
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
