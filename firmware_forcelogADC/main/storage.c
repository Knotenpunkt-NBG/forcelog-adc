/*
 * local_storage.c
 *
 *  Created on: Oct 25, 2020
 *      Author: lor
 */

#include "storage.h"

static const char* TAG = "vfs_fat_sdmmc";
static sdmmc_card_t* s_card = NULL;
static uint8_t s_pdrv = 0;
static char * s_base_path = NULL;
char* base_path = MOUNT_POINT;
FATFS* fs;
BYTE pdrv;

void fstorageInit(void)
{
//	gstu_config = calloc(1, sizeof(struct stu_initConfig));

	ESP_LOGI(TAG_REDUND, "Initializing SD card");

	ESP_LOGI(TAG_REDUND, "Using SPI peripheral");
	sdmmc_host_t host = SDSPI_HOST_DEFAULT();
	sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
	sdmmc_card_t* out_card;


	spi_bus_config_t bus_cfg = {
	        .mosi_io_num = PIN_NUM_MOSI,
	        .miso_io_num = PIN_NUM_MISO,
	        .sclk_io_num = PIN_NUM_CLK,
	        .quadwp_io_num = -1,
	        .quadhd_io_num = -1,
	        .max_transfer_sz = 4000,
	    };

	esp_err_t ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bus.");
        return;
    }


	const size_t workbuf_size = 4096;
	void* workbuf = NULL;
	fs = NULL;

	if (s_card != NULL) {
		//	        return ESP_ERR_INVALID_STATE;
	}

	// connect SDMMC driver to FATFS
	BYTE pdrv = 0xFF;
	if (ff_diskio_get_drive(&pdrv) != ESP_OK || pdrv == 0xFF) {
		ESP_LOGD(TAG, "the maximum count of volumes is already mounted");
		ESP_LOGD(TAG_REDUND, "ESP_ERR_NO_MEM");
	}

	s_base_path = strdup(base_path);
	if(!s_base_path){
		ESP_LOGD(TAG, "could not copy base_path");
		ESP_LOGD(TAG_REDUND, "ESP_ERR_NO_MEM");
	}
	esp_err_t err = ESP_OK;
	// not using ff_memalloc here, as allocation in internal RAM is preferred
	s_card = malloc(sizeof(sdmmc_card_t));
	if (s_card == NULL) {
		err = ESP_ERR_NO_MEM;
		goto fail;
	}

	err = (*host.init)();
	if (err != ESP_OK) {
		ESP_LOGD(TAG, "host init returned rc=0x%x", err);
		goto fail;
	}

	// configure SD slot
	err = sdspi_host_init_device(host.slot,(const sdspi_device_config_t*) &slot_config);
	if (err != ESP_OK) {
		ESP_LOGD(TAG, "slot_config returned rc=0x%x", err);
		goto fail;
	}

	// probe and initialize card
	err = sdmmc_card_init(&host, s_card);
	if (err != ESP_OK) {
		ESP_LOGD(TAG, "sdmmc_card_init failed 0x(%x)", err);
		goto fail;
	}
	if (out_card != NULL) {
		out_card = s_card;
	}

	ff_diskio_register_sdmmc(pdrv, s_card);
	s_pdrv = pdrv;
	ESP_LOGD(TAG, "using pdrv=%i", pdrv);
	char drv[3] = {(char)('0' + pdrv), ':', 0};

	// connect FATFS to VFS
	err = esp_vfs_fat_register(base_path, drv, (size_t)2, &fs);
	if (err == ESP_ERR_INVALID_STATE) {
		// it's okay, already registered with VFS
	} else if (err != ESP_OK) {
		ESP_LOGD(TAG, "esp_vfs_fat_register failed 0x(%x)", err);
		goto fail;
	}

	// Try to mount partition
	FRESULT res = f_mount(fs, drv, 1);
	if (res != FR_OK) {
		err = ESP_FAIL;
		ESP_LOGW(TAG, "failed to mount card (%d)", res);
		//		if (!((res == FR_NO_FILESYSTEM || res == FR_INT_ERR)
		//				&& true)) {
		//			goto fail;
		//		}
		ESP_LOGW(TAG, "partitioning card");
		workbuf = ff_memalloc(workbuf_size);
		if (workbuf == NULL) {
			err = ESP_ERR_NO_MEM;
			goto fail;
		}
		DWORD plist[] = {100, 0, 0, 0};
		res = f_fdisk(s_pdrv, plist, workbuf);
		if (res != FR_OK) {
			err = ESP_FAIL;
			ESP_LOGD(TAG, "f_fdisk failed (%d)", res);
			goto fail;
		}
		size_t alloc_unit_size = esp_vfs_fat_get_allocation_unit_size(
				s_card->csd.sector_size,
				allocation_unit_size);
		ESP_LOGW(TAG, "formatting card, allocation unit size=%d", alloc_unit_size);
		//FIXME: Update to 5.0
		//res = f_mkfs(drv, FM_ANY, alloc_unit_size, workbuf, workbuf_size);
		if (res != FR_OK) {
			err = ESP_FAIL;
			ESP_LOGD(TAG, "f_mkfs failed (%d)", res);
			goto fail;
		}
		free(workbuf);
		workbuf = NULL;
		ESP_LOGW(TAG, "mounting again");
		res = f_mount(fs, drv, 0);
		if (res != FR_OK) {
			err = ESP_FAIL;
			ESP_LOGD(TAG, "f_mount failed after formatting (%d)", res);
			goto fail;
		}
	}
	ESP_LOGD(TAG_REDUND, "ESP_OK");
	goto succ;

	fail:
	ESP_LOGD(TAG_REDUND, "FAILED");
	host.deinit();
	free(workbuf);
	if (fs) {
		f_mount(NULL, drv, 0);
	}
	esp_vfs_fat_unregister_path(base_path);
	ff_diskio_unregister(pdrv);
	free(s_card);
	s_card = NULL;
	free(s_base_path);
	s_base_path = NULL;
	ESP_LOGD(TAG_REDUND, "ERROR:%d", err);

	succ:
	if (REWRITE_INIT)
	{
		ESP_LOGD(TAG_STORAGE, "REWRITING INIT");
		fwriteInit();
	}
	char* pc_response = floadInit();
	free(pc_response);

	xTaskCreate(tstorageRun,		"tlocalStorageRun",		4096, NULL, 7, &ht_storageRun);
}

