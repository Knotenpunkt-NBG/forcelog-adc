/*
 * aux.c
 *
 *  Created on: Apr 4, 2021
 *      Author: lor
 */

#include "aux.h"


void	fsendResponse	(unsigned char	uc_flagAdditional,
						unsigned char	uc_flagFinal,
						char*			pc_response)
{
	stu_configMessage stu_response = {
			.uc_numResponses = uc_flagAdditional,
			.pc_response = pc_response
	};
	xQueueSend(q_pconfigOut,&stu_response, portMAX_DELAY);
	if(uc_flagFinal)
	{
		stu_response.pc_response = 0;
		stu_response.uc_numResponses = 0;
		xQueueSend(q_pconfigOut,&stu_response, portMAX_DELAY);
	}
}

void fsetLogLevel (int i)
{
	esp_log_level_set(TAG_ADC, i);
	esp_log_level_set(TAG_TCP, i);
	esp_log_level_set(TAG_UDP, i);
	esp_log_level_set(TAG_UART, i);
	esp_log_level_set(TAG_BLINK, i);
	esp_log_level_set(TAG_WIFI, i);
	esp_log_level_set(TAG_CONF, i);
	esp_log_level_set(TAG_BATMON, i);
	esp_log_level_set(TAG_TEMPINT, i);
	esp_log_level_set(TAG_REDUND, i);
	esp_log_level_set(TAG_STORAGE, i);
}

uint32_t fgetNextString	(char* string)
{
	uint32_t i;
	for (i = 0; i < strlen(string); i++)
	{
		if (string[i] == ':')
		{
			return i+1;
		}
	}
	return 0;
}


char* fgetValuePointer	(char* pc_value,
						char* pc_response)
{
	if (fgetNextString(pc_value))
	{
		free(pc_response);
		return pc_value + fgetNextString(pc_value);
	}
	else
	{
		fsendResponse(1, 0, pc_response);
		xQueueReceive(q_pconfigIn, &pc_value, portMAX_DELAY);
	}
	return pc_value;
}

uint64_t fconvertToUs (char* pc_value)
{
	uint64_t ul_value = 0;
	char* pc_unit = 0;
	fcharReplace(pc_value, ',', '.');
	if (strchr(pc_value, '.'))
	{
		double d_value = strtod(pc_value, &pc_unit);
		if (d_value)
		{
			if (*pc_unit != '\0')
			{
				if (!strcmp(pc_unit, "ms"))
				{
					ul_value = d_value * 1000;
				}
				else if (!strcmp(pc_unit, "us"))
				{
					ul_value = d_value;
				}
				else if (!strcmp(pc_unit, "s"))
				{
					ul_value = d_value * 1000000;
				}
				else if (!strcmp(pc_unit, "hz"))
				{
					ul_value = 1000000 / d_value;
				}
			}
			else
			{
				ul_value = d_value;
			}
		}
	}
	else
	{
		ul_value = strtoll(pc_value, &pc_unit, 10);
		if (ul_value)
		{
			if (*pc_unit != '\0')
			{
				if (!strcmp(pc_unit, "ms"))
				{
					ul_value *= 1000;
				}
				else if (!strcmp(pc_unit, "us"))
				{
				}
				else if (!strcmp(pc_unit, "s"))
				{
					ul_value *= 1000000;
				}
				else if (!strcmp(pc_unit, "hz"))
				{
					ul_value = 1000000 / ul_value;
				}
			}
		}
	}
	return ul_value;
}

uint32_t	fconvertToMs	(char* pc_value)
{
	uint32_t ui_value = 0;
	char* pc_unit = 0;
	fcharReplace(pc_value, ',', '.');
	if (strchr(pc_value, '.'))
	{
		double d_value = strtod(pc_value, &pc_unit);
		if (d_value)
		{
			if (*pc_unit != '\0')
			{
				if (!strcmp(pc_unit, "ms"))
				{
					ui_value = d_value;
				}
				else if (!strcmp(pc_unit, "us"))
				{
					ui_value = d_value / 1000;
				}
				else if (!strcmp(pc_unit, "s"))
				{
					ui_value = d_value * 1000;
				}
				else if (!strcmp(pc_unit, "hz"))
				{
					ui_value = 1000 / d_value;
				}
			}
			else
			{
				ui_value = d_value;
			}
		}
	}
	else
	{
		ui_value = strtol(pc_value, &pc_unit, 10);
		if (ui_value)
		{
			if (*pc_unit != '\0')
			{
				if (!strcmp(pc_unit, "ms"))
				{
				}
				else if (!strcmp(pc_unit, "us"))
				{
					ui_value *= 1000;
				}
				else if (!strcmp(pc_unit, "s"))
				{
					ui_value *= 1000;
				}
				else if (!strcmp(pc_unit, "hz"))
				{
					ui_value = 1000 / ui_value;
				}
			}
		}
	}
	return ui_value;
}

int		fcharReplace	(char* pc_string, char c_find, char c_replace)
{
	int i_flag = 0;
    char *current_pos = strchr(pc_string,c_find);
    while (current_pos)
    {
    	i_flag = 1;
        *current_pos = c_replace;
        current_pos = strchr(current_pos, c_find);
    }
	return i_flag;
}

int ipow(int base, int exponent)
{
	int result=1;
	for (int i = exponent; i>0; i--)
	{
		result = result * base;
	}
	return result;
}
