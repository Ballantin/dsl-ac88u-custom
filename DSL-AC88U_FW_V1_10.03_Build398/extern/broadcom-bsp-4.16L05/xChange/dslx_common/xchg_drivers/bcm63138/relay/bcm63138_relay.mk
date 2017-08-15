###########################################################################
#
# This makefile is used to build the relay module.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE           := bcm63138_relay

BCM_RELAY_ROOT          := $(XDRV_DIR_$(XCHG_LIB_BASE))


#
# List of all source files. Do not specify directories, we use vpath below.
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(BCM_RELAY_ROOT)/*.c))

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.c $(BCM_RELAY_ROOT)


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) := $(XCHG_LINUX_ROOT)/../../targets $(XCHG_LINUX_ROOT)/modulesrc/include/bcm963xx
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(XCHG_LINUX_ROOT)/../../xChange/dslx_common/voice_res_gw/inc


#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) :=


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

