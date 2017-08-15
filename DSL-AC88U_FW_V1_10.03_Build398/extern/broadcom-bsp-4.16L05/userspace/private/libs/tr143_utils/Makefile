LIB = libtr143_utils.so
default: dynamic


libtr143_utils.so:
	cp libtr143_utils.so_$(PROFILE).save libtr143_utils.so

dynamic: generic_private_lib_install

clean:
	rm -f libtr143_utils.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
