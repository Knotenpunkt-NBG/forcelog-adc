# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS "components/adc_modules/")
#set(EXTRA_COMPONENT_DIRS "components/adc_modules/simulation")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

#set(EXTRA_COMPONENT_DIRS "../components/adc_modules/hx711-a")
#list(APPEND COMPONENT_ADD_INCLUDEDIRS "../components/adc_modules/hx711-a")

project(forcelog-adc)