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
	xTaskCreate(t_ADCrun, "t_ADCrun", 2048,ht_adcRun, 10, NULL);
}

//TODO: modularise adc functions to enable different modules with sdkconfig.default
//TODO: Change event groups to direct to task notification

void t_ADCrun(void *arg)
{
	while(1)
	{
		xEventGroupWaitBits(eg_adc, BIT_ADC_START, true, true, portMAX_DELAY);
		double d_momValue = 0.0;
		uint32_t ui_tareValue = gui_tareValue;
		double d_calValue = gd_calValue;
		uint64_t time_since_boot = 0;
		xEventGroupWaitBits(eg_adc, BIT_ADC_WAIT, true, true, portMAX_DELAY);
		esp_timer_start_periodic(htim_periodicAdc, gul_adcPeriod);
		xEventGroupSetBits(eg_adc, BIT_ADC_FIRE);
		ESP_LOGD(TAG_ADC, "STARTING RECORDING");
		while (1)
		{
			xEventGroupWaitBits(eg_adc,BIT_ADC_FIRE,true,true,portMAX_DELAY);
			time_since_boot = esp_timer_get_time();
			xQueueSend(q_time_mes, &time_since_boot,  0);
			d_momValue = ((double) freadAdc() - ui_tareValue);
			ESP_LOGV(TAG_ADC, "TARED VALUE:%f", d_momValue);
			d_momValue *= d_calValue;
			ESP_LOGV(TAG_ADC, "CALIBRATED VALUE:%f", d_momValue);
			xQueueSend(q_measurement, &d_momValue,  0);
			xEventGroupSetBits(eg_tcp, BIT_TCPSEND);
			if (!b_recording)
			{
				ESP_LOGD(TAG_ADC, "STOPPING RECORDING\n");
				xEventGroupSetBits(eg_adc, BIT_ADC_STOP);
				break;
			}
		}
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
	fsetADCSpeed(0);
	for (int i = 0 ; i < ADC_TARE_CYCLES ; i++)
	{
		gui_tareValue += freadAdc();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	ui_tareValue /= ADC_TARE_CYCLES;
	return ui_tareValue;
}

double fcalADC(double d_calWheight)
{
	double d_calValue = 0.0;
	double d_momValue = 0.0;
	uint32_t ui_momValue = 0;
	fsetADCSpeed(ADC_CAL_PERIOD);
	for (int i = 0 ; i < ADC_CAL_CYCLES ; i++)
	{
		ui_momValue += freadAdc() - gui_tareValue;
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	ui_momValue /= ADC_CAL_CYCLES;
	d_calValue = d_calWheight /  (double)ui_momValue;
	fsetADCSpeed(gul_adcPeriod);
	return d_calValue;
}

int fsetADCSpeed(uint64_t ui_adcPeriod)
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

void fadcTimerCallback(void* arg)
{
	xEventGroupSetBits(eg_adc, BIT_ADC_FIRE);
}



