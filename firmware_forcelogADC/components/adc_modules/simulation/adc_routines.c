#include "adc_routines.h"


uint32_t fsimReadADC(void)
{
	uint64_t ul_time = esp_timer_get_time();
	ul_time %= 5000000;
	ul_time /= 5;
	return (uint32_t) ul_time;
}
