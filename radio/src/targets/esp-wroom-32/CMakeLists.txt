cmake_minimum_required(VERSION 3.5)
project(OpenTx C)

add_executable(${CMAKE_PROJECT_NAME}.elf main.c)

# Provides idf_import_components and idf_link_components
include($ENV{IDF_PATH}/tools/cmake/idf_functions.cmake)

# Do some configuration for idf_import_components. This enables creation of artifacts (which might not be
# needed) for some projects
set(IDF_BUILD_ARTIFACTS ON)
set(IDF_PROJECT_EXECUTABLE ${CMAKE_PROJECT_NAME}.elf)
set(IDF_BUILD_ARTIFACTS_DIR ${CMAKE_BINARY_DIR})

# Wraps add_subdirectory() to create library targets for components, and then `return` them using the given variable.
# In this case the variable is named `component`
idf_import_components(components $ENV{IDF_PATH} esp-idf)

# Wraps target_link_libraries() to link processed components by idf_import_components to target
idf_link_components(${CMAKE_PROJECT_NAME}.elf "${components}")
set(CPU_FAMILY ESP32)
option(PWRMANAGE "Pwr Manage" OFF)
option(SDCARD "SD card support" OFF)
option(ROTENC_DIV2 "Rotary encoder resolution/2" OFF)
set(ARCH ESP32)
set(MCU ESP32)
set(LUA NO)
string(TOLOWER ${PCB} FLAVOUR)
set(EEPROM EEPROM_RLC)
add_definitions(-DEEPROM_VARIANT=0 -DAUDIO -DLCD_SPI)
set(GUI_DIR 128x64)
set(TARGET_DIR esp-wroom-32)
set(FIRMWARE_DEPENDENCIES ${FIRMWARE_DEPENDENCIES} 9x_bitmaps)
add_definitions(-DPCB${PCB} -DCPUESP32 -DROTARY_ENCODERS=1)
set(TARGET_SRC ${TARGET_SRC} board.cpp)
set(GUI_SRC
  ${GUI_SRC}
  model_inputs_mixes.cpp
  radio_diagkeys.cpp
  radio_diaganas.cpp
  )
set(FIRMWARE_SRC ${FIRMWARE_SRC} targets/9x/lcd_driver.cpp)
if(PWRMANAGE)
  add_definitions(-DPWRMANAGE)
endif()
