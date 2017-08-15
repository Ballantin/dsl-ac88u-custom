###########################################################################
#
# This makefile is used to build the Zarlink API-2 module.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE           := bcm63xx_zarlink

BCM63XX_ZARLINK_ROOT  := $(XDRV_DIR_$(XCHG_LIB_BASE))


#
# List of all source files. Do not specify directories, we use vpath below.
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := \
$(notdir $(wildcard $(BCM63xx_ZARLINK_ROOT)/api_lib/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/api_lib/vp880_api/*.c))     \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/api_lib/vp886_api/*.c))     \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/api_lib/vp886_lt/*.c))     \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/api_lib/vp890_api/*.c))     \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/api_lib/common/*.c))        \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/api_lib/profiles/*.c))      \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/api_lib/vp_kernel/*.c))     \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/api_lib/arch/bcm63xx/*.c))  \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/common/*.c))                \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88111/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88221/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le89116/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le89316/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88010/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88266/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88276/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le89010/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88506/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/zl88601/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/zl88701/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/zl88702/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/zl88801/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le89136/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le89336/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88536/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88264/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le88004/*.c))               \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le9672/*.c))                \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le9662/*.c))                \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le9642/*.c))                \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le9641/*.c))                \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le9652/*.c))                \
$(notdir $(wildcard $(BCM63XX_ZARLINK_ROOT)/le9661/*.c))

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.c $(BCM63XX_ZARLINK_ROOT)/api_lib                               \
          $(BCM63XX_ZARLINK_ROOT)/api_lib/vp880_api                     \
          $(BCM63XX_ZARLINK_ROOT)/api_lib/vp886_api                     \
          $(BCM63XX_ZARLINK_ROOT)/api_lib/vp886_lt                      \
          $(BCM63XX_ZARLINK_ROOT)/api_lib/vp890_api                     \
          $(BCM63XX_ZARLINK_ROOT)/api_lib/common                        \
          $(BCM63XX_ZARLINK_ROOT)/api_lib/profiles                      \
          $(BCM63XX_ZARLINK_ROOT)/api_lib/vp_kernel                     \
          $(BCM63XX_ZARLINK_ROOT)/api_lib/arch/bcm63xx                  \
          $(BCM63XX_ZARLINK_ROOT)/common                                \
          $(BCM63XX_ZARLINK_ROOT)/le88111                               \
          $(BCM63XX_ZARLINK_ROOT)/le88221                               \
          $(BCM63XX_ZARLINK_ROOT)/le89116                               \
          $(BCM63XX_ZARLINK_ROOT)/le89316                               \
          $(BCM63XX_ZARLINK_ROOT)/le88010                               \
          $(BCM63XX_ZARLINK_ROOT)/le88266                               \
          $(BCM63XX_ZARLINK_ROOT)/le88276                               \
          $(BCM63XX_ZARLINK_ROOT)/le89010                               \
          $(BCM63XX_ZARLINK_ROOT)/le88506                               \
          $(BCM63XX_ZARLINK_ROOT)/zl88601                               \
          $(BCM63XX_ZARLINK_ROOT)/zl88701                               \
          $(BCM63XX_ZARLINK_ROOT)/zl88702                               \
          $(BCM63XX_ZARLINK_ROOT)/zl88801                               \
          $(BCM63XX_ZARLINK_ROOT)/le89136                               \
          $(BCM63XX_ZARLINK_ROOT)/le89336                               \
          $(BCM63XX_ZARLINK_ROOT)/le88536                               \
          $(BCM63XX_ZARLINK_ROOT)/le88264                               \
          $(BCM63XX_ZARLINK_ROOT)/le88004                               \
          $(BCM63XX_ZARLINK_ROOT)/le9672                                \
          $(BCM63XX_ZARLINK_ROOT)/le9661                                \
          $(BCM63XX_ZARLINK_ROOT)/le9642                                \
          $(BCM63XX_ZARLINK_ROOT)/le9641                                \
          $(BCM63XX_ZARLINK_ROOT)/le9652                                \
          $(BCM63XX_ZARLINK_ROOT)/le9662                               


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) := $(XCHG_LINUX_ROOT)/../../targets $(XCHG_LINUX_ROOT)/modulesrc/include/bcm963xx
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(VRG_ROOT)/inc        \
                                         $(VRG_ROOT)/casCtl/inc \
                                         $(VRG_ROOT)/boardHal/inc

XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(BCM63XX_ZARLINK_ROOT)/api_lib/includes      \
                                         $(BCM63XX_ZARLINK_ROOT)/api_lib/arch/bcm63xx  \
                                         $(BCM63XX_ZARLINK_ROOT)/api_lib/vp880_api     \
                                         $(BCM63XX_ZARLINK_ROOT)/api_lib/vp886_api     \
                                         $(BCM63XX_ZARLINK_ROOT)/api_lib/vp886_lt      \
                                         $(BCM63XX_ZARLINK_ROOT)/api_lib/vp890_api     \
                                         $(BCM63XX_ZARLINK_ROOT)/api_lib/vp_kernel     \
                                         $(BCM63XX_ZARLINK_ROOT)/api_lib/profiles      


#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) := XDRV_ZARLINK_TPD_SUPPORT=0

XCHG_C_LOCAL_COMPILER_OPTS := -Wno-error

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
