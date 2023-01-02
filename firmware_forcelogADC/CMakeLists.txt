# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0115 NEW)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})


# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
#cmake_minimum_required(VERSION 3.5)

#set(EXTRA_COMPONENT_DIRS "components/adc_modules/")
#set(EXTRA_COMPONENT_DIRS "components/adc_modules/simulation")

#include($ENV{IDF_PATH}/tools/cmake/project.cmake)

#set(EXTRA_COMPONENT_DIRS "../components/adc_modules/hx711-a")
#list(APPEND COMPONENT_ADD_INCLUDEDIRS "../components/adc_modules/hx711-a")
#set(REQUIRES "esp_timer" "driver" "esp_event" "esp_wifi" "esp_netif" "nvs_flash" "spi_flash" "esp32-owb" "esp32-ds18b20" "fatfs" "vfs" "module_hx711-a")

#project(forcelog-adc)