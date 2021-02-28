/*
 * temperature.c
 *
 *  Created on: Oct 19, 2020
 *      Author: lor
 */

#include "temp_int.h"

void ftempIntInit(void)
{

	owb = owb_rmt_initialize(&rmt_driver_info, OWB_GPIO, RMT_CHANNEL_1, RMT_CHANNEL_0);
	owb_use_crc(owb, true);  // enable CRC check for ROM co
	owb_use_parasitic_power(owb, true);

	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_INTR_ANYEDGE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pin_bit_mask = (1ULL<<GPIO_OWB_SENSE);
//	io_conf.pull_down_en = 0;
//	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);


	ESP_ERROR_CHECK(gpio_install_isr_service(0));
	htim_oneWire = xTimerCreate("timerTemp",
			TEMP_INTERNAL_PERIOD * 1000 / portTICK_PERIOD_MS ,
			pdTRUE,
			NULL,
			callback_timerTemp);
	xTaskCreate	(ttempIntRun,		"t_tempIntRun",		4096,	NULL	,	5,		&ht_tempIntRun);
}


void ttempIntRun(void* param)
{
	vTaskDelay(2000 / portTICK_PERIOD_MS);
	uint32_t ui_cmdlet = 0;
	float f_tempInt = 0.0;
	float f_tempExt = 0.0;
	struct stu_mesCell dataPoint =
	{
			.b_type = TYPE_TEMP_INT,
			.d_measurement = 0.0,
			.ul_time = 0
	};
	DS18B20_ERROR error = DS18B20_ERROR_NULL;
	DS18B20_Info* p_probeTempExt = ds18b20_malloc();
	DS18B20_Info* p_probeTempInt = ds18b20_malloc();
	fscanOwb(p_probeTempExt, p_probeTempInt);
	ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_OWB_SENSE, gpio_isr_handler, NULL));
	xTimerStart(htim_oneWire, portMAX_DELAY);
	while (1)
	{
		xTaskNotifyWait(false,
				ULONG_MAX,
				&ui_cmdlet,
				portMAX_DELAY);
		switch (ui_cmdlet)
		{
		case CMD_fire:
			error = ds18b20_convert_and_read_temp(p_probeTempInt, &f_tempInt);
			if (error != DS18B20_OK)
			{
				ESP_LOGD(TAG_TEMPINT, "ERROR READING INT TEMPERATURE %d", error);
			}
			else
			{
				dataPoint.b_type = TYPE_TEMP_INT;
				dataPoint.d_measurement = f_tempInt;
				dataPoint.ul_time = esp_timer_get_time();
			}
			if(p_probeTempExt->init)
			{
				error = ds18b20_convert_and_read_temp(p_probeTempExt, &f_tempExt);
				if (error != DS18B20_OK)
				{
					ESP_LOGD(TAG_TEMPINT, "ERROR READING EXT TEMPERATURE %d", error);
				}
			}
			if (dataPoint.d_measurement >= MAX_TEMP)
			{
				//TODO: implement emergency management LED STATUS is only a testing placeholder
				const uint32_t rgb_status = LEDC_STATUS_WAITING_SERIAL;
				xQueueSend(q_rgb_status, &rgb_status, portMAX_DELAY);
				ESP_LOGW(TAG_TEMPINT,
						"WARNING. TEMPERATURE (%.*f) CRITICAL.",
						2, dataPoint.d_measurement);
			}
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_PEEKING))
			{
				xQueueSendFromISR(q_measurements, &dataPoint,  0);
				if(p_probeTempExt->init)
				{
					dataPoint.b_type = TYPE_TEMP_EXT;
					dataPoint.d_measurement = f_tempExt;
					xQueueSendFromISR(q_measurements, &dataPoint,  0);
				}
			}
			break;

		case CMD_scow:
			ESP_LOGD(TAG_TEMPINT, "CMD_scow");
			vTaskDelay(100 / portTICK_PERIOD_MS);
			if (gpio_get_level(GPIO_OWB_SENSE) == 1)
			{
				ESP_LOGD(TAG_TEMPINT, "PROBE DISCONNECTED");
				p_probeTempExt->init = false;
			}
			else
			{
				ESP_LOGD(TAG_TEMPINT, "PROBE CONNECTED");
				fscanOwb(p_probeTempExt, p_probeTempInt);
				ds18b20_init(p_probeTempExt, owb, p_probeTempExt->rom_code);
				ds18b20_use_crc(p_probeTempExt, true);
				ds18b20_set_resolution(p_probeTempExt, DS18B20_RESOLUTION);
			}
			gpio_isr_handler_add(GPIO_OWB_SENSE, gpio_isr_handler, NULL);
			break;

		default:
			break;
		}
	}
}


