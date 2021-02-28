/*
 * defines.h
 *
 *  Created on: Apr 18, 2020
 *      Author: lor
 */

#ifndef MAIN_DEFINES_H_
#define MAIN_DEFINES_H_


#define FIRMWARE_VERSION		"1.0"
//TODO: implement versioning in config to automaticly rewrite the init config when firmware update has been flashed
#define REWRITE_INIT			0 //writes the init config every time the adc boots. mainly for development purpose


//COMPILER MACROS
#define STR_(X) #X
#define STR(X) STR_(X)


//Eventgroup BITs
#define BIT_CONFIG_SYNC		BIT1
#define BIT_COMM_SYNC		BIT2
#define BIT_STORAGE_SYNC	BIT3

#define BIT_WIFI_SYNC		BIT5
#define BIT_ADC_SYNC		BIT6
#define BIT_BLINK_SYNC		BIT7
#define BIT_TCPCONF_SYNC	BIT8
#define BIT_TCPMES_SYNC		BIT9
#define BIT_TEMPINT_SYNC	BIT10
#define BIT_TRIGGER_SYNC	BIT11
#define BIT_BATMON_SYNC		BIT12


//STORAGE
#define FILENAME_MAX_SIZE	20


//COMMUNICATION
#define COM_CYCLES_TIMEOUT			50
#define PERIOD_TCPMES_PACKET_MS		100
#define COM_TCPMES_KEEPALIVE_PERIOD


//Temperature internal
#define OWB_GPIO				4
#define GPIO_OWB_SENSE			34
#define MAX_DEVICES          	(8)
#define TEMP_INTERNAL_PERIOD	60   // period of temperature measurements in seconds. must be higher then 2s
#define MAX_TEMP				50.0	//Max temperature (°C)for the adc to operate in. if this is crossed the adc shuts down all functions


//Battery monitor
#define BATMON_PERIOD		1*1000
#define BAT_CUTOFF			3.3
#define BIT_BATMON_FIRE		BIT1
#define BATMON_NUM_SAMPLES	50
#define BATMON_CHANNEL		ADC1_CHANNEL_7


//ID SHIFT REGISTER
#define PIN_MODULE_ID_SELECT	0
#define PIN_MODULE_ID_CLK		2
#define PIN_MODULE_ID_DOUT		17


//Trigger
#define ACTIVE_HIGH		1
#define ACTIVE_LOW		0
#define DEFAULT_TRIGGER_PORT	16001


//ADC MODULE USED
#define ADC_MODULE_ID		MODULE_ID_HX711
#define MODULE_ID_HX711		0x01
#define MOUDLE_NAME			"HX711"


// Blink LED
#define LEDC_GPIO_BLINK			(27)
#define LEDC_GPIO_INV_BLINK		0
#define LEDC_CH_BLINK			LEDC_CHANNEL_3


//Config of RGB Status LED
#define LEDC_HS_TIMER			LEDC_TIMER_0
#define LEDC_HS_MODE			LEDC_HIGH_SPEED_MODE

#define LEDC_GPIO_RED			(15)
#define LEDC_GPIO_INV_RED		0
#define LEDC_CH_RED				LEDC_CHANNEL_0

#define LEDC_GPIO_GREEN			(16)
#define LEDC_GPIO_INV_GREEN		0
#define LEDC_CH_GREEN			LEDC_CHANNEL_1

#define LEDC_GPIO_BLUE			(17)
#define LEDC_GPIO_INV_BLUE		0
#define LEDC_CH_BLUE			LEDC_CHANNEL_2

#define LEDC_FREQ_STATUS		200

#define LEDC_STATUS_IDLE			0x0000
#define LEDC_STATUS_WAITING_SERIAL	0x0001
#define LEDC_STATUS_WAITING_SERVER	0x0002
#define LEDC_STATUS_WAITING_WEIGHT	0x0003

//Wifi Config
#define WIFI_CONNECTED_BIT		BIT0
#define WIFI_FAIL_BIT			BIT1
#define WIFI_TIMEOUT_BIT		BIT2
#define WIFI_IPASSIGNED_BIT		BIT3
#define WIFI_STA_START_BIT		BIT4

#define MIN_SIGNAL_DBM			-90
#define MAX_NUM_AP_SCAN			10


//ADC
#define ADC_MIN_PERIOD			12500		//the minimal measurement period for the module ; has to be moved to the module itself
#define ADC_CAL_PERIOD			100000		//the measurement period used for calibration ; has to be moved to module

