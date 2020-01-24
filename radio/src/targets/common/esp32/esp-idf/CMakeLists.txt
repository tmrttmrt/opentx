cmake_minimum_required(VERSION 3.5)
project(opentx)
#cmake_policy(SET CMP0057 NEW)
include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
if(PCB STREQUAL ESP_WROOM_32)
  set(SDKCONFIG_DEFAULTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../esp-wroom-32/esp-idf/sdkconfig.defaults)  
elseif(PCB STREQUAL ESP_HELTEC_32)
  set(SDKCONFIG_DEFAULTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../esp-heltec-32/esp-idf/sdkconfig.defaults)  
else()
  message(FATAL_ERROR "Unknown PCB '${PCB}'")
endif()

idf_build_process(esp32
                    # try and trim the build; additional components
                    # will be included as needed based on dependency tree
                    #
                    # although esptool_py does not generate static library,
                    # processing the component is needed for flashing related
                    # targets and file generation
                    SDKCONFIG_DEFAULTS ${SDKCONFIG_DEFAULTS}
                    COMPONENTS freertos spi_flash fatfs spiffs bootloader esptool_py
                    SDKCONFIG ${CMAKE_BINARY_DIR}/sdkconfig
                    BUILD_DIR ${CMAKE_BINARY_DIR} 
                    PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(elf_file ${CMAKE_PROJECT_NAME}.elf)
add_executable(${elf_file} main.c)
target_link_libraries(${elf_file} firmware idf::freertos idf::spi_flash idf::fatfs )
idf_build_executable(${elf_file})

add_custom_target(idf_includes cmake -E echo "-I$<JOIN:$<TARGET_PROPERTY:idf::esp32,INCLUDE_DIRECTORIES>, -I>" > "${CMAKE_BINARY_DIR}/idf.includes"
    COMMAND cmake -E echo " -I$<JOIN:$<TARGET_PROPERTY:idf::fatfs,INCLUDE_DIRECTORIES>, -I>" >> "${CMAKE_BINARY_DIR}/idf.includes"
    COMMAND cmake -E echo " -I$<JOIN:$<TARGET_PROPERTY:idf::spiffs,INCLUDE_DIRECTORIES>, -I>" >> "${CMAKE_BINARY_DIR}/idf.includes"
    COMMAND cmake -E echo " -I$<JOIN:$<TARGET_PROPERTY:idf::driver,INCLUDE_DIRECTORIES>, -I>" >> "${CMAKE_BINARY_DIR}/idf.includes"
    DEPENDS ${CMAKE_BINARY_DIR}/config/sdkconfig.h
    COMMENT "Extracting IDF include directories"
)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
ExternalProject_Add(project_firmware
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../..
        BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/firmware
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/firmware
        CMAKE_ARGS cmake -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_TOOLCHAIN_FILE=$ENV{IDF_PATH}/tools/cmake/toolchain-esp32.cmake -DPCB=${PCB} -DIDF_INCLUDES=${CMAKE_BINARY_DIR}/idf.includes
        BUILD_COMMAND mingw32-make firmware
        INSTALL_COMMAND ""
        BUILD_ALWAYS 1
)
add_library(firmware STATIC IMPORTED)
add_dependencies(firmware idf_includes)
add_dependencies(${elf_file} project_firmware idf_includes)    
set_property(TARGET firmware PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/firmware/radio/src/libfirmware.a)  

add_custom_target(ota
        DEPENDS app
        COMMAND echo "OTA to: \$(ESP32_IP):8032" & curl "\$(ESP32_IP):8032" --data-binary @- < opentx.bin
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)   
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
