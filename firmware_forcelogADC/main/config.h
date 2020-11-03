/*
 * config.h
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

#include <lwip/sockets.h>
#include "adc.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_system.h"
#include <driver/adc.h>
#include "owb.h"
#include "freertos/semphr.h"

#include "globals.h"
#include "defines.h"


int fConfig (int i_cmdlet, char* str_value);

char* fcreateInquireString();
int fSaveConfig(uint32_t nvs_id);
int fLoadConfig(uint32_t nvs_id);
void fscanOwb(void);
uint64_t fuLongFromRomCode (OneWireBus_ROMCode stu_romCode);
void fromCodeFromULong (uint64_t ul_romCode, OneWireBus_ROMCode stu_romCode);

#endif /* MAIN_CONFIG_H_ */
