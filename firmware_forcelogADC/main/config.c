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
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "\1|\2peek\3|\2"STR(ERR_RECORDING)"\3\4|\t\tCannot peek while recording or waiting.\n");
			}
			else
			{
				if ((xEventGroupGetBits(eg_status) & STATUS_BLINK_ENABLE))
				{
					while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );
				}
				while(xTaskNotify(ht_adcRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );

				while(xTaskNotify(ht_adcRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				if ((xEventGroupGetBits(eg_status) & STATUS_BLINK_ENABLE))
				{
					while(xTaskNotify(ht_blinkRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
				}
				xTimerStart(htim_tcpSend, portMAX_DELAY);


				xEventGroupClearBits(eg_status, BITS_STATI);
				xEventGroupSetBits(eg_status, STATUS_PEEKING);

				pc_configOut = malloc(64);
				sprintf(pc_configOut, "\1|\2peek\3|\20\3\4|\t\tOK, started peeking|\n");
			}
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_strt: //STaRT: starts without waiting for trigger
		{
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_PEEKING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "\1|\2strt\3|\2"STR(ERR_RECORDING)"\3\4|\t\tCannot start while recording or peeking.\n");
			}
			else
			{
				pc_configOut = malloc(512);
				sprintf(pc_configOut,
						MEASUREMENT_MESSAGE,
						pstu_adcConfig->ul_adcPeriod,
						pstu_sensorConfig->d_calValue, pstu_sensorConfig->i_tareValue,
						pstu_blinkConfig->ui_blinkPeriod, pstu_blinkConfig->ui_blinkDuration);
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
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "\1|\2strt\3|\20\3|\t\t\2Started recording.\3\n\4");
			}
			fsendResponse(0, 1, pc_configOut);
		}
		break;

		case CMD_stop: //STOP: stops the sampling
		{
			if (xEventGroupGetBits(eg_status) & STATUS_IDLE)
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|stop|"STR(ERR_RECORDING)"|\t\tCannot stop while in idle.\n");
			}
			else
			{
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

				xTimerStop(htim_tcpSend, portMAX_DELAY);
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|stop|1|\t\tOK, stopped recording|\n");

			}
			fsendResponse(0, 1, pc_configOut);

		}
		break;

		case CMD_wait: //WAIT: h
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot wait while recording or waiting.\n");
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
			}
			fsendResponse(0, 1, pc_configOut);
			break;

			//ADC CONFIG
		case CMD_cali: //CALIbration: calibrates the ADC with a known weight. uses tare value as zero reference
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate value while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_tare: //TARE: sets the momentary value to zero
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot tare while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_trzr: //TaRe ZeRo: sets the zero value of the sensor output
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot tare while recording or waiting.\n");
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_mper: //Measurement PERiod: sets the sampleperiod in us
			if (xEventGroupGetBits(eg_status) & STATUS_RECORDING)
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set measurement period while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}

			break;

		case CMD_zero: //ZERO: sets the momentary time to zero
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot zero time while recording or waiting.\n");
			}
			else
			{
				pc_configOut = malloc(64);
				while(xTaskNotify(ht_tcpMes,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				sprintf(pc_configOut, "|zero|1|\t\tOK, starting time set to 0|\n");
			}
			fsendResponse(0, 1, pc_configOut);
			break;

			//TODO VCAL IMPEMENTATION
		case CMD_vcal: //CALibration Value: reads calibration value from server and writes it to adc
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate batmon while recording or waiting.\n\4");
			}
			else
			{
//				pc_value = fgetValuePointer(pc_configIn, "|ack|0|\t\tEnter Calibration Value.\n");
//				if(pc_value !=0)
//				{
//					pc_configOut = malloc(64);
//					double d_vcal = strtod(pc_value, NULL);
//					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
//					portENTER_CRITICAL(&mux);
//					gstu_config->cell.d_calValue = d_vcal;
//					portEXIT_CRITICAL(&mux);
//					sprintf(pc_configOut,
//							"\1|\2vcal\3|\2%f\3|\t\tCalibration value set to %f\n\4",
//							gstu_config->cell.d_calValue, gstu_config->cell.d_calValue);
//				}
			}
			fsendResponse(0, 1, pc_configOut);
			break;


			//BLINK
		case CMD_bper: //Blink PERiod: period of blink
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set blink period while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_bdur: //Blink DURation: duration of blink
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set blink duration while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_bbrt: //Blink BRighTness: brightness of blink (0-1023)
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set blink brightness while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_bfrq: //Blink FReQuency: sets frequency of blink (1-78125Hz)
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set blink frequency while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_benb: //Blink ENaBle: enables blink
			if (!(xEventGroupGetBits(eg_status) & STATUS_BLINK_ENABLE))
			{
				xEventGroupSetBits(eg_status, STATUS_BLINK_ENABLE);
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
			}
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|benb|1|\t\tOK, blink enabled\n");
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_bdis: //Blink DISable: disables blink
			if (xEventGroupGetBits(eg_status) & STATUS_BLINK_ENABLE)
			{
				xEventGroupClearBits(eg_status, STATUS_BLINK_ENABLE);
				if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_PEEKING))
				{
					while(xTaskNotify(ht_blinkRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
				}
			}
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|benb|1|\t\tOK, blink disabled\n");
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_blnk: //BLiNK: single shot blink without tcp message
			while(xTaskNotify(ht_blinkRun,CMD_blnk,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|blnk|1|\t\tOK, blinking\n");
			fsendResponse(0, 1, pc_configOut);
		break;


			//STORAGE
		case CMD_mkfs: //formats SD card
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot format SD card while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_init:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot load config while recording or waiting.\n");
			}
			else
			{
				finitAll();
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "INITIATED\4");
			}
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_wrin: //write default init
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_list: //lists all the present config files
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot list files while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				{
					while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(1/ portTICK_PERIOD_MS);
					xEventGroupSync( eg_sync,
							BIT_CONFIG_SYNC,
							BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
							portMAX_DELAY );
				}
			}
			break;

		case CMD_lsst:
		case CMD_lsad:
		case CMD_lsbl:
		case CMD_lslc:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot list files while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;


		case CMD_rmbl:
		case CMD_rmad:
		case CMD_rmst:
		case CMD_rmlc:
		case CMD_ldad:
		case CMD_ldtc:
		case CMD_ldtm:
		case CMD_ldbl: //loads blink config from storage
		case CMD_svad:
		case CMD_svtc:
		case CMD_svtm:
		case CMD_svbl:
		case CMD_svst:
		case CMD_svlc:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot save config while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

