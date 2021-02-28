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
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = (1ULL<<LEDC_GPIO_BLINK);
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	xTaskCreate(t_blinkRun,		"t_blinkRun",		2048, NULL, 5, &ht_blinkRun);
}

void t_blinkRun(void* param)
{
	struct stu_mesCell dataPoint =
	{
			.b_type = TYPE_BLINK,
			.d_measurement = 0,
			.ul_time = 0
	};
	ledc_channel_config_t ledcChannelgstu_config;
	ledc_timer_config_t ledcTimergstu_config;
	ledcChannelgstu_config.channel    = LEDC_CH_BLINK;
	ledcChannelgstu_config.duty       = 0;
	ledcChannelgstu_config.gpio_num   = LEDC_GPIO_BLINK;
	ledcChannelgstu_config.speed_mode = LEDC_HS_MODE;
	ledcChannelgstu_config.hpoint     = 0;
	ledcChannelgstu_config.timer_sel  = LEDC_HS_TIMER;
	ledc_channel_config(&ledcChannelgstu_config);

	uint32_t ui_cmdlet = 0;
	while(1)
	{
		ledc_set_duty(ledcChannelgstu_config.speed_mode,
				ledcChannelgstu_config.channel,
				0);

		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false,
					ULONG_MAX,
					&ui_cmdlet,
					portMAX_DELAY);
			fblinkConfig(ui_cmdlet,
					&ledcTimergstu_config,
					&ledcChannelgstu_config);
		}

		h_timerBlink = xTimerCreate("timerBlink",
				gstu_config->blink.ui_blinkPeriod / portTICK_PERIOD_MS,
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
			ledc_set_duty(ledcChannelgstu_config.speed_mode,
					ledcChannelgstu_config.channel,
					gstu_config->blink.ui_blinkBrightness);
			ledc_update_duty(ledcChannelgstu_config.speed_mode,
					ledcChannelgstu_config.channel);
			dataPoint.ul_time = esp_timer_get_time();
			xQueueSend(q_measurements,
					&dataPoint,
					0);
			vTaskDelay(gstu_config->blink.ui_blinkDuration / portTICK_PERIOD_MS);
			ledc_set_duty(ledcChannelgstu_config.speed_mode,
					ledcChannelgstu_config.channel,
					0);
			ledc_update_duty(ledcChannelgstu_config.speed_mode,
					ledcChannelgstu_config.channel);
			xTaskNotifyWait(false,
					ULONG_MAX,
					&ui_cmdlet,
					portMAX_DELAY);
			if((ui_cmdlet != CMD_fire)
					&& (ui_cmdlet != CMD_stop))
			{
				fblinkConfig(ui_cmdlet,
						&ledcTimergstu_config,
						&ledcChannelgstu_config);
			}
		}
		xTimerStop(h_timerBlink, portMAX_DELAY);
		xTimerDelete(h_timerBlink, portMAX_DELAY);
	}
}

void fblinkConfig	(uint32_t CMDlet,
					ledc_timer_config_t* ledcTimergstu_config,
					ledc_channel_config_t* ledcChannelgstu_config)
{
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

	case CMD_bper:
	case CMD_bdur:
	case CMD_bbrt:
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_config,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_blnk:
		ledc_set_duty(ledcChannelgstu_config->speed_mode,
				ledcChannelgstu_config->channel,
				gstu_config->blink.ui_blinkBrightness);
		ledc_update_duty(ledcChannelgstu_config->speed_mode,
				ledcChannelgstu_config->channel);
		vTaskDelay(gstu_config->blink.ui_blinkDuration / portTICK_PERIOD_MS);
		ledc_set_duty(ledcChannelgstu_config->speed_mode,
				ledcChannelgstu_config->channel,
				0);
		ledc_update_duty(ledcChannelgstu_config->speed_mode,
				ledcChannelgstu_config->channel);
		break;

	case CMD_bfrq:
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_BLINK_SYNC,
				BIT_BLINK_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		ledcTimergstu_config->freq_hz = gstu_config->blink.ui_blinkFrequency;
		ledc_timer_config(ledcTimergstu_config);
		ledc_channel_config(ledcChannelgstu_config);
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




