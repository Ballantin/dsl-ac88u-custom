/****************************************************************************
*
*  Copyright (c) 2002-2008 Broadcom
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
*    Filename: cmtdCtl.h
*
****************************************************************************
*    Description:
*
*     This module contains the state machine definitions for CLASS features
*
****************************************************************************/

#ifndef CMTDCTL_H
#define CMTDCTL_H

#include <vrgTypes.h>
#include <vrgCountry.h>

typedef enum
{
    CMTD_TONE_TYPE_INVALID = -1,
    CMTD_TONE_TYPE_DIAL,
    CMTD_TONE_TYPE_RING,
    CMTD_TONE_TYPE_BUSY,
    CMTD_TONE_TYPE_CONG,
    CMTD_TONE_TYPE_RELEASE,
    CMTD_TONE_TYPE_MAX
} CMTD_TONE_TYPE;

typedef struct
{
    /* assumes we only need silence + 1 programmable filter per tone */
    VRG_UINT16 filtSize;
    VRG_SINT16 *filtCoeffp;
    int numSubStates;
    HAPI_CMTD_STM stmParam;
} CMTD_STMPARAMS;

typedef enum
{
   CMTD_FILTER_1 = 0,       /* Index for filter 1 */
   CMTD_FILTER_2 = 1,       /* Index for filter 2 */
   CMTD_FILTER_MAX = 2      /* Maximum number of filters supported by CMTD srv */
} CMTD_FILTER_INDEX;

HAPIRESULT cmtdCtlConfigFilter( VRG_UINT16 hdl, VRG_COUNTRY locale, CMTD_TONE_TYPE toneType, CMTD_FILTER_INDEX filterIndex );
HAPIRESULT cmtdCtlDisable( VRG_UINT16 hdl );
CMTD_TONE_TYPE cmtdCtlGetConfigTone( CMTD_FILTER_INDEX filterIndex );
const char* cmtdCtlMapToneType( CMTD_TONE_TYPE tone );

#endif   /* CMTDCTL_H */
