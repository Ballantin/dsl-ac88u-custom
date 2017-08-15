LIB = libcms_qdm.so
default: dynamic


libcms_qdm.so:
	cp libcms_qdm.so_$(PROFILE).save libcms_qdm.so

dynamic: generic_private_lib_install

clean:
	rm -f libcms_qdm.so

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