void tstorageRun (void* param)
{
	uint32_t ui_cmdlet = 0;
	while(1)
	{
		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			ESP_LOGD(TAG_CONF, "CMDLET RECEIVED");
			fsdConfig(ui_cmdlet);
		}
		xEventGroupSync( eg_sync, BIT_STORAGE_SYNC, BIT_STORAGE_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		while((ui_cmdlet != CMD_trig) && (ui_cmdlet != CMD_stop))
		{
			vTaskResume(ht_configRun);
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
		}

		while(ui_cmdlet != CMD_stop)
		{

		}


		stu_mesCell input1;
		stu_mesCell output;
		// Use POSIX and C standard library functions to work with files.
		// First create a file.
		ESP_LOGI(TAG_REDUND, "Opening file");

		FILE* f = fopen(MOUNT_POINT"/redund/0x00.bak", "w");
		if (f == NULL) {
			ESP_LOGE(TAG_REDUND, "Failed to open file for writing");
			return;
		}
//		if (pwrite(fileno(f), p_initConfig, sizeof(struct stu_initConfig), 0) < 0)
//		{
//			ESP_LOGD(TAG_STORAGE, "PREAD ERROR %s", strerror(errno));
//		}
		//fprintf(f, "Hello %s!\n", card->cid.name);
		//fprintf(f, "HELLO WORLD!");

		fwrite (&input1, sizeof(stu_mesCell), 1, f);

		fclose(f);
		ESP_LOGI(TAG_REDUND, "File written");

		//		// Check if destination file exists before renaming
		//		struct stat st;
		//		if (stat("/sdcard/foo.txt", &st) == 0) {
		//			// Delete it if it exists
		//			unlink("/sdcard/foo.txt");
		//		}

		//		// Rename original file
		//		ESP_LOGI(TAG_REDUND, "Renaming file");
		//		if (rename("/sdcard/hello.txt", "/sdcard/foo.txt") != 0) {
		//			ESP_LOGE(TAG_REDUND, "Rename failed");
		//			return;
		//		}

		// Open renamed file for reading
		ESP_LOGI(TAG_REDUND, "Reading file");
		f = fopen(MOUNT_POINT"/redund/0x00.bak", "r");
		if (f == NULL) {
			ESP_LOGE(TAG_REDUND, "Failed to open file for reading");
			return;
		}
		//		char line[64];
		//		fgets(line, sizeof(line), f);
		//		fclose(f);
		//		// strip newline
		//		char* pos = strchr(line, '\n');
		//		if (pos) {
		//			*pos = '\0';
		//		}
		//		ESP_LOGI(TAG_REDUND, "Read from file: '%s'", line);


		fread(&output, sizeof(stu_mesCell), 1, f);
		//		int readSize = freadBytes((char*) astu_entries[1], sizeof(astu_entries[1]));
		fclose(f);

		// All done, unmount partition and disable SDMMC or SPI peripheral
		//esp_vfs_fat_sdmmc_unmount();
		//ESP_LOGI(TAG_REDUND, "Card unmounted");
		break;

	}
}

