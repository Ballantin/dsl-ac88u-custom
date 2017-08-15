#############################################################################
#
# Copyright © 2000-2008 Broadcom Corporation
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
#############################################################################
#
#    Filename: xchg_drivers_targets.mk
#
#############################################################################
#
# This makefile provides the default list of make targets. It should only
# define variables and not have any rules in it.
#
# This should be the only makefile edited when new modules are added to
# xchg_drivers. It contains three target lists:
#  - Make targets (XDRV_ALL_TARGETS).
#  - Target directories (XDRV_DIR_xxx). For each target, this should specify
#    the relative directory location to the root directory.
#  - Target public inc (XDRV_PUBLIC_INC_xxx). For each target, this should
#    specify the module's public include directory.
#
#############################################################################


# ---------------------------------------------------------------------------
# Target lists. List of targets contained within the xchg_drivers VOB. This
# provides the default build target list.
#

# List of supported chip types. If chip 'bcmxxx' is supported, the associated
# targets listed below should be named 'bcmxxx_yyy'.
XDRV_SUPPORTED_CHIPS := bcm6358  \
                        bcm6368  \
                        bcm6816  \
                        bcm6818  \
                        bcm6838  \
                        bcm6828  \
                        bcm6362  \
                        bcm6328  \
                        bcm63268 \
                        bcm68500 \
                        bcm63381 \
                        bcm6848  \
                        bcm63138 \
                        bcm63148

# 6358 targets.
XDRV_BCM6358_TARGETS := \
   bcm6358_boot         \
   bcm6358_dlb          \
   bcm6358_handshake    \
   bcm6358_pcm          \
   bcm6358_led          \
   bcm6358_daaSi3050    \
   bcm6358_daale88311   \
   bcm6358_relay

# 6368 targets.
XDRV_BCM6368_TARGETS := \
   bcm6368_ntr          \
   bcm6368_boot         \
   bcm6368_dlb          \
   bcm6368_handshake    \
   bcm6368_pcm          \
   bcm6368_led          \
   bcm6368_daaSi3050    \
   bcm6368_relay

# 6816 targets.
XDRV_BCM6816_TARGETS := \
   bcm6816_apm          \
   bcm6816_boot         \
   bcm6816_clock        \
   bcm6816_dlb          \
   bcm6816_handshake    \
   bcm6816_led          \
   bcm6816_slicl9500    \
   bcm6816_slicl9520    \
   bcm6816_slicl9530    \
   bcm6816_slicsi3230   \
   bcm6816_slicsi3239   \
   bcm6816_tpdl9500     \
   bcm6816_tpdl9520     \
   bcm6816_tpdl9530     \
   bcm6816_tpdsi3230    \
   bcm6816_tpdsi3239

# 6818 targets.
XDRV_BCM6818_TARGETS := \
   bcm6818_apm          \
   bcm6818_boot         \
   bcm6818_clock        \
   bcm6818_dlb          \
   bcm6818_handshake    \
   bcm6818_led          \
   bcm6818_slicl9500    \
   bcm6818_slicl9520    \
   bcm6818_slicl9530    \
   bcm6818_slicl9540    \
   bcm6818_slicsi3230   \
   bcm6818_slicsi3239   \
   bcm6818_slicsi32392  \
   bcm6818_tpdl9500     \
   bcm6818_tpdl9520     \
   bcm6818_tpdl9530     \
   bcm6818_tpdsi3230    \
   bcm6818_tpdsi3239

# 6838 targets.
XDRV_BCM6838_TARGETS := \
   bcm6838_apm          \
   bcm6838_boot         \
   bcm6838_clock        \
   bcm6838_dlb          \
   bcm6838_handshake    \
   bcm6838_led          \
   bcm6838_slicl9500    \
   bcm6838_slicl9520    \
   bcm6838_slicl9530    \
   bcm6838_slicl9540    \
   bcm6838_slicsi3230   \
   bcm6838_slicsi3239   \
   bcm6838_slicsi32392  \
   bcm6838_tpdl9500     \
   bcm6838_tpdl9520     \
   bcm6838_tpdl9530     \
   bcm6838_tpdl9540     \
   bcm6838_tpdsi3230    \
   bcm6838_tpdsi32392   \
   bcm6838_tpdsi3239

# 6828 targets.
XDRV_BCM6828_TARGETS := \
   bcm6828_apm          \
   bcm6828_boot         \
   bcm6828_clock        \
   bcm6828_dlb          \
   bcm6828_handshake    \
   bcm6828_led          \
   bcm6828_slicl9500    \
   bcm6828_slicl9520    \
   bcm6828_slicl9530    \
   bcm6828_slicsi3230   \
   bcm6828_slicsi3239   \
   bcm6828_slicsi32392  \
   bcm6828_tpdl9500     \
   bcm6828_tpdl9520     \
   bcm6828_tpdl9530     \
   bcm6828_tpdsi3230    \
   bcm6828_tpdsi3239

# 6362 targets.
XDRV_BCM6362_TARGETS := \
   bcm6362_ntr          \
   bcm6362_boot         \
   bcm6362_dlb          \
   bcm6362_handshake    \
   bcm6362_pcm          \
   bcm6362_led          \
   bcm6362_daaSi3050    \
   bcm6362_relay        \
   bcm6362_dectTest

# 6328 targets.
XDRV_BCM6328_TARGETS := \
   bcm6328_ntr          \
   bcm6328_boot         \
   bcm6328_dlb          \
   bcm6328_handshake    \
   bcm6328_pcm          \
   bcm6328_led          \
   bcm6362_relay

# 63268 targets.
XDRV_BCM63268_TARGETS := \
   bcm63268_ntr          \
   bcm63268_boot         \
   bcm63268_dlb          \
   bcm63268_handshake    \
   bcm63268_pcm          \
   bcm63268_led          \
   bcm63268_daaSi3050    \
   bcm63268_relay        \
   bcm63268_dectTest

# 68500 targets.
XDRV_BCM68500_TARGETS := \
   bcm68500_boot         \
   bcm68500_dlb          \
   bcm68500_handshake    \
   bcm68500_pcm          \
   bcm68500_daaSi3050    \
   bcm68500_ceva

# 63138 targets.
XDRV_BCM63138_TARGETS := \
   bcm63138_pcm          \
   bcm63138_led          \
   bcm63138_relay        \
   bcm63138_dectTest

# 63148 targets.
XDRV_BCM63148_TARGETS := \
   bcm63148_pcm          \
   bcm63148_led          \
   bcm63148_relay        \
   bcm63148_dectTest
   
# 63381 targets.
XDRV_BCM63381_TARGETS := \
   bcm63381_ntr          \
   bcm63381_dlb          \
   bcm63381_pcm          \
   bcm63381_led          \
   bcm63381_daaSi3050    \
   bcm63381_relay

# 6848 targets.
XDRV_BCM6848_TARGETS := \
   bcm6848_ntr          \
   bcm6848_dlb          \
   bcm6848_pcm          \
   bcm6848_led          \
   bcm6848_daaSi3050    \
   bcm6848_relay

# 63xx (dsl modem) targets.
XDRV_BCM63xx_TARGETS := \
   bcm63xx_intCtrl      \
   bcm63xx_timer        \
   bcm63xx_resetCallout \
   bcm63xx_silabs       \
   bcm63xx_zarlink      \
   bcm63xx_handshake

