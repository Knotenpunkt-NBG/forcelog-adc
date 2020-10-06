/*
 * blink.c
 *
 *  Created on: Jun 22, 2020
 *      Author: lor
 */

#include "blink.h"


void t_blinkInit (void *arg)
{


	//Creates Periodic Timer for Reading ADC
	const esp_timer_create_args_t args_blinkTimer = {
			.callback = &callback_timerBlink,
			.name = "periodicBlink"
	};
	esp_timer_create(&args_blinkTimer, &h_timerBlink);
	/*struct_blinkConfig.ui_blinkDuration = 1000;
	struct_blinkConfig.ui_blinkPeriod = 2000;
	struct_blinkConfig.ui_blinkFrequency = 1000;
	struct_blinkConfig.b_blinkEnabled = 1;
	struct_blinkConfig.ui_blinkBrightness = 1023;*/

	struct_blinkConfig.ui_blinkFrequency = 1000;

	ledc_timer.duty_resolution = LEDC_TIMER_10_BIT; // resolution of PWM duty
	ledc_timer.freq_hz = struct_blinkConfig.ui_blinkFrequency;                    // frequency of PWM signal
	ledc_timer.speed_mode = LEDC_HS_MODE;           // timer mode
	ledc_timer.timer_num = LEDC_HS_TIMER;           // timer index
	ledc_timer.clk_cfg = LEDC_AUTO_CLK;              // Auto select the source clock
	ledc_timer_config(&ledc_timer);
	stu_blink_channel.channel    = LEDC_CH_BLINK;
	stu_blink_channel.duty       = 0;
	stu_blink_channel.gpio_num   = LEDC_GPIO_BLINK;
	stu_blink_channel.speed_mode = LEDC_HS_MODE;
	stu_blink_channel.hpoint     = 0;
	stu_blink_channel.timer_sel  = LEDC_HS_TIMER;
	ledc_channel_config(&stu_blink_channel);
	vTaskDelete(NULL);
}

void t_blinkRun(void *arg)
{
	xSemaphoreTake(hs_blinkConfig, portMAX_DELAY);
	uint64_t time_since_boot = 0;
	uint32_t ui_blinkBrightness = struct_blinkConfig.ui_blinkBrightness;
	uint32_t ui_blinkDuration = struct_blinkConfig.ui_blinkDuration;
	xSemaphoreGive(hs_blinkConfig);
	xEventGroupWaitBits(eg_blink, BIT_BLINK_WAIT, true, true, portMAX_DELAY);
	while (1)
	{
		xEventGroupWaitBits(eg_blink,BIT_BLINK_FIRE,true,true,portMAX_DELAY);
		ledc_set_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel, ui_blinkBrightness);
		ledc_update_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel);
		time_since_boot = esp_timer_get_time();
		xQueueSend(q_time_blink, &time_since_boot,  0);
		xEventGroupSetBits(eg_tcp, BIT_SENDBLINK);
		vTaskDelay(ui_blinkDuration / portTICK_PERIOD_MS);
		ledc_set_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel, 0);
		ledc_update_duty(stu_blink_channel.speed_mode, stu_blink_channel.channel);
	}
}

void loadBlinkConf(void)
{
	//Loading Config
	nvs_open("nvs_Blink", NVS_READWRITE, &h_nvs_Blink);
	nvs_get_u32(h_nvs_Blink, "k_blinkDuration", &struct_blinkConfig.ui_blinkDuration);
	nvs_get_u32(h_nvs_Blink, "k_blinkPeriod", &struct_blinkConfig.ui_blinkPeriod);
	nvs_get_u8(h_nvs_Blink, "k_blinkEnabled", &struct_blinkConfig.b_blinkEnabled);
	nvs_get_u32(h_nvs_Blink, "k_blinkBright", &struct_blinkConfig.ui_blinkBrightness);
	nvs_close(h_nvs_Blink);
}

void saveBlinkConf(void)
{
	//Saving Config
	nvs_open("nvs_Blink", NVS_READWRITE, &h_nvs_Blink);
	nvs_set_u32(h_nvs_Blink, "k_blinkDuration", struct_blinkConfig.ui_blinkDuration);
	nvs_set_u32(h_nvs_Blink, "k_blinkPeriod", struct_blinkConfig.ui_blinkPeriod);
	nvs_set_u8(h_nvs_Blink, "k_blinkEnabled", struct_blinkConfig.b_blinkEnabled);
	nvs_set_u32(h_nvs_Blink, "k_blinkBright", struct_blinkConfig.ui_blinkBrightness);
	nvs_commit(h_nvs_Blink);
	nvs_close(h_nvs_Blink);
}

void callback_timerBlink(void* arg)
{
	xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
}




