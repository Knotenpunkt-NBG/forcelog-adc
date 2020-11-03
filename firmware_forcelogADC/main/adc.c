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
//TODO: Change event groups to direct to task notification

void tadcRun(void *arg)
{
	struct stu_adcConfig adcConfig_mom;
	struct stu_cellConfig cellConfig_mom;
	uint32_t ui_cmdlet = 0;
	double d_momValue = 0.0;
	uint64_t time_since_boot = 0;
	while(1)
	{
		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			fadcConfig(ui_cmdlet, &adcConfig_mom, &cellConfig_mom);
			vTaskDelay(1);
		}

		while((ui_cmdlet != CMD_trig) && (ui_cmdlet != CMD_stop))
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		}
		esp_timer_start_periodic(htim_periodicAdc, adcConfig_mom.ul_adcPeriod);

		while (ui_cmdlet != CMD_stop)
		{
			time_since_boot = esp_timer_get_time();
			d_momValue = ((double) freadAdc() - cellConfig_mom.ui_tareValue);
			d_momValue *= cellConfig_mom.d_calValue;
			if(adcConfig_mom.b_sendTCP)
			{
				xQueueSend(q_time_mes_tcp, &time_since_boot,  0);
				xQueueSend(q_value_mes_tcp, &d_momValue,  0);
				while(xTaskNotify(ht_tcpMes,CMD_MEAS_READY,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);

			}
			if(adcConfig_mom.b_sendSD)
			{
				//TODO: make queues for sd
			}
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			if((ui_cmdlet != CMD_fire) && (ui_cmdlet != CMD_stop))
			{
				fadcConfig(ui_cmdlet, &adcConfig_mom, &cellConfig_mom);
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

double fcalADC(double d_calWheight, uint32_t ui_tareValue)
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
	while(xTaskNotify(ht_adcRun,BIT_ADC_FIRE,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
}

void fadcConfig(uint32_t CMDlet, struct stu_adcConfig *adcConfig_mom, struct stu_cellConfig *cellConfig_mom)
{
	struct stu_adcConfig* p_adcConfig_ext = 0;
	struct stu_cellConfig* p_cellConfig_ext = 0;
	uint64_t* ptr_ull = 0;
	double* pd_calWeight = 0;

	switch (CMDlet)
	{
	case CMD_setc:
		adcConfig_mom->b_sendTCP = true;
		break;
	case CMD_sesd:
		adcConfig_mom->b_sendSD = true;
		break;
	case CMD_retc:
		adcConfig_mom->b_sendTCP = false;
		break;
	case CMD_resd:
		adcConfig_mom->b_sendSD = false;
		break;
	case CMD_tare:
		cellConfig_mom->ui_tareValue = ftareADC();
		break;
	case CMD_cali:
		xQueueReceive(q_pointer, &pd_calWeight, portMAX_DELAY);
		fsetADCSpeed(ADC_CAL_PERIOD);
		cellConfig_mom->d_calValue = fcalADC(*pd_calWeight, cellConfig_mom->ui_tareValue);
		free(pd_calWeight);
		fsetADCSpeed(adcConfig_mom->ul_adcPeriod);
		xQueueSend(q_pointer, &cellConfig_mom, portMAX_DELAY);
		break;
	case CMD_vcal:
		break;
	case CMD_mper:
		xQueueReceive(q_pointer, &ptr_ull, portMAX_DELAY);
		adcConfig_mom->ul_adcPeriod = *ptr_ull;
		free(ptr_ull);
		break;
	case CMD_info:
		break;
	case CMD_svad:
		xQueueSend(q_pointer,&adcConfig_mom, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));
		break;
	case CMD_svlc:
		xQueueSend(q_pointer,&cellConfig_mom, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));
		break;
	case CMD_ldlc:
		xQueuePeek(q_pointer, &p_cellConfig_ext, portMAX_DELAY);
		cellConfig_mom->d_calValue		= p_cellConfig_ext->d_calValue;
		cellConfig_mom->ui_tareValue	= p_cellConfig_ext->ui_tareValue;
		free(p_cellConfig_ext);
		xQueueReceive(q_pointer, &p_cellConfig_ext, portMAX_DELAY);
		break;
	case CMD_load:
		xQueuePeek(q_pointer, &p_adcConfig_ext, portMAX_DELAY);
		adcConfig_mom->b_sendSD		= p_adcConfig_ext->b_sendSD;
		adcConfig_mom->b_sendTCP	= p_adcConfig_ext->b_sendTCP;
		adcConfig_mom->ul_adcPeriod	= p_adcConfig_ext->ul_adcPeriod;
		free(p_adcConfig_ext);
		xQueueReceive(q_pointer, &p_adcConfig_ext, portMAX_DELAY);
		break;

	default:
		break;
	}
	ESP_LOGD(TAG_ADC, "LEAVING ADC CONFIG");
}

