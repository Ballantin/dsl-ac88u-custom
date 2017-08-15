/****************************************************************************
*
* Copyright (c) 2008-2014 Broadcom
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
*    Filename: zarlinkCommonTpd.h
*    Creation Date: 04 Oct 2000 (v0.00)
*    VSS Info:
*        $Revision: 10 $
*        $Date: 11/14/02 4:02p $
*
****************************************************************************
*    Description:
*
*      This file contains functions related to linetests for the Zarlink
*      slic on the 33xx reference designs
*
****************************************************************************/

#ifndef ZARLINK_COMMON_TPD_INT_API_H
#define ZARLINK_COMMON_TPD_INT_API_H

#if defined(__cplusplus)
extern "C" {
#endif

#if XDRV_ZARLINK_TPD_SUPPORT

#include <lt_api.h>

/*
*****************************************************************************
** FUNCTION:   tpdGetTestCtx
**
** PURPOSE:    Retrieve the test structure for the selected channel
**
** PARAMETERS: chan - channel in use
**
** RETURNS:    pointer to test structure
**
*****************************************************************************
*/
LtTestCtxType* tpdGetTestCtx( unsigned int chan );

/*
*****************************************************************************
** FUNCTION:   tpdUpdateTestStatus
**
** PURPOSE:    Function for event handler to post test status updates
**
** PARAMETERS: chan - channel in use
**
** RETURNS:    pointer to test structure
**
*****************************************************************************
*/
void tpdUpdateTestStatus( LtTestStatusType status, uint8 channelId, uint16 eventId ); 

#endif

#if defined(__cplusplus)
}
#endif

#endif   /* ZARLINK_COMMON_TPD_INT_API_H */