# Common architecture-independent targets.
XDRV_COMMON_TARGETS :=  \
   casDriver            \
   dbgCom               \
   circBuf              \
   debugCmtInterface    \
   debugZspInterface    \
   flowCtrl             \
   ipcSharedMem

# Targets specific to the MIPS architecture.
XDRV_MIPS_TARGETS :=    \
   cacheMips            \
   cp0Mips              \
   exceptionMips        \
   exceptionHdlrMips    \
   exceptionHdlrCmtMips \
   globalInterruptMips  \
   intCtrlMips          \
   itpcMips             \
   spinLockMips         \
   timerMips

# Targets specific to the ARM architecture.
XDRV_ARM_TARGETS :=     \
   cacheArm             \
   globalInterruptArm   \
   spinLockArm          \
   timerArm             \
   neon

# Stub targets.
XDRV_STUB_TARGETS :=    \
   apmSim               \
   ledSim               \
   casDriverSim         \
   debugSim             \
   dlbStub              \
   ipcSim               \
   slicSim

# Targets specific to the ZSP architecture.
XDRV_ZSP_TARGETS :=     \
   zspTimer


# List of all targets.
XDRV_ALL_TARGETS :=           \
   $(XDRV_BCM6358_TARGETS)    \
   $(XDRV_BCM6368_TARGETS)    \
   $(XDRV_BCM6816_TARGETS)    \
   $(XDRV_BCM6818_TARGETS)    \
   $(XDRV_BCM6838_TARGETS)    \
   $(XDRV_BCM6828_TARGETS)    \
   $(XDRV_BCM6362_TARGETS)    \
   $(XDRV_BCM6328_TARGETS)    \
   $(XDRV_BCM63268_TARGETS)   \
   $(XDRV_BCM68500_TARGETS)   \
   $(XDRV_BCM63138_TARGETS)   \
   $(XDRV_BCM63148_TARGETS)   \
   $(XDRV_BCM63xx_TARGETS)    \
   $(XDRV_BCM63381_TARGETS)   \
   $(XDRV_BCM6848_TARGETS)    \
   $(XDRV_COMMON_TARGETS)     \
   $(XDRV_MIPS_TARGETS)       \
   $(XDRV_ARM_TARGETS)        \
   $(XDRV_STUB_TARGETS)       \
   $(XDRV_ZSP_TARGETS)


# ---------------------------------------------------------------------------
# Target directories. For each target listed above, this provides the
# directory location of the target, relative to the root VOB directory.
#

# 6358 targets.
XDRV_DIR_bcm6358_boot         := $(XDRV_ROOT)/bcm6358/boot
XDRV_DIR_bcm6358_dlb          := $(XDRV_ROOT)/bcm6358/dlb
XDRV_DIR_bcm6358_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6358_slicagere    := $(XDRV_ROOT)/bcm6358/slic/agere
XDRV_DIR_bcm6358_pcm          := $(XDRV_ROOT)/bcm6358/pcm
XDRV_DIR_bcm6358_led          := $(XDRV_ROOT)/bcm6358/leds
XDRV_DIR_bcm6358_daasi3050    := $(XDRV_ROOT)/bcm6358/daa/si3050
XDRV_DIR_bcm6358_daale88311   := $(XDRV_ROOT)/bcm6358/daa/le88311
XDRV_DIR_bcm6358_relay        := $(XDRV_ROOT)/bcm6358/relay

# 6368 targets.
XDRV_DIR_bcm6368_boot         := $(XDRV_ROOT)/bcm6368/boot
XDRV_DIR_bcm6368_dlb          := $(XDRV_ROOT)/bcm6368/dlb
XDRV_DIR_bcm6368_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6368_slicagere    := $(XDRV_ROOT)/bcm6368/slic/agere
XDRV_DIR_bcm6368_pcm          := $(XDRV_ROOT)/bcm6368/pcm
XDRV_DIR_bcm6368_ntr          := $(XDRV_ROOT)/bcm6368/ntr
XDRV_DIR_bcm6368_led          := $(XDRV_ROOT)/bcm6368/leds
XDRV_DIR_bcm6368_daasi3050    := $(XDRV_ROOT)/bcm6368/daa/si3050
XDRV_DIR_bcm6368_relay        := $(XDRV_ROOT)/bcm6368/relay

# 6816 targets.
XDRV_DIR_bcm6816_apm          := $(XDRV_ROOT)/bcm6816/apm
XDRV_DIR_bcm6816_boot         := $(XDRV_ROOT)/bcm6816/boot
XDRV_DIR_bcm6816_clock        := $(XDRV_ROOT)/bcm6816/clock
XDRV_DIR_bcm6816_dlb          := $(XDRV_ROOT)/bcm6816/dlb
XDRV_DIR_bcm6816_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6816_slicl9500    := $(XDRV_ROOT)/bcm6816/slic/l9500
XDRV_DIR_bcm6816_slicl9520    := $(XDRV_ROOT)/bcm6816/slic/l9520
XDRV_DIR_bcm6816_slicl9530    := $(XDRV_ROOT)/bcm6816/slic/l9530
XDRV_DIR_bcm6816_slicsi3230   := $(XDRV_ROOT)/bcm6816/slic/si3230
XDRV_DIR_bcm6816_slicsi3239   := $(XDRV_ROOT)/bcm6816/slic/si3239
XDRV_DIR_bcm6816_tpdl9500     := $(XDRV_ROOT)/bcm6816/tpd/l9500
XDRV_DIR_bcm6816_tpdl9520     := $(XDRV_ROOT)/bcm6816/tpd/l9520
XDRV_DIR_bcm6816_tpdl9530     := $(XDRV_ROOT)/bcm6816/tpd/l9530
XDRV_DIR_bcm6816_tpdsi3230    := $(XDRV_ROOT)/bcm6816/tpd/si3230
XDRV_DIR_bcm6816_tpdsi3239    := $(XDRV_ROOT)/bcm6816/tpd/si3239

# 6818 targets.
XDRV_DIR_bcm6818_apm          := $(XDRV_ROOT)/bcm6818/apm
XDRV_DIR_bcm6818_boot         := $(XDRV_ROOT)/bcm6818/boot
XDRV_DIR_bcm6818_clock        := $(XDRV_ROOT)/bcm6818/clock
XDRV_DIR_bcm6818_dlb          := $(XDRV_ROOT)/bcm6818/dlb
XDRV_DIR_bcm6818_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6818_slicl9500    := $(XDRV_ROOT)/bcm6818/slic/l9500
XDRV_DIR_bcm6818_slicl9520    := $(XDRV_ROOT)/bcm6818/slic/l9520
XDRV_DIR_bcm6818_slicl9530    := $(XDRV_ROOT)/bcm6818/slic/l9530
XDRV_DIR_bcm6818_slicl9540    := $(XDRV_ROOT)/bcm6818/slic/l9540
XDRV_DIR_bcm6818_slicsi3230   := $(XDRV_ROOT)/bcm6818/slic/si3230
XDRV_DIR_bcm6818_slicsi3239   := $(XDRV_ROOT)/bcm6818/slic/si3239
XDRV_DIR_bcm6818_slicsi32392  := $(XDRV_ROOT)/bcm6818/slic/si32392
XDRV_DIR_bcm6818_tpdl9500     := $(XDRV_ROOT)/bcm6818/tpd/l9500
XDRV_DIR_bcm6818_tpdl9520     := $(XDRV_ROOT)/bcm6818/tpd/l9520
XDRV_DIR_bcm6818_tpdl9530     := $(XDRV_ROOT)/bcm6818/tpd/l9530
XDRV_DIR_bcm6818_tpdsi3230    := $(XDRV_ROOT)/bcm6818/tpd/si3230
XDRV_DIR_bcm6818_tpdsi3239    := $(XDRV_ROOT)/bcm6818/tpd/si3239

