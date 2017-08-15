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
#    Filename: mtaTargets.mk
#    VSS Info:
#        $Revision: 13 $
#        $Date: 3/10/03 6:41p $
#
#############################################################################
#
#  This makefile provides the default list of make targets. It should only 
#  define variables and not have any rules in it.
#
#  This should be the only makefile edited when new modules are added to 
#  the MTA. It contains two target lists:
#     - Make targets (MTA_ALL_TARGETS).
#     - Target directories (MTA_DIR_xxx). For each target, this should specify
#       the relative directory location to the root directory.
#
#############################################################################


# ---------------------------------------------------------------------------
# Error check.
#

ifeq ($(CXC_MTA_DIR),)
$(error "CXC_MTA_DIR not set")
endif



# ---------------------------------------------------------------------------
# Target list.
#

MTA_ALL_TARGETS :=   \
   endptdrv
   
   
# ---------------------------------------------------------------------------
# Target directories. For each target listed above, this provides the 
# directory location of the target, relative to the root MTA directory.
#

MTA_DIR_endptdrv        := $(CXC_MTA_DIR)/endptdrv
