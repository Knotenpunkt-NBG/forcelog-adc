#include "main.h"

static int s_retry_num = 0; //number of retries for wifi as station


static void event_handler(void* arg, esp_event_base_t event_base,
		int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			s_retry_num++;
			printf("retry to connect to the AP\n");
		} else {
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		printf("connect to the AP fail\n");

	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		printf("got ip:" IPSTR "\n", IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}

void connectWifi(void *arg) {
	printf("Connecting to AP. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);
	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler,NULL);
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler,NULL);
	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config);
	esp_wifi_start();
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
			WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
			pdFALSE,
			pdFALSE,
			portMAX_DELAY);
	if (bits & WIFI_CONNECTED_BIT) {
		printf("connected to ap SSID:%s password:%s\n",
				(char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
	} else if (bits & WIFI_FAIL_BIT) {
		printf("Failed to connect to SSID:%s, password:%s\n",
				(char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
	} else {
		printf("UNEXPECTED EVENT\n");
	}
	printf("deleting events\n");
	esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler);
	esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler);
	vEventGroupDelete(s_wifi_event_group);
	vTaskDelete(NULL);
}


void app_main()
{
	esp_log_level_set("*", ESP_LOG_INFO);

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
	q_rgb_status 	=	xQueueCreate(	10, sizeof(uint32_t));
	q_measurement 	=	xQueueCreate(	10, sizeof(double));
	q_time_mes		=	xQueueCreate(	10, sizeof(uint64_t));
	q_time_blink	=	xQueueCreate(	3,	sizeof(uint64_t));
	q_conf			=	xQueueCreate(	2,	sizeof(double));
	q_tcpConf		=	xQueueCreate(	10,	sizeof(int));
	q_cmdlet		=	xQueueCreate(	5,	sizeof(int));

	uart_config_t uart_config =
	{
			.baud_rate 	= 	115200,
			.data_bits 	= 	UART_DATA_8_BITS,
			.parity		= 	UART_PARITY_DISABLE,
			.stop_bits	=	UART_STOP_BITS_1,
			.flow_ctrl	=	UART_HW_FLOWCTRL_DISABLE
	};
	uart_param_config(UART_NUM_0, &uart_config);
	uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);

	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL);


	s_wifi_event_group = xEventGroupCreate();
	esp_netif_init();
	esp_event_loop_create_default();

	//Creating Event Groups
	eg_adc		=	xEventGroupCreate();
	eg_tcp		=	xEventGroupCreate();
	eg_tcpRun	=	xEventGroupCreate();
	eg_uart		=	xEventGroupCreate();
	eg_blink	=	xEventGroupCreate();

	//Loading Config
	nvs_open("storage", NVS_READWRITE, &handle_nvs);
	nvs_get_u16(handle_nvs, "k_serverPortMes",	&str_serverAddressMes.sin_port);
	nvs_get_u16(handle_nvs, "k_srvPortConf",	&str_serverAddressConf.sin_port);
	nvs_get_u32(handle_nvs, "k_serverIp",		&str_serverAddressMes.sin_addr.s_addr);
	nvs_get_u32(handle_nvs, "k_serverIp",		&str_serverAddressConf.sin_addr.s_addr);
	size_t required_size = 0;
	nvs_get_str(handle_nvs, "k_ssid", NULL, &required_size);
	nvs_get_str(handle_nvs, "k_ssid", (char*) sta_config.sta.ssid, &required_size);
	nvs_get_str(handle_nvs, "k_pass", NULL, &required_size);
	nvs_get_str(handle_nvs, "k_pass", (char*) sta_config.sta.password, &required_size);
	nvs_close(handle_nvs);

	//Creating tasks

	xTaskCreate(serial_idle,	"serial_idle",	1024, NULL, 10, NULL);
	xTaskCreate(t_tcpInit,		"t_tcpInit",	4096, NULL, 10, NULL);
	xTaskCreate(status_led,		"status_led",	2048, NULL, 10, NULL);
	xTaskCreate(t_ADCInit,		"t_ADCInit",	2048, NULL, 10, NULL);
	xTaskCreate(t_blinkInit,	"t_blinkInit",	2048, NULL, 10, NULL);
	//xTaskCreate(connectWifi, "connectWifi", 2048, NULL, 10, NULL);
}







