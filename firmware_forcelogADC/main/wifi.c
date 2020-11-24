/*
 * wifi.c
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#include "wifi.h"
//TODO: add AP Mode and reconnect if wifi is lost
static int s_retry_num = 0; //number of retries for wifi as station

static void event_handler_sta(void* arg, esp_event_base_t event_base,
		int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
	{
		esp_wifi_connect();
	}
	else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
	{
		if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
		{
			esp_wifi_connect();
			s_retry_num++;
			ESP_LOGI(TAG_WIFI, "retry to connect to the AP");
		}
		else
		{
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		ESP_LOGI(TAG_WIFI,"connect to the AP fail");
	}
	else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
	{
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(TAG_WIFI, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		while(xTaskNotify(ht_tcpConf,CMD_conn,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}

static void event_handler_ap(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG_WIFI, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG_WIFI, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void fWifiInit(void)
{
	xTaskCreate(tWifiRun,		"tWifiRun",		4096, NULL, 10, &ht_wifiRun);
}

void tWifiRun(void* param)
{
	vTaskSuspend(NULL);
	uint32_t ui_cmdlet = 0;
	wifi_config_t stu_wifiConfig_mom;

	while(1)
	{
		xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		fWifiConfig(ui_cmdlet, &stu_wifiConfig_mom);
		vTaskDelay(1);
	}

}

void fWifiConfig(uint32_t ui_cmdlet, wifi_config_t* stu_wifiConfig_mom)
{
	switch(ui_cmdlet)
	{
	case CMD_save:
	case CMD_load:
	case CMD_defs:
	case CMD_svwi:
	case CMD_ldwi:
	case CMD_defl:
	case CMD_ssid:
	case CMD_pass:
		xQueueSend(q_pointer,&stu_wifiConfig_mom, portMAX_DELAY);
		vTaskSuspend( NULL );
		break;
	case CMD_conn:
		fWifiConnSTA(stu_wifiConfig_mom);

		break;
	default:
		ESP_LOGD(TAG_WIFI, "WRONG CMDLET");
		break;
	}
}

void fWifiConnSTA(wifi_config_t* stu_wifiConfig_mom)
{
	ESP_LOGI(TAG_WIFI, "STARTING CONNECTING");
	s_wifi_event_group = xEventGroupCreate();

	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler_sta, NULL));
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler_sta, NULL));

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, stu_wifiConfig_mom));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI(TAG_WIFI, "fWifiInit finished.");

	/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
	 * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler_sta() (see above) */
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
			WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
			pdFALSE,
			pdFALSE,
			portMAX_DELAY);

	/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
	 * happened. */
	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG_WIFI, "connected to ap SSID:%s password:%s",
				stu_wifiConfig_mom->sta.ssid, stu_wifiConfig_mom->sta.password);
		while(xTaskNotify(ht_tcpConf,CMD_conn,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		while(xTaskNotify(ht_tcpMes,CMD_conn,eSetValueWithoutOverwrite) != pdPASS)
					vTaskDelay(1/ portTICK_PERIOD_MS);

	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(TAG_WIFI, "|Failed to connect to SSID:%s, password:%s|",
				stu_wifiConfig_mom->sta.ssid, stu_wifiConfig_mom->sta.password);
	} else {
		ESP_LOGE(TAG_WIFI, "UNEXPECTED EVENT");
	}

	ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler_sta));
	ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler_sta));
	vEventGroupDelete(s_wifi_event_group);
}

void fWifiConnAp(wifi_config_t* wifiConfig_mom)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler_ap, NULL));


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifiConfig_mom));
    ESP_ERROR_CHECK(esp_wifi_start());


}
