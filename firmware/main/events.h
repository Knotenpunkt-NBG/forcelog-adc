/*
 * events.h
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#ifndef MAIN_EVENTS_H_
#define MAIN_EVENTS_H_

#include "freertos/event_groups.h"

EventGroupHandle_t s_wifi_event_group;
EventGroupHandle_t eg_adc;
EventGroupHandle_t eg_tcp;
EventGroupHandle_t eg_uart;
EventGroupHandle_t eg_blink;

#endif /* MAIN_EVENTS_H_ */
