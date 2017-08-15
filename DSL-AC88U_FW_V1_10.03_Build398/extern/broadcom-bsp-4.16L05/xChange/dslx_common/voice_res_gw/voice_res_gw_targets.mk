#############################################################################
#
# Copyright © 2000 - 2007 Broadcom Corporation
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
#############################################################################
#
#    Filename: voice_res_gw_targets.mk
#
#############################################################################
#
# This makefile provides the default list of make targets. It should only
# define variables and not have any rules in it.
#
# This should be the only makefile edited when new modules are added to
# voice_res_gw. It contains three target lists:
#  - Make targets (VRG_ALL_TARGETS).
#  - Target directories (VRG_DIR_xxx). For each target, this should specify
#    the relative directory location to the root directory.
#  - Target public inc (VRG_PUBLIC_INC_xxx). For each target, this should
#    specify the module's public include directory.
#
#############################################################################

# ---------------------------------------------------------------------------
# Target lists. List of targets contained within the voice_res_gw VOB. This
# provides the default build target list.
#

# List of board HAL targets.
VRG_BOARD_HAL_TARGETS = \
   bcm6368vw            \
   bcm6368vw_fxo        \
   bcm6368vw_ve890_fxo  \
   bcm6368vw_pong       \
   bcm6368vw_mbg        \
   bcm6368mvngr         \
   bcm6362advng         \
   bcm6362advngr        \
   bcm6362uni           \
   bcm6816pvwm          \
   bcm6816sv            \
   bcm6816uni           \
   bcm6818uni           \
   bcm6838uni           \
   bcm6828uni           \
   bcm6368vw_ntr        \
   bcm6368vw_wb_dect_sitel_si3217x \
   bcm6368vw_wb_dect_sitel   \
   bcm6368vw_wb_dect_sitel_nofxs \
   bcm6368uni           \
   bcm68500uni          \
   bcm6328avng          \
   bcm6328uni           \
   bcm63268uni          \
   bcm63138uni          \
   bcm63148uni          \
   bcm63381uni          \
   bcm6848uni          \
   boardHalSim


# List of all targets.
VRG_ALL_TARGETS =             \
   casCtl                     \
   classStm                   \
   cmtdCtl                    \
   codec                      \
   hdsp                       \
   heartbeat                  \
   lhapi                      \
   endpt                      \
   pstnCtl                    \
   tpdCtl                     \
   $(VRG_BOARD_HAL_TARGETS)


# ---------------------------------------------------------------------------
# Target directories. For each target listed above, this provides the
# directory location of the target, relative to the root VOB directory.
#

