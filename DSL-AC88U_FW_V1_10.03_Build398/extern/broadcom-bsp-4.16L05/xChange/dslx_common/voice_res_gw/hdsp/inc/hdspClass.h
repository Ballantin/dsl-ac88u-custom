/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
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
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
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
*    Filename: hdspclass.h
*
****************************************************************************
*    Description:
*
*      This file contains the prototypes for the class transmitter
*      functionality
*
****************************************************************************/

#ifndef _HDSPCLASS_H
#define _HDSPCLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vrgTypes.h>
#include <vrgCountry.h>
#include <vrgClass.h>
#include <hapi_clidrx.h>

/* Visual Message Waiting Indicator Formats TR-NWT-001401 */
#define VMWI_MODE_MDMF  1
#define VMWI_MODE_SDMF  0

/*
** Delay between the end of first ring and the start of FSK generation.
*/
#define CLASS_CLID_ONHOOK_DELAY_DEFAULT         750
#define CLASS_CLID_ONHOOK_DELAY_DEFAULT_JAPAN   500

/*
** Off-hook caller id delay. This defines the time between when the DSP receives
** the CLIDTX start command, and when it actually starts transmitting.
*/
#define CLASS_CLID_OFFHOOK_DELAY_DEFAULT         100

/* Class Types */
typedef enum CLASS_MSG_TYPE
{
   /* Class I, onhook, caller-id. */
   CLASS_MSG_ONHOOK_CLID = 1,

   /* Class II, offhook, caller-id. */
   CLASS_MSG_OFFHOOK_CLID,

   /* Visual Message Waiting Indicator (VMWI). */
   CLASS_MSG_VMWI

} CLASS_MSG_TYPE;


/* Class Types */
typedef enum
{
   CLASS_SIG_TYPE_FSK_CLID = 1,           /* Class I FSK Caller ID */
   CLASS_SIG_TYPE_DTMF_CLID,              /* Class I DTMF Caller ID */
   CLASS_SIG_TYPE_FSK_VMWI,               /* VMWI FSK Signalling */
   CLASS_SIG_TYPE_DTMF_VMWI               /* VMWI DTMF Signalling */

} CLASS_SIG_TYPE;


/* VMWI Types */
typedef enum
{
   CLASS_VMWI_ACTIVATE = 1,      /* Activate Visual Message Waiting Indicator */
   CLASS_VMWI_DEACTIVATE         /* De-activate Visual Message Waiting Indicator */

} CLASS_VMWI_TYPE;


#define  CLASS_DATE_SIZE         8
#define  CLASS_MAX_NUMBER_SIZE   20

#define  CLASS_MAX_NAME_SIZE     15

/* increase name buffer size to 50 according to ETSI300 659-3 requirement */
//#define  CLASS_MAX_NAME_SIZE     50

#define  CLASS_OH                3*6 + 3  // for Msg headers and check-sum 
#define  CLASS_STRING_SIZE (CLASS_DATE_SIZE + CLASS_MAX_NUMBER_SIZE + CLASS_MAX_NAME_SIZE + CLASS_OH)


typedef struct
{
   char  date[CLASS_DATE_SIZE + 1];          /* date string */
   char  number[CLASS_MAX_NUMBER_SIZE + 1];  /* phone number string */
   char  reasonNoNumber;                     /* number block character */
   char  name[CLASS_MAX_NAME_SIZE + 1];      /* name string */
   char  reasonNoName;                       /* name block character */

} CLASS_CLID_STATE;


#if 0
typedef struct
{
   char  date[ CLASS_DATE_SIZE + 1 ];          /* date string */
   char  number[ CLASS_MAX_NUMBER_SIZE + 1 ];  /* phone number string */
   char  reasonNoNumber;                     /* number block character */
   char  name[ CLASS_MAX_NAME_SIZE + 1 ];      /* name string */
   char  reasonNoName;                       /* name block character */
} HDSP_CLASS_CLIDRX_DATA;
#else
#define HDSP_CLASS_CLIDRX_DATA         CLASS_CLID_STATE
#endif

typedef struct
{
   VRG_UINT16 numMark;
   VRG_UINT16 numSeizure;
   VRG_UINT16 msgLen;
   VRG_UINT8  parseBuf[ CLASS_STRING_SIZE ];
   VRG_UINT8 *parseBufp;
   HDSP_CLASS_CLIDRX_DATA clidRxData;
} HDSP_CLASS_CLIDRX_STATE;

/* Provision-able parameters */
typedef VRG_CLASS_FSK  CLASS_MSG_FSK_PARMS;
typedef VRG_CLASS_DTMF CLASS_MSG_DTMF_PARMS;

/*
*****************************************************************************
** EXTERNAL FUNCTION PROTOTYPES
*****************************************************************************
*/

VRG_SINT16 classRxConfigure( VRG_UINT16 vhdHdl, CLASS_MSG_TYPE classtype, VRG_COUNTRY country );

VRG_SINT16 hdspClassMessage
(
   CLASS_CLID_STATE *clidInfo,
   VRG_UINT16        vhdHdl,
   int               lineId,
   VRG_COUNTRY       country,
   CLASS_MSG_TYPE    classtype,
   CLASS_MSG_FSK_PARMS *fskParmp,
   VRG_UINT16        clidTxDelay
);

VRG_SINT16 hdspClassParseStr
(
   const char       *clidStr,
   VRG_COUNTRY       country,
   CLASS_CLID_STATE *clidInfo
);

VRG_SINT16 hdspClassVmwi
(
   VRG_UINT16  vhdHdl,
   int         lineId,
   VRG_UINT16  type,
   VRG_UINT16  format,
   CLASS_MSG_FSK_PARMS *fskParmp,
   VRG_COUNTRY country
);

VRG_SINT16 hdspClassTransmit( VRG_UINT16 vhdHdl );
VRG_SINT16 hdspClassStopTransmit( VRG_UINT16 vhdHdl );
VRG_SINT16 hdspClassDtmfTransmit( VRG_UINT16 vhdHdl, CLASS_CLID_STATE *clidInfo, CLASS_MSG_DTMF_PARMS *dtmfParmp );
VRG_SINT16 hdspClassDtmfTransmitVmwi( VRG_UINT16 vhdHdl, CLASS_VMWI_TYPE vmwiType, CLASS_MSG_DTMF_PARMS *dtmfParmp );
VRG_SINT16 hdspClassDtmfStopTransmit( VRG_UINT16 vhdHdl );

VRG_SINT16 hdspClassEnable( VRG_UINT16 vhdHdl );
VRG_SINT16 hdspClassDisable( VRG_UINT16 vhdHdl );
VRG_BOOL   hdspClidRxParse( VRG_COUNTRY country, HSZCLIDRXPACKET *clidRxPktp, HDSP_CLASS_CLIDRX_STATE *clidRxStatep );

#ifdef __cplusplus
}
#endif

#endif /* _HDSPCLASS_H */

