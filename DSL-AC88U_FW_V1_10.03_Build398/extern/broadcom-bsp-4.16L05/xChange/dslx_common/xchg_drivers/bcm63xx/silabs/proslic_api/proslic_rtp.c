/****************************************************************************
*
*  Copyright (c) 2000-2015 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
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
*/

/**************************************************************************
* File Name  : proslic_rtp.c
*
* Description: This file contains the implementation for the BCM63xx board
*              parameter voice access functions.
*
***************************************************************************/
#include "proslic.h"
#include "proslic_rtp.h"
#include "proslic_api_config.h"

#ifdef SI3217X
#include "si3217x.h"
#include "si3217x_intf.h"
#include "si3217x_intf_rtp.h"
#endif
#ifdef SI3226X
#include "si3226x.h"
#include "si3226x_intf.h"
#include "si3226x_intf_rtp.h"
#endif

int ProSLIC_ReadRTPStatus (proslicChanType_ptr hProslic,uInt8 *pRTPStat){
   if(hProslic->channelType != PROSLIC) return RC_IGNORE;
#ifdef SI3217X
   if(hProslic->deviceId->chipType >= SI32171 && hProslic->deviceId->chipType <= SI32179)
     return Si3217x_ReadRTPStatus( hProslic,pRTPStat );
#endif
#ifdef SI3226X
   if (hProslic->deviceId->chipType >= SI32260 && hProslic->deviceId->chipType <= SI32269)
     return Si3226x_ReadRTPStatus( hProslic,pRTPStat );
#endif
   *pRTPStat = PROSLIC_LCRRTP_CLEAR;
   return RC_IGNORE;
}

int ProSLIC_ClearRTPStatus ( proslicChanType_ptr hProslic ){
   if(hProslic->channelType != PROSLIC) return RC_IGNORE;
#ifdef SI3217X
   if(hProslic->deviceId->chipType >= SI32171 && hProslic->deviceId->chipType <= SI32179)
     return Si3217x_ClearRTPStatus( hProslic );
#endif
#ifdef SI3226X
   if (hProslic->deviceId->chipType >= SI32260 && hProslic->deviceId->chipType <= SI32269)
     return Si3226x_ClearRTPStatus( hProslic );
#endif
   return RC_IGNORE;
}

int ProSLIC_GetLfRegId ( proslicChanType_ptr hProslic ){
   if(hProslic->channelType != PROSLIC)
   {
     return -1;
   }
#ifdef SI3217X
   if(hProslic->deviceId->chipType >= SI32171 && hProslic->deviceId->chipType <= SI32179)
     return Si3217x_GetLfRegId();
#endif
#ifdef SI3226X
   if (hProslic->deviceId->chipType >= SI32260 && hProslic->deviceId->chipType <= SI32269)
     return Si3226x_GetLfRegId();
#endif
   return -1;
}