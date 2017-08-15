/****************************************************************************
*
* Copyright © 2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
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
*    Filename: boardHalTpd6838.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware dependent Tpd driver interface for
*     the 6838.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgLog.h>
#include <xchgAssert.h>
#include <boardHalTpd.h>
#include <boardHalSlic.h>
#include <boardHalApm.h>
#include <boardHalDsp.h>
#include "boardHal6838.h"
#include <boardHalProv.h>


/* ---- Hardware Specific Include Files ---------------------------------- */
#include <tpd6838L9540.h>
#include <tpd6838Si32392.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
static XDRV_TPD gTpdDriver;

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalApmTpdInit
**
** PURPOSE:    This function initializes the Tpd service
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmTpdInit( void )
{
   XDRV_TPD             *tpdDrv;
   XDRV_SLIC            *slicDrv;
   XDRV_APM             *apmDrv;
   XDRV_DEBUG           *pDebugDriver;
   int                   endpt;

   for ( endpt = 0; endpt < BOARD_HAL_6838_NUM_ENDPTS; endpt++ )
   {
      tpdDrv = &gTpdDriver; 
      apmDrv = boardHalApmGetDriver( 0 );
      slicDrv = boardHalSlicGetDriver( endpt );
      pDebugDriver = boardHalDspGetDebugDriver();

      if(( !slicDrv ) || ( !apmDrv ) || ( !pDebugDriver ))
      {
         return ( -1 );
      }

      VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();
      switch ( voiceParams->voiceDevice[0].voiceDeviceType )/* TPD on APM Device 0 only */
      {

         case BP_VD_ZARLINK_9540:
         {
           if ( tpd6838L9540InitChannel( endpt, tpdDrv, slicDrv, apmDrv, pDebugDriver ) )
           {
              VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "L9540 TPD init FAILED" ));
              return ( -2 );
           }
         }
         break;

         case BP_VD_SILABS_32392:
         {
           if ( tpd6838Si32392InitChannel( endpt, tpdDrv, slicDrv, apmDrv, pDebugDriver ) )
           {
              VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "Si32392 TPD init FAILED" ));
              return ( -3 );
           }
         
         }
         break;
         
         default:
         {
             VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Unsupported SLIC, TPD init FAILED" ));
             return ( -4 );
         }
         break;
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalApmTpdDeinit
**
** PURPOSE:    This function de-initializes Tpd service
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmTpdDeinit( void )
{
   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalTpdGetDriver
**
** PURPOSE:    Get the TPD driver
**
** PARAMETERS: none
**
** RETURNS:    pointer to the TPD driver interface
**
*****************************************************************************
*/
XDRV_TPD* boardHalTpdGetDriver( void )
{
   return ( (XDRV_TPD*)&gTpdDriver );
}

