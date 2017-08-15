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
#    Filename: voice_res_gw_env.mk
#
#############################################################################
#
# This makefile is used to setup the voice_res_gw VOB make environment. 
# It should only define variables and not have any rules in it.
#
# The intent of this makefile to is create two make variables:
#     - VRG_COMPOSITE_PUBLIC_INCLUDE
#     - VRG_COMPOSITE_PUBLIC_DEFS
#
# These define the _public_ include directories and defines associated with the
# specified make targets (VRG_MAKE_TARGETS), e.g. in the project makefile:
#
#     include $(VRG_ROOT)/voice_res_gw_env.mk
# 
# Then the voice_res_gw public includes and defines can then be added to the 
# global include/defs make variables used to compile the projects' application
# code, e.g. for the xchg_common build environment:
#
#     export XCHG_C_GLOBAL_INCLUDE += $(VRG_COMPOSITE_PUBLIC_INCLUDE)
#     export XCHG_C_GLOBAL_DEFS    += $(VRG_COMPOSITE_PUBLIC_DEFS)
#
# (If a different build environment is used, then change XCHG_C_GLOBAL_INCLUDE
# and XCHG_C_GLOBAL_DEFS to the appropriate make variables for that environment).
#
# Include directories and defines that are local (private) to voice_res_gw
# targets should be defined in voice_res_gw.mk.
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

VRG_ENV_INCLUDED := 1


# ---------------------------------------------------------------------------
# Error checking.
#

ifeq ($(XCHG_ROOT),)
$(error "XCHG_ROOT variable hasn't been set")
endif

ifeq ($(VRG_ROOT),)
$(error "VRG_ROOT variable hasn't been set")
endif

ifeq ($(XDRV_ROOT),)
$(error "XDRV_ROOT variable hasn't been set")
endif

ifeq ($(VRG_MAKE_TARGETS),)
$(error "VRG_MAKE_TARGETS variable hasn't been set")
endif


# ---------------------------------------------------------------------------
# Include target list.
#
include $(VRG_ROOT)/voice_res_gw_targets.mk


# ---------------------------------------------------------------------------
# Setup the global include search path for this VOB.
#

# Add VOB public inc.
VRG_COMPOSITE_PUBLIC_INCLUDE := $(XDRV_ROOT)/inc
VRG_COMPOSITE_PUBLIC_INCLUDE += $(VRG_ROOT)/inc

#
# This creates a list of public include directories for this VOB, based upon
# the list of make targets, XDRV_MAKE_TARGETS. Public include directories
# (XDRV_PUBLIC_INC_xxx) that are published in xdrvTargetList.mk.
#
VRG_COMPOSITE_PUBLIC_INCLUDE += $(foreach inc,$(addprefix VRG_PUBLIC_INC_,$(VRG_MAKE_TARGETS)),$($(inc)))


# ---------------------------------------------------------------------------
# Setup the "composite" defines for this VOB.
#
XDRV_COMPOSITE_PUBLIC_DEFS =
