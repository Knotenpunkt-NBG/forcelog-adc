/*
 * blink.c
 *
 *  Created on: Jun 22, 2020
 *      Author: lor
 */

#include "blink.h"


void fblinkInit (void)
{
	ESP_LOGI(TAG_BLINK, "STARTING t-blinkInit");

	stu_ledcBlinkTimerConfig.duty_resolution = LEDC_TIMER_10_BIT; // resolution of PWM duty
	stu_ledcBlinkTimerConfig.freq_hz = gui_blinkFrequency;                    // frequency of PWM signal
	stu_ledcBlinkTimerConfig.speed_mode = LEDC_HS_MODE;           // timer mode
	stu_ledcBlinkTimerConfig.timer_num = LEDC_HS_TIMER;           // timer index
	stu_ledcBlinkTimerConfig.clk_cfg = LEDC_AUTO_CLK;              // Auto select the source clock
	ledc_timer_config(&stu_ledcBlinkTimerConfig);

	stu_ledcBlinkChannel.channel    = LEDC_CH_BLINK;
	stu_ledcBlinkChannel.duty       = 0;
	stu_ledcBlinkChannel.gpio_num   = LEDC_GPIO_BLINK;
	stu_ledcBlinkChannel.speed_mode = LEDC_HS_MODE;
	stu_ledcBlinkChannel.hpoint     = 0;
	stu_ledcBlinkChannel.timer_sel  = LEDC_HS_TIMER;
	ledc_channel_config(&stu_ledcBlinkChannel);
	xTaskCreate(t_blinkRun,		"t_blinkRun",		2048, ht_blinkRun, 10, NULL);
}

void t_blinkRun(void *arg)
{
	while(1)
	{
		ledc_set_duty(stu_ledcBlinkChannel.speed_mode, stu_ledcBlinkChannel.channel, 0);
		xEventGroupWaitBits(eg_blink, BIT_BLINK_START, true, true, portMAX_DELAY);
		ESP_LOGD(TAG_BLINK, "STARTING t_blinkRun\n");
		h_timerBlink = xTimerCreate("timerBlink",gui_blinkPeriod / portTICK_PERIOD_MS,pdTRUE,NULL,callback_timerBlink);
		uint64_t time_since_boot = 0;
		uint32_t ui_blinkBrightness = gui_blinkBrightness;
		uint32_t ui_blinkDuration = gui_blinkDuration;
		ESP_LOGD(TAG_BLINK, "WAITING FOR TRIGGER\n");
		xTimerStart(h_timerBlink, portMAX_DELAY);
		xEventGroupWaitBits(eg_blink, BIT_BLINK_WAIT, true, true, portMAX_DELAY);
		xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
		while (1)
		{
			xEventGroupWaitBits(eg_blink,BIT_BLINK_FIRE,true,true,portMAX_DELAY);
			ledc_set_duty(stu_ledcBlinkChannel.speed_mode, stu_ledcBlinkChannel.channel, ui_blinkBrightness);
			ledc_update_duty(stu_ledcBlinkChannel.speed_mode, stu_ledcBlinkChannel.channel);
			time_since_boot = esp_timer_get_time();
			xQueueSend(q_time_blink, &time_since_boot,  0);
			xEventGroupSetBits(eg_tcp, BIT_TCPSEND);
			vTaskDelay(ui_blinkDuration / portTICK_PERIOD_MS);
			ledc_set_duty(stu_ledcBlinkChannel.speed_mode, stu_ledcBlinkChannel.channel, 0);
			ledc_update_duty(stu_ledcBlinkChannel.speed_mode, stu_ledcBlinkChannel.channel);
			if (!b_recording)
			{
				ESP_LOGD(TAG_BLINK, "STOPPING RECORDING\n");
				xEventGroupSetBits(eg_blink, BIT_BLINK_STOP);
				break;
			}
		}
	}
}


void callback_timerBlink(void* arg)
{
	xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
}




