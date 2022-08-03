# Edit following two lines to set component requirements (see docs)list(APPEND EXTRA_COMPONENT_DIRS "components/hello" "components/world")
#set(COMPONENT_REQUIRES )
#set(COMPONENT_PRIV_REQUIRES )


message(STATUS ADC_MODULE_PATH: ${CONFIG_ADC_PATH})

set(COMPONENT_ADD_INCLUDEDIRS	"." 
								${CONFIG_ADC_PATH})
set(COMPONENT_SRCS	"main.c" 
					"adc.c" 
					"aux.c"
					"batmon.c" 
					"blink.c" 
					"com_serial.c" 
					"com_tcp" 
					"config.c" 
					"status_led.c" 
					"storage.c" 
					"temp_int.c" 
					"trigger.c" 
					"wifi.c" 
					"${CONFIG_ADC_PATH}/adc_routines.c")

register_component()
