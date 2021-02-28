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

void fstorageInit(void)
{
	gstu_config = calloc(1, sizeof(struct stu_initConfig));
	gstu_config->staConfig.portBroad = 16000;
	gstu_config->staConfig.portConf = 4243;
	gstu_config->staConfig.portMes = 4242;

	ESP_LOGI(TAG_REDUND, "Initializing SD card");

	ESP_LOGI(TAG_REDUND, "Using SPI peripheral");
	sdmmc_host_t host = SDSPI_HOST_DEFAULT();
	sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
	sdmmc_card_t* out_card;

	slot_config.gpio_miso = PIN_NUM_MISO;
	slot_config.gpio_mosi = PIN_NUM_MOSI;
	slot_config.gpio_sck  = PIN_NUM_CLK;
	slot_config.gpio_cs   = 0;

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
	err = sdspi_host_init_slot(host.slot,(const sdspi_slot_config_t*) &slot_config);
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
		res = f_mkfs(drv, FM_ANY, alloc_unit_size, workbuf, workbuf_size);
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


		struct stu_mesCell input1;
		struct stu_mesCell output;
		input1.b_type = 1;
		input1.d_measurement = 250.5;
		input1.ul_time = 12500;
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

		fwrite (&input1, sizeof(struct stu_mesCell), 1, f);
		printf("DATA WRITE: TYPE:%d, TIME:%llu, VALUE:%f\n", input1.b_type, input1.ul_time, input1.d_measurement);

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


		fread(&output, sizeof(struct stu_mesCell), 1, f);
		printf("DATA READ: TYPE:%d, TIME:%llu, VALUE:%f\n", output.b_type, output.ul_time, output.d_measurement);
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
	void* pv_gstu_config = NULL;
	struct stu_initConfig*		p_initConfig		=	NULL;
	char* pc_response = NULL;
	char* ac_fileAddress = malloc(280);
	char* pc_fileName = NULL;
	FILE* f;
	switch(ui_cmdlet)
	{
		//FORMATTING
	case CMD_mkfs:
		fFormatSD();
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
			//LIST STATION CONFIGS
	case CMD_lswi:
		pc_fileName = malloc(128);
		sprintf(pc_fileName, "accesspoints");
		pc_response = ffileList(pc_fileName);
		xQueueSend(q_send, &pc_response, portMAX_DELAY);
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;
			//LIST ADC CONFIGS
	case CMD_lsad:
		pc_fileName = malloc(128);
		sprintf(pc_fileName, "adcconfigs");
		pc_response = ffileList(pc_fileName);
		xQueueSend(q_send, &pc_response, portMAX_DELAY);
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;
			//LIST BLINK CONFIGS
	case CMD_lsbl:
		pc_fileName = malloc(128);
		sprintf(pc_fileName, "blinkconfigs");
		pc_response = ffileList(pc_fileName);
		xQueueSend(q_send, &pc_response, portMAX_DELAY);
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;
			//LIST LOADCELLS
	case CMD_lslc:
		pc_fileName = malloc(128);
		sprintf(pc_fileName, "loadcells");
		pc_response = ffileList(pc_fileName);
		xQueueSend(q_send, &pc_response, portMAX_DELAY);
		xEventGroupSync(eg_sync,
				BIT_STORAGE_SYNC ,
				BIT_STORAGE_SYNC | BIT_CONFIG_SYNC,
				portMAX_DELAY );
		break;


		//BATTERY MONITOR
	case CMD_bath:
	case CMD_batl:
		//SAVING BATMON CONFIG
		f = fopen(MOUNT_POINT"/config/def.ini", "r");
		if (f != 0)
		{
			p_initConfig	=	malloc(sizeof(struct stu_initConfig));
			fread(p_initConfig,
					sizeof(struct stu_initConfig),
					1,
					f);
			fclose(f);

			p_initConfig->batMon = gstu_config->batMon;

			f = fopen(MOUNT_POINT"/config/def.ini", "w");
			fwrite(p_initConfig,
					sizeof(struct stu_initConfig),
					1,
					f);
			fclose(f);
			free(p_initConfig);
		}
		else
		{
			sprintf(pc_response, "|bath|0|\t\tERROR COULD NOT OPEN FILE|\n");
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
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/adcconfigs/%s", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			struct stu_adcConfig config;
			fread(&config, sizeof(config), 1, f);
			portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
			portENTER_CRITICAL(&mux);
			gstu_config->adc = config;
			portEXIT_CRITICAL(&mux);
			fclose(f);

			sprintf(pc_response, "|ldtc|%s|\t\tConfig loaded.\n\4",ac_fileAddress);
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_ldbl:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/blinkconfigs/%s", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			struct stu_blinkConfig config;
			fread(&config, sizeof(config), 1, f);
			portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
			portENTER_CRITICAL(&mux);
			gstu_config->blink = config;
			portEXIT_CRITICAL(&mux);
			fclose(f);

			sprintf(pc_response, "|ldbl|%s|\t\tConfig loaded\n\4",ac_fileAddress);
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_ldlc:
		pc_fileName = malloc(64);
		owb_string_from_rom_code(gstu_config->temp.romExt,pc_fileName,OWB_ROM_CODE_STRING_LENGTH);
		sprintf(ac_fileAddress, MOUNT_POINT"/loadcells/%s", pc_fileName);
		free(pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			ESP_LOGD(TAG_STORAGE, "CELL %s not found", ac_fileAddress);
		}
		else
		{
			struct stu_cellConfig config;
			fread(&config,
					sizeof(struct stu_cellConfig),
					1,
					f);
			portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
			portENTER_CRITICAL(&mux);
			gstu_config->cell = config;
			portEXIT_CRITICAL(&mux);
			fclose(f);
			ESP_LOGD(TAG_STORAGE, "CELL %s opened", ac_fileAddress);
		}
		break;

	case CMD_ldwi:
		pc_response = malloc(128);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/wifi/%s", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_wifiRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_send, &pv_gstu_config, portMAX_DELAY);
			fread((wifi_config_t*)pv_gstu_config, sizeof(wifi_config_t), 1, f);
			fclose(f);
			sprintf(pc_response, "|ldwi|%s|\t\tConfig loaded\n", ac_fileAddress);
			xEventGroupSync( eg_sync, BIT_STORAGE_SYNC , BIT_STORAGE_SYNC | BIT_WIFI_SYNC, portMAX_DELAY );
		}
		xEventGroupSync( eg_config, BIT_STORAGE_SYNC , BIT_STORAGE_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		xQueueSend(q_send, &pc_response, portMAX_DELAY);
		break;

	case CMD_ldst:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/accesspoints/%s", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			struct stu_staConfig config;
			fread(&config, sizeof(config), 1, f);
			portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
			portENTER_CRITICAL(&mux);
			gstu_config->staConfig = config;
			portEXIT_CRITICAL(&mux);
			fclose(f);

			sprintf(pc_response, "|ldst|%s|\t\tConfig loaded\n\4",ac_fileAddress);
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_ldss:
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		uint32_t ui_flag = 0;
		DIR* dr = opendir(MOUNT_POINT"/accesspoints");
		struct dirent* de = 0;
		struct stu_staConfig config;
		while ((de = readdir(dr)) != NULL)
		{
			sprintf(ac_fileAddress, MOUNT_POINT"/accesspoints/%s", de->d_name);
			f = fopen(ac_fileAddress, "r");
			fread(&config, sizeof(config), 1, f);
			if (!strcmp(pc_fileName, (config.ac_ssid)))
			{
				portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
				portENTER_CRITICAL(&mux);
				gstu_config->staConfig = config;
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
		break;


		//SAVING
	case CMD_svad:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/adcconfigs/%s", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			fwrite(&(gstu_config->adc), sizeof(gstu_config->adc), 1, f);
			fclose(f);
			sprintf(pc_response, "|svad|%s|\t\tConfig saved.\n",ac_fileAddress);
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_svbl:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/blinkconfigs/%s", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			fwrite(&(gstu_config->blink), sizeof(gstu_config->blink), 1, f);
			fclose(f);
			sprintf(pc_response, "|svbl|%s|\t\tConfig saved.\n",ac_fileAddress);
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_svti:
		//SAVING TEMPERATURE
		f = fopen(MOUNT_POINT"/general/temperature", "w");
		if (f == 0) //could not find init file
		{
			ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/temperature not found");
		}
		else
		{
			fwrite(&gstu_config->temp,
					sizeof(struct stu_tempConfig),
					1,
					f);
			fclose(f);
		}
		break;

	case CMD_svst:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/accesspoints/%s", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			fwrite(&(gstu_config->staConfig), sizeof(gstu_config->staConfig), 1, f);
			fclose(f);
			sprintf(pc_response, "STA CONFIG SAVED:%s",ac_fileAddress);
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_svlc:
		pc_response = malloc(64);

		sprintf(ac_fileAddress,
				MOUNT_POINT"/loadcells/%s",
				gstu_config->cell.ac_name);
		f = fopen(ac_fileAddress, "w");

		if (f == 0)
		{
			//TODO: catch for if file could not be created
			sprintf(pc_response,
					"|svlc|1|\t\tLoadcell %s could not be saved.\n\4",
					pc_fileName);
		}
		else
		{
			fwrite(&gstu_config->cell, sizeof(struct stu_cellConfig), 1, f);
			fclose(f);
			sprintf(pc_response,
					"|svlc|1|\t\tLoadcell %s has been saved.\n\4",
					gstu_config->cell.ac_name);
			ESP_LOGD(TAG_STORAGE, "CELL %s saved", ac_fileAddress);
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_svwi:
		pc_response = malloc(128);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/wifi/%s", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_send, &i_errnum, portMAX_DELAY);
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_wifiRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_send, &pv_gstu_config, portMAX_DELAY);
			fwrite((wifi_config_t*)pv_gstu_config, sizeof(wifi_config_t), 1, f);
			fclose(f);
			sprintf(pc_response, "|svwi|%s|\t\tConfig saved.\n", ac_fileAddress);
			xEventGroupSync( eg_sync, BIT_STORAGE_SYNC , BIT_STORAGE_SYNC | BIT_WIFI_SYNC, portMAX_DELAY );
		}
		xEventGroupSync( eg_config, BIT_STORAGE_SYNC , BIT_STORAGE_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		xQueueSend(q_send, &pc_response, portMAX_DELAY);
		break;

	case CMD_inad:
		pc_response = malloc(128);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/def.ini", "r+");
		if(f == 0)
		{
			sprintf(pc_response, "|inad|0|\t\tCould not find ini file.\n");
			xEventGroupSync( eg_sync, BIT_STORAGE_SYNC | BIT_ADC_SYNC, BIT_STORAGE_SYNC | BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		}
		else
		{
			while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_send, &pv_gstu_config, portMAX_DELAY);
			fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
			p_initConfig->adc = *(struct stu_adcConfig*)pv_gstu_config;
			xEventGroupSync( eg_sync, BIT_STORAGE_SYNC, BIT_STORAGE_SYNC | BIT_ADC_SYNC, portMAX_DELAY );
			fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
			fclose(f);
			free(p_initConfig);
			sprintf(pc_response, "|inad|0|\t\tADC config written to ini.\n");
			xEventGroupSync( eg_sync, BIT_STORAGE_SYNC , BIT_STORAGE_SYNC | BIT_ADC_SYNC | BIT_CONFIG_SYNC, portMAX_DELAY );
		}
		xQueueSend(q_send, &pc_response, portMAX_DELAY);
		break;
	case CMD_intc:
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
//		strcpy(p_initConfig->tcpConf,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;
	case CMD_intm:
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
//		strcpy(p_initConfig->tcpMes,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;
	case CMD_inbl:
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
//		strcpy(p_initConfig->blink,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;
	case CMD_intr:
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
//		strcpy(p_initConfig->trigger,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;
	case CMD_inwi:
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
//		strcpy(p_initConfig->wifi,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;


		//REMOVING
	case CMD_rmst:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/accesspoints/%s", pc_fileName);
		if (!remove(ac_fileAddress))
		{
			sprintf(pc_response, "FILE DELETED:%s ",ac_fileAddress);
		}
		else
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT DELETE FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_rmlc:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/loadcells/%s", pc_fileName);
		if (!remove(ac_fileAddress))
		{
			sprintf(pc_response, "FILE DELETED:%s \n",ac_fileAddress);
		}
		else
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT DELETE FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_rmbl:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/blinkconfigs/%s", pc_fileName);
		if (!remove(ac_fileAddress))
		{
			sprintf(pc_response, "FILE DELETED:%s ",ac_fileAddress);
		}
		else
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT DELETE FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_rmad:
		pc_response = malloc(64);
		xQueueReceive(q_send, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/adcconfigs/%s", pc_fileName);
		if (!remove(ac_fileAddress))
		{
			sprintf(pc_response, "FILE DELETED:%s ",ac_fileAddress);
		}
		else
		{
			int i_errnum = errno;
			sprintf(pc_response, "COULD NOT DELETE FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		xQueueSend(q_recv, &pc_response, portMAX_DELAY);
		break;

	case CMD_cllc: //CLearLoadCell: removes loadcell from storage and clears its non volatile storage inside ther ds18b20 eeprom
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
	const size_t workbuf_size = 4096;
	void* workbuf = NULL;
	char drv[3] = {(char)('0' + pdrv), ':', 0};
	esp_err_t err = ESP_OK;

	ESP_LOGD(TAG_REDUND, "FORMATTING CARD THIS MIGHT AKE A WHILE");
	// Try to mount partition
	FRESULT res = f_mount(fs, drv, 1);
	if (res == FR_OK)
	{
		ESP_LOGW(TAG, "partitioning card");
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

		ESP_LOGW(TAG, "formatting card, allocation unit size=%d", alloc_unit_size);
		res = f_mkfs(drv, FM_FAT32, alloc_unit_size, workbuf, workbuf_size);
		if (res != FR_OK) {
			err = ESP_FAIL;
			ESP_LOGD(TAG_REDUND, "f_mkfs failed (%d)", res);
		}
		free(workbuf);
		workbuf = NULL;
		ESP_LOGW(TAG_REDUND, "mounting again");
		res = f_mount(fs, drv, 0);
		if (res != FR_OK) {
			err = ESP_FAIL;
			ESP_LOGD(TAG, "f_mount failed after formatting (%d)", res);
		}
		ESP_LOGD(TAG_REDUND, "FINISHED FORMATTING");
	}
	if (res == FR_OK)
	{
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
			"FLADC_"MOUDLE_NAME"_%X-%X-%X-%X-%X-%X",
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
			.romExt = { .fields = {.family = {0}}},
			.romInt = { .fields = {.family = {0}}},
			.ui_perInt				= 10000,
			.ui_perExt				= 10000};
	fwrite(&tempConfig,
			sizeof(struct stu_tempConfig),
			1,
			f);
	fclose(f);

	f = fopen(MOUNT_POINT"/loadcells/default", "w");
	struct stu_cellConfig cellConfig = {
			.ac_name[0]		= '\0',
			.ui_loadRating	= 0,
			.d_calValue		= 1.0,
			.ui_tareValue	= 0};
	fwrite(&cellConfig,
			sizeof(struct stu_cellConfig),
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

	f = fopen(MOUNT_POINT"/blinkconfigs/default", "w");
	struct stu_blinkConfig blinkConfig = {
			.ui_blinkPeriod		= 10000,
			.ui_blinkDuration	= 100,
			.ui_blinkBrightness	= 200,
			.ui_blinkFrequency	= 50000,
			.b_blinkEnabled		= 1};
	fwrite(&blinkConfig,
			sizeof(struct stu_blinkConfig),
			1,
			f);
	fclose(f);

	return status;
}


char* ffileList(char* pc_fileName)
{
	char* pc_fileAddress = malloc(128);
	sprintf(pc_fileAddress, MOUNT_POINT"/%s", pc_fileName);
	char* pc_fileList = malloc(512);
	DIR* dr = opendir(pc_fileAddress);
	free(pc_fileAddress);
	struct dirent* de = 0;
	uint32_t ui_offset = sprintf(pc_fileList, "\1\2%s\3:\n", pc_fileName);
	uint32_t ui_fileNumber = 1;
	while ((de = readdir(dr)) != NULL)
	{
		ui_offset += sprintf(pc_fileList + ui_offset, "\t%d:\2%s\3\n", ui_fileNumber, de->d_name);
		ui_fileNumber++;
	}
	sprintf(pc_fileList + ui_offset, "\n\4");
	 closedir(dr);
	 free(pc_fileName);

	return pc_fileList;
}


char* floadInit()
{
	char* pc_response = malloc(64);
	FILE* f = 0;
	struct stu_initConfig* stu_initConfig_mom = calloc(1, sizeof(struct stu_initConfig));
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

	f = fopen(MOUNT_POINT"/general/batmon", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/batmon not found");
	}
	else
	{
		fread((void*)&stu_initConfig_mom->batMon,
				sizeof(struct stu_batmonConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		gstu_config->batMon = stu_initConfig_mom->batMon;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/general/trigger", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/trigger not found");
	}
	else
	{
		fread((void*)&stu_initConfig_mom->trigger,
				sizeof(struct stu_triggerConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		gstu_config->trigger = stu_initConfig_mom->trigger;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/general/accesspoint", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/accesspoint not found");
	}
	else
	{
		fread((void*)&stu_initConfig_mom->apConfig,
				sizeof(struct stu_apConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		gstu_config->apConfig = stu_initConfig_mom->apConfig;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/general/temperature", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/general/temperature not found");
	}
	else
	{
		fread((void*)&stu_initConfig_mom->temp,
				sizeof(struct stu_tempConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		gstu_config->temp = stu_initConfig_mom->temp;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/loadcells/default", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/loadcells/default not found");
	}
	else
	{
		fread((void*)&stu_initConfig_mom->cell,
				sizeof(struct stu_cellConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		gstu_config->cell = stu_initConfig_mom->cell;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/adcconfigs/default", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/adcconfigs/default not found");
	}
	else
	{
		fread((void*)&stu_initConfig_mom->adc,
				sizeof(struct stu_adcConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		gstu_config->adc = stu_initConfig_mom->adc;
		portEXIT_CRITICAL(&mux);
	}
	f = fopen(MOUNT_POINT"/blinkconfigs/default", "r");
	if (f == 0) //could not find init file
	{
		ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/blinkconfigs/default not found");
	}
	else
	{
		fread((void*)&stu_initConfig_mom->blink,
				sizeof(struct stu_blinkConfig),
				1,
				f);
		fclose(f);
		portENTER_CRITICAL(&mux);
		gstu_config->blink = stu_initConfig_mom->blink;
		portEXIT_CRITICAL(&mux);
	}

	free(stu_initConfig_mom);
	ESP_LOGD(TAG_STORAGE, "LOADING FINISHED");

	return pc_response;
}
