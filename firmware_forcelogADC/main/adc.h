/*
 * adc_read.h
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#ifndef MAIN_ADC_H_
#define MAIN_ADC_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <nvs_flash.h>
#include "esp_log.h"

#include "esp_timer.h"
#include "sdkconfig.h"
#include <unistd.h>

#include "defines.h"
#include "globals.h"

#define GPIO_HX711_DT_PIN 19
#define GPIO_INPUT_ADC ((1ULL<<GPIO_HX711_DT_PIN))
#define GPIO_HX711_SCK_PIN 23
#define GPIO_HX711_RATE_PIN 22
#define GPIO_OUTPUT_ADC ((1ULL<<GPIO_HX711_SCK_PIN) | (1ULL<<GPIO_HX711_RATE_PIN))






uint32_t freadAdc (void);
uint32_t ftareADC (void);

void fadcTimerCallback(void* arg);

void fADCInit (void);
void tadcRun(void *arg);
void fadcConfig(uint32_t CMDlet, struct stu_adcConfig *adcConfig_mom, struct stu_cellConfig *cellConfig_mom);
double fcalADC(double d_calWheight, uint32_t ui_tareValue);
int fsetADCSpeed(uint64_t ui_adcPeriod);

#endif /* MAIN_ADC_H_ */
