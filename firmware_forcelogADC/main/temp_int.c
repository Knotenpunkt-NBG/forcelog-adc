/*
 * temperature.c
 *
 *  Created on: Oct 19, 2020
 *      Author: lor
 */

#include "temp_int.h"

//TODO: once multidevice support is implemented in hardware, rewrite 1-wire scan and initialisation

void ftempIntInit(void)
{



	// clean up dynamically allocated data
//	for (int i = 0; i < num_devices; ++i)
//	{
//		ds18b20_free(&devices[i]);
//	}
//	owb_uninitialize(owb);
	xTaskCreate	(ttempIntRun,		"t_tempIntRun",		2048,	NULL	,	10,		&ht_tempIntRun);
}

void ttempIntRun(void* param)
{
	vTaskSuspend(NULL);
	ESP_LOGI(TAG_TEMPINT, "INITIALISING TEMPERATURE INTERNAL");
	uint64_t time_since_boot = 0;

	// Create DS18B20 devices on the 1-Wire bus
//	xEventGroupWaitBits(eg_tempInt, BIT_TEMP_START, true, true, portMAX_DELAY);
		DS18B20_Info * ds18b20_info = ds18b20_malloc();  // heap allocation
		stu_temp_probe_int = ds18b20_info;

			ds18b20_init(ds18b20_info, owb, stu_romCode_probe_int); // associate with bus and device
		ds18b20_use_crc(ds18b20_info, true);           // enable CRC check on all reads
		ds18b20_set_resolution(ds18b20_info, DS18B20_RESOLUTION);


	// Read temperatures more efficiently by starting conversions on all devices at the same time
	int errors_count[MAX_DEVICES] = {0};
	int sample_count = 0;
	if (num_devices > 0)
	{
		TickType_t last_wake_time = xTaskGetTickCount();

		while (1)
		{
			last_wake_time = xTaskGetTickCount();

			ds18b20_convert_all(owb);

			// In this application all devices use the same resolution,
			// so use the first device to determine the delay
			ds18b20_wait_for_conversion(stu_temp_probe_int);

			// Read the results immediately after conversion otherwise it may fail
			// (using printf before reading may take too long)
			float readings[MAX_DEVICES] = { 0 };
			DS18B20_ERROR errors[MAX_DEVICES] = { 0 };

			for (int i = 0; i < num_devices; ++i)
			{
				errors[i] = ds18b20_read_temp(stu_temp_probe_int, &readings[i]);
			}

			// Print results in a separate loop, after all have been read
//			printf("\nTemperature readings (degrees C): sample %d\n", ++sample_count);

			time_since_boot = esp_timer_get_time();
			xQueueSend(q_time_tempint_tcp, &time_since_boot,  0);
			xQueueSend(q_value_tempint_tcp, &readings[0],  0);
			while(xTaskNotify(ht_tcpMes,CMD_TEMPINT_READY,eSetValueWithoutOverwrite) != pdPASS)
				vTaskDelay(1/ portTICK_PERIOD_MS);

//			for (int i = 0; i < num_devices; ++i)
//			{
//				if (errors[i] != DS18B20_OK)
//				{
//					++errors_count[i];
//				}
//
//				printf("  %d: %.1f    %d errors\n", i, readings[i], errors_count[i]);
//			}

			vTaskDelayUntil(&last_wake_time, TEMP_INTERNAL_PERIOD / portTICK_PERIOD_MS);
		}
	}
	else
	{
		printf("\nNo DS18B20 devices detected!\n");
	}

//	    // Read temperatures from all sensors sequentially
//	    while (1)
//	    {
//	        printf("\nTemperature readings (degrees C):\n");
//	        for (int i = 0; i < num_devices; ++i)
//	        {
//	            float temp = ds18b20_get_temp(devices[i]);
//	            printf("  %d: %.3f\n", i, temp);
//	        }
//	        vTaskDelay(1000 / portTICK_PERIOD_MS);
//	    }
	while(1);
}