# 6838 targets.
XDRV_DIR_bcm6838_apm          := $(XDRV_ROOT)/bcm6838/apm
XDRV_DIR_bcm6838_boot         := $(XDRV_ROOT)/bcm6838/boot
XDRV_DIR_bcm6838_clock        := $(XDRV_ROOT)/bcm6838/clock
XDRV_DIR_bcm6838_dlb          := $(XDRV_ROOT)/bcm6838/dlb
XDRV_DIR_bcm6838_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6838_slicl9500    := $(XDRV_ROOT)/bcm6838/slic/l9500
XDRV_DIR_bcm6838_slicl9520    := $(XDRV_ROOT)/bcm6838/slic/l9520
XDRV_DIR_bcm6838_slicl9530    := $(XDRV_ROOT)/bcm6838/slic/l9530
XDRV_DIR_bcm6838_slicl9540    := $(XDRV_ROOT)/bcm6838/slic/l9540
XDRV_DIR_bcm6838_slicsi3230   := $(XDRV_ROOT)/bcm6838/slic/si3230
XDRV_DIR_bcm6838_slicsi3239   := $(XDRV_ROOT)/bcm6838/slic/si3239
XDRV_DIR_bcm6838_slicsi32392  := $(XDRV_ROOT)/bcm6838/slic/si32392
XDRV_DIR_bcm6838_tpdl9500     := $(XDRV_ROOT)/bcm6838/tpd/l9500
XDRV_DIR_bcm6838_tpdl9520     := $(XDRV_ROOT)/bcm6838/tpd/l9520
XDRV_DIR_bcm6838_tpdl9530     := $(XDRV_ROOT)/bcm6838/tpd/l9530
XDRV_DIR_bcm6838_tpdl9540     := $(XDRV_ROOT)/bcm6838/tpd/l9540
XDRV_DIR_bcm6838_tpdsi3230    := $(XDRV_ROOT)/bcm6838/tpd/si3230
XDRV_DIR_bcm6838_tpdsi32392   := $(XDRV_ROOT)/bcm6838/tpd/si32392
XDRV_DIR_bcm6838_tpdsi3239    := $(XDRV_ROOT)/bcm6838/tpd/si3239

# 6828 targets.
XDRV_DIR_bcm6828_apm          := $(XDRV_ROOT)/bcm6828/apm
XDRV_DIR_bcm6828_boot         := $(XDRV_ROOT)/bcm6828/boot
XDRV_DIR_bcm6828_clock        := $(XDRV_ROOT)/bcm6828/clock
XDRV_DIR_bcm6828_dlb          := $(XDRV_ROOT)/bcm6828/dlb
XDRV_DIR_bcm6828_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6828_slicl9500    := $(XDRV_ROOT)/bcm6828/slic/l9500
XDRV_DIR_bcm6828_slicl9520    := $(XDRV_ROOT)/bcm6828/slic/l9520
XDRV_DIR_bcm6828_slicl9530    := $(XDRV_ROOT)/bcm6828/slic/l9530
XDRV_DIR_bcm6828_slicsi3230   := $(XDRV_ROOT)/bcm6828/slic/si3230
XDRV_DIR_bcm6828_slicsi3239   := $(XDRV_ROOT)/bcm6828/slic/si3239
XDRV_DIR_bcm6828_slicsi32392  := $(XDRV_ROOT)/bcm6828/slic/si32392
XDRV_DIR_bcm6828_tpdl9500     := $(XDRV_ROOT)/bcm6828/tpd/l9500
XDRV_DIR_bcm6828_tpdl9520     := $(XDRV_ROOT)/bcm6828/tpd/l9520
XDRV_DIR_bcm6828_tpdl9530     := $(XDRV_ROOT)/bcm6828/tpd/l9530
XDRV_DIR_bcm6828_tpdsi3230    := $(XDRV_ROOT)/bcm6828/tpd/si3230
XDRV_DIR_bcm6828_tpdsi3239    := $(XDRV_ROOT)/bcm6828/tpd/si3239

# 6362 targets.
XDRV_DIR_bcm6362_boot         := $(XDRV_ROOT)/bcm6362/boot
XDRV_DIR_bcm6362_dlb          := $(XDRV_ROOT)/bcm6362/dlb
XDRV_DIR_bcm6362_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6362_slicagere    := $(XDRV_ROOT)/bcm6362/slic/agere
XDRV_DIR_bcm6362_pcm          := $(XDRV_ROOT)/bcm6362/pcm
XDRV_DIR_bcm6362_ntr          := $(XDRV_ROOT)/bcm6362/ntr
XDRV_DIR_bcm6362_led          := $(XDRV_ROOT)/bcm6362/leds
XDRV_DIR_bcm6362_daasi3050    := $(XDRV_ROOT)/bcm6362/daa/si3050
XDRV_DIR_bcm6362_relay        := $(XDRV_ROOT)/bcm6362/relay
XDRV_DIR_bcm6362_dectTest     := $(XDRV_ROOT)/bcm6362/dect/dectTest

# 6328 targets.
XDRV_DIR_bcm6328_boot         := $(XDRV_ROOT)/bcm6328/boot
XDRV_DIR_bcm6328_dlb          := $(XDRV_ROOT)/bcm6328/dlb
XDRV_DIR_bcm6328_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6328_slicagere    := $(XDRV_ROOT)/bcm6328/slic/agere
XDRV_DIR_bcm6328_pcm          := $(XDRV_ROOT)/bcm6328/pcm
XDRV_DIR_bcm6328_ntr          := $(XDRV_ROOT)/bcm6328/ntr
XDRV_DIR_bcm6328_led          := $(XDRV_ROOT)/bcm6328/leds
XDRV_DIR_bcm6328_relay        := $(XDRV_ROOT)/bcm6328/relay

# 63268 targets.
XDRV_DIR_bcm63268_boot        := $(XDRV_ROOT)/bcm63268/boot
XDRV_DIR_bcm63268_dlb         := $(XDRV_ROOT)/bcm63268/dlb
XDRV_DIR_bcm63268_handshake   := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm63268_slicagere   := $(XDRV_ROOT)/bcm63268/slic/agere
XDRV_DIR_bcm63268_pcm         := $(XDRV_ROOT)/bcm63268/pcm
XDRV_DIR_bcm63268_ntr         := $(XDRV_ROOT)/bcm63268/ntr
XDRV_DIR_bcm63268_led         := $(XDRV_ROOT)/bcm63268/leds
XDRV_DIR_bcm63268_daasi3050   := $(XDRV_ROOT)/bcm63268/daa/si3050
XDRV_DIR_bcm63268_relay       := $(XDRV_ROOT)/bcm63268/relay
XDRV_DIR_bcm63268_dectTest    := $(XDRV_ROOT)/bcm63268/dect/dectTest

