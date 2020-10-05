/*
 * status_led.h
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#ifndef MAIN_STATUS_LED_H_
#define MAIN_STATUS_LED_H_

#include "freertos/FreeRTOS.h"
#include "driver/ledc.h"

#include "defines.h"
#include "globals.h"

void status_led(void *arg);

#endif /* MAIN_STATUS_LED_H_ */
