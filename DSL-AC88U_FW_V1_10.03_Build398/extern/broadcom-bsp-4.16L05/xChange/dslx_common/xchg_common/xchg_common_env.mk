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
#    Filename: xchg_env.mk
#
#############################################################################
#
# This makefile is used to setup the xchg_common VOB make environment. 
# It should only define variables and not have any rules in it.
#
# The intent of this makefile to is create two make variables:
#     - XCHG_COMPOSITE_PUBLIC_INCLUDE
#     - XCHG_COMPOSITE_PUBLIC_DEFS
#
# These define the _public_ include directories and defines associated with the
# specified make targets.
#
# This makefile may be included by a project makefile whose targets have
# dependencies on xchg_common targets.
#
#############################################################################


# ---------------------------------------------------------------------------
# Include target list.
#
include $(XCHG_ROOT)/xchg_common_targets.mk


# ---------------------------------------------------------------------------
# Error checking.
#

ifeq ($(XCHG_ROOT),)
$(error "XCHG_ROOT variable hasn't been set")
endif

ifeq ($(XCHG_MAKE_TARGETS),)
$(error "XCHG_MAKE_TARGETS variable hasn't been set")
endif

ifneq ($(filter-out $(XCHG_ALL_TARGETS),$(XCHG_MAKE_TARGETS)),)
$(error "XCHG_MAKE_TARGETS contains unknown targets - $(filter-out $(XCHG_ALL_TARGETS),$(XCHG_MAKE_TARGETS))")
endif


# ---------------------------------------------------------------------------
# Setup the global include search path for this VOB.
#

#
# This creates a list of public include directories for this VOB, based upon
# the list of make targets, XCHG_MAKE_TARGETS. Public include directories
# (XCHG_PUBLIC_INC_xxx) are published in xchg_common_targets.mk.
#
XCHG_COMPOSITE_PUBLIC_INCLUDE := $(foreach inc,$(addprefix XCHG_PUBLIC_INC_,$(XCHG_MAKE_TARGETS)),$($(inc)))


# ---------------------------------------------------------------------------
# Setup the "composite" defines for this VOB.
#

#
# This creates a list of public defines for this VOB, based upon
# the list of make targets, XCHG_MAKE_TARGETS. Public defines
# (XCHG_PUBLIC_DEF_xxx) are published in xchg_common_targets.mk.
#
XCHG_COMPOSITE_PUBLIC_DEFS := $(foreach def,$(addprefix XCHG_PUBLIC_DEFS_,$(XCHG_MAKE_TARGETS)),$($(def)))
