#include "adc_routines.h"


void fADCInit(void)
{
	xTaskCreate(tadcRun, "tadcRun", 2048, NULL, 10, &ht_adcRun);
	//Creates Periodic Timer for Reading ADC
	const esp_timer_create_args_t stu_adcTimerArgs = {
			.callback = &fadcTimerCallback,
			.name = "ADCTimer"
	};
	esp_timer_create(&stu_adcTimerArgs, &htim_periodicAdc);
}

int32_t freadAdc(stu_mesCell* dataPoint)
{
	uint64_t ul_time = esp_timer_get_time();
	ul_time %= 5000000;
	ul_time /= 5;
	double d_measuremente = sin(((double)ul_time/1000000)*3.14*2) * gstu_config->cell.d_calValue - gstu_config->cell.i_tareValue;
	sprintf(dataPoint->str_data, "%8.*f",gstu_config->adc.uc_numDecimals, d_measuremente);
	return 1;
}


void fADCConfig	(uint32_t CMDlet)
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
		gstu_config->cell.i_tareValue += 1;
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
