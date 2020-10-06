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
		if (struct_blinkConfig.b_blinkEnabled)
		{
			xTaskCreate(t_blinkRun, "t_blinkRun", 2048,NULL, 10, &ht_blinkRun);
		}
		xEventGroupSetBits(eg_adc, BIT_ADC_WAIT);
		xEventGroupSetBits(eg_blink, BIT_BLINK_WAIT);
		break;

	case CMD_stop: //stops the sampling and kills the tasks
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		xQueueSend(q_cmdlet, &i_cmdlet,  0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		esp_timer_stop(h_timerBlink);
		ledc_set_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel, 0);
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
		struct_blinkConfig.ui_blinkPeriod  = atoi((char*)str_value);
		break;
	case CMD_bdur: //duration of blink
		struct_blinkConfig.ui_blinkDuration = atoi((char*)str_value);
		break;
	case CMD_bbrt: //brightness of blink (0-1023)
		struct_blinkConfig.ui_blinkBrightness  = atoi((char*)str_value);
		break;
	case CMD_bfrq: //sets frequency of blink (1-78125Hz)
		struct_blinkConfig.ui_blinkFrequency = atoi((char*)str_value);
		break;
	case CMD_benb: //enables blink
		struct_blinkConfig.b_blinkEnabled = 1;
		break;
	case CMD_bdis: //disables blink
		struct_blinkConfig.b_blinkEnabled = 0;
		break;

	case CMD_zero: //sets the momentary time to zero
		ul_zeroTime = esp_timer_get_time();
		break;

	case CMD_blnk: //single shot blink without tcp message
		ledc_set_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel, struct_blinkConfig.ui_blinkBrightness);
		ledc_update_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel);
		vTaskDelay(struct_blinkConfig.ui_blinkDuration / portTICK_PERIOD_MS);
		ledc_set_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel, 0);
		ledc_update_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel);
		break;

	case CMD_save: //saves config of ADC and blink to non volatile storage
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		fSaveConfig(hnvs_conf_0);
		break;
	case CMD_load: //load config of ADC and blink from volatile storage
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_blink, BIT_BLINK_BREAK);
		xQueueSend(q_cmdlet, &i_cmdlet, 0);
		xEventGroupSetBits(eg_adc, BIT_ADC_BREAK);
		fLoadConfig(hnvs_conf_0);
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

	case CMD_ssid: //ssid of access point
		//strcpy((char*) sta_config.sta.ssid, str_value);
		break;
	case CMD_pass: //pass for access point
		//strcpy((char*) sta_config.sta.password, str_value);
		break;

	case CMD_ipco: //ip for configuration server
		inet_pton(AF_INET, (char*)str_value, &str_serverAddressMes.sin_addr.s_addr);
		break;
	case CMD_ipme: //ip for logging server
		inet_pton(AF_INET, (char*)str_value, &str_serverAddressConf.sin_addr.s_addr);
		break;
	case CMD_poco: //port for configuration server
		str_serverAddressConf.sin_port = htons(atoi((char*)str_value));
		break;
	case CMD_pome: //port for logging server
		str_serverAddressMes.sin_port = htons(atoi((char*)str_value));
		break;
	case CMD_potr: //port for UDP trigger
		str_serverAddressUdp.sin_port = htons(atoi((char*)str_value));
		break;

	case CMD_conn: //connects to the access point

		break;

	default:
		break;
	}
	return	err;
}


int fSaveConfig (uint32_t nvs_id)
{
	nvs_open("storage", NVS_READWRITE, &nvs_id);
	nvs_set_u16(nvs_id, "k_serverPortMes", str_serverAddressMes.sin_port);
	nvs_set_u16(nvs_id, "k_srvPortConf", str_serverAddressConf.sin_port);
	nvs_set_u32(nvs_id, "k_serverIp", str_serverAddressMes.sin_addr.s_addr);
	nvs_set_u32(nvs_id, "k_serverIp", str_serverAddressConf.sin_addr.s_addr);
	//nvs_set_str(nvs_id, "k_ssid", (char*) sta_config.sta.ssid);
	//nvs_set_str(nvs_id, "k_pass", (char*) sta_config.sta.password);
	nvs_commit(nvs_id);
	nvs_close(nvs_id);
	printf("ack\n");
	//printf("Saved. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
	printf("Saved Mes. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
	printf("Saved Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);
	return 0;
}

int fLoadConfig (uint32_t nvs_id)
{
	nvs_open("storage", NVS_READWRITE, &nvs_id);
	nvs_get_u16(nvs_id, "k_serverPortMes", &str_serverAddressMes.sin_port);
	nvs_get_u16(nvs_id, "k_srvPortConf", &str_serverAddressConf.sin_port);
	nvs_get_u32(nvs_id, "k_serverIp", &str_serverAddressMes.sin_addr.s_addr);
	nvs_get_u32(nvs_id, "k_serverIp", &str_serverAddressConf.sin_addr.s_addr);
	size_t required_size = 0;
	nvs_get_str(nvs_id, "k_ssid", NULL, &required_size);
	//nvs_get_str(nvs_id, "k_ssid", (char*) sta_config.sta.ssid, &required_size);
	nvs_get_str(nvs_id, "k_pass", NULL, &required_size);
	//nvs_get_str(nvs_id, "k_pass", (char*) sta_config.sta.password, &required_size);
	nvs_close(nvs_id);
	//printf("Load. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
	printf("Load Mes. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
	printf("Load Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);
	return 0;
}
