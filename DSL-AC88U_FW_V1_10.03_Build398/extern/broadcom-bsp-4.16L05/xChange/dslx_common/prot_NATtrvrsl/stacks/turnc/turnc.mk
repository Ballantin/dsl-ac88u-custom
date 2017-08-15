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
#                  This makefile is used to build the TURNC module.
#
##################################################################################


#
# NOTE: The following variable MUST be defined prior to calling this makefile
#       in order for it to work properly:
#
#     TURNC_ROOT:  The absolute path to the directory containing the TURNC module.
#                Example: 'export TURNC_ROOT := prot_NATtrvrsl/stacks/turnc'
#
#     TURNC_BUILD_CONFIG_INCLUDE: TURNC configuration includes
#
#     TURNC_BUILD_IMPORTED_INCLUDE: External includes needed by TURNC e.g. bos


#
# Reference the actual sources which are kept in a different makefile for ease of
# usage and readability.
#
include $(TURNC_ROOT)/turnc_target.mk
include $(TURNC_ROOT)/turnc_srcs.mk

#
# Export variables with the correct settings such that an application can
# use this information to build their own version.
#

export TURNC_BUILD_SRC_FILES = $(TURNC_ALL_SRCS)
export TURNC_BUILD_SRC_DIRS  = $(addprefix $(TURNC_ROOT)/, $(TURNC_ALL_DIRS))

#
# Add defines need for building TURNC.
#
export TURNC_BUILD_PRIVATE_DEFS    :=

#
# Construct full include path necessary for proper compilation of the TURNC.
#     TURNC_BUILD_CONFIG_INCLUDE - TURNC configuration includes
#     TURNC_BUILD_IMPORTED_INCLUDE - External includes needed by TURNC e.g. bos
#     TURNC_BUILD_LOCAL_INCLUDE - Private includes of the TURNC module

ifeq ($(TURNC_BUILD_CONFIG_INCLUDE), )
$(error "TURNC_BUILD_CONFIG_INCLUDE has not been set")
endif
ifeq ($(TURNC_BUILD_IMPORTED_INCLUDE), )
$(error "TURNC_BUILD_IMPORTED_INCLUDE has not been set")
endif

export TURNC_BUILD_LOCAL_INCLUDE = $(TURNC_ROOT)

export TURNC_COMPOSITE_BUILD_INCLUDE = $(TURNC_BUILD_CONFIG_INCLUDE) \
                                       $(TURNC_BUILD_IMPORTED_INCLUDE) \
                                       $(TURNC_BUILD_EXPORTED_INCLUDE) \
                                       $(TURNC_BUILD_LOCAL_INCLUDE)
