###########################################################################
#
# This makefile is used to build the bcm96816pvwm module.
#
###########################################################################


#
# This should match the target name.
#
XCHG_LIB_BASE     := bcm6816uni

BCM6816VMS_ROOT  := $(VRG_DIR_$(XCHG_LIB_BASE))

#
# List of all source files. Do not specify directories, we use vpath below.
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(BCM6816VMS_ROOT)/*.c \
                                                       $(BCM6816VMS_ROOT)/../common/*.c))


# Source files used for both IKOS simulations and the real chip.
XCHG_SRC_FILES_$(XCHG_LIB_BASE) += boardHalDsp6816.c     \
                                   boardHalIpc6816.c     \
                                   boardHalPower6816.c   \


# Source files only used on the real chip.
XCHG_SRC_FILES_$(XCHG_LIB_BASE) += boardHalApm6816.c  \
                                   flexiCalc6816.c    \
                                   boardHalTpd6816.c  \
                                   boardHalGpio6816.c \
                                   boardHalSlic6816L9530.c \
                                   boardHalSlic6816Si3239.c

ifeq ($(strip $(BRCM_NONDIST_BUILD)),1)
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(filter-out boardHalIpc6816.c,$(XCHG_SRC_FILES_$(XCHG_LIB_BASE)))
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(filter-out boardHalInitCommon.c,$(XCHG_SRC_FILES_$(XCHG_LIB_BASE)))
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(filter-out boardHalDspTask.c,$(XCHG_SRC_FILES_$(XCHG_LIB_BASE)))
ifeq ($(strip $(BRCM_SMP_BUILD)),)
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(filter-out boardHalDspTaskNonDist.c,$(XCHG_SRC_FILES_$(XCHG_LIB_BASE)))
endif
else
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(filter-out boardHalInitCommonNonDist.c,$(XCHG_SRC_FILES_$(XCHG_LIB_BASE)))
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(filter-out boardHalDspTaskNonDist.c,$(XCHG_SRC_FILES_$(XCHG_LIB_BASE)))
endif								   

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.c $(BCM6816VMS_ROOT) $(BCM6816VMS_ROOT)/../common6816/flexicalc/ $(BCM6816VMS_ROOT)/../common $(BCM6816VMS_ROOT)/../common6816 $(BCM6816VMS_ROOT)/../sim


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) := $(VRG_LDX_DSP_APP_DIR) \
                                         $(BCM6816VMS_ROOT)/../common6816 \
                                         $(BCM6816VMS_ROOT)/../common6816/flexicalc \
                                         $(BCM6816VMS_ROOT)/../common6816/flexicalc/L9530 \
                                         $(BCM6816VMS_ROOT)/../common6816/flexicalc/Si3239 \
                                         $(BCM6816VMS_ROOT)/../common \
                                         $(BCM6816VMS_ROOT) \
                                         $(VRG_LDX_HAUSWARE_INC_DIR) \
                                         $(VRG_LDX_HAUSWARE_SYSINC_DIR) \
                                         $(VRG_LDX_HAUSWARE_CFGINC_DIR)

ifeq ($(strip $(BRCM_NONDIST_BUILD)),1)
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(VRG_LDX_DSP_APP_DIR)/../../../../../common/inc \
                                         $(VRG_LDX_DSP_APP_DIR)/../../../../../dsl/common/inc \
                                         $(VRG_LDX_DSP_APP_DIR)/../../../../common/chipinc \
                                         $(VRG_LDX_DSP_APP_DIR)/../../../../common/source/hal_dsp_nondist_flexible
else
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(VRG_LDX_DSP_APP_DIR)/../../../../common/inc \
                                         $(VRG_LDX_DSP_APP_DIR)/../../../common/chipinc \
                                         $(VRG_LDX_DSP_APP_DIR)/../../../common/inc
endif

#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) := VRG_DSP_DATE_STR=\"$(VRG_LDX_DATE)\"



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
