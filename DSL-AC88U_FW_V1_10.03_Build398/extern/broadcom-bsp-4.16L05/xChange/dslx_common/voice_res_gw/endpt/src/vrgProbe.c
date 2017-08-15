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
*    Filename: vrgProbe.c
*
****************************************************************************
*    Description:
*
*     This file implements the endpoint provisioning layer
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <string.h>

#include <vrgTypes.h>
#include <vrgLog.h>
#include <vrgEndpt.h>
#include <vrgProbe.h>
#include <vrgEndptCfg.h>
#include <xchgAssert.h>
#include <boardHalSlic.h>
#include <boardHalDaa.h>
#include <xdrvSlic.h>                  /* SLIC driver interface */
#include <xdrvDaa.h>                   /* DAA driver interface */

/* ---- Public Variables ------------------------------------------------- */




/*
*****************************************************************************
** FUNCTION:   endptProbeSetValue
**
** PURPOSE:    Set value into the endpoint provisioning subsystem
**
** PARAMETERS: line        -  [IN]  Line id (0-based)
**             itemId      -  [IN]  Id of the provisioning item (based on the EPPROV enum)
**             itemValue   -  [IN]  Pointer to the value that is passed in
**             itemLength  -  [IN]  Pointer to the length of the value that is passed in
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProbeSetValue( int deviceId, int chan, int reg, int regSize, void *probeValue, int probeValueSize, int indirectValue )
{
   XDRV_SLIC         *slicDriver;
   XDRV_DAA          *daaDriver;
   XDRV_UINT8        setValue;
   EPSTATUS          status = EPSTATUS_SUCCESS;
   
   if ( (probeValue == NULL) || (probeValueSize == 0) )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROBE, "NULL pointer passed to %s", __FUNCTION__ ));
      return ( EPSTATUS_ERROR );
   }

   if ( chan < 0 || chan >= VRG_ENDPT_CFG_NUM_ENDPT )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROV, "Trying to set a value for invalid channel %d", chan ));
      return ( EPSTATUS_ERROR );
   }

   /* Indicate SET operation */
   setValue = 1;
   
   if( boardHalIsSlic( chan ) )
   {
      slicDriver = boardHalSlicGetDriver( chan );   
      status = xdrvSlicProbe( slicDriver, deviceId, chan, reg, regSize, probeValue, probeValueSize, indirectValue, setValue );
   }
   else if( boardHalIsDaa( chan ) )
   {
      daaDriver = boardHalDaaGetDriver( chan );   
      status = xdrvDaaProbe( daaDriver, deviceId, chan, reg, regSize, probeValue, probeValueSize, indirectValue, setValue );
   }
   else
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROBE, "Device Probe (set) unsupported for device on channel %d.", chan ));
      status = EPSTATUS_ERROR;
   }

      
   return( status );

}


/*
*****************************************************************************
** FUNCTION:   endptProbeGetValue
**
** PURPOSE:    Get value from the endpoint provisioning subsystem
**
** PARAMETERS: line        -  [IN]  Line id (0-based)
**             itemId      -  [IN]  Id of the provisioning item (based on the EPPROV enum)
**             itemValue   -  [OUT  Pointer to the value that needs to be set
**                                  (note it is assumed that adequate memory was
**                                  allocated by the caller)
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProbeGetValue( int deviceId, int chan, int reg, int regSize, void *probeValue, int probeValueSize, int indirectValue )
{
   XDRV_SLIC         *slicDriver;
   XDRV_DAA          *daaDriver;
   XDRV_UINT8        setValue;
   EPSTATUS          status = EPSTATUS_SUCCESS;
   
   if ( probeValue == NULL )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROBE, "NULL pointer passed to %s", __FUNCTION__ ));
      return ( EPSTATUS_PROV_FAILURE );
   }

   if ( chan < 0 || chan >= VRG_ENDPT_CFG_NUM_ENDPT )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROBE, "Trying to get a value for invalid channel %d", chan ));
      return ( EPSTATUS_PROV_FAILURE );
   }
   
   /* Indicate GET operation */
   setValue = 0;
   
   if( boardHalIsSlic( chan ) )
   {
      slicDriver = boardHalSlicGetDriver( chan );   
      status = xdrvSlicProbe( slicDriver, deviceId, chan, reg, regSize, probeValue, probeValueSize, indirectValue, setValue );
   }
   else if( boardHalIsDaa( chan ) )
   {
      daaDriver = boardHalDaaGetDriver( chan );   
      status = xdrvDaaProbe( daaDriver, deviceId, chan, reg, regSize, probeValue, probeValueSize, indirectValue, setValue );
   }
   else
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROBE, "Device Probe (get) unsupported for device on channel %d.", chan ));
      status = EPSTATUS_ERROR;
   }
      
   return( status );
}


