/*
 * batmon.c
 *
 *  Created on: Oct 17, 2020
 *      Author: lor
 */

#include "batmon.h"

adc_oneshot_unit_handle_t adc1_handle;

void fbatMonInit(void)
{
    //-------------ADC1 Init---------------//

    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, BATMON_CHANNEL, &config));


    //-------------Starting Task---------------//
	xTaskCreate(tbatmonRun,		"t_batmonRun",		2048, NULL, 10, &ht_batmonRun);
}

void tbatmonRun	(void* param)
{
	uint32_t ui_cmdlet = 0;
	float f_batVol = 0.0;
	int		i_batRaw = 0;
	static int adc_raw[2][10];
	char *message = NULL;
	while(ui_cmdlet != CMD_init)
	{
		xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		fconfigBatMon(ui_cmdlet);
	}

	vTaskDelay(5000 / portTICK_PERIOD_MS);
	h_timerBatMon = xTimerCreate("timerBlink",BATMON_PERIOD / portTICK_PERIOD_MS,pdTRUE,NULL,callback_timerBatmon);

	while(1)
	{
		xTimerStart(h_timerBatMon, portMAX_DELAY);
		do
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			if(ui_cmdlet == CMD_fire)
			{
				for (int i = 0; i < BATMON_NUM_SAMPLES; i++)
				{
					//TODO: rewrite Batmon mmeasuring for 5.0 adc oneshot
					//f_batVol += fmap(adc1_get_raw(BATMON_CHANNEL),pstu_batMonConfig->i_batRawLow, pstu_batMonConfig->f_batVolLow, pstu_batMonConfig->i_batRawHigh, pstu_batMonConfig->f_batVolHigh);
					i_batRaw += adc_oneshot_read(adc1_handle, BATMON_CHANNEL, &adc_raw[0][0]);
				}
				f_batVol /= BATMON_NUM_SAMPLES;
				i_batRaw /= BATMON_NUM_SAMPLES;
				message = malloc(30+1);
				sprintf(message, "|bvol|%.*f|\t\tBattery voltage|\n",2, f_batVol);
				xQueueSend(q_tcpMessages,&message, portMAX_DELAY);
				while(xTaskNotify(ht_tcpConf, CMD_fire,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
			}
		}while(ui_cmdlet == CMD_fire);
		fconfigBatMon(ui_cmdlet);
		xTimerStop(h_timerBatMon, portMAX_DELAY);
	}
}

void callback_timerBatmon	(void* arg)
{
	while(xTaskNotify(ht_batmonRun,CMD_fire,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
}

void fconfigBatMon	(uint32_t ui_cmdlet)
{
	switch(ui_cmdlet)
	{
	case CMD_bath:
		for (int i = 0; i < BATMON_NUM_SAMPLES; i++)
		{
			//TODO: rewrite battery read with 5.0 adc oneshot
			//pstu_batMonConfig->i_batRawHigh += adc1_get_raw(BATMON_CHANNEL);
		}
		pstu_batMonConfig->i_batRawHigh /= BATMON_NUM_SAMPLES;
		xEventGroupSync( eg_sync,
				BIT_BATMON_SYNC,
				BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_BATMON_SYNC,
				BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_batl:
		for (int i = 0; i < BATMON_NUM_SAMPLES; i++)
		{
			//pstu_batMonConfig->i_batRawLow += adc1_get_raw(BATMON_CHANNEL);
		}
		pstu_batMonConfig->i_batRawLow /= BATMON_NUM_SAMPLES;
		xEventGroupSync( eg_sync,
				BIT_BATMON_SYNC,
				BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_BATMON_SYNC,
				BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );

		break;

	case CMD_init:
		xEventGroupSync( eg_config,
				BIT_BATMON_SYNC,
				BIT_BATMON_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_BATMON_SYNC ,
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
		break;
	}
}

float fmap	(int x,
			int us_batRawLow,
			float f_batVolLow,
			int us_batRawHigh,
			float f_batVolHigh)
{
	return (x - us_batRawLow) * (f_batVolHigh - f_batVolLow) / (us_batRawHigh - us_batRawLow) + f_batVolLow;
}

