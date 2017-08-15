##################################################################################
#
# Copyright © 2008 Broadcom Corporation
#
# This program  is the  proprietary software  of Broadcom  Corporation and/or  its
# licensors, and may only be used, duplicated, modified or distributed pursuant to
# the  terms and  conditions of  a separate,  written license  agreement executed
# between you and Broadcom (an "Authorized  License").  Except as set forth in  an
# Authorized License, Broadcom  grants no license  (express or implied),  right to
# use, or waiver of any kind with respect to the Software, and Broadcom  expressly
# reserves all rights in and to the Software and all intellectual property  rights
# therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
# SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
# USE OF THE SOFTWARE.
#
# Except as expressly set forth in the Authorized License,
#
# 1.      This  program,  including  its  structure,  sequence  and  organization,
# constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
# reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
# information only  in connection  with your  use of  Broadcom integrated  circuit
# products.
#
# 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
# AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
# WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
# THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
# TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
# OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
# CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
# PERFORMANCE OF THE SOFTWARE.
#
# 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
# LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
# EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
# OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
# PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
# SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
# REMEDY.
#
##################################################################################
#
#    Filename: prot_NATtrvrsl_env.mk
#
#############################################################################
#
# This makefile is used to setup the prot_NATtrvrsl VOB make environment.
# It should only define variables and not have any rules in it.
#
# The intent of this makefile to is create two make variables:
#     - NATT_COMPOSITE_PUBLIC_INCLUDE
#     - NATT_COMPOSITE_PUBLIC_DEFS
#
# These define the _public_ include directories and defines associated with the
# specified make targets (NATT_MAKE_TARGETS), e.g. in the project makefile:
#
#     include $(NATT_ROOT)/prot_NATtrvrsl_env.mk
#
# Then the prot_NATtrvrsl public includes and defines can then be added to the
# global include/defs make variables used to compile the projects' application
# code, e.g. for the xchg_common build environment:
#
#     export XCHG_C_GLOBAL_INCLUDE += $(NATT_COMPOSITE_PUBLIC_INCLUDE)
#     export XCHG_C_GLOBAL_DEFS    += $(NATT_COMPOSITE_PUBLIC_DEFS)
#
# (If a different build environment is used, then change XCHG_C_GLOBAL_INCLUDE
# and XCHG_C_GLOBAL_DEFS to the appropriate make variables for that environment).
#
# Include directories and defines that are local (private) to prot_NATtrvrsl
# targets should be defined in prot_NATtrvrsl.mk.
#
# ---------------------------------------------------------------------------
#
# The following variables MUST be provided by the user, everything else should
# have a default value:
#     XCHG_OS           - Operating system type (e.g. VxWorks, pSOS).
#     XCHG_ROOT         - Full path of root xchg_common directory.
#     NATT_ROOT           - Full path of root prot_NATtrvrsl directory.
#     NATT_MAKE_TARGETS   - List of targets to make.
#
#############################################################################

NATT_ENV_INCLUDED := 1


# ---------------------------------------------------------------------------
# Error checking.
#

ifeq ($(XCHG_ROOT),)
$(error "XCHG_ROOT variable hasn't been set")
endif

ifeq ($(NATT_ROOT),)
$(error "NATT_ROOT variable hasn't been set")
endif

ifeq ($(NATT_MAKE_TARGETS),)
$(error "NATT_MAKE_TARGETS variable hasn't been set")
endif

# ---------------------------------------------------------------------------
# Include target list.
#
include $(NATT_ROOT)/xcbuild/prot_NATtrvrsl_targets.mk



# ---------------------------------------------------------------------------
# Setup the global include search path for this VOB.
#
# Add VOB public inc.
NATT_COMPOSITE_PUBLIC_INCLUDE := $(NATT_ROOT)/stacks/cfginc

#
# This creates a list of public include directories for this VOB, based upon
# the list of make targets, NATT_MAKE_TARGETS. Public include directories
# (NATT_PUBLIC_INC_xxx) are published in prot_NATtrvrsl_targets.mk.
#
NATT_PATH_LIST := $(addprefix NATT_PUBLIC_INC_, $(NATT_MAKE_TARGETS))

NATT_COMPOSITE_PUBLIC_INCLUDE += $(foreach inc,$(addprefix NATT_PUBLIC_INC_,$(NATT_MAKE_TARGETS)),$($(inc)))


#
# This creates a list of public defines for this VOB, based upon
# the list of make targets, NATT_MAKE_TARGETS. Public defines
# (NATT_PUBLIC_DEF_xxx) are published in prot_NATtrvrsl_targets.mk.
#
NATT_COMPOSITE_PUBLIC_DEFS := $(foreach def,$(addprefix NATT_PUBLIC_DEFS_,$(NATT_MAKE_TARGETS)),$($(def)))

