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



void callback_timerBlink(void* param);

void fblinkInit (void);

void fblinkConfig(uint32_t CMDlet, struct stu_blinkConfig* blinkConfig_mom, ledc_timer_config_t* ledcTimerConfig_mom, ledc_channel_config_t* ledcChannelConfig_mom);




#endif /* MAIN_BLINK_H_ */
