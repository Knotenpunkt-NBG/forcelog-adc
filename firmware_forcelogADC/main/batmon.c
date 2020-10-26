/*
 * batmon.c
 *
 *  Created on: Oct 17, 2020
 *      Author: lor
 */

#include "batmon.h"

void fbatMonInit(void)
{
	adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
	xTaskCreate(t_batmonRun,		"t_batmonRun",		2048, ht_blinkRun, 10, NULL);
}

void t_batmonRun (void* param)
{
	float f_batVol = 0.0;
	int		i_batRaw = 0;
	char *message = malloc(30+1);
	vTaskDelay(2000 / portTICK_PERIOD_MS);
	h_timerBatMon = xTimerCreate("timerBlink",BATMON_PERIOD / portTICK_PERIOD_MS,pdTRUE,NULL,callback_timerBatmon);
	xTimerStart(h_timerBatMon, portMAX_DELAY);
	while(1)
	{
		float f_batVolLow = gf_batVolLow;
		float f_batVolHigh = gf_batVolHigh;
		int i_batRawLow = gi_batRawLow;
		int i_batRawHigh = gi_batRawHigh;
		while(1)
		{
			xEventGroupWaitBits(eg_batmon, BIT_BATMON_FIRE, true, true, portMAX_DELAY);
			for (int i = 0; i < BATMON_NUM_SAMPLES; i++)
			{
				f_batVol += map(adc1_get_raw(ADC1_CHANNEL_4),gi_batRawLow, gf_batVolLow, gi_batRawHigh, gf_batVolHigh);
				i_batRaw += adc1_get_raw(ADC1_CHANNEL_4);
			}
			f_batVol /= BATMON_NUM_SAMPLES;
			i_batRaw /= BATMON_NUM_SAMPLES;
			ESP_LOGV(TAG_BATMON, "Battery voltage:%f	raw:%d", f_batVol,i_batRaw);
			sprintf(message, "|bvol|%.*f|\t\tBattery voltage|\n",2, f_batVol);
			xQueueSend(q_tcpConf,&message, 0);
			xEventGroupSetBits(eg_tcp, BIT_TCPQUEUE_READY);
		}
	}
}

void callback_timerBatmon(void* arg)
{
	xEventGroupSetBits(eg_batmon, BIT_BATMON_FIRE);
}

float map(int x, int us_batRawLow, float f_batVolLow, int us_batRawHigh, float f_batVolHigh)
{
	return (x - us_batRawLow) * (f_batVolHigh - f_batVolLow) / (us_batRawHigh - us_batRawLow) + f_batVolLow;
}
