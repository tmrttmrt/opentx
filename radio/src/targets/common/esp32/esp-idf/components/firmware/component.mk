COMPONENT_OWNBUILDTARGET := true
COMPONENT_OWNCLEANTARGET := true
RADIO_SRCDIR := $(realpath ../../../../../..)

.PHONY: build

${COMPONENT_BUILD_DIR}/$(COMPONENT_LIBRARY): build

build: ${COMPONENT_BUILD_DIR}/configured 
	echo Building $(COMPONENT_LIBRARY)
	rm -f ${COMPONENT_BUILD_DIR}/$(COMPONENT_LIBRARY)
	cd ${COMPONENT_BUILD_DIR}/build && \
	mingw32-make MAKEFLAGS= firmware &&\
	cp ${COMPONENT_BUILD_DIR}/build/radio/src/libfirmware.a ${COMPONENT_BUILD_DIR}/$(COMPONENT_LIBRARY)
	grep CXX_DEFINES ${COMPONENT_BUILD_DIR}/build/radio/src/CMakeFiles/firmware.dir/flags.make |\
	sed -E 's/CXX_DEFINES =\s*//;s/-I//g' > $(COMPONENT_PATH)/defines.mk
	grep CXX_INCLUDES ${COMPONENT_BUILD_DIR}/build/radio/src/CMakeFiles/firmware.dir/flags.make |\
	sed -E 's/CXX_INCLUDES =\s*//;s_-I(.):_/\l\1_g; s_\\_/_g' > $(COMPONENT_PATH)/includes.mk

clean:
	cd ${COMPONENT_BUILD_DIR}/build && rm -r *
	rm ${COMPONENT_BUILD_DIR}/configured    

${COMPONENT_BUILD_DIR}/configured:
	echo $(RADIO_SRCDIR)
	mkdir -p ${COMPONENT_BUILD_DIR}/build && \
	env -i HOME="${HOME}" /bin/bash -l -c 'cd ${COMPONENT_BUILD_DIR}/build && pwd && cmake -G "MinGW Makefiles" $(RADIO_SRCDIR)/../.. -DPCB=ESP_WROOM_32 -DTRANSLATIONS=EN -DCMAKE_MAKE_PROGRAM=mingw32-make' &&\
	touch ${COMPONENT_BUILD_DIR}/configured