#include "main.h"

void app_main()
{
	esp_log_level_set("*", ESP_LOG_ERROR);
	fsetLogLevel(LOG_DEBUG);

	h_netif = 0;
	ht_wifiBroadcast = 0;
	//Creating Semaphores
	hs_configCom = xSemaphoreCreateBinary();
	xSemaphoreGive(hs_configCom);
	hs_pointerQueue = xSemaphoreCreateBinary();
	xSemaphoreGive(hs_pointerQueue);
	hs_oneWire = xSemaphoreCreateBinary();
	xSemaphoreGive(hs_oneWire);

	//Creating Event Groups
	eg_sync		= xEventGroupCreate();
	eg_config	= xEventGroupCreate();
	eg_status	= xEventGroupCreate();

	//Creating queues q_time_temp
	q_rgb_status 		=	xQueueCreate(	10, sizeof(uint32_t));
	q_tcpMessages		=	xQueueCreate(	10,	sizeof(char *));
	q_uartMessages		=	xQueueCreate(	10,	sizeof(char *));
	q_send				=	xQueueCreate(	1,	sizeof(void *));
	q_recv				=	xQueueCreate(	1,	sizeof(void *));
	q_pconfigIn			=	xQueueCreate(	5,	sizeof(void *));
	q_pconfigOut		=	xQueueCreate(	5,	sizeof(void *));

	q_measurements			=	xQueueCreate(	50,	sizeof(struct stu_mesCell));
	q_measurements_redund	=	xQueueCreate(	50,	sizeof(struct stu_mesCell));


	//CHECKING FOR MODULE ID
	gb_moduleID = fcheckModuleId();


	//STARTING BASIC TASKS FOR INITALISING
	xTaskCreate(status_led,		"status_led",		2048, NULL, 10, NULL);
	fstorageInit();
	fserialInit();

	while(xTaskNotify(ht_storageRun,CMD_init,eSetValueWithoutOverwrite) != pdPASS)
		vTaskDelay(1/ portTICK_PERIOD_MS);
	xEventGroupSync( eg_sync,
			BIT_CONFIG_SYNC ,
			BIT_CONFIG_SYNC | BIT_STORAGE_SYNC,
			portMAX_DELAY );


	//STARTING OTHER TASKS
	fWifiInit();
	ftcpInit();
	fblinkInit();
	fbatMonInit();
	ftempIntInit();
//	if (gb_moduleID == 0x00)
	if(1)
	{
		ESP_LOGE(TAG_CONF, "NO LOADCELL PRESENT. STARTING SIMULATION");
		fsimADCInit();
	}
	else if (gb_moduleID == (uint8_t)MODULE_ID_HX711)
	{
		ESP_LOGE(TAG_CONF, "MODULE MATCH STARTING ADC FOR MODULE(0x%02X)", MODULE_ID_HX711);
		fADCInit();
	}
	else
	{
		ESP_LOGE(TAG_CONF, "ADC MODULE MISMATCH! MODULE(0x%02X) does not equal Firmware(0x%02X)\n", gb_moduleID,MODULE_ID_HX711);
		while(1)
			vTaskDelay(100);
	}
	fconfigInit();

	vTaskDelete(NULL);
}



uint8_t fcheckModuleId ()
{
	gpio_config_t io_conf;
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pin_bit_mask = (1ULL<<PIN_MODULE_ID_DOUT);
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = ((1ULL<<PIN_MODULE_ID_SELECT) | (1ULL<<PIN_MODULE_ID_CLK));
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	uint8_t b_moduleId = 0;
	gpio_set_level(PIN_MODULE_ID_CLK, 0);
	gpio_set_level(PIN_MODULE_ID_SELECT, 1);
	vTaskDelay(1 / portTICK_PERIOD_MS);
	gpio_set_level(PIN_MODULE_ID_SELECT, 0);
	vTaskDelay(1 / portTICK_PERIOD_MS);
	gpio_set_level(PIN_MODULE_ID_SELECT, 1);
	vTaskDelay(1 / portTICK_PERIOD_MS);
	for (int i = 0; i<8; i++)
	{
		vTaskDelay(1 / portTICK_PERIOD_MS);
		b_moduleId |= (gpio_get_level(PIN_MODULE_ID_DOUT) << (7 - i));
		gpio_set_level(PIN_MODULE_ID_CLK, 1);
		vTaskDelay(1/ portTICK_PERIOD_MS);
		gpio_set_level(PIN_MODULE_ID_CLK, 0);
	}
	return b_moduleId;
}

