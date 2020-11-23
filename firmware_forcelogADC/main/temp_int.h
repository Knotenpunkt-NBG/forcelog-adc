/*
 * temperature.h
 *
 *  Created on: Oct 19, 2020
 *      Author: lor
 */

#ifndef MAIN_TEMP_INT_H_
#define MAIN_TEMP_INT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "defines.h"
#include "globals.h"

#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"




#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)


void ftempIntInit(void);
void ttempIntRun(void* param);



#endif /* MAIN_TEMP_INT_H_ */
