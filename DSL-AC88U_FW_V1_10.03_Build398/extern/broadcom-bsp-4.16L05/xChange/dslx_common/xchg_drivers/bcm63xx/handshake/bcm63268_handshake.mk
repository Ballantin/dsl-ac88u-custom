##################################################################################
#
# This makefile is a wrapper for 6362 used to build the bcm63xx_hankshake module.
#
##################################################################################

XCHG_LIB_BASE  :=  bcm63268_handshake

include $(XDRV_DIR_bcm63xx_handshake)/bcm63xx_handshake.mk

XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(XCHG_SRC_FILES);

