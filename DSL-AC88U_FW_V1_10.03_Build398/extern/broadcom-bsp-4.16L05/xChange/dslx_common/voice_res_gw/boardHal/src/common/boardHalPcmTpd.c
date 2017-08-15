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
#include <boardHalProv.h>
/*#include <tpdCtl.h>*/
#include <boardHalDaa.h>

/* ---- Hardware Specific Include Files ---------------------------------- */
#if SLACVENDOR_CFG_ZARLINK
#include <zarlinkCommonTpd.h>
#endif
#if SLACVENDOR_CFG_SILABS
#include <silabsCommonTpd.h>
#endif 
/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
static XDRV_TPD gPcmTpdDriver;

/* ---- Private Function Prototypes -------------------------------------- */
static int tpdSlicIds[]={ 
                         BP_VD_ZARLINK_88601,
                         BP_VD_ZARLINK_88701,
                         BP_VD_ZARLINK_88702_ZSI,
                         BP_VD_ZARLINK_88801,
                         BP_VD_ZARLINK_9662,
                         BP_VD_ZARLINK_9672_ZSI,
                         BP_VD_ZARLINK_9642_ZSI,
                         BP_VD_SILABS_32176,
                         BP_VD_SILABS_32178,
                         BP_VD_SILABS_32261,
                         BP_VD_SILABS_32267
                       };
/* ---- Functions -------------------------------------------------------- */
/*
*****************************************************************************
** FUNCTION:   boardHalPcmTpdInit
**
** PURPOSE:    This function initializes the Tpd service
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalPcmTpdInit( void )
{
   int                               endpt;
   XDRV_TPD                         *tpdDrv;
   XDRV_SLIC                        *slicDrv;
  

   for ( endpt = 0; endpt < boardHalGetNumEndpoints(); endpt++ )
   {
      if( !boardHalTpdIsSupportedLine( endpt ) || boardHalIsDaa(endpt) )
      {
        continue;
      }

      tpdDrv = &gPcmTpdDriver;
      slicDrv = boardHalSlicGetDriver( endpt );
      if( !slicDrv )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "PCM TPD: Invalid slic driver(0x%p)!",slicDrv));
         return ( -1 );
      }

#if SLACVENDOR_CFG_ZARLINK
      if( boardHalTpdIsZarlinkLine( endpt ) )
      {
         if ( zarlinkTpdInitChannel( endpt, tpdDrv, slicDrv, NULL, NULL ) )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "PCM TPD init FAILED" ));
            return ( -2 );
         }
         VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "PCM TPD: Zarlink Device initialized!"));
      }
      else
#endif
      {
#if SLACVENDOR_CFG_SILABS
	     if ( silabsTpdInitChannel( endpt, tpdDrv, slicDrv, NULL, NULL ) )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "PCM TPD init FAILED" ));
            return ( -2 );
         }
         VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "PCM TPD: Silabs Device initialized!"));
#endif
      }
   } 
   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalPcmTpdDeinit
**
** PURPOSE:    This function de-initializes Tpd service
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalPcmTpdDeinit( void )
{
   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalPcmTpdGetDriver
**
** PURPOSE:    Get the PCM TPD driver
**
** PARAMETERS: none
**
** RETURNS:    pointer to the PCM TPD driver interface
**
*****************************************************************************
*/
XDRV_TPD* boardHalPcmTpdGetDriver( void )
{
   return ( (XDRV_TPD*)&gPcmTpdDriver );
}

/*
*****************************************************************************
** FUNCTION:   boardHalPcmTpdMapLineId
**
** PURPOSE:    Get the TPD Mapped Line ID
**
** PARAMETERS: endpoint id
**
** RETURNS:    The mapped line ID of SLAC/SLIC
**
*****************************************************************************
*/
int boardHalPcmTpdMapLineId( int lineId )
{
   int endptCnt;
   int mappedId = 0;
   
   if( lineId < 0 || lineId >= boardHalGetNumEndpoints() )
   {
       VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "PCM TPD: Invalid Line ID !" ));
       XCHG_ASSERT( 0 );
   }
   
   for( endptCnt = 0; endptCnt < lineId; endptCnt++)
   {
       if( boardHalIsSlic ( endptCnt )) 
       {
          mappedId++;
       }
   }
   
   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "PCM TPD mapped ID = %d ", mappedId ));
   return ( mappedId );
}

/*
*****************************************************************************
** FUNCTION:   boardHalTpdIsSupportedLine
**
** PURPOSE:    Get the TPD is supported on the line or not
**
** PARAMETERS: endpoint id
**
** RETURNS:    XDRV_TRUE when line supported TPD, otherwise XDRV_FALSE
**
*****************************************************************************
*/
XDRV_BOOL boardHalTpdIsSupportedLine( int lineId )
{
   int i;
   BOARDHALPROV_DEVICE_CHANNEL_MAP  *pDeviceChannelMap;

   pDeviceChannelMap = boardHalProvGetChannelMap( lineId );
   for( i=0; i<sizeof(tpdSlicIds)/sizeof(tpdSlicIds[0]); i++ )
   {
     if ( pDeviceChannelMap->deviceType == tpdSlicIds[i] )
     {
      return XDRV_TRUE;
     }
   }
   return XDRV_FALSE;
}

/*
*****************************************************************************
** FUNCTION:   boardHalTpdIsZarlinkLine
**
** PURPOSE:    Get the Line is Zarlink or not
**
** PARAMETERS: endpoint id
**
** RETURNS:    XDRV_TRUE when line is Zarlink Line, otherwise XDRV_FALSE
**
*****************************************************************************
*/
XDRV_BOOL boardHalTpdIsZarlinkLine( int lineId )
{
   BOARDHALPROV_DEVICE_CHANNEL_MAP  *pDeviceChannelMap;

   pDeviceChannelMap = boardHalProvGetChannelMap( lineId );
   if ( pDeviceChannelMap->deviceType > BP_VD_SILABS_MAXID )
   {
      return XDRV_TRUE;
   }
   return XDRV_FALSE;
}
