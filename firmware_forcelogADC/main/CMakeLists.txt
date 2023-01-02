idf_component_register(SRCS "main.c" "adc_task.c" "aux.c" "batmon.c" "blink.c" "com_serial.c" "com_tcp" "config.c" "status_led.c" "storage.c" "temp_int.c" "trigger.c" "wifi.c"
                       INCLUDE_DIRS "."
                       REQUIRES "ds18b20" "esp_timer" "driver" "esp_event" "esp_wifi" "esp_netif" "nvs_flash" "spi_flash" "fatfs" "vfs" "esp_adc")
set_property(TARGET ${COMPONENT_LIB} APPEND PROPERTY LINK_INTERFACE_MULTIPLICITY 4)

# Edit following two lines to set component requirements (see docs)list(APPEND EXTRA_COMPONENT_DIRS "components/hello" "components/world")
#set(COMPONENT_REQUIRES "esp_adc" "esp_timer" "driver" "esp_event" "esp_wifi" "esp_netif" "nvs_flash" "spi_flash" "esp32-owb" "esp32-ds18b20" "fatfs" "vfs" "module_hx711-a")
#set(COMPONENT_PRIV_REQUIRES )


#message(STATUS ADC_MODULE_PATH: ${CONFIG_ADC_PATH})

#set(COMPONENT_ADD_INCLUDEDIRS	"." 
#								${CONFIG_ADC_PATH})
#set(COMPONENT_SRCS	"main.c" 
#					"adc.c" 
#					"aux.c"
#					"batmon.c" 
#					"blink.c" 
#					"com_serial.c" 
#					"com_tcp" 
#					"config.c" 
#					"status_led.c" 
#					"storage.c" 
#					"temp_int.c" 
#					"trigger.c" 
#					"wifi.c" )
#					"${CONFIG_ADC_PATH}/adc_routines.c")

#register_component()