//		case CMD_svlc:
//			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
//			{
//				pc_configOut = malloc(64);
//				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot save loadcell while recording or waiting.\n");
//			}
//			else
//			{
//				owb_string_from_rom_code(pstu_tempConfig->romExt,pstu_sensorConfig->ac_name,OWB_ROM_CODE_STRING_LENGTH);
//				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
//				while(xTaskNotify(ht_storageRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
//					taskYIELD();
//				xQueueReceive(q_recv, &pc_configOut, portMAX_DELAY);
//				xSemaphoreGive(hs_pointerQueue);
//			}
//			fsendResponse(0, 1, pc_configOut);
//			break;

		case CMD_lmlc: //LiMit LoadCell: sets limit of Loadcell and clears the recorded percentile
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot save config while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				owb_string_from_rom_code(	pstu_tempConfig->romExt,
											pstu_sensorConfig->ac_name,
											sizeof(OWB_ROM_CODE_STRING_LENGTH));
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_ADC_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_cllc: //CLear LoadCell: deletes the loadcell from storage
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate value while recording or waiting.\n");
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
						xSemaphoreGive(hs_pointerQueue);
					}
			}
			fsendResponse(0, 1, pc_configOut);
			break;


			//LOGGING
		case CMD_loge: //Sets logging to error (highest)
			fsetLogLevel(LOG_ERROR);
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|loge|1|\t\tOK, logging set to ERROR|\n");
			fsendResponse(0, 1, pc_configOut);
			break;
		case CMD_logw: //Sets logging to warning
			fsetLogLevel(LOG_WARNING);
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|logw|1|\t\tOK, logging set to WARNING|\n");
			fsendResponse(0, 1, pc_configOut);
			break;
		case CMD_logi: //Sets logging to info
			fsetLogLevel(LOG_INFO);
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|logi|1|\t\tOK, logging set to INFO|\n");
			fsendResponse(0, 1, pc_configOut);
			break;
		case CMD_logd: //Sets logging to debug
			fsetLogLevel(LOG_DEBUG);
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|logd|1|\t\tOK, logging set to DEBUG|\n");
			fsendResponse(0, 1, pc_configOut);
			break;
		case CMD_logv: //Sets logging to verbose (lowest)
			fsetLogLevel(LOG_VERBOSE);
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|logv|1|\t\tOK, logging set to VERBOSE|\n");
			fsendResponse(0, 1, pc_configOut);
			break;


			//WIFI
		case CMD_ssid: //ssid of access point

			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set SSID while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_wifiRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_pass: //pass for access point
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set password while recording or waiting.\n");
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_wifiRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
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
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_stto:
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) || (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set Station Timeout.\n");
			}
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
						pstu_staConfig->ui_wifiTimeout = ui_stationTimeout * 1000;
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
				}
			}
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_stpb: //STation Port Broadcast
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set broadcast port while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_wifiRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
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
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|conn|1|\t\tstarting connection\n");
			}
			fsendResponse(0, 1, pc_configOut);
			break;


			//SOCKETS FOR HOTSPOT
		case CMD_hspc: //port for configuration server
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set port while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
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
						pstu_hotspotConfig->portConf = ui_port;
						sprintf(pc_configOut,
								"|ipco|%d|\t\tOK, port of config server set to %d|\n",
								ui_port, ui_port);
						xSemaphoreGive(hs_pointerQueue);
					}
				}
				fsendResponse(0, 1, pc_configOut);
			}
			break;

		case CMD_hspm: //port for measurement server
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set port while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
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
						pstu_hotspotConfig->portMes = ui_port;
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
				}
				fsendResponse(0, 1, pc_configOut);
			}
			break;

		case CMD_hspt: //port for UDP trigger
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|%d|\t\tcannot set port while recording or waiting|\n", ERR_RECORDING);
				fsendResponse(0, 1, pc_configOut);
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
				fsendResponse(0, 1, pc_configOut);
			}
			break;



			//PORTS FOR STATION
		case CMD_stpc: //port for configuration server
			ESP_LOGD(TAG_CONF, "CHANGING CONFIG PORT");
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set port while recording or waiting.\n");

			}
			else
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|ack|0|\t\tEnter port.\n");
				pc_value = fgetValuePointer(pc_configIn, pc_configOut);
				pc_configOut = malloc(64);
				if(pc_value != 0)
				{

					uint32_t ui_port = (uint32_t)strtol(pc_value, NULL, 10);
					if(ui_port <= 65353)
					{
						pc_configOut = malloc(64);
						portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
						portENTER_CRITICAL(&mux);
						pstu_staConfigMom->portConf = ui_port;
						portEXIT_CRITICAL(&mux);

						sprintf(pc_configOut,
								"|stpc|%d|\t\tOK, port of config server set to %d|\n",
								ui_port, ui_port);
					}
				}
				else
				{
					sprintf(pc_configOut,
							"|stpc|0|\t\tCould not find value in string %s|\n", pc_value);
				}
			}
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_stpm: //port for measurement server
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot set port while recording or waiting.\n");
				fsendResponse(0, 1, pc_configOut);
			}
			else
			{
				xQueueSend(q_pconfigIn,
						&pc_configIn,
						portMAX_DELAY);
				while(xTaskNotify(ht_tcpMes,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xEventGroupSync( eg_sync,
						BIT_CONFIG_SYNC,
						BIT_TCPMES_SYNC | BIT_CONFIG_SYNC,
						portMAX_DELAY );
			}
			break;

		case CMD_stpt: //port for UDP trigger
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|%d|\t\tcannot set port while recording or waiting|\n", ERR_RECORDING);
				fsendResponse(0, 1, pc_configOut);
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
				fsendResponse(0, 1, pc_configOut);
			}
			break;


			//BATTERY MONITOR
		case CMD_bath: //BATtery High: sets the high value of the battery monitor
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate batmon while recording or waiting.\n\4");
			}
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
					pstu_batMonConfig->f_batVolHigh = (float)strtod(pc_value, NULL);
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
							pstu_batMonConfig->i_batRawHigh, pstu_batMonConfig->f_batVolHigh);
				}
			}
			fsendResponse(0, 1, pc_configOut);
			break;

		case CMD_batl: //BATtery Low: sets the low value of the battery monitor
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
			{
				pc_configOut = malloc(64);
				sprintf(pc_configOut, "|err|"STR(ERR_RECORDING)"|\t\tCannot calibrate batmon while recording or waiting.\n\4");
			}
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
					pstu_batMonConfig->f_batVolLow = (float)strtod(pc_value, NULL);
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
							pstu_batMonConfig->i_batRawLow, pstu_batMonConfig->f_batVolLow);
				}
			}
			fsendResponse(0, 1, pc_configOut);
			break;


		default:
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|err|1|\t\tCOMMAND NOT FOUND\n");
			fsendResponse(0, 1, pc_configOut);
			break;
		}
		ui_cmdlet = 0;
		ESP_LOGD(TAG_CONF, "FINISHED SWITCH");
	}
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


void finitAll(void)
{
	xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
	ESP_LOGD(TAG_CONF, "STARTING WIFI SCAN");
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
	if(!ap_count) // NO AP WITH SUFFICIENT SIGNAL FOUND; STARTING IN AP MODE
	{
		ESP_LOGD(TAG_CONF, "NO AP FOUND");
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
				i = ap_count;
			}
			else
			{
			}
		}

		if (!ui_flag) //NO AP FOUND IN STORAGE; STARTING IN HOTSPOT MODE
		{
			while(xTaskNotify(ht_wifiRun,CMD_coap,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xEventGroupSync( eg_sync,
					BIT_CONFIG_SYNC,
					BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
					portMAX_DELAY );

		}
	}
	xSemaphoreGive(hs_pointerQueue);

	xEventGroupClearBits(eg_status, BITS_STATI);
	xEventGroupSetBits(eg_status, STATUS_IDLE);
}
