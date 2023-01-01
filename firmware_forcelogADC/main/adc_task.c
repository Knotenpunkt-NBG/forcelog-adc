/*
 * adc_read.c
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#include "adc.h"




void tadcRun(void* param)
{
	uint32_t ui_cmdlet = 0;
	int32_t i32_limit = 0;
	int32_t err = 0;
	stu_mesCell dataPoint =
	{
			.str_type = "data",
			.str_data[0] = '\0',
			.ul_time = 0
	};
	while(1)
	{
		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			fADCConfig(ui_cmdlet);
		}
		i32_limit = fgetLimit(pstu_sensorConfig->ui8_maxLoad, pstu_sensorConfig->i8_perOverload, pstu_sensorConfig->i_tareZero);
		ESP_LOGD(TAG_ADC, "LIMIT %d", i32_limit);
		xEventGroupSync( eg_sync, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		while((ui_cmdlet != CMD_trig) && (ui_cmdlet != CMD_stop))
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		}
		if (ui_cmdlet == CMD_trig)
			esp_timer_start_periodic(htim_periodicAdc, pstu_adcConfig->ul_adcPeriod);

		while (ui_cmdlet != CMD_stop)
		{
			dataPoint.ul_time = esp_timer_get_time();
			err = freadAdc(&dataPoint, &i32_limit);
			if (err != 0)
			{
				switch (err)
				{
					case -1://Limit has been reached

						ESP_LOGD(TAG_ADC, "LIMIT REACHED: %d", i32_limit);
						break;
					default:
						break;
				}
			}
			xQueueSendFromISR(q_measurements, &dataPoint,  0);
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			if((ui_cmdlet != CMD_fire) && (ui_cmdlet != CMD_stop))
			{
				fADCConfig(ui_cmdlet);
			}
		}
		esp_timer_stop(htim_periodicAdc);
	}

}


void fadcTimerCallback	(void* arg)
{
	while(xTaskNotify(ht_adcRun,CMD_fire,eSetValueWithoutOverwrite) != pdPASS);
}



//FUNCTIONS FOR ADC SIMULATION





