LIB = libtmctl.so
default: dynamic


libtmctl.so:
	cp libtmctl.so_$(PROFILE).save libtmctl.so

dynamic: generic_private_lib_install

clean:
	rm -f libtmctl.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