void fsdConfig(uint32_t ui_cmdlet)
{
	uint32_t ui_offset = 0;
	char* pc_configIn = 0;
	char* pc_configOut = 0;
	char* pc_value = 0;
	char* pc_response = NULL;
	char* ac_fileAddress = malloc(280);
	char* pc_fileName = NULL;
	FILE* f;
	switch(ui_cmdlet)
	{
		//FORMATTING
	case CMD_mkfs:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tDo you really want to format (y/n)?\n");
		fsendResponse(1, 0, pc_configOut);
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		if (*pc_configIn == 'y')
		{
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|ack|0|\t\tStarting format. This might take some time.\n");
			fsendResponse(0, 0, pc_configOut);
			pc_configOut = malloc(64);
			if (fFormatSD() == ESP_OK)
			{
				sprintf(pc_configOut, "|ack|0|\t\tFinished format.\n");
			}
			else
			{
				sprintf(pc_configOut, "|ack|0|\t\tFormat failes.\n");
			}
			fsendResponse(0, 1, pc_configOut);
		}
		else if (*pc_configIn == 'n')
		{
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|ack|0|\t\tAborted formatting.\n");
			fsendResponse(0, 1, pc_configOut);
		}
		else
		{
			pc_configOut = malloc(64);
			sprintf(pc_configOut, "|ack|0|\t\tWrong input. Formatting canceled.\n");
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_wrin:
		fwriteInit();
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	//TODO: implement check for max string size
		//LISTING
	case CMD_list:
		pc_configOut = malloc(1400);
		ui_offset = sprintf(pc_configOut, "\1\2lsst\3:\n");
		pc_fileName = malloc(128);
		sprintf(pc_fileName, "accesspoints");
		ffileList(pc_fileName, pc_configOut + ui_offset);
		fsendResponse(0, 0, pc_configOut);

		pc_configOut = malloc(1400);
		ui_offset = sprintf(pc_configOut, "\1\2lsad\3:\n");
		pc_fileName = malloc(128);
		sprintf(pc_fileName, "adcconfigs");
		ffileList(pc_fileName, pc_configOut + ui_offset);
		fsendResponse(0, 0, pc_configOut);

		pc_configOut = malloc(1400);
		ui_offset = sprintf(pc_configOut, "\1\2lsbl\3:\n");
		pc_fileName = malloc(128);
		sprintf(pc_fileName, "blinkconfigs");
		ffileList(pc_fileName, pc_configOut + ui_offset);
		fsendResponse(0, 0, pc_configOut);

		pc_configOut = malloc(1400);
		ui_offset = sprintf(pc_configOut, "\1\2lslc\3:\n");
		pc_fileName = malloc(128);
		sprintf(pc_fileName, "loadcells");
		ffileList(pc_fileName, pc_configOut + ui_offset);
		fsendResponse(0, 1, pc_configOut);

		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;
			//LIST STATION CONFIGS
	case CMD_lsst:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(1400);
		ui_offset = sprintf(pc_configOut, "\1\2lsst\3:\n");

		pc_fileName = malloc(128);
		sprintf(pc_fileName, "accesspoints");
		ffileList(pc_fileName, pc_configOut + ui_offset);
		fsendResponse(0, 1, pc_configOut);
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;
			//LIST ADC CONFIGS
	case CMD_lsad:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(1400);
		ui_offset = sprintf(pc_configOut, "\1\2lsad\3:\n");

		pc_fileName = malloc(128);
		sprintf(pc_fileName, "adcconfigs");
		ffileList(pc_fileName, pc_configOut + ui_offset);
		fsendResponse(0, 1, pc_configOut);
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

			//LIST BLINK CONFIGS
	case CMD_lsbl:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(1400);
		ui_offset = sprintf(pc_configOut, "\1\2lsbl\3:\n");

		pc_fileName = malloc(128);
		sprintf(pc_fileName, "blinkconfigs");
		ffileList(pc_fileName, pc_configOut + ui_offset);
		fsendResponse(0, 1, pc_configOut);
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;
			//LIST LOADCELLS
	case CMD_lslc:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(1400);
		ui_offset = sprintf(pc_configOut, "\1\2lslc\3:\n");

		pc_fileName = malloc(128);
		sprintf(pc_fileName, "loadcells");
		ffileList(pc_fileName, pc_configOut + ui_offset);
		fsendResponse(0, 1, pc_configOut);
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;


		//BATTERY MONITOR
	case CMD_bath:
	case CMD_batl:
		//SAVING BATMON CONFIG
		f = fopen(MOUNT_POINT"/general/batmon", "w");
		if (f == 0) //could not find init file
		{
			ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/batmon not found");
		}
		else
		{
			fwrite(pstu_batMonConfig,
					sizeof(struct stu_batmonConfig),
					1,
					f);
			fclose(f);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;


		//LOADING
	case CMD_init:
		pc_response = floadInit();
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_CONFIG_SYNC | BIT_STORAGE_SYNC,
				portMAX_DELAY );
		break;

	case CMD_ldad:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/adcconfigs/%s", pc_value);
			f = fopen(ac_fileAddress, "r");
			if (f == 0)
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT OPEN FILE:%s ERROR:%s\n",ac_fileAddress, strerror(i_errnum));
			}
			else
			{
				pc_configOut = malloc(64);
				struct stu_adcConfig config;
				fread(&config, sizeof(config), 1, f);
				portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
				portENTER_CRITICAL(&mux);
				*pstu_adcConfig = config;
				portEXIT_CRITICAL(&mux);
				fclose(f);
				sprintf(pc_configOut, "|ldad|%s|\t\tConfig loaded.\n\4",ac_fileAddress);
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_ldbl:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/blinkconfigs/%s", pc_value);
			f = fopen(ac_fileAddress, "r");
			if (f == 0)
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT OPEN FILE:%s ERROR:%s\n",ac_fileAddress, strerror(i_errnum));
			}
			else
			{
				pc_configOut = malloc(64);
				struct stu_blinkConfig config;
				fread(&config, sizeof(config), 1, f);
				portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
				portENTER_CRITICAL(&mux);
				*pstu_blinkConfig = config;
				portEXIT_CRITICAL(&mux);
				fclose(f);
				sprintf(pc_configOut, "|ldbl|%s|\t\tConfig loaded.\n\4",pc_value);
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_ldlc:
		xQueueReceive(q_send, &pc_value, portMAX_DELAY);
		ESP_LOGD(TAG_STORAGE, "LOADING LOACELL %s", pc_value);
		uint32_t ui32_return = 0;
		sprintf (ac_fileAddress, MOUNT_POINT"/loadcells/%s", pc_value);
		f = fopen(ac_fileAddress, "r");
		if (f != 0)
		{
			pc_configOut = malloc(64);
			struct stu_sensorConfig config;
			fread(&config, sizeof(config), 1, f);
			portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
			portENTER_CRITICAL(&mux);
			*pstu_sensorConfig = config;
			portEXIT_CRITICAL(&mux);
			fclose(f);
			ui32_return = 1;
		}
		xQueueSend(q_recv, &ui32_return, portMAX_DELAY);
		break;

	case CMD_ldst:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/accesspoints/%s", pc_value);
			f = fopen(ac_fileAddress, "r");
			if (f == 0)
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT OPEN FILE:%s ERROR:%s\n",ac_fileAddress, strerror(i_errnum));
			}
			else
			{
				struct stu_staConfig config;
				fread(&config, sizeof(config), 1, f);
				portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
				portENTER_CRITICAL(&mux);
				*pstu_staConfigMom = config;
				portEXIT_CRITICAL(&mux);
				fclose(f);
				sprintf(pc_configOut, "|ldst|%s|\t\tConfig loaded.\n\4",pc_value);
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_ldss: //searching for stationconfig by ssid and loading if found
		ESP_LOGD(TAG_STORAGE, "SEARCHING FOR MATCHING AP");
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		uint32_t ui_flag = 0;
		DIR* dr = opendir(MOUNT_POINT"/accesspoints");
		struct dirent* de = 0;
		struct stu_staConfig config;
		while ((de = readdir(dr)) != NULL)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/accesspoints/%s", de->d_name);
			f = fopen(ac_fileAddress, "r");
			fread(&config, sizeof(struct stu_staConfig), 1, f);
			if (!strcmp(pc_fileName, (config.ac_ssid)))
			{
				portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
				portENTER_CRITICAL(&mux);
				*pstu_staConfigMom = config;
				portEXIT_CRITICAL(&mux);
				fclose(f);
				ui_flag = 1;
				break;
			}
			else
			{
			}
			fclose(f);
		}
		 closedir(dr);

		 xQueueSend(q_recv, &ui_flag, portMAX_DELAY);
		 vTaskDelay(100);
		 ESP_LOGD(TAG_STORAGE, "FINISHED SEARCH");
		break;


		//SAVING
	case CMD_svad:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/adcconfigs/%s", pc_value);
			f = fopen(ac_fileAddress, "w");
			if (f == 0)
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			}
			else
			{
				pc_configOut = malloc(64);
				fwrite(pstu_adcConfig, sizeof(struct stu_adcConfig), 1, f);
				fclose(f);
				sprintf(pc_configOut, "|svad|%s|\t\tConfig saved.\n",ac_fileAddress);
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_svbl:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/blinkconfigs/%s", pc_value);
			f = fopen(ac_fileAddress, "w");
			if (f == 0)
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			}
			else
			{
				pc_configOut = malloc(64);
				fwrite(pstu_blinkConfig, sizeof(struct stu_blinkConfig), 1, f);
				fclose(f);
				sprintf(pc_configOut, "|svbl|%s|\t\tConfig saved.\n",ac_fileAddress);
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_svti://SAVING TEMPERATURE
//		xQueueReceive(q_pconfigIn,
//				&pc_configIn,
//				portMAX_DELAY);
			sprintf(ac_fileAddress, MOUNT_POINT"/general/temperature");
			f = fopen(ac_fileAddress, "w");
			if (f == 0)
			{
//				pc_configOut = malloc(128);
//				int i_errnum = errno;
//				sprintf(pc_configOut, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			}
			else
			{
//				pc_configOut = malloc(64);
				fwrite(pstu_tempConfig,
						sizeof(struct stu_tempConfig),
						1,
						f);
				fclose(f);
//				sprintf(pc_configOut, "|svti|%s|\t\tConfig saved.\n",ac_fileAddress);
			}
//			fsendResponse(0, 1, pc_configOut);
//		xEventGroupSync( eg_sync,
//				BIT_STORAGE_SYNC,
//				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
//				portMAX_DELAY );
		break;

	case CMD_svst://SAVING STATION CONFIG
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/accesspoints/%s", pc_value);
			f = fopen(ac_fileAddress, "w");
			if (f == 0)
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			}
			else
			{
				pc_configOut = malloc(64);
				fwrite(pstu_staConfigMom, sizeof(struct stu_staConfig), 1, f);
				fclose(f);
				sprintf(pc_configOut, "|svst|%s|\t\tConfig saved.\n",ac_fileAddress);
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_svlc:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);

		xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
		while(xTaskNotify(ht_tempIntRun,CMD_svlc,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xQueueReceive(q_recv,
				&pc_value,
				portMAX_DELAY);
		xSemaphoreGive(hs_pointerQueue);
			sprintf(ac_fileAddress,
					MOUNT_POINT"/loadcells/%s",
					pc_value);
		free(pc_value);
				f = fopen(ac_fileAddress, "w");
				if (f == 0)
				{
					pc_configOut = malloc(128);
					int i_errnum = errno;
					sprintf(pc_configOut, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
				}
				else
				{
					pc_configOut = malloc(64);
					fwrite(pstu_sensorConfig, sizeof(struct stu_sensorConfig), 1, f);
					fclose(f);
					sprintf(pc_configOut, "|svlc|%s|\t\tConfig saved.\n",ac_fileAddress);
				}
				fsendResponse(0, 1, pc_configOut);
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

		//REMOVING
	case CMD_rmst://ReMove STation: deletes station config file from storage
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/accesspoints/%s", pc_value);
			if (!remove(ac_fileAddress))
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "FILE DELETED:%s ",ac_fileAddress);
			}
			else
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT DELETE FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_rmlc://ReMove LoadCell: deletes loadcell config file from storage
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/loadcells/%s", pc_value);
			if (!remove(ac_fileAddress))
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "FILE DELETED:%s ",ac_fileAddress);
			}
			else
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT DELETE FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_rmbl:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/blinkconfigs/%s", pc_value);
			if (!remove(ac_fileAddress))
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "FILE DELETED:%s ",ac_fileAddress);
			}
			else
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT DELETE FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_rmad:
		xQueueReceive(q_pconfigIn,
				&pc_configIn,
				portMAX_DELAY);
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|user|\t\tInsert filename:\n");
		pc_value = fgetValuePointer(pc_configIn, pc_configOut);
		if(pc_value != 0)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/adcconfigs/%s", pc_value);
			if (!remove(ac_fileAddress))
			{
				pc_configOut = malloc(128);
				sprintf(pc_configOut, "FILE DELETED:%s ",ac_fileAddress);
			}
			else
			{
				pc_configOut = malloc(128);
				int i_errnum = errno;
				sprintf(pc_configOut, "COULD NOT DELETE FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			}
			fsendResponse(0, 1, pc_configOut);
		}
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;

	case CMD_cllc: //CLearLoadCell: removes loadcell from storage and clears its non volatile storage inside the ds18b20 eeprom
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress,
				MOUNT_POINT"/loadcells/%s.cfg",
				pc_fileName);

		pc_response = malloc(128);
		if (!remove(ac_fileAddress))
		{
			sprintf(pc_response,
					"|cllc|1|\t\tLoadcell %s has been removed.\n\4",
					pc_fileName);
		}
		else
		{
			sprintf(pc_response,
					"|cllc|0|\t\tLoadcell %s could not be removed ERRNO:%d %s.\n\4",
					ac_fileAddress, errno, strerror(errno));
		}
		xQueueSend(q_send, &pc_response, portMAX_DELAY);
		xEventGroupSync( eg_sync,
				BIT_STORAGE_SYNC,
				BIT_CONFIG_SYNC | BIT_STORAGE_SYNC,
				portMAX_DELAY );
		break;


	default:
		ESP_LOGD(TAG_STORAGE, "WRONG CMDLET");
		break;
	}
