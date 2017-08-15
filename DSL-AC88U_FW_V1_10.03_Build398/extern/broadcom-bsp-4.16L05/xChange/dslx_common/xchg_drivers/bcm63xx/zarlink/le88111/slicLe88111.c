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
*      This is the Legerity Le88111 - Single Channel Digital SLIC Device
*      driver implementation.
*
****************************************************************************/

#if 0

/* ---- Include Files ---------------------------------------- */
#include <vp_api.h>
#include <vp_profile.h>

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <mspiChip.h>
#include <bosSleep.h>
#include "slicLe88111.h"
#include <zarlinkCommonSlic.h>

/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */

#define  PCM_MODE_ALAW    0x00
#define  PCM_MODE_ULAW    0x40
#define  PCM_MODE_LINEAR  0x80

#ifdef   PCM_ALAW
#  define  PCM_MODE PCM_MODE_ALAW
#else
#  ifdef   PCM_ULAW
#     define  PCM_MODE PCM_MODE_ULAW
#  else
#     define  PCM_MODE PCM_MODE_LINEAR
#  endif
#endif

/*
 * Application memory space for the API requires Device/Line contexts and
 * objects.  This application uses the simplest memory scheme -- statically allocated, 
 * managed by the single CAS process
 */
VpDevCtxType pDevCtx[MAX_NUM_DEVICES];

static VpLineCtxType pLineCtx[NUM_LINES];
static Vp880LineObjectType pLineObj[NUM_LINES]; 

/* ---- Private Variables ------------------------------------ */

/* Create a static array of each country's WinSLAC parameters */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##Profile,
static const VpProfileDataType *gFXSProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_WB##country##Profile,
static const VpProfileDataType *gFXSWBProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##RingProfile,
static const VpProfileDataType *gFXSRingProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

/* ---- Private Function Prototypes -------------------------- */
static SLIC_STATUS CloseSlac( void );

static void slicLe88111ModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );
static void slicLe88111LedControl( XDRV_SLIC *pDrv, int value );
static void slicLe88111PhaseReversalControl( XDRV_SLIC *pDrv, int value );
static XDRV_BOOL slicLe88111IsOffhook( XDRV_SLIC *pDrv );
static int slicLe88111SetHighVring( XDRV_SLIC *pDrv, int value );
static int slicLe88111SetRingDCoffset( XDRV_SLIC *pDrv, int value );
static int slicLe88111SetBoostedLoopCurrent( XDRV_SLIC *pDrv, int value );
static int slicLe88111SetPowerSource( XDRV_SLIC *pDrv, int value );
static int slicLe88111SetPowerRingFrequency( XDRV_SLIC *pDrv, int freq, int waveform );

static VpProfileDataType *slicLe88111GetLocale( VRG_COUNTRY country, VpOptionCodecType codec );
static VpProfileDataType *slicLe88111GetRingLocale( VRG_COUNTRY country );

static void slicLe88111Reset(XDRV_UINT8 rstGpioPin);

/* SLAC "meta-driver" interface */
static const XDRV_SLIC_FUNCS slicLe88111DrvFuncs =
{
   slicLe88111ModeControl,
   slicLe88111LedControl,
   slicLe88111PhaseReversalControl,
   slicLe88111IsOffhook,
   slicLe88111SetHighVring,
   slicLe88111SetRingDCoffset,
   slicLe88111SetBoostedLoopCurrent,
   slicLe88111SetPowerSource,
   slicLe88111SetPowerRingFrequency
};

/* Private Functions */

/*
*****************************************************************************
** FUNCTION:   slicLe88111Init
**
** PURPOSE:    Initialize and open the LE88111 SLAC
**
** PARAMETERS: 
**
** RETURNS:    none
**
*****************************************************************************
*/
SLIC_STATUS slicLe88111Init( SLIC_LE88111_DRV *pDev, int resetGpioPin )
{
   SLIC_STATUS retStatus = SLIC_SUCCESS;
   VpStatusType status;

   XDRV_UINT8 deviceNum = 0;
   XDRV_UINT8 chanNum = 0;

   VpOptionTimeslotType timeslot;
   VpOptionCodecType codecType;
   
   pDev->pDrvFuncs = &slicLe88111DrvFuncs;

   switch ( pDev->pcmMode )
   {
      case LE88111_LINEAR:
      {
         codecType = VP_OPTION_LINEAR;
         break;
      }
      case LE88111_ULAW:
      {
         codecType = VP_OPTION_MLAW;
         break;
      }
      case LE88111_ALAW:
      {
         codecType = VP_OPTION_ALAW;
         break;
      }
      case LE88116_WIDEBAND:
      {
         break;
         codecType = VP_OPTION_WIDEBAND;
      }
      default:
      {
         codecType = VP_OPTION_LINEAR;
         break;
      }
   }

   /* Open the SLIC */
   slicLe88111Reset(resetGpioPin);
   bosSleep(5);

   if ( initZarlinkSlicDevice( VP_DEV_880_SERIES, pDev->mspiDevId, deviceNum ) == XDRV_FALSE )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC ERROR: Device NOT initialized "));
      return ( SLIC_ERROR );
   }

   status = VpMakeLineObject(VP_TERM_FXS_GENERIC, chanNum, &pLineCtx[chanNum], &pLineObj[chanNum], &pDevCtx[deviceNum]);
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
   }
   
   status = VpInitLine( &pLineCtx[chanNum], 
                         slicLe88111GetLocale(pDev->country, codecType), 
                         DC_25MA_CC, slicLe88111GetRingLocale(pDev->country));
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
   }
    
   VpSetLineState( &pLineCtx[chanNum], VP_LINE_STANDBY );
   
   timeslot.tx = pDev->txTimeSlot;
   timeslot.rx = pDev->rxTimeSlot;
   /* Don't give device context (just give NULL) if you are configuring any parameters for a specific line */
   status = VpSetOption( &pLineCtx[chanNum], VP_NULL, VP_OPTION_ID_TIMESLOT, (void*)&timeslot );
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
   }   
   
   /* First time we always try wideband. If we get an invalid argument error, it means the chip is not wideband-capable so we try linear */

   status = VpSetOption( &pLineCtx[chanNum], VP_NULL, VP_OPTION_ID_CODEC, (void*)&codecType );   
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
      if ( status == VP_STATUS_INVALID_ARG )
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: NOT a WIDEBAND-capable chip. Trying Linear codec. " ));
         codecType = VP_OPTION_LINEAR;
         status = VpSetOption( &pLineCtx[chanNum], VP_NULL, VP_OPTION_ID_CODEC, (void*)&codecType );   
         if ( status != VP_STATUS_SUCCESS )
         {
            XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
         }
      }
   }

   return( retStatus );
}


