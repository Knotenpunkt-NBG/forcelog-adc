#include "adc_routines.h"

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

	gpio_set_level(GPIO_HX711_SCK_PIN, 1);
	gpio_set_level(GPIO_HX711_RATE_PIN, 0);

	xTaskCreate(tadcRun, "tadcRun", 2048, NULL, 10, &ht_adcRun);
	//Creates Periodic Timer for Reading ADC
	const esp_timer_create_args_t stu_adcTimerArgs = {
			.callback = &fadcTimerCallback,
			.name = "ADCTimer"
	};
	esp_timer_create(&stu_adcTimerArgs, &htim_periodicAdc);
}

int32_t fgetRawADC ()
{
	uint32_t count = 0;
	uint32_t ui_cyclecount = 0;
	while(gpio_get_level(GPIO_HX711_DT_PIN))
	{
		ESP_LOGD(TAG_ADC, "HX711 NOT READY %d", ui_cyclecount);
		vTaskDelay(1 / portTICK_PERIOD_MS);
		ui_cyclecount++;
	}
	ets_delay_us(2);
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
	portENTER_CRITICAL(&mux);
	for (int i = 0; i<24; i++)
	{
		gpio_set_level(GPIO_HX711_SCK_PIN, 0);
		ets_delay_us(10);
		gpio_set_level(GPIO_HX711_SCK_PIN, 1);

		count = count << 1;
		if (gpio_get_level(GPIO_HX711_DT_PIN) == 1)
		{
			count ++;
		}
	}
	portEXIT_CRITICAL(&mux);
	gpio_set_level(GPIO_HX711_SCK_PIN, 0);
	ets_delay_us(10);

	gpio_set_level(GPIO_HX711_SCK_PIN, 1);
	if (count & 0x00800000)
	{
		count |= 0xFF000000;
	}
	else
	{
		count &= 0x00FFFFFF;
	}
	return (int32_t) count;
}

int32_t		freadAdc		(stu_mesCell* dataPoint, int32_t* i32_limit)
{
	int32_t err = 0;
	int32_t count = fgetRawADC();
	if (abs(count) > *i32_limit)
	{
		ESP_LOGD(TAG_ADC, "count:%d, limit:%d", count, *i32_limit);
		err = -1;
	}
	double d_dataValue = (double)(count - pstu_sensorConfig->i_tareValue) * pstu_sensorConfig->d_calValue;
	sprintf(dataPoint->str_data, "%8.*f",pstu_adcConfig->uc_numDecimals, d_dataValue);
	return err;
}

