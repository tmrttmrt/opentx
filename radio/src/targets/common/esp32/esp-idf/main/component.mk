#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_DEPENDS := firmware

COMPONENT_EXTRA_INCLUDES := $(shell cat $(COMPONENT_PATH)/../components/firmware/includes.mk)
CPPFLAGS += $(shell cat $(COMPONENT_PATH)/../components/firmware/defines.mk)