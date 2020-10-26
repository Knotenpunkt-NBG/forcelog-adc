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
#include "owb.h"
#include "ds18b20.h"
#include "defines.h"

EXTERN uint8_t gb_moduleID;

//Temperature internal
OneWireBus * owb;
OneWireBus_ROMCode device_rom_codes[MAX_DEVICES];
DS18B20_Info * stu_temp_probe_int;
OneWireBus_ROMCode stu_romCode_probe_int;
int num_devices;

//Battery monitor
EXTERN int gi_batRawLow;
EXTERN float gf_batVolLow;
EXTERN int gi_batRawHigh;
EXTERN float gf_batVolHigh;

//structs wifi
EXTERN wifi_config_t sta_config;

//FLAGS
EXTERN bool b_staOrAp;
EXTERN bool b_recording;

//ADC
uint32_t	gui_tareValue;
double		gd_calValue;
uint64_t	gul_adcPeriod;

//BLINK
uint32_t	gui_blinkPeriod;
uint32_t	gui_blinkDuration;
uint32_t	gui_blinkBrightness;
uint32_t	gui_blinkFrequency;
uint8_t		gb_blinkEnabled;

//structs tcp
EXTERN struct sockaddr_in stu_serverAddressConf;
EXTERN struct sockaddr_in stu_serverAddressMes;
EXTERN struct sockaddr_in stu_serverAddressUdp;

//structs blink
ledc_channel_config_t stu_ledcBlinkChannel;

//timing
EXTERN uint64_t ul_zeroTime;

//Event groups
EventGroupHandle_t s_wifi_event_group;
EventGroupHandle_t eg_adc;
EventGroupHandle_t eg_tcp;
EventGroupHandle_t eg_blink;
EventGroupHandle_t eg_batmon;
EventGroupHandle_t eg_tempInt;

//Queues
QueueHandle_t q_measurement;
QueueHandle_t q_time_mes;
QueueHandle_t q_time_blink;
QueueHandle_t q_rgb_status;
QueueHandle_t q_tcpConf;
QueueHandle_t q_temperature;
QueueHandle_t q_time_temp;

//Semaphores
SemaphoreHandle_t xs_sockets;
SemaphoreHandle_t hs_blinkConfig;
SemaphoreHandle_t hs_wifiConfig;
SemaphoreHandle_t hs_oneWire;

//Non Volatile Storage
nvs_handle_t hnvs_conf_0;
nvs_handle_t h_nvs_Blink;
nvs_handle_t h_nvs_ADC;
nvs_handle_t h_nvs_batmon;
nvs_handle_t h_nvs_tempi;

//Timers
esp_timer_handle_t htim_periodicAdc;
ledc_timer_config_t stu_ledcBlinkTimerConfig;
TimerHandle_t h_timerBlink;
TimerHandle_t h_timerBatMon;

//Tasks Handles
TaskHandle_t ht_blinkRun;
TaskHandle_t ht_tcpSend;
TaskHandle_t ht_tcpConf;
TaskHandle_t ht_udpWait;
TaskHandle_t ht_adcRun;
TaskHandle_t ht_tlocalStorageRun;

//Functions
EXTERN void	sendRgbLedStatus(int i);
EXTERN bool fcmdCheck(int i_cmdlet);
EXTERN void fsetLogLevel(int i);

//Tasks
EXTERN void t_blinkRun(void *arg);
EXTERN void t_udpWait (void* param);


#endif /* MAIN_GLOBVARS_H_ */
