cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(CPU_FAMILY ESP32)
option(PWRMANAGE "Pwr Manage" OFF)
option(SDCARD "SD card support" OFF)
option(ROTENC_DIV2 "Rotary encoder resolution/2" OFF)
set(ARCH ESP32)
set(MCU ESP32)
set(LUA NO)
string(TOLOWER ${PCB} FLAVOUR)
set(EEPROM EEPROM_RLC)
add_definitions(-DEEPROM_VARIANT=0 -DAUDIO)
set(GUI_DIR 128x64)
set(TARGET_DIR esp-wroom-32)
set(FIRMWARE_DEPENDENCIES ${FIRMWARE_DEPENDENCIES} 9x_bitmaps)
add_definitions(-DPCB${PCB} -DCPUESP32 -DROTARY_ENCODERS=2)
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
