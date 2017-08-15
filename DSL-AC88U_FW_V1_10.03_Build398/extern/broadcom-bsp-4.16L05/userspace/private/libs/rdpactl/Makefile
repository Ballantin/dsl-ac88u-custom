LIB = librdpactl.so
default: dynamic


librdpactl.so:
	cp librdpactl.so_$(PROFILE).save librdpactl.so

dynamic: generic_private_lib_install

clean:
	rm -f librdpactl.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