int32_t fTareADC(void)
{
	int32_t i_tareValue = 0;
	for (int i = 0 ; i < ADC_TARE_CYCLES ; i++)
	{
		i_tareValue += fgetRawADC();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	i_tareValue /= ADC_TARE_CYCLES;
	return i_tareValue;
}

double fcalADC	(double d_calWheight,
				uint32_t i_tareValue)
{
	double d_calValue = 0.0;
	int32_t ui_momValue = 0;
	for (int i = 0 ; i < ADC_CAL_CYCLES ; i++)
	{
		ui_momValue += fgetRawADC();
		vTaskDelay(100 / portTICK_RATE_MS);
	}
	ui_momValue /= ADC_CAL_CYCLES;
	ui_momValue -= i_tareValue;
	d_calValue = d_calWheight / ui_momValue;
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
		if (ui_adcPeriod < 100000)
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

void fADCConfig	(uint32_t CMDlet)
{
	char* pc_configIn = 0;
	char* pc_configOut = 0;
	char* pc_value = 0;

	switch (CMDlet)
	{
	case 0:
		break;
	case CMD_sesd:
	case CMD_resd:
	case CMD_ldad:
		xEventGroupSync( eg_sync, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		xEventGroupSync( eg_config, BIT_ADC_SYNC, BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		break;

	case CMD_lmlc:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tEnter limit in g.\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			pc_configOut = malloc(64);
			uint32_t ui32_limitRaw = 0;
			ui32_limitRaw = strtol(pc_value, NULL, 10);
			uint32_t ui32_limitConverted = 0;
			for(uint32_t i = ui32_limitRaw; i >= 10; i/=10)
			{
				ui32_limitConverted += 1;
			}

			ui32_limitConverted += (ui32_limitRaw * 10 / ipow(10, ui32_limitConverted))* 20 / 10;
			pstu_sensorConfig->i8_perOverload = 20;
			pstu_sensorConfig->ui8_maxLoad = (uint8_t) ui32_limitConverted;

			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			xQueueSend(q_send,&ui32_limitConverted, portMAX_DELAY);
			while(xTaskNotify(ht_tempIntRun,CMD_lmlc,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			int32_t response = 0;
			xQueueReceive(q_recv, &response, portMAX_DELAY);
			if (!response)
			{
				sprintf(pc_configOut, "|lmlc|1|\t\tOK, limit set to %d|\n", ui32_limitConverted);
			}
			else
			{
				sprintf(pc_configOut, "|lmlc|1|\t\t Error writing to scratchpad %d|\n", response);
			}
			xSemaphoreGive(hs_pointerQueue);


		}
		else
			sprintf(pc_configOut,
					"|err|"STR(ERR_RECORDING)"|\t\tCould not find value in message(%s).\n",
					pc_value);


		fsendResponse(0, 1, pc_configOut);
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
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
		ESP_LOGD(TAG_ADC, "TARING");
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		esp_timer_stop(htim_periodicAdc);
		fsetADCSpeed(ADC_CAL_PERIOD);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		pstu_sensorConfig->i_tareValue = fTareADC();
		fsetADCSpeed(pstu_adcConfig->ul_adcPeriod);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		if (xEventGroupGetBits(eg_status) & STATUS_PEEKING)
			esp_timer_start_periodic(htim_periodicAdc, pstu_adcConfig->ul_adcPeriod);
		sprintf(pc_configOut, "|tare|1|\t\tOK, tare successful|\n");
		fsendResponse(0, 1, pc_configOut);
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_trzr:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		esp_timer_stop(htim_periodicAdc);
		fsetADCSpeed(ADC_CAL_PERIOD);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		pstu_sensorConfig->i_tareZero = fTareADC();
		fsetADCSpeed(pstu_adcConfig->ul_adcPeriod);
		vTaskDelay(100 / portTICK_PERIOD_MS);
		if (xEventGroupGetBits(eg_status) & STATUS_PEEKING)
			esp_timer_start_periodic(htim_periodicAdc, pstu_adcConfig->ul_adcPeriod);
		sprintf(pc_configOut, "|trzr|1|\t\tOK, tare successful|\n");
		fsendResponse(0, 1, pc_configOut);
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_cali:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tEnter calibration weight (xxx).\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			double d_calWheight = 0;
			pc_configOut = malloc(64);
			char* pc_unit = 0;
			d_calWheight = strtod(pc_value, &pc_unit);
			if(d_calWheight != 0.0)
			{
				esp_timer_stop(htim_periodicAdc);
				fsetADCSpeed(ADC_CAL_PERIOD);
				vTaskDelay(100 / portTICK_PERIOD_MS);
				pstu_sensorConfig->d_calValue = fcalADC(d_calWheight, pstu_sensorConfig->i_tareValue);
				fsetADCSpeed(pstu_adcConfig->ul_adcPeriod);
				vTaskDelay(100 / portTICK_PERIOD_MS);
				if (xEventGroupGetBits(eg_status) & STATUS_PEEKING)
					esp_timer_start_periodic(htim_periodicAdc, pstu_adcConfig->ul_adcPeriod);
				sprintf(pc_configOut,
						"|cali|%.*f|\t\tOK, calibration finished|\n",
						20, pstu_sensorConfig->d_calValue);
			}
			else
				sprintf(pc_configOut,
						"|err|"STR(ERR_RECORDING)"|\t\tcould not find value in message(%s).\n",
						pc_value);
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_mper:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tEnter measurement period.\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			pc_configOut = malloc(64);
			uint64_t ul_mesPeriod = fconvertToUs(pc_value);
			if (ul_mesPeriod)
			{
				if (ul_mesPeriod >= ADC_MIN_PERIOD)
				{
					esp_timer_stop(htim_periodicAdc);
					portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
					portENTER_CRITICAL(&mux);
					pstu_adcConfig->ul_adcPeriod = ul_mesPeriod;
					portEXIT_CRITICAL(&mux);
					sprintf(pc_configOut,
							"|mper|%llu|\t\tOK, Measurement period set to %lluus.|\n\4",
							pstu_adcConfig->ul_adcPeriod, pstu_adcConfig->ul_adcPeriod);
					if (xEventGroupGetBits(eg_status) & STATUS_PEEKING)
						esp_timer_start_periodic(htim_periodicAdc, pstu_adcConfig->ul_adcPeriod);
				}
				else if (ul_mesPeriod < ADC_MIN_PERIOD)
				{
					sprintf(pc_configOut,
							"|err|"STR(ERR_RECORDING)"|\t\tPeriod (%llu)too small (min. %d).\n",
							ul_mesPeriod, ADC_MIN_PERIOD);
				}
			}
			else
				sprintf(pc_configOut,
						"|err|"STR(ERR_RECORDING)"|\t\tcould not find value in message(%s).\n",
						pc_value);
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_ADC_SYNC,
				BIT_ADC_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	default:
		ESP_LOGW(TAG_ADC, "WRONG CMDLET");
		break;
	}
}

int32_t fgetLimit (uint8_t ui8_maxLoad, int8_t i8_perOverload, int32_t	i32_tareZero)
{
	ESP_LOGD(TAG_ADC, "ui8_maxLoad:%d  i8_perOverload:%d  i32_tareZero:%d", ui8_maxLoad, i8_perOverload, i32_tareZero);
	int32_t i32_limit = 0;
	i32_limit = ipow(10, ui8_maxLoad % 10) * ((float)(ui8_maxLoad / 10) / 2);
	//TODO: fit calvalue into limitcalculation
	i32_limit = pstu_sensorConfig->d_calValue;
	i32_limit -= i32_tareZero;
	i32_limit *= ((float) abs(i8_perOverload) / 100);

	return i32_limit;
}
