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
	struct_blinkConfig.ui_blinkDuration = 1000;
	struct_blinkConfig.ui_blinkPeriod = 2000;
	struct_blinkConfig.ui_blinkFrequency = 1000;
	struct_blinkConfig.b_blinkEnabled = 1;

	ledc_timer.duty_resolution = LEDC_TIMER_10_BIT; // resolution of PWM duty
	ledc_timer.freq_hz = struct_blinkConfig.ui_blinkFrequency;                    // frequency of PWM signal
	ledc_timer.speed_mode = LEDC_HS_MODE;           // timer mode
	ledc_timer.timer_num = LEDC_HS_TIMER;           // timer index
	ledc_timer.clk_cfg = LEDC_AUTO_CLK;              // Auto select the source clock
	ledc_timer_config(&ledc_timer);
	ledc_channel.channel    = LEDC_CH_BLINK;
	ledc_channel.duty       = 0;
	ledc_channel.gpio_num   = LEDC_GPIO_BLINK;
	ledc_channel.speed_mode = LEDC_HS_MODE;
	ledc_channel.hpoint     = 0;
	ledc_channel.timer_sel  = LEDC_HS_TIMER;
	ledc_channel_config(&ledc_channel);

	ht_blinkRun = NULL;
	xTaskCreate(t_blinkIdle, "t_blinkIdle", 2048,NULL, 10, NULL);

	vTaskDelete(NULL);
}

void t_blinkConfig (void *arg)
{
	if (ht_blinkRun != NULL)
	{
		vTaskSuspend( ht_blinkRun );
	}
	uint32_t i_cmdlet= 0;
	xQueueReceive(q_cmdlet, &i_cmdlet, portMAX_DELAY);
	switch (i_cmdlet)
	{
	case CMD_stop:
		struct_blinkConfig.b_isRunning = 0;
		break;

	case CMD_strt:
		if (struct_blinkConfig.b_blinkEnabled)
		{
			struct_blinkConfig.b_isRunning = 1;
			if (ht_blinkRun == NULL)
			{
				xTaskCreate(t_blinkRun, "t_blinkRun", 2048,NULL, 10, &ht_blinkRun);
				esp_timer_start_periodic(h_timerBlink, struct_blinkConfig.ui_blinkPeriod * 1000);
				xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
			}
			else
			{
				vTaskResume(ht_blinkRun);
				esp_timer_start_periodic(h_timerBlink, struct_blinkConfig.ui_blinkPeriod * 1000);
				xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
			}
		}
		break;

	case CMD_benb: //Enables Blink
		struct_blinkConfig.b_blinkEnabled = 1;
		break;

	case CMD_bdis: //Disables Blink
		struct_blinkConfig.b_blinkEnabled = 0;
		break;

	case CMD_bper: //sets Blink Period
		//const char mystring[] = "ack\n";
		//sprintf((char*)mystring, "ack\n");
		//xQueueSend(q_tcpConf, &mystring, 0);
		xEventGroupSync(eg_adc, BIT_TCPQUEUE_READY, BIT_TCPQUEUE_READY,portMAX_DELAY);
		xQueueReceive(q_conf, &struct_blinkConfig.ui_blinkPeriod, portMAX_DELAY);
		if (struct_blinkConfig.b_isRunning ==1)
		{
			esp_timer_start_periodic(h_timerBlink, struct_blinkConfig.ui_blinkPeriod * 1000);
			xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
		}
		break;

	case CMD_bdur: //sets blink duration
		xQueueReceive(q_conf, &struct_blinkConfig.ui_blinkDuration, portMAX_DELAY);
		if (struct_blinkConfig.b_isRunning ==1)
		{
			esp_timer_start_periodic(h_timerBlink, struct_blinkConfig.ui_blinkPeriod * 1000);
			xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
		}
		break;

	case CMD_bfrq: //sets blink pwm frequency
		xQueueReceive(q_conf, &struct_blinkConfig.ui_blinkFrequency, portMAX_DELAY);
		if (struct_blinkConfig.b_isRunning ==1)
		{
			esp_timer_start_periodic(h_timerBlink, struct_blinkConfig.ui_blinkPeriod * 1000);
			xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
		}
		break;

	case CMD_bbrt: //sets blink pwm duty cycle (brightness)
		xQueueReceive(q_conf, &struct_blinkConfig.ui_blinkBrightness, portMAX_DELAY);
		if (struct_blinkConfig.b_isRunning ==1)
		{
			esp_timer_start_periodic(h_timerBlink, struct_blinkConfig.ui_blinkPeriod * 1000);
			xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
		}
		break;

	case CMD_save: //saves config to non volatile storage
		saveBlinkConf();
		break;

	case CMD_load: //loads config from non volatile storage
		loadBlinkConf();
		if (struct_blinkConfig.b_isRunning ==1)
		{
			esp_timer_start_periodic(h_timerBlink, struct_blinkConfig.ui_blinkPeriod * 1000);
			xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
		}
		break;

	case CMD_bsht: //single shot blink without tcp message
		ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, struct_blinkConfig.ui_blinkBrightness);
		ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
		vTaskDelay(struct_blinkConfig.ui_blinkDuration / portTICK_PERIOD_MS);
		ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
		ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
		break;

	default:
		printf("inv\n");
		break;
	}

	xTaskCreate(t_blinkIdle, "t_blinkIdle", 1024,NULL, 10, NULL);
	vTaskDelete(NULL);
}

