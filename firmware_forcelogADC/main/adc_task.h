/*
 * adc_read.h
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#ifndef MAIN_ADC_TASK_H_
#define MAIN_ADC_TASK_H_

//#define ADC_MODULE_PATH_HEADER ${CONFIG_ADC_PATH} ## /adc_routines.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "rom/ets_sys.h"

#include "esp_timer.h"
#include "sdkconfig.h"
#include <unistd.h>
#include "math.h"


#include "globals.h"
#include "aux.h"


void		fADCInit		(void);
int32_t		freadAdc		(stu_mesCell*, int* i32_limit);

void		fADCConfig		(uint32_t CMDlet);
double		fcalADC			(double d_calWheight, uint32_t i_tareValue);
int			fsetADCSpeed	(uint64_t ui_adcPeriod);
int32_t		fTareADC		(void);
double		fcalADC			(double d_calWheight, uint32_t i_tareValue);

int32_t fgetLimit (uint8_t ui8_maxLoad, int8_t i8_perOverload, int	i32_tareZero);

void		tadcRun			(void *arg);

void		fadcTimerCallback(void* param);

int32_t fgetRawADC ();




#endif /* MAIN_ADC_TASK_H_ */
