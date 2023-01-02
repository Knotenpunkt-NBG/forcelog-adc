/*
 * blink.c
 *
 *  Created on: Jun 22, 2020
 *      Author: lor
 */

#include "blink.h"


void fblinkInit (void)
{
	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = (1ULL<<LEDC_GPIO_BLINK);
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	xTaskCreate(t_blinkRun,		"t_blinkRun",		2048, NULL, 5, &ht_blinkRun);
}

void t_blinkRun(void* param)
{
	stu_mesCell dataPoint =
	{
			.str_type		= "blnk",
			.str_data[0] = '\0',
			.ul_time = 0
	};
	ledc_channel_config_t ledcChannelBlink;
	ledc_timer_config_t ledcTimerBlink;
	ledcChannelBlink.channel    = LEDC_CH_BLINK;
	ledcChannelBlink.duty       = 0;
	ledcChannelBlink.gpio_num   = LEDC_GPIO_BLINK;
	ledcChannelBlink.speed_mode = LEDC_HS_MODE;
	ledcChannelBlink.hpoint     = 0;
	ledcChannelBlink.timer_sel  = LEDC_HS_TIMER;
	ledc_channel_config(&ledcChannelBlink);

	uint32_t ui_cmdlet = 0;
	while(1)
	{
		ledc_set_duty(ledcChannelBlink.speed_mode,
				ledcChannelBlink.channel,
				0);

		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false,
					ULONG_MAX,
					&ui_cmdlet,
					portMAX_DELAY);
			fblinkConfig(ui_cmdlet,
					&ledcTimerBlink,
					&ledcChannelBlink);
		}

		h_timerBlink = xTimerCreate("timerBlink",
				pstu_blinkConfig->ui_blinkPeriod / portTICK_PERIOD_MS,
				pdTRUE,
				NULL,
				cb_timerBlink);
		xEventGroupSync(eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		while((ui_cmdlet != CMD_trig)
				&& (ui_cmdlet != CMD_stop))
		{
			xTaskNotifyWait(false,
					ULONG_MAX,
					&ui_cmdlet,
					portMAX_DELAY);
		}
		if(ui_cmdlet == CMD_trig)
			xTimerStart(h_timerBlink, portMAX_DELAY);

		while(ui_cmdlet != CMD_stop)
		{
			ledc_set_duty(ledcChannelBlink.speed_mode,
					ledcChannelBlink.channel,
					pstu_blinkConfig->ui_blinkBrightness);
			ledc_update_duty(ledcChannelBlink.speed_mode,
					ledcChannelBlink.channel);
			dataPoint.ul_time = esp_timer_get_time();
			xQueueSend(q_measurements,
					&dataPoint,
					0);
			vTaskDelay(pstu_blinkConfig->ui_blinkDuration / portTICK_PERIOD_MS);
			ledc_set_duty(ledcChannelBlink.speed_mode,
					ledcChannelBlink.channel,
					0);
			ledc_update_duty(ledcChannelBlink.speed_mode,
					ledcChannelBlink.channel);
			xTaskNotifyWait(false,
					ULONG_MAX,
					&ui_cmdlet,
					portMAX_DELAY);
			if((ui_cmdlet != CMD_fire)
					&& (ui_cmdlet != CMD_stop))
			{
				fblinkConfig(ui_cmdlet,
						&ledcTimerBlink,
						&ledcChannelBlink);
			}
		}
		xTimerStop(h_timerBlink, portMAX_DELAY);
		xTimerDelete(h_timerBlink, portMAX_DELAY);
	}
}

