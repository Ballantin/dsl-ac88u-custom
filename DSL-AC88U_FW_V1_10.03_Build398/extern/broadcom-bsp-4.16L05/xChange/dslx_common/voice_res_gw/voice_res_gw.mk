#############################################################################
#
#    Copyright 2000-2002  Broadcom Corporation
#    All Rights Reserved
#    No portions of this material may be reproduced in any form without the
#    written permission of:
#        Broadcom Corporation
#        16215 Alton Parkway
#        P.O. Box 57013
#        Irvine, California 92619-7013
#    All information contained in this document is Broadcom Corporation
#    company private, proprietary, and trade secret.
#
#############################################################################
#
#    Filename: voice_res_gw.mk
#
#############################################################################
#
# This makefile provides the rules necessary to build the voice_res_gw VOB.
#
# It is intended to be invoked (not included) from a project makefile, but can
# be invoked directly from the command line if the appropriate environment
# variables are set.
#
# ---------------------------------------------------------------------------
#
# The following is an example of how to build voice_res_gw targets from a
# a project makefile:
#
#     export VRG_MAKE_TARGETS = casCtl endpt ...
#     default: vrgTargets
#
#     .PHONY: vrgTargets
#     vrgTargets
#        @$(MAKE) -C $(VRG_ROOT) -f voice_res_gw.mk
#
#     .PHONY: clean_vrg
#     clean_vrg:
#        @$(MAKE) -C $(VRG_ROOT) -f voice_res_gw.mk clean
#
# ---------------------------------------------------------------------------
#
# The following variables MUST be provided by the user, everything else should
# have a default value:
#     XCHG_OS           - Operating system type (e.g. VxWorks, pSOS).
#     XCHG_ROOT         - Full path of root xchg_common directory.
#     XDRV_ROOT         - Full path of root xchg_drivers directory.
#     VRG_ROOT          - Full path of root voice_res_gw directory.
#     VRG_MAKE_TARGETS  - List of targets to make.
#
#############################################################################


# ---------------------------------------------------------------------------
# Error check.
#
ifeq ($(VRG_LDX_DSP_APP_DIR),)
$(error "VRG_LDX_DSP_APP_DIR hasn't been set" )
endif


# ---------------------------------------------------------------------------
# Setup generic build environment.
#
include $(XCHG_ROOT)/rules/xChgEnv.mk


#
# Setup voice_res_gw build environment.
#
include $(VRG_ROOT)/voice_res_gw_env.mk



# ---------------------------------------------------------------------------
# Setup default paths for VRG.
#

ifeq ($(VRG_LDX_HAUSWARE_INC_DIR),)
export VRG_LDX_HAUSWARE_INC_DIR = $(VRG_ROOT)/../ldx_hausware/hausware/inc
endif

ifeq ($(VRG_LDX_HAUSWARE_SYSINC_DIR),)
export VRG_LDX_HAUSWARE_SYSINC_DIR = $(VRG_ROOT)/../ldx_hausware/hausware/sysinc
endif

ifeq ($(VRG_LDX_HAUSWARE_CFGINC_DIR),)
export VRG_LDX_HAUSWARE_CFGINC_DIR = $(VRG_ROOT)/../ldx_hausware/hausware/cfginc
endif


# ---------------------------------------------------------------------------
# Determine the set of modules to make.
#
# If no target is explicitly specified, the default behaviour is to build all
# targets specified by $(VRG_ALL_TARGETS).
#
ifeq ($(strip $(filter $(MAKECMDGOALS), $(VRG_ALL_TARGETS) $(addprefix clean_,$(VRG_ALL_TARGETS)))),)
# Either no target was specified, or a target that does not match a module name
# was specified (e.g. clean).
libTargets := $(VRG_MAKE_TARGETS)
else
# Build a specific module (e.g. endpt), or clean a specific module (using the clean_xxx target).
libTargets := $(subst clean_,,$(MAKECMDGOALS))
endif



