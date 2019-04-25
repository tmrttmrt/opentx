I wanted to revive an old Futaba FC-16 transmitter, but I did not want to use already obsolete AVR MEGA 2650 so I decided to port OpenTX to ESP32 based boards.

ESP32 is a powerful and cheap processor with a number of analog inputs (max 8 if WiFi radio is used, WROOM based boards have only 6 WiFi compatible ADC pins broken out), but it is a bit short on digital IO pins. Digital ports therefore need to be expanded. A couple MCP23017 I2C 16 IO port ICs was selected. 

The project is still in development. The radio firmware has most of the functionality working. Implemented features are:

- basic functionality (6 analog sticks + Vbatt (when WiFi is off), navigation keys, trims, switches, 2 rotary encoders, model storage in SPI flash)
- ST7565P 128x64 SPI display with LED back light
- audio output (needs additional speaker driver)
- 8 channel PPM output
- Frsky RS232 telemetry input. (some others might work but were not tested)
- sdcard support, sound messages work from downloaded Taranis X9 (2.2V0018) SD card
- import of MEGA2560 exported model files
- WiFi ftp server to download/upload files
- OTA firmware update
- Only English version of translation files (en.h.txt)
- compiles under MinGW 

Compilation

To build radio firmware ESP-IDF environment is needed (only tested on Windows with ESP-IDF 4.0-dev). Due to peculiarities of ESP-IDF (and since I am not familiar with CMake) the compilation is different than for ATMEL and ARM based systems:
```
cd [OpenTX root folder]/firmware/opentx/radio/src/targets/common/esp32/esp-idf
make flash 
```
This will start also menuconfig where proper serial port needs to chosen, other necessary options are (hopefully) all set in sdkconfig.defaults.

OTA firmware upload:
First enable WiFi on the radio. Radio IP will be shown on LCD upon succesfull connection.
```
cd [OpenTX root folder]/firmware/opentx/radio/src/targets/common/esp32/esp-idf
make ota ESP32_IP=[radio IP]
```

Todo list:

- translation to other languages
- integration into Companion through WiFi
- finding if any concurrency issues will arise due to symmetric multiprocessing architecture (task priorities are an initial guess that probably needs more tunning)
- Lua
- additional radio modules support
- ESP32 sources and CMakeLists.txt still include some old code that is unnecessary
- converting build system to CMake
- compilation on linux
