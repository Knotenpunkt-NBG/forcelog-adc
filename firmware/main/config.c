/*
 * config.c
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#include "config.h"

int fConfig (int i_cmdlet, char* str_value)
{
	int err = 0;

	switch (i_cmdlet)
	{
	case 0:
		//xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		break;

	case CMD_tare: //tare
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		break;

	case CMD_rcal: //calibrates with an known weight and sends the factor back to the server
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		double d_calValue = atof((char*)str_value);
		xQueueSend(q_conf, &d_calValue,  0);
		break;

	case CMD_vcal: //reads calibration value from server and writes it to adc
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		break;

	case CMD_strt: //starts the adc measurements as soon as socket mes is connected without wating for an trigger (udp or pin)
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		xEventGroupSetBits(eg_adc, BIT_ADC_WAIT);
		break;

	case CMD_stop: //stops the sampling and kills the tasks
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		break;

	case CMD_wait: //starts the adc in an easy to trigger status until trigger (udp or pin) is received
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		break;

	case CMD_mper: //sets the sampleperiod in us
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		uint64_t ul_mesPeriod = atoll((char*)str_value);
		xQueueSend(q_conf, &ul_mesPeriod,  0);
		break;

	case CMD_bper: //period of blink
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		uint32_t ui_blinkPeriod = atoi((char*)str_value);
		xQueueSend(q_conf, &ui_blinkPeriod, 0);
		break;
	case CMD_bdur: //duration of blink
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		uint32_t ui_blinkDuration = atoi((char*)str_value);
		xQueueSend(q_conf, &ui_blinkDuration, 0);
		break;
	case CMD_bbrt: //brightness of blink (0-1023)
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		uint32_t ui_blinkBrightness = atoi((char*)str_value);
		xQueueSend(q_conf, &ui_blinkBrightness, 0);
		break;
	case CMD_bfrq: //sets frequency of blink (1-78125Hz)
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		uint32_t ui_blinkFrequency = atoi((char*)str_value);
		xQueueSend(q_conf, &ui_blinkFrequency, 0);
		break;
	case CMD_benb: //enables blink
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);;
		break;
	case CMD_bdis: //disables blink
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		break;

	case CMD_zero: //sets the momentary time to zero
		ul_zeroTime = esp_timer_get_time();
		break;

	case CMD_blnk: //single shot blink without tcp message
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		break;

	case CMD_save: //saves config of ADC and blink to non volatile storage
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		break;
	case CMD_load: //load config of ADC and blink from volatile storage
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		break;

	case CMD_loge: //Sets logging to error (highest)
		esp_log_level_set("*", ESP_LOG_ERROR);
		break;
	case CMD_logw: //Sets logging to warning
		esp_log_level_set("*", ESP_LOG_WARN);
		break;
	case CMD_logi: //Sets logging to info
		esp_log_level_set("*", ESP_LOG_INFO);
		break;
	case CMD_logd: //Sets logging to debug
		esp_log_level_set("*", ESP_LOG_DEBUG);
		break;
	case CMD_logv: //Sets logging to verbose (lowest)
		esp_log_level_set("*", ESP_LOG_VERBOSE);
		break;


	default:
		//send(sock, (const char*) "inv\n", strlen((const char*)"inv\n"), 0);
		break;
	}


	return	err;
}
