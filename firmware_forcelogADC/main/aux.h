/*
 * aux.h
 *
 *  Created on: Apr 4, 2021
 *      Author: lor
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#include "defines.h"
#include "globals.h"

void		fsendResponse			(unsigned char	uc_flagAdditional,
									unsigned char	uc_flagFinal,
									char*			pc_response);

void		fsetLogLevel			(int i);

char*		fgetValuePointer		(char* pc_value,
									char* pc_response);

uint32_t	fgetNextString			(char* string);

uint64_t	fconvertToUs			(char* pc_value);

uint32_t	fconvertToMs			(char* pc_value);

int		fcharReplace	(char* pc_string, char c_find, char c_replace);

int ipow(int base, int exponent);

#endif /* MAIN_AUX_H_ */
