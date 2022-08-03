/*
 * globals.h
 *
 *  Created on: Mar 9, 2021
 *      Author: lor
 */

#ifndef MAIN_GLOBALS_H_
#define MAIN_GLOBALS_H_


#include "limits.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "owb.h"
#include "ds18b20.h"
#include "lwip/sockets.h"
#include "defines.h"
#include "adc_globals.h"


//struct stu_initConfig* gstu_config;

struct stu_adcConfig*		pstu_adcConfig;
struct stu_sensorConfig*	pstu_sensorConfig;
struct stu_blinkConfig*		pstu_blinkConfig;
struct stu_triggerConfig*	pstu_triggerConfig;
struct stu_apConfig*		pstu_hotspotConfig;
struct stu_batmonConfig* 	pstu_batMonConfig;
struct stu_tempConfig*		pstu_tempConfig;
struct stu_staConfig*		pstu_staConfigMom;
struct stu_staConfig*		pstu_staConfig;

EXTERN uint64_t ul_zeroTime;

// Config structs
struct stu_adcConfig{
	uint64_t	ul_adcPeriod;
	uint8_t		uc_txMode;
	uint8_t		uc_numDecimals;
	uint8_t		uc_unit;
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
};

struct stu_triggerConfig
{
	bool b_pinMode;
	uint32_t ui_timeout;
};
struct stu_apConfig
{
	char		ac_hostName[64];
	char		ac_ssid[64];
	char		ac_pass[64];
	esp_netif_ip_info_t ipInfo;
	in_port_t	portConf;
	in_port_t	portMes;
	in_port_t	portBroad;
	in_port_t	portTrig;
	uint32_t	ipTrig;
};
struct stu_staConfig
{
	char		ac_hostName[64];
	char		ac_ssid[64];
	char		ac_pass[64];
	in_port_t	portConf;
	in_port_t	portMes;
	in_port_t	portBroad;
	in_port_t	portTrig;
	uint32_t	ipTrig;
	uint32_t	ui_wifiTimeout;
};
struct stu_tempConfig
{
	OneWireBus_ROMCode	romInt;
	OneWireBus_ROMCode	romExt;
	uint32_t			ui_perInt;
	uint32_t			ui_perExt;
};

struct stu_initConfig
{
	struct stu_adcConfig		adc;
	struct stu_sensorConfig		cell;
	struct stu_blinkConfig		blink;
	struct stu_triggerConfig	trigger;
	struct stu_apConfig			apConfig;
	struct stu_batmonConfig 	batMon;
	struct stu_tempConfig		temp;
	struct stu_staConfig		pstu_staConfigMom;
	struct stu_staConfig		pstu_staConfig;
};

typedef struct	{
	unsigned char	uc_numResponses;
	char*			pc_response;
} stu_configMessage;


//Event groups
EventGroupHandle_t eg_wifi;
EventGroupHandle_t eg_sync;
EventGroupHandle_t eg_config;
EventGroupHandle_t eg_status;


//Queues
QueueHandle_t q_send;
QueueHandle_t q_recv;
QueueHandle_t q_rgb_status;
QueueHandle_t q_tcpMessages;
QueueHandle_t q_uartMessages;
QueueHandle_t q_pconfigIn;
QueueHandle_t q_pconfigOut;

QueueHandle_t q_measurements;
QueueHandle_t q_measurements_redund;


//Semaphores
SemaphoreHandle_t hs_oneWire;
SemaphoreHandle_t hs_pointerQueue;
SemaphoreHandle_t hs_configCom;


//Timers
esp_timer_handle_t 	htim_periodicAdc;
TimerHandle_t 		h_timerBlink;
TimerHandle_t 		h_timerBatMon;
TimerHandle_t		htim_tcpSend;
TimerHandle_t		htim_oneWire;
TimerHandle_t		htim_staTimeOut;


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
TaskHandle_t ht_tempExtRun;
TaskHandle_t ht_configRun;
TaskHandle_t ht_wifiBroadcast;

//General Handles
esp_netif_t* h_netif;

//Functions
EXTERN void	sendRgbLedStatus(int i);






#endif /* MAIN_GLOBALS_H_ */
