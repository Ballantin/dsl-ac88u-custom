###########################################################################
#
# This makefile is used to build the MEM test program.
#
###########################################################################

XCHG_ROOT = ../..
include $(XCHG_ROOT)/rules/xChgEnv.mk

XCHG_C_GLOBAL_INCLUDE = $(call ABS_PATH,.)

export XCHG_MAKE_TARGETS := assert bos mem str

libs:
    $(MAKE) -C $(XCHG_ROOT) -f xchg_common.mk mem

clean_lib:
    $(MAKE) -C $(XCHG_ROOT) -f xchg_common.mk clean_mem

include $(XCHG_ROOT)/rules/xChgRules.mk
