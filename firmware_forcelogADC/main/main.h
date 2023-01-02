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
#include "spi_flash_mmap.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_wifi.h"
#include <lwip/sockets.h>
#include "sdkconfig.h"
#include "driver/spi_master.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "freertos/semphr.h"

#include "adc_task.h"
#include "com_tcp.h"
#include "status_led.h"
#include "defines.h"
#include "blink.h"
#include "config.h"
#include "wifi.h"
#include "trigger.h"
#include "com_serial.h"
#include "globals.h"
#include "batmon.h"
#include "temp_int.h"
#include "storage.h"


//#include "testlib.h"


uint8_t fcheckModuleId ();

#endif /* MAIN_MAIN_H_ */
