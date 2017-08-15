###########################################################################
#
# This makefile is used to build the bcm63xx_hankshake module.
#
###########################################################################

#
# This should match the target name.
#
BCM63XX_HANDSHAKE_ROOT  := $(XDRV_DIR_bcm63xx_handshake)


#
# We use notdir and vpath so that the object files don't wind up in
# nested subdirectories
#
XCHG_SRC_FILES := $(notdir $(wildcard $(BCM63XX_HANDSHAKE_ROOT)/*.s $(BCM63XX_HANDSHAKE_ROOT)/*.c))
XCHG_SRC_FILES_bcm63xx_handshake := $(XCHG_SRC_FILES)

vpath %.s $(BCM63XX_HANDSHAKE_ROOT)
vpath %.c $(BCM63XX_HANDSHAKE_ROOT)



#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_bcm63xx_handshake := 

#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_DEFS_bcm63xx_handshake :=


#
# To set a module specific compiler option, create a target specific make
# variable which sets the option and depends on the module target name.
# For example, to enable compiler optimizations for this module:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_ENABLE)
#
# or, to disable optimizations:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_DISABLE)
