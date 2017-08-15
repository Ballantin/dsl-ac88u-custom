##################################################################################
#
# Copyright (c) 2008-2012 Broadcom Corporation
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
# NOTE: The following variable MUST be defined prior to calling this makefile
#       in order for it to work properly:
#
#     CCTK_ROOT: The absolute path to the directory containing the 'cctk' module
#                Example: 'export CCTK_ROOT := prot_cctk'
#

#
# Reference the actual sources which are kept in a different makefile for ease of
# usage and readability.
#
include	$(CCTK_ROOT)/cctk/build/xchg/cctk_target.mk
include	$(CCTK_ROOT)/cctk/build/xchg/cctk_srcs.mk

#
# Export variables with the correct settings such that an application can
# use this information to build their own version.
#

export	CCTK_BUILD_SRC_FILES	=	$(CCTK_ALL_SRCS)
export	CCTK_BUILD_SRC_DIRS	=	$(addprefix $(CCTK_ROOT)/cctk/, $(CCTK_ALL_DIRS))


CCTK_BUILD_DEFS					=	MXD_RELEASE

export	CCTK_BUILD_PRIVATE_DEFS
#
# Construct full include path necessary for proper compilation of the STUNC.
#     CCTK_BUILD_CONFIG_INCLUDE - STUNC configuration includes
#     CCTK_BUILD_IMPORTED_INCLUDE - External includes needed by STUNC e.g. bos
#     CCTK_BUILD_LOCAL_INCLUDE - Private includes of the STUNC module

ifeq	($(CCTK_BUILD_CONFIG_INCLUDE), )
$(error "CCTK_BUILD_CONFIG_INCLUDE has not been set")
endif
ifeq	($(CCTK_BUILD_IMPORTED_INCLUDE), )
$(error "CCTK_BUILD_IMPORTED_INCLUDE has not been set")
endif

#
# Add all the include path necessary for proper compilation of the CCTK.
#
export	CCTK_BUILD_LOCAL_INCLUDE	=	\
											$(CCTK_ROOT)/cctk/cfgsrc									\
											$(CCTK_ROOT)/cctk/src/ce									\
											$(CCTK_ROOT)/cctk/src/ce/fsm								\
											$(CCTK_ROOT)/cctk/src/ce/res								\
											$(CCTK_ROOT)/cctk/src/ce/cdr								\
											$(CCTK_ROOT)/cctk/src/ce/dlg								\
											$(CCTK_ROOT)/cctk/src/ce/loc								\
											$(CCTK_ROOT)/cctk/src/ce/uap								\
											$(CCTK_ROOT)/cctk/src/cfg									\
											$(CCTK_ROOT)/cctk/src/cfg/extcfg							\
											$(CCTK_ROOT)/cctk/src/main									\
											$(CCTK_ROOT)/cctk/src/mse									\
											$(CCTK_ROOT)/cctk/src/ns/br								\
											$(CCTK_ROOT)/cctk/src/ns/dlm								\
											$(CCTK_ROOT)/cctk/src/ns/nat								\
											$(CCTK_ROOT)/cctk/src/ns/sc								\
											$(CCTK_ROOT)/cctk/src/ns/ice								\
											$(CCTK_ROOT)/cctk/src/util									\
											$(CCTK_ROOT)/cctk/src/pm

export	CCTK_BUILD_COMPOSITE_INCLUDE	=	$(CCTK_BUILD_CONFIG_INCLUDE)			\
														$(CCTK_BUILD_IMPORTED_INCLUDE)		\
														$(CCTK_BUILD_EXPORTED_INCLUDE)		\
														$(CCTK_BUILD_LOCAL_INCLUDE)