# 68500 targets.
XDRV_DIR_bcm68500_boot        := $(XDRV_ROOT)/bcm68500/boot
XDRV_DIR_bcm68500_dlb         := $(XDRV_ROOT)/bcm68500/dlb
XDRV_DIR_bcm68500_handshake   := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm68500_slicagere   := $(XDRV_ROOT)/bcm68500/slic/agere
XDRV_DIR_bcm68500_pcm         := $(XDRV_ROOT)/bcm68500/pcm
XDRV_DIR_bcm68500_ntr         := $(XDRV_ROOT)/bcm68500/ntr
XDRV_DIR_bcm68500_daasi3050   := $(XDRV_ROOT)/bcm68500/daa/si3050
XDRV_DIR_bcm68500_ceva        := $(XDRV_ROOT)/bcm68500/ceva

# 63138 targets.
XDRV_DIR_bcm63138_pcm         := $(XDRV_ROOT)/bcm63138/pcm
XDRV_DIR_bcm63138_led         := $(XDRV_ROOT)/bcm63138/leds
XDRV_DIR_bcm63138_relay       := $(XDRV_ROOT)/bcm63138/relay
XDRV_DIR_bcm63138_dectTest    := $(XDRV_ROOT)/bcm63138/dect/dectTest

# 63148 targets.
XDRV_DIR_bcm63148_pcm         := $(XDRV_ROOT)/bcm63148/pcm
XDRV_DIR_bcm63148_led         := $(XDRV_ROOT)/bcm63148/leds
XDRV_DIR_bcm63148_relay       := $(XDRV_ROOT)/bcm63148/relay
XDRV_DIR_bcm63148_dectTest    := $(XDRV_ROOT)/bcm63148/dect/dectTest

# 63xx targets.
XDRV_DIR_bcm63xx_intCtrl      := $(XDRV_ROOT)/bcm63xx/intCtrl
XDRV_DIR_bcm63xx_timer        := $(XDRV_ROOT)/bcm63xx/timer
XDRV_DIR_bcm63xx_resetCallout := $(XDRV_ROOT)/bcm63xx/resetCallout
XDRV_DIR_bcm63xx_silabs       := $(XDRV_ROOT)/bcm63xx/silabs
XDRV_DIR_bcm63xx_zarlink      := $(XDRV_ROOT)/bcm63xx/zarlink
XDRV_DIR_bcm63xx_handshake    := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm63xx_pcm          := $(XDRV_ROOT)/bcm63xx/pcm

# 63381 targets.
XDRV_DIR_bcm63381_boot        := $(XDRV_ROOT)/bcm63381/boot
XDRV_DIR_bcm63381_dlb         := $(XDRV_ROOT)/bcm63381/dlb
XDRV_DIR_bcm63381_handshake   := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm63381_slicagere   := $(XDRV_ROOT)/bcm63381/slic/agere
XDRV_DIR_bcm63381_pcm         := $(XDRV_ROOT)/bcm63381/pcm
XDRV_DIR_bcm63381_ntr         := $(XDRV_ROOT)/bcm63381/ntr
XDRV_DIR_bcm63381_led         := $(XDRV_ROOT)/bcm63381/leds
XDRV_DIR_bcm63381_daasi3050   := $(XDRV_ROOT)/bcm63381/daa/si3050
XDRV_DIR_bcm63381_relay       := $(XDRV_ROOT)/bcm63381/relay

# 63381 targets.
XDRV_PUBLIC_INC_bcm63381_boot           := $(XDRV_DIR_bcm63381_boot) \
                                           $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm63381_dlb            := $(XDRV_DIR_bcm63381_dlb)/inc $(XDRV_DIR_bcm63381_dlb)/cfginc
XDRV_PUBLIC_INC_bcm63381_handshake      := $(XDRV_DIR_bcm63381_handshake)
XDRV_PUBLIC_INC_bcm63381_slicagere      := $(XDRV_DIR_bcm63381_slicagere)
XDRV_PUBLIC_INC_bcm63381_pcm            := $(XDRV_DIR_bcm63381_pcm) \
                                           $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm63381_ntr            := $(XDRV_DIR_bcm63381_ntr)
XDRV_PUBLIC_INC_bcm63381_led            := $(XDRV_DIR_bcm63381_led)
XDRV_PUBLIC_INC_bcm63381_daasi3050      := $(XDRV_DIR_bcm63381_daasi3050)
XDRV_PUBLIC_INC_bcm63381_daale89316     := $(XDRV_DIR_bcm63381_daale89316)
XDRV_PUBLIC_INC_bcm63381_relay          := $(XDRV_DIR_bcm63381_relay)

# 6848 targets.
XDRV_DIR_bcm6848_boot        := $(XDRV_ROOT)/bcm6848/boot
XDRV_DIR_bcm6848_dlb         := $(XDRV_ROOT)/bcm6848/dlb
XDRV_DIR_bcm6848_handshake   := $(XDRV_ROOT)/bcm63xx/handshake
XDRV_DIR_bcm6848_slicagere   := $(XDRV_ROOT)/bcm6848/slic/agere
XDRV_DIR_bcm6848_pcm         := $(XDRV_ROOT)/bcm6848/pcm
XDRV_DIR_bcm6848_ntr         := $(XDRV_ROOT)/bcm6848/ntr
XDRV_DIR_bcm6848_led         := $(XDRV_ROOT)/bcm6848/leds
XDRV_DIR_bcm6848_daasi3050   := $(XDRV_ROOT)/bcm6848/daa/si3050
XDRV_DIR_bcm6848_relay       := $(XDRV_ROOT)/bcm6848/relay

