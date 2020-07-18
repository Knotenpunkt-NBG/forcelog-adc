#include "main.h"


wifi_config_t sta_config = {
		.sta = {
				.ssid = "",
				.password = "",
				.bssid_set = 0
		}
};





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
			printf("retry to connect to the AP");
		} else {
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		printf("connect to the AP fail");

	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		printf("got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}

static void serial_config(void *arg) //TODO:fix bug with SSID (does not accept full string)
{
	while(1) {
		sendRgbLedStatus(LEDC_STATUS_WAITING_SERIAL);
		printf("ack\n");
		printf("Entering configuration mode. Send man for manual\n");
		sendRgbLedStatus(LEDC_STATUS_WAITING_SERVER);

		while (1){
			uint32_t i_cmdlet = 0;
			while ((i_cmdlet = readUartCmdlet()) == 0);
			switch(i_cmdlet)
			{
			case ('m' << 8 * 3) + ('a' << 8 * 2) + ('n' << 8 * 1) + (0xFF << 8 * 0):
								printf("ack\n");
			printf("man\n");
			break;

			case ('s' << 8 * 3) + ('s' << 8 * 2) + ('i' << 8 * 1) + ('d' << 8 * 0):
			printf("ack\n");
			printf("Enter SSID:\n");
			readUartString((char *)sta_config.sta.ssid, 33);
			printf("ack\n");
			printf("SSID:%s\n", (char*) sta_config.sta.ssid);
			break;

			case ('p' << 8 * 3) + ('a' << 8 * 2) + ('s' << 8 * 1) + ('s' << 8 * 0):
			printf("ack\n");
			printf("Enter password:\n");
			readUartString((char *)sta_config.sta.password, 64);
			printf("ack\n");
			break;

			case ('s' << 8 * 3) + ('v' << 8 * 2) + ('i' << 8 * 1) + ('p' << 8 * 0):
			case ('i' << 8 * 3) + ('p' << 8 * 2) + (0xFF << 8 * 1) + (0xFF << 8 * 0):
			printf("ack\n");
			printf("Enter server IP:\n");
			char* s_ip[16] = {};
			readUartString((char *)s_ip, 16);
			inet_pton(AF_INET, (char*)s_ip, &str_serverAddressMes.sin_addr.s_addr);
			inet_pton(AF_INET, (char*)s_ip, &str_serverAddressConf.sin_addr.s_addr);
			printf("ack\n");
			break;

			case ('p' << 8 * 3) + ('c' << 8 * 2) + ('o' << 8 * 1) + ('n' << 8 * 0):
			printf("ack\n");
			printf("Enter server port for config:\n");
			char* s_portConf[16] = {};
			readUartString((char*)s_portConf, 16);
			str_serverAddressConf.sin_port = htons(atoi((char*)s_portConf));
			printf("ack\n");
			printf("Config Port:%d\n", str_serverAddressConf.sin_port);
			break;

			case ('p' << 8 * 3) + ('m' << 8 * 2) + ('e' << 8 * 1) + ('s' << 8 * 0):
			printf("ack\n");
			printf("Enter server port for measurements:\n");
			char* s_portMes[16] = {};
			readUartString((char*)s_portMes, 16);
			str_serverAddressMes.sin_port = htons(atoi((char*)s_portMes));
			printf("ack\n");
			printf("Measurements Port:%d\n", str_serverAddressMes.sin_port);
			break;

			case ('s' << 8 * 3) + ('a' << 8 * 2) + ('v' << 8 * 1) + ('e' << 8 * 0):
			case ('s' << 8 * 3) + ('a' << 8 * 2) + ('v' << 8 * 1) + (0xFF << 8 * 0):
				printf("ack\n");
				printf("Saving network config to non volative memory...\n");
				str_serverAddressConf.sin_port = 37648;
				printf("ConfPort:%d\n", str_serverAddressConf.sin_port);
				nvs_open("storage", NVS_READWRITE, &handle_nvs);
				nvs_set_u16(handle_nvs, "k_serverPortMes", str_serverAddressMes.sin_port);
				nvs_set_u16(handle_nvs, "k_srvPortConf", str_serverAddressConf.sin_port);
				nvs_set_u32(handle_nvs, "k_serverIp", str_serverAddressMes.sin_addr.s_addr);
				nvs_set_u32(handle_nvs, "k_serverIp", str_serverAddressConf.sin_addr.s_addr);
				nvs_set_str(handle_nvs, "k_ssid", (char*) sta_config.sta.ssid);
				nvs_set_str(handle_nvs, "k_pass", (char*) sta_config.sta.password);
				nvs_commit(handle_nvs);
				nvs_close(handle_nvs);
				printf("ack\n");
				printf("Saved. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
				printf("Saved Mes. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
				printf("Saved Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);
			break;

			case ('l' << 8 * 3) + ('o' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0):
				nvs_open("storage", NVS_READWRITE, &handle_nvs);
				nvs_get_u16(handle_nvs, "k_serverPortMes", &str_serverAddressMes.sin_port);
				nvs_get_u16(handle_nvs, "k_srvPortConf", &str_serverAddressConf.sin_port);
				nvs_get_u32(handle_nvs, "k_serverIp", &str_serverAddressMes.sin_addr.s_addr);
				nvs_get_u32(handle_nvs, "k_serverIp", &str_serverAddressConf.sin_addr.s_addr);
				size_t required_size = 0;
				nvs_get_str(handle_nvs, "k_ssid", NULL, &required_size);
				nvs_get_str(handle_nvs, "k_ssid", (char*) sta_config.sta.ssid, &required_size);
				nvs_get_str(handle_nvs, "k_pass", NULL, &required_size);
				nvs_get_str(handle_nvs, "k_pass", (char*) sta_config.sta.password, &required_size);
				nvs_close(handle_nvs);
				printf("Load. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
				printf("Load Mes. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
				printf("Load Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);
			break;

			case ('s' << 8 * 3) + ('t' << 8 * 2) + ('r' << 8 * 1) + ('t' << 8 * 0):
				printf("ack\n");
				printf("Connecting to AP. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);
				printf("Connecting to Socket MES. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
				printf("Connecting to Socket Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);

				xTaskCreate(connectWifi, "connectWifi", 2048, NULL, 10, NULL);
				xEventGroupSetBits(eg_uart, BIT_CONFIG_FINISHED);
				vTaskDelete(NULL);
			break;

			default:
				printf("inv\n");
				break;
			}
		}
	}
}

static void serial_idle(void *arg)	//TODO: add automatic UART Check
{
	/*printf("conf\n");
	vTaskDelay(100 / portTICK_PERIOD_MS);
	if (readUartCmdlet() == ('a' << 8 * 3) + ('c' << 8 * 2) + ('k' << 8 * 1) + (0xFF << 8 * 0)) //code:ack
	{
		xTaskCreate(serial_config, "serial_config", 2048, NULL, 10, NULL);
		xEventGroupWaitBits(eg_uart,BIT_CONFIG_FINISHED,true,true,portMAX_DELAY);
	}*/
	while(1)
	{
		while (
				(readUartCmdlet() != (('c' << 8 * 3) + ('o' << 8 * 2) + ('s' << 8 * 1) + ('e' << 8 * 0))) //code: cose
		)
		{
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
		xTaskCreate(serial_config, "serial_config", 2048, NULL, 10, NULL);
		xEventGroupWaitBits(eg_uart,BIT_CONFIG_FINISHED,true,true,portMAX_DELAY);
	}
}

static void connectWifi(void *arg) {
	printf("Connecting to AP. SSID:%s PASS:%s\n", (char*) sta_config.sta.ssid, (char*) sta_config.sta.password);

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);
	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler,
			NULL);
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler,
			NULL);
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
	q_rgb_status 	=	xQueueCreate( 10, sizeof(uint32_t));
	q_measurement 	=	xQueueCreate( 3, sizeof(double));
	q_time 			=	xQueueCreate( 3, sizeof(uint64_t));
	q_conf		=	xQueueCreate( 2, sizeof(double));
	q_tcpConf		=	xQueueCreate(10, sizeof(int));
	q_cmdlet		=	xQueueCreate(5, sizeof(int));

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
	tcpip_adapter_init();
	esp_event_loop_create_default();

	//Creating Event Groups
	eg_adc		=	xEventGroupCreate();
	eg_tcp		=	xEventGroupCreate();
	eg_uart		=	xEventGroupCreate();
	eg_blink	=	xEventGroupCreate();

	//Loading Config
	nvs_open("storage", NVS_READWRITE, &handle_nvs);
	nvs_get_u16(handle_nvs, "k_serverPortMes", &str_serverAddressMes.sin_port);
	nvs_get_u16(handle_nvs, "k_srvPortConf", &str_serverAddressConf.sin_port);
	nvs_get_u32(handle_nvs, "k_serverIp", &str_serverAddressMes.sin_addr.s_addr);
	nvs_get_u32(handle_nvs, "k_serverIp", &str_serverAddressConf.sin_addr.s_addr);
	size_t required_size = 0;
	nvs_get_str(handle_nvs, "k_ssid", NULL, &required_size);
	nvs_get_str(handle_nvs, "k_ssid", (char*) sta_config.sta.ssid, &required_size);
	nvs_get_str(handle_nvs, "k_pass", NULL, &required_size);
	nvs_get_str(handle_nvs, "k_pass", (char*) sta_config.sta.password, &required_size);
	nvs_close(handle_nvs);

	printf("Connecting to Socket MES. IP:%d PORT:%d\n", str_serverAddressMes.sin_addr.s_addr, str_serverAddressMes.sin_port);
	printf("Connecting to Socket Conf. IP:%d PORT:%d\n", str_serverAddressConf.sin_addr.s_addr, str_serverAddressConf.sin_port);
	//Creating tasks

	xTaskCreate(serial_idle, "serial_idle", 1024, NULL, 10, NULL);
	xTaskCreate(t_tcpInit, "t_tcpInit", 4096, NULL, 10, NULL);
	xTaskCreate(status_led, "status_led", 1024, NULL, 10, NULL);
	xTaskCreate(t_ADCInit, "t_ADCInit", 2048, NULL, 10, NULL);
	xTaskCreate(t_blinkInit, "t_blinkInit", 2048, NULL, 10, NULL);
	//xTaskCreate(connectWifi, "connectWifi", 2048, NULL, 10, NULL);
}


char* readUartString(char* out, int i_maxNumChars) {
	uint8_t *data = (uint8_t *) malloc(i_maxNumChars);
	while(1)
	{
		int i_len = uart_read_bytes(UART_NUM_0, data, i_maxNumChars, 20 / portTICK_RATE_MS);
		for (int i = 0; i < i_len; i++)
		{
			if((data[i]== '\r') || (data[i] == '\n') || (data[i] == '\0'))
			{
				out[i] = '\0';
			}
			else
			{
				out[i] = data[i];
			}
		}
		out[i_len+1] = '\0';
		if(out[0] != '\0')
			break;
	}
	free(data);
	return out;
}

int readUartCmdlet()
{
	int i_cmdlet = 0;
	uint8_t *data = (uint8_t *) malloc(6);
	while (uart_read_bytes(UART_NUM_0, data, 6, 20 / portTICK_RATE_MS) == 0)
		vTaskDelay(1 / portTICK_PERIOD_MS);
	for(int i = 0; i<4; i++)
	{
		i_cmdlet += (data[i] << (8*(3-i)));
	}
	free(data);
	return i_cmdlet;
}

void sendRgbLedStatus(int i)
{
	xQueueSend(q_rgb_status, &i, 100/portTICK_PERIOD_MS);
}



