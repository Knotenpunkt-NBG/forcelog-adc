#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define DEFINETEST "DEFINETEST A"
#define GPIO_HX711_DT_PIN 19
#define GPIO_INPUT_ADC ((1ULL<<GPIO_HX711_DT_PIN))
#define GPIO_HX711_SCK_PIN 23
#define GPIO_HX711_RATE_PIN 22
#define GPIO_OUTPUT_ADC ((1ULL<<GPIO_HX711_SCK_PIN) | (1ULL<<GPIO_HX711_RATE_PIN))


uint32_t	freadAdc	(void);
