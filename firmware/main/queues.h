/*
 * global.h
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#ifndef MAIN_QUEUES_H_
#define MAIN_QUEUES_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t q_measurement;
QueueHandle_t q_time;
QueueHandle_t q_conf;
QueueHandle_t q_rgb_status;
QueueHandle_t q_tcpConf;
QueueHandle_t q_cmdlet;

#endif /* MAIN_QUEUES_H_ */
