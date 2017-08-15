#****************************************************************************
#
# Copyright © 2000-2009 Broadcom Corporation
#
# This program is the proprietary software of Broadcom Corporation and/or its
# licensors, and may only be used, duplicated, modified or distributed
# pursuant to the terms and conditions of a separate, written license
# agreement executed between you and Broadcom (an "Authorized License").
# Except as set forth in an Authorized License, Broadcom grants no license
# (express or implied), right to use, or waiver of any kind with respect to
# the Software, and Broadcom expressly reserves all rights in and to the
# Software and all intellectual property rights therein.  IF YOU HAVE NO
# AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
# AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
# SOFTWARE.
#
# Except as expressly set forth in the Authorized License,
#
# 1.     This program, including its structure, sequence and organization,
#        constitutes the valuable trade secrets of Broadcom, and you shall
#        use all reasonable efforts to protect the confidentiality thereof,
#        and to use this information only in connection with your use of
#        Broadcom integrated circuit products.
#
# 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
#        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
#        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
#        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
#        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
#        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
#        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
#        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
#        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
#
# 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
#        OR ITS LICENSORS BE LIABLE FOR
#        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
#             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
#             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
#             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
#        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
#             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
#             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
#             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
#
#****************************************************************************
#
#  This makefile is used to build all the components of the 63148uni application
#
#****************************************************************************



# ---------------------------------------------------------------------------
# xchg_common defs.
#

#
# CXC_XCHG_TARGET_LIST specifies a list of xchg_common targets to build.
# If none are specified, the default list of targets required for an cablex
# application will be built.
#
#CXC_XCHG_TARGET_LIST :=


# ---------------------------------------------------------------------------
# voice_res_gw defs.
#

#
# CXC_VRG_BOARD_TYPE and CXC_VRG_VENDOR_BOARD_TYPE specify the board HAL to
# build (only one should be specified). CXC_VRG_BOARD_TYPE should be used
# for standard BRCM reference designs. If a custom vendor board is to be used,
# CXC_VRG_VENDOR_BOARD_TYPE should be specified. If a vendor target is specified,
# the target directory (VRG_DIR_xxx) and public inc path (VRG_PUBLIC_INC_xxx)
# must also be exported (where 'xxx' is the target name).
#
CXC_VRG_BOARD_TYPE := bcm63148uni
CXC_VRG_VENDOR_BOARD_TYPE :=


#
# CXC_VRG_TARGET_LIST specifies a list of standard voice_res_gw targets to build.
# If none are specified, the default list of targets required for an cablex
# application will be built.
#
#CXC_VRG_TARGET_LIST =

#
# CXC_VRG_VENDOR_TARGET_LIST specifies a list of custom vendor-specific
# voice_res_gw targets to build. If specified, the target directory (VRG_DIR_xxx)
# and public inc path (VRG_PUBLIC_INC_xxx) must also be exported for each custom
# target (where 'xxx' is the target name).
#
CXC_VRG_VENDOR_TARGET_LIST := pstnCtl

ifeq ($(strip $(BRCM_TPD_SUPPORT)),1)
CXC_VRG_VENDOR_TARGET_LIST += tpdCtl
endif

# ---------------------------------------------------------------------------
# xchg_drivers defs.
#

#
# CXC_XDRV_TARGET_LIST specifies a list of standard xchg_driver targets to be built.
#
ifeq ($(strip $(BRCM_63148_DECTTEST_SUPPORT)),1)
CXC_XDRV_TARGET_LIST = $(CXC_XDRV_63148_DEFAULT_TARGETS)     \
                       $(CXC_XDRV_COMMON_DEFAULT_TARGETS)   \
                       bcm63148_dectTest \
                       slicSim
else
CXC_XDRV_TARGET_LIST = $(CXC_XDRV_63148_DEFAULT_TARGETS)   \
                       $(CXC_XDRV_COMMON_DEFAULT_TARGETS)  \
                       slicSim
endif

ifeq ($(strip $(BRCM_VRG_SLACVENDOR_CFG_ZARLINK)),1)
CXC_XDRV_TARGET_LIST += bcm63xx_zarlink
else ifeq ($(strip $(BRCM_VRG_SLACVENDOR_CFG_SILABS)),1)
CXC_XDRV_TARGET_LIST += bcm63xx_silabs
else
CXC_XDRV_TARGET_LIST += bcm63xx_silabs bcm63xx_zarlink
endif

ifeq ($(strip $(BRCM_NTR_SUPPORT)),1)
CXC_XDRV_TARGET_LIST += bcm63148_ntr
endif

#
# CXC_XDRV_VENDOR_TARGET_LIST specifies a list of custom vendor-specific
# xchg_driver targets to build. If specified, the target directory (XDRV_DIR_xxx)
# and public inc path (XDRV_PUBLIC_INC_xxx) must also be exported for each custom
# target (where 'xxx' is the target name). In addition, XDRV_VENDOR_CHIP_TYPES
# should be exported if the custom drivers are for a non-standard chip type.
#
CXC_XDRV_VENDOR_TARGET_LIST :=


#
# Directory where the LDX hausware host library resides (this directory should be under ldx_apps/apps)
#
ifeq ($(strip $(BRCM_NONDIST_BUILD)),1)
CXC_XDRV_TARGET_LIST += debugSim

CXC_LDX_HAUSWARE_HOST_LIB_DIR = hausware_libs_gateway_nodist_arm
CXC_LDX_HAUSWARE_HOST_LIB_SUBDIR=lib/arm_uclibc_risc.arm.armv7.conly

export XCHG_ASM_GLOBAL_DEFS += BRCM_NONDIST_BUILD=1
else
CXC_LDX_HAUSWARE_HOST_LIB_DIR = hausware_libs_gateway_disthost_63xx_2fxs_fxo
CXC_LDX_HAUSWARE_HOST_LIB_SUBDIR=lib/mips_gnu_2_risc.mips.3306.asm
endif


# ---------------------------------------------------------------------------
# LDX defs.
#

ifeq ($(strip $(BRCM_63148_DECTTEST_SUPPORT)),1)
XCHG_C_GLOBAL_DEFS += DECT_TEST=1
endif

export XCHG_C_GLOBAL_DEFS += $(BLD_FLAGS_GLOBAL)

ifeq ($(strip $(BRCM_NONDIST_BUILD)),1)
#
# LDX app name
#
CXC_LDX_APP_NAME = hausware_libs_gateway_nodist_arm
#
# LDX app target
#
CXC_LDX_APP_TARGET = lib/arm_uclibc_risc.arm.armv7.conly
else
#
# LDX app name
#
CXC_LDX_APP_NAME = dspApp_flexible
#
# LDX app target
#
CXC_LDX_APP_TARGET = output
endif

# ---------------------------------------------------------------------------
# Include the generic cablex app rules file.
#
include ../rules/cxcAppRules.mk
