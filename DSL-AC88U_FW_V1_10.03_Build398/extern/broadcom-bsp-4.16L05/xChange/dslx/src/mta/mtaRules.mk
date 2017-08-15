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
#    Filename: mtaRules.mk
#    VSS Info:
#        $Revision: 13 $
#        $Date: 3/10/03 6:41p $
#
#############################################################################
#
#  This makefile provides the rules necessary to build MTA targets.
#
# It is intended to be invoked (not included) from a project makefile, but can 
# be invoked directly from the command line if the appropriate environment 
# variables are set.
#
#############################################################################


# ---------------------------------------------------------------------------
# Setup generic build environment.
#
include $(XCHG_ROOT)/rules/xChgEnv.mk

#
#  Include MTA target list.
#
include mtaTargets.mk


# ---------------------------------------------------------------------------
# Determine the set of modules to make.
#
# If no target is explicitly specified, the default behaviour is to build all 
# targets specified by $(MTA_ALL_TARGETS).
# 
ifeq ($(strip $(filter $(MAKECMDGOALS), $(MTA_ALL_TARGETS) $(addprefix clean_,$(MTA_ALL_TARGETS)))),)
# Either no target was specified, or a target that does not match a module name
# was specified (e.g. clean).
libTargets := $(MTA_ALL_TARGETS)
else
# Build a specific module (e.g. callclient), or clean a specific module (using the clean_xxx target).
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


# ---------------------------------------------------------------------------
#  Setup the public include and defines required to make the MTA.
#

# MTA public includes.
export XCHG_C_GLOBAL_INCLUDE += \
	$(CXC_MTA_DIR)/inc \
	$(CXC_MTA_DIR)/common/net/publicInc


# Add xchg_common public includes.
include $(CXC_XCHG_COMMON_DIR)/xchg_common_env.mk
export XCHG_C_GLOBAL_INCLUDE += $(XCHG_COMPOSITE_PUBLIC_INCLUDE)
export XCHG_C_GLOBAL_DEFS    += $(XCHG_COMPOSITE_PUBLIC_DEFS)


# Add xchg_drivers public includes.
export XCHG_C_GLOBAL_INCLUDE += $(CXC_XDRV_DIR)/inc


# Add Voice Residential Gateway public include and defs.
include $(CXC_VRG_DIR)/voice_res_gw_env.mk
export XCHG_C_GLOBAL_INCLUDE += $(VRG_COMPOSITE_PUBLIC_INCLUDE)
export XCHG_C_GLOBAL_DEFS    += $(VRG_COMPOSITE_PUBLIC_DEFS)
    
    
# Add LDX public includes.
export XCHG_C_GLOBAL_INCLUDE += \
	$(CXC_LDX_HAUSWARE_INC_DIR)



# SNMP specific includes and defines.
ifeq ($(CXC_OS_TYPE),VxWorks)
export XCHG_C_GLOBAL_INCLUDE += $(CXC_OS_ROOT_DIR)/target/h/wrn/wm/snmp/engine
export XCHG_C_GLOBAL_DEFS    += SNMP_WINDMANAGE=1
endif

#
# This is required so that the MTA knows what OS it is dealing with.
#
XCHG_C_GLOBAL_DEFS += CXC_OS_$(shell $(ECHO) $(CXC_OS_TYPE) | $(TR) 'a-z' 'A-Z')=1


# ---------------------------------------------------------------------------
# Include all the module makefiles.
#

# This phony target is necessary because some of the module makefiles may have
# targets in them, and we don't want one of them to become the default 
# make target. Also, xChgRules.mk needs to be included after the module 
# makefiles. The real 'all' target is specified in xChgRules.mk.
.PHONY: all
all:


MTA_MODULE_MAKEFILES := $(foreach target,$(libTargets),$(MTA_DIR_$(target))/$(target).mk)
include $(MTA_MODULE_MAKEFILES)



include $(XCHG_ROOT)/rules/xChgRules.mk


# ---------------------------------------------------------------------------
# Default target.
#
.PHONY: target $(MTA_ALL_TARGETS)
target: $(MTA_ALL_TARGETS)

#
# Target to clean a specific module, e.g. clean_bos.
#
.PHONY: $(addprefix clean_,$(MTA_ALL_TARGETS))
$(addprefix clean_,$(MTA_ALL_TARGETS)): clean_obj clean_dep clean_mta_libs


.PHONY: clean_mta_libs
clean_lib: clean_mta_libs
clean_mta_libs:
	$(RM) -f $(call XCHG_MAKE_LIB_NAME,$(libTargets))


# ---------------------------------------------------------------------------
# Generate the make syntax that defines the rules that are used to build the
# module libraries.
# 

# Error check.
$(foreach target,$(libTargets),$(if $(XCHG_SRC_FILES_$(target)),,$(error "XCHG_SRC_FILES_$(target) is empty.")))

# Generate library targets.
$(foreach target,$(libTargets),$(eval $(call XCHG_ARCHIVE_TEMPLATE,$(target))))
