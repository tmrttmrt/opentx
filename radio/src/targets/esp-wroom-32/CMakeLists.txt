set(CPU_FAMILY ESP32)
set(ARCH ESP32)
set(LUA NO)
add_definitions(-DEEPROM_VARIANT=${EEPROM_VARIANT} -DLCD_SPI)
set(SDCARD YES)
set(HAPTIC NO)
set(EEPROM SPIFLASH)
set(GUI_DIR 128x64)
set(NAVIGATION_TYPE 9x)
set(TARGET_DIR esp-wroom-32)
set(FIRMWARE_DEPENDENCIES ${FIRMWARE_DEPENDENCIES} 9x_bitmaps)
set(PPM_LIMITS_SYMETRICAL YES)
set(PXX1 NO)

set(FLAVOUR espWroom32)
set(TARGET_SRC
${TARGET_SRC}
#    coproc_driver.cpp
#    rotenc_driver.cpp
#    rtc_driver.cpp
    )

add_definitions(-DPCBESP_WROOM_32 -DPPM_PIN_SERIAL -DAUDIO_DUAL_BUFFER)
add_definitions(-DAUDIO -DVOICE -DVARIO)
add_definitions(-DEEPROM_VARIANT=0)
add_definitions(-DSOFTWARE_VOLUME)

set(GUI_SRC
  ${GUI_SRC}
  model_input_edit.cpp
  model_mix_edit.cpp
  model_display.cpp
  radio_diagkeys.cpp
  radio_diaganas.cpp
  radio_hardware.cpp
  view_telemetry.cpp
  view_about.cpp
  )

if(GVARS)
  set(GUI_SRC
    ${GUI_SRC}
    model_gvars.cpp
    )
endif()

set(FIRMWARE_TARGET_SRC
  ${FIRMWARE_TARGET_SRC}
  )

set(TARGET_SRC
  ${TARGET_SRC}
  board.cpp
  )

set(SRC
  ${SRC}
  debug.cpp
  )
