/*
 * status_led.c
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#include "status_led.h"

void status_led	(void *param)	//TODO: defining different stati
{
	int ch = 0;
	ledc_timer_config_t ledc_timer = {
			.duty_resolution = LEDC_TIMER_13_BIT, 		// resolution of PWM duty
			.freq_hz = LEDC_FREQ_STATUS,             	// frequency of PWM signal
			.speed_mode = LEDC_HS_MODE,           		// timer mode
			.timer_num = LEDC_HS_TIMER,            		// timer index
			.clk_cfg = LEDC_AUTO_CLK,              		// Auto select the source clock
	};
	ledc_timer_config(&ledc_timer);
	ledc_timer.speed_mode = LEDC_HS_MODE;
	ledc_timer.timer_num = LEDC_HS_TIMER;
	ledc_timer_config(&ledc_timer);
	ledc_channel_config_t ledc_channel[3] = {
			{
					.channel    = LEDC_CH_RED,
					.duty       = 0,
					.gpio_num   = LEDC_GPIO_RED,
					.speed_mode = LEDC_HS_MODE,
					.hpoint     = 0,
					.timer_sel  = LEDC_HS_TIMER
			},
			{
					.channel    = LEDC_CH_GREEN,
					.duty       = 0,
					.gpio_num   = LEDC_GPIO_GREEN,
					.speed_mode = LEDC_HS_MODE,
					.hpoint     = 0,
					.timer_sel  = LEDC_HS_TIMER
			},
			{
					.channel    = LEDC_CH_BLUE,
					.duty       = 0,
					.gpio_num   = LEDC_GPIO_BLUE,
					.speed_mode = LEDC_HS_MODE,
					.hpoint     = 0,
					.timer_sel  = LEDC_HS_TIMER
			},
	};
	for (ch = 0; ch < 3; ch++) {
		ledc_channel_config(&ledc_channel[ch]);
	}

#if LEDC_GPIO_INV_RED == 1
	GPIO.func_out_sel_cfg[LEDC_GPIO_RED].inv_sel = 1;
#endif
#if LEDC_GPIO_INV_GREEN == 1
	GPIO.func_out_sel_cfg[LEDC_GPIO_GREEN].inv_sel = 1;
#endif
#if LEDC_GPIO_INV_BLUE == 1
	GPIO.func_out_sel_cfg[LEDC_GPIO_BLUE].inv_sel = 1;
#endif

	while(1)
	{

		if(q_rgb_status != NULL)
		{
			int element = 0;
			xQueueReceive(q_rgb_status, &element, portMAX_DELAY);
			switch (element)
			{
			case LEDC_STATUS_IDLE:
				ledc_set_duty(ledc_channel[LEDC_CH_RED].speed_mode, ledc_channel[LEDC_CH_RED].channel, 0);
				ledc_set_duty(ledc_channel[LEDC_CH_GREEN].speed_mode, ledc_channel[LEDC_CH_GREEN].channel, 0);
				ledc_set_duty(ledc_channel[LEDC_CH_BLUE].speed_mode, ledc_channel[LEDC_CH_BLUE].channel, 0);
				for (int i = 0; i< 3; i++)
				{
					ledc_update_duty(ledc_channel[i].speed_mode, ledc_channel[i].channel);
				}
				break;
			case LEDC_STATUS_WAITING_SERIAL:
				ledc_set_duty(ledc_channel[LEDC_CH_RED].speed_mode, ledc_channel[LEDC_CH_RED].channel, 0);
				ledc_set_duty(ledc_channel[LEDC_CH_GREEN].speed_mode, ledc_channel[LEDC_CH_GREEN].channel, 1000);
				ledc_set_duty(ledc_channel[LEDC_CH_BLUE].speed_mode, ledc_channel[LEDC_CH_BLUE].channel, 0);
				for (int i = 0; i< 3; i++)
				{
					ledc_update_duty(ledc_channel[i].speed_mode, ledc_channel[i].channel);
				}
				break;
			case LEDC_STATUS_WAITING_SERVER:
				ledc_set_duty(ledc_channel[LEDC_CH_RED].speed_mode, ledc_channel[LEDC_CH_RED].channel, 0);
				ledc_set_duty(ledc_channel[LEDC_CH_GREEN].speed_mode, ledc_channel[LEDC_CH_GREEN].channel, 0);
				ledc_set_duty(ledc_channel[LEDC_CH_BLUE].speed_mode, ledc_channel[LEDC_CH_BLUE].channel, 0);
				for (int i = 0; i< 3; i++)
				{
					ledc_update_duty(ledc_channel[i].speed_mode, ledc_channel[i].channel);
				}
				break;
			default:
				break;
			}
		}
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void sendRgbLedStatus	(int i)
{
	xQueueSend(q_rgb_status, &i, 100/portTICK_PERIOD_MS);
}
