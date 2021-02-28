/*
 * config.c
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#include "config.h"

void fconfigInit(void)
{
	xTaskCreate(tconfigRun,	"tconfigRun",	4096, NULL, 10, &ht_configRun);
}

void tconfigRun	(void* param)
{
	ESP_LOGD(TAG_CONF, "STARTING CONFIG");
	//INITIALISING VARIABLES
	uint32_t ui_cmdlet = 0;
	char* pc_value = NULL;
	char* pc_configIn = NULL;
	char* pc_configOut = NULL;
	finitAll();


	//MAIN TASK LOOP
	while(1)
	{
		xQueueReceive(q_pconfigIn, &pc_configIn, portMAX_DELAY);
		for (int i = 0; i<4 ; i++)
		{
			if ((pc_configIn[i] == '\r') || (pc_configIn[i] == '\n') || (pc_configIn[i] == '\0') || (pc_configIn[i] == ':'))
			{
				break;
			}
			ui_cmdlet += ((uint32_t)pc_configIn[i] << (8*(3-i)));
		}
		ESP_LOGD(TAG_CONF, "CMD RECEIVED");
		switch (ui_cmdlet)
		{
			//INFORMATION
		case CMD_man: //sends back the manual
			break;
		case CMD_info: //sends back the momentary configuration
			break;

			//CONTROL
		case CMD_peek: //PEEK: starts the adc measurements as soon as socket mes is connected without waiting for an trigger (udp or pin)
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot peek while recording or peeking.\n");
			}
			else
			{
				pc_configOut = malloc(64);
				while(xTaskNotify(ht_adcRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
				while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );

				while(xTaskNotify(ht_adcRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_blinkRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xTimerStart(htim_tcpSend, portMAX_DELAY);
				sprintf(pc_configOut,
						"|peek|1|\t\tOK, started peeking|\n");
				xEventGroupClearBits(eg_status, BITS_STATI);
				xEventGroupSetBits(eg_status, STATUS_PEEKING);
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}
			break;
		case CMD_strt: //STaRT: starts without waiting for trigger
		{
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_PEEKING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot start while recording or peeking.\n");
			}
			else
			{
				pc_configOut = malloc(512);
				sprintf(pc_configOut,
						MEASUREMENT_MESSAGE,
						gstu_config->adc.ul_adcPeriod,
						gstu_config->cell.d_calValue, gstu_config->cell.ui_tareValue,
						gstu_config->blink.ui_blinkPeriod, gstu_config->blink.ui_blinkDuration);
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				while(xTaskNotify(ht_tcpMes,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xQueueSend(q_send, &pc_configOut, portMAX_DELAY);
				xEventGroupSync(eg_sync,
						BIT_CONFIG_SYNC,
						BIT_CONFIG_SYNC | BIT_TCPMES_SYNC,
						portMAX_DELAY);
				xSemaphoreGive(hs_pointerQueue);

				while(xTaskNotify(ht_adcRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync(eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
				while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
//				while(xTaskNotify(ht_tempIntRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
//					vTaskDelay(1/ portTICK_PERIOD_MS);

//				while(xTaskNotify(ht_storageRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
//					vTaskDelay(1/ portTICK_PERIOD_MS);

				while(xTaskNotify(ht_adcRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_blinkRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
//				while(xTaskNotify(ht_tempIntRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
//					vTaskDelay(1/ portTICK_PERIOD_MS);
//				while(xTaskNotify(ht_storageRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
//					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupClearBits(eg_status, BITS_STATI);
				xEventGroupSetBits(eg_status, STATUS_RECORDING);
				xTimerStart(htim_tcpSend, portMAX_DELAY);
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}

		}
		break;
		case CMD_stop: //STOP: stops the sampling
		{
			if (xEventGroupGetBits(eg_status) & STATUS_IDLE)
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot stop while in idle.\n");
			}
			else
			{
				pc_configOut = malloc(64);
				xEventGroupClearBits(eg_status, BITS_STATI);
				xEventGroupSetBits(eg_status, STATUS_IDLE);
				while(xTaskNotify(ht_adcRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_blinkRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				//				while(xTaskNotify(ht_tempIntRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
				//					vTaskDelay(1/ portTICK_PERIOD_MS);
				//				while(xTaskNotify(ht_storageRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
				//					vTaskDelay(1/ portTICK_PERIOD_MS);
				//				while(xTaskNotify(ht_tcpMes,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
				//					vTaskDelay(1/ portTICK_PERIOD_MS);
				sprintf(pc_configOut,
						"|stop|1|\t\tOK, stopped recording|\n");
				xEventGroupClearBits(eg_status, BITS_STATI);
				xEventGroupSetBits(eg_status, STATUS_IDLE);
				xTimerStop(htim_tcpSend, portMAX_DELAY);
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}

		}
		break;
		case CMD_wait: //WAIT: starts the adc in an fast to trigger status until trigger (udp or pin) is received
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot wait while recording or waiting.\n");
			}
			else
			{
				pc_configOut = malloc(64);
				while(xTaskNotify(ht_tcpMes,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_adcRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_tempIntRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_storageRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_STORAGE_SYNC,
						portMAX_DELAY );
				xTaskCreate (t_udpWait,
						"t_udpWait",
						2048,
						NULL,
						10,
						NULL);
				sprintf(pc_configOut,
						"|wait|1|\t\tOK, waiting for trigger|\n");
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}

			break;

			//ADC CONFIG
		case CMD_cali: //CALIbration: calibrates the ADC with a known weight. uses tare value as zero reference
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate value while recording or waiting.\n");
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter calibration weight (xxx).\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					double* pd  = malloc(sizeof(double));
					*pd = strtod(pc_value, NULL);
					if(*pd !=0)
					{
						xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
						xQueueSend(q_send,&pd, portMAX_DELAY);
						while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
							vTaskDelay(1/ portTICK_PERIOD_MS);
						xEventGroupSync( eg_sync,
								BIT_CONFIG_SYNC,
								BIT_ADC_SYNC | BIT_CONFIG_SYNC,
								portMAX_DELAY );
						sprintf(pc_configOut,
								"|cali|%.*f|\t\tOK, calibration finished|\n",
								20, gstu_config->cell.d_calValue);
						xSemaphoreGive(hs_pointerQueue);
						xEventGroupSync( eg_sync,
								BIT_CONFIG_SYNC,
								BIT_ADC_SYNC | BIT_CONFIG_SYNC,
								portMAX_DELAY );
						freturnMessage(pc_configOut);
						free(pc_configOut);
					}
				}
			}
			break;

		case CMD_tare: //TARE: sets the momentary value to zero
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot tare while recording or waiting.\n");
			}
			else
			{
				pc_configOut = malloc(64);
				while(xTaskNotify(ht_adcRun,CMD_tare,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
				xEventGroupSync( eg_config,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
				sprintf(pc_configOut, "|tare|1|\t\tOK, tare successful|\n");
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}

			break;

		//TODO: implement response for wrong period
		case CMD_mper: //Measurement PERiod: sets the sampleperiod in us
			if (xEventGroupGetBits(eg_status) & STATUS_RECORDING)
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set measurement period while recording or waiting.\n");
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter measurement period in ms.\n");

				if(pc_value != 0)
				{
					uint64_t ul_mesPeriod = strtoll((char*)pc_value, NULL, 10);
					pc_configOut = malloc(64);
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					gstu_config->adc.ul_adcPeriod = ul_mesPeriod;
					portEXIT_CRITICAL(&mux);
					while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
					sprintf(pc_configOut,
							"|mper|%llu|\t\tOK, Measurement period set to %lluus.|\n\4",
							gstu_config->adc.ul_adcPeriod, gstu_config->adc.ul_adcPeriod);
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}

			break;

		case CMD_zero: //ZERO: sets the momentary time to zero
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot zero time while recording or waiting.\n");
			}
			else
			{
				pc_configOut = malloc(64);
				while(xTaskNotify(ht_tcpMes,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				sprintf(pc_configOut, "|zero|1|\t\tOK, starting time set to 0|\n");
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}

			break;

		case CMD_vcal: //CALibration Value: reads calibration value from server and writes it to adc
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate batmon while recording or waiting.\n\4");
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter Calibration Value.\n");
				if(pc_value !=0)
				{
					pc_configOut = malloc(64);
					double d_vcal = strtod(pc_value, NULL);
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					gstu_config->cell.d_calValue = d_vcal;
					portEXIT_CRITICAL(&mux);
					sprintf(pc_configOut,
							"\1|\2vcal\3|\2%f\3|\t\tCalibration value set to %f\n\4",
							gstu_config->cell.d_calValue, gstu_config->cell.d_calValue);
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;


			//BLINK
		case CMD_bper: //Blink PERiod: period of blink
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set blink period while recording or waiting.\n");
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink period in ms.\n");
				if(pc_value != 0)
				{
					uint32_t ui_blinkPeriod = 0;
					pc_configOut = malloc(64);
					ui_blinkPeriod = strtol(pc_value, NULL, 10);
					if(ui_blinkPeriod !=0)
					{
						if(ui_blinkPeriod > gstu_config->blink.ui_blinkDuration)
						{
							portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
							portENTER_CRITICAL(&mux);
							gstu_config->blink.ui_blinkPeriod = ui_blinkPeriod;
							portEXIT_CRITICAL(&mux);

							sprintf(pc_configOut,
									"|bdur|%d|\t\tOK, blink period set to %d ms.\n",
									ui_blinkPeriod, ui_blinkPeriod);
						}
						else
						{
							sprintf(pc_configOut,
									"|err|"STR(ERR_RECORDING)"|\t\tPeriod(%d) must be higher then duration(%d).\n"
									, ui_blinkPeriod, gstu_config->blink.ui_blinkDuration);
						}
					}
					else
					{
						sprintf(pc_configOut,
								"|err|"STR(ERR_RECORDING)"|\t\tcould not find value in message(%s).\n",
								pc_value);
					}

					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;

		case CMD_bdur: //Blink DURation: duration of blink
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set blink duration while recording or waiting.\n");
			else{
				uint32_t ui_blinkDuration = 0;
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					ui_blinkDuration = strtol(pc_value, NULL, 10);
					if(ui_blinkDuration !=0)
					{
						if(ui_blinkDuration < gstu_config->blink.ui_blinkPeriod)
						{
							portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
							portENTER_CRITICAL(&mux);
							gstu_config->blink.ui_blinkDuration = ui_blinkDuration;
							portEXIT_CRITICAL(&mux);

							sprintf(pc_configOut, "|bdur|%d|\t\tBlink duration set to %d ms.\n", ui_blinkDuration, ui_blinkDuration);
						}
						else
							sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tDuration(%d) must be lower then Period(%d).\n"
									, ui_blinkDuration, gstu_config->blink.ui_blinkPeriod);
					}
					else
						sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCould not find value in message(%s).\n", pc_value);

					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;

		case CMD_bbrt: //Blink BRighTness: brightness of blink (0-1023)
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set blink brightness while recording or waiting.\n");
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					uint32_t ui_blinkBrightness = 0;
					pc_configOut = malloc(64);
					ui_blinkBrightness = strtol(pc_value, NULL, 10);
					if(ui_blinkBrightness !=0)
					{
						if((0 < ui_blinkBrightness) && (ui_blinkBrightness < 1024))
						{
							portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
							portENTER_CRITICAL(&mux);
							gstu_config->blink.ui_blinkBrightness = ui_blinkBrightness;
							portEXIT_CRITICAL(&mux);
							sprintf(pc_configOut,
									"|bdur|%d|\t\tBrightness set to %d ms.\n",
									ui_blinkBrightness, ui_blinkBrightness);
						}
						else
							sprintf(pc_configOut,
									"|err|"STR(ERR_RECORDING)"|\t\tBrightness(%d) must be between 1 and 1023.\n",
									ui_blinkBrightness);
					}
					else
						sprintf(pc_configOut,
								"|err|"STR(ERR_RECORDING)"|\t\tCould not find value in message(%s).\n",
								pc_value);

					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;

		case CMD_bfrq: //Blink FReQuency: sets frequency of blink (1-78125Hz)
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set blink frequency while recording or waiting.\n");
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tEnter blink frequency in ms:\n");
				if(pc_value != 0)
				{
					uint32_t ui_blinkFrequency = 0;
					pc_configOut = malloc(64);
					ui_blinkFrequency = strtol(pc_value, NULL, 10);
					if(ui_blinkFrequency !=0)
					{
						if((1 < ui_blinkFrequency) && (ui_blinkFrequency < 78125))
						{
							portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
							portENTER_CRITICAL(&mux);
							gstu_config->blink.ui_blinkFrequency = ui_blinkFrequency;
							portEXIT_CRITICAL(&mux);
							sprintf(pc_configOut,
									"|bfrq|%d|\t\tFrequency set to %d Hz.\n",
									ui_blinkFrequency, ui_blinkFrequency);
						}
						else
							sprintf(pc_configOut,
									"|err|"STR(ERR_RECORDING)"|\t\tFrequency(%d) must be between 1 and 78125hz.\n"
									, ui_blinkFrequency);
					}
					else
						sprintf(pc_configOut,
								"|err|"STR(ERR_RECORDING)"|\t\tcould not find value in message(%s)|\n",
								pc_value);

					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}

			break;

		case CMD_benb: //Blink ENaBle: enables blink
			while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_PEEKING))
			{
				while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
				while(xTaskNotify(ht_blinkRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
			}
			freturnMessage("|benb|1|\t\tOK, blink enabled\n");
			break;

		case CMD_bdis: //Blink DISable: disables blink
			while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_PEEKING))
			{
				while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
				while(xTaskNotify(ht_blinkRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
			}
			freturnMessage("|bdis|0|\t\tOK, blink disabled|\n");
			break;

		case CMD_blnk: //BLiNK: single shot blink without tcp message
			while(xTaskNotify(ht_blinkRun,CMD_blnk,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			freturnMessage("|blnk|1|\t\tOK, blinking\n");
		break;


			//STORAGE
		case CMD_mkfs: //formats SD card
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot format SD card while recording or waiting.\n");
			else
			{
				pc_configOut = malloc(64);
				while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC ,
						BIT_STORAGE_SYNC |  BIT_CONFIG_SYNC,
						portMAX_DELAY );
				sprintf(pc_configOut, "|mkfs|1|\t\tSD card has been formatted.|\n");
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}
			break;

		case CMD_init:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot load config while recording or waiting.\n");
			}
			else{
				finitAll();
				freturnMessage("INITIATED\4");
			}
			break;

		case CMD_wrin: //write default init

			break;

		case CMD_list: //lists all the present config files
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot list files while recording or waiting.\n");
			else
			{
				{
					uint32_t ui_offset = 0;
					pc_configOut = malloc(1400);
					xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
					while(xTaskNotify(ht_storageRun,CMD_lswi,eSetValueWithoutOverwrite) != pdPASS)
						taskYIELD();
					xQueueReceive(q_send,&pc_value, portMAX_DELAY);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_STORAGE_SYNC | BIT_CONFIG_SYNC ,
							portMAX_DELAY );
					ui_offset += sprintf(pc_configOut + ui_offset, "%s", pc_value);
					free(pc_value);

					while(xTaskNotify(ht_storageRun,CMD_lsad,eSetValueWithoutOverwrite) != pdPASS)
						taskYIELD();
					xQueueReceive(q_send,&pc_value, portMAX_DELAY);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_STORAGE_SYNC | BIT_CONFIG_SYNC ,
							portMAX_DELAY );
					ui_offset += sprintf(pc_configOut + ui_offset, "%s", pc_value);
					free(pc_value);

					while(xTaskNotify(ht_storageRun,CMD_lsbl,eSetValueWithoutOverwrite) != pdPASS)
						taskYIELD();
					xQueueReceive(q_send,&pc_value, portMAX_DELAY);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_STORAGE_SYNC | BIT_CONFIG_SYNC ,
							portMAX_DELAY );
					ui_offset += sprintf(pc_configOut + ui_offset, "%s", pc_value);
					free(pc_value);

					while(xTaskNotify(ht_storageRun,CMD_lslc,eSetValueWithoutOverwrite) != pdPASS)
						taskYIELD();
					xQueueReceive(q_send,&pc_value, portMAX_DELAY);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_STORAGE_SYNC | BIT_CONFIG_SYNC ,
							portMAX_DELAY );
					ui_offset += sprintf(pc_configOut + ui_offset, "%s", pc_value);
					free(pc_value);

					xSemaphoreGive(hs_pointerQueue);
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;

		case CMD_rmbl:
		case CMD_rmad:
		case CMD_rmst:
		case CMD_rmlc:
		case CMD_ldad:
		case CMD_ldwi:
		case CMD_ldtc:
		case CMD_ldtm:
		case CMD_ldbl: //loads blink config from storage
		case CMD_ldlc:
		case CMD_svad:
		case CMD_svtc:
		case CMD_svtm:
		case CMD_svbl:
		case CMD_svwi:
		case CMD_svst:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot save config while recording or waiting.\n");
			}
			else{
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tInsert filename:\n");
				if(pc_value != 0)
				{
					xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
					while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
						taskYIELD();
					xQueueSend(q_send,&pc_value, portMAX_DELAY);
					xQueueReceive(q_recv, &pc_configOut, portMAX_DELAY);
					xSemaphoreGive(hs_pointerQueue);
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;

		case CMD_svlc:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot save loadcell while recording or waiting.\n");
			}
			else
			{
				owb_string_from_rom_code(gstu_config->temp.romExt,gstu_config->cell.ac_name,OWB_ROM_CODE_STRING_LENGTH);
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
				xQueueReceive(q_recv, &pc_configOut, portMAX_DELAY);
				xSemaphoreGive(hs_pointerQueue);
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}
			break;

		case CMD_cllc: //CLear LoadCell: deletes the loadcell from storage
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate value while recording or waiting.\n");
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter loadcell ID.\n");
					if(pc_value !=0)
					{
						xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
						xQueueSend(q_send,&pc_value, portMAX_DELAY);
						while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
							vTaskDelay(1/ portTICK_PERIOD_MS);
						xEventGroupSync( eg_sync,
								BIT_CONFIG_SYNC,
								BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
								portMAX_DELAY );
						xQueueReceive(q_send, &pc_configOut, portMAX_DELAY);
						ESP_LOGD(TAG_CONF, "RESPONSE GOTTEN");
						xSemaphoreGive(hs_pointerQueue);
						freturnMessage(pc_configOut);
						ESP_LOGD(TAG_CONF, "MESSAGE RETURNED");
						free(pc_configOut);
						ESP_LOGD(TAG_CONF, "FINISHED CLEAR");
					}
			}
			break;

		case CMD_inad:
		case CMD_intc:
		case CMD_intm:
		case CMD_inbl:
		case CMD_inwi:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set init while recording or waiting.\n");
			else{
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
				xQueueReceive(q_send, &pc_configOut, portMAX_DELAY);
				freturnMessage(pc_configOut);
				xSemaphoreGive(hs_pointerQueue);
			}
			break;
		

			//LOGGING
		case CMD_loge: //Sets logging to error (highest)
			fsetLogLevel(LOG_ERROR);
			freturnMessage("|loge|1|\t\tOK, logging set to ERROR|\n");
			break;
		case CMD_logw: //Sets logging to warning
			fsetLogLevel(LOG_WARNING);
			freturnMessage("|loge|1|\t\tOK, logging set to WARNING|\n");
			break;
		case CMD_logi: //Sets logging to info
			fsetLogLevel(LOG_INFO);
			freturnMessage("|loge|1|\t\tOK, logging set to INFO|\n");
			break;
		case CMD_logd: //Sets logging to debug
			fsetLogLevel(LOG_DEBUG);
			freturnMessage("|loge|1|\t\tOK, logging set to DEBUG|\n");
			break;
		case CMD_logv: //Sets logging to verbose (lowest)
			fsetLogLevel(LOG_VERBOSE);
			freturnMessage("|loge|1|\t\tOK, logging set to VERBOSE|\n");
			break;


			//WIFI
		case CMD_ssid: //ssid of access point

			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set SSID while recording or waiting.\n");
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter SSID:.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					strcpy((char*)&(gstu_config->staConfig.ac_ssid), (char*) pc_value);
					portEXIT_CRITICAL(&mux);
					sprintf(pc_configOut, "|ssid|%s|\t\tSSID set to %s.\n", pc_value, pc_value);
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}

			break;

		case CMD_pass: //pass for access point
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set password while recording or waiting.\n");
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter pass:\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					strcpy((char*)&(gstu_config->staConfig.ac_pass), (char*)pc_value);
					portEXIT_CRITICAL(&mux);
					sprintf(pc_configOut, "|ssid|%s|\t\tPassword set to %s.\n", pc_value, pc_value);
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}

			break;

		case CMD_scwi:
			xEventGroupSync( eg_config,
					BIT_WIFI_SYNC,
					BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
					portMAX_DELAY );
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_send, &pc_configOut, portMAX_DELAY);
			wifi_ap_record_t* ap_info = 0;
			uint32_t ap_count = 0;
			xQueueSend(q_send,&ap_count, portMAX_DELAY);
			xQueueSend(q_send,&ap_info, portMAX_DELAY);
			xEventGroupSync( eg_config,
							BIT_WIFI_SYNC,
							BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );
			free(ap_info);
			break;

		case CMD_stto:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set Station Timeout.\n");
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter station Timeout in s.\n");
				if(pc_value != 0)
				{
					uint32_t ui_stationTimeout = 0;
					pc_configOut = malloc(64);
					ui_stationTimeout = strtol(pc_value, NULL, 10);
					if(ui_stationTimeout !=0)
					{
						portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
						portENTER_CRITICAL(&mux);
						gstu_config->staConfig.ui_wifiTimeout = ui_stationTimeout * 1000;
						portEXIT_CRITICAL(&mux);
							sprintf(pc_configOut,
									"|stto|%d|\t\tOK, Station Timeout set to %d s.\n",
									ui_stationTimeout, ui_stationTimeout);
					}
					else
					{
						sprintf(pc_configOut,
								"|err|"STR(ERR_RECORDING)"|\t\tcould not find value in message(%s).\n",
								pc_value);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;


			//SOCKETS
		case CMD_hspc: //port for configuration server
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set port while recording or waiting.\n");
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					uint32_t ui_port = (uint32_t)strtol(pc_value, NULL, 10);
					if(ui_port <= 65353)
					{
						gstu_config->apConfig.portConf = ui_port;
						sprintf(pc_configOut,
								"|ipco|%d|\t\tOK, port of config server set to %d|\n",
								ui_port, ui_port);
						freturnMessage(pc_configOut);
						xEventGroupSync( eg_config,
								BIT_CONFIG_SYNC,
								BIT_TCPCONF_SYNC | BIT_CONFIG_SYNC,
								portMAX_DELAY );
						xSemaphoreGive(hs_pointerQueue);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}

			}
			break;

		case CMD_hspm: //port for logging server
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set port while recording or waiting.\n");
			}
			else
			{

				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					uint32_t ui_port = (uint32_t)strtol(pc_value, NULL, 10);
					if(ui_port <= 65353)
					{
						xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
						gstu_config->apConfig.portMes = ui_port;
						sprintf(pc_configOut,
								"|ipco|%d|\t\tOK, port of measurement server set to %d|\n",
								ui_port, ui_port);
						xSemaphoreGive(hs_pointerQueue);
					}
					else
					{
						sprintf(pc_configOut,
								"|ipco|%d|\t\tERROR, port of measurement server out of range %d|\n",
								ui_port, ui_port);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;

		case CMD_hspt: //port for UDP trigger
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot set port while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				uint32_t ui_port = 0;
				if (fgetNextString(pc_configIn))
				{
					ui_port = atoi(pc_configIn + fgetNextString(pc_configIn));
				}
				else
				{
					int i = 0;
					xQueueSend(q_pconfigOut,&i, portMAX_DELAY);
					xQueueReceive(q_pconfigIn, &pc_configIn, portMAX_DELAY);
					ui_port = atoi(pc_configIn);
				}
				//				triggergstu_config->trigSocket.sin_port = ui_port;
				sprintf(pc_configOut,
						"|ipco|%d|\t\tOK, IP of measurement server set to %d|\n",
						ui_port, ntohs(ui_port));
				xQueueSend(q_pconfigOut,&pc_configOut, portMAX_DELAY);
			}
			break;

		case CMD_iptr: //ip for trigger
			//			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			//			{
			//				sprintf(pc_configOut, "|err|%d|\t\tcannot set IP while recording or waiting|\n", ERR_RECORDING);
			//			}
			//			else
			//			{
			//				uint32_t ui_ip = 0;
			//				if (fgetNextString(pc_configIn))
			//				{
			//					inet_pton(AF_INET,pc_configIn + fgetNextString(pc_configIn), &ui_ip);
			//				}
			//				else
			//				{
			//					int i = 0;
			//					xQueueSend(q_pconfigOut,&i, portMAX_DELAY);
			//					xQueueReceive(q_pconfigIn, &pc_configIn, portMAX_DELAY);
			//					inet_pton(AF_INET,pc_configIn + fgetNextString(pc_configIn), &ui_ip);
			//				}
			//				triggergstu_config->trigSocket.sin_addr.s_addr = ui_ip;
			//				char ipConf[16];
			//				inet_ntop(AF_INET, &ui_ip, ipConf, sizeof(ipConf));
			//				sprintf(pc_configOut, "|ipco|%d|\t\tOK, IP of measurement server set to %s|\n", ui_ip, ipConf);
			//				xQueueSend(q_pconfigOut,&pc_configOut, portMAX_DELAY);
			//			}
			break;

			//PORTS FOR STATION
		case CMD_stpc: //port for configuration server
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set port while recording or waiting.\n");
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					uint32_t ui_port = (uint32_t)strtol(pc_value, NULL, 10);
					if(ui_port <= 65353)
					{
						portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
						portENTER_CRITICAL(&mux);
						gstu_config->staConfig.portConf = ui_port;
						portEXIT_CRITICAL(&mux);

						sprintf(pc_configOut,
								"|stpc|%d|\t\tOK, port of config server set to %d|\n",
								ui_port, ui_port);
						freturnMessage(pc_configOut);
						xEventGroupSync( eg_config,
								BIT_CONFIG_SYNC,
								BIT_TCPCONF_SYNC | BIT_CONFIG_SYNC,
								portMAX_DELAY );
						xSemaphoreGive(hs_pointerQueue);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}

			}
			break;

		case CMD_stpm: //port for logging server
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set port while recording or waiting.\n");
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					uint32_t ui_port = (uint32_t)strtol(pc_value, NULL, 10);
					if(ui_port <= 65353)
					{
						portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
						portENTER_CRITICAL(&mux);
						gstu_config->staConfig.portMes = ui_port;
						portEXIT_CRITICAL(&mux);

						sprintf(pc_configOut,
								"|ipco|%d|\t\tOK, port of measurement server set to %d|\n",
								ui_port, ui_port);
					}
					else
					{
						sprintf(pc_configOut,
								"|ipco|%d|\t\tERROR, port of measurement server out of range %d|\n",
								ui_port, ui_port);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;

		case CMD_stpt: //port for UDP trigger
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot set port while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				uint32_t ui_port = 0;
				if (fgetNextString(pc_configIn))
				{
					ui_port = atoi(pc_configIn + fgetNextString(pc_configIn));
				}
				else
				{
					int i = 0;
					xQueueSend(q_pconfigOut,&i, portMAX_DELAY);
					xQueueReceive(q_pconfigIn, &pc_configIn, portMAX_DELAY);
					ui_port = atoi(pc_configIn);
				}
				//				triggergstu_config->trigSocket.sin_port = ui_port;
				sprintf(pc_configOut,
						"|ipco|%d|\t\tOK, IP of measurement server set to %d|\n",
						ui_port, ntohs(ui_port));
				xQueueSend(q_pconfigOut,&pc_configOut, portMAX_DELAY);
			}
			break;


		case CMD_conn: //connects to the access point
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				sprintf(pc_configOut,
						"|err|%d|\t\tERROR cannot reconnect while recording!|\n",
						ERR_RECORDING);
			}
			else
			{
				while(xTaskNotify(ht_wifiRun,CMD_conn,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_CONFIG_SYNC | BIT_WIFI_SYNC,
						portMAX_DELAY );
				freturnMessage("|conn|1|\t\tstarting connection\n");
			}
			break;


			//BATTERY MONITOR
		case CMD_bath: //BATtery High: sets the high value of the battery monitor
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate batmon while recording or waiting.\n\4");
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter voltage of battery.\n");
				if(pc_value !=0)
				{
					pc_configOut = malloc(64);
					while(xTaskNotify(ht_batmonRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );
					gstu_config->batMon.f_batVolHigh = (float)strtod(pc_value, NULL);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );

					while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );
					sprintf(pc_configOut,
							"|batl|%d|\t\tHigh Battery Level set to %f\n\4",
							gstu_config->batMon.i_batRawHigh, gstu_config->batMon.f_batVolHigh);
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;

		case CMD_batl: //BATtery Low: sets the low value of the battery monitor
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate batmon while recording or waiting.\n\4");
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter voltage of battery.\n");
				if(pc_value !=0)
				{
					pc_configOut = malloc(64);
					while(xTaskNotify(ht_batmonRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );
					gstu_config->batMon.f_batVolLow = (float)strtod(pc_value, NULL);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );

					while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );
					sprintf(pc_configOut,
							"|batl|%d|\t\tLow Battery Level set to %f\n\4",
							gstu_config->batMon.i_batRawLow, gstu_config->batMon.f_batVolLow);
					freturnMessage(pc_configOut);
					free(pc_configOut);
				}
			}
			break;


		default:
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|err|1|\t\tCOMMAND NOT FOUND\n");
			freturnMessage(pc_configOut);
			free(pc_configOut);
			break;
		}
		ESP_LOGD(TAG_CONF, "FINISHED SWITCH");
		ui_cmdlet = 0;
	}
}


uint32_t fgetNextString	(char* string)
{
	uint32_t i;
	for (i = 0; i < strlen(string); i++)
	{
		if (string[i] == ':')
		{
			return i+1;
		}
	}
	return 0;
}


void freturnMessage	(const char* pc_response)
{
	int i = 0;
	xQueueSend(q_pconfigOut,&i, portMAX_DELAY);
	xQueueSend(q_pconfigOut,&pc_response, portMAX_DELAY);
	xEventGroupSync( eg_config, BIT_CONFIG_SYNC, BIT_COMM_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
}


char* fgetValuePointer	(char* pc_value,
						const char* pc_response)
{
	if (fgetNextString(pc_value))
	{
		return pc_value + fgetNextString(pc_value);
	}
	else
	{
		xQueueSend(q_pconfigOut,&pc_response, portMAX_DELAY);
		xQueueReceive(q_pconfigIn, &pc_value, portMAX_DELAY);
	}
	return pc_value;
}


void* fgetConfigFromTask	(TaskHandle_t ht_taskHandle,
							uint32_t ui_cmdlet)
{
	void* pv_config = NULL;
	while(xTaskNotify(ht_taskHandle,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
		taskYIELD();
	xQueueReceive(q_send,&pv_config, portMAX_DELAY);
	return pv_config;
}


void fsetLogLevel (int i)
{
	esp_log_level_set(TAG_ADC, i);
	esp_log_level_set(TAG_TCP, i);
	esp_log_level_set(TAG_UDP, i);
	esp_log_level_set(TAG_UART, i);
	esp_log_level_set(TAG_BLINK, i);
	esp_log_level_set(TAG_WIFI, i);
	esp_log_level_set(TAG_CONF, i);
	esp_log_level_set(TAG_BATMON, i);
	esp_log_level_set(TAG_TEMPINT, i);
	esp_log_level_set(TAG_REDUND, i);
	esp_log_level_set(TAG_STORAGE, i);
}


uint64_t fuLongFromRomCode	(OneWireBus_ROMCode stu_romCode)
{
	uint64_t ul_romCode = 0;
	for (int i = sizeof(stu_romCode.bytes) - 1; i >= 0; i--)
	{
		ul_romCode |=  stu_romCode.bytes[i];
		ul_romCode <<= 8;
	}
	return ul_romCode;
}


void fromCodeFromULong	(uint64_t ul_romCode,
						OneWireBus_ROMCode stu_romCode)
{
	for ( int i = sizeof(stu_romCode.bytes) - 1; i >= 0; i--)
	{
		stu_romCode.bytes[i] = ((ul_romCode >> (8*i)) & 0x00000000000000FF);
	}
	stu_romCode.fields.family[0] = (ul_romCode & 0x00000000000000FF);
	stu_romCode.fields.crc[0] = (ul_romCode >> (8*7)) & 0x00000000000000FF;
	for (int i = 0; i < 6; i++)
	{
		stu_romCode.fields.serial_number[i+1] = ((ul_romCode >> (8*(i+1))) & 0x00000000000000FF);
	}
}


void finitAll(void)
{
	ESP_LOGD(TAG_CONF, "STARTING CONFIG INIT");
	xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
	while(xTaskNotify(ht_wifiRun,CMD_scwi,eSetValueWithoutOverwrite) != pdPASS)
		taskYIELD();
	xEventGroupSync( eg_config,
			BIT_CONFIG_SYNC,
			BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
			portMAX_DELAY );
	wifi_ap_record_t* ap_info = 0;
	uint32_t ap_count = 0;
	xQueueReceive(q_recv,&ap_count, portMAX_DELAY);
	xQueueReceive(q_recv,&ap_info, portMAX_DELAY);
	ESP_LOGD(TAG_CONF, "RECEIVED AP INFO");
	if(!ap_count) // NO AP WITH SUFFICIENT SIGNAL FOUND; STARTING IN AP MODE
	{
		while(xTaskNotify(ht_wifiRun,CMD_coap,eSetValueWithoutOverwrite) != pdPASS)
			taskYIELD();
		xEventGroupSync( eg_sync,
				BIT_CONFIG_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
	}
	else //AP WITH SIGNAL FOUND
	{
		uint32_t ui_flag = 0;
		for (int i = 0; i < ap_count; i++)
		{
			while(xTaskNotify(ht_storageRun,CMD_ldss,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			char* pc_ssidName = (char*) ap_info[i].ssid;
			xQueueSend(q_send, &pc_ssidName, portMAX_DELAY);
			xQueueReceive(q_recv, &ui_flag, portMAX_DELAY);
			if (ui_flag) //AP FOUND IN STORAGE STARTING IN STA MODE
			{
				while(xTaskNotify(ht_wifiRun,CMD_cost,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
				break;
			}
			else
			{
			}
		}

		if (!ui_flag) //NO AP FOUND IN STORAGE; STARTING IN AP MODE
		{
			while(xTaskNotify(ht_wifiRun,CMD_coap,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xEventGroupSync( eg_sync,
					BIT_CONFIG_SYNC,
					BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
					portMAX_DELAY );

		}
	}
	ESP_LOGD(TAG_CONF, "FINISHED INIT");
	xSemaphoreGive(hs_pointerQueue);


//	free(ap_info);
}
