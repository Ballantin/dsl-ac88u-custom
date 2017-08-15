##################################################################################
#
# Copyright © 2011 Broadcom Corporation
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
# List all the include paths used by CCTK library.
#
CCTK_ALL_INCS           := $(CCTK_INC_ROOT)/cctk/cfginc                 \
                           $(CCTK_INC_ROOT)/cctk/cfgsrc                 \
                           $(CCTK_INC_ROOT)/cctk/inc                    \
                           $(CCTK_INC_ROOT)/cctk/src/                   \
                           $(CCTK_INC_ROOT)/cctk/src/ce                 \
                           $(CCTK_INC_ROOT)/cctk/src/ce/cdr             \
                           $(CCTK_INC_ROOT)/cctk/src/ce/dlg             \
                           $(CCTK_INC_ROOT)/cctk/src/ce/fsm             \
                           $(CCTK_INC_ROOT)/cctk/src/ce/loc             \
                           $(CCTK_INC_ROOT)/cctk/src/ce/res             \
                           $(CCTK_INC_ROOT)/cctk/src/ce/uap             \
                           $(CCTK_INC_ROOT)/cctk/src/cfg                \
                           $(CCTK_INC_ROOT)/cctk/src/cfg/extcfg         \
                           $(CCTK_INC_ROOT)/cctk/src/main               \
                           $(CCTK_INC_ROOT)/cctk/src/mse                \
                           $(CCTK_INC_ROOT)/cctk/src/ns/br              \
                           $(CCTK_INC_ROOT)/cctk/src/ns/dlm             \
                           $(CCTK_INC_ROOT)/cctk/src/ns/nat             \
                           $(CCTK_INC_ROOT)/cctk/src/ns/sc              \
                           $(CCTK_INC_ROOT)/cctk/src/util               \
                           $(CCTK_INC_ROOT)/cctk/src/pm                 \
                           $(XCHG_COMMON_INC_ROOT)/bos/publicInc        \
                           $(XCHG_COMMON_INC_ROOT)/assert/cfginc        \
                           $(XCHG_COMMON_INC_ROOT)/assert/inc           \
                           $(XCHG_COMMON_INC_ROOT)/str                  \
                           $(NATT_INC_ROOT)/cfginc                      \
                           $(NATT_INC_ROOT)/stunc/inc                   \
                           $(NATT_INC_ROOT)/stunc/stun/inc              \
                           $(M5T_SCE_INC_ROOT)/M5TSce                   \
                           $(M5T_SCE_INC_ROOT)/M5TSdp                   \
                           $(M5T_SCE_INC_ROOT)/M5TSipUa                 \
                           $(M5T_SCE_INC_ROOT)/M5TFramework             \
                           $(M5T_SCE_INC_ROOT)/ThirdParty/regex


#
# List all the sources used by CCTK library.
#
# Note that is applicable, modules can be selected in/out depending on build
# configuration option.
#
CCTK_CE_ROOT            := $(CCTK_SRC_ROOT)/cctk/src/ce
CCTK_CE                 := $(CCTK_CE_ROOT)/ceMain.cpp

CCTK_CE_CDR_ROOT        := $(CCTK_SRC_ROOT)/cctk/src/ce/cdr
CCTK_CE_CDR             := $(CCTK_CE_CDR_ROOT)/ceCdr.cpp                \
                           $(CCTK_CE_CDR_ROOT)/ceCdrMgr.cpp

CCTK_CE_DLG_ROOT        := $(CCTK_SRC_ROOT)/cctk/src/ce/dlg
CCTK_CE_DLG             := $(CCTK_CE_DLG_ROOT)/ceDlg.cpp

CCTK_CE_FSM_ROOT        := $(CCTK_SRC_ROOT)/cctk/src/ce/fsm
CCTK_CE_FSM             := $(CCTK_CE_FSM_ROOT)/ceFsmBase.cpp            \
                           $(CCTK_CE_FSM_ROOT)/ceFsmCall.cpp            \
                           $(CCTK_CE_FSM_ROOT)/ceFsmCallIn.cpp          \
                           $(CCTK_CE_FSM_ROOT)/ceFsmCallOut.cpp         \
                           $(CCTK_CE_FSM_ROOT)/ceFsmHold.cpp            \
                           $(CCTK_CE_FSM_ROOT)/ceFsmPub.cpp             \
                           $(CCTK_CE_FSM_ROOT)/ceFsmReg.cpp             \
                           $(CCTK_CE_FSM_ROOT)/ceFsmSub.cpp             \
                           $(CCTK_CE_FSM_ROOT)/ceFsmXfer.cpp

