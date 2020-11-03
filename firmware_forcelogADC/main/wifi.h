/*
 * wifi.h
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_

#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include <string.h>

#include "globals.h"
#include "defines.h"

void fWifiInit(void);
void tWifiRun(void*param);
void fWifiConfig(uint32_t ui_cmdlet, wifi_config_t* stu_wifiConfig_mom);
void fWifiConnSTA(wifi_config_t* stu_wifiConfig_mom);

#endif /* MAIN_WIFI_H_ */
