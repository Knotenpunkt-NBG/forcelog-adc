/*
 * blink.h
 *
 *  Created on: Jun 22, 2020
 *      Author: lor
 */

#ifndef MAIN_BLINK_H_
#define MAIN_BLINK_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <nvs_flash.h>
#include "freertos/timers.h"

#include "esp_system.h"
#include "esp_timer.h"
#include "sdkconfig.h"
#include <unistd.h>
#include "driver/ledc.h"
#include <esp_event.h>

#include "defines.h"
#include "globals.h"



void cb_timerBlink	(void* param);

void fblinkInit 	(void);

void fblinkConfig	(uint32_t CMDlet,
					ledc_timer_config_t* ledcTimergstu_config,
					ledc_channel_config_t* ledcChannelgstu_config);

void t_blinkRun		(void *arg);


#endif /* MAIN_BLINK_H_ */
