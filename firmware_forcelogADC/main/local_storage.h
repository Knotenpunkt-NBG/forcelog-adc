/*
 * local_storage.h
 *
 *  Created on: Oct 25, 2020
 *      Author: lor
 */

#ifndef MAIN_LOCAL_STORAGE_H_
#define MAIN_LOCAL_STORAGE_H_

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
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

void flocalStorageInit(void);
void tlocalStorageRun (void*param);

struct stu_mesCell
{
	double 		d_measurement;
	uint64_t	ul_time;
	char		b_type;
};

#endif /* MAIN_LOCAL_STORAGE_H_ */
