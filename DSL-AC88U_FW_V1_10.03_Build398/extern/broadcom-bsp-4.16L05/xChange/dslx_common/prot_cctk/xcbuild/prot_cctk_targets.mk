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
#    Filename: prot_cctk_targets.mk
#
#############################################################################
#
# This makefile provides the default list of make targets. It should only
# define variables and not have any rules in it.
#
# This should be the only makefile edited when new modules are added to
# prot_cctk. It contains three target lists:
#  - Make targets (CCTK_ALL_TARGETS).
#  - Target directories (CCTK_DIR_xxx). For each target, this should specify
#    the relative directory location to the root directory.
#  - Target public inc (CCTK_PUBLIC_INC_xxx). For each target, this should
#    specify the module's public include directory.
#
#############################################################################

export CMGR_ROOT:=$(CCTK_ROOT)/cmgr
include $(CMGR_ROOT)/build/xchg/cmgr_target.mk
include $(CCTK_ROOT)/cctk/build/xchg/cctk_target.mk

# ---------------------------------------------------------------------------
# Target lists. List of targets contained within the prot_cctk VOB. This
# provides the default build target list.
#

# List of all targets.
CCTK_ALL_TARGETS := \
   cctk \
   cmgr

# ---------------------------------------------------------------------------
# Target directories. For each target listed above, this provides the
# directory location of the target, relative to the root VOB directory.
#

# CCTK targets.
CCTK_DIR_cctk                := $(CCTK_ROOT)/xcbuild
CCTK_DIR_cmgr                := $(CCTK_ROOT)/xcbuild
 
# ---------------------------------------------------------------------------
# Target public includes. For each target listed above, this provides the
# public include directory for the module. By default, these will be added
# to the global search path for this VOB.
#

CCTK_PUBLIC_INC_cctk   :=  $(CCTK_BUILD_EXPORTED_INCLUDE)
CCTK_PUBLIC_INC_cmgr   :=  $(CMGR_BUILD_EXPORTED_INCLUDE)
