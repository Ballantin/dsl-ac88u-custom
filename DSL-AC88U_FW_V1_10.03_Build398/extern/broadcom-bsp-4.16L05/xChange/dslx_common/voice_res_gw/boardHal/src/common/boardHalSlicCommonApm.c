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
*    Filename: boardHalSlicCommonApm.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent SLIC interface for APM
*     
*     The terms SLIC and SLAC are used interchangeably in the comments
*     or variables names in this file.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bcmChip.h>
#include <boardparms_voice.h>
#include <vrgTypes.h>
#include <kernelMainWrapper.h>
#include <mspiDef.h>
#include <vrgLog.h>
#include <xchgAssert.h>

#include <boardHalInit.h>
#include "boardHal6xxx.h"
#include <boardHalSlic.h>
#include <boardHalProv.h>
#include <boardHalApm.h>


#ifdef BRCM_6816
   #include <boardHal6816.h>
   #include <boardHalSlic6816uni.h>
#endif

#ifdef BRCM_6828
   #include <boardHal6828.h>
   #include <boardHalSlic6828uni.h>
#endif

#ifdef BRCM_6818
   #include <boardHal6818.h>
   #include <boardHalSlic6818uni.h>
#endif

#ifdef BRCM_6838
   #include <boardHal6838.h>
   #include <boardHalSlic6838uni.h>