CCTK_CE_LOC_ROOT        := $(CCTK_SRC_ROOT)/cctk/src/ce/loc
CCTK_CE_LOC             := $(CCTK_CE_LOC_ROOT)/ceLoc.cpp

CCTK_CE_RES_ROOT        := $(CCTK_SRC_ROOT)/cctk/src/ce/res
CCTK_CE_RES             := $(CCTK_CE_RES_ROOT)/ceResBase.cpp            \
                           $(CCTK_CE_RES_ROOT)/ceResCall.cpp            \
                           $(CCTK_CE_RES_ROOT)/ceResNot.cpp             \
                           $(CCTK_CE_RES_ROOT)/ceResSub.cpp             \
                           $(CCTK_CE_RES_ROOT)/ceResUser.cpp

CCTK_CE_UAP_ROOT        := $(CCTK_SRC_ROOT)/cctk/src/ce/uap
CCTK_CE_UAP             := $(CCTK_CE_UAP_ROOT)/ceUap.cpp

CCTK_CFG_ROOT           := $(CCTK_SRC_ROOT)/cctk/src/cfg
CCTK_CFG                := $(CCTK_CFG_ROOT)/cfgConvert.cpp              \
                           $(CCTK_CFG_ROOT)/cfgEngine.cpp               \
                           $(CCTK_CFG_ROOT)/cfgMain.cpp

CCTK_MAIN_ROOT          := $(CCTK_SRC_ROOT)/cctk/src/main
CCTK_MAIN               := $(CCTK_MAIN_ROOT)/cctkApi.cpp                \
                           $(CCTK_MAIN_ROOT)/cctkFramework.cpp

CCTK_MSE_ROOT           := $(CCTK_SRC_ROOT)/cctk/src/mse
CCTK_MSE                := $(CCTK_MSE_ROOT)/mseMain.cpp

CCTK_NS_BR_ROOT         := $(CCTK_SRC_ROOT)/cctk/src/ns/br
CCTK_NS_BR              := $(CCTK_NS_BR_ROOT)/nsBrApi.cpp               \
                           $(CCTK_NS_BR_ROOT)/nsBrCore.cpp

CCTK_NS_DLM_ROOT        := $(CCTK_SRC_ROOT)/cctk/src/ns/dlm
CCTK_NS_DLM             := $(CCTK_NS_DLM_ROOT)/nsDlmCore.cpp

CCTK_NS_NAT_ROOT        := $(CCTK_SRC_ROOT)/cctk/src/ns/nat
CCTK_NS_NAT             := $(CCTK_NS_NAT_ROOT)/nsNatApi.cpp             \
                           $(CCTK_NS_NAT_ROOT)/nsNatKa.cpp              \
                           $(CCTK_NS_NAT_ROOT)/nsNatStun.cpp

CCTK_NS_SC_ROOT         := $(CCTK_SRC_ROOT)/cctk/src/ns/sc
CCTK_NS_SC              := $(CCTK_NS_SC_ROOT)/nsScCore.cpp

CCTK_UTIL_ROOT          := $(CCTK_SRC_ROOT)/cctk/src/util
CCTK_UTIL               := $(CCTK_UTIL_ROOT)/utilData.cpp               \
                           $(CCTK_UTIL_ROOT)/utilMain.cpp               \
                           $(CCTK_UTIL_ROOT)/utilMem.cpp                \
                           $(CCTK_UTIL_ROOT)/utilStr.cpp                \
                           $(CCTK_UTIL_ROOT)/utilTel.cpp

CCTK_PM_ROOT            := $(CCTK_SRC_ROOT)/cctk/src/pm
CCTK_PM                 := $(CCTK_PM_ROOT)/pmObserver.cpp

CCTK_CFGSRC_ROOT        := $(CCTK_SRC_ROOT)/cctk/cfgsrc
CCTK_CFGSRC             := $(CCTK_CFGSRC_ROOT)/cctkExtCfg.cpp

CCTK_ALL_SRCS           := $(CCTK_CE)                                   \
                           $(CCTK_CE_CDR)                               \
                           $(CCTK_CE_DLG)                               \
                           $(CCTK_CE_FSM)                               \
                           $(CCTK_CE_LOC)                               \
                           $(CCTK_CE_RES)                               \
                           $(CCTK_CE_UAP)                               \
                           $(CCTK_CFG)                                  \
                           $(CCTK_MAIN)                                 \
                           $(CCTK_MSE)                                  \
                           $(CCTK_NS_BR)                                \
                           $(CCTK_NS_DLM)                               \
                           $(CCTK_NS_NAT)                               \
                           $(CCTK_NS_SC)                                \
                           $(CCTK_UTIL)                                 \
                           $(CCTK_PM)                                   \
                           $(CCTK_CFGSRC)

