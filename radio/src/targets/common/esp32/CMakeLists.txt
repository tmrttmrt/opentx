set(SRC
  ${SRC}
  main_esp32.cpp
)
if(AUDIO)
  add_definitions(-DAUDIO)
  set(SRC ${SRC} audio_avr.cpp)
  set(BUZZER OFF)
else()
  set(BUZZER ON)
endif()