/*
*****************************************************************************
** FUNCTION:   slicLe88111Deinit
**
** PURPOSE:    Shutdown the LE88111 SLAC
**
** PARAMETERS: 
**
** RETURNS:    SLAC_STATUS
**
*****************************************************************************
*/
SLIC_STATUS slicLe88111Deinit( void )
{
   /* Close the SLAC */
   //CloseSlac();

   return( SLIC_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   slicLe88111IsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: 
**
** RETURNS:    TRUE if offhook, FALSE if onhook
**
*****************************************************************************
*/
static XDRV_BOOL slicLe88111IsOffhook( XDRV_SLIC *pDrv )
{
   return ( zarlinkFxsLineIsOffhook(&pLineCtx[0]) );
}


/*
*****************************************************************************
** FUNCTION:   slicLe88111ModeControl
**
** PURPOSE:    Sets the SLAC into one of the defined modes
**
** PARAMETERS: mode  - the mode to set the SLAC into.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slicLe88111ModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode )
{
   zarlinkFxsLineModeControl(&pLineCtx[0], mode);
}


/*
*****************************************************************************
** FUNCTION:   slicLe88111LedControl
**
** PURPOSE:    This function controls the on board Hook State LED
**
** PARAMETERS: 
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicLe88111LedControl( XDRV_SLIC *pDrv, int value )
{
   /* Currently not applicable */
}


/*
*****************************************************************************
** FUNCTION:   slicLe88111GetLocale
**
** PURPOSE:    This function gets the WinSLAC profile for the
**             selected country.
**
** PARAMETERS: country - the country to select.
**
** RETURNS:    Pointer to the correct profile.
**
*****************************************************************************
*/
static VpProfileDataType *slicLe88111GetLocale( VRG_COUNTRY country, VpOptionCodecType codec )
{
   VpProfileDataType *pCountryProfile = NULL;
   
   if( country > VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "slicLe88111GetLocale: ERROR! Unsupported locale %d", 
                country));
   }
   else
   {
#ifdef PCM_WIDEBAND_DECT
      if ( codec == VP_OPTION_WIDEBAND )
      {
         pCountryProfile = (VpProfileDataType *)gFXSWBProfiles[country];
      }
      else
#endif
      {
         pCountryProfile = (VpProfileDataType *)gFXSProfiles[country];
      }
   }

   return pCountryProfile;
}

/*
*****************************************************************************
** FUNCTION:   slicLe88111GetRingLocale
**
** PURPOSE:    This function gets the Ring tone profile for the
**             selected country.
**
** PARAMETERS: country - the country to select.
**
** RETURNS:    Pointer to the correct profile data.
**
*****************************************************************************
*/
static VpProfileDataType *slicLe88111GetRingLocale( VRG_COUNTRY country )
{
   VpProfileDataType *pCountryProfile = NULL;
   
   if( country > VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "slicLe88111GetLocale: ERROR! Unsupported locale %d", 
                country));
   }
   else
   {
      pCountryProfile = (VpProfileDataType *)gFXSRingProfiles[country];
   }

   return pCountryProfile;
}

static void slicLe88111PhaseReversalControl( XDRV_SLIC *pDrv, int value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;
};

static int slicLe88111SetHighVring( XDRV_SLIC *pDrv, int value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

static int slicLe88111SetRingDCoffset( XDRV_SLIC *pDrv, int value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

static int slicLe88111SetBoostedLoopCurrent( XDRV_SLIC *pDrv, int value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

static int slicLe88111SetPowerSource( XDRV_SLIC *pDrv, int value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

static int slicLe88111SetPowerRingFrequency( XDRV_SLIC *pDrv, int freq, int waveform )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) freq;
   (void) waveform;

   return( 0 );
}

/*
*****************************************************************************
** FUNCTION:   CloseSlac
**
** PURPOSE:    Close the dual SLAC
**
** PARAMETERS: 
**
** RETURNS:    SLAC_STATUS
**
*****************************************************************************
*/
SLIC_STATUS CloseSlac( void )
{
   /*
      Currently not implemented.
   */
   return( SLIC_SUCCESS );
}

/*****************************************************************************
*
* FUNCTION:   slicLe88111Reset
*
* PURPOSE:    Reset the SLIC
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
static void slicLe88111Reset(
   XDRV_UINT8 rstGpioPin
)
{
   /* 
    * Set up the GPIO output pin that resets the DAA, reset the DAA 
    */
   kerSysSetGpio( rstGpioPin, kGpioActive );
   bosSleep(5);
   kerSysSetGpio( rstGpioPin, kGpioInactive );
   bosSleep(10);
   kerSysSetGpio( rstGpioPin, kGpioActive );
}


#endif
