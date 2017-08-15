/****************************************************************************
*
* Copyright © 2002-2008 Broadcom
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
****************************************************************************
*
*    Filename: classstm.h
*
****************************************************************************
*    Description:
*
*     This module contains the state machine definitions for CLASS features
*
****************************************************************************/

#ifndef CLASSSTM_H
#define CLASSSTM_H

#include <vrgTypes.h>
#include <sme.h>
#include <hdspClass.h>
#include <casCtl.h>
#include <hdspTone.h>


/* ringing state for on-hook caller-id */
typedef CAS_CTL_PROV_PATTERN  CLASSSTM_PROV_PATTERN;
typedef CAS_CTL_RING_PROFILE  CLASSSTM_RING_STATE;


/* Class state-machine types */
typedef enum  CLASSSTM_TYPE
{
   /* Class I, onhook, during ringing caller-id. */
   CLASSSTM_TYPE_ONHOOK_RING_CLID,

   /* Class I, onhook, line-reversal CAR/IR (Japan style) caller-id. */
   CLASSSTM_TYPE_ONHOOK_LRCARIR_CLID,

   /* Class I, onhook, dual-tone alerting signal (DTAS) caller-id. */
   CLASSSTM_TYPE_ONHOOK_DTAS_CLID,

   /* Class I, onhook, ring-pulse alerting signal (RPAS) caller-id. */
   CLASSSTM_TYPE_ONHOOK_RPAS_CLID,

   /* Class I, onhook, line-reversal alerting signal (LRAS) caller-id. */
   CLASSSTM_TYPE_ONHOOK_LRAS_CLID,

   /* Class II, offhook, caller-id (provisioned tones). */
   CLASSSTM_TYPE_OFFHOOK_CLID,

   /* Visual Message Waiting Indicator (VMWI) with open switch interval (OSI). */
   CLASSSTM_TYPE_OSI_VMWI,

   /* Visual Message Waiting Indicator (VMWI) with dual-tone alerting signal (DTAS). */
   CLASSSTM_TYPE_DTAS_VMWI,

   /* Visual Message Waiting Indicator (VMWI) with ring-pulse alerting signal (RPAS). */
   CLASSSTM_TYPE_RPAS_VMWI,

   /* Visual Message Waiting Indicator (VMWI) with line-reversal alerting signal (LRAS). */
   CLASSSTM_TYPE_LRAS_VMWI

} CLASSSTM_TYPE;


/* CAS Events the Class State machine wishes to
** take temporary control over (block/unblock) */
typedef enum CLASSSTM_CASEVT_BLOCK
{
   CLASSSTM_CASEVT_BLOCK_NONE = 0,   /* Do not block any CAS events */
   CLASSSTM_CASEVT_BLOCK_ALL,        /* Block all CAS events */
   CLASSSTM_CASEVT_BLOCK_PULSE,      /* Block only pulse dial events */
} CLASSSTM_CASEVT_BLOCK;

/* call waiting state (SAS & CAS tones) for off-hook caller-id */
typedef struct CLASSSTM_CALLWTG_STATE
{
   int                     profile;                /* call-waiting tone profile index        */
   VRG_UINT16              delay;                  /* delay between call-waiting tones       */
   HDSP_TONE_PROV_PATTERN  casCustomProfile;       /* provisionable profile for CAS tone     */
   HDSP_TONE_PROV_PATTERN  sasCustomProfile;       /* provisionable profile for SAS tone     */
} CLASSSTM_CALLWTG_STATE;


/* structure of parameters passed into the class state machines */
typedef struct
{
   CAS_CTL_HANDLE          casCtlHandle;  /* CAS device ID                        */
   VRG_UINT16              vhdHdl;        /* handle of VHD associated with device */
   int                     lineId;        /* line id of associated VHD            */
   CLASSSTM_CASEVT_BLOCK  *evBlockp;      /* pointer to CAS event blocking flag   */
   VRG_COUNTRY             country;       /* country type                         */
   CLASS_MSG_TYPE          classType;     /* class feature type                   */
   CLASS_VMWI_TYPE         vmwiType;      /* vmwi feature type                    */
   CLASS_CLID_STATE       *clidInfo;      /* calling line id state                */
   CLASSSTM_RING_STATE    *ringInfo;      /* ring profile state                   */
   CLASSSTM_CALLWTG_STATE *callWtgInfo;   /* call waiting state                   */
   VRG_UINT32              fskAfterRing;  /* Time between ring signal and FSK     */
   VRG_UINT32              ringAfterFsk;  /* Time between FSK and ring signal     */
   VRG_UINT32              fskAfterDtas;  /* Time between DTAS and FSK            */
   VRG_UINT32              fskAfterRp;    /* Time between Ring Pulse and FSK      */
   VRG_UINT32              dtasAfterLr;   /* Time between Line Reversal and DTAS  */
   VRG_UINT32              clidProtocol;  /* Protocol used for caller ID          */
   VRG_UINT32              vmwiProtocol;  /* Protocol used for VMWI               */
   VRG_UINT32              vmwiMsgFormat; /* VMWI message format (SDMF/MDMF)      */
   CLASS_MSG_FSK_PARMS     *fskParmp;     /* Pointer to FSK parameters            */
   CLASS_MSG_DTMF_PARMS    *dtmfParmp;    /* Pointer to DTMF parameters           */

} CLASSSTM_PARM;


