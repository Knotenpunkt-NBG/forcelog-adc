/*
 * temperature.h
 *
 *  Created on: Oct 19, 2020
 *      Author: lor
 */

#ifndef MAIN_TEMP_INT_H_
#define MAIN_TEMP_INT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "ds18b20.h"
#include "esp_timer.h"


#include "globals.h"
#include "aux.h"



#define DS18B20_RESOLUTION   	DS18B20_RESOLUTION_12_BIT		//Resolution in bit. results in a conversion time of around one second




void		ftempIntInit				(void);
void		ttempIntRun					(void* param);
void		ttempExtRun					(void* param);

void		fowbConfig					(uint32_t ui_cmdlet);

/**TODO: write function description
 * @brief Compares two rom codes
 *
 * @param[in] handle 1-wire handle with DS18B20 on
 * @param[in] rom_number ROM number to specify which DS18B20 to read from, NULL to skip ROM
 * @param[in] resolution resolution of DS18B20's temperation conversion
 * @return
 *         - ESP_OK                Set DS18B20 resolution success.
 *         - ESP_ERR_INVALID_ARG   Invalid argument.
 *         - ESP_ERR_NOT_FOUND     There is no device present on 1-wire bus.
 */
uint32_t	fowbcmp						(uint8_t* romCodeA, uint8_t* romCodeB);


/**TODO: write function description
 * @brief Scan OneWire Bus for Devices
 *
 * @param[in] handle 1-wire handle with DS18B20 on
 * @param[in] rom_number ROM number to specify which DS18B20 to read from, NULL to skip ROM
 * @param[in] resolution resolution of DS18B20's temperation conversion
 * @return
 *         - ESP_OK                Set DS18B20 resolution success.
 *         - ESP_ERR_INVALID_ARG   Invalid argument.
 *         - ESP_ERR_NOT_FOUND     There is no device present on 1-wire bus.
 */
esp_err_t fscanOwb(onewire_rom_search_context_handler_t contexthandle, uint8_t* p_ROM_INT, uint8_t* p_ROM_EXT, onewire_bus_handle_t onewirehandle);



void		callback_timerTemp			(TimerHandle_t);
void		IRAM_ATTR gpio_isr_handler	(void* arg);

#endif /* MAIN_TEMP_INT_H_ */