void t_blinkIdle (void *arg)
{
	xEventGroupWaitBits(eg_blink, BIT_BLINK_BREAK, true, true, portMAX_DELAY);
	esp_timer_stop(h_timerBlink);
	ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
	xTaskCreate(t_blinkConfig, "t_blinkConfig", 2048, NULL, 10, NULL);
	vTaskDelete(NULL);
}

void t_blinkRun(void *arg)
{
	uint64_t time_since_boot = 0;
	while (1)
	{
		xEventGroupWaitBits(eg_blink,BIT_BLINK_FIRE,true,true,portMAX_DELAY);
		ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, struct_blinkConfig.ui_blinkBrightness);
		ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
		time_since_boot = esp_timer_get_time();
		xQueueSend(q_time, &time_since_boot,  0);
		xEventGroupSetBits(eg_tcp, BIT_SENDBLINK);
		vTaskDelay(struct_blinkConfig.ui_blinkDuration / portTICK_PERIOD_MS);
		ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
		ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
	}
}

void loadBlinkConf(void)
{
	//Loading Config
	nvs_open("nvs_Blink", NVS_READWRITE, &h_nvs_Blink);
	nvs_get_u32(h_nvs_Blink, "k_blinkDuration", &struct_blinkConfig.ui_blinkDuration);
	nvs_get_u32(h_nvs_Blink, "k_blinkPeriod", &struct_blinkConfig.ui_blinkDuration);
	nvs_get_u8(h_nvs_Blink, "k_blinkEnabled", &struct_blinkConfig.b_blinkEnabled);
	nvs_close(h_nvs_Blink);
}

void saveBlinkConf(void)
{
	//Saving Config
	nvs_open("nvs_Blink", NVS_READWRITE, &h_nvs_Blink);
	nvs_set_u32(h_nvs_Blink, "k_blinkDuration", struct_blinkConfig.ui_blinkDuration);
	nvs_set_u32(h_nvs_Blink, "k_blinkPeriod", struct_blinkConfig.ui_blinkPeriod);
	nvs_set_u8(h_nvs_Blink, "k_blinkEnabled", struct_blinkConfig.b_blinkEnabled);
	nvs_commit(h_nvs_Blink);
	nvs_close(h_nvs_Blink);
}

void callback_timerBlink(void* arg)
{
	xEventGroupSetBits(eg_blink, BIT_BLINK_FIRE);
}