# ---------------------------------------------------------------------------
# These will be added to by the module makefiles. Initialize these here
# using immediate evaluation to guarantee that when things get added
# to these variables that it will be done using immediate evaluation. This is
# more efficient than using deferred variable evaluation.
#
XCHG_SRC_FILES                   :=
XCHG_C_IMPORTED_GLOBAL_DEFS      := $(XCHG_C_GLOBAL_DEFS)
XCHG_C_GLOBAL_DEFS               := $(XCHG_C_IMPORTED_GLOBAL_DEFS)
XCHG_C_IMPORTED_GLOBAL_INCLUDE   := $(XCHG_C_GLOBAL_INCLUDE)
XCHG_C_GLOBAL_INCLUDE            := $(XCHG_C_IMPORTED_GLOBAL_INCLUDE)


#
# Append the public include paths and defines for the specified voice_res_gw
# targets to the global compiler options.
#
XCHG_C_GLOBAL_DEFS               += $(VRG_COMPOSITE_PUBLIC_DEFS)
XCHG_C_GLOBAL_INCLUDE            += $(VRG_COMPOSITE_PUBLIC_INCLUDE)


include $(XDRV_ROOT)/xchg_drivers_env.mk
XCHG_C_GLOBAL_DEFS               += $(XDRV_COMPOSITE_PUBLIC_DEFS)
XCHG_C_GLOBAL_INCLUDE            += $(XDRV_COMPOSITE_PUBLIC_INCLUDE)


#
# Add the public includes paths for xchg_common modules.
#
include $(XCHG_ROOT)/xchg_common_env.mk
XCHG_C_GLOBAL_DEFS               += $(XCHG_COMPOSITE_PUBLIC_DEFS)
XCHG_C_GLOBAL_INCLUDE            += $(XCHG_COMPOSITE_PUBLIC_INCLUDE)


#
# This is required so that hausware knows what OS it is dealing with.
#
XCHG_C_GLOBAL_DEFS += HAUS_OS_CFG_$(shell $(ECHO) $(XCHG_OS) | $(TR) 'a-z' 'A-Z')=1



# ---------------------------------------------------------------------------
# Include all the module makefiles.
#

# This phony target is necessary because some of the module makefiles may have
# targets in them, and we don't want one of them to become the default
# make target. Also, xChgRules.mk needs to be included after the module
# makefiles. The real 'all' target is specified in xChgRules.mk.
.PHONY: all
all:


VRG_MODULE_MAKEFILES := $(foreach target,$(libTargets),$(VRG_DIR_$(target))/$(target).mk)
include $(VRG_MODULE_MAKEFILES)



include $(XCHG_ROOT)/rules/xChgRules.mk


# ---------------------------------------------------------------------------
# Default target.
#
.PHONY: target $(VRG_MAKE_TARGETS)
target: $(VRG_MAKE_TARGETS)

#
# Target to clean a specific module, e.g. clean_endpt.
#
.PHONY: $(addprefix clean_,$(VRG_ALL_TARGETS))
$(addprefix clean_,$(VRG_ALL_TARGETS)): clean_obj clean_dep clean_voice_res_gw_libs


.PHONY: clean_voice_res_gw_libs
clean_lib: clean_voice_res_gw_libs
clean_voice_res_gw_libs:
	$(RM) -f $(call XCHG_MAKE_LIB_NAME,$(libTargets))


# ---------------------------------------------------------------------------
# Generate the make syntax that defines the rules that are used to build the
# module libraries.
#

# Error check.
$(foreach target,$(libTargets),$(if $(XCHG_SRC_FILES_$(target)),,$(error "XCHG_SRC_FILES_$(target) is empty.")))

# Generate library targets.
$(foreach target,$(libTargets),$(eval $(call XCHG_ARCHIVE_TEMPLATE,$(target))))
