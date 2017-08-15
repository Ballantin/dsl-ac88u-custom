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
#                  This makefile is used to build the CCTK module.
#
##################################################################################
#
# This makefile provides the rules necessary to build the prot_CCTK VOB.
#
# It is intended to be invoked (not included) from a project makefile, but can
# be invoked directly from the command line if the appropriate environment
# variables are set.
#
# ---------------------------------------------------------------------------
#
# The following is an example of how to build prot_cctk targets from a
# a project makefile:
#
#     export CCTK_MAKE_TARGETS = cctk
#     default: prot_cctkTargets
#
#     .PHONY: prot_cctkTargets
#     prot_cctkTargets
#        @$(MAKE) -C $(CCTK_ROOT) -f prot_cctk.mk
#
#     .PHONY: clean_prot_cctk
#     clean_prot_cctk:
#        @$(MAKE) -C $(CCTK_ROOT) -f prot_cctk.mk clean
#
# ---------------------------------------------------------------------------
#
# The following variables MUST be provided by the user, everything else should
# have a default value:
#     XCHG_ROOT         - Full path of root xchg_common directory.
#     CCTK_ROOT         - Full path of root prot_cctk directory.
#     CCTK_MAKE_TARGETS - List of targets to make.
#
#############################################################################

ifeq ($(CCTK_CONFIG_INCLUDE), )
$(error "CCTK_CONFIG_INCLUDE has not been set")
endif


# ---------------------------------------------------------------------------
# Setup generic build environment.
#
include $(XCHG_ROOT)/rules/xChgEnv.mk

#
# Setup prot_cctk build environment.
#
include $(CCTK_ROOT)/xcbuild/prot_cctk_env.mk



# ---------------------------------------------------------------------------
# Determine the set of modules to make.
#
# If no target is explicitly specified, the default behaviour is to build all
# targets specified by $(CCTK_ALL_TARGETS).
#
CCTK_VALID_TARGETS=$(CCTK_ALL_TARGETS) $(addprefix clean_,$(CCTK_ALL_TARGETS)) $(addprefix audit_,$(CCTK_ALL_TARGETS))

ifeq ($(strip $(filter $(MAKECMDGOALS),${CCTK_VALID_TARGETS})),)
# Either no target was specified, or a target that does not match a module name
# was specified (e.g. clean).
libTargets := $(CCTK_MAKE_TARGETS)
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
# prot_cctk targets to the global compiler options.
#
XCHG_C_GLOBAL_DEFS               += $(CCTK_COMPOSITE_PUBLIC_DEFS)
XCHG_C_GLOBAL_INCLUDE            += $(CCTK_COMPOSITE_PUBLIC_INCLUDE)


#
# Add the public includes paths for the BOS, ASSERT and STR module.
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


CCTK_MODULE_MAKEFILES := $(foreach target,$(libTargets),$(CCTK_DIR_$(target))/$(target).mk)
include $(CCTK_MODULE_MAKEFILES)



include $(XCHG_ROOT)/rules/xChgRules.mk


# ---------------------------------------------------------------------------
# Default target.
#
.PHONY: target $(CCTK_MAKE_TARGETS)
target: $(CCTK_MAKE_TARGETS)

#
# Target to clean a specific module, e.g. clean_bos.
#
.PHONY: $(addprefix clean_,$(CCTK_ALL_TARGETS))
$(addprefix clean_,$(CCTK_ALL_TARGETS)): clean_obj clean_dep clean_cctk_libs


.PHONY: clean_cctk_libs
clean_lib: clean_cctk_libs
clean_cctk_libs:
   $(RM) -f $(call XCHG_MAKE_LIB_NAME,$(libTargets))


# ---------------------------------------------------------------------------
# Generate the make syntax that defines the rules that are used to build the
# module libraries.
#

# Error check.
$(foreach target,$(libTargets),$(if $(XCHG_SRC_FILES_$(target)),,$(error "XCHG_SRC_FILES_$(target) is empty.")))

# Generate library targets.
$(foreach target,$(libTargets),$(eval $(call XCHG_ARCHIVE_TEMPLATE,$(target))))
