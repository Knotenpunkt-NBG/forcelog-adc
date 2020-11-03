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
	xTaskCreate(tstorageRun,		"tlocalStorageRun",		4096, NULL, 10, &ht_storageRun);

	// Card has been initialized, print its properties

}

void tstorageRun (void*param)
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
			vTaskDelay(1);
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
	struct stu_adcConfig*	p_adcConfig			=	NULL;
	struct stu_cellConfig*	p_cellConfig		=	NULL;
	wifi_config_t*			p_wifiConfig		=	NULL;
	struct sockaddr_in*		p_sockAddr			=	NULL;
	struct stu_initConfig*	p_initConfig		=	NULL;
	struct stu_blinkConfig*	p_blinkConfig		=	NULL;
	struct stu_batmonConfig*	p_batmonConfig	=	NULL;

	char ac_fileAddress[35] = {};
	FILE* f;
	switch(ui_cmdlet)
	{
	case CMD_mkfs:
		fFormatSD();
		break;

	case CMD_SAVE_BATMON:
		//SAVING BATMON CONFIG
		xQueuePeek(q_pointer, &p_batmonConfig, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/batmon/0.cfg");
		f = fopen(ac_fileAddress, "w");
		fwrite(p_batmonConfig, sizeof(struct stu_batmonConfig), 1, f);
		fclose(f);
		xQueueReceive(q_pointer, &p_batmonConfig, portMAX_DELAY);
		xSemaphoreGive(hs_pointerQueue);
		break;

	case CMD_load:
		ESP_LOGD(TAG_REDUND, "LOADING");
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init.cfg", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);

		//LOADING WIFI CONFIG
		ESP_LOGD(TAG_REDUND, "LOADING WIFI");
		while(xTaskNotify(ht_wifiRun,CMD_load,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/wifi/%d.cfg", p_initConfig->wifi);
		p_wifiConfig = malloc(sizeof(wifi_config_t));
		f = fopen(ac_fileAddress, "r");
		fread(p_wifiConfig, sizeof(wifi_config_t), 1, f);
		fclose(f);
		xQueueSend(q_pointer,&p_wifiConfig, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));

		//LOADING ADC CONFIG
		ESP_LOGD(TAG_REDUND, "LOADING ADC");
		while(xTaskNotify(ht_adcRun,CMD_load,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/adc/%d.cfg", p_initConfig->adc);
		p_adcConfig = malloc(sizeof(struct stu_adcConfig));
		f = fopen(ac_fileAddress, "r");
		fread(p_adcConfig, sizeof(struct stu_adcConfig), 1, f);
		fclose(f);
		xQueueSend(q_pointer,&p_adcConfig, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));

		ESP_LOGD(TAG_REDUND, "LOADING ADC TCP");
		while(xTaskNotify(ht_tcpMes,CMD_ldad,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		p_adcConfig = malloc(sizeof(struct stu_adcConfig));
		f = fopen(ac_fileAddress, "r");
		fread(p_adcConfig, sizeof(struct stu_adcConfig), 1, f);
		fclose(f);
		xQueueSend(q_pointer,&p_adcConfig, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));

		// LOADING TCP MES CONFIG
		ESP_LOGD(TAG_REDUND, "LOADING TCP MES");
		while(xTaskNotify(ht_tcpMes,CMD_ldtm,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/tcpmes/%d.cfg", p_initConfig->tcpMes);
		p_sockAddr = malloc(sizeof(struct sockaddr_in));
		f = fopen(ac_fileAddress, "r");
		fread(p_sockAddr, sizeof(struct sockaddr_in), 1, f);
		fclose(f);
		xQueueSend(q_pointer,&p_sockAddr, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));

		//LOADING TCP CONF CONFIG
		ESP_LOGD(TAG_REDUND, "LOADING TCP CONF");
		while(xTaskNotify(ht_tcpConf,CMD_ldtc,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/tcpconf/%d.cfg", p_initConfig->tcpConf);
		p_sockAddr = malloc(sizeof(struct sockaddr_in));
		f = fopen(ac_fileAddress, "r");
		fread(p_sockAddr, sizeof(struct sockaddr_in), 1, f);
		fclose(f);
		xQueueSend(q_pointer,&p_sockAddr, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));

		//		//LOADING TRIGGER CONFIG
		//		while(xTaskNotify(ht_tcpConf,CMD_load,eSetValueWithoutOverwrite) != pdPASS)
		//			vTaskDelay(1/ portTICK_PERIOD_MS);
		//		sprintf(ac_fileAddress, MOUNT_POINT"/config/trigconf/%d.cfg", p_initConfig->trigger);
		//		p_sockAddr = malloc(sizeof(struct sockaddr_in));
		//		f = fopen(ac_fileAddress, "r");
		//		fread(p_sockAddr, sizeof(struct sockaddr_in), 1, f);
		//		fclose(f);
		//		xQueueSend(q_pointer,&p_sockAddr, portMAX_DELAY);
		//		while(uxQueueMessagesWaiting(q_pointer));

		//LOADING CELL CONFIG
		ESP_LOGD(TAG_REDUND, "LOADING CELL");
		while(xTaskNotify(ht_adcRun,CMD_ldlc,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		p_cellConfig = malloc(sizeof(struct stu_cellConfig));
		f = fopen(MOUNT_POINT"/config/cells/0.cfg", "r");
		fread(p_cellConfig, sizeof(struct stu_cellConfig), 1, f);
		fclose(f);
		xQueueSend(q_pointer,&p_cellConfig, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));

		//LOADING BLINK CONFIG
		ESP_LOGD(TAG_REDUND, "LOADING BLINK");
		while(xTaskNotify(ht_blinkRun,CMD_load,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		p_blinkConfig = malloc(sizeof(struct stu_blinkConfig));
		f = fopen(MOUNT_POINT"/config/blink/0.cfg", "r");
		fread(p_blinkConfig, sizeof(struct stu_blinkConfig), 1, f);
		fclose(f);
		xQueueSend(q_pointer,&p_blinkConfig, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));

		//LOADING BATMON CONFIG
		ESP_LOGD(TAG_REDUND, "LOADING BATMON");
		while(xTaskNotify(ht_batmonRun,CMD_load,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		p_batmonConfig = malloc(sizeof(struct stu_batmonConfig));
		f = fopen(MOUNT_POINT"/config/blink/0.cfg", "r");
		fread(p_batmonConfig, sizeof(struct stu_batmonConfig), 1, f);
		fclose(f);
		xQueueSend(q_pointer,&p_batmonConfig, portMAX_DELAY);
		while(uxQueueMessagesWaiting(q_pointer));

		free(p_initConfig);
		xSemaphoreGive(hs_pointerQueue);
		ESP_LOGD(TAG_REDUND, "LOAD FINISHED");
		break;

	case CMD_save:
		ESP_LOGD(TAG_REDUND, "SAVING");
		p_initConfig	=	malloc(sizeof(struct stu_initConfig));
		f = fopen(MOUNT_POINT"/config/init.cfg", "r");
		fread(p_initConfig, sizeof(struct stu_initConfig), 1, f);
		fclose(f);

		//SAVING WIFI CONFIG
		ESP_LOGD(TAG_REDUND, "SAVING WIFI");
		while(xTaskNotify(ht_wifiRun,CMD_save,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xQueuePeek(q_pointer, &p_wifiConfig, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/wifi/%d.cfg", p_initConfig->wifi);
		f = fopen(ac_fileAddress, "w");
		fwrite(p_wifiConfig, sizeof(wifi_config_t), 1, f);
		fclose(f);
		xQueueReceive(q_pointer, &p_wifiConfig, portMAX_DELAY);

		//SAVING ADC CONFIG
		ESP_LOGD(TAG_REDUND, "SAVING ADC");
		while(xTaskNotify(ht_adcRun,CMD_svad,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xQueuePeek(q_pointer, &p_adcConfig, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/adc/%d.cfg", p_initConfig->adc);
		xQueueReceive(q_pointer, &p_adcConfig, portMAX_DELAY);

		struct stu_adcConfig* p_adcConfig_temp = malloc(sizeof(struct stu_adcConfig));
		while(xTaskNotify(ht_tcpMes,CMD_svad,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xQueuePeek(q_pointer, &p_adcConfig_temp, portMAX_DELAY);
		p_adcConfig->uc_numDecimals = p_adcConfig_temp->uc_numDecimals;
		f = fopen(ac_fileAddress, "w");
		fwrite(p_adcConfig, sizeof(struct stu_adcConfig), 1, f);
		fclose(f);
		xQueueReceive(q_pointer, &p_adcConfig_temp, portMAX_DELAY);

		//SAVING TCP MES CONFIG
		ESP_LOGD(TAG_REDUND, "SAVING MES");
		while(xTaskNotify(ht_tcpMes,CMD_svtm,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xQueuePeek(q_pointer, &p_sockAddr, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/tcpmes/%d.cfg", p_initConfig->tcpMes);
		f = fopen(ac_fileAddress, "w");
		fwrite(p_sockAddr, sizeof(struct sockaddr_in), 1, f);
		fclose(f);
		xQueueReceive(q_pointer, &p_sockAddr, portMAX_DELAY);

		//SAVING TCP CONF CONFIG
		ESP_LOGD(TAG_REDUND, "SAVING TCP CONF");
		while(xTaskNotify(ht_tcpConf,CMD_svtc,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xQueuePeek(q_pointer, &p_sockAddr, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/tcpconf/%d.cfg", p_initConfig->tcpConf);
		f = fopen(ac_fileAddress, "w");
		fwrite(p_sockAddr, sizeof(struct sockaddr_in), 1, f);
		fclose(f);
		xQueueReceive(q_pointer, &p_sockAddr, portMAX_DELAY);

		//SAVING BLINK CONFIG
		ESP_LOGD(TAG_REDUND, "SAVING BLINK");
		while(xTaskNotify(ht_blinkRun,CMD_svbl,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xQueuePeek(q_pointer, &p_blinkConfig, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/blink/%d.cfg", p_initConfig->tcpConf);
		f = fopen(ac_fileAddress, "w");
		fwrite(p_blinkConfig, sizeof(struct sockaddr_in), 1, f);
		fclose(f);
		xQueueReceive(q_pointer, &p_blinkConfig, portMAX_DELAY);

		//SAVING CELL CONFIG
		ESP_LOGD(TAG_REDUND, "SAVING CELL");
		while(xTaskNotify(ht_adcRun,CMD_svlc,eSetValueWithoutOverwrite) != pdPASS)
			vTaskDelay(1/ portTICK_PERIOD_MS);
		xQueuePeek(q_pointer, &p_cellConfig, portMAX_DELAY);
		sprintf(ac_fileAddress, MOUNT_POINT"/config/cells/%d.cfg", p_initConfig->tcpConf);
		f = fopen(ac_fileAddress, "w");
		fwrite(p_cellConfig, sizeof(struct sockaddr_in), 1, f);
		fclose(f);
		xQueueReceive(q_pointer, &p_cellConfig, portMAX_DELAY);

		free(p_initConfig);
		xSemaphoreGive(hs_pointerQueue);
		ESP_LOGD(TAG_REDUND, "FINISHED SAVING");
		break;
		//		TODO: add granular saving mechanism
	default:
		break;
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
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

		ESP_LOGD(TAG_REDUND, "WRITING INIT CONFIG");
		mkdir(MOUNT_POINT"/config/init", 0777);
		f = fopen(MOUNT_POINT"/config/init.cfg", "w");
		struct stu_initConfig initConfig =
		{
				.adc		=	0,
				.wifi		=	0,
				.blink		=	0,
				.tempi		=	0,
				.tcpMes		=	0,
				.tcpConf	=	0,
				.trigger	=	0
		};
		fwrite(&initConfig, sizeof(struct stu_adcConfig), 1, f);
		fclose(f);

		mkdir(MOUNT_POINT"/config/adc", 0777);
		f = fopen(MOUNT_POINT"/config/adc/0.cfg", "w");
		struct stu_adcConfig adcConfig_default =
		{
				.b_sendSD = false,
				.b_sendTCP = true,
				.uc_numDecimals = 2
		};
		fwrite(&adcConfig_default, sizeof(struct stu_adcConfig), 1, f);
		fclose(f);

		mkdir(MOUNT_POINT"/config/cells", 0777);
		f = fopen(MOUNT_POINT"/config/cells/0.cfg", "w");
		struct stu_cellConfig cellConfig_default =
		{
				.ac_name 		=	"default",
				.ul_cellID		=	0,
				.d_calValue 	= 	1.0,
				.ui_tareValue	=	0
		};
		fwrite(&cellConfig_default, sizeof(struct stu_cellConfig), 1, f);
		fclose(f);

		mkdir(MOUNT_POINT"/config/wifi", 0777);
		f = fopen(MOUNT_POINT"/config/wifi/0.cfg", "w");
		wifi_config_t wifiConfig_default =
		{
				.sta=
				{
						.ssid = "",
						.password = "",
						.threshold.authmode = WIFI_AUTH_WPA2_PSK,
						.pmf_cfg =
						{
								.capable = true,
								.required = false
						}
				}
		};
		fwrite(&wifiConfig_default, sizeof(wifi_config_t), 1, f);
		fclose(f);


		mkdir(MOUNT_POINT"/config/blink", 0777);
		f = fopen(MOUNT_POINT"/config/blink/0.cfg", "w");
		struct stu_blinkConfig blinkConfig_default =
		{
				.ui_blinkPeriod		=	10000,
				.ui_blinkDuration	=	500,
				.ui_blinkBrightness	=	200,
				.ui_blinkFrequency	=	10000,
				.b_blinkEnabled		=	1,
		};
		blinkConfig_default.ui_blinkBrightness = 1000;
		fwrite(&blinkConfig_default, sizeof(struct stu_blinkConfig), 1, f);
		fclose(f);

		mkdir(MOUNT_POINT"/config/tcpmes", 0777);
		f = fopen(MOUNT_POINT"/config/tcpmes/0.cfg", "w");
		struct sockaddr_in tcpMesAddr =
		{
				.sin_family			=	AF_INET,
				.sin_addr.s_addr	=	0,
				.sin_port			=	37392

		};
		fwrite(&tcpMesAddr, sizeof(struct sockaddr_in), 1, f);
		fclose(f);

		mkdir(MOUNT_POINT"/config/tcpconf", 0777);
		f = fopen(MOUNT_POINT"/config/tcpconf/0.cfg", "w");
		struct sockaddr_in tcpConfAddr =
		{
				.sin_family			=	AF_INET,
				.sin_addr.s_addr	=	0,
				.sin_port			=	37648

		};
		fwrite(&tcpConfAddr, sizeof(struct sockaddr_in), 1, f);
		fclose(f);

		mkdir(MOUNT_POINT"/config/trig", 0777);
		f = fopen(MOUNT_POINT"/config/trig/0.cfg", "w");
		struct sockaddr_in trigConfAddr =
		{
				.sin_family			=	AF_INET,
				.sin_addr.s_addr	=	0,
				.sin_port			=	37904

		};
		fwrite(&trigConfAddr, sizeof(struct sockaddr_in), 1, f);
		fclose(f);

		mkdir(MOUNT_POINT"/config/batmon", 0777);
		f = fopen(MOUNT_POINT"/config/batmon/0.cfg", "w");
		struct stu_batmonConfig batmonConfig =
		{
				.f_batVolHigh = 4.2,
				.f_batVolLow = 0,
				.i_batRawHigh = 1023,
				.i_batRawLow = 0,
		};
		fwrite(&batmonConfig, sizeof(struct sockaddr_in), 1, f);
		fclose(f);

		ESP_LOGD(TAG_REDUND, "FINISHED INIT");
	}

	return err;
}
