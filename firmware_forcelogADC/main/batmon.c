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
	xTaskCreate(tbatmonRun,		"t_batmonRun",		2048, NULL, 10, &ht_batmonRun);
}

void tbatmonRun (void* param)
{
	vTaskSuspend(NULL);
	struct stu_batmonConfig batmonConfig_mom;
	uint32_t ui_cmdlet = 0;
	float f_batVol = 0.0;
	int		i_batRaw = 0;
	char *message = malloc(30+1);
	vTaskDelay(20 / portTICK_PERIOD_MS);
	h_timerBatMon = xTimerCreate("timerBlink",BATMON_PERIOD / portTICK_PERIOD_MS,pdTRUE,NULL,callback_timerBatmon);

	while(1)
	{
		xTimerStart(h_timerBatMon, portMAX_DELAY);
		do
		{
			for (int i = 0; i < BATMON_NUM_SAMPLES; i++)
			{
				f_batVol += fmap(adc1_get_raw(ADC1_CHANNEL_4),batmonConfig_mom.i_batRawLow, batmonConfig_mom.f_batVolLow, batmonConfig_mom.i_batRawHigh, batmonConfig_mom.f_batVolHigh);
				i_batRaw += adc1_get_raw(ADC1_CHANNEL_4);
			}
			f_batVol /= BATMON_NUM_SAMPLES;
			i_batRaw /= BATMON_NUM_SAMPLES;
			ESP_LOGV(TAG_BATMON, "Battery voltage:%f	raw:%d", f_batVol,i_batRaw);
			sprintf(message, "|bvol|%.*f|\t\tBattery voltage|\n",2, f_batVol);
			xQueueSend(q_tcpConf,&message, 0);
			while(xTaskNotify(ht_tcpConf,CMD_BATMON_READY,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		}while(ui_cmdlet == CMD_fire);
		fconfigBatMon(ui_cmdlet, &batmonConfig_mom);
		xTimerStop(h_timerBatMon, portMAX_DELAY);
	}
}

void callback_timerBatmon(void* arg)
{
	while(xTaskNotify(ht_batmonRun,CMD_fire,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
}

void fconfigBatMon(uint32_t ui_cmdlet, struct stu_batmonConfig* batmonConfig_mom)
{
	float f_temp = 0.0;
	uint32_t ui_temp = 0;
	struct stu_batmonConfig* batmonConfig_ext = NULL;
	switch(ui_cmdlet)
	{
	case CMD_bath:
		xQueuePeek(q_pointer, &f_temp, portMAX_DELAY);
		batmonConfig_mom->f_batVolHigh = f_temp;
		xQueueReceive(q_pointer, &ui_temp, portMAX_DELAY);
		ui_temp = adc1_get_raw(ADC1_CHANNEL_4);
		batmonConfig_mom->i_batRawHigh = ui_temp;
		xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
		ESP_LOGD(TAG_CONF, "NOTIFYING SD CARD");
		xQueueSend(q_pointer,&batmonConfig_mom, portMAX_DELAY);
		while(xTaskNotify(ht_storageRun,CMD_SAVE_BATMON,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		break;
	case CMD_batl:
		xQueuePeek(q_pointer, &f_temp, portMAX_DELAY);
		batmonConfig_mom->f_batVolLow = f_temp;
		xQueueReceive(q_pointer, &ui_temp, portMAX_DELAY);
		ui_temp = adc1_get_raw(ADC1_CHANNEL_4);
		batmonConfig_mom->i_batRawLow = ui_temp;
		xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
		ESP_LOGD(TAG_CONF, "NOTIFYING SD CARD");
		xQueueSend(q_pointer,&batmonConfig_mom, portMAX_DELAY);
		while(xTaskNotify(ht_storageRun,CMD_SAVE_BATMON,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		break;
	case CMD_load:
		xQueuePeek(q_pointer, &batmonConfig_ext, portMAX_DELAY);
		batmonConfig_mom->f_batVolHigh = batmonConfig_ext->f_batVolHigh;
		batmonConfig_mom->f_batVolLow = batmonConfig_ext->f_batVolLow;
		batmonConfig_mom->i_batRawLow = batmonConfig_ext->i_batRawLow;
		batmonConfig_mom->i_batRawHigh = batmonConfig_ext->i_batRawHigh;
		free(batmonConfig_ext);
		xQueueReceive(q_pointer, &batmonConfig_ext, portMAX_DELAY);
		break;
	default:
		break;
	}
}

float fmap(int x, int us_batRawLow, float f_batVolLow, int us_batRawHigh, float f_batVolHigh)
{
	return (x - us_batRawLow) * (f_batVolHigh - f_batVolLow) / (us_batRawHigh - us_batRawLow) + f_batVolLow;
}

