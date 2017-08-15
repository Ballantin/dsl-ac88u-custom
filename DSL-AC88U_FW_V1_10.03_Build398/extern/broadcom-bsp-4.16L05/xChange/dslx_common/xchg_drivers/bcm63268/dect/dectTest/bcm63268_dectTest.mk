###########################################################################
#
# This makefile is used to build the bcm63268 dect Test module.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE           := bcm63268_dectTest

BCM63268_DECTTEST_ROOT  := $(XDRV_DIR_$(XCHG_LIB_BASE))

#
# List of all source files. Do not specify directories, we use vpath below.
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := \
$(notdir $(wildcard $(BCM63268_DECTTEST_ROOT)/*.c))   \
$(notdir $(wildcard $(BCM63268_DECTTEST_ROOT)/dectIP/*.c))               

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.c $(BCM63268_DECTTEST_ROOT)  \
          $(BCM63268_DECTTEST_ROOT)/dectIP


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) := $(XCHG_LINUX_ROOT)/../../targets $(XCHG_LINUX_ROOT)/modulesrc/include/bcm963xx
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(XCHG_LINUX_ROOT)/../../xChange/dslx_common/voice_res_gw/inc
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(XCHG_LINUX_ROOT)/../../xChange/dslx_common/xchg_drivers/arch/mips/intCtrl/inc


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

