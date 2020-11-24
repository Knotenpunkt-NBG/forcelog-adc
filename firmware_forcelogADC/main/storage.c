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
	ESP_LOGI(TAG_REDUND, "Initializing SD card");

	ESP_LOGI(TAG_REDUND, "Using SPI peripheral");
	esp_log_level_set(TAG, 5);
	sdmmc_host_t host = SDSPI_HOST_DEFAULT();
	sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
	sdmmc_card_t* out_card;

	slot_config.gpio_miso = PIN_NUM_MISO;
	slot_config.gpio_mosi = PIN_NUM_MOSI;
	slot_config.gpio_sck  = PIN_NUM_CLK;
	slot_config.gpio_cs   = PIN_NUM_CS;

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
	xTaskCreate(tstorageRun,		"tlocalStorageRun",		4096, NULL, 7, &ht_storageRun);

	// Card has been initialized, print its properties

}

void tstorageRun (void* param)
{
	vTaskSuspend(NULL);
	ESP_LOGD(TAG_REDUND, "STARTING tlocalStorageRun");
	uint32_t ui_cmdlet = 0;
	while(1)
	{
		while(ui_cmdlet != CMD_wait)
		{
			xTaskNotifyWait(false, ULONG_MAX, &ui_cmdlet, portMAX_DELAY);
			fsdConfig(ui_cmdlet);
		}

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
	vTaskDelete(NULL);
}

void fsdConfig(uint32_t ui_cmdlet)
{
	void* pv_config_mom = NULL;
	struct stu_initConfig*		p_initConfig		=	NULL;
	char* pc_response = NULL;
	char* ac_fileAddress = malloc(64);
	char* pc_fileName = NULL;
	FILE* f;
	switch(ui_cmdlet)
	{
	case CMD_mkfs:
		fFormatSD();
		vTaskResume(ht_configRun);
		break;

	case CMD_bath:
	case CMD_batl:
		//SAVING BATMON CONFIG
		xQueuePeek(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/batmon.def", "w");
		fwrite((struct stu_batmonConfig*)pv_config_mom, sizeof(struct stu_batmonConfig), 1, f);
		fclose(f);
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		xSemaphoreGive(hs_pointerQueue);
		break;

	case CMD_init:
		f = fopen(MOUNT_POINT"/config/def.ini", "r");
		if (f == 0)
		{
			ESP_LOGW(TAG_STORAGE, MOUNT_POINT"/config/def.ini");
		}
		else
		{
			p_initConfig	=	malloc(sizeof(struct stu_initConfig));
			fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
			fclose(f);

			xSemaphoreTake(hs_pointerQueue, portMAX_DELAY);
			while(xTaskNotify(ht_adcRun,CMD_ldad,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			*((struct stu_adcConfig*)pv_config_mom) = p_initConfig->adc;
			vTaskResume(ht_adcRun);

			while(xTaskNotify(ht_tcpMes,CMD_ldad,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			*((struct stu_adcConfig*)pv_config_mom) = p_initConfig->adc;
			vTaskResume(ht_tcpMes);

			while(xTaskNotify(ht_tcpConf,CMD_ldtc,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			*((struct sockaddr_in*)pv_config_mom) = p_initConfig->tcpConf;
			vTaskResume(ht_tcpConf);

			while(xTaskNotify(ht_wifiRun,CMD_ldwi,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			*((struct stu_wifiConfig*)pv_config_mom) = p_initConfig->wifi;
			vTaskResume(ht_wifiRun);

			while(xTaskNotify(ht_tcpMes,CMD_ldtm,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			*((struct sockaddr_in*)pv_config_mom) = p_initConfig->tcpMes;
			vTaskResume(ht_tcpMes);

			while(xTaskNotify(ht_blinkRun,CMD_ldbl,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			*((struct stu_blinkConfig*)pv_config_mom) = p_initConfig->blink;
			vTaskResume(ht_blinkRun);

			while(xTaskNotify(ht_adcRun,CMD_ldlc,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			*((struct stu_cellConfig*)pv_config_mom) = p_initConfig->blink;
			vTaskResume(ht_adcRun);

			while(xTaskNotify(ht_batmonRun,CMD_init,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			*((struct stu_batmonConfig*)pv_config_mom) = p_initConfig->batMon;
			fread((struct stu_batmonConfig*)pv_config_mom, sizeof(struct stu_batmonConfig), 1, f);
			fclose(f);
			vTaskResume(ht_batmonRun);

			free(p_initConfig);
			xSemaphoreGive(hs_pointerQueue);
		}


		vTaskResume(ht_configRun);
		ESP_LOGD(TAG_STORAGE, "FINISHED INIT");
		break;
	case CMD_ldad:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/adc/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread((struct stu_adcConfig*)pv_config_mom, sizeof(struct stu_adcConfig), 1, f);
			vTaskResume(ht_adcRun);

			while(xTaskNotify(ht_tcpMes,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread((struct stu_adcConfig*)pv_config_mom, sizeof(struct stu_adcConfig), 1, f);
			vTaskResume(ht_tcpMes);

			fclose(f);
			sprintf(pc_response, "|ldtc|%s|\t\tConfig loaded\n", ac_fileAddress);

		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		vTaskResume(ht_configRun);
		vTaskSuspend(NULL);
		free(pc_response);
		break;
	case CMD_ldtc:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/tcpconf/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_tcpConf,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread((struct sockaddr_in*)pv_config_mom, sizeof(struct sockaddr_in), 1, f);
			fclose(f);
			sprintf(pc_response, "|ldtc|%s|\t\tConfig loaded\n", ac_fileAddress);
			vTaskResume(ht_tcpConf);
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		vTaskResume(ht_configRun);
		vTaskSuspend(NULL);
		free(pc_response);
		break;
	case CMD_ldtm:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/tcpmes/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_tcpMes,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread((struct sockaddr_in*)pv_config_mom, sizeof(struct sockaddr_in), 1, f);
			fclose(f);
			sprintf(pc_response, "|ldtc|%s|\t\tConfig loaded\n", ac_fileAddress);
			vTaskResume(ht_tcpMes);
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		vTaskResume(ht_configRun);
		vTaskSuspend(NULL);
		free(pc_response);
		break;
	case CMD_ldbl:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/blink/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread((struct stu_blinkConfig*)pv_config_mom, sizeof(struct stu_blinkConfig), 1, f);
			fclose(f);
			sprintf(pc_response, "|ldwi|%s|\t\tConfig loaded\n", ac_fileAddress);
			vTaskResume(ht_blinkRun);
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		vTaskResume(ht_configRun);
		vTaskSuspend(NULL);
		free(pc_response);
		break;
	case CMD_ldlc:
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/cells/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			vTaskResume(ht_configRun);
		}
		else
		{
			int i = 0;
			xQueueSend(q_pointer, &i, portMAX_DELAY);
			vTaskResume(ht_configRun);
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread((struct stu_cellConfig*)pv_config_mom, sizeof(struct stu_cellConfig), 1, f);
			fclose(f);
			vTaskResume(ht_adcRun);
			vTaskResume(ht_configRun);
		}
		break;
	case CMD_ldwi:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/wifi/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_wifiRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread((wifi_config_t*)pv_config_mom, sizeof(wifi_config_t), 1, f);
			fclose(f);
			sprintf(pc_response, "|ldwi|%s|\t\tConfig loaded\n", ac_fileAddress);
			vTaskResume(ht_wifiRun);
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		vTaskResume(ht_configRun);
		vTaskSuspend(NULL);
		free(pc_response);
		break;
	case CMD_defl:
		ESP_LOGD(TAG_REDUND, "LOADING DEFAULTS");

		//WIFI
		ESP_LOGD(TAG_STORAGE, "LOADING WIFI");
		while(xTaskNotify(ht_wifiRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
			taskYIELD();
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/wifi/def.cfg", "r");
		fread((wifi_config_t*)pv_config_mom, sizeof(wifi_config_t), 1, f);
		fclose(f);
		vTaskResume(ht_wifiRun);

		//ADC
		ESP_LOGD(TAG_STORAGE, "LOADING ADC");
		while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/adc/def.cfg", "r");
		fread((struct stu_adcConfig*)pv_config_mom, sizeof(struct stu_adcConfig), 1, f);
		fclose(f);
		vTaskResume(ht_adcRun);

		ESP_LOGD(TAG_STORAGE, "LOADING ADC");
		while(xTaskNotify(ht_tcpMes,CMD_ldad,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/adc/def.cfg", "r");
		fread((struct stu_adcConfig*)pv_config_mom, sizeof(struct stu_adcConfig), 1, f);
		fclose(f);
		vTaskResume(ht_tcpMes);

		//TCP MES
		ESP_LOGD(TAG_STORAGE, "LOADING MES");
		while(xTaskNotify(ht_tcpMes,CMD_ldtm,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/tcpmes/def.cfg", "r");
		fread((struct sockaddr_in*)pv_config_mom, sizeof(struct sockaddr_in), 1, f);
		fclose(f);
		vTaskResume(ht_tcpMes);

		//LOADCELL
		ESP_LOGD(TAG_STORAGE, "LOADING LOADCELL");
		while(xTaskNotify(ht_adcRun,CMD_ldlc,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/cell/def.cfg", "r");
		fread((struct stu_cellConfig*)pv_config_mom, sizeof(struct stu_cellConfig), 1, f);
		fclose(f);
		vTaskResume(ht_adcRun);

		//TCP CONF
		ESP_LOGD(TAG_STORAGE, "LOADING CONF");
		while(xTaskNotify(ht_tcpConf,CMD_ldtc,eSetValueWithoutOverwrite) != pdPASS)
					taskYIELD();
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/tcpconf/def.cfg", "r");
		fread((struct sockaddr_in*)pv_config_mom, sizeof(struct sockaddr_in), 1, f);
		fclose(f);
		vTaskResume(ht_tcpConf);



		//BLINK
		ESP_LOGD(TAG_STORAGE, "LOADING BLINK");
		while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
							taskYIELD();
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/blink/def.cfg", "r");
		fread((struct stu_blinkConfig*)pv_config_mom, sizeof(struct stu_blinkConfig), 1, f);
		fclose(f);
		vTaskResume(ht_blinkRun);

		//BATTERY MONITOR
		ESP_LOGD(TAG_STORAGE, "LOADING BATMON");
		while(xTaskNotify(ht_batmonRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
							taskYIELD();
		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		f = fopen(MOUNT_POINT"/config/batmon.def", "r");
		fread((struct stu_batmonConfig*)pv_config_mom, sizeof(struct stu_batmonConfig), 1, f);
		fclose(f);
		vTaskResume(ht_batmonRun);

		//		sprintf(ac_fileAddress, MOUNT_POINT"/config/trigconf/%d.cfg", p_initConfig->trigger);
		//		f = fopen(ac_fileAddress, "r");
		//		fread(p_sockAddr, sizeof(struct sockaddr_in), 1, f);
		//		fclose(f);
		//		xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
		ESP_LOGD(TAG_REDUND, "LOADING FINISHED");

		vTaskResume(ht_configRun);
		break;

	case CMD_svad:
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/adc/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			vTaskResume(ht_configRun);
		}
		else
		{
			int i = 0;
			xQueueSend(q_pointer, &i, portMAX_DELAY);
			vTaskResume(ht_configRun);
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fwrite((struct stu_adcConfig*)pv_config_mom, sizeof(struct stu_adcConfig), 1, f);
			fclose(f);
			vTaskResume(ht_blinkRun);
			vTaskResume(ht_configRun);
		}
		break;
	case CMD_svtc:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/tcpconf/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_tcpConf,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fwrite((struct sockaddr_in*)pv_config_mom, sizeof(struct sockaddr_in), 1, f);
			fclose(f);
			sprintf(pc_response, "|svtc|%s|\t\tConfig saved\n", ac_fileAddress);
			vTaskResume(ht_tcpConf);
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		vTaskResume(ht_configRun);
		vTaskSuspend(NULL);
		free(pc_response);
		break;
	case CMD_svtm:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/tcpmes/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_tcpMes,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fwrite((struct sockaddr_in*)pv_config_mom, sizeof(struct sockaddr_in), 1, f);
			fclose(f);
			sprintf(pc_response, "|svtm|%s|\t\tConfig saved.\n", ac_fileAddress);
			vTaskResume(ht_tcpMes);
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		vTaskResume(ht_configRun);
		vTaskSuspend(NULL);
		free(pc_response);
		break;
	case CMD_svbl:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/blink/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "r");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_blinkRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread((struct stu_blinkConfig*)pv_config_mom, sizeof(struct stu_blinkConfig), 1, f);
			fclose(f);
			sprintf(pc_response, "|svbl|%s|\t\tConfig saved.\n", ac_fileAddress);
			vTaskResume(ht_blinkRun);
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		vTaskResume(ht_configRun);
		vTaskSuspend(NULL);
		free(pc_response);
		break;
	case CMD_svlc:
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/cell/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			vTaskResume(ht_configRun);
		}
		else
		{
			int i = 0;
			xQueueSend(q_pointer, &i, portMAX_DELAY);
			vTaskResume(ht_configRun);
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fwrite((struct stu_cellConfig*)pv_config_mom, sizeof(struct stu_cellConfig), 1, f);
			fclose(f);
			vTaskResume(ht_blinkRun);
			vTaskResume(ht_configRun);
		}
		break;
	case CMD_svwi:
		pc_response = malloc(128);
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/wifi/%s.cfg", pc_fileName);
		f = fopen(ac_fileAddress, "w");
		if (f == 0)
		{
			int i_errnum = errno;
			xQueueSend(q_pointer, &i_errnum, portMAX_DELAY);
			ESP_LOGD(TAG_STORAGE, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
			sprintf(pc_response, "COULD NOT OPEN FILE:%s ERROR:%s",ac_fileAddress, strerror(i_errnum));
		}
		else
		{
			while(xTaskNotify(ht_wifiRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fwrite((wifi_config_t*)pv_config_mom, sizeof(wifi_config_t), 1, f);
			fclose(f);
			sprintf(pc_response, "|svwi|%s|\t\tConfig saved.\n", ac_fileAddress);
			xEventGroupSync( eg_sync, BIT_STORAGE_FIN, BIT_STORAGE_FIN | BIT_CONFIG_FIN | BIT_RECEIVER_FIN, portMAX_DELAY );
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		xEventGroupSync( eg_sync, BIT_STORAGE_FIN, BIT_STORAGE_FIN | BIT_CONFIG_FIN | BIT_COMM_FIN, portMAX_DELAY );
		free(pc_response);
		break;
	case CMD_defs:

	case CMD_inad:
		pc_response = malloc(128);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/def.ini", "r+");
		if(f == 0)
		{
			sprintf(pc_response, "|inad|0|\t\tCould not find ini file.\n");
		}
		else
		{
			while(xTaskNotify(ht_adcRun,ui_cmdlet,eSetValueWithoutOverwrite) != pdPASS)
				taskYIELD();
			xQueueReceive(q_pointer, &pv_config_mom, portMAX_DELAY);
			fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
			p_initConfig->adc = *(struct stu_adcConfig*)pv_config_mom;
			xEventGroupSync( eg_sync, BIT_STORAGE_FIN, BIT_STORAGE_FIN | BIT_RECEIVER_FIN, portMAX_DELAY );
			fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
			fclose(f);
			free(p_initConfig);
			sprintf(pc_response, "|inad|0|\t\tADC config written to ini.\n");
		}
		xQueueSend(q_pointer, &pc_response, portMAX_DELAY);
		xEventGroupSync( eg_sync, BIT_STORAGE_FIN, BIT_STORAGE_FIN | BIT_COMM_FIN | BIT_CONFIG_FIN, portMAX_DELAY );
		free(pc_response);
		break;
	case CMD_intc:
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		strcpy(p_initConfig->tcpConf,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;
	case CMD_intm:
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		strcpy(p_initConfig->tcpMes,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;
	case CMD_inbl:
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		strcpy(p_initConfig->blink,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;
	case CMD_intr:
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		strcpy(p_initConfig->trigger,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;
	case CMD_inwi:
		xQueueReceive(q_pointer, &pc_fileName, portMAX_DELAY);
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		strcpy(p_initConfig->wifi,pc_fileName);
		f = fopen(MOUNT_POINT"/config/init", "w");
		fwrite(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		free(p_initConfig);
		break;

	default:
		break;
	vTaskDelay(10/portTICK_PERIOD_MS);
	}
	free(ac_fileAddress);
}


esp_err_t fFormatSD()
{
	const size_t workbuf_size = 4096;
	void* workbuf = NULL;
	char drv[3] = {(char)('0' + pdrv), ':', 0};
	esp_err_t err = ESP_OK;
	FILE* f;

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
		ESP_LOGD(TAG_REDUND, "INITALISING");
		mkdir(MOUNT_POINT"/redund", 0777);
		mkdir(MOUNT_POINT"/config", 0777);
		mkdir(MOUNT_POINT"/config/adc", 0777);
		mkdir(MOUNT_POINT"/config/cell", 0777);
		mkdir(MOUNT_POINT"/config/wifi", 0777);
		mkdir(MOUNT_POINT"/config/blink", 0777);
		mkdir(MOUNT_POINT"/config/tcpmes", 0777);
		mkdir(MOUNT_POINT"/config/tcpconf", 0777);
		mkdir(MOUNT_POINT"/config/trig", 0777);

		ESP_LOGD(TAG_REDUND, "WRITING INIT CONFIG");
		f = fopen(MOUNT_POINT"/config/init.def", "w");
		struct stu_initConfig initConfig =
		{
				.adc =
				{
						.b_sendTCP		= 1,
						.b_sendSD		= 0,
						.ul_adcPeriod	= 1000000,
						.uc_numDecimals	= 2
				},
				.cell =
				{
						.ac_name 		= "default",
						.d_calValue		= 1.0,
						.ui_tareValue	= 0,
						.ul_cellID		= 0
				},
				.batMon =
				{
						.i_batRawLow	= 0,
						.f_batVolLow	= 0,
						.i_batRawHigh	= 1023,
						.f_batVolHigh	= 3.3,
				},
				.blink =
				{
						.ui_blinkPeriod		= 10000,
						.ui_blinkDuration	= 100,
						.ui_blinkBrightness	= 200,
						.ui_blinkFrequency	= 50000,
						.b_blinkEnabled		= 1
				},
				.trigger =
				{
						.b_pinMode = 1,
						.ui_timeout	= 60000,
						.trigSocket =
						{
								.sin_family			=	AF_INET,
								.sin_addr.s_addr	=	0,
								.sin_port			=	37904
						}
				},
				.wifi =
				{
						.c_wifiType	= TYPE_AP,
						.ac_apPass = "",
						.wifiConfig
						{
							.ap =
							{
									.ssid	= "espdefaultinit",
									.ssid_len = strlen("espdefaultinit"),
									.password = "espdefaultinit",
									.channel = 9,
									.authmode = WIFI_AUTH_WPA_WPA2_PSK,
									.max_connection = 1
							}
						}
				},
				.tcpMes =
				{
						.sin_family			=	AF_INET,
						.sin_addr.s_addr	=	0,
						.sin_port			=	37392
				},
				.tcpConf =
				{
						.sin_family			=	AF_INET,
						.sin_addr.s_addr	=	0,
						.sin_port			=	37648
				}

		};
		fwrite(&initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);
		ESP_LOGD(TAG_REDUND, "FINISHED INIT");
	}

	return err;
}
