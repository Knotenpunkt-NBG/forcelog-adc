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

#include "limits.h"
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
#include <lwip/sockets.h>
#include "defines.h"

EXTERN uint8_t gb_moduleID;

// Config structs
struct stu_adcConfig{
	bool b_sendTCP;
	bool b_sendSD;
	uint64_t ul_adcPeriod;
	uint8_t uc_numDecimals;
};
struct stu_cellConfig{
	char		ac_name[21];
	double 		d_calValue;
	uint32_t	ui_tareValue;
	uint64_t	ul_cellID;
};
struct stu_batmonConfig
{
	int		i_batRawLow;
	float	f_batVolLow;
	int		i_batRawHigh;
	float	f_batVolHigh;
};
struct stu_blinkConfig
{
	uint32_t	ui_blinkPeriod;
	uint32_t	ui_blinkDuration;
	uint32_t	ui_blinkBrightness;
	uint32_t	ui_blinkFrequency;
	uint8_t		b_blinkEnabled;
};
struct stu_mesCell
{
	char		b_type;
	double 		d_measurement;
	uint64_t	ul_time;
};
struct stu_triggerConfig
{
	bool b_pinMode;
	uint32_t ui_timeout;
	struct sockaddr_in trigSocket;
};
struct stu_wifiConfig
{
	char c_wifiType;
	char ac_apPass[65];
	wifi_config_t wifiConfig;
};
struct stu_initConfig
{
	struct stu_adcConfig adc;
	struct stu_cellConfig cell;
	struct stu_blinkConfig blink;
	struct stu_triggerConfig trigger;
	struct stu_wifiConfig wifi;
	struct sockaddr_in tcpConf;
	struct sockaddr_in tcpMes;
	struct stu_batmonConfig batMon;
};


//Temperature internal
OneWireBus * owb;
OneWireBus_ROMCode device_rom_codes[MAX_DEVICES];
DS18B20_Info * stu_temp_probe_int;
OneWireBus_ROMCode stu_romCode_probe_int;
int num_devices;


//timing
EXTERN uint64_t ul_zeroTime;


//Event groups
EventGroupHandle_t s_wifi_event_group;
EventGroupHandle_t eg_sync;

//Queues
QueueHandle_t q_pointer;
QueueHandle_t q_rgb_status;
QueueHandle_t q_tcpMessages;
QueueHandle_t q_uartMessages;
QueueHandle_t q_pconfigIn;
QueueHandle_t q_pconfigOut;

QueueHandle_t q_value_mes_tcp;
QueueHandle_t q_time_mes_tcp;
QueueHandle_t q_time_blink_tcp;
QueueHandle_t q_value_tempint_tcp;
QueueHandle_t q_time_tempint_tcp;

QueueHandle_t q_value_mes_sd;
QueueHandle_t q_time_mes_sd;
QueueHandle_t q_time_blink_sd;
QueueHandle_t q_value_tempint_sd;
QueueHandle_t q_time_tempint_sd;


//Semaphores
SemaphoreHandle_t hs_oneWire;
SemaphoreHandle_t hs_pointerQueue;
SemaphoreHandle_t hs_configCom;


//Timers
esp_timer_handle_t 	htim_periodicAdc;
TimerHandle_t 		h_timerBlink;
TimerHandle_t 		h_timerBatMon;


//Tasks Handles
TaskHandle_t ht_blinkRun;
TaskHandle_t ht_tcpMes;
TaskHandle_t ht_tcpConf;
TaskHandle_t ht_udpWait;
TaskHandle_t ht_adcRun;
TaskHandle_t ht_storageRun;
TaskHandle_t ht_wifiRun;
TaskHandle_t ht_batmonRun;
TaskHandle_t ht_serialRun;
TaskHandle_t ht_tempIntRun;
TaskHandle_t ht_configRun;

//Functions
EXTERN void	sendRgbLedStatus(int i);
EXTERN void fsetLogLevel(int i);


//Tasks
EXTERN void t_blinkRun(void *arg);



#endif /* MAIN_GLOBVARS_H_ */
