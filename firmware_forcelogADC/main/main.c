#include "main.h"

void app_main()
{
	esp_log_level_set("*", ESP_LOG_ERROR);
	fsetLogLevel(LOG_DEBUG);

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	//Creating Semaphores
	hs_configCom = xSemaphoreCreateBinary();
	xSemaphoreGive(hs_configCom);
	hs_pointerQueue = xSemaphoreCreateBinary();
	xSemaphoreGive(hs_pointerQueue);

	//Creating Event Groups
	eg_sync = xEventGroupCreate();

	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	nvs_flash_init();

	//Creating queues q_time_temp
	q_rgb_status 		=	xQueueCreate(	10, sizeof(uint32_t));
	q_tcpMessages		=	xQueueCreate(	10,	sizeof(char *));
	q_uartMessages		=	xQueueCreate(	10,	sizeof(char *));
	q_pointer			=	xQueueCreate(	1,	sizeof(void *));
	q_pconfigIn			=	xQueueCreate(	5,	sizeof(void *));
	q_pconfigOut		=	xQueueCreate(	5,	sizeof(void *));

	q_value_mes_tcp		=	xQueueCreate(	10, sizeof(double));
	q_time_mes_tcp		=	xQueueCreate(	10, sizeof(uint64_t));
	q_time_blink_tcp	=	xQueueCreate(	3,	sizeof(uint64_t));
	q_value_tempint_tcp	=	xQueueCreate(	3,	sizeof(float));
	q_time_tempint_tcp	=	xQueueCreate(	3,	sizeof(uint64_t));

	q_value_mes_sd		=	xQueueCreate(	10, sizeof(double));
	q_time_mes_sd		=	xQueueCreate(	10, sizeof(uint64_t));
	q_time_blink_sd		=	xQueueCreate(	3,	sizeof(uint64_t));
	q_value_tempint_sd	=	xQueueCreate(	3,	sizeof(float));
	q_time_tempint_sd	=	xQueueCreate(	3,	sizeof(uint64_t));

	//Loading Config


	// Create a 1-Wire bus, using the RMT timeslot driver
	owb_rmt_driver_info rmt_driver_info;
	owb = owb_rmt_initialize(&rmt_driver_info, OWB_GPIO, RMT_CHANNEL_1, RMT_CHANNEL_0);
	owb_use_crc(owb, true);  // enable CRC check for ROM co
	// Check for parasitic-powered devices
	bool parasitic_power = false;
	ds18b20_check_for_parasite_power(owb, &parasitic_power);
	if (parasitic_power) {
		owb_use_parasitic_power(owb, parasitic_power);
//		printf("Parasitic-powered devices detected");
	}


	//INITIALISING TASKS
	xTaskCreate(status_led,		"status_led",		2048, NULL, 10, NULL);
	gb_moduleID = fcheckModuleId();
	if (gb_moduleID != (uint8_t)MODULE_ID_HX711)
	{
		ESP_LOGE(TAG_CONF, "ADC MODULE MISMATCH! MODULE(0x%02X) does not equal Firmware(0x%02X)\n", gb_moduleID,MODULE_ID_HX711);
		while(1)
			vTaskDelay(100);
	}
	fstorageInit();

	fserialInit();
	fADCInit();
	fWifiInit();
	ftcpInit();
	fblinkInit();
	fbatMonInit();
	ftempIntInit();

	//STARTING TASKS
	vTaskResume(ht_storageRun);
	vTaskResume(ht_blinkRun);
	vTaskResume(ht_tcpMes);
	vTaskResume(ht_tcpConf);
	vTaskResume(ht_adcRun);
	vTaskResume(ht_wifiRun);
	vTaskResume(ht_batmonRun);
	vTaskResume(ht_serialRun);
//	vTaskResume(ht_t_tempIntRun);
	fconfigInit();
//	while(xTaskNotify(ht_configRun,CMD_init,eSetValueWithoutOverwrite) != pdPASS)
//		vTaskDelay(1/ portTICK_PERIOD_MS);


	while(1)
		vTaskDelay(100);
}



uint8_t fcheckModuleId ()
{
	uint8_t b_moduleId = 0;
	gpio_set_level(PIN_MODULE_ID_CLK, 0);
	gpio_set_level(PIN_MODULE_ID_SELECT, 1);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	gpio_set_level(PIN_MODULE_ID_SELECT, 0);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	for (int i = 0; i<8; i++)
	{
		b_moduleId |= (gpio_get_level(PIN_MODULE_ID_DOUT) << (7 - i));
		gpio_set_level(PIN_MODULE_ID_CLK, 1);
		vTaskDelay(1 / portTICK_PERIOD_MS);
		gpio_set_level(PIN_MODULE_ID_CLK, 0);
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
	gpio_set_level(PIN_MODULE_ID_SELECT, 1);
	return b_moduleId;
}

