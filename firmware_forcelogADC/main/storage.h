/*
 * local_storage.h
 *
 *  Created on: Oct 25, 2020
 *      Author: lor
 */

#ifndef MAIN_STORAGE_H_
#define MAIN_STORAGE_H_

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"

#include "esp_wifi.h"
#include <lwip/sockets.h>

#include "vfs_fat_internal.h"
#include "diskio_impl.h"
#include "diskio_sdmmc.h"
#include "sdmmc_cmd.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"


#include "defines.h"
#include "globals.h"

#define MOUNT_POINT "/sdcard"
#define USE_SPI_MODE
#define SPI_DMA_CHAN    1

#define PIN_NUM_MISO 12
#define PIN_NUM_MOSI 13
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   15
#define allocation_unit_size	(16 * 1024)
#define max_files	2

void fstorageInit(void);
void tstorageRun (void*param);
void fsdConfig(uint32_t ui_cmdlet);
esp_err_t fFormatSD();

sdmmc_card_t* card;

BYTE pdrv;
FATFS* fs;
size_t alloc_unit_size;

struct stu_initConfig
{
	uint8_t adc;
	uint8_t wifi;
	uint8_t blink;
	uint8_t tempi;
	uint8_t tcpMes;
	uint8_t tcpConf;
	uint8_t trigger;
};




#endif /* MAIN_STORAGE_H_ */
