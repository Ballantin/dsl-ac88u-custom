EXE = fw_upgrade
dynamic: generic_exe_install

clean:
	rm -f fw_upgrade


fw_upgrade:
	cp fw_upgrade_$(PROFILE).save fw_upgrade

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
