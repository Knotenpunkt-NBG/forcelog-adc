#include "main.h"

static int s_retry_num = 0; //number of retries for wifi as station
static const char *TAG = "wifi station";

wifi_config_t sta_config = {
    .sta = {
        .ssid = "",
        .password = "",
        /* Setting a password implies station will connect to all security modes including WEP/WPA.
         * However these modes are deprecated and not advisable to be used. Incase your Access point
         * doesn't support WPA2, these mode can be enabled by commenting below line */
     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

        .pmf_cfg = {
            .capable = true,
            .required = false
        },
    },
};

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
	s_wifi_event_group = xEventGroupCreate();

	    ESP_ERROR_CHECK(esp_netif_init());

	    ESP_ERROR_CHECK(esp_event_loop_create_default());
	    esp_netif_create_default_wifi_sta();

	    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
	    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Flauschnetz_2.4Ghz",
            .password = "Hurenscheise42",
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
     ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
     ESP_ERROR_CHECK(esp_wifi_start() );

     ESP_LOGI(TAG, "wifi_init_sta finished.");

     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
      * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
     EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
             WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
             pdFALSE,
             pdFALSE,
             portMAX_DELAY);

     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
      * happened. */
     if (bits & WIFI_CONNECTED_BIT) {
         ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
        		 "Flauschnetz_2.4Ghz", "Hurenscheise42");
     } else if (bits & WIFI_FAIL_BIT) {
         ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
        		 "Flauschnetz_2.4Ghz", "Hurenscheise42");
     } else {
         ESP_LOGE(TAG, "UNEXPECTED EVENT");
     }

     ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
     ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
     vEventGroupDelete(s_wifi_event_group);
 }




void app_main()
{
	esp_log_level_set("*", ESP_LOG_INFO);
	uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);
	serial_init();
	//Creating Semaphores
	hs_blinkConfig = xSemaphoreCreateBinary();

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
	q_conf			=	xQueueCreate(	2,	sizeof(double));
	q_tcpConf		=	xQueueCreate(	10,	sizeof(int));
	q_cmdlet		=	xQueueCreate(	5,	sizeof(int));




	//Creating Event Groups
	eg_adc		=	xEventGroupCreate();
	eg_tcp		=	xEventGroupCreate();
	eg_tcpRun	=	xEventGroupCreate();
	eg_uart		=	xEventGroupCreate();
	eg_blink	=	xEventGroupCreate();

	//Loading Config
	nvs_open("storage", NVS_READWRITE, &hnvs_conf_0);
	nvs_get_u16(hnvs_conf_0, "k_serverPortMes",	&str_serverAddressMes.sin_port);
	nvs_get_u16(hnvs_conf_0, "k_srvPortConf",	&str_serverAddressConf.sin_port);
	nvs_get_u32(hnvs_conf_0, "k_serverIp",		&str_serverAddressMes.sin_addr.s_addr);
	nvs_get_u32(hnvs_conf_0, "k_serverIp",		&str_serverAddressConf.sin_addr.s_addr);
	size_t required_size = 0;
	nvs_get_str(hnvs_conf_0, "k_ssid", NULL, &required_size);
	//nvs_get_str(hnvs_conf_0, "k_ssid", (char*) sta_config.sta.ssid, &required_size);
	nvs_get_str(hnvs_conf_0, "k_pass", NULL, &required_size);
	//nvs_get_str(hnvs_conf_0, "k_pass", (char*) sta_config.sta.password, &required_size);
	nvs_close(hnvs_conf_0);

	//Creating tasks




	xTaskCreate(serial_config,	"serial_config",	4096, NULL, 10, NULL);

	xTaskCreate(status_led,		"status_led",		2048, NULL, 10, NULL);
	xTaskCreate(t_ADCInit,		"t_ADCInit",		2048, NULL, 10, NULL);
	xTaskCreate(t_blinkInit,	"t_blinkInit",		2048, NULL, 10, NULL);
	//xTaskCreate(connectWifi, 	"connectWifi", 		4096, NULL, 10, NULL);


	xTaskCreate(t_tcpInit,		"t_tcpInit",		4096, NULL, 10, NULL);
	wifi_init_sta();
}





