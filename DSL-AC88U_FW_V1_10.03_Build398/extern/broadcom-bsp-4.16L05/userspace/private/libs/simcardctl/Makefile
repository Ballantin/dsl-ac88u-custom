LIB = libsimcardctl.so
default: dynamic


libsimcardctl.so:
	cp libsimcardctl.so_$(PROFILE).save libsimcardctl.so

dynamic: generic_private_lib_install

clean:
	rm -f libsimcardctl.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