//	free(ac_fileAddress);
}


esp_err_t fFormatSD()
{
	char* pc_configOut = 0;
	const size_t workbuf_size = 4096;
	void* workbuf = NULL;
	char drv[3] = {(char)('0' + pdrv), ':', 0};
	esp_err_t err = ESP_OK;

	// Try to mount partition
	FRESULT res = f_mount(fs, drv, 1);
	if (res == FR_OK)
	{
		workbuf = ff_memalloc(workbuf_size);
		if (workbuf == NULL) {
			err = ESP_ERR_NO_MEM;
		}
		DWORD plist[] = {100, 0, 0, 0};
		res = f_fdisk(s_pdrv, plist, workbuf);
		if (res != FR_OK) {
			err = ESP_FAIL;
			ESP_LOGD(TAG_REDUND, "f_fdisk failed (%d)", res);
		}

		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tMaking Filesystem.\n");
		fsendResponse(0, 0, pc_configOut);
		//FIXME: Update to 5.0
		//res = f_mkfs(drv, FM_FAT32, alloc_unit_size, workbuf, workbuf_size);
		if (res != FR_OK) {
			err = ESP_FAIL;
			ESP_LOGD(TAG_REDUND, "f_mkfs failed (%d)", res);
		}
		free(workbuf);
		workbuf = NULL;

		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tMounting Filesystem.\n");
		fsendResponse(0, 0, pc_configOut);
		res = f_mount(fs, drv, 0);
		if (res != FR_OK) {
			err = ESP_FAIL;
			ESP_LOGD(TAG, "f_mount failed after formatting (%d)", res);
		}
	}
	if (res == FR_OK)
	{
		pc_configOut = malloc(64);
		sprintf(pc_configOut, "|ack|0|\t\tWriting Init.\n");
		fsendResponse(0, 0, pc_configOut);
		fwriteInit();
	}

	return err;
}


