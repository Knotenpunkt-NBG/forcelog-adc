/*
 * globals.h
 *
 *  Created on: Mar 9, 2021
 *      Author: lor
 */

#ifndef MAIN_GLOBALS_H_
#define MAIN_GLOBALS_H_


#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "esp_mac.h"
#include "limits.h"

#include "esp_timer.h"
#include "esp_wifi.h"
#include "ds18b20.h"

#include "defines.h"


extern struct stu_initConfig* gstu_config;

extern struct stu_adcConfig*		pstu_adcConfig;
extern struct stu_sensorConfig*	pstu_sensorConfig;
extern struct stu_blinkConfig*		pstu_blinkConfig;
extern struct stu_triggerConfig*	pstu_triggerConfig;
extern struct stu_apConfig*		pstu_hotspotConfig;
extern struct stu_batmonConfig* 	pstu_batMonConfig;
extern struct stu_tempConfig*		pstu_tempConfig;
extern struct stu_staConfig*		pstu_staConfigMom;
extern struct stu_staConfig*		pstu_staConfig;

EXTERN unsigned long long ul_zeroTime;

// Config structs

//ADC MODULE STRUCTS
struct stu_sensorConfig{
	char		ac_name[17];
	double 		d_calValue;
	int	i_tareValue;
	int	i_tareZero;
	int8_t i8_perOverload;
	uint8_t ui8_maxLoad;
};

typedef struct
{
	char		str_type[5];
	char		str_data[NUM_DATA_CHARS];
	unsigned long long	ul_time;
} stu_mesCell;


struct stu_adcConfig{
	unsigned long long	ul_adcPeriod;
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
	unsigned int	ui_blinkPeriod;
	unsigned int	ui_blinkDuration;
	unsigned int	ui_blinkBrightness;
	unsigned int	ui_blinkFrequency;
};

struct stu_triggerConfig
{
	bool b_pinMode;
	unsigned int ui_timeout;
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
	unsigned int	ipTrig;
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
	unsigned int	ipTrig;
	unsigned int	ui_wifiTimeout;
};
struct stu_tempConfig
{
	uint8_t	romInt[8];
	uint8_t	romExt[8];
	unsigned int			ui_perInt;
	unsigned int			ui_perExt;
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
extern EventGroupHandle_t eg_wifi;
extern EventGroupHandle_t eg_sync;
extern EventGroupHandle_t eg_config;
extern EventGroupHandle_t eg_status;


//Queues
extern QueueHandle_t q_send;
extern QueueHandle_t q_recv;
extern QueueHandle_t q_rgb_status;
extern QueueHandle_t q_tcpMessages;
extern QueueHandle_t q_uartMessages;
extern QueueHandle_t q_pconfigIn;
extern QueueHandle_t q_pconfigOut;

extern QueueHandle_t q_measurements;
extern QueueHandle_t q_measurements_redund;


//Semaphores
extern SemaphoreHandle_t hs_oneWire;
extern SemaphoreHandle_t hs_pointerQueue;
extern SemaphoreHandle_t hs_configCom;


//Timers
extern esp_timer_handle_t 	htim_periodicAdc;
extern TimerHandle_t 		h_timerBlink;
extern TimerHandle_t 		h_timerBatMon;
extern TimerHandle_t		htim_tcpSend;
extern TimerHandle_t		htim_oneWire;
extern TimerHandle_t		htim_staTimeOut;


//Tasks Handles
extern TaskHandle_t ht_blinkRun;
extern TaskHandle_t ht_tcpMes;
extern TaskHandle_t ht_tcpConf;
extern TaskHandle_t ht_udpWait;
extern TaskHandle_t ht_adcRun;
extern TaskHandle_t ht_storageRun;
extern TaskHandle_t ht_wifiRun;
extern TaskHandle_t ht_batmonRun;
extern TaskHandle_t ht_serialRun;
extern TaskHandle_t ht_tempIntRun;
extern TaskHandle_t ht_tempExtRun;
extern TaskHandle_t ht_configRun;
extern TaskHandle_t ht_wifiBroadcast;

//General Handles
extern esp_netif_t* h_netif;


//Functions
EXTERN void	sendRgbLedStatus(int i);






#endif /* MAIN_GLOBALS_H_ */
