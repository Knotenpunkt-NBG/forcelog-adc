/*
 * batmon.h
 *
 *  Created on: Oct 17, 2020
 *      Author: lor
 */

#ifndef MAIN_BATMON_H_
#define MAIN_BATMON_H_

#include "freertos/FreeRTOS.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_system.h"
#include <esp_event.h>
#include "sdkconfig.h"
#include "freertos/task.h"
//#include <string.h>

#include "defines.h"
#include "globals.h"


void	fbatMonInit		(void);
void	tbatmonRun		(void* param);
void	callback_timerBatmon	(void* param);
void	fconfigBatMon	(uint32_t ui_cmdlet);
float	fmap			(int x,
						int us_batRawLow,
						float f_batVolLow,
						int us_batRawHigh,
						float f_batVolHigh);


#endif /* MAIN_BATMON_H_ */
