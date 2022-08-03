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
#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"


#include "globals.h"
#include "aux.h"



#define DS18B20_RESOLUTION   	DS18B20_RESOLUTION_12_BIT		//Resolution in bit. results in a conversion time of around one second



OneWireBus*		owb;
 owb_rmt_driver_info rmt_driver_info;

void		ftempIntInit				(void);
void		ttempIntRun					(void* param);
void		ttempExtRun					(void* param);

void		fscanOwb					(DS18B20_Info* devTempExt,DS18B20_Info* p_probeTempInt);
void		fowbConfig					(uint32_t ui_cmdlet);
uint32_t	fowbcmp						(uint8_t* romCodeA, uint8_t* romCodeB);

DS18B20_ERROR	freadScratchpad				(DS18B20_Info * ds18b20_info, uint8_t* ui8_maxLoad, uint8_t* i8_perOverload, uint8_t* ui8_resolution);
DS18B20_ERROR	fwriteScratchpad			(DS18B20_Info * ds18b20_info, uint8_t ui8_maxLoad, uint8_t ui8_perOverload);

void		callback_timerTemp			(void* arg);
void		IRAM_ATTR gpio_isr_handler	(void* arg);

#endif /* MAIN_TEMP_INT_H_ */
