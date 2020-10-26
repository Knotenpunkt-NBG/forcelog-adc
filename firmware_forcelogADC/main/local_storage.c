/*
 * local_storage.c
 *
 *  Created on: Oct 25, 2020
 *      Author: lor
 */

#include "local_storage.h"

void flocalStorageInit(void)
{
	ESP_LOGI(TAG_REDUND, "Initializing SD card");

	ESP_LOGI(TAG_REDUND, "Using SPI peripheral");

	sdmmc_host_t host = SDSPI_HOST_DEFAULT();
	sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
	slot_config.gpio_miso = PIN_NUM_MISO;
	slot_config.gpio_mosi = PIN_NUM_MOSI;
	slot_config.gpio_sck  = PIN_NUM_CLK;
	slot_config.gpio_cs   = PIN_NUM_CS;
	// This initializes the slot without card detect (CD) and write protect (WP) signals.
	// Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.

	// Options for mounting the filesystem.
	// If format_if_mount_failed is set to true, SD card will be partitioned and
	// formatted in case when mounting fails.
	esp_vfs_fat_sdmmc_mount_config_t mount_config = {
			.format_if_mount_failed = false,
			.max_files = 5,
			.allocation_unit_size = 16 * 1024
	};

	// Use settings defined above to initialize SD card and mount FAT filesystem.
	// Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
	// Please check its source code and implement error recovery when developing
	// production applications.
	sdmmc_card_t* card;
	esp_err_t ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);

	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			ESP_LOGE(TAG_REDUND, "Failed to mount filesystem. "
					"If you want the card to be formatted, set format_if_mount_failed = true.");
		} else {
			ESP_LOGE(TAG_REDUND, "Failed to initialize the card (%s). "
					"Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
		}
		return;
	}
	mkdir(MOUNT_POINT"/redund", 0777);
	remove(MOUNT_POINT"/foo.txt");
	// Card has been initialized, print its properties
	//sdmmc_card_print_info(stdout, card);
	xTaskCreate(tlocalStorageRun,		"tlocalStorageRun",		2048, ht_tlocalStorageRun, 10, NULL);
}

void tlocalStorageRun (void*param)
{
	while(1)
	{
		// Use POSIX and C standard library functions to work with files.
		// First create a file.
		ESP_LOGI(TAG_REDUND, "Opening file");

		FILE* f = fopen(MOUNT_POINT"/redund/0x00.bak", "a");
		if (f == NULL) {
			ESP_LOGE(TAG_REDUND, "Failed to open file for writing");
			return;
		}
		//fprintf(f, "Hello %s!\n", card->cid.name);
		fprintf(f, "HELLO WORLD!");
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
		char line[64];
		fgets(line, sizeof(line), f);
		fclose(f);
		// strip newline
		char* pos = strchr(line, '\n');
		if (pos) {
			*pos = '\0';
		}
		ESP_LOGI(TAG_REDUND, "Read from file: '%s'", line);

		// All done, unmount partition and disable SDMMC or SPI peripheral
		//esp_vfs_fat_sdmmc_unmount();
		//ESP_LOGI(TAG_REDUND, "Card unmounted");
		break;

	}
	vTaskDelete(NULL);
}
