/*
 * batmon.h
 *
 *  Created on: Oct 17, 2020
 *      Author: lor
 */

#ifndef MAIN_BATMON_H_
#define MAIN_BATMON_H_

#include "freertos/FreeRTOS.h"
#include <driver/adc.h>
#include "esp_system.h"
#include <esp_event.h>
#include "sdkconfig.h"
#include "freertos/task.h"
//#include <string.h>

#include "defines.h"
#include "globals.h"




void fbatMonInit(void);
void t_batmonRun (void* param);
void callback_timerBatmon(void* arg);
void floadBatConfig (void);
float map(int x, int us_batRawLow, float f_batVolLow, int us_batRawHigh, float f_batVolHigh);

#endif /* MAIN_BATMON_H_ */