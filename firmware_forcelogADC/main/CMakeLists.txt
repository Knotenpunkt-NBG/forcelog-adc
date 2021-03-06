# Edit following two lines to set component requirements (see docs)list(APPEND EXTRA_COMPONENT_DIRS "components/hello" "components/world")
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "main.c" "adc.c" "batmon.c" "blink.c" "com_serial.c" "com_tcp" "config.c" "status_led.c" "storage.c" "temp_int.c" "trigger.c" "wifi.c")

set(COMPONENT_ADD_INCLUDEDIRS ".")

if (${CONFIG_ADC_MODULE_HX711A})
list(APPEND EXTRA_COMPONENT_DIRS "../components/adc_modules/hx711-a")
list(APPEND COMPONENT_ADD_INCLUDEDIRS "../components/adc_modules/hx711-a")
message(STATUS "HX711A REGISTERED")
elseif(${CONFIG_ADC_MODULE_SIMULATION})
list(APPEND EXTRA_COMPONENT_DIRS "../components/adc_modules/simulation")
list(APPEND COMPONENT_ADD_INCLUDEDIRS "../components/adc_modules/simulation")
message(STATUS "SIMULATION REGISTERED")
else()
message(STATUS "NO MODULE REGISTERES")
endif()

register_component()

