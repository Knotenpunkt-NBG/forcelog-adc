/*
 * config.c
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#include "config.h"

int fConfig (int i_cmdlet, char* str_value)
{
	int i_flag = 0;

	switch (i_cmdlet)
	{
	case CMD_tare: //TARE: sets the momentary value to zero
		if (!b_recording)
		{
			uint32_t gui_tareValue = ftareADC();
			sprintf((char*)str_value, "|tare|%d|\t\tOK, tare successful|\n", gui_tareValue);
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot tare while recording!|\n", ERR_RECORDING);
		break;

	case CMD_cali: //CALIbration: calibrates the ADC with a known weight. uses gui_tarevalue (set by tare) as zero reference
	{
		if (!b_recording)
		{
			double d_weight = atof((char*)str_value);
			double gd_calValue = fcalADC(d_weight, 0);
			sprintf((char*)str_value, "|cali|%.*f|\t\tOK, calibration finished|\n", 20, gd_calValue);
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot calibrate while recording!|\n", ERR_RECORDING);
		break;
	}

	case CMD_vcal: //CALibration Value: reads calibration value from server and writes it to adc
		if (!b_recording)
		{
			double gd_calValue = atof((char*)str_value);
			sprintf((char*)str_value, "|vcal|%.*f|\t\tOK, calibration set|\n",20, gd_calValue);
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set calibration value while recording!|\n", ERR_RECORDING);
		break;

	case CMD_peek: //PEEK: starts the adc measurements as soon as socket mes is connected without waiting for an trigger (udp or pin)
		if (!b_recording)
		{
			b_recording = 1;

//			while(xTaskNotify(ht_adcRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
//				vTaskDelay(1/ portTICK_PERIOD_MS);
//			while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
//				vTaskDelay(1/ portTICK_PERIOD_MS);
//			while(xTaskNotify(ht_tempIntRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
//				vTaskDelay(1/ portTICK_PERIOD_MS);
//			while(xTaskNotify(ht_storageRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
//				vTaskDelay(1/ portTICK_PERIOD_MS);

			while(xTaskNotify(ht_tcpMes,CMD_peek,eSetValueWithoutOverwrite) != pdPASS)
							vTaskDelay(1/ portTICK_PERIOD_MS);

//			while(xTaskNotify(ht_adcRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
//				vTaskDelay(1/ portTICK_PERIOD_MS);
//			while(xTaskNotify(ht_blinkRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
//				vTaskDelay(1/ portTICK_PERIOD_MS);
//			while(xTaskNotify(ht_tempIntRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
//				vTaskDelay(1/ portTICK_PERIOD_MS);
//			while(xTaskNotify(ht_storageRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
//				vTaskDelay(1/ portTICK_PERIOD_MS);

			sprintf((char*)str_value, "|peek|1|\t\tOK, started peeking|\n");
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot peek while recording or peeking!|\n", ERR_RECORDING);
		break;
	case CMD_strt: //STaRT: starts without wating for trigger
	{
		ESP_LOGD(TAG_CONF, "CMDlet:start\n");
		if (b_recording)
		{
			ESP_LOGD(TAG_CONF, "SENDING Notifications\n");
			b_recording = 1;
			while(xTaskNotify(ht_adcRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_blinkRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_tempIntRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_storageRun,CMD_wait,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);

			while(xTaskNotify(ht_adcRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_blinkRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_tempIntRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_storageRun,CMD_trig,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			sprintf((char*)str_value, "|stop|1|\t\tOK, stopped recording|\n");
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot stop while in idle!|\n", ERR_RECORDING);
	}
		break;
	case CMD_stop: //STOP: stops the sampling, kills the tasks and recreates them
	{
		ESP_LOGD(TAG_CONF, "CMDlet:stop\n");
		if (b_recording)
		{
			ESP_LOGD(TAG_CONF, "SENDING Notifications\n");
			b_recording = 0;
			while(xTaskNotify(ht_adcRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_blinkRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_tempIntRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			while(xTaskNotify(ht_storageRun,CMD_stop,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			sprintf((char*)str_value, "|stop|1|\t\tOK, stopped recording|\n");
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot stop while in idle!|\n", ERR_RECORDING);
	}
	break;

	case CMD_mkfs: //formats SD card
		if (!b_recording)
		{
			while(xTaskNotify(ht_storageRun,CMD_mkfs,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			sprintf((char*)str_value, "|mkfs|1|\t\tOK, formatting SD card|\n");
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot wait while recording!|\n", ERR_RECORDING);
		break;
	case CMD_wait: //WAIT: starts the adc in an fast to trigger status until trigger (udp or pin) is received
		if (!b_recording)
		{
			b_recording = 1;
			xTaskCreate (t_udpWait, "t_udpWait", 2048, NULL, 10, NULL);
			sprintf((char*)str_value, "|wait|1|\t\tOK, waiting for trigger|\n");
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot wait while recording!|\n", ERR_RECORDING);
		break;

	case CMD_mper: //Measurement PERiod: sets the sampleperiod in us
		if (!b_recording)
		{
			uint64_t ul_mesPeriod = atoll((char*)str_value);
			if (fsetADCSpeed(ul_mesPeriod))
			{
				uint64_t* ptr_ull = malloc(sizeof(uint64_t));
				*ptr_ull = ul_mesPeriod;
				while(xTaskNotify(ht_adcRun,CMD_mper,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);
				xQueueSend(q_pointer,&ptr_ull, portMAX_DELAY);
				sprintf((char*)str_value, "|mper|%"PRIu64"|\t\tOK, ADC period set to x us|\n", ul_mesPeriod);
				i_flag = 1;
			}
			else
			{
				sprintf((char*)str_value, "|err|%d|\t\tERROR, measurement period(%"PRIu64") smaller then minimum period(%d)|\n",ERR_MPER_TOO_LOW,ul_mesPeriod, ADC_MIN_PERIOD);
			}
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR, cannot set measurement period while recording!|\n", ERR_RECORDING);
		break;

	case CMD_bper: //Blink PERiod: period of blink
		if (!b_recording)
		{
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			uint32_t ui_blinkPeriod = atoi((char*)str_value);
			while(xTaskNotify(ht_blinkRun,CMD_bper,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueSend(q_pointer,&ui_blinkPeriod, portMAX_DELAY);
			while(uxQueueMessagesWaiting(q_pointer));
			xSemaphoreGive(hs_pointerQueue);
			sprintf((char*)str_value, "|bper|%d|\t\tOK, blink period set to x ms|\n", ui_blinkPeriod);
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set blink period while recording!|\n", ERR_RECORDING);
		break;
	case CMD_bdur: //Blink DURation: duration of blink
		if (!b_recording)
		{
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			uint32_t ui_blinkDuration = atoi((char*)str_value);
			while(xTaskNotify(ht_blinkRun,CMD_bdur,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueSend(q_pointer,&ui_blinkDuration, portMAX_DELAY);
			while(uxQueueMessagesWaiting(q_pointer));
			xSemaphoreGive(hs_pointerQueue);
				sprintf((char*)str_value, "|bdur|%d|\t\tOK, blink duration set to x ms|\n", ui_blinkDuration);
				i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set blink duration while recording!|\n", ERR_RECORDING);
		break;
	case CMD_bbrt: //Blink BRighTness: brightness of blink (0-1023)
		if (!b_recording)
		{
			uint32_t ui_blinkBrightness = atoi((char*)str_value);
			if ((0 < ui_blinkBrightness) && (ui_blinkBrightness < 1024))
			{
				ESP_LOGD(TAG_BLINK, "Set blink brightness to %d\n", ui_blinkBrightness);
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				while(xTaskNotify(ht_blinkRun,CMD_bbrt,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
				xQueueSend(q_pointer,&ui_blinkBrightness, portMAX_DELAY);
				while(uxQueueMessagesWaiting(q_pointer));
				xSemaphoreGive(hs_pointerQueue);
				sprintf((char*)str_value, "|bbrt|%d|\t\tOK, blink brightness set|\n", ui_blinkBrightness);
				i_flag = 1;
			}
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set blink brightness while recording!|\n", ERR_RECORDING);
		break;
	case CMD_bfrq: //Blink FReQuency: sets frequency of blink (1-78125Hz)
		if (!b_recording)
		{
			uint32_t ui_blinkFrequency = atoi((char*)str_value);
			if ((1 < ui_blinkFrequency) && (ui_blinkFrequency < 78125))
			{
				xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
				while(xTaskNotify(ht_blinkRun,CMD_bfrq,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
				xQueueSend(q_pointer,&ui_blinkFrequency, portMAX_DELAY);
				while(uxQueueMessagesWaiting(q_pointer));
				xSemaphoreGive(hs_pointerQueue);
				sprintf((char*)str_value, "|bfrq|%d|\t\tOK, blink frequency set|\n", ui_blinkFrequency);
				i_flag = 1;
			}
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot blink frequency while recording!|\n", ERR_RECORDING);
		break;
	case CMD_benb: //Blink ENaBle: enables blink
		gb_blinkEnabled = 1;
		sprintf((char*)str_value, "|benb|1|\t\tOK, blink enabled|\n");
		i_flag = 1;
		break;
	case CMD_bdis: //Blink DISable: disables blink
		gb_blinkEnabled = 0;
		sprintf((char*)str_value, "|bdis|0|\t\tOK, blink disabled|\n");
		i_flag = 1;
		break;

	case CMD_zero: //ZERO: sets the momentary time to zero
		ul_zeroTime = esp_timer_get_time();
		sprintf((char*)str_value, "|zero|1|\t\tOK, starting time set to 0|\n");
		i_flag = 1;
		break;

	case CMD_blnk: //BLiNK: single shot blink without tcp message
		while(xTaskNotify(ht_blinkRun,CMD_blnk,eSetValueWithoutOverwrite) != pdPASS)
			taskYIELD();
		sprintf((char*)str_value, "|blnk|1|\t\tOK, blinking|\n");
		i_flag = 1;
		break;

	case CMD_save: //SAVE: saves config of ADC and blink to non volatile storage
		if (!b_recording)
		{
			fSaveConfig(hnvs_conf_0);
			sprintf((char*)str_value, "|save|1|\t\tOK, config saved to non volatile storage|\n");
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot save config while recording!|\n", ERR_RECORDING);
		break;
	case CMD_load: //LOAD: load config of ADC and blink from volatile storage
		if (!b_recording)
		{
			fLoadConfig(hnvs_conf_0);
			sprintf((char*)str_value, "|load|0|\t\tOK, config loaded|\n");
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot load config while recording!|\n", ERR_RECORDING);
		break;

	case CMD_loge: //Sets logging to error (highest)
		fsetLogLevel(LOG_ERROR);
		sprintf((char*)str_value, "|loge|1|\t\tOK, logging set to ERROR|\n");
		i_flag = 1;
		break;
	case CMD_logw: //Sets logging to warning
		fsetLogLevel(LOG_WARNING);
		sprintf((char*)str_value, "|logw|1|\t\tOK, logging set to WARNING|\n");
		i_flag = 1;
		break;
	case CMD_logi: //Sets logging to info
		fsetLogLevel(LOG_INFO);
		sprintf((char*)str_value, "|logi|1|\t\tOK, logging set to INFO|\n");
		i_flag = 1;
		break;
	case CMD_logd: //Sets logging to debug
		fsetLogLevel(LOG_DEBUG);
		sprintf((char*)str_value, "|logd|1|\t\tOK, logging set to DEBUG|\n");
		i_flag = 1;
		break;
	case CMD_logv: //Sets logging to verbose (lowest)
		fsetLogLevel(LOG_VERBOSE);
		sprintf((char*)str_value, "|logv|1|\t\tOK, logging set to VERBOSE|\n");
		i_flag = 1;
		break;

	case CMD_ssid: //ssid of access point
		if (!b_recording)
		{
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			ESP_LOGD(TAG_CONF, "SSID:%s",str_value);
			while(xTaskNotify(ht_wifiRun,CMD_ssid,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			xQueueSend(q_pointer,&str_value, portMAX_DELAY);
			while(uxQueueMessagesWaiting(q_pointer));
			xSemaphoreGive(hs_pointerQueue);
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set SSID while recording!|\n", ERR_RECORDING);
		break;
	case CMD_pass: //pass for access point
		if (!b_recording)
		{
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			while(xTaskNotify(ht_wifiRun,CMD_pass,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			xQueueSend(q_pointer,&str_value, portMAX_DELAY);
			while(uxQueueMessagesWaiting(q_pointer));
			xSemaphoreGive(hs_pointerQueue);
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set password while recording!|\n", ERR_RECORDING);
		break;

	case CMD_ipco: //ip for configuration server
		if (!b_recording)
		{
			uint32_t ui_temp = 0;
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			while(xTaskNotify(ht_tcpConf,CMD_ipco,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			inet_pton(AF_INET, (char*)str_value, &ui_temp);
			xQueueSend(q_pointer,&ui_temp, portMAX_DELAY);
			while(uxQueueMessagesWaiting(q_pointer));
			xSemaphoreGive(hs_pointerQueue);

			char ipConf[16];
			inet_ntop(AF_INET, &ui_temp, ipConf, sizeof(ipConf));
			sprintf((char*)str_value, "|ipco|%d|\t\tOK, IP of config server set to %s|\n", ui_temp, ipConf);
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set IP while recording!|\n", ERR_RECORDING);
		break;
	case CMD_ipme: //ip for logging server
		if (!b_recording)
		{
			uint32_t ui_temp = 0;
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			while(xTaskNotify(ht_tcpMes,CMD_ipme,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			inet_pton(AF_INET, (char*)str_value, &ui_temp);
			xQueueSend(q_pointer,&ui_temp, portMAX_DELAY);
			while(uxQueueMessagesWaiting(q_pointer));
			xSemaphoreGive(hs_pointerQueue);

			char ipConf[16];
			inet_ntop(AF_INET, &ui_temp, ipConf, sizeof(ipConf));
			sprintf((char*)str_value, "|ipco|%d|\t\tOK, IP of measurement server set to %s|\n", ui_temp, ipConf);
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set IP while recording!|\n", ERR_RECORDING);
		break;
	case CMD_poco: //port for configuration server
		if (!b_recording)
		{
			uint32_t ui_temp = 0;
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			while(xTaskNotify(ht_tcpConf,CMD_poco,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			ui_temp = htons(atoi((char*)str_value));
			xQueueSend(q_pointer,&ui_temp, portMAX_DELAY);
			while(uxQueueMessagesWaiting(q_pointer));
			xSemaphoreGive(hs_pointerQueue);
			sprintf((char*)str_value, "|ipco|%d|\t\tOK, port of config server set to %d|\n", ui_temp, ntohs(ui_temp));
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set port while recording!|\n", ERR_RECORDING);
		break;
	case CMD_pome: //port for logging server
		if (!b_recording)
		{
			uint32_t ui_temp = 0;
			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			while(xTaskNotify(ht_tcpMes,CMD_pome,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			ui_temp = htons(atoi((char*)str_value));
			xQueueSend(q_pointer,&ui_temp, portMAX_DELAY);
			while(uxQueueMessagesWaiting(q_pointer));
			xSemaphoreGive(hs_pointerQueue);
			sprintf((char*)str_value, "|ipco|%d|\t\tOK, port of measurement server set to %d|\n", ui_temp, ntohs(ui_temp));
			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set port while recording!|\n", ERR_RECORDING);
		break;
	case CMD_potr: //port for UDP trigger
		if (!b_recording)
		{
			//			stu_serverAddressUdp.sin_port = htons(atoi((char*)str_value));
			//			sprintf((char*)str_value, "|ipco|%d|\t\tOK, port of trigger set to %d|\n", stu_serverAddressUdp.sin_port, ntohs(stu_serverAddressUdp.sin_port));
			//			i_flag = 1;
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot set port while recording!|\n", ERR_RECORDING);
		break;

	case CMD_conn: //connects to the access point
		if (!b_recording)
		{
			while(xTaskNotify(ht_wifiRun,CMD_conn,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
		}
		else
			sprintf((char*)str_value, "|err|%d|\t\tERROR cannot reconnect while recording!|\n", ERR_RECORDING);
		break;

	case CMD_bath: //BATtery High: sets the high value of the battery monitor
//		ESP_LOGD(TAG_CONF, "CMD_bath\n");
//		xTimerStop(h_timerBatMon, portMAX_DELAY);
//		gf_batVolHigh = atof((str_value));
//		gi_batRawHigh = adc1_get_raw(ADC1_CHANNEL_4);
//		xTimerStart(h_timerBatMon, portMAX_DELAY);

		break;
	case CMD_batl: //BATtery Low: sets the low value of the battery monitor
//		ESP_LOGD(TAG_CONF, "CMD_batl\n");
//		xTimerStop(h_timerBatMon, portMAX_DELAY);
//		gf_batVolLow = atof((str_value));
//		gi_batRawLow = adc1_get_raw(ADC1_CHANNEL_4);
//		xTimerStart(h_timerBatMon, portMAX_DELAY);
		break;

	case CMD_scow://scans 1-Wire bus
		fscanOwb();
		break;
	case CMD_stpi://scans 1-Wire and write the found adress of internal d18b20 to non volatile storage

		break;
	default:
		i_flag = -1;
		break;
	}

	return	i_flag;
}

int fSaveConfig (uint32_t nvs_id)
{
	xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);

	while(xTaskNotify(ht_storageRun,CMD_save,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
	xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
	xSemaphoreGive(hs_pointerQueue);

//	uint32_t ui_temp = 0;
//	nvs_open("nvs_batmon", NVS_READWRITE, &h_nvs_batmon);
//	nvs_set_i32(h_nvs_batmon, "k_batRawLow", gi_batRawLow);
//	ui_temp = (uint32_t)(gf_batVolLow * 1000);
//	nvs_set_u32(h_nvs_batmon, "k_batVolLow", ui_temp);
//	nvs_set_i32(h_nvs_batmon, "k_batRawHigh", gi_batRawHigh);
//	ui_temp = (uint32_t)(gf_batVolHigh * 1000);
//	nvs_set_u32(h_nvs_batmon, "k_batVolHigh", ui_temp);
//	nvs_close(h_nvs_batmon);

	nvs_open("nvs_tempi", NVS_READWRITE, &h_nvs_tempi);
	uint64_t ul_romTempInt = fuLongFromRomCode(stu_romCode_probe_int);
	//nvs_set_u64()
	nvs_close(h_nvs_tempi);

	return 0;
}

int fLoadConfig (uint32_t nvs_id)
{
	ESP_LOGD(TAG_CONF, "LOADING");
	xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
	ESP_LOGD(TAG_CONF, "NOTIFYING SD CARD");
	while(xTaskNotify(ht_storageRun,CMD_load,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
	xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
	xSemaphoreGive(hs_pointerQueue);

	//
	//	nvs_open("nvs_Blink", NVS_READWRITE, &h_nvs_Blink);
	//	nvs_get_u32(h_nvs_Blink, "k_blinkDuration", &gui_blinkDuration);
	//	nvs_get_u32(h_nvs_Blink, "k_blinkPeriod", &gui_blinkPeriod);
	//	nvs_get_u8(h_nvs_Blink, "k_blinkEnabled", &gb_blinkEnabled);
	//	nvs_get_u32(h_nvs_Blink, "k_blinkBright", &gui_blinkBrightness);
	//	nvs_close(h_nvs_Blink);
	//
	//	uint32_t ui_temp = 0;
	//	nvs_open("nvs_batmon", NVS_READWRITE, &h_nvs_batmon);
	//	nvs_get_i32(h_nvs_batmon, "k_batRawLow", &gi_batRawLow);
	//	nvs_get_u32(h_nvs_batmon, "k_batVolLow", &ui_temp);
	//	gf_batVolLow = (float) ui_temp;
	//	gf_batVolLow /=  1000;
	//	nvs_get_i32(h_nvs_batmon, "k_batRawHigh", &gi_batRawHigh);
	//	nvs_get_u32(h_nvs_batmon, "k_batVolHigh", &ui_temp);
	//	gf_batVolHigh = (float) ui_temp;
	//	gf_batVolHigh /=  1000;
	//	nvs_close(h_nvs_batmon);

	return 0;
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
	xEventGroupSetBits(eg_tempInt, BIT_TEMP_START);
}

uint64_t fuLongFromRomCode (OneWireBus_ROMCode stu_romCode)
{
	uint64_t ul_romCode = 0;
	for (int i = sizeof(stu_romCode.bytes) - 1; i >= 0; i--)
	{
		ul_romCode |=  stu_romCode.bytes[i];
		ul_romCode <<= 8;
	}
	return ul_romCode;
}

void fromCodeFromULong (uint64_t ul_romCode, OneWireBus_ROMCode stu_romCode)
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