void fblinkConfig	(uint32_t CMDlet,
					ledc_timer_config_t* ledcTimerBlink,
					ledc_channel_config_t* ledcChannelBlink)
{

	char* pc_value = 0;
	char* pc_configIn = 0;
	char* pc_configOut = 0;

	switch(CMDlet)
	{
	case CMD_ldbl:
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_config,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC | BIT_STORAGE_SYNC,
				portMAX_DELAY );
		break;

	case CMD_bbrt:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tEnter blink brightness (0-1023).\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			unsigned int ui_blinkBrightness = 0;
			pc_configOut = malloc(64);
			ui_blinkBrightness = strtol(pc_value, NULL, 10);
			if(ui_blinkBrightness !=0)
			{
				if((0 < ui_blinkBrightness) && (ui_blinkBrightness < 1024))
				{
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					pstu_blinkConfig->ui_blinkBrightness = ui_blinkBrightness;
					portEXIT_CRITICAL(&mux);
					sprintf(pc_configOut,
							"|bbrt|%d|\t\tBrightness set to %d.\n",
							ui_blinkBrightness, ui_blinkBrightness);
				}
				else
					sprintf(pc_configOut,
							"|bbrt|"STR(ERR_RECORDING)"|\t\tBrightness(%d) must be between 1 and 1023.\n",
							ui_blinkBrightness);
			}
			else
				sprintf(pc_configOut,
						"|bbrt|"STR(ERR_RECORDING)"|\t\tCould not find value in message(%s).\n",
						pc_value);
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_bdur:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|0|\t\tEnter blink duration in ms.\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			unsigned int ui_blinkDuration = 0;
			pc_configOut = malloc(64);
			ui_blinkDuration = fconvertToMs(pc_value);
			if(ui_blinkDuration !=0)
			{
				if(ui_blinkDuration < pstu_blinkConfig->ui_blinkPeriod)
				{
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					pstu_blinkConfig->ui_blinkDuration = ui_blinkDuration;
					portEXIT_CRITICAL(&mux);

					sprintf(pc_configOut, "|bdur|%d|\t\tBlink duration set to %d ms.\n", ui_blinkDuration, ui_blinkDuration);
				}
				else
					sprintf(pc_configOut, "|bdur|"STR(ERR_RECORDING)"|\t\tDuration(%d) must be lower then Period(%d).\n"
							, ui_blinkDuration, pstu_blinkConfig->ui_blinkPeriod);
			}
			else
				sprintf(pc_configOut, "|bdur|"STR(ERR_RECORDING)"|\t\tCould not find value in message(%s).\n", pc_value);
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_bper:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tEnter blink period in ms.\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			unsigned int ui_blinkPeriod = 0;
			pc_configOut = malloc(64);
			ui_blinkPeriod = fconvertToMs(pc_value);
			if(ui_blinkPeriod !=0)
			{
				if(ui_blinkPeriod > pstu_blinkConfig->ui_blinkDuration)
				{
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					pstu_blinkConfig->ui_blinkPeriod = ui_blinkPeriod;
					portEXIT_CRITICAL(&mux);
					frestartBlinkTimer();
					sprintf(pc_configOut,
							"|bper|%d|\t\tOK, blink period set to %d ms.\n",
							ui_blinkPeriod, ui_blinkPeriod);
				}
				else
					sprintf(pc_configOut,
							"|err|"STR(ERR_RECORDING)"|\t\tPeriod(%d) must be higher then duration(%d).\n"
							, ui_blinkPeriod, pstu_blinkConfig->ui_blinkDuration);
			}
			else
				sprintf(pc_configOut,
						"|err|"STR(ERR_RECORDING)"|\t\tcould not find value in message(%s).\n",
						pc_value);
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_blnk:
		ledc_set_duty(ledcChannelBlink->speed_mode,
				ledcChannelBlink->channel,
				pstu_blinkConfig->ui_blinkBrightness);
		ledc_update_duty(ledcChannelBlink->speed_mode,
				ledcChannelBlink->channel);
		vTaskDelay(pstu_blinkConfig->ui_blinkDuration / portTICK_PERIOD_MS);
		ledc_set_duty(ledcChannelBlink->speed_mode,
				ledcChannelBlink->channel,
				0);
		ledc_update_duty(ledcChannelBlink->speed_mode,
				ledcChannelBlink->channel);
		break;

	case CMD_bfrq:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tEnter blink frequency in ms:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);;
		if(pc_value != 0)
		{
			unsigned int ui_blinkFrequency = 0;
			pc_configOut = malloc(64);
			ui_blinkFrequency = strtol(pc_value, NULL, 10);
			if(ui_blinkFrequency !=0)
			{
				if((1 < ui_blinkFrequency) && (ui_blinkFrequency < 78125))
				{
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					pstu_blinkConfig->ui_blinkFrequency = ui_blinkFrequency;
					portEXIT_CRITICAL(&mux);
					ledcTimerBlink->freq_hz = pstu_blinkConfig->ui_blinkFrequency;
					ledc_timer_config(ledcTimerBlink);
					ledc_channel_config(ledcChannelBlink);
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
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_init:
		xEventGroupSync( eg_config,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC ,
				BIT_CONFIG_SYNC
					| BIT_ADC_SYNC
					| BIT_TCPMES_SYNC
					| BIT_TCPCONF_SYNC
					| BIT_WIFI_SYNC
					| BIT_BLINK_SYNC
					| BIT_BATMON_SYNC
					| BIT_STORAGE_SYNC,
				portMAX_DELAY );
		break;

	default:
		ESP_LOGD(TAG_BLINK, "CMDLET NOT IN CONFIG");
		break;
	}
}

void cb_timerBlink(void* arg)
{
	while(xTaskNotify(ht_blinkRun,CMD_fire,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
}

int frestartBlinkTimer(void)
{
	if (pdPASS == xTimerStop(h_timerBlink, portMAX_DELAY))
	{
		xTimerDelete(h_timerBlink, portMAX_DELAY);
		h_timerBlink = xTimerCreate("timerBlink",
				pstu_blinkConfig->ui_blinkPeriod / portTICK_PERIOD_MS,
				pdTRUE,
				NULL,
				cb_timerBlink);
		xTimerStart(h_timerBlink, portMAX_DELAY);
		return 1;
	}
	return 0;
}


