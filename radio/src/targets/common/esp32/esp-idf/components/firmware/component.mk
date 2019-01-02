COMPONENT_OWNBUILDTARGET := true
COMPONENT_OWNCLEANTARGET := true
RADIO_SRCDIR := $(realpath ../../../../../..)
#COMPONENT_ADD_INCLUDEDIRS := $(RADIO_SRCDIR)/ $(RADIO_SRCDIR)/targets/esp-wroom-32 \ 
#$(RADIO_SRCDIR)/gui $(RADIO_SRCDIR)/gui/128x64 build/radio/src
#CPPFLAGS += -DAUDIO -DAUTOSOURCE -DAUTOSWITCH -DCORRECT_NEGATIVE_SHIFTS -DCPUESP32 -DCURVES -DDBLKEYS -DEEPROM -DEEPROM_RLC -DEEPROM_VARIANT=0 -DFLAVOUR=\"esp_wroom_32\" -DFLIGHT_MODES -DFONT="" -DGUI -DGVARS -DHELI -DLCD_SPI -DOVERRIDE_CHANNEL_FUNCTION -DPCBESP_WROOM_32 -DPPM_CENTER_ADJUSTABLE -DPPM_UNIT_PERCENT_PREC1 -DROTARY_ENCODERS=1 -DSPLASH -DTRACE_SIMPGMSPACE -DTRANSLATIONS=\"EN\" -DTRANSLATIONS_EN

build: ${COMPONENT_BUILD_DIR}/configured
	echo Building $(COMPONENT_LIBRARY)
	cd ${COMPONENT_BUILD_DIR}/build ; \
	mingw32-make MAKEFLAGS= firmware ;\
	cp ${COMPONENT_BUILD_DIR}/build/radio/src/libfirmware.a ${COMPONENT_BUILD_DIR}/$(COMPONENT_LIBRARY)
	grep CXX_DEFINES ${COMPONENT_BUILD_DIR}/build/radio/src/CMakeFiles/firmware.dir/flags.make |\
	sed -E 's/CXX_DEFINES =\s*//;s/-I//g' > $(COMPONENT_PATH)/../startup/defines.mk
	grep CXX_INCLUDES ${COMPONENT_BUILD_DIR}/build/radio/src/CMakeFiles/firmware.dir/flags.make |\
	sed -E 's/CXX_INCLUDES =\s*//;s_-I(.):_/\l\1_g; s_\\_/_g' > $(COMPONENT_PATH)/../startup/includes.mk

clean:
	cd ${COMPONENT_BUILD_DIR}/build && rm -r *
	rm ${COMPONENT_BUILD_DIR}/configured    
	
	
${COMPONENT_BUILD_DIR}/configured:
	echo $(RADIO_SRCDIR)
	mkdir -p ${COMPONENT_BUILD_DIR}/build && \
	env -i HOME="${HOME}" /bin/bash -l -c 'cd ${COMPONENT_BUILD_DIR}/build && pwd && cmake -G "MinGW Makefiles" $(RADIO_SRCDIR)/../.. -DPCB=ESP_WROOM_32 -DTRANSLATIONS=EN -DCMAKE_MAKE_PROGRAM=mingw32-make' &&\
	touch ${COMPONENT_BUILD_DIR}/configured