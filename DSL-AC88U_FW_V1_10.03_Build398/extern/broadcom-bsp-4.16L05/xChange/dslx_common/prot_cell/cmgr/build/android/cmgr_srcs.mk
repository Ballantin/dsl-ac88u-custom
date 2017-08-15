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
#                  This makefile is used to build the CMGR module.
#
##################################################################################

#
# List all the include paths used by CMGR library.
#
CMGR_INC_ROOT           := $(CCTK_INC_ROOT)/cmgr
CMGR_ALL_INCS           := $(CMGR_INC_ROOT)/cfginc                      \
                           $(CMGR_INC_ROOT)/inc/pub                     \
                           $(CMGR_INC_ROOT)/inc/priv                    \
                           $(CCTK_INC_ROOT)/cctk/cfginc                 \
                           $(CCTK_INC_ROOT)/cctk/inc                    \
                           $(XCHG_COMMON_INC_ROOT)/bos/publicInc        \
                           $(XCHG_COMMON_INC_ROOT)/assert/cfginc        \
                           $(XCHG_COMMON_INC_ROOT)/assert/inc           \
                           $(XCHG_COMMON_INC_ROOT)/str

#
# List all the sources used by CMGR library.
#
CMGR_SRC_ROOT           := $(CCTK_SRC_ROOT)/cmgr/src
CMGR_SRC                := $(CMGR_SRC_ROOT)/cmgrApi.c

CMGR_FSM_ROOT           := $(CMGR_SRC_ROOT)/fsm
CMGR_FSM                := $(CMGR_FSM_ROOT)/cmFsm.c                     \
                           $(CMGR_FSM_ROOT)/cmFsmCctk.c                 \
                           $(CMGR_FSM_ROOT)/cmFsmPhy.c                  \
                           $(CMGR_FSM_ROOT)/cmFsmTmr.c

CMGR_MAIN_ROOT          := $(CMGR_SRC_ROOT)/main
CMGR_MAIN               := $(CMGR_MAIN_ROOT)/cmCfg.c                    \
                           $(CMGR_MAIN_ROOT)/cmFeat.c                   \
                           $(CMGR_MAIN_ROOT)/cmMain.c                   \
                           $(CMGR_MAIN_ROOT)/cmMap.c                    \
                           $(CMGR_MAIN_ROOT)/cmPhy.c

CMGR_PROF_ROOT          := $(CMGR_SRC_ROOT)/prof
CMGR_PROF               := $(CMGR_PROF_ROOT)/cmProfCustom.c             \
                           $(CMGR_PROF_ROOT)/cmProfIms.c                \
                           $(CMGR_PROF_ROOT)/cmProfRfc3261.c            \
                           $(CMGR_PROF_ROOT)/cmProfStand.c

CMGR_RES_ROOT           := $(CMGR_SRC_ROOT)/res
CMGR_RES                := $(CMGR_RES_ROOT)/cmCdr.c                     \
                           $(CMGR_RES_ROOT)/cmDm.c                      \
                           $(CMGR_RES_ROOT)/cmSes.c                     \
                           $(CMGR_RES_ROOT)/cmTmr.c                     \
                           $(CMGR_RES_ROOT)/cmUsr.c

CMGR_UTIL_ROOT          := $(CMGR_SRC_ROOT)/util
CMGR_UTIL               := $(CMGR_UTIL_ROOT)/cmDate.c                   \
                           $(CMGR_UTIL_ROOT)/cmShow.c

CMGR_ALL_SRCS           := $(CMGR_SRC)                                  \
                           $(CMGR_FSM)                                  \
                           $(CMGR_MAIN)                                 \
                           $(CMGR_PROF)                                 \
                           $(CMGR_RES)                                  \
                           $(CMGR_UTIL)

