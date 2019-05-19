set(IDF_TARGET "esp32")
include_directories(targets/common/esp32) 
include_directories(targets/common/esp32/ota_server/include)
include_directories(targets/common/esp32/drivers)
set(TIMERS 3 CACHE STRING "Timers count (2 or 3)")
set_property(CACHE TIMERS PROPERTY STRINGS 2 3)
option(CLI "Command Line Interface" OFF)
option(DEBUG "Debug mode" OFF)
option(LOG_TELEMETRY "Telemetry Logs on SD card" OFF)
option(TRACE_SD_CARD "Traces SD enabled" OFF)
option(TRACE_FATFS "Traces FatFS enabled" OFF)
option(TRACE_AUDIO "Traces audio enabled" OFF)
option(DEBUG_TRACE_BUFFER "Debug Trace Screen" OFF)
option(MULTIMODULE "DIY Multiprotocol TX Module (https://github.com/pascallanger/DIY-Multiprotocol-TX-Module)" OFF)
option(CROSSFIRE "Crossfire TX Module" OFF)
option(SUPPORT_D16_EU_ONLY "XJT module only supports D16-EU and LR12-EU" OFF) # TODO rename to XJT_EU_ONLY
option(DEBUG_INTERRUPTS "Count interrupts" OFF)
option(DEBUG_USB_INTERRUPTS "Count individual USB interrupts" OFF)
option(DEBUG_TASKS "Task switching statistics" OFF)
option(DEBUG_TIMERS "Time critical parts of the code" OFF)

add_definitions(-DCPUESP32)

set(GUI_SRC ${GUI_SRC}
    radio_hardware_esp32.cpp
)
if(TIMERS EQUAL 3)
  add_definitions(-DTIMERS=3)
else()
  add_definitions(-DTIMERS=2)
endif()

if(LOG_TELEMETRY)
  add_definitions(-DLOG_TELEMETRY)
endif()

if(TRACE_SD_CARD)
  add_definitions(-DTRACE_SD_CARD)
  set(DEBUG ON)
  set(DEBUG_TRACE_BUFFER ON)
endif()

if(TRACE_FATFS)
  add_definitions(-DTRACE_FATFS)
  set(DEBUG ON)
  set(DEBUG_TRACE_BUFFER ON)
endif()

if(TRACE_AUDIO)
  add_definitions(-DTRACE_AUDIO)
  set(DEBUG ON)
  set(DEBUG_TRACE_BUFFER ON)
endif()

if(DEBUG_TRACE_BUFFER)
  add_definitions(-DDEBUG_TRACE_BUFFER)
endif()

if(DEBUG_INTERRUPTS)
  add_definitions(-DDEBUG_INTERRUPTS)
  set(DEBUG ON)
  if(DEBUG_USB_INTERRUPTS)
    add_definitions(-DDEBUG_USB_INTERRUPTS)
  endif()
endif()

if(DEBUG_TASKS)
  add_definitions(-DDEBUG_TASKS)
  set(DEBUG ON)
endif()

if(DEBUG_TIMERS)
  add_definitions(-DDEBUG_TIMERS)
  set(DEBUG ON)
endif()

if(CLI)
  add_definitions(-DCLI)
  set(FIRMWARE_SRC ${FIRMWARE_SRC} cli.cpp)
  set(SRC ${SRC} dump.cpp)
elseif(DEBUG)
  set(SRC ${SRC} dump.cpp)
endif()

if(DEBUG)
  add_definitions(-DDEBUG)
endif()

if(SUPPORT_D16_EU_ONLY)
  add_definitions(-DMODULE_D16_EU_ONLY_SUPPORT)
endif()

if(HAPTIC)
  add_definitions(-DHAPTIC)
  set(SRC ${SRC} haptic.cpp)
  set(TARGET_SRC ${TARGET_SRC} haptic_driver.cpp)
endif()

# Protocols supported

add_definitions(-DDSM2 -DSBUS -DPPM -DEXTERNAL_MODULE_PPM)

if(PXX1)
  add_definitions(-DPXX -DPXX1)
  set(PULSES_SRC
    ${PULSES_SRC}
    pxx1.cpp
    )
endif()

if(PXX2)
  add_definitions(-DPXX -DPXX2)
  set(PULSES_SRC
    ${PULSES_SRC}
    pxx2.cpp
    )
endif()

if(MULTIMODULE)
  add_definitions(-DMULTIMODULE)
  set(SRC ${SRC} pulses/multi.cpp telemetry/spektrum.cpp telemetry/flysky_ibus.cpp telemetry/multi.cpp)
endif()

if(CROSSFIRE)
  add_definitions(-DCROSSFIRE)
  set(PULSES_SRC
    ${PULSES_SRC}
    crossfire.cpp
    )
  set(SRC
    ${SRC}
    telemetry/crossfire.cpp
    )
endif()

add_definitions(-DTELEMETRY_FRSKY -DTELEMETRY_FRSKY_SPORT -DGPS)

add_definitions(-DBOLD_FONT -DBATTGRAPH -DTHRTRACE)
add_definitions(-DREQUIRED_SDCARD_VERSION="${SDCARD_VERSION}")
foreach(LANGUAGE ${TTS_LANGUAGES})
  string(TOLOWER ${LANGUAGE} lang_lower)
  set(SRC ${SRC} translations/tts_${lang_lower}.cpp)
endforeach()

set(SRC
  ${SRC}
  main.cpp
  tasks.cpp
  gui/common/arm/widgets.cpp
  telemetry/telemetry.cpp
  telemetry/telemetry_sensors.cpp
  telemetry/frsky.cpp
  telemetry/frsky_d.cpp
  telemetry/frsky_sport.cpp
  telemetry/frsky_pxx2.cpp
  crc16.cpp
  vario.cpp
  targets/common/esp32/main/startup.cpp
  targets/common/esp32/drivers/ana_driver.cpp
  targets/common/esp32/drivers/audio_driver.cpp
  targets/common/esp32/drivers/audio_esp32.cpp
  targets/common/esp32/drivers/eeprom_conversions.cpp.dis
  targets/common/esp32/drivers/eeprom_driver.cpp
  targets/common/esp32/drivers/fs_driver.cpp
  targets/common/esp32/drivers/fs_wrappers.cpp
  targets/common/esp32/drivers/keys_driver.cpp
  targets/common/esp32/drivers/lcd_driver.cpp
  targets/common/esp32/drivers/pulses_esp32.cpp
  targets/common/esp32/drivers/sdcard.cpp
  targets/common/esp32/drivers/telemetry_driver.cpp
  targets/common/esp32/ftp-server/ftp.c         
  targets/common/esp32/ftp-server/WiFi.c
  targets/common/esp32/ftp-server/initWiFi.cpp  
  targets/common/esp32/ota_server/ota_server.c  
  )

set(PULSES_SRC
  ${PULSES_SRC}
  pulses.cpp
  dsm2.cpp
  sbus.cpp
  )