#define UNIT_RAW			0
#define UNIT_N				1
#define UNIT_KG				2
#define UNIT_G				3
#define UNIT_T				4
#define UNIT_LB				10
#define UNIT_OZ				11
#define UNIT_LBF			30
#define UNIT_NB				50

#define TYPE_ADC			1
#define TYPE_BLINK			2
#define TYPE_TEMP_INT		3
#define TYPE_TEMP_EXT		4

#define TXMODE_TCP_PLUS_SDBACK	1
#define TXMODE_TCP				2
#define TXMODE_SD				3


//TCP
#define TICKS_RECON_MES			10			//defines how many ticks to wait until measurement socket tries a keepalive
#define	CYCLES_RECON_CONF		100			//how many cycles of timeout to wait until a keepalive is sent
#define TICKS_TIMEOUT_CONF		5			//how many ticks to wait until config socket times out



//Cmdlets
	// CMDlets Control
#define CMD_peek	('p' << 8 * 3) + ('e' << 8 * 2) + ('e' << 8 * 1) + ('k' << 8 * 0)
#define CMD_stop	('s' << 8 * 3) + ('t' << 8 * 2) + ('o' << 8 * 1) + ('p' << 8 * 0)
#define CMD_wait	('w' << 8 * 3) + ('a' << 8 * 2) + ('i' << 8 * 1) + ('t' << 8 * 0)
#define CMD_rec		('r' << 8 * 3) + ('e' << 8 * 2) + ('c' << 8 * 1) + (0x00 << 8 * 0)
#define CMD_strt	('s' << 8 * 3) + ('t' << 8 * 2) + ('r' << 8 * 1) + ('t' << 8 * 0)
#define CMD_fire	('f' << 8 * 3) + ('i' << 8 * 2) + ('r' << 8 * 1) + ('e' << 8 * 0)
#define CMD_trig	('t' << 8 * 3) + ('r' << 8 * 2) + ('i' << 8 * 1) + ('g' << 8 * 0)
#define CMD_send	('s' << 8 * 3) + ('e' << 8 * 2) + ('n' << 8 * 1) + ('d' << 8 * 0)


	//CMDlets ADC
#define CMD_cali	('c' << 8 * 3) + ('a' << 8 * 2) + ('l' << 8 * 1) + ('i' << 8 * 0)
#define CMD_vcal	('v' << 8 * 3) + ('c' << 8 * 2) + ('a' << 8 * 1) + ('l' << 8 * 0)
#define CMD_mper	('m' << 8 * 3) + ('p' << 8 * 2) + ('e' << 8 * 1) + ('r' << 8 * 0)
#define CMD_tare	('t' << 8 * 3) + ('a' << 8 * 2) + ('r' << 8 * 1) + ('e' << 8 * 0)
#define CMD_fire	('f' << 8 * 3) + ('i' << 8 * 2) + ('r' << 8 * 1) + ('e' << 8 * 0)
#define CMD_sesd	('s' << 8 * 3) + ('e' << 8 * 2) + ('s' << 8 * 1) + ('d' << 8 * 0)
#define CMD_setc	('s' << 8 * 3) + ('e' << 8 * 2) + ('t' << 8 * 1) + ('c' << 8 * 0)
#define CMD_retc	('r' << 8 * 3) + ('e' << 8 * 2) + ('t' << 8 * 1) + ('c' << 8 * 0)
#define CMD_resd	('r' << 8 * 3) + ('e' << 8 * 2) + ('s' << 8 * 1) + ('d' << 8 * 0)


	//CMDlets general config
#define CMD_loge	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('e' << 8 * 0)
#define CMD_logw	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('w' << 8 * 0)
#define CMD_logi	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('i' << 8 * 0)
#define CMD_logd	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('d' << 8 * 0)
#define CMD_logv	('l' << 8 * 3) + ('o' << 8 * 2) + ('g' << 8 * 1) + ('v' << 8 * 0)
#define CMD_zero	('z' << 8 * 3) + ('e' << 8 * 2) + ('r' << 8 * 1) + ('o' << 8 * 0)
#define CMD_man		('m' << 8 * 3) + ('a' << 8 * 2) + ('n' << 8 * 1) + (0x00 << 8 * 0)

