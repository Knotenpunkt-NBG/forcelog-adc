/*
 * trigger.h
 *
 *  Created on: Oct 16, 2020
 *      Author: lor
 */

#ifndef MAIN_TRIGGER_H_
#define MAIN_TRIGGER_H_

#include <lwip/sockets.h>

#include "defines.h"
#include "globals.h"


void t_udpWait (void* param);
void t_pinWait(void* param);

int fcheckTrigger(int sock, int *i_cmdlet);


#endif /* MAIN_TRIGGER_H_ */
