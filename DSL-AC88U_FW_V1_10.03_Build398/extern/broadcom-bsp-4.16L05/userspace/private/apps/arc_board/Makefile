EXE = arc_board
dynamic: generic_exe_install

clean:
	rm -f arc_board


arc_board:
	cp arc_board_$(PROFILE).save arc_board

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
