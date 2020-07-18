/*
 * main.h
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_wifi.h"
#include <lwip/sockets.h>
#include "sdkconfig.h"
#include "driver/spi_master.h"

#include "adc_read.h"
#include "queues.h"
#include "events.h"
#include "tcp_client.h"
#include "status_led.h"
#include "defines.h"
#include "blink.h"

nvs_handle_t handle_nvs;

char* readUartString(char* out, int i_maxNumChars);
int readUartCmdlet();
void sendRgbLedStatus(int i);
static void connectWifi(void *arg);
static void serial_config(void *arg);

#endif /* MAIN_MAIN_H_ */
