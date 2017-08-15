/****************************************************************************
*
*  Copyright (c) 2000-2009 Broadcom Corporation
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
****************************************************************************
*    Filename: boardHal63268DectI.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-dependent DECT interface for
*     the 63268 internal DECT.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <boardHalSlic.h>
#include <vrgLog.h>

#include <xchgAssert.h>
#include "boardHalDect63268I.h"
#include <dectLineIf.h>
#include "boardHal63268vcm.h"
#include <dectI63268.h>

#include <boardparms_voice.h>
#include <boardHalProv.h>

static DECT_LINE_IF_DRV gDectDriver[] =
{ 
  {NULL, 0},
  {NULL, 1},
  {NULL, 2},
  {NULL, 3}, 
};


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* ---- Private Variables ------------------------------------------------ */

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalDectInit
**
** PURPOSE:    Initialize the DECT
**
** PARAMETERS: 
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalDectInit( VRG_BOOL arg )
{
   unsigned int i =0;
   DECT_LINE_IF_DRV *pDev;
   DECT_STATUS status = DECT_SUCCESS;
   int deviceId = 0;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();
   
   
   for (i=0; i<voiceParams->numDectLines; i++)
   {
     pDev = &( gDectDriver[i] );
     dectLineIfInitFuncs( pDev ); 
   }
   
   /* Initialize dect specific hardware settings */
   return (0); // dectI63268_Init() );
 
}


/*
*****************************************************************************
** FUNCTION:   boardHalDectDeinit
**
** PURPOSE:    Shutdown the DECT
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalDectDeinit( void )
{
   DECT_STATUS status = DECT_SUCCESS;
   if ( dectLineIfDeinit() ==  DECT_ERROR)
   {
      //dectI63268_DeInit();
      return ( 1 );
   }
   else
   {
     return (0); // dectI63268_DeInit() );
   }
}

/*
*****************************************************************************
** FUNCTION:   boardHalDectGetDriver
**
** PURPOSE:    Get the DECT driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the DECT driver interface
**
*****************************************************************************
*/
XDRV_SLIC* boardHalDectGetDriver( int chan )
{
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap = boardHalProvGetChannelMap( chan );

   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "     ***** %s %d chan = %d deviceChannelType = %d ", __FUNCTION__, __LINE__, chan, pDeviceChannelMap->deviceChannelType));

   XCHG_ASSERT( pDeviceChannelMap->deviceChannelType == BP_VCTYPE_DECT );
   return ( (XDRV_SLIC*)&( gDectDriver[chan] ) );
}

/*
*****************************************************************************
** FUNCTION:   boardHalIsDect
**
** PUPOSE:     Indicate which Channels are connected to Dects
**
** PARAMETERS: Channel number
**
** Returns:    True if Dect, false if not
**
*****************************************************************************
**/
#error test
XDRV_BOOL boardHalIsDect( int chan )
{
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap = boardHalProvGetChannelMap( chan );

   if ( pDeviceChannelMap->deviceChannelType == BP_VCTYPE_DECT )
   {
     return XDRV_TRUE;
   }
   else
   {
     return XDRV_FALSE;
   }
}