#define CMD_info	('i' << 8 * 3) + ('n' << 8 * 2) + ('f' << 8 * 1) + ('o' << 8 * 0)
#define CMD_iqad	('i' << 8 * 3) + ('q' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0)
#define CMD_iqtc	('i' << 8 * 3) + ('n' << 8 * 2) + ('t' << 8 * 1) + ('c' << 8 * 0)
#define CMD_iqtm	('i' << 8 * 3) + ('n' << 8 * 2) + ('t' << 8 * 1) + ('m' << 8 * 0)
#define CMD_iqtr	('i' << 8 * 3) + ('n' << 8 * 2) + ('t' << 8 * 1) + ('r' << 8 * 0)
#define CMD_iqbl	('i' << 8 * 3) + ('n' << 8 * 2) + ('b' << 8 * 1) + ('l' << 8 * 0)
#define CMD_iqlc	('i' << 8 * 3) + ('n' << 8 * 2) + ('l' << 8 * 1) + ('c' << 8 * 0)


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
#define CMD_come	('c' << 8 * 3) + ('o' << 8 * 2) + ('m' << 8 * 1) + ('e' << 8 * 0)
#define CMD_iptr	('i' << 8 * 3) + ('p' << 8 * 2) + ('t' << 8 * 1) + ('r' << 8 * 0)


#define CMD_hspc	('h' << 8 * 3) + ('s' << 8 * 2) + ('p' << 8 * 1) + ('c' << 8 * 0)
#define CMD_hspm	('h' << 8 * 3) + ('s' << 8 * 2) + ('p' << 8 * 1) + ('m' << 8 * 0)
#define CMD_hspt	('h' << 8 * 3) + ('s' << 8 * 2) + ('t' << 8 * 1) + ('r' << 8 * 0)
#define CMD_hspb	('h' << 8 * 3) + ('s' << 8 * 2) + ('b' << 8 * 1) + ('r' << 8 * 0)


#define CMD_stpc	('s' << 8 * 3) + ('t' << 8 * 2) + ('p' << 8 * 1) + ('c' << 8 * 0)
#define CMD_stpm	('s' << 8 * 3) + ('t' << 8 * 2) + ('p' << 8 * 1) + ('m' << 8 * 0)
#define CMD_stpt	('s' << 8 * 3) + ('t' << 8 * 2) + ('t' << 8 * 1) + ('r' << 8 * 0)
#define CMD_stpb	('s' << 8 * 3) + ('t' << 8 * 2) + ('b' << 8 * 1) + ('r' << 8 * 0)
#define CMD_stto	('s' << 8 * 3) + ('t' << 8 * 2) + ('t' << 8 * 1) + ('o' << 8 * 0)



	//CMDlets wifi
#define CMD_ssid	('s' << 8 * 3) + ('s' << 8 * 2) + ('i' << 8 * 1) + ('d' << 8 * 0)
#define CMD_pass	('p' << 8 * 3) + ('a' << 8 * 2) + ('s' << 8 * 1) + ('s' << 8 * 0)
#define CMD_wity	('w' << 8 * 3) + ('i' << 8 * 2) + ('t' << 8 * 1) + ('y' << 8 * 0)
#define CMD_appa	('a' << 8 * 3) + ('p' << 8 * 2) + ('p' << 8 * 1) + ('a' << 8 * 0)
#define CMD_apss	('a' << 8 * 3) + ('p' << 8 * 2) + ('s' << 8 * 1) + ('s' << 8 * 0)
#define CMD_cowi	('c' << 8 * 3) + ('o' << 8 * 2) + ('w' << 8 * 1) + ('i' << 8 * 0)
#define CMD_cost	('c' << 8 * 3) + ('o' << 8 * 2) + ('s' << 8 * 1) + ('t' << 8 * 0)
#define CMD_coap	('c' << 8 * 3) + ('o' << 8 * 2) + ('a' << 8 * 1) + ('p' << 8 * 0)
#define CMD_scwi	('s' << 8 * 3) + ('c' << 8 * 2) + ('w' << 8 * 1) + ('i' << 8 * 0)


	//CMDlets storage
#define CMD_mkfs	('m' << 8 * 3) + ('k' << 8 * 2) + ('f' << 8 * 1) + ('s' << 8 * 0)
#define CMD_wrin	('w' << 8 * 3) + ('r' << 8 * 2) + ('i' << 8 * 1) + ('n' << 8 * 0)

