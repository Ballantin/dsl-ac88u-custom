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
*    Filename: boardHalDect.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-dependent DECT interface for DECT.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>

#include <xchgAssert.h>

#ifdef BRCM_NONDIST_BUILD
#include <hal_dectI.h>
#endif 

#include <boardHalSlic.h>
#include "boardHalDect.h"
#include <boardparms_voice.h>
#include <boardHalProv.h>

#ifdef BRCM_6368
#define UART1_OUTPUT_PIN 31
#define UART1_INPUT_PIN 30
#endif


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
   return (0); 
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
   return ( 0 );
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
   return (NULL);   
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

#ifdef BRCM_NONDIST_BUILD
/*
NOTE: may need to include other wrappers for functions in hal_dectI.c:
halDectIRegisterDefault
halDectIRegisterCustom  
halDectIDefaultDectBuffProcessing
halDectISetDectBuffProcessing
halDectIGetDectBuffProcessingFlag
halDectIResetDectBuffProcessingFlag
*/
/*
*****************************************************************************
** FUNCTION:   boardHalDectIStartDectBuffProcessing
**
** PURPOSE:    Invoke halDectIStartDectBuffPRocessing() in hal_dectI.c
**
** PARAMETERS: None. 
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
void boardHalDectIStartDectBuffProcessing( XDRV_UINT32 ddrAddrTxV, XDRV_UINT32 ddrAddrRxV )
{
   halDectIStartDectBuffProcessing( ddrAddrTxV, ddrAddrRxV);
}

/*
*****************************************************************************
** FUNCTION:   boardHalDectIResetDectBuffProcessing
**
** PURPOSE:    Invoke halDectIStartDectBuffPRocessing() in hal_dectI.c
**
** PARAMETERS: None. 
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
void boardHalDectIStopDectBuffProcessing( void )
{
   halDectIStopDectBuffProcessing();
}



#endif

