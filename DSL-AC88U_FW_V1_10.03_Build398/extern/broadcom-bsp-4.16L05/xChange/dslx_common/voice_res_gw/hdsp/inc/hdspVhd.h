/***************************************************************************
*    Copyright © 2000-2007 Broadcom
*
*    This program is the proprietary software of Broadcom and/or
*    its licensors, and may only be used, duplicated, modified or
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized
*    License").  Except as set forth in an Authorized License, Broadcom
*    grants no license (express or implied), right to use, or waiver of any
*    kind with respect to the Software, and Broadcom expressly reserves all
*    rights in and to the Software and all intellectual property rights
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.
*
*
*    Except as expressly set forth in the Authorized License,
*
*    1.     This program, including its structure, sequence and
*    organization, constitutes the valuable trade secrets of Broadcom, and
*    you shall use all reasonable efforts to protect the confidentiality
*    thereof, and to use this information only in connection with your use
*    of Broadcom integrated circuit products.
*
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT
*    OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL,
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: hdspVhd.h
*
****************************************************************************
*    Description:
*
*     This file contains the common definitions used by the ... interface.
*
****************************************************************************/

#ifndef HDSP_VHD_H
#define HDSP_VHD_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgCountry.h>
#include <cmtdCtl.h>
#include <hapi.h>
#include <hapi_pte.h>
#include <hapi_hec.h>

/* ---- Constants and Types ---------------------------------------------- */

#define hdspVhdAddService     hapiAddService
#define hdspVhdRemoveService  hapiRemoveService

/* Minimum and maximum gain settings (dB) */
#define HDSPVHD_MIN_GAIN  -16
#define HDSPVHD_MAX_GAIN   16
#define HDSPVHD_MUTE_GAIN -96

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

VRG_SINT16 hdspVhdOpen( VRG_UINT16 vhdType, HAPICALLBKP callbackp, VRG_UINT16 dspNum,
                        HAPI_PTE_ADM pteDetectAdmin, VRG_UINT16 pteMinDetectPower,
                        VRG_UINT16 pveEgressVolume, VRG_UINT16 *vhdhdl );
VRG_SINT16 hdspVhdClose( VRG_UINT16 vhdHdl, VRG_UINT16 dspNum );
VRG_SINT16 hdspVhdSetMode( VRG_UINT16 vhdHdl, VRG_UINT16 mode, VRG_UINT16 initMode );
VRG_SINT16 hdspVhdSetVoiceCodec( VRG_UINT16 vhdHdl, VRG_UINT16 codec, VRG_UINT16 vad,
                                 VRG_UINT16 frames, VRG_BOOL bInitialChange );
VRG_SINT16 hdspVhdSetVBDCodec( VRG_UINT16 vhdHdl, VRG_UINT16 codec, VRG_UINT16 vad );
VRG_SINT16 hdspVhdSetVadCngPlc( VRG_UINT16 vhdHdl, VRG_UINT16 vad, VRG_UINT16 cng, VRG_UINT16 plc );
VRG_SINT16 hdspVhdGetVoiceStats( VRG_UINT16 vhdhdl,
                                 HSZNETVOICESTATS *netvoicestats );
void hdspVhdDisplayVoiceStatsRaw( VRG_UINT16 vhdhdl,
                                  HSZNETVOICESTATS *netvoicestats );
void hdspVhdFormatVoiceStats( HSZNETVOICESTATS *netvoicestats, char *strBuffer,
                              unsigned int bufSize );
void hdspVhdDisplayPveState( VRG_UINT16 vhdhdl );
void hdspVhdDisplayEndptStats( VRG_UINT16 vhdhdl );
void hdspVhdDisplayFaxrStats( VRG_UINT16 vhdhdl );
VRG_SINT16 hdspVhdSetJitterBuffer( VRG_UINT16 vhdhdl, VRG_BOOL fixed,
                                   VRG_UINT16 voiceMin, VRG_UINT16 voiceMax,
                                   VRG_UINT16 voiceTarget, VRG_UINT16 dataTarget, VRG_UINT16 ajcCustomReg );
VRG_SINT16 hdspVhdGetJitterBuffer( VRG_UINT16 vhdhdl, VRG_BOOL *fixed,
                                   VRG_UINT16 *voiceMin, VRG_UINT16 *voiceMax,
                                   VRG_UINT16 *voiceTarget, VRG_UINT16 *dataTarget, VRG_UINT16 *ajcCustomReg );
VRG_SINT16 hdspVhdGetAvgJBDelay( VRG_UINT16 vhdhdl, VRG_UINT16 *delay );
VRG_SINT16 hdspVhdSetAmrSettings( VRG_UINT16 vhdhdl, VRG_UINT16 modeSet, VRG_BOOL modeChangeNeighbor,
                                  VRG_BOOL modeChangePeriod, VRG_UINT16 ptime, VRG_UINT16 red );
VRG_UINT16 hdspVhdAdjustIngressPowerLevel( VRG_UINT16 powerLevel );
VRG_SINT16 hdspVhdGetCmtdPowerLevel( VRG_UINT16 vhdhdl, VRG_UINT32 *pPower );
VRG_SINT16 hdspVhdEnableCmtd( VRG_UINT16 lineVhdHdl, VRG_COUNTRY country );
VRG_SINT16 hdspVhdDisableCmtd( VRG_UINT16 lineVhdHdl );
VRG_SINT16 hdspVhdSetSSRC( VRG_UINT16 vhdHdl, VRG_UINT32 localSsrc );
VRG_SINT16 hdspVhdLineEquEnable( VRG_UINT16 vhdhdl, VRG_BOOL enable );
VRG_SINT16 hdspVhdSetEgressGain( VRG_UINT16 vhdhdl, int gaindB );
VRG_SINT16 hdspVhdSetIngressGain( VRG_UINT16 vhdhdl, int gaindB );
VRG_SINT16 hdspVhdSetPVEEgressVolume( VRG_UINT16 vhdhdl, int volumedB );
VRG_SINT16 hdspVhdSetPVEIngressVolume( VRG_UINT16 vhdhdl, int volumedB );

#ifdef __cplusplus
    }
#endif

#endif  /* HDSP_VHD_H  */
