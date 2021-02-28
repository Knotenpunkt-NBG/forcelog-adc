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
#include "trigger.h"


void		tconfigRun				(void* param);
void		fconfigInit				(void);
char*		fcreateInquireString	(void);
char*		fgetValuePointer		(char* pc_value,
									const char* pc_response);

void		finitAll				(void);
void*		fgetConfigFromTask		(TaskHandle_t ht_taskHandle,
									uint32_t ui_cmdlet);
void		freturnMessage			(const char* pc_response);

uint64_t	fuLongFromRomCode		(OneWireBus_ROMCode stu_romCode);
void		fromCodeFromULong		(uint64_t ul_romCode, OneWireBus_ROMCode stu_romCode);
uint32_t	fgetNextString			(char* string);

#endif /* MAIN_CONFIG_H_ */
