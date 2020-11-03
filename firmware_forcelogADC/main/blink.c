/*
 * blink.c
 *
 *  Created on: Jun 22, 2020
 *      Author: lor
 */

#include "blink.h"


void fblinkInit (void)
{
	xTaskCreate(t_blinkRun,		"t_blinkRun",		2048, NULL, 10, &ht_blinkRun);
}

void t_blinkRun(void *arg)
{
	vTaskSuspend(NULL);
	ledc_channel_config_t ledcChannelConfig_mom;
	ledc_timer_config_t ledcTimerConfig_mom;
	ledcChannelConfig_mom.channel    = LEDC_CH_BLINK;
	ledcChannelConfig_mom.duty       = 0;
	ledcChannelConfig_mom.gpio_num   = LEDC_GPIO_BLINK;
	ledcChannelConfig_mom.speed_mode = LEDC_HS_MODE;
	ledcChannelConfig_mom.hpoint     = 0;
	ledcChannelConfig_mom.timer_sel  = LEDC_HS_TIMER;
	ledc_channel_config(&ledcChannelConfig_mom);

	uint32_t ui_cmdlet = 0;
	struct stu_blinkConfig blinkConfig_mom;
	while(1)
	{
		ledc_set_duty(ledcChannelConfig_mom.speed_mode, ledcChannelConfig_mom.channel, 0);

		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			fblinkConfig(ui_cmdlet, &blinkConfig_mom, &ledcTimerConfig_mom, &ledcChannelConfig_mom);
			vTaskDelay(1);
		}
		h_timerBlink = xTimerCreate("timerBlink",blinkConfig_mom.ui_blinkPeriod / portTICK_PERIOD_MS,pdTRUE,NULL,callback_timerBlink);
		uint64_t time_since_boot = 0;
		while((ui_cmdlet != CMD_trig) && (ui_cmdlet != CMD_stop))
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		}
		xTimerStart(h_timerBlink, portMAX_DELAY);
		while(ui_cmdlet != CMD_stop)
		{
			ledc_set_duty(ledcChannelConfig_mom.speed_mode, ledcChannelConfig_mom.channel, blinkConfig_mom.ui_blinkBrightness);
			ledc_update_duty(ledcChannelConfig_mom.speed_mode, ledcChannelConfig_mom.channel);
			time_since_boot = esp_timer_get_time();
			xQueueSend(q_time_blink_tcp, &time_since_boot,  0);
			while(xTaskNotify(ht_tcpMes,CMD_BLINK_READY,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			vTaskDelay(blinkConfig_mom.ui_blinkDuration / portTICK_PERIOD_MS);
			ledc_set_duty(ledcChannelConfig_mom.speed_mode, ledcChannelConfig_mom.channel, 0);
			ledc_update_duty(ledcChannelConfig_mom.speed_mode, ledcChannelConfig_mom.channel);
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			if((ui_cmdlet != CMD_fire) && (ui_cmdlet != CMD_stop))
			{
				fblinkConfig(ui_cmdlet, &blinkConfig_mom, &ledcTimerConfig_mom, &ledcChannelConfig_mom);
			}
		}
		xTimerStop(h_timerBlink, portMAX_DELAY);
		xTimerDelete(h_timerBlink, portMAX_DELAY);
	}
}

void fblinkConfig(uint32_t CMDlet, struct stu_blinkConfig *blinkConfig_mom, ledc_timer_config_t* ledcTimerConfig_mom, ledc_channel_config_t* ledcChannelConfig_mom)
{
	struct stu_blinkConfig* p_blinkConfig_ext = 0;

	switch(CMDlet)
	{
	case CMD_load:
		xQueuePeek(q_pointer, &p_blinkConfig_ext, portMAX_DELAY);
		blinkConfig_mom->b_blinkEnabled		= p_blinkConfig_ext->b_blinkEnabled;
		blinkConfig_mom->ui_blinkBrightness	= p_blinkConfig_ext->ui_blinkBrightness;
		blinkConfig_mom->ui_blinkDuration	= p_blinkConfig_ext->ui_blinkDuration;
		blinkConfig_mom->ui_blinkFrequency	= p_blinkConfig_ext->ui_blinkFrequency;
		blinkConfig_mom->ui_blinkPeriod		= p_blinkConfig_ext->ui_blinkPeriod;
		free(p_blinkConfig_ext);

		ledcTimerConfig_mom->duty_resolution = LEDC_TIMER_10_BIT; // resolution of PWM duty
		ledcTimerConfig_mom->freq_hz = blinkConfig_mom->ui_blinkFrequency;                    // frequency of PWM signal
		ledcTimerConfig_mom->speed_mode = LEDC_HS_MODE;           // timer mode
		ledcTimerConfig_mom->timer_num = LEDC_HS_TIMER;           // timer index
		ledcTimerConfig_mom->clk_cfg = LEDC_AUTO_CLK;              // Auto select the source clock
		ledc_timer_config(ledcTimerConfig_mom);

		xQueueReceive(q_pointer, &p_blinkConfig_ext, portMAX_DELAY);
		break;
	case CMD_svbl:
		xQueueSend(q_pointer,&blinkConfig_mom, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));
		break;
	case CMD_blnk:
		ledc_set_duty(ledcChannelConfig_mom->speed_mode, ledcChannelConfig_mom->channel, blinkConfig_mom->ui_blinkBrightness);
		ledc_update_duty(ledcChannelConfig_mom->speed_mode, ledcChannelConfig_mom->channel);
		vTaskDelay(blinkConfig_mom->ui_blinkDuration / portTICK_PERIOD_MS);
		ledc_set_duty(ledcChannelConfig_mom->speed_mode, ledcChannelConfig_mom->channel, 0);
		ledc_update_duty(ledcChannelConfig_mom->speed_mode, ledcChannelConfig_mom->channel);
		break;
	case CMD_bper:
		xQueueReceive(q_pointer, &(blinkConfig_mom->ui_blinkPeriod), portMAX_DELAY);
		break;
	case CMD_bdur:
		xQueueReceive(q_pointer, &(blinkConfig_mom->ui_blinkDuration), portMAX_DELAY);
		break;
	case CMD_bbrt:
		xQueueReceive(q_pointer, &(blinkConfig_mom->ui_blinkBrightness), portMAX_DELAY);
		break;
	case CMD_bfrq:
		xQueueReceive(q_pointer, &(blinkConfig_mom->ui_blinkFrequency), portMAX_DELAY);
		ledcTimerConfig_mom->freq_hz = blinkConfig_mom->ui_blinkFrequency;
		ledc_timer_config(ledcTimerConfig_mom);
		ledc_channel_config(ledcChannelConfig_mom);
		break;
	default:
		break;
	}
}

void callback_timerBlink(void* arg)
{
	while(xTaskNotify(ht_blinkRun,CMD_fire,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
}