#endif

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalSlicInitApm
**
** PURPOSE:    Initialize the SLIC
**
** PARAMETERS: boardHalInitCfg   [IN]  -  Init configuration flags
**                                        (currently used only to fetch country code)
**             bHighVring        [IN]  -  NOT USED
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicInitApm( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL bHighVring )
{
   int deviceId;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {
#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818)
         case BP_VD_ZARLINK_9530:
         {
            return boardHalSlicInit9530(boardHalInitCfg, bHighVring);
         }
         break;

         case BP_VD_SILABS_3239:
         {
            return boardHalSlicInit3239(boardHalInitCfg, bHighVring);
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) */

#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)         
         case BP_VD_ZARLINK_9540:
         case BP_VD_ZARLINK_9541:
         {
            return boardHalSlicInit9540(boardHalInitCfg, bHighVring);
         }
         break;                  

#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838) */

#if defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_SILABS_32392:
         {
            return boardHalSlicInit32392(boardHalInitCfg, bHighVring);
         }
         break;
#endif
         default:
         break;
      }
   }

   return (0);
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicDeinitApm
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicDeinitApm( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg )
{
   int deviceId;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {
#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818)
         case BP_VD_ZARLINK_9530:
         {
            return boardHalSlicDeinit9530(boardHalInitCfg);
         }
         break;
         
         case BP_VD_SILABS_3239:
         {
            return boardHalSlicDeinit3239(boardHalInitCfg);
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) */

#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_ZARLINK_9540:
         case BP_VD_ZARLINK_9541:
         {
            return boardHalSlicDeinit9540(boardHalInitCfg);
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838) */


#if defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_SILABS_32392:
         {
            return boardHalSlicDeinit32392(boardHalInitCfg);
         }
         break;
#endif
  
         default:
         break;
      }
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicGetDriverApm
**
** PURPOSE:    Get the SLIC driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the SLIC driver interface
**
*****************************************************************************
*/
XDRV_SLIC* boardHalSlicGetDriverApm( int chan )
{
   XDRV_SLIC* pSlicDriver = NULL;
   int deviceId;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {
#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) 
         case BP_VD_ZARLINK_9530:
         {
            pSlicDriver = boardHalSlicGetDriver9530( chan );
         }
         break;

         case BP_VD_SILABS_3239:
         {
            pSlicDriver = boardHalSlicGetDriver3239( chan );
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) */

#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_ZARLINK_9540:
         case BP_VD_ZARLINK_9541:
         {
            pSlicDriver = boardHalSlicGetDriver9540( chan );
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838) */

#if defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_SILABS_32392:
         {
            pSlicDriver = boardHalSlicGetDriver32392( chan );
         }
         break;
#endif
         default:
            VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL,
               "(%s, %d): Unrecognized SLIC device %d for channel %d !!!",
               __FUNCTION__, __LINE__,
               voiceParams->voiceDevice[deviceId].voiceDeviceType,
               chan ));
         break;
      }
   }

   return pSlicDriver;
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicEnableApm
**
** PURPOSE:    Disable the SLIC interface on the line
**
** PARAMETERS: lineId      - physical interface to control
**             enableSlic  - controls whether to enable the SLIC i/f
**                           (1 = enable; 0 = disable )
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/                 
int boardHalSlicEnableApm( int lineId, VRG_BOOL enableSlic )
{
   int deviceId;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();
   
   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {

      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {
#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) 
         case BP_VD_ZARLINK_9530:
         {
            return boardHalSlicEnable9530(lineId, enableSlic);
         }
         break;

         case BP_VD_SILABS_3239:
         {
            return boardHalSlicEnable3239(lineId, enableSlic);
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) */

#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_ZARLINK_9540:
         case BP_VD_ZARLINK_9541:
         {
            return boardHalSlicEnable9540(lineId, enableSlic);
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838) */

#if defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_SILABS_32392:
         {
            return boardHalSlicEnable32392(lineId, enableSlic);
         }
         break;
#endif

         default:
         break;
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicHvgRefreshApm
**
** PURPOSE:    Refreshes the HVG based on the current operating state
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
**  NOTES:     There are requirements for line voltage behaviour during different
**             states of MTA operation (board reset, voice only reset, connectivity
**             loss) to be programmable and assynchrounously controlled from a  
**             higher application layer (such as Maintanance Task). Satisfying these 
**             requirements may necessitate to shutdown the HVG even after the 
**             boardHalInit has configured the HVG for the last time. 
**             This API allows the application to check if such late shutdown has
**             occured and revive the HVG module if needed.            
**
*****************************************************************************
*/
int boardHalSlicHvgRefreshApm( void )
{
   int deviceId;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {
#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) 
         case BP_VD_ZARLINK_9530:
         {
            return boardHalSlicHvgRefresh9530();
         }
         break;

         case BP_VD_SILABS_3239:
         {
            return boardHalSlicHvgRefresh3239();
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) */

#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_ZARLINK_9540:
         case BP_VD_ZARLINK_9541:
         {
            return boardHalSlicHvgRefresh9540();
         }
         break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838) */

#if defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
         case BP_VD_SILABS_32392:
         {
            return boardHalSlicHvgRefresh32392();
         }
         break;
#endif

         default:
         break;
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalSetAnalogLoopbackApm
**
** PURPOSE:    Set GPIO offhook pin to enable/disable analog loopback.
**
** PARAMETERS: lineId - lineID to enable/disable analog loopback
**             enabled - 0 to disable, 1 to enable
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSetAnalogLoopbackApm( int lineId, VRG_BOOL enabled )
{
   /* We will need to fetch the device index based on the channel number 
    ** in order to determine the appropriate slicDriver structure */
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap = boardHalProvGetChannelMap( lineId );
   XCHG_ASSERT( pDeviceChannelMap->deviceChannelType == BP_VCTYPE_SLIC );

   switch (  pDeviceChannelMap->deviceType )
   {
#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818)
      case BP_VD_ZARLINK_9530:
      {
         boardHalSetAnalogLoopback9530(lineId, enabled);
      }
      break;

      case BP_VD_SILABS_3239:
      {
         boardHalSetAnalogLoopback3239(lineId, enabled);
      }
      break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) */

#if defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
      case BP_VD_ZARLINK_9540:
      case BP_VD_ZARLINK_9541:
      {
         boardHalSetAnalogLoopback9540(lineId, enabled);
      }
      break;
#endif /* defined(BRCM_6816) || defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838) */

#if defined(BRCM_6828) || defined(BRCM_6818) || defined(BRCM_6838)
      case BP_VD_SILABS_32392:
      {
         boardHalSetAnalogLoopback32392(lineId, enabled);
      }
      break;
#endif

      default:
      break;
   }
}

