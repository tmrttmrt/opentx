set(SRC
  ${SRC}
  main_esp32.cpp
)
include_directories(targets/common/esp32)
include_directories($ENV{IDF_PATH}/components/esp32/include $ENV{IDF_PATH}/components/soc/include)
if(AUDIO)
  add_definitions(-DAUDIO)
  set(SRC ${SRC} audio_avr.cpp)
  set(BUZZER OFF)
else()
  set(BUZZER ON)
endif()