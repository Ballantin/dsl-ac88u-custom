#############################################################################
#
# Copyright © 2000-2008 Broadcom Corporation
#
# This program is the proprietary software of Broadcom Corporation and/or its
# licensors, and may only be used, duplicated, modified or distributed
# pursuant to the terms and conditions of a separate, written license
# agreement executed between you and Broadcom (an "Authorized License").
# Except as set forth in an Authorized License, Broadcom grants no license
# (express or implied), right to use, or waiver of any kind with respect to
# the Software, and Broadcom expressly reserves all rights in and to the
# Software and all intellectual property rights therein.  IF YOU HAVE NO
# AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
# AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
# SOFTWARE.
#
# Except as expressly set forth in the Authorized License,
#
# 1.     This program, including its structure, sequence and organization,
#        constitutes the valuable trade secrets of Broadcom, and you shall
#        use all reasonable efforts to protect the confidentiality thereof,
#        and to use this information only in connection with your use of
#        Broadcom integrated circuit products.
#
# 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
#        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
#        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
#        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
#        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
#        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
#        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
#        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
#        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
#
# 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
#        OR ITS LICENSORS BE LIABLE FOR
#        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
#             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
#             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
#             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
#        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
#             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
#             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
#             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
#
#############################################################################
#
#    Filename: xchg_drivers_env.mk
#
#############################################################################
#
# This makefile is used to setup the xchg_drivers VOB make environment.
# It should only define variables and not have any rules in it.
#
# The intent of this makefile to is create two make variables:
#     - XDRV_COMPOSITE_PUBLIC_INCLUDE
#     - XDRV_COMPOSITE_PUBLIC_DEFS
#
# These define the _public_ include directories and defines associated with the
# specified make targets (XDRV_MAKE_TARGETS), e.g. in the project makefile:
#
#     include $(XDRV_ROOT)/xchg_drivers_env.mk
#
# Then the xchg_drivers public includes and defines can then be added to the
# global include/defs make variables used to compile the projects' application
# code, e.g. for the xchg_common build environment:
#
#     export XCHG_C_GLOBAL_INCLUDE += $(XDRV_COMPOSITE_PUBLIC_INCLUDE)
#     export XCHG_C_GLOBAL_DEFS    += $(XDRV_COMPOSITE_PUBLIC_DEFS)
#
# (If a different build environment is used, then change XCHG_C_GLOBAL_INCLUDE
# and XCHG_C_GLOBAL_DEFS to the appropriate make variables for that environment).
#
# Include directories and defines that are local (private) to xchg_drivers
# targets should be defined in xchg_drivers.mk.
#
# ---------------------------------------------------------------------------
#
# The following variables MUST be provided by the user, everything else should
# have a default value:
#     XCHG_OS           - Operating system type (e.g. VxWorks, pSOS).
#     XCHG_ROOT         - Full path of root xchg_common directory.
#     XDRV_ROOT         - Full path of root xchg_drivers directory.
#     XDRV_MAKE_TARGETS - List of targets to make.
#
#############################################################################

XDRV_ENV_INCLUDED := 1


# ---------------------------------------------------------------------------
# Error checking.
#

ifeq ($(XCHG_ROOT),)
$(error "XCHG_ROOT variable hasn't been set")
endif

ifeq ($(XDRV_ROOT),)
$(error "XDRV_ROOT variable hasn't been set")
endif

ifeq ($(XDRV_MAKE_TARGETS),)
$(error "XDRV_MAKE_TARGETS variable hasn't been set")
endif


# ---------------------------------------------------------------------------
# Include target list.
#
include $(XDRV_ROOT)/xchg_drivers_targets.mk

include $(XCHG_ROOT)/rules/xChgEnv.mk



# ---------------------------------------------------------------------------
# Setup the global include search path for this VOB.
#

# Add VOB public inc.
XDRV_COMPOSITE_PUBLIC_INCLUDE := $(XDRV_ROOT)/inc

#
# This creates a list of public include directories for this VOB, based upon
# the list of make targets, XDRV_MAKE_TARGETS. Public include directories
# (XDRV_PUBLIC_INC_xxx) that are published in xdrvTargetList.mk.
#
XDRV_PATH_LIST := $(addprefix XDRV_PUBLIC_INC_, $(XDRV_MAKE_TARGETS))

XDRV_COMPOSITE_PUBLIC_INCLUDE += $(foreach inc,$(addprefix XDRV_PUBLIC_INC_,$(XDRV_MAKE_TARGETS)),$($(inc)))


#
# Determine the list of chip-types associated with the target list. This is
# necessary in order to be able to include the appropriate chip-specific
# include paths.
#
# XDRV_SUPPORTED CHIPS contains a list of all supported chip-types. For each one,
# determine if any of the specified targets is associated with that chip type.
# It is assumed that chip-types are of the form 'bcmxxx' and targets are of the
# form 'bcmxx_yyy'. XDRV_TARGET_CHIP_TYPES will contain a list of all chips
# associated with the specified targets.
XDRV_TARGET_CHIP_TYPES := $(foreach chip,$(XDRV_SUPPORTED_CHIPS),$(findstring $(chip),$(XDRV_MAKE_TARGETS)))


# Add chip-specific includes.
XDRV_CHIP_TYPES_DIR := $(addprefix $(XDRV_ROOT)/,$(addsuffix /chipinc,$(XDRV_TARGET_CHIP_TYPES)))
XDRV_COMPOSITE_PUBLIC_INCLUDE += $(XDRV_CHIP_TYPES_DIR)


# Add architecture-specific (MIPS, ZSP, etc) include directories.
#
XCHG_XARCH_CONVERTED=0

ifeq (${XCHG_ARCH},MIPS)
XCHG_ARCH_CONVERTED=1
XCHG_ARCH_LOWER_CASE := mips
endif

ifeq (${XCHG_ARCH},ARM)
XCHG_ARCH_CONVERTED=1
XCHG_ARCH_LOWER_CASE := arm
endif

ifeq (${XCHG_ARCH},ZSP)
XCHG_ARCH_CONVERTED=1
XCHG_ARCH_LOWER_CASE := zsp
endif

ifeq (${XCHG_ARCH_CONVERTED},0)
$(error XCHG_ARCH was not converted properly XCHG_ARCH is ${XCHG_ARCH})
endif

XDRV_COMPOSITE_PUBLIC_INCLUDE += $(XDRV_ROOT)/arch/$(XCHG_ARCH_LOWER_CASE)/inc


# ---------------------------------------------------------------------------
# Setup the "composite" defines for this VOB.
#
XDRV_COMPOSITE_PUBLIC_DEFS =