void fscanOwb(DS18B20_Info* devTempExt,DS18B20_Info* p_probeTempInt)
{
	if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
	{}
	else
	{
		OneWireBus_ROMCode device_rom_codes[MAX_DEVICES] = {0};
		int num_devices = 0;
		num_devices = 0;
		OneWireBus_SearchState search_state = {0};
		bool found = false;

		owb_search_first(owb, &search_state, &found);
		while (found)
		{
			device_rom_codes[num_devices] = search_state.rom_code;
			++num_devices;
			owb_search_next(owb, &search_state, &found);
		}
		if (num_devices == 1)// ONLY INTERNAL TEMPERATURE PROBE IS PRESENT
		{
			ESP_LOGD(TAG_TEMPINT, "SINGLE DEVICE FOUND");
			owb_status status = owb_read_rom(owb, &device_rom_codes[0]);
			if (status == OWB_STATUS_OK)
			{
				//COMPARE FOUND ROMCODE WITH ROMCODE IN CONFIG AND SAVE IT TO NON VOLATILE STORAGE
				if (fowbcmp(gstu_config->temp.romInt.bytes, device_rom_codes[0].bytes) != 1)
				{
					memcpy((void*)&gstu_config->temp.romInt,(void*)device_rom_codes,sizeof(OneWireBus_ROMCode));
					while(xTaskNotify(ht_storageRun, CMD_svti,eSetValueWithoutOverwrite) != pdPASS)
						taskYIELD();
				}
			}
			else
			{
				ESP_LOGD(TAG_TEMPINT, "An error occurred reading ROM code: %d", status);
			}
			ds18b20_init(p_probeTempInt, owb, gstu_config->temp.romInt); // associate with bus and device
			ds18b20_use_crc(p_probeTempInt, true);           // enable CRC check on all reads
			ds18b20_set_resolution(p_probeTempInt, DS18B20_RESOLUTION);
		}
		else if (num_devices == 0) //NO DEVICES FOUND
		{
			ESP_LOGD(TAG_TEMPINT, "NO DEVICES PRESENT");
		}
		else //INTERNAL AND EXTERNAL PROBE FOUND
		{
			ESP_LOGD(TAG_TEMPINT, "MULTIPLE DEVICES FOUND");
			bool is_present = false;
			for (int i = 0; i <= num_devices; i++)
			{
				owb_status search_status = owb_verify_rom(owb, device_rom_codes[i], &is_present);
				if (search_status == OWB_STATUS_OK)
				{
					if (is_present && !fowbcmp(gstu_config->temp.romInt.bytes, device_rom_codes[i].bytes)) //DEVICE IS EXTERNAL
					{
						char* ac_cellName = malloc(OWB_ROM_CODE_STRING_LENGTH);
						gstu_config->temp.romExt = device_rom_codes[i];
						owb_string_from_rom_code(device_rom_codes[i],
								ac_cellName,
								sizeof(OWB_ROM_CODE_STRING_LENGTH));
						while(xTaskNotify(ht_storageRun, CMD_ldlc,eSetValueWithoutOverwrite) != pdPASS)
							taskYIELD();

						ds18b20_init(devTempExt, owb, gstu_config->temp.romExt); // associate with bus and device
						ds18b20_use_crc(devTempExt, true);           // enable CRC check on all reads
						ds18b20_set_resolution(devTempExt, DS18B20_RESOLUTION);
					}
					else if (is_present && fowbcmp(gstu_config->temp.romInt.bytes, device_rom_codes[i].bytes)) //DEVICE IS INTERNAL
					{
						ds18b20_init(p_probeTempInt, owb, gstu_config->temp.romInt); // associate with bus and device
						ds18b20_use_crc(p_probeTempInt, true);           // enable CRC check on all reads
						ds18b20_set_resolution(p_probeTempInt, DS18B20_RESOLUTION);
					}
					else //DEVICE NOT PRESENT
					{

					}
				}
				else
				{
					printf("An error occurred searching for known device: %d", search_status);
				}
			}
		}
	}
}

void fowbConfig(uint32_t ui_cmdlet)
{

}

void callback_timerTemp(void* arg)
{
	while(xTaskNotify(ht_tempIntRun,CMD_fire,eSetValueWithoutOverwrite) != pdPASS)
		taskYIELD();
}

void IRAM_ATTR gpio_isr_handler(void* arg)
{
	gpio_isr_handler_remove(GPIO_OWB_SENSE);
	while(xTaskNotify(ht_tempIntRun,CMD_scow,eSetValueWithoutOverwrite) != pdPASS)
		taskYIELD();
}

uint32_t fowbcmp(uint8_t* romCodeA, uint8_t* romCodeB)
{
	uint32_t res = 1;
	for(int i = 0; i < 8; i++)
	{
		if (romCodeA[i] != (romCodeB[i]))
			res = 0;
	}
	return res;
}