# 6848 targets.
XDRV_PUBLIC_INC_bcm6848_boot           := $(XDRV_DIR_bcm6848_boot) \
                                           $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6848_dlb            := $(XDRV_DIR_bcm6848_dlb)/inc $(XDRV_DIR_bcm6848_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6848_handshake      := $(XDRV_DIR_bcm6848_handshake)
XDRV_PUBLIC_INC_bcm6848_slicagere      := $(XDRV_DIR_bcm6848_slicagere)
XDRV_PUBLIC_INC_bcm6848_pcm            := $(XDRV_DIR_bcm6848_pcm) \
                                           $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm6848_ntr            := $(XDRV_DIR_bcm6848_ntr)
XDRV_PUBLIC_INC_bcm6848_led            := $(XDRV_DIR_bcm6848_led)
XDRV_PUBLIC_INC_bcm6848_daasi3050      := $(XDRV_DIR_bcm6848_daasi3050)
XDRV_PUBLIC_INC_bcm6848_daale89316     := $(XDRV_DIR_bcm6848_daale89316)
XDRV_PUBLIC_INC_bcm6848_relay          := $(XDRV_DIR_bcm6848_relay)


# Common targets.
XDRV_DIR_casDriver            := $(XDRV_ROOT)/arch/common/casDriver
XDRV_DIR_dbgCom               := $(XDRV_ROOT)/arch/common/dbgCom
XDRV_DIR_circBuf              := $(XDRV_ROOT)/arch/common/circBuf
XDRV_DIR_debugCmtInterface    := $(XDRV_ROOT)/arch/common/debug
XDRV_DIR_debugZspInterface    := $(XDRV_ROOT)/arch/common/debug
XDRV_DIR_flowCtrl             := $(XDRV_ROOT)/arch/common/flowCtrl
XDRV_DIR_ipcSharedMem         := $(XDRV_ROOT)/arch/common/ipcSharedMem

# MIPS targets.
XDRV_DIR_cacheMips            := $(XDRV_ROOT)/arch/mips/cache
XDRV_DIR_cp0Mips              := $(XDRV_ROOT)/arch/mips/cp0
XDRV_DIR_exceptionMips        := $(XDRV_ROOT)/arch/mips/exception
XDRV_DIR_exceptionHdlrMips    := $(XDRV_ROOT)/arch/mips/exceptionHdlr
XDRV_DIR_exceptionHdlrCmtMips := $(XDRV_ROOT)/arch/mips/exceptionHdlrCmt
XDRV_DIR_globalInterruptMips  := $(XDRV_ROOT)/arch/mips/globalInterrupt
XDRV_DIR_intCtrlMips          := $(XDRV_ROOT)/arch/mips/intCtrl
XDRV_DIR_itpcMips             := $(XDRV_ROOT)/arch/mips/itpc
XDRV_DIR_spinLockMips         := $(XDRV_ROOT)/arch/mips/spinLock
XDRV_DIR_timerMips            := $(XDRV_ROOT)/arch/mips/timer

# ARM targets.
XDRV_DIR_cacheArm             := $(XDRV_ROOT)/arch/arm/cache
XDRV_DIR_globalInterruptArm   := $(XDRV_ROOT)/arch/arm/globalInterrupt
XDRV_DIR_spinLockArm          := $(XDRV_ROOT)/arch/arm/spinLock
XDRV_DIR_timerArm             := $(XDRV_ROOT)/arch/arm/timer
XDRV_DIR_neon                 := $(XDRV_ROOT)/arch/arm/neon

# Stub targets.
XDRV_DIR_apmSim               := $(XDRV_ROOT)/arch/stub/apm
XDRV_DIR_ledSim               := $(XDRV_ROOT)/arch/stub/leds
XDRV_DIR_casDriverSim         := $(XDRV_ROOT)/arch/stub/casDriver
XDRV_DIR_debugSim             := $(XDRV_ROOT)/arch/stub/debug
XDRV_DIR_dlbStub              := $(XDRV_ROOT)/arch/stub/dlb
XDRV_DIR_ipcSim               := $(XDRV_ROOT)/arch/stub/ipc
XDRV_DIR_slicSim              := $(XDRV_ROOT)/arch/stub/slic

# ZSP targets.
XDRV_DIR_zspTimer             := $(XDRV_ROOT)/arch/zsp/zspTimer


# ---------------------------------------------------------------------------
# Target public includes. For each target listed above, this provides the
# public include directory for the module. By default, these will be added
# to the global search path for this VOB.
#

# 6358 targets.
XDRV_PUBLIC_INC_bcm6358_boot           := $(XDRV_DIR_bcm6358_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6358_dlb            := $(XDRV_DIR_bcm6358_dlb)/inc $(XDRV_DIR_bcm638_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6358_handshake      := $(XDRV_DIR_bcm63xx_handshake)
XDRV_PUBLIC_INC_bcm6358_slicagere      := $(XDRV_DIR_bcm6358_slicagere)
XDRV_PUBLIC_INC_bcm6358_pcm            := $(XDRV_DIR_bcm6358_pcm)        \
                                          $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm6358_led            := $(XDRV_DIR_bcm6358_led)
XDRV_PUBLIC_INC_bcm6358_daasi3050      := $(XDRV_DIR_bcm6358_daasi3050)
XDRV_PUBLIC_INC_bcm6358_daale88311     := $(XDRV_DIR_bcm6358_daale88311)
XDRV_PUBLIC_INC_bcm6358_relay          := $(XDRV_DIR_bcm6358_relay)

# 6368 targets.
XDRV_PUBLIC_INC_bcm6368_boot           := $(XDRV_DIR_bcm6368_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6368_dlb            := $(XDRV_DIR_bcm6368_dlb)/inc $(XDRV_DIR_bcm6368_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6368_handshake      := $(XDRV_DIR_bcm63xx_handshake)
XDRV_PUBLIC_INC_bcm6368_slicagere      := $(XDRV_DIR_bcm6368_slicagere)
XDRV_PUBLIC_INC_bcm6368_pcm            := $(XDRV_DIR_bcm6368_pcm)        \
                                          $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm6368_ntr            := $(XDRV_DIR_bcm6368_ntr)
XDRV_PUBLIC_INC_bcm6368_led            := $(XDRV_DIR_bcm6368_led)
XDRV_PUBLIC_INC_bcm6368_daasi3050      := $(XDRV_DIR_bcm6368_daasi3050)
XDRV_PUBLIC_INC_bcm6368_daale89316     := $(XDRV_DIR_bcm6368_daale89316)
XDRV_PUBLIC_INC_bcm6368_relay          := $(XDRV_DIR_bcm6368_relay)

# 6816 targets.
XDRV_PUBLIC_INC_bcm6816_apm            := $(XDRV_DIR_bcm6816_apm) \
                                          $(XDRV_DIR_bcm63xx_pcm) \
                                          $(XDRV_ROOT)/bcm6816/chipinc
XDRV_PUBLIC_INC_bcm6816_boot           := $(XDRV_DIR_bcm6816_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6816_clock          := $(XDRV_DIR_bcm6816_clock)
XDRV_PUBLIC_INC_bcm6816_dlb            := $(XDRV_DIR_bcm6816_dlb)/inc $(XDRV_DIR_bcm6816_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6816_handshake      := $(XDRV_DIR_bcm6816_handshake)
XDRV_PUBLIC_INC_bcm6816_slicl9500      := $(XDRV_DIR_bcm6816_slicl9500)
XDRV_PUBLIC_INC_bcm6816_slicl9520      := $(XDRV_DIR_bcm6816_slicl9520)
XDRV_PUBLIC_INC_bcm6816_slicl9530      := $(XDRV_DIR_bcm6816_slicl9530)
XDRV_PUBLIC_INC_bcm6816_slicsi3230     := $(XDRV_DIR_bcm6816_slicsi3230)
XDRV_PUBLIC_INC_bcm6816_slicsi3239     := $(XDRV_DIR_bcm6816_slicsi3239)
XDRV_PUBLIC_INC_bcm6816_tpdl9500       := $(XDRV_DIR_bcm6816_tpdl9500)
XDRV_PUBLIC_INC_bcm6816_tpdl9520       := $(XDRV_DIR_bcm6816_tpdl9520)
XDRV_PUBLIC_INC_bcm6816_tpdl9530       := $(XDRV_DIR_bcm6816_tpdl9530)
XDRV_PUBLIC_INC_bcm6816_tpdsi3230      := $(XDRV_DIR_bcm6816_tpdsi3230)
XDRV_PUBLIC_INC_bcm6816_tpdsi3239      := $(XDRV_DIR_bcm6816_tpdsi3239)

# 6818 targets.
XDRV_PUBLIC_INC_bcm6818_apm            := $(XDRV_DIR_bcm6818_apm) \
                                          $(XDRV_DIR_bcm63xx_pcm) \
                                          $(XDRV_DIR_bcm6818_apm)/cfginc \
                                          $(XDRV_ROOT)/bcm6818/chipinc
XDRV_PUBLIC_INC_bcm6818_boot           := $(XDRV_DIR_bcm6818_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6818_clock          := $(XDRV_DIR_bcm6818_clock)
XDRV_PUBLIC_INC_bcm6818_dlb            := $(XDRV_DIR_bcm6818_dlb)/inc $(XDRV_DIR_bcm6818_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6818_handshake      := $(XDRV_DIR_bcm6818_handshake)
XDRV_PUBLIC_INC_bcm6818_slicl9500      := $(XDRV_DIR_bcm6818_slicl9500)
XDRV_PUBLIC_INC_bcm6818_slicl9520      := $(XDRV_DIR_bcm6818_slicl9520)
XDRV_PUBLIC_INC_bcm6818_slicl9530      := $(XDRV_DIR_bcm6818_slicl9530)
XDRV_PUBLIC_INC_bcm6818_slicl9540      := $(XDRV_DIR_bcm6818_slicl9540) 
XDRV_PUBLIC_INC_bcm6818_slicsi3230     := $(XDRV_DIR_bcm6818_slicsi3230)
XDRV_PUBLIC_INC_bcm6818_slicsi3239     := $(XDRV_DIR_bcm6818_slicsi3239)
XDRV_PUBLIC_INC_bcm6818_slicsi32392    := $(XDRV_DIR_bcm6818_slicsi32392)
XDRV_PUBLIC_INC_bcm6818_tpdl9500       := $(XDRV_DIR_bcm6818_tpdl9500)
XDRV_PUBLIC_INC_bcm6818_tpdl9520       := $(XDRV_DIR_bcm6818_tpdl9520)
XDRV_PUBLIC_INC_bcm6818_tpdl9530       := $(XDRV_DIR_bcm6818_tpdl9530)
XDRV_PUBLIC_INC_bcm6818_tpdsi3230      := $(XDRV_DIR_bcm6818_tpdsi3230)
XDRV_PUBLIC_INC_bcm6818_tpdsi3239      := $(XDRV_DIR_bcm6818_tpdsi3239)

# 6838 targets.
XDRV_PUBLIC_INC_bcm6838_apm            := $(XDRV_DIR_bcm6838_apm) \
                                          $(XDRV_DIR_bcm63xx_pcm) \
                                          $(XDRV_DIR_bcm6838_apm)/cfginc \
                                          $(XDRV_ROOT)/bcm6838/chipinc \
                                          $(XCHG_LINUX_ROOT)/../../shared/opensource/include/pmc
XDRV_PUBLIC_INC_bcm6838_boot           := $(XDRV_DIR_bcm6838_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6838_clock          := $(XDRV_DIR_bcm6838_clock)
XDRV_PUBLIC_INC_bcm6838_dlb            := $(XDRV_DIR_bcm6838_dlb)/inc $(XDRV_DIR_bcm6838_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6838_handshake      := $(XDRV_DIR_bcm6838_handshake)
XDRV_PUBLIC_INC_bcm6838_slicl9500      := $(XDRV_DIR_bcm6838_slicl9500)
XDRV_PUBLIC_INC_bcm6838_slicl9520      := $(XDRV_DIR_bcm6838_slicl9520)
XDRV_PUBLIC_INC_bcm6838_slicl9530      := $(XDRV_DIR_bcm6838_slicl9530)
XDRV_PUBLIC_INC_bcm6838_slicl9540      := $(XDRV_DIR_bcm6838_slicl9540)
XDRV_PUBLIC_INC_bcm6838_slicsi3230     := $(XDRV_DIR_bcm6838_slicsi3230)
XDRV_PUBLIC_INC_bcm6838_slicsi3239     := $(XDRV_DIR_bcm6838_slicsi3239)
XDRV_PUBLIC_INC_bcm6838_slicsi32392    := $(XDRV_DIR_bcm6838_slicsi32392)
XDRV_PUBLIC_INC_bcm6838_tpdl9500       := $(XDRV_DIR_bcm6838_tpdl9500)
XDRV_PUBLIC_INC_bcm6838_tpdl9520       := $(XDRV_DIR_bcm6838_tpdl9520)
XDRV_PUBLIC_INC_bcm6838_tpdl9530       := $(XDRV_DIR_bcm6838_tpdl9530)
XDRV_PUBLIC_INC_bcm6838_tpdl9540       := $(XDRV_DIR_bcm6838_tpdl9540) \
                                          $(XDRV_DIR_bcm6838_tpdl9540)/cfginc
XDRV_PUBLIC_INC_bcm6838_tpdsi3230      := $(XDRV_DIR_bcm6838_tpdsi3230)
XDRV_PUBLIC_INC_bcm6838_tpdsi32392     := $(XDRV_DIR_bcm6838_tpdsi32392) \
                                          $(XDRV_DIR_bcm6838_tpdsi32392)/cfginc
XDRV_PUBLIC_INC_bcm6838_tpdsi3239      := $(XDRV_DIR_bcm6838_tpdsi3239)

# 6828 targets.
XDRV_PUBLIC_INC_bcm6828_apm            := $(XDRV_DIR_bcm6828_apm) \
                                          $(XDRV_DIR_bcm63xx_pcm) \
                                          $(XDRV_DIR_bcm6828_apm)/cfginc \
                                          $(XDRV_ROOT)/bcm6828/chipinc
XDRV_PUBLIC_INC_bcm6828_boot           := $(XDRV_DIR_bcm6828_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6828_clock          := $(XDRV_DIR_bcm6828_clock)
XDRV_PUBLIC_INC_bcm6828_dlb            := $(XDRV_DIR_bcm6828_dlb)/inc $(XDRV_DIR_bcm6828_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6828_handshake      := $(XDRV_DIR_bcm6828_handshake)
XDRV_PUBLIC_INC_bcm6828_slicl9500      := $(XDRV_DIR_bcm6828_slicl9500)
XDRV_PUBLIC_INC_bcm6828_slicl9520      := $(XDRV_DIR_bcm6828_slicl9520)
XDRV_PUBLIC_INC_bcm6828_slicl9530      := $(XDRV_DIR_bcm6828_slicl9530)
XDRV_PUBLIC_INC_bcm6828_slicsi3230     := $(XDRV_DIR_bcm6828_slicsi3230)
XDRV_PUBLIC_INC_bcm6828_slicsi3239     := $(XDRV_DIR_bcm6828_slicsi3239)
XDRV_PUBLIC_INC_bcm6828_slicsi32392    := $(XDRV_DIR_bcm6828_slicsi32392)
XDRV_PUBLIC_INC_bcm6828_tpdl9500       := $(XDRV_DIR_bcm6828_tpdl9500)
XDRV_PUBLIC_INC_bcm6828_tpdl9520       := $(XDRV_DIR_bcm6828_tpdl9520)
XDRV_PUBLIC_INC_bcm6828_tpdl9530       := $(XDRV_DIR_bcm6828_tpdl9530)
XDRV_PUBLIC_INC_bcm6828_tpdsi3230      := $(XDRV_DIR_bcm6828_tpdsi3230)
XDRV_PUBLIC_INC_bcm6828_tpdsi3239      := $(XDRV_DIR_bcm6828_tpdsi3239)

# 6362 targets.
XDRV_PUBLIC_INC_bcm6362_boot           := $(XDRV_DIR_bcm6362_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6362_dlb            := $(XDRV_DIR_bcm6362_dlb)/inc $(XDRV_DIR_bcm6362_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6362_handshake      := $(XDRV_DIR_bcm6362_handshake)
XDRV_PUBLIC_INC_bcm6362_slicagere      := $(XDRV_DIR_bcm6362_slicagere)
XDRV_PUBLIC_INC_bcm6362_pcm            := $(XDRV_DIR_bcm6362_pcm) \
                                          $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm6362_ntr            := $(XDRV_DIR_bcm6362_ntr)
XDRV_PUBLIC_INC_bcm6362_led            := $(XDRV_DIR_bcm6362_led)
XDRV_PUBLIC_INC_bcm6362_daasi3050      := $(XDRV_DIR_bcm6362_daasi3050)
XDRV_PUBLIC_INC_bcm6362_daale89316     := $(XDRV_DIR_bcm6362_daale89316)
XDRV_PUBLIC_INC_bcm6362_relay          := $(XDRV_DIR_bcm6362_relay)
XDRV_PUBLIC_INC_bcm6362_dectTest       := $(XDRV_DIR_bcm6362_dectTest)

# 6328 targets.
XDRV_PUBLIC_INC_bcm6328_boot           := $(XDRV_DIR_bcm6328_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6328_dlb            := $(XDRV_DIR_bcm6328_dlb)/inc $(XDRV_DIR_bcm6362_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6328_handshake      := $(XDRV_DIR_bcm63xx_handshake)
XDRV_PUBLIC_INC_bcm6328_slicagere      := $(XDRV_DIR_bcm6328_slicagere)
XDRV_PUBLIC_INC_bcm6328_pcm            := $(XDRV_DIR_bcm6328_pcm) \
                                          $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm6328_ntr            := $(XDRV_DIR_bcm6328_ntr)
XDRV_PUBLIC_INC_bcm6328_led            := $(XDRV_DIR_bcm6328_led)
XDRV_PUBLIC_INC_bcm6328_relay          := $(XDRV_DIR_bcm6328_relay)

# 63268 targets.
XDRV_PUBLIC_INC_bcm63268_boot           := $(XDRV_DIR_bcm63268_boot) \
                                           $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm63268_dlb            := $(XDRV_DIR_bcm63268_dlb)/inc $(XDRV_DIR_bcm63268_dlb)/cfginc
XDRV_PUBLIC_INC_bcm63268_handshake      := $(XDRV_DIR_bcm63268_handshake)
XDRV_PUBLIC_INC_bcm63268_slicagere      := $(XDRV_DIR_bcm63268_slicagere)
XDRV_PUBLIC_INC_bcm63268_pcm            := $(XDRV_DIR_bcm63268_pcm) \
                                           $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm63268_ntr            := $(XDRV_DIR_bcm63268_ntr)
XDRV_PUBLIC_INC_bcm63268_led            := $(XDRV_DIR_bcm63268_led)
XDRV_PUBLIC_INC_bcm63268_daasi3050      := $(XDRV_DIR_bcm63268_daasi3050)
XDRV_PUBLIC_INC_bcm63268_daale89316     := $(XDRV_DIR_bcm63268_daale89316)
XDRV_PUBLIC_INC_bcm63268_relay          := $(XDRV_DIR_bcm63268_relay)
XDRV_PUBLIC_INC_bcm63268_dectTest       := $(XDRV_DIR_bcm63268_dectTest)

# 63381 targets.
XDRV_PUBLIC_INC_bcm63381_boot           := $(XDRV_DIR_bcm63381_boot) \
                                           $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm63381_dlb            := $(XDRV_DIR_bcm63381_dlb)/inc $(XDRV_DIR_bcm63381_dlb)/cfginc
XDRV_PUBLIC_INC_bcm63381_handshake      := $(XDRV_DIR_bcm63381_handshake)
XDRV_PUBLIC_INC_bcm63381_slicagere      := $(XDRV_DIR_bcm63381_slicagere)
XDRV_PUBLIC_INC_bcm63381_pcm            := $(XDRV_DIR_bcm63381_pcm) \
                                           $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm63381_ntr            := $(XDRV_DIR_bcm63381_ntr)
XDRV_PUBLIC_INC_bcm63381_led            := $(XDRV_DIR_bcm63381_led)
XDRV_PUBLIC_INC_bcm63381_daasi3050      := $(XDRV_DIR_bcm63381_daasi3050)
XDRV_PUBLIC_INC_bcm63381_daale89316     := $(XDRV_DIR_bcm63381_daale89316)
XDRV_PUBLIC_INC_bcm63381_relay          := $(XDRV_DIR_bcm63381_relay)

# 6848 targets.
XDRV_PUBLIC_INC_bcm6848_boot           := $(XDRV_DIR_bcm6848_boot) \
                                          $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm6848_dlb            := $(XDRV_DIR_bcm6848_dlb)/inc $(XDRV_DIR_bcm6848_dlb)/cfginc
XDRV_PUBLIC_INC_bcm6848_handshake      := $(XDRV_DIR_bcm6848_handshake)
XDRV_PUBLIC_INC_bcm6848_slicagere      := $(XDRV_DIR_bcm6848_slicagere)
XDRV_PUBLIC_INC_bcm6848_pcm            := $(XDRV_DIR_bcm6848_pcm) \
                                           $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm6848_ntr            := $(XDRV_DIR_bcm6848_ntr)
XDRV_PUBLIC_INC_bcm6848_led            := $(XDRV_DIR_bcm6848_led)
XDRV_PUBLIC_INC_bcm6848_daasi3050      := $(XDRV_DIR_bcm6848_daasi3050)
XDRV_PUBLIC_INC_bcm6848_daale89316     := $(XDRV_DIR_bcm6848_daale89316)
XDRV_PUBLIC_INC_bcm6848_relay          := $(XDRV_DIR_bcm6848_relay)

# 68500 targets.
XDRV_PUBLIC_INC_bcm68500_boot           := $(XDRV_DIR_bcm68500_boot) \
                                           $(XDRV_ROOT)/bcm63xx/inc
XDRV_PUBLIC_INC_bcm68500_dlb            := $(XDRV_DIR_bcm68500_dlb)/inc $(XDRV_DIR_bcm68500_dlb)/cfginc
XDRV_PUBLIC_INC_bcm68500_handshake      := $(XDRV_DIR_bcm63xx_handshake)
XDRV_PUBLIC_INC_bcm68500_slicagere      := $(XDRV_DIR_bcm68500_slicagere)
XDRV_PUBLIC_INC_bcm68500_pcm            := $(XDRV_DIR_bcm68500_pcm)        \
                                           $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm68500_ntr            := $(XDRV_DIR_bcm68500_ntr)
XDRV_PUBLIC_INC_bcm68500_daasi3050      := $(XDRV_DIR_bcm68500_daasi3050)
XDRV_PUBLIC_INC_bcm68500_daale89316     := $(XDRV_DIR_bcm68500_daale89316)
XDRV_PUBLIC_INC_bcm68500_ceva           := $(XDRV_DIR_bcm68500_ceva) \
                                           $(XCHG_LINUX_ROOT)/xchg_common/crt/inc/LinuxKernel \
                                           $(XCHG_LINUX_ROOT)/../../shared/opensource/include/rdp \
                                           $(XCHG_LINUX_ROOT)/../../shared/opensource/include/bcm968500 \
                                           $(XCHG_LINUX_ROOT)/../../shared/opensource/include/bcm968500/os \
                                           $(XCHG_LINUX_ROOT)/../../shared/opensource/include/bcm968500/soc \
                                           $(XCHG_LINUX_ROOT)/../../shared/opensource/include/bcm968500/base \
                                           $(XCHG_LINUX_ROOT)/../../shared/opensource/include/bcm968500/registers \
                                           $(XCHG_LINUX_ROOT)/../../shared/opensource/include/bcm968500/brd \
                                           $(XCHG_LINUX_ROOT)/../../shared/opensource/include/bcm968500/drv

# 63138 targets.
XDRV_PUBLIC_INC_bcm63138_pcm         := $(XDRV_DIR_bcm63138_pcm) \
                                        $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm63138_led         := $(XDRV_DIR_bcm63138_led)
XDRV_PUBLIC_INC_bcm63138_relay       := $(XDRV_DIR_bcm63138_relay)
XDRV_PUBLIC_INC_bcm63138_dectTest    := $(XDRV_DIR_bcm63138_dectTest)

# 63148 targets.
XDRV_PUBLIC_INC_bcm63148_pcm         := $(XDRV_DIR_bcm63148_pcm) \
                                        $(XDRV_DIR_bcm63xx_pcm)
XDRV_PUBLIC_INC_bcm63148_led         := $(XDRV_DIR_bcm63148_led)
XDRV_PUBLIC_INC_bcm63148_relay       := $(XDRV_DIR_bcm63148_relay)
XDRV_PUBLIC_INC_bcm63148_dectTest    := $(XDRV_DIR_bcm63148_dectTest)

# 63xx targets.
XDRV_PUBLIC_INC_bcm63xx_intCtrl        := $(XDRV_DIR_bcm63xx_intCtrl)
XDRV_PUBLIC_INC_bcm63xx_timer          := $(XDRV_DIR_bcm63xx_timer)
XDRV_PUBLIC_INC_bcm63xx_resetCallout   := $(XDRV_DIR_bcm63xx_resetCallout)
XDRV_PUBLIC_INC_bcm63xx_silabs         := $(XDRV_DIR_bcm63xx_silabs)/common                                 \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api                            \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/slic_drivers/si321x/       \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/slic_drivers/si3217x/      \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/slic_drivers/si3226/       \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/slic_drivers/si3226x/      \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/interface_drivers/common/  \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/interface_drivers/si321x/  \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/interface_drivers/si3217x/ \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/interface_drivers/si3226/  \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/interface_drivers/si3226x/ \
                                          $(XDRV_DIR_bcm63xx_silabs)/proslic_api/interface_drivers/si3226x_isi/ \
                                          $(XDRV_DIR_bcm63xx_silabs)/si3050                                 \
                                          $(XDRV_DIR_bcm63xx_silabs)/si3215                                 \
                                          $(XDRV_DIR_bcm63xx_silabs)/si3216                                 \
                                          $(XDRV_DIR_bcm63xx_silabs)/si32178                                \
                                          $(XDRV_DIR_bcm63xx_silabs)/si3226                                 \
                                          $(XDRV_DIR_bcm63xx_silabs)/si32176                                \
                                          $(XDRV_DIR_bcm63xx_silabs)/si32261                                \
                                          $(XDRV_DIR_bcm63xx_silabs)/si32267
XDRV_PUBLIC_INC_bcm63xx_zarlink        := $(XDRV_DIR_bcm63xx_zarlink)/api_lib/includes       \
                                          $(XDRV_DIR_bcm63xx_zarlink)/api_lib/arch/bcm63xx   \
                                          $(XDRV_DIR_bcm63xx_zarlink)/api_lib/profiles       \
                                          $(XDRV_DIR_bcm63xx_zarlink)/common                 \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88010                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88111                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88221                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le89010                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le89116                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le89316                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88266                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88276                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88506                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/zl88601                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/zl88701                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/zl88702                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/zl88801                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le89136                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le89336                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88536                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88264                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le88004                \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le9672                 \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le9661                 \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le9642                 \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le9641                 \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le9652                 \
                                          $(XDRV_DIR_bcm63xx_zarlink)/le9662
XDRV_PUBLIC_INC_bcm63xx_handshake      := $(XDRV_DIR_bcm63xx_handshake)
XDRV_PUBLIC_INC_bcm63xx_pcm            := $(XDRV_DIR_bcm63xx_pcm)

# Common targets.
XDRV_PUBLIC_INC_casDriver              := $(XDRV_DIR_casDriver)/inc $(XDRV_DIR_casDriver)/cfginc
XDRV_PUBLIC_INC_dbgCom                 := $(XDRV_DIR_dbgCom)
XDRV_PUBLIC_INC_circBuf                := $(XDRV_DIR_circBuf)
XDRV_PUBLIC_INC_debugCmtInterface      := $(XDRV_DIR_debugCmtInterface)/inc
XDRV_PUBLIC_INC_debugZspInterface      := $(XDRV_DIR_debugZspInterface)/inc
XDRV_PUBLIC_INC_flowCtrl               := $(XDRV_DIR_flowCtrl)/inc $(XDRV_DIR_flowCtrl)/cfginc
XDRV_PUBLIC_INC_ipcSharedMem           := $(XDRV_DIR_ipcSharedMem)/inc $(XDRV_DIR_ipcSharedMem)/cfginc

# MIPS targets.
XDRV_PUBLIC_INC_cacheMips              := $(XDRV_DIR_cacheMips)
XDRV_PUBLIC_INC_cp0Mips                := $(XDRV_DIR_cp0Mips)
XDRV_PUBLIC_INC_exceptionMips          := $(XDRV_DIR_exceptionMips)/inc
XDRV_PUBLIC_INC_exceptionHdlrMips      := $(XDRV_DIR_exceptionHdlrMips)/inc
XDRV_PUBLIC_INC_exceptionHdlrCmtMips   := $(XDRV_DIR_exceptionHdlrCmtMips)/inc $(XDRV_DIR_exceptionHdlrCmtMips)/cfginc
XDRV_PUBLIC_INC_globalInterruptMips    := $(XDRV_DIR_globalInterruptMips)
XDRV_PUBLIC_INC_intCtrlMips            := $(XDRV_DIR_intCtrlMips)/inc
XDRV_PUBLIC_INC_itpcMips               := $(XDRV_DIR_itpcMips)/inc $(XDRV_DIR_itpcMips)/cfginc
XDRV_PUBLIC_INC_spinLockMips           := $(XDRV_DIR_spinLockMips)
XDRV_PUBLIC_INC_timerMips              := $(XDRV_DIR_timerMips)/inc

# ARM targets.
XDRV_PUBLIC_INC_cacheArm               := $(XDRV_DIR_cacheArm)
XDRV_PUBLIC_INC_globalInterruptArm     := $(XDRV_DIR_globalInterruptArm)
XDRV_PUBLIC_INC_spinLockArm            := $(XDRV_DIR_spinLockArm)
XDRV_PUBLIC_INC_timerArm               := $(XDRV_DIR_timerArm)/inc
XDRV_PUBLIC_INC_neon                   := $(XDRV_DIR_neon)

# Stub targets.
XDRV_PUBLIC_INC_apmSim                 := $(XDRV_DIR_apmSim)
XDRV_PUBLIC_INC_ledSim                 := $(XDRV_DIR_ledSim)
XDRV_PUBLIC_INC_casDriverSim           := $(XDRV_DIR_casDriverSim)
XDRV_PUBLIC_INC_debugSim               := $(XDRV_DIR_debugSim) \
                                          $(XDRV_ROOT)/inc
XDRV_PUBLIC_INC_dlbStub                := $(XDRV_DIR_dlbStub)
XDRV_PUBLIC_INC_ipcSim                 := $(XDRV_DIR_ipcSim)
XDRV_PUBLIC_INC_slicSim                := $(XDRV_DIR_slicSim)

# ZSP targets.
XDRV_PUBLIC_INC_zspTimer               := $(XDRV_DIR_zspTimer)

