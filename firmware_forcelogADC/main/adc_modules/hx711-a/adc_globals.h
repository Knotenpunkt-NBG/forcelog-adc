/*
 * adc_globals.h
 *
 *  Created on: Mar 10, 2021
 *      Author: lor
 */

#ifndef MAIN_ADC_MODULES_HX711_A_ADC_GLOBALS_H_
#define MAIN_ADC_MODULES_HX711_A_ADC_GLOBALS_H_

#include "freertos/FreeRTOS.h"

struct stu_sensorConfig{
	char		ac_name[17];
	double 		d_calValue;
	int32_t	i_tareValue;
	int32_t	i_tareZero;
	int8_t i8_perOverload;
	uint8_t ui8_maxLoad;
};

typedef struct
{
	char		str_type[5];
	char		str_data[NUM_DATA_CHARS];
	uint64_t	ul_time;
} stu_mesCell;


#endif /* MAIN_ADC_MODULES_HX711_A_ADC_GLOBALS_H_ */
