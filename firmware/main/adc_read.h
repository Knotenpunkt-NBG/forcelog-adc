/*
 * adc_read.h
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#ifndef MAIN_ADC_READ_H_
#define MAIN_ADC_READ_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <nvs_flash.h>

#include "esp_timer.h"
#include "sdkconfig.h"
#include <unistd.h>

#include "defines.h"
#include "events.h"
#include "queues.h"



nvs_handle_t h_nvs_ADC;
TaskHandle_t xHandle;
esp_timer_handle_t periodic_timer;



void periodic_timer_callback(void* arg);


void t_ADCidle(void *arg);
void t_ADCconfig (void *arg);
void t_ADCInit (void *arg);
void t_ADCrun(void *arg);

void loadAdcConf(void);
void saveAdcConf(void);

uint32_t readAdc (void);
uint32_t tareADC (void);
double calADC (double d_calWheight);
void setADCSpeed(uint32_t ui_ADCSpeed);


struct {
   uint32_t ui_tareValue;
   double d_calValue;
   uint64_t ul_period;
   uint32_t ui_ADCSpeed;
}struct_ADCConf;



#endif /* MAIN_ADC_READ_H_ */
