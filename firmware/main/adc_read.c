/*
 * adc_read.c
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#include "adc_read.h"

void t_ADCInit (void *arg)
{
	//loadAdcConf();
	struct_ADCConf.d_calValue = 1;
	struct_ADCConf.ul_period = 1000000;
	struct_ADCConf.ui_tareValue = 0;

	//Creates Periodic Timer for Reading ADC
	const esp_timer_create_args_t periodic_timer_args = {
				.callback = &periodic_timer_callback,
				.name = "periodic"
		};
	esp_timer_create(&periodic_timer_args, &periodic_timer);

	xHandle = NULL;
	xTaskCreate(t_ADCidle, "t_ADCidle", 2048,NULL, 10, NULL);

	vTaskDelete(NULL);
}

void t_ADCconfig (void *arg)
{
	printf("adcconfig\n");
	double d_cal = 0.0;
	if (xHandle != NULL)
	{
		vTaskSuspend( xHandle );
	}
	uint32_t i_cmdlet= 0;
	xQueueReceive(q_cmdlet, &i_cmdlet, portMAX_DELAY);

	switch (i_cmdlet)
	{
	case CMD_tare:
		struct_ADCConf.ui_tareValue = tareADC();
		char mystring[10] = "";
		sprintf((char*)mystring, "ack\n");
		xQueueSend(q_tcpConf, &mystring, 0);
		xEventGroupSync(eg_tcp, BIT_TCPQUEUE_READY, BIT_TCPQUEUE_READY,portMAX_DELAY);
		if (xHandle != NULL)
		{
			vTaskResume(xHandle);
			esp_timer_start_periodic(periodic_timer, struct_ADCConf.ul_period);
		}
		break;
	case CMD_cali:
		printf("calibrating\n");
		xQueueReceive(q_conf, &d_cal, portMAX_DELAY);
		struct_ADCConf.d_calValue = calADC(d_cal);
		printf("d_calValue:%lf\n", struct_ADCConf.d_calValue);
		if (xHandle != NULL)
		{
			vTaskResume(xHandle);
			esp_timer_start_periodic(periodic_timer, struct_ADCConf.ul_period);
		}
		break;
	case CMD_strt:
		printf("Starting..\n");
		if (xHandle == NULL)
		{
			xTaskCreate(t_ADCrun, "t_ADCrun", 2048,NULL, 10, &xHandle);
			esp_timer_start_periodic(periodic_timer, struct_ADCConf.ul_period);
		}
		else
		{
			vTaskResume(xHandle);
		}
		break;
	case CMD_mper:
		printf("Changing Period\n");
		xQueueReceive(q_conf, &struct_ADCConf.ul_period, portMAX_DELAY);
		if (struct_ADCConf.ul_period <= 100000)
		{
			setADCSpeed(1);
		}
		else
		{
			setADCSpeed(0);
		}
		if (xHandle != NULL)
		{
			vTaskResume(xHandle);
			esp_timer_start_periodic(periodic_timer, struct_ADCConf.ul_period);
		}
		break;
	default:
		printf("inv\n");
		break;
	}
	xTaskCreate(t_ADCidle, "t_ADCidle", 1024,NULL, 10, NULL);
	vTaskDelete(NULL);
}

void t_ADCidle (void *arg)
{
	xEventGroupWaitBits(eg_adc, BIT_ADC_BREAK, true, true, portMAX_DELAY);
	esp_timer_stop(periodic_timer);
	printf("breaking\n");
	xTaskCreate(t_ADCconfig, "t_ADCconfig", 2048, NULL, 10, NULL);
	vTaskDelete(NULL);
}

void t_ADCrun(void *arg)
{
	while (1)
	{
		xEventGroupWaitBits(eg_adc,BIT_ADC_FIRE,true,true,portMAX_DELAY);
		uint64_t time_since_boot = esp_timer_get_time();
		xQueueSend(q_time, &time_since_boot,  0);
		double d_momValue = ((double) readAdc() - struct_ADCConf.ui_tareValue) / struct_ADCConf.d_calValue;
		xQueueSend(q_measurement, &d_momValue,  0);
		xEventGroupSetBits(eg_tcp, BIT_SENDMES);
	}

}

uint32_t readAdc ()
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
	return count;
}

uint32_t tareADC()
{
	uint32_t ui_tareValue = 0;
	setADCSpeed(0);
	for (int i = 0 ; i < ADC_TARE_CYCLES ; i++)
	{
		ui_tareValue += readAdc();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	ui_tareValue /= ADC_TARE_CYCLES;
	return ui_tareValue;
}

double calADC(double d_calWheight)
{
	double d_calFactor = 0.0;
	double d_momValue = 0.0;
	setADCSpeed(0);
	for (int i = 0 ; i < ADC_CAL_CYCLES ; i++)
	{
		d_momValue += (double) readAdc() - struct_ADCConf.ui_tareValue;
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	d_momValue /= ADC_CAL_CYCLES;
	d_calFactor = d_momValue / d_calWheight;
	printf("d_calValue:%lf\n", d_calFactor);
	return d_calFactor;
}

void setADCSpeed(uint32_t ui_ADCSpeed)
{
	if (ui_ADCSpeed == 1)
	{
		gpio_set_level(GPIO_HX711_RATE_PIN, 0);
	}
	else
	{
		gpio_set_level(GPIO_HX711_RATE_PIN, 1);
	}
}

void loadAdcConf(void)
{
	//Loading Config
	nvs_open("nvs_ADC", NVS_READWRITE, &h_nvs_ADC);
	nvs_get_u32(h_nvs_ADC, "k_tareValue", &struct_ADCConf.ui_tareValue);
	uint64_t ul_momCalValue = 0;
	nvs_get_u64(h_nvs_ADC, "k_tareValue", &ul_momCalValue);
	struct_ADCConf.d_calValue = (double) ul_momCalValue / 10000;
	nvs_close(h_nvs_ADC);
}

void saveAdcConf(void)
{
	//Saving Config
	nvs_open("nvs_ADC", NVS_READWRITE, &h_nvs_ADC);
	nvs_set_u32(h_nvs_ADC, "k_tareValue", struct_ADCConf.ui_tareValue);
	uint64_t ul_momCalValue = struct_ADCConf.d_calValue * 10000;
	nvs_set_u64(h_nvs_ADC, "k_calValue", ul_momCalValue);
	nvs_commit(h_nvs_ADC);
	nvs_close(h_nvs_ADC);
}

void periodic_timer_callback(void* arg)
{

	xEventGroupSetBits(eg_adc, BIT_ADC_FIRE);
}



