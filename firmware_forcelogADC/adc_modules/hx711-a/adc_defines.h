/*
 * adc_defines.h
 *
 *  Created on: Mar 7, 2021
 *      Author: lor
 */

#ifndef COMPONENTS_ADC_MODULES_HX711_A_ADC_DEFINES_H_
#define COMPONENTS_ADC_MODULES_HX711_A_ADC_DEFINES_H_

#include "freertos/FreeRTOS.h"

#define GPIO_HX711_DT_PIN 19
#define GPIO_HX711_SCK_PIN 18
#define GPIO_HX711_RATE_PIN 21

#define GPIO_INPUT_ADC ((1ULL<<GPIO_HX711_DT_PIN))
#define GPIO_OUTPUT_ADC ((1ULL<<GPIO_HX711_SCK_PIN) | (1ULL<<GPIO_HX711_RATE_PIN))

#define NUM_DATA_CHARS 20

#define MODULE_ID 0x01
#define MODULE_NAME	"hx711-a"

#define ADC_MIN_PERIOD 	12500
#define ADC_CAL_PERIOD	100000		//the measurement period used for calibration

#endif /* COMPONENTS_ADC_MODULES_HX711_A_ADC_DEFINES_H_ */
