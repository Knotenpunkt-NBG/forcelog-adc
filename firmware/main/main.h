/*
 * main.h
 *
 *  Created on: Apr 17, 2020
 *      Author: lor
 */

#ifndef MAIN_MAIN_H_
#define MAIN_MAIN_H_

#define EXTERN

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include "esp_event.h"
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
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "adc_read.h"
#include "tcp_client.h"
#include "status_led.h"
#include "defines.h"
#include "blink.h"
#include "config.h"
#include "wifi.h"
#include "serial.h"
#include "globals.h"

//void connectWifi(void *arg);



#endif /* MAIN_MAIN_H_ */
