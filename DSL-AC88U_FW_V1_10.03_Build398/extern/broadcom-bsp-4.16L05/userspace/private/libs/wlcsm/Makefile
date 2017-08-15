LIB = libwlcsm.so
all: dynamic
CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

dynamic: generic_private_lib_install

clean:
	rm -f libwlcsm.so

libwlcsm.so:
	cp libwlcsm.so_$(PROFILE).save libwlcsm.so
	-cp libwlcsm.a_$(PROFILE).save libwlcsm.a
dynamic: generic_private_lib_install

clean:
	-rm -f *.a *.so
