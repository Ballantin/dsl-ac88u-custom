/****************************************************************************
*
*  Copyright (c) 2005-2008 Broadcom Corporation
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
*    Description: 
*      This is the Legerity LE88004 - Special Thalassa device to convert
*      10 pins into 4 pins for digital isolation.
*
****************************************************************************/


/* ---- Include Files ---------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <bosSleep.h>
#include <xchgAssert.h>
#include <xdrvTypes.h>
#include <xdrvLog.h>

#include "slicLe88004.h"
#include "zarlinkCommonSlic.h"


/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */

#define LOG_MODULE_NAME       "THALASSA"
#define LOG_DEV_NAME          "88004"


/* ---- Private Variables ------------------------------------ */


/*
*****************************************************************************
** FUNCTION:   slicLe88004Init
**
** PURPOSE:    Initialize the Thalassa device.
**
** PARAMETERS: pDev           -  SLIC(FXS) configuration structure
**             resetGpioPin   -  GPIO pin for resetting the device
**
** RETURNS:    XDRV_SLIC_STATUS
**
*****************************************************************************
*/
XDRV_SLIC_STATUS slicLe88004Init( SLIC_DEVICE_DRV *pDev, XDRV_UINT16 resetGpioPin, XDRV_UINT8 isolatorPresent )
{
   XDRV_SLIC_STATUS retStatus = XDRV_SLIC_STATUS_SUCESS;
   XDRV_UINT8       buffer[14];

   
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Starting %s device...", LOG_MODULE_NAME, LOG_DEV_NAME ));

   /* Reset the device if needed (Note: Thalassa must be reset at least once prior to VpMpiCmd) */
   if ( pDev->requiresReset )
   {
     deviceReset(resetGpioPin);
     bosSleep(5);
   }
   else
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Skipping Thalassa reset", LOG_MODULE_NAME ));
   }   

   /* 
    * Setup Thalassa device.  The settings here for CODEC and timeslot are retrieved from
    * the settings used for the sub-SLAC behind the Thalassa device.
    *
    * CAUTION: This is configuration-specific.  Setup for Thalassa with 1 sub-SLAC with 2 channels.
    *          These settings MUST be changed for 2 sub-SLAC setup.
    */
   buffer[0]  = 0x89;
   buffer[1]  = 0x02; /* PCM clock = 2.048Mhz - 32 timeslots */
   buffer[2]  = 0x8F; 
   buffer[3]  = 0x00; /* Narrowband mode */
   /* Note: 0x90, 0x91, 0x92, 0x93 available on Thalassa device for sub-SLAC1 */
   buffer[4]  = 0x90;
   buffer[5]  = pDev->slicChannel[0].txTimeSlot;       /* sub-SLAC1 Starting timeslot ch1 */
   buffer[6]  = 0x91; 
   buffer[7]  = (pDev->slicChannel[0].txTimeSlot + 1); /* sub-SLAC1 Ending timeslot ch1 */
   buffer[8]  = 0x92; 
   buffer[9]  = pDev->slicChannel[1].txTimeSlot;       /* sub-SLAC1 Starting timeslot ch2 */
   buffer[10] = 0x93;
   buffer[11] = (pDev->slicChannel[1].txTimeSlot + 1); /* sub-SLAC1 Ending timeslot ch2 */
   buffer[12] = 0x95;                                  
   buffer[13] = 0x4;                                   /* sub-SLAC 1 IDLE time setup */
   /* Note: 0xA0, 0xA1, 0xA2, 0xA3 available on Thalassa device for sub-SLAC2 */

   VpMpiCmd( (ZARLINK_THALASSA|pDev->mspiDevId), 0, 0xFF, 0xE, buffer );

   /* Unmask interrupts associated with sub-SLACS.  Not required for simple polled mode. */
#if 0
   buffer[0] = 0x8B;
   buffer[1] = 0xF3;
   VpMpiCmd( (ZARLINK_THALASSA|pDev->mspiDevId), 0, 0xFF, 0x2, buffer );
#endif

   /* If daughtercard has a real digital isolator present. */
   if( isolatorPresent == 1 )
   {
      /* Add extra delay for Thalassa due to onboard digital isolators */
      buffer[0] = 0x94;
      buffer[1] = 0x02;
      VpMpiCmd( (ZARLINK_THALASSA|pDev->mspiDevId), 0, 0xFF, 0x2, buffer );
   }
   
   XDRV_UINT8  data;
   VpMpiCmd( (ZARLINK_THALASSA|pDev->mspiDevId), 0, 0x09, 0x1, &data );
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s PLL Divisor register value 0x%02x", LOG_MODULE_NAME, LOG_DEV_NAME, data));

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s device %d initialization COMPLETED OK !", LOG_MODULE_NAME, LOG_DEV_NAME, pDev->deviceId ));

   return ( retStatus );
}

/*
*****************************************************************************
** FUNCTION:   slicLe88004Deinit
**
** PURPOSE:    Shutdown the LE88004 SLIC
**
** PARAMETERS: 
**
** RETURNS:    XDRV_SLIC_STATUS
**
*****************************************************************************
*/
XDRV_SLIC_STATUS slicLe88004Deinit( SLIC_DEVICE_DRV *pDev )
{
   /* Close the SLIC */ 

   return( XDRV_SLIC_STATUS_SUCESS );
}

#ifdef DYING_GASP_API
/*****************************************************************************
*
* FUNCTION:   slicLe88004Shutdown
*
* PURPOSE:    Reset the SLIC and keep SLIC in shutdown mode
*
* PARAMETERS: none
*
* RETURNS:    0 - success 
*
*****************************************************************************/
XDRV_SLIC_STATUS slicLe88004Shutdown( XDRV_UINT16 rstGpioPin )
{

   kerSysSetGpio( rstGpioPin, kGpioActive );

   return ( XDRV_SLIC_STATUS_SUCESS );
}
#endif





