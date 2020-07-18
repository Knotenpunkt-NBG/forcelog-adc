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

#include "esp_timer.h"
#include "sdkconfig.h"
#include <unistd.h>
#include "driver/ledc.h"

#include "defines.h"
#include "events.h"
#include "queues.h"

nvs_handle_t h_nvs_Blink;
TaskHandle_t ht_blinkRun;
esp_timer_handle_t h_timerBlink;

void callback_timerBlink(void* arg);
void t_blinkIdle (void *arg);
void t_blinkRun(void *arg);
void t_blinkConfig (void *arg);
void t_blinkInit (void *arg);

struct {
	uint32_t	ui_blinkPeriod;
	uint32_t	ui_blinkDuration;
	uint32_t	ui_blinkFrequency;
	uint8_t		b_blinkEnabled;
	uint32_t	ui_blinkBrightness;
	uint8_t		b_isRunning;
}struct_blinkConfig;

ledc_timer_config_t ledc_timer;
ledc_channel_config_t ledc_channel;

void saveBlinkConf(void);
void loadBlinkConf(void);



#endif /* MAIN_BLINK_H_ */
