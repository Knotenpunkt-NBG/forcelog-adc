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
#include "aux.h"

void	cb_staTimeout		(void* arg);

void	fWifiInit			(void);

void	tWifiRun			(void* param);

void	tWifiBroadcast		(void* param);

void	fWifiConfig			(uint32_t ui_cmdlet);

void	fWifiConnSTA		(void);

void	fWifiConnAp			(void);

uint32_t	fwifiScan		(wifi_ap_record_t* ap_info);

void	print_cipher_type	(int pairwise_cipher, int group_cipher);

void	print_auth_mode		(int authmode);

void	fswapApInfo			(wifi_ap_record_t* apInfoA, wifi_ap_record_t* apInfoB);

#endif /* MAIN_WIFI_H_ */
