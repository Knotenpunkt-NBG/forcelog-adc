/*
 * globals.h
 *
 *  Created on: Oct 4, 2020
 *      Author: lor
 */

#ifndef MAIN_GLOBVARS_H_
#define MAIN_GLOBVARS_H_

#ifndef EXTERN
#define EXTERN extern
#endif

#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <nvs_flash.h>
#include "driver/ledc.h"
#include "esp_log.h"

//structs wifi
EXTERN wifi_config_t sta_config;

//structs tcp
EXTERN struct sockaddr_in str_serverAddressConf;
EXTERN struct sockaddr_in str_serverAddressMes;
EXTERN struct sockaddr_in str_serverAddressUdp;

//timing
EXTERN uint64_t ul_zeroTime;

//Event groups
EventGroupHandle_t s_wifi_event_group;
EventGroupHandle_t eg_adc;
EventGroupHandle_t eg_tcp;
EventGroupHandle_t eg_tcpRun;
EventGroupHandle_t eg_uart;
EventGroupHandle_t eg_blink;


//Queues
QueueHandle_t q_measurement;
QueueHandle_t q_time_mes;
QueueHandle_t q_time_blink;
QueueHandle_t q_conf;
QueueHandle_t q_rgb_status;
QueueHandle_t q_tcpConf;
QueueHandle_t q_cmdlet;

//Semaphores
SemaphoreHandle_t xs_sockets;

//Non Volatie Storage
nvs_handle_t handle_nvs;
nvs_handle_t h_nvs_Blink;
nvs_handle_t h_nvs_ADC;

//Timers
esp_timer_handle_t h_timerBlink;
esp_timer_handle_t periodic_timer;
ledc_timer_config_t ledc_timer;

//Tasks Handles
TaskHandle_t ht_blinkRun;
TaskHandle_t xHandle;
TaskHandle_t h_t_tcpIdle;

//Tags
static const char* TAG_ADC = "ADC";
static const char* TAG_TCP = "TCP";
static const char* TAG_UDP = "UDP";

//Functions
EXTERN void	sendRgbLedStatus(int i);
EXTERN void connectWifi(void *arg);

#endif /* MAIN_GLOBVARS_H_ */
