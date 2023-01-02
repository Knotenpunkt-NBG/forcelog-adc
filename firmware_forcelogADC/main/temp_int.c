/*
 * temperature.c
 *
 *  Created on: Oct 19, 2020
 *      Author: lor
 */

#include "temp_int.h"

onewire_bus_handle_t handle;
onewire_rom_search_context_handler_t context_handler;

void ftempIntInit(void)
{
    onewire_rmt_config_t config = {
        .gpio_pin = OWB_GPIO,
        .max_rx_bytes = 10, // 10 tx bytes(1byte ROM command + 8byte ROM number + 1byte device command)
    };

    // install new 1-wire bus
    ESP_ERROR_CHECK(onewire_new_bus_rmt(&config, &handle));
    ESP_LOGI(TAG_TEMPINT, "1-wire bus installed");

    // create 1-wire rom search context
    ESP_ERROR_CHECK(onewire_rom_search_context_create(handle, &context_handler));

    ESP_ERROR_CHECK(fscanOwb(context_handler, pstu_tempConfig->romInt, pstu_tempConfig->romExt, handle));



    //LEFT OFF

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
	esp_err_t err;
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

	if (1)
	{
		xSemaphoreTake(hs_pointerQueue,portMAX_DELAY);
		while(xTaskNotify(ht_storageRun,CMD_ldlc,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		char* ac_cellName = malloc(OWB_ROM_CODE_STRING_LENGTH);
		owb_string_from_rom_code(pstu_tempConfig->romInt,
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

			ds18b20_read_scratchpad(handle, pstu_tempConfig->romExt, &ui8_maxLoad, &ui8_perOverload);
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
			//Trigger Conversion
			err = ds18b20_trigger_temperature_conversion(handle, NULL); // skip rom to send command to all devices on the bus
			vTaskDelay(pdMS_TO_TICKS(800)); // 12-bit resolution needs 750ms to convert

			//Read Internal Temp Sensor
			err =  ds18b20_get_temperature(handle, pstu_tempConfig->romInt, &f_tempInt);
			sprintf(dataPoint.str_type, "tpin");
			sprintf(dataPoint.str_data, "%.2f", f_tempInt);
			dataPoint.ul_time = esp_timer_get_time();
			if (f_tempInt >= MAX_TEMP)
			{
				//TODO: implement emergency management LED STATUS is only a testing placeholder
				const uint32_t rgb_status = LEDC_STATUS_WAITING_SERIAL;
				xQueueSend(q_rgb_status, &rgb_status, portMAX_DELAY);
				ESP_LOGW(TAG_TEMPINT,
						"WARNING. TEMPERATURE (%.*f) CRITICAL.",
						2, f_tempInt);
			}

			//Read External Temp Sensor
			err =  ds18b20_get_temperature(handle, pstu_tempConfig->romExt, &f_tempExt);
			if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_PEEKING))
			{
				xQueueSendFromISR(q_measurements, &dataPoint,  0);
					sprintf(dataPoint.str_type, "tpex");
					sprintf(dataPoint.str_data, "%4.2f", f_tempExt);
					xQueueSendFromISR(q_measurements, &dataPoint,  0);
			}
			break;


		case CMD_scow:		//SCan OneWire
			ESP_LOGD(TAG_TEMPINT, "CMD_scow");
			vTaskDelay(100 / portTICK_PERIOD_MS);
			if (gpio_get_level(GPIO_OWB_SENSE) == 1)
			{
				ESP_LOGD(TAG_TEMPINT, "PROBE DISCONNECTED");
			}
			else
			{
				ESP_ERROR_CHECK(fscanOwb(context_handler, pstu_tempConfig->romInt, pstu_tempConfig->romExt, handle));
			}
			gpio_isr_handler_add(GPIO_OWB_SENSE, gpio_isr_handler, NULL);
			break;

		case CMD_lmlc:		//LiMit LoadCell: writes new limit to loadcell
		{
			uint32_t ui32_maxLoad = 0;
			xQueueReceive(q_send,
					&ui32_maxLoad,
					portMAX_DELAY);
			err = ds18b20_write_scratchpad(handle,pstu_tempConfig->romExt, ui32_maxLoad, 20);
			int32_t response = (int32_t)err;
			xQueueSend(q_recv,
					&response,
					portMAX_DELAY);
		}
		break;

		case CMD_lmup:		//LiMit UPdate: writes new overload to loadcell
		{
			uint32_t ui32_perOverload = 0;
			xQueueReceive(q_send,
					&ui32_perOverload,
					portMAX_DELAY);
			err = ds18b20_write_scratchpad(handle,pstu_tempConfig->romExt, 0, (uint8_t) ui32_perOverload);
		}
			break;

		case CMD_svlc:		//SaVe LoadCell
			pc_value = malloc(OWB_ROM_CODE_STRING_LENGTH);
			owb_string_from_rom_code(pstu_tempConfig->romExt,
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


esp_err_t fscanOwb(onewire_rom_search_context_handler_t context_handler, uint8_t* p_ROM_INT, uint8_t* p_ROM_EXT, onewire_bus_handle_t handle)
{
	esp_err_t err = ESP_OK;

	if ((xEventGroupGetBits(eg_status) & STATUS_RECORDING) | (xEventGroupGetBits(eg_status) & STATUS_WAITING))
	{}
	else
	{
		uint8_t num_devices = 0;
		uint8_t device_rom_id[2][8];
	    // search for devices on the bus
	    do {
	        esp_err_t search_result = onewire_rom_search(context_handler);

	        if (search_result == ESP_ERR_INVALID_CRC) {
	            continue; // continue on crc error
	        } else if (search_result == ESP_FAIL || search_result == ESP_ERR_NOT_FOUND) {
	            break; // break on finish or no device
	        }

	        ESP_ERROR_CHECK(onewire_rom_get_number(context_handler, device_rom_id[num_devices]));
	        ESP_LOGI(TAG_TEMPINT, "found device with rom id " ONEWIRE_ROM_ID_STR, ONEWIRE_ROM_ID(device_rom_id[num_devices]));
	        num_devices ++;
	    } while (num_devices < 2);

		if (num_devices == 1)// ONLY INTERNAL TEMPERATURE PROBE IS PRESENT
		{
			ESP_LOGD(TAG_TEMPINT, "SINGLE DEVICE FOUND");
			//COMPARE FOUND ROMCODE WITH ROMCODE IN CONFIG AND SAVE IT TO NON VOLATILE STORAGE IF UNKNOWN
			if (fowbcmp(device_rom_id[0], pstu_tempConfig->romInt) != 1)
			{
				for(int i = 0; i < sizeof(device_rom_id[0]);i++)
				{
					pstu_tempConfig->romInt[i] = device_rom_id[0][i];
				}
				while(xTaskNotify(ht_storageRun, CMD_svti,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(10 / portTICK_PERIOD_MS);
			}
			//SET ROM EXT TO ALL 0
			for(int i = 0; i < sizeof(device_rom_id[0]);i++)
			{
				pstu_tempConfig->romExt[i] = 0;
			}
			err = ds18b20_set_resolution(handle, device_rom_id[0], DS18B20_RESOLUTION_12B);
		}
		else if (num_devices == 0) //NO DEVICES FOUND
		{
			ESP_LOGD(TAG_TEMPINT, "NO DEVICES PRESENT");
		}
		else //INTERNAL AND EXTERNAL PROBE FOUND
		{
			ESP_LOGD(TAG_TEMPINT, "TWO DEVICES FOUND");
			//CHECK IF ONE OF THEM IS INTERNAL
			if (fowbcmp(device_rom_id[0], pstu_tempConfig->romInt) == 1)
			{
				for(int i = 0; i < sizeof(device_rom_id[0]);i++)
				{
					pstu_tempConfig->romExt[i] = device_rom_id[1][i];
				}
			}
			else if (fowbcmp(device_rom_id[1], pstu_tempConfig->romInt) == 1)
			{
				for(int i = 0; i < sizeof(device_rom_id[0]);i++)
				{
					pstu_tempConfig->romExt[i] = device_rom_id[0][i];
				}
			}
			else
			{
				//COULD NOT FIND INTERNAL PROBE
				return ESP_ERR_NOT_FOUND;
			}

			//LOADING LOADCELL FROM STORAGE
			xSemaphoreTake(hs_pointerQueue,portMAX_DELAY);
			while(xTaskNotify(ht_storageRun,CMD_ldlc,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);
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

				ds18b20_read_scratchpad	(handle, &ui8_maxLoad, &ui8_perOverload, &ui8_resolution);
				pstu_sensorConfig->ui8_maxLoad = ui8_maxLoad;
				pstu_sensorConfig->i8_perOverload = ui8_perOverload;
				strcpy(pstu_sensorConfig->ac_name, ac_cellName);
			}
			else
			{
				ESP_LOGD(TAG_TEMPINT, "LOADING LOADCELL FAILED");
			}
			xSemaphoreGive(hs_pointerQueue);

			//SETTING RESOLUTION OF ALL TEMPERATUR SENSORS
			for (int i = 0; i< 2; i++)
			{
				err = ds18b20_set_resolution(handle, device_rom_id[0], DS18B20_RESOLUTION_12B);
			}
		}
	}
	return err;
}

void fowbConfig(uint32_t ui_cmdlet)
{

}
void callback_timerTemp(TimerHandle_t pxTimer)
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

