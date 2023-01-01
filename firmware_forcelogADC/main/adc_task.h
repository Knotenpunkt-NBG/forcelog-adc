/*
 * adc_read.h
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#ifndef MAIN_ADC_H_
#define MAIN_ADC_H_

//#define ADC_MODULE_PATH_HEADER ${CONFIG_ADC_PATH} ## /adc_routines.h

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <nvs_flash.h>
#include "esp_log.h"

#include "esp_timer.h"
#include "sdkconfig.h"
#include <unistd.h>
#include "math.h"


#include "globals.h"
#include "aux.h"


void		fADCInit		(void);
int32_t		freadAdc		(stu_mesCell*, int32_t* i32_limit);

void		fADCConfig		(uint32_t CMDlet);
double		fcalADC			(double d_calWheight, uint32_t i_tareValue);
int			fsetADCSpeed	(uint64_t ui_adcPeriod);
int32_t		fTareADC		(void);
double		fcalADC			(double d_calWheight, uint32_t i_tareValue);

int32_t fgetLimit (uint8_t ui8_maxLoad, int8_t i8_perOverload, int32_t	i32_tareZero);

void		tadcRun			(void *arg);

void		fadcTimerCallback(void* param);




#endif /* MAIN_ADC_H_ */
