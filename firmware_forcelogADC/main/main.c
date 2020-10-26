#include "main.h"

void app_main()
{
	esp_log_level_set("*", ESP_LOG_ERROR);
	fsetLogLevel(LOG_DEBUG);

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	//Creating Semaphores
	hs_blinkConfig = xSemaphoreCreateBinary();
	hs_wifiConfig = xSemaphoreCreateBinary();

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

	//Creating queues
	q_rgb_status 	=	xQueueCreate(	10, sizeof(uint32_t));
	q_measurement 	=	xQueueCreate(	10, sizeof(double));
	q_time_mes		=	xQueueCreate(	10, sizeof(uint64_t));
	q_time_blink	=	xQueueCreate(	3,	sizeof(uint64_t));
	q_tcpConf		=	xQueueCreate(	10,	sizeof(char *));
	q_temperature	=	xQueueCreate(	3,	sizeof(float));
	q_time_temp		=	xQueueCreate(	3,	sizeof(uint64_t));


	//Creating Event Groups
	eg_adc		=	xEventGroupCreate();
	eg_tcp		=	xEventGroupCreate();
	eg_blink	=	xEventGroupCreate();
	eg_batmon	=	xEventGroupCreate();
	eg_tempInt	=	xEventGroupCreate();

	//Loading Config
	fLoadConfig(hnvs_conf_0);

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


	//Creating tasks
	fbatMonInit();
	fserialInit();
	xTaskCreate(status_led,		"status_led",		2048, NULL, 10, NULL);
	fblinkInit();
	fWifiInit();
	ftcpInit();
	xTaskCreate	(f_tempIntInit,		"f_tempIntInit",		2048,	NULL	,	10,		NULL);

	//f_tempIntInit();
	gb_moduleID = fcheckModuleId();
	if (gb_moduleID == (uint8_t)MODULE_ID_HX711)
	{
		ESP_LOGD(TAG_CONF, "MODULE ID CORRECT:0x%02X\n", gb_moduleID);
		fADCInit();
	}
	else
	{
		ESP_LOGE(TAG_CONF, "ADC MODULE MISMATCH! MODULE(0x%02X) does not equal Firmware(0x%02X)\n", gb_moduleID,MODULE_ID_HX711);
	}

	flocalStorageInit();
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

bool fcmdCheck(int i_cmdlet) //checks if cmdlet needs an aditional string as input
{
	bool b_flag = 0;
	switch (i_cmdlet)
	{
	case CMD_bbrt: //brightness of blink (0-1023)
	case CMD_bdur: //duration of blink
	case CMD_bper: //period of blink
	case CMD_bfrq: //sets frequency of blink (1-78125Hz)
	case CMD_mper: //sets the sample period in us
	case CMD_vcal: //reads calibration value from server and writes it to adc
	case CMD_cali: //calibrates with an known weight and sends the factor back to the server
	case CMD_ssid: //SSID of access point
	case CMD_pass: //pass for access point
	case CMD_ipco: //IP for configuration server
	case CMD_ipme: //IP for logging server
	case CMD_poco: //port for configuration server
	case CMD_pome: //port for logging server
	case CMD_potr: //port for UDP trigger
	case CMD_bath:
	case CMD_batl:
		b_flag = 1;
		break;
	default:
		break;
	}
	return b_flag;
}