#define CMD_list	('l' << 8 * 3) + ('i' << 8 * 2) + ('s' << 8 * 1) + ('t' << 8 * 0)
#define CMD_lswi	('l' << 8 * 3) + ('s' << 8 * 2) + ('w' << 8 * 1) + ('i' << 8 * 0)
#define CMD_lsad	('l' << 8 * 3) + ('s' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0)
#define CMD_lsbl	('l' << 8 * 3) + ('s' << 8 * 2) + ('b' << 8 * 1) + ('l' << 8 * 0)
#define CMD_lslc	('l' << 8 * 3) + ('s' << 8 * 2) + ('l' << 8 * 1) + ('c' << 8 * 0)

#define CMD_init	('i' << 8 * 3) + ('n' << 8 * 2) + ('i' << 8 * 1) + ('t' << 8 * 0)
#define CMD_ldad	('l' << 8 * 3) + ('d' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0)
#define CMD_ldtc	('l' << 8 * 3) + ('d' << 8 * 2) + ('t' << 8 * 1) + ('c' << 8 * 0)
#define CMD_ldtm	('l' << 8 * 3) + ('d' << 8 * 2) + ('t' << 8 * 1) + ('m' << 8 * 0)
#define CMD_ldbl	('l' << 8 * 3) + ('d' << 8 * 2) + ('b' << 8 * 1) + ('l' << 8 * 0)
#define CMD_ldlc	('l' << 8 * 3) + ('d' << 8 * 2) + ('l' << 8 * 1) + ('c' << 8 * 0)
#define CMD_ldwi	('l' << 8 * 3) + ('d' << 8 * 2) + ('w' << 8 * 1) + ('i' << 8 * 0)
#define CMD_ldst	('l' << 8 * 3) + ('d' << 8 * 2) + ('s' << 8 * 1) + ('t' << 8 * 0)
#define CMD_ldss	('l' << 8 * 3) + ('d' << 8 * 2) + ('s' << 8 * 1) + ('s' << 8 * 0)
#define CMD_defl	('d' << 8 * 3) + ('e' << 8 * 2) + ('f' << 8 * 1) + ('l' << 8 * 0)

#define CMD_svad	('s' << 8 * 3) + ('v' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0)
#define CMD_svtc	('s' << 8 * 3) + ('v' << 8 * 2) + ('t' << 8 * 1) + ('c' << 8 * 0)
#define CMD_svtm	('s' << 8 * 3) + ('v' << 8 * 2) + ('t' << 8 * 1) + ('m' << 8 * 0)
#define CMD_svbl	('s' << 8 * 3) + ('v' << 8 * 2) + ('b' << 8 * 1) + ('l' << 8 * 0)
#define CMD_svlc	('s' << 8 * 3) + ('v' << 8 * 2) + ('l' << 8 * 1) + ('c' << 8 * 0)
#define CMD_svwi	('s' << 8 * 3) + ('v' << 8 * 2) + ('w' << 8 * 1) + ('i' << 8 * 0)
#define CMD_svti	('s' << 8 * 3) + ('v' << 8 * 2) + ('t' << 8 * 1) + ('i' << 8 * 0)
#define CMD_svst	('s' << 8 * 3) + ('v' << 8 * 2) + ('s' << 8 * 1) + ('t' << 8 * 0)
#define CMD_defs	('d' << 8 * 3) + ('e' << 8 * 2) + ('f' << 8 * 1) + ('s' << 8 * 0)

#define CMD_inad	('i' << 8 * 3) + ('n' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0)
#define CMD_intc	('i' << 8 * 3) + ('n' << 8 * 2) + ('t' << 8 * 1) + ('c' << 8 * 0)
#define CMD_intm	('i' << 8 * 3) + ('n' << 8 * 2) + ('t' << 8 * 1) + ('m' << 8 * 0)
#define CMD_inbl	('i' << 8 * 3) + ('n' << 8 * 2) + ('b' << 8 * 1) + ('l' << 8 * 0)
#define CMD_intr	('i' << 8 * 3) + ('n' << 8 * 2) + ('t' << 8 * 1) + ('r' << 8 * 0)
#define CMD_inwi	('i' << 8 * 3) + ('n' << 8 * 2) + ('w' << 8 * 1) + ('i' << 8 * 0)

#define CMD_rmst	('r' << 8 * 3) + ('m' << 8 * 2) + ('s' << 8 * 1) + ('t' << 8 * 0)
#define CMD_rmlc	('r' << 8 * 3) + ('m' << 8 * 2) + ('l' << 8 * 1) + ('c' << 8 * 0)
#define CMD_rmbl	('r' << 8 * 3) + ('m' << 8 * 2) + ('b' << 8 * 1) + ('l' << 8 * 0)
#define CMD_rmad	('r' << 8 * 3) + ('m' << 8 * 2) + ('a' << 8 * 1) + ('d' << 8 * 0)

