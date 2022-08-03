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
	char* pc_configOut = 0;
	char* pc_value = 0;
	char* pc_configIn = 0;
//	vTaskDelay(10000 / portTICK_PERIOD_MS);
	uint32_t ui_cmdlet = 0;
	float f_tempInt = 0.0;
	float f_tempExt = 0.0;
	stu_mesCell dataPoint =
	{
			.str_type[0] = '\0',
			.str_data[0] = '\0',
			.ul_time = 0
	};
	DS18B20_ERROR error = DS18B20_ERROR_NULL;
	DS18B20_Info* p_probeTempExt = ds18b20_malloc();
	DS18B20_Info* p_probeTempInt = ds18b20_malloc();
	fscanOwb(p_probeTempExt, p_probeTempInt);
	if (p_probeTempExt->init)
	{
		xSemaphoreTake(hs_pointerQueue,portMAX_DELAY);
		while(xTaskNotify(ht_storageRun,CMD_ldlc,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		char* ac_cellName = malloc(OWB_ROM_CODE_STRING_LENGTH);
		owb_string_from_rom_code(p_probeTempExt->rom_code,
				ac_cellName,
				sizeof(OWB_ROM_CODE_STRING_LENGTH));
		xQueueSend(q_send, &ac_cellName, portMAX_DELAY);
		uint32_t ui32_response = 0;
		xQueueReceive (q_recv, &ui32_response, portMAX_DELAY);

		if(ui32_response)
		{
			uint8_t ui8_maxLoad = 0;
			uint8_t ui8_perOverload = 0;
			uint8_t ui8_resolution;

			freadScratchpad	(p_probeTempExt, &ui8_maxLoad, &ui8_perOverload, &ui8_resolution);
			pstu_sensorConfig->ui8_maxLoad = ui8_maxLoad;
			pstu_sensorConfig->i8_perOverload = ui8_perOverload;
			strcpy(pstu_sensorConfig->ac_name, ac_cellName);
		}
		else
		{
			ESP_LOGD(TAG_TEMPINT, "LOADING LOADCELL FAILED");
		}
		xSemaphoreGive(hs_pointerQueue);
	}
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
				sprintf(dataPoint.str_type, "tpin");
				sprintf(dataPoint.str_data, "%.2f", f_tempInt);
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
			if (f_tempInt >= MAX_TEMP)
			{
				//TODO: implement emergency management LED STATUS is only a testing placeholder
				const uint32_t rgb_status = LEDC_STATUS_WAITING_SERIAL;
				xQueueSend(q_rgb_status, &rgb_status, portMAX_DELAY);
				ESP_LOGW(TAG_TEMPINT,
						"WARNING. TEMPERATURE (%.*f) CRITICAL.",
						2, f_tempInt);
			}
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_PEEKING))
			{
				xQueueSendFromISR(q_measurements, &dataPoint,  0);
				if(p_probeTempExt->init)
				{
					sprintf(dataPoint.str_type, "tpex");
					sprintf(dataPoint.str_data, "%4.2f", f_tempExt);
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

				while(xTaskNotify(ht_storageRun,CMD_ldlc,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);

				xSemaphoreTake(hs_pointerQueue,portMAX_DELAY);
				char* ac_cellName = malloc(OWB_ROM_CODE_STRING_LENGTH);
				owb_string_from_rom_code(pstu_tempConfig->romExt,
						ac_cellName,
						sizeof(OWB_ROM_CODE_STRING_LENGTH));
				xQueueSend(q_send, &ac_cellName, portMAX_DELAY);
				uint32_t ui32_response = 0;
				xQueueReceive (q_recv, &ui32_response, portMAX_DELAY);

				if(ui32_response)
				{
					uint8_t ui8_maxLoad = 0;
					uint8_t ui8_perOverload = 0;
					uint8_t ui8_resolution;
					ESP_LOGD(TAG_TEMPINT, "LIMITS READ. MAXLOAD:%d PEROVERLOAD:%d", ui8_maxLoad, ui8_perOverload);
					freadScratchpad	(p_probeTempExt, &ui8_maxLoad, &ui8_perOverload, &ui8_resolution);
					pstu_sensorConfig->ui8_maxLoad = ui8_maxLoad;
					pstu_sensorConfig->i8_perOverload = ui8_perOverload;
				}
				xSemaphoreGive(hs_pointerQueue);


			}
			gpio_isr_handler_add(GPIO_OWB_SENSE, gpio_isr_handler, NULL);
			break;

		case CMD_lmlc: //LiMit LoadCell: writes new limit to loadcell
		{
			uint32_t ui32_maxLoad = 0;
			xQueueReceive(q_send,
					&ui32_maxLoad,
					portMAX_DELAY);
			DS18B20_ERROR err = fwriteScratchpad(p_probeTempExt, (uint8_t) ui32_maxLoad, 20);
			int32_t response = (int32_t)err;
			xQueueSend(q_recv,
					&response,
					portMAX_DELAY);
		}
		break;

		case CMD_lmup: //LiMit UPdate: writes new overload to loadcell
		{
			uint32_t ui32_perOverload = 0;
			xQueueReceive(q_send,
					&ui32_perOverload,
					portMAX_DELAY);
			DS18B20_ERROR err = fwriteScratchpad(p_probeTempExt, 0, (uint8_t) ui32_perOverload);
		}
			break;

		case CMD_svlc:
			pc_value = malloc(OWB_ROM_CODE_STRING_LENGTH);
			owb_string_from_rom_code(p_probeTempExt->rom_code,
					pc_value,
					sizeof(OWB_ROM_CODE_STRING_LENGTH));
			strcpy(pstu_sensorConfig->ac_name, pc_value);
			xQueueSend(q_recv,
					&pc_value,
					portMAX_DELAY);
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
				if (fowbcmp(pstu_tempConfig->romInt.bytes, device_rom_codes[0].bytes) != 1)
				{
					memcpy((void*)&pstu_tempConfig->romInt,(void*)device_rom_codes,sizeof(OneWireBus_ROMCode));
					while(xTaskNotify(ht_storageRun, CMD_svti,eSetValueWithoutOverwrite) != pdPASS)
						vTaskDelay(10 / portTICK_PERIOD_MS);
				}
			}
			else
			{
				ESP_LOGD(TAG_TEMPINT, "An error occurred reading ROM code: %d", status);
			}
			ds18b20_init(p_probeTempInt, owb, pstu_tempConfig->romInt); // associate with bus and device
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
					if (is_present && !fowbcmp(pstu_tempConfig->romInt.bytes, device_rom_codes[i].bytes)) //DEVICE IS EXTERNAL
					{
						ds18b20_init(devTempExt, owb, device_rom_codes[i]);
						ds18b20_use_crc(devTempExt, true);
						ds18b20_set_resolution(devTempExt, DS18B20_RESOLUTION);
					}
					else if (is_present && fowbcmp(pstu_tempConfig->romInt.bytes, device_rom_codes[i].bytes)) //DEVICE IS INTERNAL
					{
						ds18b20_init(p_probeTempInt, owb, pstu_tempConfig->romInt); // associate with bus and device
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

//TODO:Write Overload Scratchpad routines
DS18B20_ERROR	freadScratchpad				(DS18B20_Info * ds18b20_info, uint8_t* ui8_maxLoad, uint8_t* ui8_perOverload, uint8_t* ui8_resolution)
{
	uint8_t buffer[9] = {0};
	DS18B20_ERROR err = DS18B20_ERROR_UNKNOWN;
	bool present = false;
	owb_reset(ds18b20_info->bus, &present);
	owb_write_byte(ds18b20_info->bus, OWB_ROM_MATCH);
	owb_write_rom_code(ds18b20_info->bus, ds18b20_info->rom_code);

	owb_write_byte(ds18b20_info->bus, 0xBE);
	owb_read_bytes(ds18b20_info->bus, buffer, 9);

    // With CRC:
    if (owb_crc8_bytes(0, buffer, sizeof(buffer)) != 0)
    {
    	ESP_LOGD(TAG_TEMPINT, "CRC FAILED READING SCRATCHPAD");
        err = DS18B20_ERROR_CRC;
    }
    else
    {
    	 err = DS18B20_OK;
    	 if (ui8_maxLoad)
    		 *ui8_maxLoad = buffer[2];
    	 if(ui8_perOverload)
    		 *ui8_perOverload = buffer[3];
    	 if(ui8_resolution)
    	 *ui8_resolution = buffer[4];
    }
	return err;
}

DS18B20_ERROR	fwriteScratchpad			(DS18B20_Info * ds18b20_info, uint8_t ui8_maxLoad, uint8_t ui8_perOverload)
{
	DS18B20_ERROR err = DS18B20_ERROR_UNKNOWN;
	uint8_t bufferOut[4] = {0x4E};
	if (freadScratchpad (ds18b20_info, &bufferOut[1], &bufferOut[2], &bufferOut[4]) == DS18B20_OK)
	{
		if (ui8_maxLoad)
			bufferOut[1] = ui8_maxLoad;
		if(ui8_perOverload)
			bufferOut[2] = ui8_perOverload;

		bool present = false;
		owb_reset(ds18b20_info->bus, &present);
		owb_write_byte(ds18b20_info->bus, OWB_ROM_MATCH);
		owb_write_rom_code(ds18b20_info->bus, ds18b20_info->rom_code);
		owb_write_bytes(ds18b20_info->bus, bufferOut, sizeof(bufferOut));

		owb_reset(ds18b20_info->bus, &present);
		owb_write_byte(ds18b20_info->bus, OWB_ROM_MATCH);
		owb_write_rom_code(ds18b20_info->bus, ds18b20_info->rom_code);
		owb_write_byte(ds18b20_info->bus, 0x48);
		vTaskDelay( 10 / portTICK_PERIOD_MS);

		err = DS18B20_OK;
	}

	return err;
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
