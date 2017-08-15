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
#                  This makefile is used to build the NATT module.
#
##################################################################################
#
# This makefile provides the rules necessary to build the prot_NATtrvrsl VOB.
#
# It is intended to be invoked (not included) from a project makefile, but can
# be invoked directly from the command line if the appropriate environment
# variables are set.
#
# ---------------------------------------------------------------------------
#
# The following is an example of how to build prot_NATtrvrsl targets from a
# a project makefile:
#
#     export NATT_MAKE_TARGETS = stunc turnc ice ...
#     default: prot_NATtrvrslTargets
#
#     .PHONY: prot_NATtrvrslTargets
#     prot_NATtrvrslTargets
#        @$(MAKE) -C $(NATT_ROOT) -f prot_NATtrvrsl.mk
#
#     .PHONY: clean_prot_NATtrvrsl
#     clean_prot_NATtrvrsl:
#        @$(MAKE) -C $(NATT_ROOT) -f prot_NATtrvrsl.mk clean
#
# ---------------------------------------------------------------------------
#
# The following variables MUST be provided by the user, everything else should
# have a default value:
#     XCHG_ROOT         - Full path of root xchg_common directory.
#     NATT_ROOT         - Full path of root prot_NATtrvrsl directory.
#     NATT_MAKE_TARGETS - List of targets to make.
#
#############################################################################

ifeq ($(NATT_CONFIG_INCLUDE), )
$(error "NATT_CONFIG_INCLUDE has not been set")
endif


# ---------------------------------------------------------------------------
# Setup generic build environment.
#
include $(XCHG_ROOT)/rules/xChgEnv.mk

#
# Setup prot_NATtrvrsl build environment.
#
include $(NATT_ROOT)/xcbuild/prot_NATtrvrsl_env.mk



# ---------------------------------------------------------------------------
# Determine the set of modules to make.
#
# If no target is explicitly specified, the default behaviour is to build all
# targets specified by $(NATT_ALL_TARGETS).
#
NATT_VALID_TARGETS=$(NATT_ALL_TARGETS) $(addprefix clean_,$(NATT_ALL_TARGETS)) $(addprefix audit_,$(NATT_ALL_TARGETS))

ifeq ($(strip $(filter $(MAKECMDGOALS),${NATT_VALID_TARGETS})),)
# Either no target was specified, or a target that does not match a module name
# was specified (e.g. clean).
libTargets := $(NATT_MAKE_TARGETS)
else
# Build a specific module (e.g. stunc), or clean a specific module (using the clean_xxx target).
libTargets := $(subst clean_,,$(MAKECMDGOALS))
libTargets := $(subst audit_,,$(libTargets))
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
# Append the public include paths and defines for the specified
# prot_NATtrvrsl targets to the global compiler options.
#
XCHG_C_GLOBAL_DEFS               += $(NATT_COMPOSITE_PUBLIC_DEFS)
XCHG_C_GLOBAL_INCLUDE            += $(NATT_COMPOSITE_PUBLIC_INCLUDE)


#
# Add the public includes paths for the BOS and ASSERT module.
#
include $(XCHG_ROOT)/xchg_common_targets.mk

# ---------------------------------------------------------------------------
# Include all the module makefiles.
#

# This phony target is necessary because some of the module makefiles may have
# targets in them, and we don't want one of them to become the default
# make target. Also, xChgRules.mk needs to be included after the module
# makefiles. The real 'all' target is specified in xChgRules.mk.
.PHONY: all
all:


NATT_MODULE_MAKEFILES := $(foreach target,$(libTargets),$(NATT_DIR_$(target))/$(target).mk)
include $(NATT_MODULE_MAKEFILES)

include $(XCHG_ROOT)/rules/xChgRules.mk

# ---------------------------------------------------------------------------
# Default target.
#
.PHONY: target $(NATT_MAKE_TARGETS)
target: $(NATT_MAKE_TARGETS)

#
# Target to clean a specific module, e.g. clean_bos.
#
.PHONY: $(addprefix clean_,$(NATT_ALL_TARGETS))
$(addprefix clean_,$(NATT_ALL_TARGETS)): clean_obj clean_dep clean_natt_libs


.PHONY: clean_natt_libs
clean_lib: clean_natt_libs
clean_natt_libs:
   $(RM) -f $(call XCHG_MAKE_LIB_NAME,$(libTargets))


# ---------------------------------------------------------------------------
# Generate the make syntax that defines the rules that are used to build the
# module libraries.
#

# Error check.
$(foreach target,$(libTargets),$(if $(XCHG_SRC_FILES_$(target)),,$(error "XCHG_SRC_FILES_$(target) is empty.")))

# Generate library targets.
$(foreach target,$(libTargets),$(eval $(call XCHG_ARCHIVE_TEMPLATE,$(target))))
