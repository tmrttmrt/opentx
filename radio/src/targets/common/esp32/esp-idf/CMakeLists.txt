cmake_minimum_required(VERSION 3.5)
project(opentx_idf_lib)
cmake_policy(SET CMP0057 NEW)
include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
idf_build_process(esp32
                    # try and trim the build; additional components
                    # will be included as needed based on dependency tree
                    #
                    # although esptool_py does not generate static library,
                    # processing the component is needed for flashing related
                    # targets and file generation
                    SDKCONFIG_DEFAULTS ${SDKCONFIG_DEFAULTS}
                    COMPONENTS esp32 freertos spi_flash fatfs spiffs bootloader esptool_py
                    SDKCONFIG ${CMAKE_BINARY_DIR}/sdkconfig
                    BUILD_DIR ${CMAKE_BINARY_DIR}
                    PROJECT_DIR ${IDF_PROJECT_DIR})
add_executable(idfotx EXCLUDE_FROM_ALL stub.c)
add_custom_command(TARGET idfotx 
    POST_BUILD
    COMMAND cmake -E echo "-I$<JOIN:$<TARGET_PROPERTY:idf::esp32,INCLUDE_DIRECTORIES>, -I>" > "${CMAKE_BINARY_DIR}/libidfotx.includes"
    COMMAND cmake -E echo " -I$<JOIN:$<TARGET_PROPERTY:idf::fatfs,INCLUDE_DIRECTORIES>, -I>" >> "${CMAKE_BINARY_DIR}/libidfotx.includes"
    COMMAND cmake -E echo " -I$<JOIN:$<TARGET_PROPERTY:idf::spiffs,INCLUDE_DIRECTORIES>, -I>" >> "${CMAKE_BINARY_DIR}/libidfotx.includes"
    COMMAND cmake -E echo " -I$<JOIN:$<TARGET_PROPERTY:idf::driver,INCLUDE_DIRECTORIES>, -I>" >> "${CMAKE_BINARY_DIR}/libidfotx.includes"
    )
  
target_link_libraries(idfotx INTERFACE idf::esp32 idf::freertos idf::spi_flash idf:fatfs idf:spiffs)
idf_build_executable(idfotx)
add_executable(firmware stub.c)
add_library(libfirmware STATIC IMPORTED)
set_property(TARGET libfirmware PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/libfirmware.a)
target_link_libraries(firmware libfirmware) 
idf_build_executable(firmware)  