uint32_t fwriteInit(void)
{
	uint32_t status = 0;
	FILE* f;
	mkdir(MOUNT_POINT"/databackup", 0777);
	mkdir(MOUNT_POINT"/general", 0777);
	mkdir(MOUNT_POINT"/loadcells", 0777);
	mkdir(MOUNT_POINT"/adcconfigs", 0777);
	mkdir(MOUNT_POINT"/blinkconfigs", 0777);
	mkdir(MOUNT_POINT"/accesspoints", 0777);

	f = fopen(MOUNT_POINT"/general/batmon", "w");
	struct stu_batmonConfig batmonConfig = {
			.i_batRawLow	= 0,
			.f_batVolLow	= 0,
			.i_batRawHigh	= 1023,
			.f_batVolHigh	= 3.2};
	fwrite(&batmonConfig,
			sizeof(struct stu_batmonConfig),
			1,
			f);
	fclose(f);

	f = fopen(MOUNT_POINT"/general/trigger", "w");
	struct stu_triggerConfig trigger = {
			.b_pinMode = 1,
			.ui_timeout	= 60000};
	fwrite(&trigger,
			sizeof(struct stu_triggerConfig),
			1,
			f);
	fclose(f);

	f = fopen(MOUNT_POINT"/general/accesspoint", "w");
	struct stu_apConfig accesspoint = {
			.ac_ssid		= "",
			.ac_pass		= "",
			.portConf	= 4243,
			.portMes	= 4242,
			.portBroad	= 4241,
			.ipTrig		= 0
			};
	uint8_t mac[6];
	esp_read_mac(mac, 0);
	sprintf(accesspoint.ac_pass,
			"%x%x%x%x%x%x",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	sprintf(accesspoint.ac_ssid,
			"FLADC_"MODULE_NAME"_%X-%X-%X-%X-%X-%X",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	IP4_ADDR(&accesspoint.ipInfo.ip, 10, 0, 0, 1);
	IP4_ADDR(&accesspoint.ipInfo.gw, 10, 0, 0, 0);
	IP4_ADDR(&accesspoint.ipInfo.netmask, 255, 255, 255, 0);
	fwrite(&accesspoint,
			sizeof(struct stu_apConfig),
			1,
			f);
	fclose(f);

	f = fopen(MOUNT_POINT"/general/temperature", "w");
	struct stu_tempConfig tempConfig = {
			.romExt = {0},
			.romInt = {0},
			.ui_perInt				= 10000,
			.ui_perExt				= 10000};
	fwrite(&tempConfig,
			sizeof(struct stu_tempConfig),
			1,
			f);
	fclose(f);

	f = fopen(MOUNT_POINT"/loadcells/default", "w");
	struct stu_sensorConfig cellConfig = {
			.ac_name[0]		= '\0',
			.d_calValue		= 1.0,
			.i_tareValue	= 0,
			.i_tareZero		= 0,
			.i8_perOverload = 20,
			.ui8_maxLoad	= 209};
	fwrite(&cellConfig,
			sizeof(struct stu_sensorConfig),
			1,
			f);
	fclose(f);

	f = fopen(MOUNT_POINT"/adcconfigs/default", "w");
	struct stu_adcConfig adcConfig = {
			.uc_txMode		= TXMODE_TCP_PLUS_SDBACK,
			.ul_adcPeriod	= 1000000,
			.uc_numDecimals	= 2,
			.uc_unit		= UNIT_N};
	fwrite(&adcConfig,
			sizeof(struct stu_adcConfig),
			1,
			f);
	fclose(f);

	f = fopen(MOUNT_POINT"/accesspoints/default", "w");
	struct stu_staConfig staConfig = {
			.ac_ssid		= "",
			.ac_pass		= "",
			.portConf	= 4243,
			.portMes	= 4242,
			.portBroad	= 4241,
			.ipTrig		= 0};
	fwrite(&staConfig,
			sizeof(struct stu_staConfig),
			1,
			f);
	fclose(f);

	f = fopen(MOUNT_POINT"/blinkconfigs/default", "w");
	struct stu_blinkConfig blinkConfig = {
			.ui_blinkPeriod		= 10000,
			.ui_blinkDuration	= 100,
			.ui_blinkBrightness	= 200,
			.ui_blinkFrequency	= 50000};
	fwrite(&blinkConfig,
			sizeof(struct stu_blinkConfig),
			1,
			f);
	fclose(f);
	return status;
}


uint32_t		ffileList		(char* pc_fileName,
								char* pc_configOut)
{
	char* pc_fileAddress = malloc(128);
	sprintf(pc_fileAddress, MOUNT_POINT"/%s", pc_fileName);
	DIR* dr = opendir(pc_fileAddress);
	free(pc_fileAddress);
	struct dirent* de = 0;
	uint32_t ui_offset = 0;
	unsigned int ui_fileNumber = 1;
	while ((de = readdir(dr)) != NULL)
	{
		ui_offset += sprintf(pc_configOut + ui_offset, "\t%d:\2%s\3\n", ui_fileNumber, de->d_name);
		ui_fileNumber++;
	}
	sprintf(pc_configOut + ui_offset, "\n\4");
	 closedir(dr);
	 free(pc_fileName);

	return 1;
}


char* floadInit()
{
	char* pc_response = malloc(64);
	FILE* f = 0;
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

	f = fopen(MOUNT_POINT"/general/batmon", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/batmon not found");
	}
	else
	{
		struct stu_batmonConfig config;
		fread(&config,
				sizeof(struct stu_batmonConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		*pstu_batMonConfig = config;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/general/trigger", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/trigger not found");
	}
	else
	{
		struct stu_triggerConfig config;
		fread(&config,
				sizeof(struct stu_triggerConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		*pstu_triggerConfig = config;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/general/accesspoint", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/accesspoint not found");
	}
	else
	{
		struct stu_apConfig config;
		fread(&config,
				sizeof(struct stu_apConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		*pstu_hotspotConfig = config;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/general/temperature", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/temperature not found");
	}
	else
	{
		struct stu_tempConfig config;
		fread(&config,
				sizeof(struct stu_tempConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		*pstu_tempConfig = config;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/loadcells/default", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/loadcells/default not found");
	}
	else
	{
		struct stu_sensorConfig config;
		fread(&config,
				sizeof(struct stu_sensorConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		*pstu_sensorConfig = config;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/adcconfigs/default", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/adcconfigs/default not found");
	}
	else
	{
		struct stu_adcConfig config;
		fread(&config,
				sizeof(struct stu_adcConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		*pstu_adcConfig = config;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/blinkconfigs/default", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/blinkconfigs/default not found");
	}
	else
	{
		struct stu_blinkConfig config;
		fread(&config,
				sizeof(struct stu_blinkConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		*pstu_blinkConfig = config;
		portEXIT_CRITICAL(&mux);
	}

	f = fopen(MOUNT_POINT"/accesspoints/default", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/accesspoints/default not found");
	}
	else
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/accesspoints/default loaded");
		struct stu_staConfig config;
		fread(&config,
				sizeof(struct stu_staConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		*pstu_staConfigMom = config;
		portEXIT_CRITICAL(&mux);
	}


	return pc_response;
}
