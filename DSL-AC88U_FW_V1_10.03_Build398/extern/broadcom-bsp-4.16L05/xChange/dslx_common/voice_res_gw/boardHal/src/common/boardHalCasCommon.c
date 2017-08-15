/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
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
*    Filename: boardHalCas.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent CAS driver interface
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <boardparms_voice.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <boardHalCas.h>
#include <boardHalSlic.h>
#include <boardHalApm.h>
#include <casDriver.h>
#include <boardHalProv.h>
#include <boardHalDect.h>
#include <globalCfg.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* The CAS task will perform state machine processing at the 2ms rate */
#define CAS_BLOCK_RATE_MS                 2


/* ---- Private Variables ------------------------------------------------ */

static XDRV_CAS gCasDriver[ GLOBAL_CFG_MAX_ENDPT ];


/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalCasInit
**
** PURPOSE:    This function initializes the CAS drivers
**
** PARAMETERS: fastSlicSupported (in) True if fast SLIC mode changing should
**                                    be supported on the platform.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalCasInit( VRG_BOOL bFastSlicSupported, VRG_BOOL bCasHistorySupported )
{
   int   endpt;
   int   result;
   XDRV_SLIC *slicDrv = NULL;
   XDRV_APM *apmDrv   = NULL;
   
   apmDrv = boardHalApmGetDriver( 0 );
   /*Check validity of pointers */
   if( !apmDrv )
   {
         VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "BoardHalCas - failed: Bad APM drv !!!"));
         return ( 1 );
   }

   for ( endpt = 0; endpt < boardHalGetNumEndpoints(); endpt++ )
   {
      if ( boardHalIsSlic(endpt) ) 
      {
         slicDrv = boardHalSlicGetDriver( endpt );
      }
      else
      {
         VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "BoardHalCas - : endpt(%d) Not a SLIC !!!", endpt ));
         continue;
      }

      /*Check validity of pointers */
      if( !slicDrv )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "BoardHalCas - failed: Bad SLIC driver !!!"));
         return ( 1 );
      }

      /* Initialize CAS driver */
      result = casDriverInit( &gCasDriver[ endpt ],
                              slicDrv,
                              apmDrv,
                              endpt,
                              bFastSlicSupported,
                              bCasHistorySupported );
      if( result != 0 )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "BoardHalCas - CasDriverInit failed !!!"));
         return ( result );
      }

      if( bFastSlicSupported )
      {
         /* Set fast SLIC mode to on state initially */
         xdrvCasEnableFastSlicStateMode( &gCasDriver[ endpt ], VRG_TRUE );
      }
   }

   if ( bCasHistorySupported )
   {
      /* Start CAS history service */
      if (casDriverStartHistoryLog( CAS_BLOCK_RATE_MS, BOARD_HAL_VCM_SW_DEBOUNCE_MS ))
      {
         return ( 1 );
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalCasDeinit
**
** PURPOSE:    This function de-initializes CAS drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalCasDeinit( void )
{
   int  endpt;
   int  result;

   casDriverStopHistoryLog();

   for ( endpt = 0; endpt < boardHalGetNumEndpoints(); endpt++ )
   {
      if (( result = casDriverDeinit( &gCasDriver[ endpt ] )) != 0 )
      {
         return ( 1 );
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalGetHookStateDebounceRate
**
** PURPOSE:    Get overall hookstate debounce rate for this platform
**             (this is both CAS and SLIC/hw combined)
**
** PARAMETERS: none
**
** RETURNS:    Hook state debounce interval in ms.
**
*****************************************************************************
*/
int boardHalGetHookStateDebounceRate( void )
{
   return( BOARD_HAL_VCM_HW_DEBOUNCE_MS + BOARD_HAL_VCM_SW_DEBOUNCE_MS );
}

/*
*****************************************************************************
** FUNCTION:   boardHalCasGetDriver
**
** PURPOSE:    Get the CAS driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the CAS driver interface
**
*****************************************************************************
*/
XDRV_CAS* boardHalCasGetDriver( int chan )
{
   XCHG_ASSERT( ( 0 <= chan ) && ( chan < boardHalGetNumEndpoints() ) );

   return ( (XDRV_CAS*)&(gCasDriver[chan]) );
}