#define CMD_cllc	('c' << 8 * 3) + ('l' << 8 * 2) + ('l' << 8 * 1) + ('c' << 8 * 0)


	//CMDlets One Wire Bus
#define CMD_scow	('s' << 8 * 3) + ('c' << 8 * 2) + ('o' << 8 * 1) + ('w' << 8 * 0)
#define CMD_stpi	('s' << 8 * 3) + ('t' << 8 * 2) + ('p' << 8 * 1) + ('i' << 8 * 0)


	//CMDlets Battery monitor
#define CMD_bath	('b' << 8 * 3) + ('a' << 8 * 2) + ('t' << 8 * 1) + ('h' << 8 * 0)
#define CMD_batl	('b' << 8 * 3) + ('a' << 8 * 2) + ('t' << 8 * 1) + ('l' << 8 * 0)


//STATI OF THE ADC
#define STATUS_IDLE				BIT0
#define STATUS_PEEKING			BIT1
#define STATUS_RECORDING		BIT2
#define STATUS_WAITING			BIT3
#define STATUS_TCPCONF_CONN		BIT4
#define STATUS_TCPMES_CONN		BIT5
#define STATUS_WIFI_CONN		BIT6
#define STATUS_HOTSPOT			BIT7
#define STATUS_STATION			BIT8
#define STATUS_SCANNING			BIT9

#define BITS_STATI			STATUS_IDLE | STATUS_PEEKING | STATUS_RECORDING | STATUS_WAITING
#define BITS_COMM			STATUS_WIFI_CONN | STATUS_TCPMES_CONN | STATUS_TCPCONF_CONN



//ADC defines
#define ADC_TARE_CYCLES		10
#define ADC_CAL_CYCLES		10


//LOG LEVELS
#define LOG_ERROR		1
#define LOG_WARNING		2
#define LOG_INFO		3
#define LOG_DEBUG		4
#define LOG_VERBOSE		5

//Buffer sizes
#define BUFSIZE_CONFIGIN_IDLE	64
#define BUFSIZE_CONFIGOUT_IDLE	64


//MESSAGES
#define MANUAL 	"|user|Manual for Loadcell\n"\
				"\tssid - Sets SSID of WiFi network\n"\
				"\tpass - Sets password of WiFi network\n"\
				"\tsvip - Sets server IP (format xxx.xxx.xxx.xxx)\n"
#define INQUIRE_MESSAGE		"|moid|%02X|\t\tModule ID:\n"\
							"|fwve|%s|\t\tFirmware version:\n"\
							"|user|ADC\n"\
							"|mper|Measurement Period:\n"\
							"|calv|%f|\t\tCalibration Value:\n"\
							"|tare|%lu|\t\tTare Value:\n"\
							"|user|BLINK\n"\
							"|bper|%d|\t\tPeriod:\n"\
							"|bdur|%d|\t\tDuration\n"\
							"|user|WIFI\n"\
							"|ssid|%s|\t\tSSID of access point:\n"\
							"|pass|%s|\t\tPassword of access point:\n"\
							"|user|SOCKETS\n"\
							"|ipme|%s|\t\tIP of measurement server:\n"\
							"|pome|%d|\t\tPort of measurement server:\n"\
							"|ipco|%s|\t\tIP of config server:\n"\
							"|poco|%d|\t\tPort of config server:\n"\
							"|iptr|%s|\t\tIP of trigger:\n"\
							"|potr|%d|\t\tPort of trigger:\n"\

#define MEASUREMENT_MESSAGE	"|moid|"STR(ADC_MODULE_ID)"|\t\tModule ID\n\4"\
							"|fwve|"FIRMWARE_VERSION"|\t\tFirmware version\n\4"\
							"|mper|%llu|\t\tMeasurement Period\n\4"\
							"|calv|%f|\t\tCalibration Value\n\4"\
							"|tare|%u|\t\tTare Value\n\4"\
							"|bper|%d|\t\tBlink period\n\4"\
							"|bdur|%u|\t\tBlink duration\n\4"\

#define GREETING_MESSAGE	"|user|send man for manual\n"
#define MESS_INVALID		"|inv|Message could not be received correctly\n"

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
#define TAG_STORAGE "STORAGE"

//ERRORS
#define ERR_RECORDING		10
#define ERR_MPER_TOO_LOW	20

#endif /* MAIN_DEFINES_H_ */
