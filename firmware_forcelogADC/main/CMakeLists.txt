# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "main.c" "adc.c" "batmon.c" "blink.c" "com_serial.c" "com_tcp" "config.c" "status_led.c" "storage.c" "temp_int.c" "trigger.c" "wifi.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
