/*
 * config.h
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

#include "freertos/FreeRTOS.h"
#include <lwip/sockets.h>
#include "adc_task.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "esp_adc/adc_continuous.h"
//#include "owb.h"
#include "freertos/semphr.h"

#include "globals.h"
#include "trigger.h"
#include "aux.h"


void		tconfigRun				(void* param);

void		fconfigInit				(void);

char*		fcreateInquireString	(void);

void		finitAll				(void);

void*		fgetConfigFromTask		(TaskHandle_t ht_taskHandle,
									uint32_t ui_cmdlet);


#endif /* MAIN_CONFIG_H_ */