/* CLASS instance memory structure */
typedef struct
{
   const SME_STATE        *currentStatep; /* the current state of state machine   */
   const SME_STATE        *idleStatep;    /* the idle state of state machine      */
   CAS_CTL_HANDLE          casCtlHandle;  /* CAS device ID                        */
   VRG_UINT16              vhdHdl;        /* handle of VHD associated with device */
   CLASSSTM_CASEVT_BLOCK   evBlock;       /* CAS event blocking flag              */
   VRG_COUNTRY             country;       /* country type                         */
   CLASS_MSG_TYPE          classType;     /* class feature type                   */
   CLASS_VMWI_TYPE         vmwiType;      /* vmwi feature type                    */
   CLASS_CLID_STATE        clidInfo;      /* calling line id state                */
   CLASSSTM_RING_STATE     ringInfo;      /* ring profile state                   */
   CLASSSTM_CALLWTG_STATE  callWtgInfo;   /* call waiting state                   */
   VRG_UINT32              fskAfterRing;  /* Time between ring signal and FSK     */
   VRG_UINT32              ringAfterFsk;  /* Time between FSK and ring signal     */
   VRG_UINT32              fskAfterDtas;  /* Time between DTAS and FSK            */
   VRG_UINT32              fskAfterRp;    /* Time between Ring Pulse and FSK      */
   VRG_UINT32              dtasAfterLr;   /* Time between Line Reversal and DTAS  */
   VRG_UINT32              clidProtocol;  /* Protocol used for caller ID          */
   VRG_UINT32              vmwiProtocol;  /* Protocol used for VMWI               */
   VRG_UINT32              vmwiMsgFormat; /* VMWI message format (SDMF/MDMF)      */
   CLASS_MSG_FSK_PARMS     *fskParmp;     /* Pointer to FSK parameters            */
   CLASS_MSG_DTMF_PARMS    *dtmfParmp;    /* Pointer to DTMF parameters           */

} CLASSSTM_MEM;


/* events specific to this module (non-Hausware) and only used in this module */
enum
{
   CLASSSTM_CLID_RING,
   CLASSSTM_CLID_NODATA_RING,
   CLASSSTM_CLID_SAS_CAS,
   CLASSSTM_CLID_SAS_ONLY,
   CLASSSTM_CLID_SAS_CAS_OFF,
   CLASSSTM_CLID_ACK_RCVD,
   CLASSSTM_TONE_GEN_EVT,
   CLASSSTM_CLID_INFO,
   CLASSSTM_CLID_TX_END,
   CLASSSTM_CLID_FIRST_RINGOFF,

   CLASSSTM_OFFHOOK,
   CLASSSTM_ONHOOK,
   CLASSSTM_FLASH,

   CLASSSTM_VMWI_ON,
   CLASSSTM_VMWI_OFF,
   CLASSSTM_VMWI_OSI_END,

   CLASSSTM_RING_OFF,
   CLASSSTM_TIMER_EXPIRE
};

/* function protypes */
VRG_SINT16 classStmInit
(
   CLASSSTM_MEM  *stmMemp,
   CAS_CTL_HANDLE casCtlHandle,
   VRG_UINT16     vhdhdl,
   VRG_COUNTRY    country
);

void   classStmDeinit( CLASSSTM_MEM *stmMemp );
void   classStmReset( CLASSSTM_MEM *clidState, CLASSSTM_TYPE classType );
void   classStmEventProcess( CLASSSTM_MEM *clidState, int event, int lineId );
VRG_BOOL classStmIsIdle( CLASSSTM_MEM *stmMemp );

#endif   /* CLASSSTM_H */
