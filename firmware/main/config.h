/*
 * config.h
 *
 *  Created on: Oct 3, 2020
 *      Author: lor
 */

#ifndef MAIN_CONFIG_H_
#define MAIN_CONFIG_H_

#include <lwip/sockets.h>

#include "globals.h"
#include "defines.h"


int fConfig (int i_cmdlet, char* str_value);
int fSaveConfig(uint32_t nvs_id);
int fLoadConfig(uint32_t nvs_id);

#endif /* MAIN_CONFIG_H_ */
