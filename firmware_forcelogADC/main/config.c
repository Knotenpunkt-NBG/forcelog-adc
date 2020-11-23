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
	while(xTaskNotify(ht_storageRun,CMD_init,eSetValueWithoutOverwrite) != pdPASS)
		taskYIELD();
	vTaskSuspend(NULL);
	while(xTaskNotify(ht_wifiRun,CMD_conn,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
	vTaskSuspend(NULL);
	ESP_LOGD(TAG_CONF, "INIT FINISHED");

	uint32_t ui_cmdlet = 0;
	char* pc_value = NULL;
	char* pc_configIn = NULL;
	char* pc_configOut = NULL;
	void* pv_config_mom = NULL;
	uint8_t status = STATUS_IDLE;

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
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tCannot peek while recording or waiting.|\n", ERR_RECORDING);
			}
			else
			{
				while(xTaskNotify(ht_tcpMes,CMD_peek,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_adcRun,CMD_peek,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_blinkRun,CMD_peek,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_tempIntRun,CMD_peek,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				sprintf(pc_configOut, "|peek|1|\t\tOK, started peeking|\n");
				status = STATUS_RECORDING;
			}
			break;
		case CMD_strt: //STaRT: starts without wating for trigger
		{
			if ((status == STATUS_RECORDING) || (status == STATUS_PEEKING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot start while recording or peeking|\n", ERR_RECORDING);
			}
			else
			{
				while(xTaskNotify(ht_tcpMes,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				while(xTaskNotify(ht_adcRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				while(xTaskNotify(ht_tempIntRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				while(xTaskNotify(ht_storageRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);

				while(xTaskNotify(ht_adcRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_blinkRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_tempIntRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_storageRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				sprintf(pc_configOut, "|strt|1|\t\tOK, started recording|\n");
				status = STATUS_RECORDING;
			}
		}
		break;
		case CMD_stop: //STOP: stops the sampling, kills the tasks and recreates them
		{
			if (status == STATUS_IDLE)
			{
				sprintf(pc_configOut, "|err|%d|\t\tERROR cannot stop while in idle!|\n", ERR_RECORDING);
			}
			else
			{
				status = STATUS_IDLE;
				while(xTaskNotify(ht_adcRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_blinkRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_tempIntRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_storageRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				while(xTaskNotify(ht_tcpMes,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				sprintf(pc_configOut, "|stop|1|\t\tOK, stopped recording|\n");
			}
		}
		break;
		case CMD_wait: //WAIT: starts the adc in an fast to trigger status until trigger (udp or pin) is received
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot wait while recording or already waiting|\n", ERR_RECORDING);
			}
			else
			{
				while(xTaskNotify(ht_tcpMes,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				while(xTaskNotify(ht_adcRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				while(xTaskNotify(ht_tempIntRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				while(xTaskNotify(ht_storageRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				status = STATUS_WAITING;
				xTaskCreate (t_udpWait, "t_udpWait", 2048, NULL, 10, NULL);
				sprintf(pc_configOut, "|wait|1|\t\tOK, waiting for trigger|\n");
			}
			break;

			//ADC CONFIG
		case CMD_cali: //CALIbration: calibrates the ADC with a known weight. uses tare value as zero reference
		{
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot calibrate while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				double* pd = malloc(sizeof(double));
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				if(fgetNextString(pc_configIn))
					*pd = atof(pc_configIn + fgetNextString(pc_configIn));
				else
				{
					int i = 0;
					xQueueSend(q_pconfigOut,&i, portMAX_DELAY);
					xQueueReceive(q_pconfigIn, &pc_configIn, portMAX_DELAY);
					*pd = atof(pc_configIn);
				}
				while(xTaskNotify(ht_adcRun,CMD_cali,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xQueueSend(q_pointer,&pd, portMAX_DELAY);
				while(uxQueueMessagesWaiting(q_pointer));
				xQueueReceive(q_pointer, &pd, portMAX_DELAY);
				xSemaphoreGive(hs_pointerQueue);
				sprintf(pc_configOut, "|cali|%.*f|\t\tOK, calibration finished|\n", 20, *pd);
				free(pd);
			}
			break;
		}
		case CMD_vcal: //CALibration Value: reads calibration value from server and writes it to adc
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot set calibration value while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				double* pd_calValue = malloc(sizeof(double));
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				if(fgetNextString(pc_configIn))
					*pd_calValue = atof(pc_configIn + fgetNextString(pc_configIn));
				else
				{
					int i = 0;
					xQueueSend(q_pconfigOut,&i, portMAX_DELAY);
					xQueueReceive(q_pconfigIn, &pc_configIn, portMAX_DELAY);
					*pd_calValue = atof(pc_configIn);
				}
				while(xTaskNotify(ht_adcRun,CMD_cali,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xQueueSend(q_pointer,&pd_calValue, portMAX_DELAY);
				xSemaphoreGive(hs_pointerQueue);
				free(pd_calValue);
				sprintf(pc_configOut, "|vcal|%.*f|\t\tOK, calibration set|\n",20, *pd_calValue);
				free(pd_calValue);
			}
			break;
		case CMD_tare: //TARE: sets the momentary value to zero
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot tare while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				while(xTaskNotify(ht_adcRun,CMD_tare,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				sprintf(pc_configOut, "|tare|1|\t\tOK, tare successful|\n");
			}
			break;
		case CMD_mper: //Measurement PERiod: sets the sampleperiod in us
			if (status == STATUS_RECORDING)
			{
				sprintf(pc_configOut, "|err|%d|\t\tERROR, cannot set measurement period while recording!|\n", ERR_RECORDING);
			}
			else
			{
				uint64_t ul_mesPeriod = atoll((char*)pc_value);
				if (fsetADCSpeed(ul_mesPeriod))
				{
					uint64_t* ptr_ull = malloc(sizeof(uint64_t));
					*ptr_ull = ul_mesPeriod;
					while(xTaskNotify(ht_adcRun,CMD_mper,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
					xQueueSend(q_pointer,&ptr_ull, portMAX_DELAY);
					sprintf(pc_configOut, "|mper|%"PRIu64"|\t\tOK, ADC period set to x us|\n", ul_mesPeriod);
				}
				else
				{
					sprintf(pc_configOut, "|err|%d|\t\tERROR, measurement period(%"PRIu64") smaller then minimum period(%d)|\n",ERR_MPER_TOO_LOW,ul_mesPeriod, ADC_MIN_PERIOD);
				}
			}
			break;
		case CMD_zero: //ZERO: sets the momentary time to zero
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot zero time while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				ul_zeroTime = esp_timer_get_time();
				sprintf(pc_configOut, "|zero|1|\t\tOK, starting time set to 0|\n");
			}
			break;

			//BLINK
		case CMD_bper: //Blink PERiod: period of blink
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set blink period while recording or waiting.\n");
			}
			else
			{
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				uint32_t ui_blinkPeriod = 0;
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink period in ms.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					ui_blinkPeriod = strtol(pc_value, NULL, 10);
					if(ui_blinkPeriod !=0)
					{
						pv_config_mom = fgetConfigFromTask(ht_blinkRun, CMD_svbl);
						if(ui_blinkPeriod > ((struct stu_blinkConfig*)pv_config_mom)->ui_blinkPeriod)
						{
							((struct stu_blinkConfig*)pv_config_mom)->ui_blinkPeriod = ui_blinkPeriod;
							sprintf(pc_configOut, "|bdur|%d|\t\tOK, blink period set to %d ms.\n", ui_blinkPeriod, ui_blinkPeriod);
						}
						else
						{
							sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tPeriod(%d) must be higher then duration(%d).\n"
									, ui_blinkPeriod, ((struct stu_blinkConfig*)pv_config_mom)->ui_blinkPeriod);
						}
					}
					else
					{
						sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tcould not find value in message(%s).\n", pc_value);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
					vTaskResume(ht_blinkRun);
					xSemaphoreGive(hs_pointerQueue);
				}
			}
			break;
		case CMD_bdur: //Blink DURation: duration of blink
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set blink duration while recording or waiting.\n");
			}
			else
			{
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				uint32_t ui_blinkDuration = 0;
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					ui_blinkDuration = strtol(pc_value, NULL, 10);
					if(ui_blinkDuration !=0)
					{
						pv_config_mom = fgetConfigFromTask(ht_blinkRun, CMD_svbl);
						if(ui_blinkDuration < ((struct stu_blinkConfig*)pv_config_mom)->ui_blinkPeriod)
						{
							((struct stu_blinkConfig*)pv_config_mom)->ui_blinkDuration = ui_blinkDuration;
							sprintf(pc_configOut, "|bdur|%d|\t\tBlink duration set to %d ms.\n", ui_blinkDuration, ui_blinkDuration);

						}
						else
						{
							sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tDuration(%d) must be lower then Period(%d).\n"
									, ui_blinkDuration, ((struct stu_blinkConfig*)pv_config_mom)->ui_blinkPeriod);
						}
					}
					else
					{
						sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCould not find value in message(%s).\n", pc_value);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
					vTaskResume(ht_blinkRun);
					xSemaphoreGive(hs_pointerQueue);
				}
			}
			break;
		case CMD_bbrt: //Blink BRighTness: brightness of blink (0-1023)
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set blink brightness while recording or waiting.\n");
			}
			else
			{
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				uint32_t ui_blinkBrightness = 0;
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					ui_blinkBrightness = strtol(pc_value, NULL, 10);
					if(ui_blinkBrightness !=0)
					{
						pv_config_mom = fgetConfigFromTask(ht_blinkRun, CMD_svbl);
						if((0 < ui_blinkBrightness) && (ui_blinkBrightness < 1024))
						{
							((struct stu_blinkConfig*)pv_config_mom)->ui_blinkBrightness = ui_blinkBrightness;
							sprintf(pc_configOut, "|bdur|%d|\t\tBrightness set to %d ms.\n", ui_blinkBrightness, ui_blinkBrightness);
						}
						else
						{
							sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tBrightness(%d) must be between 1 and 1023.\n"
									, ui_blinkBrightness);
						}
					}
					else
					{
						sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCould not find value in message(%s).\n", pc_value);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
					vTaskResume(ht_blinkRun);
					xSemaphoreGive(hs_pointerQueue);
				}
			}
			break;
		case CMD_bfrq: //Blink FReQuency: sets frequency of blink (1-78125Hz)
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set blink frequency while recording or waiting.\n");
			}
			else
			{
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				uint32_t ui_blinkFrequency = 0;
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tEnter blink frequency in ms:\n");
				if(pc_value != 0)
				{
					pc_configOut = malloc(64);
					ui_blinkFrequency = strtol(pc_value, NULL, 10);
					if(ui_blinkFrequency !=0)
					{
						pv_config_mom = fgetConfigFromTask(ht_blinkRun, CMD_svbl);
						if((1 < ui_blinkFrequency) && (ui_blinkFrequency < 78125))
						{
							((struct stu_blinkConfig*)pv_config_mom)->ui_blinkFrequency = ui_blinkFrequency;
							sprintf(pc_configOut, "|bfrq|%d|\t\tFrequency set to %d Hz.\n", ui_blinkFrequency, ui_blinkFrequency);
						}
						else
						{
							sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tFrequency(%d) must be between 1 and 78125hz.\n"
									, ui_blinkFrequency);
						}
					}
					else
					{
						sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tcould not find value in message(%s)|\n", pc_value);
					}
					freturnMessage(pc_configOut);
					free(pc_configOut);
					vTaskResume(ht_blinkRun);
					xSemaphoreGive(hs_pointerQueue);
				}
			}

			break;
		case CMD_benb: //Blink ENaBle: enables blink
			while(xTaskNotify(ht_blinkRun,CMD_benb,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			freturnMessage("|benb|1|\t\tOK, blink enabled\n");
			break;
		case CMD_bdis: //Blink DISable: disables blink
			while(xTaskNotify(ht_blinkRun,CMD_bdis,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			freturnMessage("|bdis|0|\t\tOK, blink disabled|\n");
			break;
		case CMD_blnk: //BLiNK: single shot blink without tcp message
		{
			while(xTaskNotify(ht_blinkRun,CMD_blnk,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			freturnMessage("|blnk|1|\t\tOK, blinking\n");
		}
		break;

			//STORAGE
		case CMD_mkfs: //formats SD card
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot format SD card while recording or waiting|\n", ERR_RECORDING);
				freturnMessage(pc_configOut);
			}
			else
			{
				while(xTaskNotify(ht_storageRun,CMD_mkfs,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				vTaskSuspend(NULL);
				sprintf(pc_configOut, "|mkfs|1|\t\tSD card has been formatted.|\n");
				freturnMessage(pc_configOut);
			}
			break;
//		case CMD_init:
//			while(xTaskNotify(ht_storageRun,CMD_init,eSetValueWithoutOverwrite) != pdPASS)
//				taskYIELD();
//			vTaskSuspend(NULL);
////			while(xTaskNotify(ht_wifiRun,CMD_conn,eSetValueWithoutOverwrite) != pdPASS)
////				vTaskDelay(1/ portTICK_PERIOD_MS);
//			break;


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
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot load config while recording or waiting.\n");
			else{
				pc_configOut = malloc(64);
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tInsert filename:\n");
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
				xQueueSend(q_pointer,&pc_value, portMAX_DELAY);
				vTaskSuspend(NULL);
				xQueueReceive(q_pointer, &pc_configOut, portMAX_DELAY);
				xSemaphoreGive(hs_pointerQueue);
				freturnMessage(pc_configOut);
				vTaskResume(ht_storageRun);
				free(pc_configOut);
			}
			break;

		case CMD_inad:
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set init while recording or waiting.\n");
			else{
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tInsert filename:\n");

				if (fchangeInit(ui_cmdlet, pc_configIn) != 0){
					sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCould not find file:%s\n", pc_value);
					freturnMessage(pc_configOut);
				}
				else{
					sprintf(pc_configOut, "|inad|%s|\t\tInit changed.\n", pc_value);
					freturnMessage(pc_configOut);
				}
			}
			break;
		case CMD_intc:
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set init while recording or waiting.\n");
			else{
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tInsert filename:\n");

				if (fchangeInit(ui_cmdlet, pc_configIn) != 0){
					sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCould not find file:%s\n", pc_value);
					freturnMessage(pc_configOut);
				}
				else{
					sprintf(pc_configOut, "|intc|%s|\t\tInit changed.\n", pc_value);
					freturnMessage(pc_configOut);
				}
			}
			break;
		case CMD_intm:
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set init while recording or waiting.\n");
			else{
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tInsert filename:\n");

				if (fchangeInit(ui_cmdlet, pc_configIn) != 0){
					sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCould not find file:%s\n", pc_value);
					freturnMessage(pc_configOut);
				}
				else{
					sprintf(pc_configOut, "|intm|%s|\t\tInit changed.\n", pc_value);
					freturnMessage(pc_configOut);
				}
			}
			break;
		case CMD_inbl:
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set init while recording or waiting.\n");
			else{
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tInsert filename:\n");

				if (fchangeInit(ui_cmdlet, pc_configIn) != 0){
					sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCould not find file:%s\n", pc_value);
					freturnMessage(pc_configOut);
				}
				else{
					sprintf(pc_configOut, "|inbl|%s|\t\tInit changed.\n", pc_value);
					freturnMessage(pc_configOut);
				}
			}
			break;
		case CMD_inwi:
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
				freturnMessage("|err|"STR(ERR_RECORDING)"|\t\tCannot set init while recording or waiting.\n");
			else{
				pc_value = fgetValuePointer(pc_configIn, "|user|\t\tInsert filename:\n");

				if (fchangeInit(ui_cmdlet, pc_configIn) != 0){
					sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCould not find file:%s\n", pc_value);
					freturnMessage(pc_configOut);
				}
				else{
					sprintf(pc_configOut, "|inwi|%s|\t\tInit changed.\n", pc_value);
					freturnMessage(pc_configOut);
				}
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
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot set SSID while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				pc_configOut = malloc(64);
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter SSID:.\n");
				if(pc_value != 0)
				{
					pv_config_mom = fgetConfigFromTask(ht_wifiRun, CMD_ssid);
					strcpy((char*)((wifi_config_t*)pv_config_mom)->sta.ssid, pc_value);
					sprintf(pc_configOut, "|ssid|%s|\t\tSSID set to %s.\n", pc_value, pc_value);
					freturnMessage(pc_configOut);
					vTaskResume(ht_wifiRun);
					xSemaphoreGive(hs_pointerQueue);
				}
			}
			break;
		case CMD_pass: //pass for access point
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tERROR cannot set password while recording!|\n", ERR_RECORDING);
			}
			else
			{
				pc_configOut = malloc(64);
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					pv_config_mom = fgetConfigFromTask(ht_wifiRun, CMD_pass);
					strcpy((char*)((wifi_config_t*)pv_config_mom)->sta.password, pc_value);
					sprintf(pc_configOut, "|ssid|%s|\t\tPassword set to %s.\n", pc_value, pc_value);
					freturnMessage(pc_configOut);
					free(pc_configOut);
					vTaskResume(ht_wifiRun);
					xSemaphoreGive(hs_pointerQueue);
				}
			}
			break;

			//SOCKETS
		case CMD_ipco: //ip for configuration server
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot set IP while recording or waiting|\n", ERR_RECORDING);
				freturnMessage(pc_configOut);
			}
			else
			{
				pc_configOut = malloc(64);
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					uint32_t ui_ip = 0;
					inet_pton(AF_INET,pc_value, &ui_ip);

					xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
					pv_config_mom = fgetConfigFromTask(ht_tcpConf, ui_cmdlet);
					((struct sockaddr_in*)pv_config_mom)->sin_addr.s_addr = ui_ip;

					sprintf(pc_configOut, "|ipco|%d|\t\tOK, IP of config server set to %s|\n", ui_ip, pc_value);
					freturnMessage(pc_configOut);
					free(pc_configOut);
					vTaskResume(ht_tcpConf);
					xSemaphoreGive(hs_pointerQueue);
				}
			}
			break;
		case CMD_poco: //port for configuration server
			pc_configOut = malloc(64);
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot set IP while recording or waiting|\n", ERR_RECORDING);
				freturnMessage(pc_configOut);
			}
			else
			{
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					uint32_t ui_port = (uint32_t)strtol(pc_value, NULL, 10);
					if(ui_port <= 65353)
					{
						pv_config_mom = fgetConfigFromTask(ht_tcpConf, ui_cmdlet);
						((struct sockaddr_in*)pv_config_mom)->sin_port = htons(ui_port);
						sprintf(pc_configOut, "|ipco|%d|\t\tOK, port of config server set to %d|\n", ui_port, ui_port);
						freturnMessage(pc_configOut);
						vTaskResume(ht_tcpConf);
						xSemaphoreGive(hs_pointerQueue);
					}
				}
			}
			free(pc_configOut);
			break;
		case CMD_ipme: //ip for logging server
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot set IP while recording or waiting|\n", ERR_RECORDING);
				freturnMessage(pc_configOut);
			}
			else
			{
				pc_configOut = malloc(64);
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					uint32_t ui_ip = 0;
					if (inet_pton(AF_INET,pc_value, &ui_ip))
					{
						xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
						pv_config_mom = fgetConfigFromTask(ht_tcpMes, ui_cmdlet);
						((struct sockaddr_in*)pv_config_mom)->sin_addr.s_addr = ui_ip;

						sprintf(pc_configOut, "|ipco|%d|\t\tOK, IP of config server set to %s|\n", ui_ip, pc_value);
						freturnMessage(pc_configOut);
						free(pc_configOut);
						vTaskResume(ht_tcpMes);
						xSemaphoreGive(hs_pointerQueue);
					}
				}
			}
			break;
		case CMD_pome: //port for logging server
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot set IP while recording or waiting|\n", ERR_RECORDING);
				freturnMessage(pc_configOut);
			}
			else
			{
				pc_configOut = malloc(64);
				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter blink duration in ms.\n");
				if(pc_value != 0)
				{
					uint32_t ui_port = (uint32_t)strtol(pc_value, NULL, 10);
					if(ui_port <= 65353)
					{
						pv_config_mom = fgetConfigFromTask(ht_tcpMes, ui_cmdlet);
						((struct sockaddr_in*)pv_config_mom)->sin_port = htons(ui_port);
						sprintf(pc_configOut, "|ipco|%d|\t\tOK, port of measurement server set to %d|\n", ui_port, ui_port);
						freturnMessage(pc_configOut);
						free(pc_configOut);
						vTaskResume(ht_tcpMes);
						xSemaphoreGive(hs_pointerQueue);
					}
				}
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
			//				triggerConfig_mom->trigSocket.sin_addr.s_addr = ui_ip;
			//				char ipConf[16];
			//				inet_ntop(AF_INET, &ui_ip, ipConf, sizeof(ipConf));
			//				sprintf(pc_configOut, "|ipco|%d|\t\tOK, IP of measurement server set to %s|\n", ui_ip, ipConf);
			//				xQueueSend(q_pconfigOut,&pc_configOut, portMAX_DELAY);
			//			}
			break;
		case CMD_potr: //port for UDP trigger
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
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
				//				triggerConfig_mom->trigSocket.sin_port = ui_port;
				sprintf(pc_configOut, "|ipco|%d|\t\tOK, IP of measurement server set to %d|\n", ui_port, ntohs(ui_port));
				xQueueSend(q_pconfigOut,&pc_configOut, portMAX_DELAY);
			}
			break;

		case CMD_conn: //connects to the access point
			if (status == STATUS_RECORDING)
			{
				sprintf(pc_configOut, "|err|%d|\t\tERROR cannot reconnect while recording!|\n", ERR_RECORDING);
			}
			else
			{
				while(xTaskNotify(ht_wifiRun,CMD_conn,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|conn|1|\t\tstarting connection\n");
				freturnMessage(pc_configOut);
				free(pc_configOut);
			}
			break;

			//BATTERY MONITOR
		case CMD_bath: //BATtery High: sets the high value of the battery monitor
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot calibrate batmon while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				float f_battery = 0;
				if (fgetNextString(pc_configIn))
				{
					f_battery = atof(pc_configIn + fgetNextString(pc_configIn));
				}
				else
				{
					int i = 0;
					xQueueSend(q_pconfigOut,&i, portMAX_DELAY);
					xQueueReceive(q_pconfigIn, &pc_configIn, portMAX_DELAY);
					f_battery = atof(pc_configIn);
				}
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				while(xTaskNotify(ht_batmonRun,CMD_bath,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xQueueSend(q_pointer,&f_battery, portMAX_DELAY);
				while(uxQueueMessagesWaiting(q_pointer));
				xSemaphoreGive(hs_pointerQueue);
				sprintf(pc_configOut, "|bath|%f|\t\tOK, battery high is set to %f|\n", f_battery, f_battery);
			}
			break;
		case CMD_batl: //BATtery Low: sets the low value of the battery monitor
			if ((status == STATUS_RECORDING) || (status == STATUS_WAITING))
			{
				sprintf(pc_configOut, "|err|%d|\t\tcannot calibrate batmon while recording or waiting|\n", ERR_RECORDING);
			}
			else
			{
				float f_battery = 0;
				if (fgetNextString(pc_configIn))
				{
					f_battery = atof(pc_configIn + fgetNextString(pc_configIn));
				}
				else
				{
					int i = 0;
					xQueueSend(q_pconfigOut,&i, portMAX_DELAY);
					xQueueReceive(q_pconfigIn, &pc_configIn, portMAX_DELAY);
					f_battery = atof(pc_configIn);
				}
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				while(xTaskNotify(ht_batmonRun,CMD_batl,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xQueueSend(q_pointer,&f_battery, portMAX_DELAY);
				while(uxQueueMessagesWaiting(q_pointer));
				xSemaphoreGive(hs_pointerQueue);
				sprintf(pc_configOut, "|bath|%f|\t\tOK, battery low is set to %f|\n", f_battery, f_battery);
			}
			break;


			//ONE WIRE (OWB)
		case CMD_scow://scans 1-Wire bus
			if (status == STATUS_RECORDING)
			{
				sprintf(pc_configOut, "|err|%d|\t\tERROR cannot reconnect while recording!|\n", ERR_RECORDING);
			}
			else
			{
				fscanOwb();
			}
			break;
		case CMD_stpi://scans 1-Wire and write the found adress of internal d18b20 to non volatile storage
			break;

		default:
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|err|1|\t\tCOMMAND NOT FOUND\n");
			freturnMessage(pc_configOut);
			vTaskSuspend(NULL);
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
	vTaskSuspend(NULL);
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
		while(uxQueueMessagesWaiting(q_pconfigOut));
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
	xQueueReceive(q_pointer,&pv_config, portMAX_DELAY);
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

void fscanOwb(void)
{
	// Find all connected devices
	printf("Find devices:\n");
	//OneWireBus_ROMCode device_rom_codes[MAX_DEVICES] = {0};
	num_devices = 0;
	OneWireBus_SearchState search_state = {0};
	bool found = false;
	owb_search_first(owb, &search_state, &found);
	while (found)
	{
		char rom_code_s[17];
		owb_string_from_rom_code(search_state.rom_code, rom_code_s, sizeof(rom_code_s));
		//		printf("  %d : %s\n", num_devices, rom_code_s);
		device_rom_codes[num_devices] = search_state.rom_code;
		++num_devices;
		owb_search_next(owb, &search_state, &found);
	}

	if (num_devices == 1)
	{
		// For a single device only:
		//OneWireBus_ROMCode rom_code;
		owb_status status = owb_read_rom(owb, &stu_romCode_probe_int);
		if (status == OWB_STATUS_OK)
		{
			char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
			owb_string_from_rom_code(stu_romCode_probe_int, rom_code_s, sizeof(rom_code_s));
			printf("Single device %s present\n", rom_code_s);
		}
		else
		{
			//printf("An error occurred reading ROM code: %d", status);
		}
	}
	else
	{
		// Search for a known ROM code (LSB first):
		// For example: 0x1502162ca5b2ee28
		OneWireBus_ROMCode known_device = {
				.fields.family = { 0x28 },
				.fields.serial_number = { 0xee, 0xb2, 0xa5, 0x2c, 0x16, 0x02 },
				.fields.crc = { 0x15 },
		};
		char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
		owb_string_from_rom_code(known_device, rom_code_s, sizeof(rom_code_s));
		bool is_present = false;

		owb_status search_status = owb_verify_rom(owb, known_device, &is_present);
		if (search_status == OWB_STATUS_OK)
		{
			printf("Device %s is %s\n", rom_code_s, is_present ? "present" : "not present");
		}
		else
		{
			printf("An error occurred searching for known device: %d", search_status);
		}
	}
	//	xEventGroupSetBits(eg_tempInt, BIT_TEMP_START);
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

uint32_t	floadConfig	(uint32_t ui_cmdlet,
						TaskHandle_t ht_taskHandle,
						char* pc_value)
{
	while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
		taskYIELD();
	xQueueSend(q_pointer,&pc_value, portMAX_DELAY);
	vTaskSuspend( NULL );
	xQueueReceive(q_pointer,&pc_value, portMAX_DELAY);
	if (pc_value == 0)
	{
		fgetConfigFromTask(ht_taskHandle, ui_cmdlet);
		vTaskSuspend( NULL );
	}
	return (uint32_t)pc_value;
}

uint32_t	fchangeInit	(uint32_t ui_cmdlet,
						char* pc_value)
{
	return (uint32_t) pc_value;
}
