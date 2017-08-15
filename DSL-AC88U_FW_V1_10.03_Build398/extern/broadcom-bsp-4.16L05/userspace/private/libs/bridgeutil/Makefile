LIB = libbridgeutil.so
default: dynamic


libbridgeutil.so:
	cp libbridgeutil.so_$(PROFILE).save libbridgeutil.so

dynamic: generic_private_lib_install

clean:
	rm -f libbridgeutil.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
