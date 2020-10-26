/*
 * defines.h
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#ifndef MAIN_DEFINES_H_
#define MAIN_DEFINES_H_


#define FIRMWARE_VERSION		"1.0"

//Temperature internal
#define OWB_GPIO			4
#define MAX_DEVICES          (8)
#define TEMP_INTERNAL_PERIOD        (1000)   // milliseconds
#define BIT_TEMP_START		BIT1



//Battery monitor
#define BATMON_PERIOD		60*1000
#define BAT_CUTOFF			3.3
#define BIT_BATMON_FIRE		BIT1
#define BATMON_NUM_SAMPLES	50


//ID SHIFT REGISTER
#define PIN_MODULE_ID_SELECT	2
#define PIN_MODULE_ID_CLK		14
#define PIN_MODULE_ID_DOUT		34

//Trigger

//ADC MODULE USED
#define ADC_MODULE_ID		MODULE_ID_HX711
#define MODULE_ID_HX711		1

//GPIO Module
#define GPIO_INPUT_PIN_SEL (1ULL<<PIN_MODULE_ID_DOUT)
#define GPIO_OUTPUT_PIN_SEL ((1ULL<<LEDC_GPIO_BLINK) | (1ULL<<PIN_MODULE_ID_SELECT) | (1ULL<<PIN_MODULE_ID_CLK))

// Blink LED
#define LEDC_GPIO_BLINK			(27)
#define LEDC_GPIO_INV_BLINK		0
#define LEDC_CH_BLINK			LEDC_CHANNEL_3

//Config of RGB Status LED
#define LEDC_HS_TIMER			LEDC_TIMER_0
#define LEDC_HS_MODE			LEDC_HIGH_SPEED_MODE

#define LEDC_GPIO_RED			(26)
#define LEDC_GPIO_INV_RED		1
#define LEDC_CH_RED				LEDC_CHANNEL_0

#define LEDC_GPIO_GREEN			(25)
#define LEDC_GPIO_INV_GREEN		1
#define LEDC_CH_GREEN			LEDC_CHANNEL_1

#define LEDC_GPIO_BLUE			(33)
#define LEDC_GPIO_INV_BLUE		1
#define LEDC_CH_BLUE			LEDC_CHANNEL_2

#define LEDC_TEST_DUTY			(4000)
#define LEDC_TEST_FADE_TIME		(3000)
#define LEDC_FREQ_STATUS		200

#define LEDC_STATUS_IDLE			0x0000
#define LEDC_STATUS_WAITING_SERIAL	0x0001
#define LEDC_STATUS_WAITING_SERVER	0x0002
#define LEDC_STATUS_WAITING_WEIGHT	0x0003

//Wifi Config
#define WIFI_CONNECTED_BIT		BIT0
#define WIFI_FAIL_BIT			BIT1
#define EXAMPLE_ESP_MAXIMUM_RETRY  5
#define FLAG_STATION			0
#define FLAG_AP					1
#define NUM_WIFI_RECON			3

//ADC Run Events
#define BIT_ADC_START			BIT1
#define BIT_ADC_WAIT			BIT2
#define BIT_ADC_STOP			BIT3
#define BIT_ADC_FIRE			BIT5
#define BIT_ADC_BREAK			BIT6


//ADC Config Events
#define ADC_MIN_PERIOD			12500
#define ADC_CAL_PERIOD			100000

//TCP Events init eg_tcpInit
#define BIT_TCPSENDMES_READY	BIT0
#define BIT_TCPSENDCONF_READY	BIT2
#define BIT_TCPQUEUE_READY		BIT3
#define BIT_TCPSEND				BIT4

//TCP
#define TICKS_RECON_MES			10			//defines how many ticks to wait until measurement socket tries a reconnect
#define	CYCLES_RECON_CONF		100			//how many cycles of timeout to wait until a keepalive is sent
#define TICKS_TIMEOUT_CONF		5			//how many ticks to wait until config socket times out

//Blink Events
#define BIT_BLINK_FIRE			BIT0
#define BIT_BLINK_START			BIT2
#define BIT_BLINK_WAIT			BIT3
#define BIT_BLINK_STOP			BIT4


//Cmdlets
// CMDlets Control
#define CMD_peek	('p' << 8 * 3) + ('e' << 8 * 2) + ('e' << 8 * 1) + ('k' << 8 * 0)
#define CMD_stop	('s' << 8 * 3) + ('t' << 8 * 2) + ('o' << 8 * 1) + ('p' << 8 * 0)
#define CMD_wait	('w' << 8 * 3) + ('a' << 8 * 2) + ('i' << 8 * 1) + ('t' << 8 * 0)
#define CMD_rec		('r' << 8 * 3) + ('e' << 8 * 2) + ('c' << 8 * 1) + (0x00 << 8 * 0)

//CMDlets ADC
#define CMD_cali	('c' << 8 * 3) + ('a' << 8 * 2) + ('l' << 8 * 1) + ('i' << 8 * 0)
#define CMD_vcal	('v' << 8 * 3) + ('c' << 8 * 2) + ('a' << 8 * 1) + ('l' << 8 * 0)
#define CMD_mper	('m' << 8 * 3) + ('p' << 8 * 2) + ('e' << 8 * 1) + ('r' << 8 * 0)
#define CMD_tare	('t' << 8 * 3) + ('a' << 8 * 2) + ('r' << 8 * 1) + ('e' << 8 * 0)

//CMDlets general config
#define CMD_load	('l' << 8 * 3) + ('o' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0)
#define CMD_save	('s' << 8 * 3) + ('a' << 8 * 2) + ('v' << 8 * 1) + ('e' << 8 * 0)
#define CMD_loge	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('e' << 8 * 0)
#define CMD_logw	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('w' << 8 * 0)
#define CMD_logi	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('i' << 8 * 0)
#define CMD_logd	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('d' << 8 * 0)
#define CMD_logv	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('v' << 8 * 0)
#define CMD_zero	('z' << 8 * 3) + ('e' << 8 * 2) + ('r' << 8 * 1) + ('o' << 8 * 0)
#define CMD_man		('m' << 8 * 3) + ('a' << 8 * 2) + ('n' << 8 * 1) + (0x00 << 8 * 0)
#define CMD_info	('i' << 8 * 3) + ('n' << 8 * 2) + ('f' << 8 * 1) + ('o' << 8 * 0)

//CMDlets blink
#define CMD_bper	('b' << 8 * 3) + ('p' << 8 * 2) + ('e' << 8 * 1) + ('r' << 8 * 0)
#define CMD_bdur	('b' << 8 * 3) + ('d' << 8 * 2) + ('u' << 8 * 1) + ('r' << 8 * 0)
#define CMD_bbrt	('b' << 8 * 3) + ('b' << 8 * 2) + ('r' << 8 * 1) + ('t' << 8 * 0)
#define CMD_bfrq	('b' << 8 * 3) + ('f' << 8 * 2) + ('r' << 8 * 1) + ('q' << 8 * 0)
#define CMD_benb	('b' << 8 * 3) + ('e' << 8 * 2) + ('n' << 8 * 1) + ('b' << 8 * 0)
#define CMD_bdis	('b' << 8 * 3) + ('d' << 8 * 2) + ('i' << 8 * 1) + ('s' << 8 * 0)
#define CMD_blnk	('b' << 8 * 3) + ('l' << 8 * 2) + ('n' << 8 * 1) + ('k' << 8 * 0)

//CMDlets connection
#define CMD_conn	('c' << 8 * 3) + ('o' << 8 * 2) + ('n' << 8 * 1) + ('n' << 8 * 0)
#define CMD_ssid	('s' << 8 * 3) + ('s' << 8 * 2) + ('i' << 8 * 1) + ('d' << 8 * 0)
#define CMD_pass	('p' << 8 * 3) + ('a' << 8 * 2) + ('s' << 8 * 1) + ('s' << 8 * 0)
#define CMD_ipco	('i' << 8 * 3) + ('p' << 8 * 2) + ('c' << 8 * 1) + ('o' << 8 * 0)
#define CMD_ipme	('i' << 8 * 3) + ('p' << 8 * 2) + ('m' << 8 * 1) + ('e' << 8 * 0)
#define CMD_poco	('p' << 8 * 3) + ('o' << 8 * 2) + ('c' << 8 * 1) + ('o' << 8 * 0)
#define CMD_pome	('p' << 8 * 3) + ('o' << 8 * 2) + ('m' << 8 * 1) + ('e' << 8 * 0)
#define CMD_potr	('p' << 8 * 3) + ('o' << 8 * 2) + ('t' << 8 * 1) + ('r' << 8 * 0)

//CMDlets One Wire Bus
#define CMD_scow	('s' << 8 * 3) + ('c' << 8 * 2) + ('o' << 8 * 1) + ('w' << 8 * 0)
#define CMD_stpi	('s' << 8 * 3) + ('t' << 8 * 2) + ('p' << 8 * 1) + ('i' << 8 * 0)

//CMDlets Battery monitor
#define CMD_bath	('b' << 8 * 3) + ('a' << 8 * 2) + ('t' << 8 * 1) + ('h' << 8 * 0)
#define CMD_batl	('b' << 8 * 3) + ('a' << 8 * 2) + ('t' << 8 * 1) + ('l' << 8 * 0)

//ADC defines
#define ADC_TARE_CYCLES		10
#define ADC_CAL_CYCLES		10

//LOG LEVELS
#define LOG_ERROR		1
#define LOG_WARNING		2
#define LOG_INFO		3
#define LOG_DEBUG		4
#define LOG_VERBOSE		5

//MESSAGES
#define MANUAL 	":mess::Manual for Loadcell\n"\
				"	ssid - Sets SSID of WiFi network\n"\
				"	pass - Sets passphrase of WiFi network\n"\
				"	svip - Sets server IP (format xxx.xxx.xxx.xxx)\n"

#define INQUIRE_MESSAGE		":moid:%02X:		Module ID:\n"\
							":fwve:%s:		Firmware version:\n"\
							":user::ADC:\n"\
							":mper:: Measurement Period:\n"\
							":calv:%f:	Calibration Value:\n"\
							":tare:%lu:	Tare Value:\n"\
							":user:BLINK:\n"\
							":bper:%d:	Period:\n"\
							":bdur:%d:	Duration\n"\
							":user:WIFI:\n"\
							":ssid:%s:	SSID of access point:\n"\
							":pass:%s:	Password of access point:\n"\
							":ipme:%s:	IP of measurement server:\n"\
							":pome:%d:	Port of measurement server:\n"\
							":ipco:%s:	IP of config server:\n"\
							":poco:%d:	Port of config server:\n"\
							":potr:%d:	Port of Trigger:\n"\

#define MEASUREMENT_MESSAGE	":moid:%02X:		Module ID:\n"\
							":fwve:%s:		Firmware version:\n"\
							":mper:%lu: Measurement Period:\n"\
							":calv:%f:	Calibration Value:\n"\
							":tare:%lu:	Tare Value:\n"\
							":bper:%d:	Blink period:\n"\
							":bdur:%d:	Blink duration\n"\


#define GREETING_SERIAL		":mess::send man for manual:\n"
#define GREETING_TCP		":mess::send man for manual:\n"

//TAGS
#define TAG_ADC		"ADC"
#define TAG_TCP		"TCP"
#define TAG_UDP		"UDP"
#define TAG_UART	"UART"
#define TAG_BLINK	"BLINK"
#define TAG_RGB		"RGB"
#define TAG_WIFI	"WiFi"
#define TAG_CONF	"CONFIG"
#define TAG_BATMON	"BATMON"
#define TAG_TEMPINT	"TEMPINT"
#define TAG_REDUND	"REDUND"

//ERRORS
#define ERR_RECORDING		10
#define ERR_MPER_TOO_LOW	20

#endif /* MAIN_DEFINES_H_ */
