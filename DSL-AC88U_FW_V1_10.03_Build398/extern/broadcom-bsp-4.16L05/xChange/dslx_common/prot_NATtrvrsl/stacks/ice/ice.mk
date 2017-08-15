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
#                  This makefile is used to build the ICE module.
#
##################################################################################


#
# NOTE: The following variable MUST be defined prior to calling this makefile
#       in order for it to work properly:
#
#     ICE_ROOT:  The absolute path to the directory containing the NAT module(s)
#                Example: 'export ICE_ROOT := prot_NATtrvrsl/stacks/ice'
#
#     ICE_BUILD_CONFIG_INCLUDE: ICE configuration includes
#
#     ICE_BUILD_IMPORTED_INCLUDE: External includes needed by ICE e.g. bos
#


#
# Reference the actual sources which are kept in a different makefile for ease of
# usage and readability.
#
include $(ICE_ROOT)/ice_target.mk
include $(ICE_ROOT)/ice_srcs.mk

#
# Export variables with the correct settings such that an application can
# use this information to build their own version.
#

export ICE_BUILD_SRC_FILES = $(ICE_ALL_SRCS)
export ICE_BUILD_SRC_DIRS  = $(addprefix $(ICE_ROOT)/, $(ICE_ALL_DIRS))

#
# Add defines needed for building ICE.
#
export ICE_BUILD_PRIVATE_DEFS :=

#
# Construct full include path necessary for proper compilation of the ICE.
#     ICE_BUILD_CONFIG_INCLUDE   - ICE configuration includes
#     ICE_BUILD_IMPORTED_INCLUDE - External includes needed by ICE e.g. bos
#     ICE_BUILD_LOCAL_INCLUDE    - Private includes of the ICE module

ifeq ($(ICE_BUILD_CONFIG_INCLUDE), )
$(error "ICE_BUILD_CONFIG_INCLUDE has not been set")
endif
ifeq ($(ICE_BUILD_IMPORTED_INCLUDE), )
$(error "ICE_BUILD_IMPORTED_INCLUDE has not been set")
endif
export ICE_BUILD_LOCAL_INCLUDE = $(ICE_ROOT)/ice

export ICE_BUILD_COMPOSITE_INCLUDE = $(ICE_BUILD_CONFIG_INCLUDE)     \
                                     $(ICE_BUILD_IMPORTED_INCLUDE)   \
                                     $(ICE_BUILD_EXPORTED_INCLUDE)   \
                                     $(ICE_BUILD_LOCAL_INCLUDE)
