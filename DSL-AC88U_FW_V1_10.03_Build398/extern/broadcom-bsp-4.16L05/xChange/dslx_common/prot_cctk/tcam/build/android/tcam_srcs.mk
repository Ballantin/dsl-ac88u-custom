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
#                  This makefile is used to build the TCAM module.
#
##################################################################################

#
# List all the include paths used by TCAM library.
#
TCAM_INC_ROOT           := $(CCTK_INC_ROOT)/tcam
TCAM_ALL_INCS           := $(TCAM_INC_ROOT)/cfginc                      \
                           $(TCAM_INC_ROOT)/inc/pub                     \
                           $(TCAM_INC_ROOT)/inc/priv                    \
                           $(CCTK_INC_ROOT)/cmgr/cfginc                 \
                           $(CCTK_INC_ROOT)/cmgr/inc/pub                \
                           $(CCTK_INC_ROOT)/cctk/cfginc                 \
                           $(CCTK_INC_ROOT)/cctk/inc                    \
                           $(XCHG_COMMON_INC_ROOT)/bos/publicInc        \
                           $(XCHG_COMMON_INC_ROOT)/assert/cfginc        \
                           $(XCHG_COMMON_INC_ROOT)/assert/inc           \
                           $(XCHG_COMMON_INC_ROOT)/str

#
# List all the sources used by TCAM library.
#
TCAM_SRC_ROOT           := $(CCTK_SRC_ROOT)/tcam/src
TCAM_SRC                := $(TCAM_SRC_ROOT)/tcamApi.c

TCAM_COM_ROOT           := $(TCAM_SRC_ROOT)/com
TCAM_COM                := $(TCAM_COM_ROOT)/tcamCom.c

TCAM_IFC_ROOT           := $(TCAM_SRC_ROOT)/ifc
TCAM_IFC                := $(TCAM_IFC_ROOT)/tcamIfcEpt.c                \
                           $(TCAM_IFC_ROOT)/tcamIfcCfg.c                \
                           $(TCAM_IFC_ROOT)/tcamIfcMsg.c                \
                           $(TCAM_IFC_ROOT)/tcamCb.c

TCAM_MAIN_ROOT          := $(TCAM_SRC_ROOT)/main
TCAM_MAIN               := $(TCAM_MAIN_ROOT)/tcamAsyncCmgrCb.c          \
                           $(TCAM_MAIN_ROOT)/tcamMap.c                  \
                           $(TCAM_MAIN_ROOT)/tcamMain.c

TCAM_RES_ROOT           := $(TCAM_SRC_ROOT)/res
TCAM_RES                := $(TCAM_RES_ROOT)/tcamRes.c

TCAM_ALL_SRCS           := $(TCAM_SRC)                                  \
                           $(TCAM_COM)                                  \
                           $(TCAM_IFC)                                  \
                           $(TCAM_MAIN)                                 \
                           $(TCAM_RES)

