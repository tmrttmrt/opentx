#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_DEPENDS := otxcore

COMPONENT_EXTRA_INCLUDES := $(shell cat $(COMPONENT_PATH)/../otxcore/includes.mk)
CPPFLAGS += $(shell cat $(COMPONENT_PATH)/../otxcore/defines.mk)
