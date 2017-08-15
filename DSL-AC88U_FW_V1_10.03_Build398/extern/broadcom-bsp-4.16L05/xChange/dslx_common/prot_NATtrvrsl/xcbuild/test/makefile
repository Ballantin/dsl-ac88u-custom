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
#    Filename: Makefile
#
#############################################################################
#
#  This makefile is a sample application makefile that builds the
#     stunc/turnc/ice/natt library
#
#############################################################################
ifneq ($(XCHG_OS),)
export TARGET_OS_TYPE:=$(XCHG_OS)
endif

# Helper Macro PWD - for getting current directory
ifeq ($(findstring win32,$(BUILD_HOST_TYPE)),win32)
# Win32 host.
PWD = $(shell cygpath -m -a .)
EXE = .exe
endif

ifeq ($(TARGET_OS_TYPE),VxWorks)
ifeq ($(XCHG_VXWORKS_ROOT),)
$(error "XCHG_VXWORKS_ROOT hasn't been set")
endif
endif

ifeq ($(TARGET_OS_TYPE),eCos)
ifeq ($(XCHG_ECOS_ROOT),)
$(error "XCHG_ECOS_ROOT hasn't been set")
endif
endif

##############################################
#	NATT Default SETTING
##############################################
ifeq ($(NATT_ROOT),)
export NATT_ROOT:=$(SOURCE_PROJ_DIR)/prot_NATtrvrsl
endif
ifeq ($(NATT_OS),)
export NATT_OS          := $(TARGET_OS_TYPE)
endif
ifeq ($(NATT_HOST_TYPE),)
export NATT_HOST_TYPE   := $(BUILD_HOST_TYPE)
endif
ifeq ($(NATT_MAKE_TARGETS),)
export NATT_MAKE_TARGETS := stunc turnc ice natt
endif

ifeq ($(TARGET_OS_TYPE),VxWorks)
export XCHG_C_GLOBAL_DEFS += XCBUILD_OS_VXWORKS
endif

ifeq ($(TARGET_OS_TYPE),eCos)
ifeq ($(BFC_V4_ROOT),)
$(error "BFC_V4_ROOT hasn't been set")
endif
export XCHG_C_GLOBAL_DEFS += XCBUILD_OS_ECOS
export XCHG_C_GLOBAL_INCLUDE += $(BFC_V4_ROOT)/Bfc/LibSupport/eCos/bcm3xx_ipv6_install/include
endif


##############################################
#	XCHG_COMMON SETTING
##############################################
# Set the XCHG Setting
export XCHG_ROOT:=$(SOURCE_PROJ_DIR)/xchg_common

ifeq ($(XCHG_OS),)
export XCHG_OS:=$(TARGET_OS_TYPE)
endif

export XCHG_HOST_TYPE:=$(BUILD_HOST_TYPE)

##############################################
#	LIB SETTING
##############################################

# makefile

#override lib output directorory
export NATT_LIB_DIR = $(PWD)

export NATT_CONFIG_INCLUDE := $(NATT_ROOT)/xcbuild/test/cfginc 

.PHONY: stunc turnc ice natt clean
all: stunc turnc ice natt


stunc:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk stunc

clean_stunc:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk clean_stunc

turnc:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk turnc

clean_turnc:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk clean_turnc

ice:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk ice

clean_ice:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk clean_ice

natt:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk natt

clean_natt:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk clean_natt

clean:
	$(MAKE) -C $(NATT_ROOT)/xcbuild -f prot_NATtrvrsl.mk clean

