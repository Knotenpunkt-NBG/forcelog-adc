/*
 * adc_read.c
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#include "adc.h"

void fADCInit (void)
{
	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pin_bit_mask = GPIO_INPUT_ADC;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = GPIO_OUTPUT_ADC;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);
	//Creates Periodic Timer for Reading ADC
	const esp_timer_create_args_t stu_adcTimerArgs = {
			.callback = &fadcTimerCallback,
			.name = "ADCTimer"
	};
	esp_timer_create(&stu_adcTimerArgs, &htim_periodicAdc);
	xTaskCreate(tadcRun, "tadcRun", 2048, NULL, 10, &ht_adcRun);
}

//TODO: modularise adc functions to select different modules with sdkconfig.default

void tadcRun(void* param)
{
	uint32_t ui_cmdlet = 0;
	struct stu_mesCell dataPoint =
	{
			.b_type = TYPE_ADC,
			.d_measurement = 0.0,
			.ul_time = 0
	};
	while(1)
	{
		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			fadcConfig(ui_cmdlet);
		}
		xEventGroupSync( eg_sync, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		while((ui_cmdlet != CMD_trig) && (ui_cmdlet != CMD_stop))
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		}
		if (ui_cmdlet == CMD_trig)
			esp_timer_start_periodic(htim_periodicAdc, gstu_config->adc.ul_adcPeriod);

		while (ui_cmdlet != CMD_stop)
		{
			dataPoint.ul_time = esp_timer_get_time();
			dataPoint.d_measurement = ((double) freadAdc() - gstu_config->cell.ui_tareValue);
			dataPoint.d_measurement *= gstu_config->cell.d_calValue;
			xQueueSendFromISR(q_measurements, &dataPoint,  0);
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			if((ui_cmdlet != CMD_fire) && (ui_cmdlet != CMD_stop))
			{
				fadcConfig(ui_cmdlet);
			}
		}
		esp_timer_stop(htim_periodicAdc);
	}

}

uint32_t freadAdc ()
{
	uint32_t count = 0;
	gpio_set_level(GPIO_HX711_SCK_PIN, 0);
	while(gpio_get_level(GPIO_HX711_DT_PIN))
	{
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
	for (int i = 0; i<24; i++)
	{
		gpio_set_level(GPIO_HX711_SCK_PIN, 1);
		count = count << 1;
		gpio_set_level(GPIO_HX711_SCK_PIN, 0);
		if (gpio_get_level(GPIO_HX711_DT_PIN) == 1)
		{
			count ++;
		}
	}
	gpio_set_level(GPIO_HX711_SCK_PIN, 1);
	count = count ^ 0x800000;
	gpio_set_level(GPIO_HX711_SCK_PIN, 0);
	ESP_LOGV(TAG_ADC, "RAW ADC:%d\n", count);
	return count;
}



uint32_t ftareADC()
{
	uint32_t ui_tareValue = 0;
	fsetADCSpeed(100000);
	for (int i = 0 ; i < ADC_TARE_CYCLES ; i++)
	{
		ui_tareValue += freadAdc();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	ui_tareValue /= ADC_TARE_CYCLES;
	return ui_tareValue;
}


double fcalADC	(double d_calWheight,
				uint32_t ui_tareValue)
{
	double d_calValue = 0.0;
	uint32_t ui_momValue = 0;
	for (int i = 0 ; i < ADC_CAL_CYCLES ; i++)
	{
		ui_momValue += freadAdc() - ui_tareValue;
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	ui_momValue /= ADC_CAL_CYCLES;
	d_calValue = d_calWheight /  (double)ui_momValue;
	return d_calValue;
}

int fsetADCSpeed	(uint64_t ui_adcPeriod)
{
	bool b_flag = 1;
	if (ui_adcPeriod < ADC_MIN_PERIOD)
	{
		b_flag = 0;
	}
	else
	{
		b_flag = 1;
		if (ui_adcPeriod > 10000)
		{
			gpio_set_level(GPIO_HX711_RATE_PIN, 0);
		}
		else
		{
			gpio_set_level(GPIO_HX711_RATE_PIN, 1);
		}
	}
	return b_flag;
}

void fadcTimerCallback	(void* arg)
{
	while(xTaskNotify(ht_adcRun,CMD_fire,eSetValueWithoutOverwrite) != pdPASS);
}

void fadcConfig	(uint32_t CMDlet)
{
	double* pd_calWeight = 0;

	switch (CMDlet)
	{
	case 0:
		break;
	case CMD_setc:
	case CMD_sesd:
	case CMD_retc:
	case CMD_resd:
	case CMD_ldad:
		xEventGroupSync( eg_sync, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		xEventGroupSync( eg_config, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		break;

	case CMD_ldlc:
		ESP_LOGD(TAG_ADC, "LOADING");
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_STORAGE_SYNC,
				portMAX_DELAY );
		ESP_LOGD(TAG_ADC, "SYNCING");
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_STORAGE_SYNC,
				portMAX_DELAY );
		ESP_LOGD(TAG_ADC, "FINISHED LOADING");
		break;
	case CMD_init:
		xEventGroupSync( eg_config,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC ,
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
	case CMD_tare:
		gstu_config->cell.ui_tareValue = ftareADC();
		xEventGroupSync( eg_sync, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		xEventGroupSync( eg_config, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		break;
	case CMD_cali:
		xQueueReceive(q_send, &pd_calWeight, portMAX_DELAY);
		fsetADCSpeed(ADC_CAL_PERIOD);
		gstu_config->cell.d_calValue = fcalADC(*pd_calWeight, gstu_config->cell.ui_tareValue);
		free(pd_calWeight);
		fsetADCSpeed(gstu_config->adc.ul_adcPeriod);

//		while(xTaskNotify(ht_adcRun,CMD_svlc,eSetValueWithoutOverwrite) != pdPASS)
//			vTaskDelay(1/ portTICK_PERIOD_MS);
//		xEventGroupSync( eg_sync, NULL, BIT_STORAGE_SYNC, portMAX_DELAY );

		xEventGroupSync( eg_sync, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		break;

	case CMD_mper:
		xEventGroupSync( eg_sync, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		xEventGroupSync( eg_config, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		if (esp_timer_stop(htim_periodicAdc) == ESP_OK)
			esp_timer_start_periodic(htim_periodicAdc, gstu_config->adc.ul_adcPeriod);
		break;

	default:
		ESP_LOGW(TAG_ADC, "WRONG CMDLET");
		break;
	}
}

//FUNCTIONS FOR ADC SIMULATION

void fsimADCInit(void)
{
	//Creates Periodic Timer for Reading ADC
	const esp_timer_create_args_t stu_adcTimerArgs = {
			.callback = &fadcTimerCallback,
			.name = "ADCTimer"
	};
	esp_timer_create(&stu_adcTimerArgs, &htim_periodicAdc);
	xTaskCreate(tsimADCRun, "tsimdADCRun", 2048, NULL, 10, &ht_adcRun);
}

uint32_t fsimReadADC(void)
{
	uint64_t ul_time = esp_timer_get_time();
	ul_time %= 5000000;
	ul_time /= 5;
	return (uint32_t) ul_time;
}

void tsimADCRun (void* param)
{
	uint32_t ui_cmdlet = 0;
	struct stu_mesCell dataPoint =
	{
			.b_type = TYPE_ADC,
			.d_measurement = 0,
			.ul_time = 0
	};
	while(1)
	{
		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			fsimADCConfig(ui_cmdlet);
		}
		xEventGroupSync( eg_sync, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		while((ui_cmdlet != CMD_trig) && (ui_cmdlet != CMD_stop))
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		}
		if (ui_cmdlet == CMD_trig)
			esp_timer_start_periodic(htim_periodicAdc, gstu_config->adc.ul_adcPeriod);

		while (ui_cmdlet != CMD_stop)
		{
			dataPoint.ul_time = esp_timer_get_time();
			dataPoint.d_measurement = sin(((double)fsimReadADC()/1000000)*3.14*2) * gstu_config->cell.d_calValue - gstu_config->cell.ui_tareValue;
			xQueueSendFromISR(q_measurements, &dataPoint,  0);
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			if((ui_cmdlet != CMD_fire) && (ui_cmdlet != CMD_stop))
			{
				fsimADCConfig(ui_cmdlet);
			}
		}
		esp_timer_stop(htim_periodicAdc);
	}

}

void fsimADCConfig	(uint32_t CMDlet)
{
	double* pd_calWeight = 0;

	switch (CMDlet)
	{
	case 0:
		break;
	case CMD_setc:
	case CMD_sesd:
	case CMD_retc:
	case CMD_resd:
	case CMD_ldad:
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_ldlc:
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_STORAGE_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_STORAGE_SYNC,
				portMAX_DELAY );
		break;

	case CMD_init:
		xEventGroupSync( eg_config,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC ,
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
	case CMD_tare:
		gstu_config->cell.ui_tareValue += 1;
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_config,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;
	case CMD_cali:
		xQueueReceive(q_send, &pd_calWeight, portMAX_DELAY);
		gstu_config->cell.d_calValue = *pd_calWeight;
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		free(pd_calWeight);

		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_mper:
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_config,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		if (esp_timer_stop(htim_periodicAdc) == ESP_OK)
			esp_timer_start_periodic(htim_periodicAdc, gstu_config->adc.ul_adcPeriod);
		break;

	default:
		ESP_LOGW(TAG_ADC, "WRONG CMDLET");
		break;
	}
}
