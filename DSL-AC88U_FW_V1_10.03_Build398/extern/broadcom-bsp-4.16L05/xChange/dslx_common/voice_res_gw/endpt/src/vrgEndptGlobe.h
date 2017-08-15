/****************************************************************************
*
* Copyright © 2000-2008 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************

*
*    Filename: vrgEndptGlobe.h
*
****************************************************************************
*    Description:
*
*      This file contains the API definitions used by the
*      Endpoint around the Globe (international endpoint)
*
****************************************************************************/

#ifndef VRG_ENDPT_GLOBE_H
#define VRG_ENDPT_GLOBE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <endpt.h>
#include <vrgTypes.h>
#include <vrgCountry.h>
#include <casCtl.h>
/*
*****************************************************************************
** TYPEDEFS AND CONSTANTS
*****************************************************************************
*/
/*
** Endpoint (around the) Globe IDs (country specific items)
** Some of them are provisionable (in which case the Prov defaults would apply),
** but some are not (in which case the Enpoint (around the) Globe country
** specific static values would apply).
*/
typedef enum
{
   EPGLOBE_ID_NULL,
   EPGLOBE_ID_CallWtgDelay,
   EPGLOBE_ID_MinConnect,
   EPGLOBE_ID_MinDisconnect,
   EPGLOBE_ID_MinHookFlash,
   EPGLOBE_ID_MaxHookFlash,
   EPGLOBE_ID_PlsDlInterdigitTime,
   EPGLOBE_ID_PlsDlMinMakeTime,
   EPGLOBE_ID_PlsDlMaxMakeTime,
   EPGLOBE_ID_PlsDlMinBreakTime,
   EPGLOBE_ID_PlsDlMaxBreakTime,

   EPGLOBE_ID_PowerRingFrequency,   /* ring frequency */
   EPGLOBE_ID_RingVoltage,          /* ring voltage   */
   EPGLOBE_ID_RingWaveform,         /* ring waveform  */
   EPGLOBE_ID_RingDCOffset,         /* ring offset    */

   EPGLOBE_ID_CIDMode,
   EPGLOBE_ID_CIDFskAfterRing,
   EPGLOBE_ID_CIDFskAfterDTAS,
   EPGLOBE_ID_CIDFskAfterRPAS,
   EPGLOBE_ID_CIDRingAfterFSK,
   EPGLOBE_ID_CIDDTASAfterLR,
   EPGLOBE_ID_CIDSigProtocol,

   EPGLOBE_ID_CIDFskParms,        /* CLID FSK parameters */
   EPGLOBE_ID_CIDDtmfParms,       /* CLID DTMF parameters */


   EPGLOBE_ID_VMWIMode,
   EPGLOBE_ID_VMWISigProtocol,
   EPGLOBE_ID_VMWIFskAfterDTAS,
   EPGLOBE_ID_VMWIFskAfterRPAS,
   EPGLOBE_ID_VMWIDTASAfterLR,
   EPGLOBE_ID_VMWIMsgFormat,        /* Visual Message Waiting Indicator Formats TR-NWT-001401 */

   EPGLOBE_ID_PteDetectAdmin,       /* Specifies the parameters to be used in the PTE DTMF detector
                                    ** for particular administrations. This setting controls frequency
                                    ** thresholds, twist thresholds, and signal interruption thresholds. */
   EPGLOBE_ID_PteMinDetectPower,    /* Specifies the minimum power level at which to detect DTMF digits.
                                    ** The power is specified in negative dBm per frequency. Valid range is (22 - 45) */
   EPGLOBE_ID_PveEgressVolume,      /* PVE volume control on the decoded signal. This is a Q12 format number
                                    ** with 0x1000 equal 0dB (ie. 0x1424=2dD; 0x0a18=-4dB, etc.).
                                    ** Set to 0 to skip programming this value and use PVE default. */
   EPGLOBE_ID_DtmfDbLevel,          /* Specifies dB level for DTMF signal generation
                                    ** (this is not used for DTMF CLID generation, see CIDDtmfParms)*/
   EPGLOBE_ID_DtmfPulseTime,        /* Specifies on/off pulse width for DTMF signal generation */

   EPGLOBE_ID_HighVringSupport,     /* Indicates support for high voltage ring mode */
   EPGLOBE_ID_MAX
} EPGLOBE_ID;



/*****************************************************************************
*  FUNCTION:   globeEndptGetRingPattern
*
*  PURPOSE:    Look up the ring pattern associated with the Endpt ring signal.
*
*  PARAMETERS: country    - country code
*              signal     - EPSIG id to look up the ring profile for
*              pProfile   - pointer to CAS ring profile to return
*              provItemId - pointer to prov item ID to return associated
*                            with this ring profile
*
*  RETURNS:    EPSTATUS
*
*  NOTES:      If ring pattern not found in the country specific table,
*              we will use NorthAmerican ring profiles instead.
*
*****************************************************************************/
EPSTATUS globeEndptGetRingPattern( VRG_COUNTRY country, EPSIG signal, CAS_CTL_RING_PROFILE *pProfile, int *provItemId);


#if defined(__cplusplus)
}
#endif

#endif /* #ifndef VRG_ENDPT_GLOBE_H */
