/****************************************************************************
*  
*  Copyright (c) 2000-2012 Broadcom
*
*    This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.  
*
*    Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: eptSig.h
*
****************************************************************************
*    Description:
*
*      Public endpoint interface for signal-related parameters
*
****************************************************************************/

#ifndef EPT_SIG_H
#define EPT_SIG_H

#if defined(__cplusplus)
extern "C"
{
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <bosTypes.h>

/* ---- Constants and Types ---------------------------------------------- */

#define  RING_NO_INTERLEAVE   0xABBA

/* Only used by E-Package */
#define MAX_CADENCE_RING      128

#define EPT_SIG_PARM_UNUSED   0xFFFF

/*
** Signals generated
*/
typedef enum
{
   eEPTSIG_NULL,                      /* internal use: Null signal */
   eEPTSIG_BUSY,                      /* busy tone */
   eEPTSIG_CONF,                      /* confirmation tone */
   eEPTSIG_DIAL,                      /* dial tone */
   eEPTSIG_MSGW,                      /* message waiting indicator tone */
   eEPTSIG_OHWARN,                    /* off-hook warning tone */
   eEPTSIG_RINGING,                   /* enable ringer */
   eEPTSIG_RINGINT,                   /* enable ringer for internal call */
   eEPTSIG_RING0,                     /* distinctive ringing 0 */
   eEPTSIG_RING1,                     /* distinctive ringing 1 */
   eEPTSIG_RING2,                     /* distinctive ringing 2 */
   eEPTSIG_RING3,                     /* distinctive ringing 3 */
   eEPTSIG_RING4,                     /* distinctive ringing 4 */
   eEPTSIG_RING5,                     /* distinctive ringing 5 */
   eEPTSIG_RING6,                     /* distinctive ringing 6 */
   eEPTSIG_RING7,                     /* distinctive ringing 7 */
   eEPTSIG_CALLID_RINGING,            /* enable ringer, pending callid */
   eEPTSIG_CALLID_RINGINT,            /* enable ringer, pending callid for internal call */
   eEPTSIG_CALLID_RING0,              /* distinctive ring 0, pending callid */
   eEPTSIG_CALLID_RING1,              /* distinctive ring 1, pending callid */
   eEPTSIG_CALLID_RING2,              /* distinctive ring 2, pending callid */
   eEPTSIG_CALLID_RING3,              /* distinctive ring 3, pending callid */
   eEPTSIG_CALLID_RING4,              /* distinctive ring 4, pending callid */
   eEPTSIG_CALLID_RING5,              /* distinctive ring 5, pending callid */
   eEPTSIG_CALLID_RING6,              /* distinctive ring 6, pending callid */
   eEPTSIG_CALLID_RING7,              /* distinctive ring 7, pending callid */
   eEPTSIG_REORDER,                   /* reorder tone */
   eEPTSIG_SPLASH,                    /* ring splash tone */
   eEPTSIG_RINGBACK,                  /* ringback tone */
   eEPTSIG_STUTTER,                   /* stutter dial tone */
   eEPTSIG_CONF_STUTTER,              /* Confirmation tone followed by stutter */
   eEPTSIG_CALLW1,                    /* call waiting tone 1 */
   eEPTSIG_CALLW2,                    /* call waiting tone 2 */
   eEPTSIG_CALLW3,                    /* call waiting tone 3 */
   eEPTSIG_CALLW4,                    /* call waiting tone 4  */
   eEPTSIG_SAS_CAS1,                  /* combined SAS and CAS tone for CLASS2 */
   eEPTSIG_SAS_CAS2,                  /* combined SAS and CAS tone for CLASS2 */
   eEPTSIG_SAS_CAS3,                  /* combined SAS and CAS tone for CLASS2 */
   eEPTSIG_SAS_CAS4,                  /* combined SAS and CAS tone for CLASS2 */
   eEPTSIG_VMWI,                      /* visual message waiting indicator */
   eEPTSIG_REV,                       /* phase reversal */
   eEPTSIG_DTMF0,                     /* DTMF Tone 0 */
   eEPTSIG_DTMF1,                     /* DTMF Tone 1 */
   eEPTSIG_DTMF2,                     /* DTMF Tone 2 */
   eEPTSIG_DTMF3,                     /* DTMF Tone 3 */
   eEPTSIG_DTMF4,                     /* DTMF Tone 4 */
   eEPTSIG_DTMF5,                     /* DTMF Tone 5 */
   eEPTSIG_DTMF6,                     /* DTMF Tone 6 */
   eEPTSIG_DTMF7,                     /* DTMF Tone 7 */
   eEPTSIG_DTMF8,                     /* DTMF Tone 8 */
   eEPTSIG_DTMF9,                     /* DTMF Tone 9 */
   eEPTSIG_DTMFS,                     /* DTMF Tone * */
   eEPTSIG_DTMFH,                     /* DTMF Tone # */
   eEPTSIG_DTMFA,                     /* DTMF Tone A */
   eEPTSIG_DTMFB,                     /* DTMF Tone B */
   eEPTSIG_DTMFC,                     /* DTMF Tone C */
   eEPTSIG_DTMFD,                     /* DTMF Tone D */
   eEPTSIG_CALLID,                    /* Caller ID message */
   eEPTSIG_CALLCARD,                  /* Calling Card Service tone */
   eEPTSIG_NETBUSY,                   /* Network busy tone */
   eEPTSIG_OSI,                       /* Open switch interval */
   eEPTSIG_RINGBACK_CUST1,            /* Custom ringback tone 1 */
   eEPTSIG_SPECIAL_DIAL,              /* Special dial tone */
   eEPTSIG_SB_DISCONNECT,             /* Disconnect switchboard */
   eEPTSIG_INGRESS_DTMF,              /* Generate ingress DTMF tone */
   eEPTSIG_DENY,                      /* Deny tone (opposite to Confirm tone) */

/*
** E-line package-specific defines
*/

   /*
   ** Pulsed Signals encoding request
   */
   eEPTSIG_INIT_RING,
   eEPTSIG_LOOP_CLOSE,
   eEPTSIG_LOOP_OPEN,
   eEPTSIG_ENABLE_METERING,
   eEPTSIG_METERING_BURST,
   eEPTSIG_NO_BATTERY,
   eEPTSIG_NORMAL_POLARITY,
   eEPTSIG_REDUCED_BATTERY,
   eEPTSIG_REVERSE_POLARITY,

   /*
   ** Steady signals
   */
   eEPTSIG_STEADY_FULL_BATTERY,
   eEPTSIG_STEADY_LOOP_CLOSE,
   eEPTSIG_STEADY_LOOP_OPEN,
   eEPTSIG_STEADY_NO_BATTERY,
   eEPTSIG_STEADY_NORMAL_POLARITY,
   eEPTSIG_STEADY_REDUCED_BATTERY,
   eEPTSIG_STEADY_REVERSED_POLARITY,

   /*
   ** Battery indicator signals
   */
   eEPTSIG_RF_LOSS,
   eEPTSIG_ENDPT_DISABLED,
   eEPTSIG_ENDPT_DISCONNECTED,
   eEPTSIG_AC_LOSS,
   eEPTSIG_LOW_BATTERY,
   eEPTSIG_BAD_BATTERY,
   eEPTSIG_OVER_TEMP_BATTERY,


   /*
   ** Cadence Ringing base enum (i.e. cr(0))
   */
   eEPTSIG_CADENCE_RING_BASE,

   /*** DO NOT insert anything here!! ***/

   /* Cadence Ringing max enum (i.e. cr(127)) */
   eEPTSIG_CADENCE_RING_MAX = ( eEPTSIG_CADENCE_RING_BASE + MAX_CADENCE_RING - 1),

/*
** end of E-line package-specific defines
*/

   eEPTSIG_LAST

} EPTSIG;

/* 
** Parameter associated with signal processing reqeuests
 */
typedef struct EPTSIGPARM
{ 
   /* Defines the signal characteristics.  This can be on or off, or it
   ** can point to a NULL-terminated string value for caller ID generation */
   unsigned int   uValue;

   /* Signal duration.  Specify EPT_SIG_PARM_UNUSED if not used */
   BOS_UINT16     uDuration;

   /* Signal period.  Specify EPT_SIG_PARM_UNUSED if not used */
   BOS_UINT16     uPeriod;

   /* Signal repeition.  Specify EPT_SIG_PARM_UNUSED if not used */
   BOS_UINT16     uRepetition;

} EPTSIGPARM;


#if defined(__cplusplus)
}
#endif

#endif  /* EPT_SIG_H  */


