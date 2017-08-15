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
#    Filename: SMAPI_target.mk
#
###########################################################################
#
# /SMAPI/SMAPI_target.mk
#
# This makefile is provides a list of SMAPI targets.
#
###########################################################################


#
# This is needed to get access to the tools that are used in the public defs
# below, e.g. $(TR).
#
#include $(SMAPI_ROOT)/rules/SMAPIEnv.mk


# ---------------------------------------------------------------------------
# Target list.
#

SMAPI_ALL_TARGETS := \
   srtp          
   
   
# ---------------------------------------------------------------------------
# Target directories. For each target listed above, this provides the 
# directory location of the target, relative to the SMAPI_ROOT directory.
#
   
SMAPI_DIR_srtp            := $(SMAPI_ROOT)/srtp

# ---------------------------------------------------------------------------
# Target public includes. For each target listed above, this provides the
# public include directory for the module. By default, these will be added
# to the global search path for this VOB.
#

SMAPI_PUBLIC_INC_srtp           := $(SMAPI_DIR_srtp)

# ---------------------------------------------------------------------------
# Target public defines. For each target listed above, this provides the
# public defines for the module. By default, these will be added
# to the global compiler options for this VOB.
#

SMAPI_PUBLIC_DEFS_srtp            :=

