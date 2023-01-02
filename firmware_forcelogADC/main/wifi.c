/*
 * wifi.c
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#include "wifi.h"


static void cb_Wifi(void* arg, esp_event_base_t event_base,
		int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
	{
		wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
		ESP_LOGI(TAG_WIFI, "station "MACSTR" connected, AID=%d",
				MAC2STR(event->mac), event->aid);
	}

	else if(event_base == IP_EVENT && event_id == IP_EVENT_AP_STAIPASSIGNED)
	{

		xEventGroupSetBits(eg_wifi, WIFI_IPASSIGNED_BIT);
	}

	else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
	{
		wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
		ESP_LOGI(TAG_WIFI, "station "MACSTR" leave, AID=%d",
				MAC2STR(event->mac), event->aid);
	}

	else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
	{
//		ESP_LOGD(TAG_WIFI, "GOT IP");
		xEventGroupSetBits(eg_wifi, WIFI_CONNECTED_BIT);
	}
	else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
	{
		xEventGroupSetBits(eg_wifi, WIFI_FAIL_BIT);
	}
}

void cb_staTimeout(void* arg)
{
	ESP_LOGD(TAG_WIFI, "TIMEOUT TIMER");
	xEventGroupSetBits(eg_wifi, WIFI_TIMEOUT_BIT);
}

void fWifiInit(void)
{
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	eg_wifi = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &cb_Wifi, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &cb_Wifi, NULL));
	xTaskCreate(tWifiRun,		"tWifiRun",		4096, NULL, 9, &ht_wifiRun);
}

void tWifiRun(void* param)
{
	uint32_t ui_cmdlet = 0;
	while(1)
	{
		while(xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, 50 / portTICK_PERIOD_MS) != 0)
		{
			fWifiConfig(ui_cmdlet);
		}

		EventBits_t bits = xEventGroupWaitBits(eg_wifi,
				0xFFFFFF,
				false,
				false,
				100 / portTICK_PERIOD_MS);

		if(bits)
		{
			if (bits & WIFI_IPASSIGNED_BIT)
			{
			    if(ht_wifiBroadcast)
			    {
			    	vTaskDelete(ht_wifiBroadcast);
			    	ht_wifiBroadcast = 0;
			    }
			    xTaskCreate(tWifiBroadcast,		"tWifiBroadcast",		4096, NULL, 5, &ht_wifiBroadcast);
				xEventGroupSetBits(eg_status, STATUS_WIFI_CONN);
				xEventGroupClearBits(eg_wifi, WIFI_IPASSIGNED_BIT);
			}
			else if (bits & WIFI_CONNECTED_BIT)
			{
				if (htim_staTimeOut)
				{
					ESP_LOGD(TAG_WIFI, "DELETING TIMEOUT TIMER");
					xTimerDelete(htim_staTimeOut, portMAX_DELAY);
					htim_staTimeOut = 0;
				}
				else
				{
					ESP_LOGD(TAG_WIFI, "CONNECTED NO TIMEOUT TIMER PRESENT");
				}

				xEventGroupSetBits(eg_status, STATUS_WIFI_CONN);
				*pstu_staConfig = *pstu_staConfigMom;
			    if(ht_wifiBroadcast)
			    {
			    	vTaskDelete(ht_wifiBroadcast);
			    	ht_wifiBroadcast = 0;
			    }
				xTaskCreate(tWifiBroadcast,		"tWifiBroadcast",		4096, NULL, 5, &ht_wifiBroadcast);
				xEventGroupClearBits(eg_wifi, WIFI_CONNECTED_BIT);
			}
			else if (bits & WIFI_FAIL_BIT)
			{
				xEventGroupClearBits(eg_status, STATUS_WIFI_CONN);
				if (bits & WIFI_TIMEOUT_BIT)
				{
					xTimerDelete(htim_staTimeOut, portMAX_DELAY);
					htim_staTimeOut = 0;
					xEventGroupClearBits(eg_wifi, WIFI_TIMEOUT_BIT);
					fWifiConnAp();
				}
				else
				{
					if (!htim_staTimeOut)
					{
						if (pstu_staConfigMom->ui_wifiTimeout)
						{
							ESP_LOGD(TAG_WIFI, "CREATING WIFI TIMEOUT");
							htim_staTimeOut = xTimerCreate("wifiTimeout",
									pstu_staConfigMom->ui_wifiTimeout * 1000 / portTICK_PERIOD_MS,
									pdFALSE,
									NULL,
									cb_staTimeout);
							xTimerStart(htim_staTimeOut, portMAX_DELAY);
						}
						else
						{
							ESP_LOGD(TAG_WIFI, "NO TIMEOUT SPECIFIED");
						}
					}
					esp_wifi_connect();
					vTaskDelay(100 / portTICK_PERIOD_MS);
				}

				xEventGroupClearBits(eg_wifi, WIFI_FAIL_BIT);
			}
			else
			{
				ESP_LOGE(TAG_WIFI, "UNEXPECTED EVENT:0x%X", (unsigned int) bits);
			}

		}
	}

}

void tWifiBroadcast(void* param)
{
	uint32_t ui_charCount = 0;
	char ac_broadCast[128];
	esp_netif_ip_info_t ip_info;
	esp_netif_get_ip_info(h_netif, &ip_info);
	char ipConf[17];
	inet_ntop(AF_INET, &ip_info.ip, ipConf, sizeof(ipConf));
	uint8_t mac[6];
	esp_read_mac(mac, 0);
	ui_charCount += sprintf(ac_broadCast,
				"\1\2FLADC\3_"MODULE_NAME"_\2%X-%X-%X-%X-%X-%X\3 at \2%s\3:",
				mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ipConf);
	struct sockaddr_in dest_addr;
	if (xEventGroupGetBits(eg_status) & STATUS_HOTSPOT)
	{
		sprintf(ac_broadCast + ui_charCount, "\2%d\3\4\n", pstu_hotspotConfig->portConf);
		dest_addr.sin_port = htons(pstu_hotspotConfig->portBroad);
	}
	else
	{
		sprintf(ac_broadCast + ui_charCount, "\2%d\3\4\n", pstu_staConfigMom->portConf);
		dest_addr.sin_port = htons(pstu_staConfigMom->portBroad);
	}
	dest_addr.sin_addr.s_addr = ip_info.ip.addr | (~ip_info.netmask.addr);
	dest_addr.sin_family = AF_INET;

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	char ac_broadcastAddress[17];
	esp_ip4addr_ntoa((esp_ip4_addr_t *)&(dest_addr.sin_addr.s_addr), ac_broadcastAddress, 17);
	if (sock < 0) {
		ESP_LOGE(TAG_TCP, "Unable to create socket: errno %d", errno);
	}
	else
	{
		ESP_LOGI(TAG_TCP, "Socket created, sending to %s:%d", ac_broadcastAddress, ntohs(dest_addr.sin_port));
		ESP_LOGD(TAG_WIFI, "SENDING BROADCAST %s", ac_broadCast);
		while (1)
		{
			if (xEventGroupGetBits(eg_status) & STATUS_WIFI_CONN)
			{
				int err = sendto(sock, ac_broadCast, strlen(ac_broadCast), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
				if (err < 0)
				{
					ESP_LOGE(TAG_TCP, "Error occurred during sending: errno %d", errno);
				}
			}
			vTaskDelay(2000 / portTICK_PERIOD_MS);
			if (xEventGroupGetBits(eg_status) & STATUS_TCPCONF_CONN)
			{
				vTaskDelete(NULL);
			}
		}
	}


}

void	fWifiConfig			(uint32_t ui_cmdlet)
{
	char* pc_configOut = 0;
	char* pc_configIn = 0;
	char* pc_value = 0;

	switch(ui_cmdlet)
	{
	case CMD_ldwi:
	case CMD_inwi:
		xEventGroupSync( eg_sync,
				BIT_WIFI_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_config,
				BIT_WIFI_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC | BIT_STORAGE_SYNC,
				portMAX_DELAY );
		break;

	case CMD_ssid:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tEnter ssid.\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			pc_configOut = malloc(200);
			portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
			portENTER_CRITICAL(&mux);
			strcpy(pstu_staConfigMom->ac_ssid, pc_value);
			portEXIT_CRITICAL(&mux);

			sprintf(pc_configOut,
					"\1|\2ssid\3|\2%s\3\4|\t\tOK, ssid set to %s.\n",
					pstu_staConfigMom->ac_ssid, pstu_staConfigMom->ac_ssid);
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_WIFI_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_pass:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tEnter password.\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			pc_configOut = malloc(200);
			portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
			portENTER_CRITICAL(&mux);
			strcpy(pstu_staConfigMom->ac_pass, pc_value);
			portEXIT_CRITICAL(&mux);

			sprintf(pc_configOut,
					"|pass|%s|\t\tOK, password set to %s.\n",
					pstu_staConfigMom->ac_pass, pstu_staConfigMom->ac_pass);
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_WIFI_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_coap:
		fWifiConnAp();
		xEventGroupSync( eg_sync,
				BIT_WIFI_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;
	case CMD_cost:
		fWifiConnSTA();
		xEventGroupSync( eg_sync,
				BIT_WIFI_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_init:
		xEventGroupSync( eg_config,
				BIT_WIFI_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		xEventGroupSync( eg_sync,
				BIT_WIFI_SYNC ,
				BIT_CONFIG_SYNC
					| BIT_ADC_SYNC
					| BIT_TCPMES_SYNC
					| BIT_TCPCONF_SYNC
					| BIT_WIFI_SYNC
					| BIT_BLINK_SYNC
					| BIT_BATMON_SYNC
					| BIT_STORAGE_SYNC,
				portMAX_DELAY );
		break;
	case CMD_scwi:
		xEventGroupSync( eg_config,
				BIT_WIFI_SYNC,
				BIT_WIFI_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		wifi_ap_record_t* ap_info = malloc(sizeof(wifi_ap_record_t) * MAX_NUM_AP_SCAN);
		uint32_t ap_count = fwifiScan(ap_info);
		xQueueSend(q_recv,&ap_count, portMAX_DELAY);
		xQueueSend(q_recv,&ap_info, portMAX_DELAY);
		ESP_LOGD(TAG_WIFI, "FINISHED SCWI");
		break;

	default:
		ESP_LOGD(TAG_WIFI, "WRONG CMDLET");
		break;
	}
}

void fWifiConnSTA()
{
	if (h_netif)
	{
		esp_netif_destroy(h_netif);
	}
	h_netif = esp_netif_create_default_wifi_sta();
//	esp_netif_set_hostname(h_netif, gstu_config->staConfig.ac_hostName);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    xEventGroupClearBits(eg_status, STATUS_HOTSPOT);
    xEventGroupSetBits(eg_status, STATUS_STATION);

//    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &cb_Wifi, NULL));
//    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &cb_Wifi, NULL));
//    esp_netif_dhcp_status_t* status = malloc(sizeof (esp_netif_dhcp_status_t));
//    ESP_ERROR_CHECK(esp_netif_dhcps_stop(h_netif));
//    ESP_ERROR_CHECK(esp_netif_dhcpc_start(h_netif));

    wifi_config_t station = { .sta = {
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,
         .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    strcpy((char*)station.sta.password, pstu_staConfigMom->ac_pass);
    strcpy((char*)station.sta.ssid, pstu_staConfigMom->ac_ssid);

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &station));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGD(TAG_WIFI, "CONNECTING TO %s %s.", station.sta.ssid, station.sta.password);
	esp_wifi_connect();
//	vEventGroupDelete(eg_wifi);
}

void fWifiConnAp()
{
	if (h_netif)
	{
		esp_netif_destroy(h_netif);
	}
	ESP_LOGD(TAG_WIFI, "STARTING AP");
	h_netif = esp_netif_create_default_wifi_ap();
	ESP_ERROR_CHECK(esp_netif_set_hostname(h_netif, pstu_hotspotConfig->ac_hostName));
	ESP_ERROR_CHECK(esp_netif_dhcps_stop(h_netif));
	ESP_ERROR_CHECK(esp_netif_set_ip_info(h_netif, &pstu_hotspotConfig->ipInfo));
	ESP_ERROR_CHECK(esp_netif_dhcps_start(h_netif));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    xEventGroupClearBits(eg_status, STATUS_STATION);
    xEventGroupSetBits(eg_status, STATUS_HOTSPOT);

    wifi_config_t accessPoint= {
            .ap = {
                .ssid = "",
                .ssid_len = strlen(""),
                .channel = 0,
                .password = "",
                .max_connection = 0,
                .authmode = 0
            },
        };

    strcpy((char*)accessPoint.ap.password, pstu_hotspotConfig->ac_pass);
    strcpy((char*)accessPoint.ap.ssid, pstu_hotspotConfig->ac_ssid);
    accessPoint.ap.channel = 9;
    accessPoint.ap.max_connection = 2;
    accessPoint.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    accessPoint.ap.ssid_len = strlen(pstu_hotspotConfig->ac_ssid);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &accessPoint));

    ESP_ERROR_CHECK(esp_wifi_start());

}


uint32_t fwifiScan(wifi_ap_record_t* ap_info)
{
	if (h_netif)
	{
		esp_netif_destroy(h_netif);
	}
	h_netif = esp_netif_create_default_wifi_sta();
	assert(h_netif);
	xEventGroupClearBits(eg_status, STATUS_HOTSPOT);
	xEventGroupSetBits(eg_status, STATUS_STATION);

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	uint16_t number = MAX_NUM_AP_SCAN;

	uint16_t ap_count = 0;
	memset(ap_info, 0, sizeof(*ap_info));

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_start());
	esp_wifi_scan_start(NULL, true);
	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

	//SORTING ENTRIES IN DESCENDING ORDER OF SIGNAL STRENGTH
	for (int i = 0; (i < MAX_NUM_AP_SCAN) && (i < ap_count); i++) {
		for (int j = 0; (j < (MAX_NUM_AP_SCAN - i - 1)) && (j < (ap_count - i - 1)); j++)
		{
			if(ap_info[j].rssi < ap_info[j+1].rssi)
			{
				fswapApInfo(&ap_info[j], &ap_info[j+1]);
			}
		}
	}
	uint16_t us_numAP = ap_count;
	for (int i = 0; i < ap_count; i++)
	{
		if (ap_info[i].rssi <= MIN_SIGNAL_DBM)
		{
			us_numAP--;
		}
	}
	ap_count = us_numAP;
	ESP_LOGD(TAG_WIFI, "WIFI SCAN FINISHED NUMBER FOUND:%d", ap_count);
	esp_wifi_stop();
	return (uint32_t)ap_count;
}

void print_cipher_type(int pairwise_cipher, int group_cipher)
{
    switch (pairwise_cipher) {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG_WIFI, "Pairwise Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG_WIFI, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG_WIFI, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG_WIFI, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG_WIFI, "Pairwise Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG_WIFI, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    default:
        ESP_LOGI(TAG_WIFI, "Pairwise Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }

    switch (group_cipher) {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG_WIFI, "Group Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG_WIFI, "Group Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG_WIFI, "Group Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG_WIFI, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG_WIFI, "Group Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG_WIFI, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    default:
        ESP_LOGI(TAG_WIFI, "Group Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }
}

void print_auth_mode(int authmode)
{
    switch (authmode) {
    case WIFI_AUTH_OPEN:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_OPEN");
        break;
    case WIFI_AUTH_WEP:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_WEP");
        break;
    case WIFI_AUTH_WPA_PSK:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_WPA_PSK");
        break;
    case WIFI_AUTH_WPA2_PSK:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA_WPA2_PSK:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_WPA_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA2_ENTERPRISE:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_WPA2_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA3_PSK:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_WPA3_PSK");
        break;
    case WIFI_AUTH_WPA2_WPA3_PSK:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_WPA2_WPA3_PSK");
        break;
    default:
        ESP_LOGI(TAG_WIFI, "Authmode \tWIFI_AUTH_UNKNOWN");
        break;
    }
}

void fswapApInfo(wifi_ap_record_t* apInfoA, wifi_ap_record_t* apInfoB)
{
	wifi_ap_record_t apInfoTemp;
	apInfoTemp = *apInfoA;
	*apInfoA = *apInfoB;
	*apInfoB = apInfoTemp;
}
