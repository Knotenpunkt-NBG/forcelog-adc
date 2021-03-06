#include "adc_routines.h"

uint32_t freadAdc ()
{
	uint32_t count = 0;
	gpio_set_level(GPIO_HX711_SCK_PIN, 0);
	while(gpio_get_level(GPIO_HX711_DT_PIN))
	{
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
	for (int i = 0; i<24; i++)
	{
		gpio_set_level(GPIO_HX711_SCK_PIN, 1);
		count = count << 1;
		gpio_set_level(GPIO_HX711_SCK_PIN, 0);
		if (gpio_get_level(GPIO_HX711_DT_PIN) == 1)
		{
			count ++;
		}
	}
	gpio_set_level(GPIO_HX711_SCK_PIN, 1);
	count = count ^ 0x800000;
	gpio_set_level(GPIO_HX711_SCK_PIN, 0);
//	ESP_LOGV(TAG_ADC, "RAW ADC:%d\n", count);
	return count;
}