VRG_DIR_boardHalSim          := $(VRG_ROOT)/boardHal/src/sim
VRG_DIR_casCtl               := $(VRG_ROOT)/casCtl
VRG_DIR_classStm             := $(VRG_ROOT)/classStm
VRG_DIR_cmtdCtl              := $(VRG_ROOT)/cmtdCtl
VRG_DIR_codec                := $(VRG_ROOT)/codec
VRG_DIR_hdsp                 := $(VRG_ROOT)/hdsp
VRG_DIR_heartbeat            := $(VRG_ROOT)/heartbeat
VRG_DIR_lhapi                := $(VRG_ROOT)/lhapi
VRG_DIR_endpt                := $(VRG_ROOT)/endpt
VRG_DIR_bcm6368vw_fxo         := $(VRG_ROOT)/boardHal/src/bcm6368vw_fxo
VRG_DIR_bcm6368vw_ve890_fxo   := $(VRG_ROOT)/boardHal/src/bcm6368vw_ve890_fxo
VRG_DIR_bcm6368vw_pong        := $(VRG_ROOT)/boardHal/src/bcm6368vw_pong
VRG_DIR_bcm6368vw_mbg         := $(VRG_ROOT)/boardHal/src/bcm6368vw_mbg
VRG_DIR_bcm6362advng          := $(VRG_ROOT)/boardHal/src/bcm6362advng
VRG_DIR_bcm6362advngr         := $(VRG_ROOT)/boardHal/src/bcm6362advngr
VRG_DIR_bcm6362uni            := $(VRG_ROOT)/boardHal/src/bcm6362uni
VRG_DIR_bcm6816pvwm           := $(VRG_ROOT)/boardHal/src/bcm6816pvwm
VRG_DIR_bcm6816sv             := $(VRG_ROOT)/boardHal/src/bcm6816sv
VRG_DIR_bcm6816uni            := $(VRG_ROOT)/boardHal/src/bcm6816uni
VRG_DIR_bcm6818uni            := $(VRG_ROOT)/boardHal/src/bcm6818uni
VRG_DIR_bcm6838uni            := $(VRG_ROOT)/boardHal/src/bcm6838uni
VRG_DIR_bcm6828uni            := $(VRG_ROOT)/boardHal/src/bcm6828uni
VRG_DIR_bcm63268uni           := $(VRG_ROOT)/boardHal/src/bcm63268uni
VRG_DIR_bcm63381uni           := $(VRG_ROOT)/boardHal/src/bcm63381uni
VRG_DIR_bcm6848uni           := $(VRG_ROOT)/boardHal/src/bcm6848uni
VRG_DIR_bcm68500uni           := $(VRG_ROOT)/boardHal/src/bcm68500uni
VRG_DIR_$(VRG_VENDOR_BOARD_HAL_TARGET) := boardHal/src/$(VRG_VENDOR_BOARD_HAL_TARGET)
VRG_DIR_pstnCtl               := $(VRG_ROOT)/pstnCtl
VRG_DIR_tpdCtl                := $(VRG_ROOT)/tpdCtl
VRG_DIR_bcm6368vw          := $(VRG_ROOT)/boardHal/src/bcm6368vw
VRG_DIR_bcm6368vw_ntr      := $(VRG_ROOT)/boardHal/src/bcm6368vw_ntr
VRG_DIR_bcm6368vw_wb_dect_sitel_si3217x   := $(VRG_ROOT)/boardHal/src/bcm6368vw_wb_dect_sitel_si3217x
VRG_DIR_bcm6368vw_wb_dect_sitel   := $(VRG_ROOT)/boardHal/src/bcm6368vw_wb_dect_sitel
VRG_DIR_bcm6368vw_wb_dect_sitel_nofxs   := $(VRG_ROOT)/boardHal/src/bcm6368vw_wb_dect_sitel_nofxs
VRG_DIR_bcm6368uni            := $(VRG_ROOT)/boardHal/src/bcm6368uni
VRG_DIR_bcm6368mvngr       := $(VRG_ROOT)/boardHal/src/bcm6368mvngr
VRG_DIR_bcm6328avng        := $(VRG_ROOT)/boardHal/src/bcm6328avng
VRG_DIR_bcm6328uni         := $(VRG_ROOT)/boardHal/src/bcm6328uni
VRG_DIR_bcm63138uni        := $(VRG_ROOT)/boardHal/src/bcm63138uni
VRG_DIR_bcm63148uni        := $(VRG_ROOT)/boardHal/src/bcm63148uni




# ---------------------------------------------------------------------------
# Target public includes. For each target listed above, this provides the
# public include directory for the module. By default, these will be added
# to the global search path for this VOB.
#

VRG_PUBLIC_INC_boardHal             := $(VRG_ROOT)/boardHal/inc $(XCHG_C_GLOBAL_INCLUDE)
VRG_PUBLIC_INC_boardHalSim          := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_casCtl               := $(VRG_DIR_casCtl)/inc $(VRG_DIR_casCtl)/cfginc
VRG_PUBLIC_INC_classStm             := $(VRG_DIR_classStm)
VRG_PUBLIC_INC_cmtdCtl              := $(VRG_DIR_cmtdCtl)
VRG_PUBLIC_INC_codec                := $(VRG_DIR_codec)
VRG_PUBLIC_INC_hdsp                 := $(VRG_DIR_hdsp)/inc $(VRG_DIR_hdsp)/cfginc
VRG_PUBLIC_INC_heartbeat            := $(VRG_DIR_heartbeat)/inc $(VRG_DIR_heartbeat)/cfginc
VRG_PUBLIC_INC_lhapi                := $(VRG_DIR_lhapi)/inc
VRG_PUBLIC_INC_endpt                := $(VRG_DIR_endpt)/inc $(VRG_DIR_endpt)/cfginc
VRG_PUBLIC_INC_bcm6368vw_fxo        := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368vw_ve890_fxo  := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368vw_pong       := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368vw_mbg        := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6362advng         := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6362advngr        := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6362uni           := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6816pvwm          := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6816sv            := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6818uni           := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6838uni           := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6828uni           := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6816uni           := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368vw         := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368vw_ntr     := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368vw_wb_dect_sitel_si3217x    := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368vw_wb_dect_sitel    := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368vw_wb_dect_sitel_nofxs    := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368uni           := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6368mvngr       := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6328avng        := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6328uni         := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm63268uni          := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm68500uni          := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm63138uni          := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm63148uni          := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm63381uni          := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_bcm6848uni           := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_$(VRG_VENDOR_BOARD_HAL_TARGET)  := $(VRG_PUBLIC_INC_boardHal)
VRG_PUBLIC_INC_pstnCtl           := $(VRG_DIR_pstnCtl)
VRG_PUBLIC_INC_tpdCtl            := $(VRG_DIR_tpdCtl)/inc $(VRG_DIR_tpdCtl)/cfginc

