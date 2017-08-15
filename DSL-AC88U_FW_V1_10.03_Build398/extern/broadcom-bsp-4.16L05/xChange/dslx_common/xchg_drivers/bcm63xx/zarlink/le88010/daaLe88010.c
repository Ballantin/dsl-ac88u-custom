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
*    Filename: daaLe88010.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the control of the
*      Le88010 Data Access Arrangement circuit (DAA).
*
****************************************************************************/


/* ---- Include Files ---------------------------------------- */
#include <xchgAssert.h>
#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <string.h>
#include <bosSleep.h>
#include <bcmChip.h>

#include "daaLe88010.h"
#include "daaLe88010Defs.h"

#include <vp_api.h>
#include <vp_profile.h>

#include <zarlinkCommonDaa.h>
#include "board.h"

/*
	Uncomment the following line for debug information.
*/
               

/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */

#define MAX_NUM_DEVICES    2

/*
 * Application memory space for the API required Device/Line contexts and
 * objects.  This application uses the simplest memory scheme -- global, but the
 * API will work with any memory management scheme.
 */

static VpDevCtxType pDaaDevCtx[MAX_NUM_DEVICES];
static VpLineCtxType pLineCtx[LE88010_NUM_CHANNELS];
static Vp880DeviceObjectType  pDevObj[MAX_NUM_DEVICES];
static Vp880LineObjectType pLineObj[LE88010_NUM_CHANNELS]; 

/* ---- Private Variables ------------------------------------ */
/* Create a static array of each country's WinSLAC parameters */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXO_##country##Profile,
static const VpProfileDataType *gFXOProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};
#undef COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXO_DIALING_##country##_Profile,
static const VpProfileDataType *gFXODialProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};
/* ---- Private Function Prototypes -------------------------- */

/* DAA "meta-driver" interface functions */
static void daaLe88010ModeControl( XDRV_DAA *pDrv, XDRV_DAA_MODE mode );
static void daaLe88010LedControl( XDRV_DAA *pDrv, int value );
static XDRV_BOOL daaLe88010IsOffhook( XDRV_DAA *pDrv );
static XDRV_BOOL daaLe88010IsRingActive( XDRV_DAA *pDrv );
static XDRV_DAA_LOOP daaLe88010GetLoopCurrent( XDRV_DAA *pDrv );

/* DAA "meta-driver" interface */
static const XDRV_DAA_FUNCS daaLe88010DrvFuncs =
{
   daaLe88010ModeControl,
   daaLe88010LedControl,
   daaLe88010IsOffhook,
   daaLe88010IsRingActive,
   daaLe88010GetLoopCurrent
};

/* Private Functions */
static void CloseDaa( DAA_Le88010_DRV *pDev );

static VpProfileDataType *daaLe88010GetLocale( VRG_COUNTRY country, VpProfileDataType** pFXOProfiles);

/*
*****************************************************************************
** FUNCTION:   daaLe88010Init
**
** PURPOSE:    Initialize and open the Le88010 DAA
**
** PARAMETERS: locale      - locale/country enum
**             chan        - the line number ( 0 referenced )
**             pDev        - pointer to the Le88010 DAA info structure
**             rstGpioPin  - GPIO pin used to reset the DAA
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaLe88010Init
(
   VRG_COUNTRY             locale,
   int                     chan,
   DAA_Le88010_DRV*        pDev,
   XDRV_UINT16             rstGpioPin
)
{
   int daaId = chan;
   int i;
   bool vpApiEventPending = FALSE;
   XDRV_BOOL daaDeviceInitialized = XDRV_FALSE;

   (void)daaDeviceInitialized;

   pDev->daaId                     = daaId;

   /* Initialize the DAA information for each channel */
   pDev->daaInfo.pDrvFuncs = &daaLe88010DrvFuncs;

   bosSleep(5);
   
   VpStatusType status;
 
   uint32 chanNum = 0; /* physical channel ID on Legerity device: 0 for single channel FXO, 1 for FXS+FXO combination (8831x) */
   
   VpOptionTimeslotType timeslot;
   VpOptionCodecType codecType;

   switch ( pDev->pcmMode )
   {
      case LE88010_PCM_MODE_LINEAR:
         codecType = VP_OPTION_LINEAR;
         break;

      case LE88010_PCM_MODE_ULAW:
         codecType = VP_OPTION_MLAW;
         break;

      case LE88010_PCM_MODE_ALAW:
         codecType = VP_OPTION_ALAW;
         break;

      default:
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "PCM mode not specified !!! "));
         XCHG_ASSERT(0);
   }

   daaLe88010Reset(rstGpioPin);

   bosSleep(5);
   
   VpMakeDeviceObject(VP_DEV_880_SERIES, pDev->mspiId, &pDaaDevCtx[0], &pDevObj[0]);
   status = VpInitDevice(&pDaaDevCtx[0], DEV_VE880_PROFILE, 
              VP_PTABLE_NULL, DC_25MA_CC, RING_20HZ_SINE, VP_PTABLE_NULL, VP_PTABLE_NULL); 
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: VpInitDevice status = %d ", __FUNCTION__, status));

   #if 0
   if ( initZarlinkDaaDevice( VP_DEV_880_SERIES, pDev->mspiId, 0 ) == XDRV_FALSE )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR: Device NOT initialized "));
      return;
   }   
   #else
   /* Initial calling of vpApiTick until the device is initialized successfully.
    * Afterwards vpApiTick will be called by CAS task
    */
   
   for (i = 0; i < DAA_INIT_TIMEOUT; i++)
   {
      status = VpApiTick( &pDaaDevCtx[0], &vpApiEventPending );
      if ( status == VP_STATUS_SUCCESS )
      {
         break;
      }
            
      bosSleep(10);
   }
   
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,"DAA: Device didn't initialize properly !"));
   }
   else
   {
      if ( vpApiEventPending == TRUE )
      {
         VpEventType pEvent;
         while(VpGetEvent(&pDaaDevCtx[0], &pEvent)) 
         {
            /*
                                 * If the event was a Response:Device Init Complete event,
                                 * we need to initialize the device and associated lines, so
                                 * send the event structure (pointer) to the Init function for
                                 * handling.
                                 */
            if (pEvent.eventCategory == VP_EVCAT_RESPONSE) 
            {
               if(pEvent.eventId == VP_DEV_EVID_DEV_INIT_CMP)
               {
                  XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"DAA: Received DEV_INIT_CMP event (i = %d)", i));
                  daaDeviceInitialized = XDRV_TRUE;
               } 
               else 
               {
                  /* Do nothing */
                  XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA: ERROR expected DEV_INIT_CMP event. Event %d came from the DAA.", pEvent.eventId));
               }
            }
         }
      }
   }
   
   #endif

   status = VpMakeLineObject(VP_TERM_FXO_GENERIC, 0 , &pLineCtx[0], &pLineObj[0], &pDaaDevCtx[0]);
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
   }

   status = VpInitLine( &pLineCtx[chanNum], 
                        daaLe88010GetLocale(locale, (VpProfileDataType**)gFXOProfiles), 
                        daaLe88010GetLocale(locale, (VpProfileDataType**)gFXODialProfiles), 
                        VP_PTABLE_NULL );
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,"DAA ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
   }
    
   VpSetLineState( &pLineCtx[chanNum], VP_LINE_FXO_LOOP_OPEN );
               
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "pDev->txTimeSlot = %d ", pDev->txTimeSlot));
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "pDev->rxTimeSlot = %d ", pDev->rxTimeSlot));
   
   timeslot.tx = pDev->txTimeSlot;
   timeslot.rx = pDev->rxTimeSlot;

   /* Don't give device context (just give NULL) if you are configuring any parameters for a specific line */
   status = VpSetOption( &pLineCtx[chanNum], VP_NULL, VP_OPTION_ID_TIMESLOT, (void*)&timeslot );
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
   }

   /* First time we always try wideband. If we get an invalid argument error, it means the chip is not wideband-capable so we try linear */
   status = VpSetOption( &pLineCtx[chanNum], VP_NULL, VP_OPTION_ID_CODEC, (void*)&codecType );   
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "88010 DAA Device Init completed. country is %d", locale)); 
   
}

/*****************************************************************************
*
* FUNCTION:   daaLe88010Reset
*
* PURPOSE:    Reset the DAAs
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
void daaLe88010Reset
(
   XDRV_UINT16 rstGpioPin
)
{
   kerSysSetGpio( rstGpioPin, kGpioInactive );
   bosSleep( 10 ); 
   kerSysSetGpio( rstGpioPin, kGpioActive );
   bosSleep( 10 );
   kerSysSetGpio( rstGpioPin, kGpioInactive );
}

/*
*****************************************************************************
** FUNCTION:   daaLe88010Deinit
**
** PURPOSE:    Shutdown the Le88010 DAA
**
** PARAMETERS: pDev  - pointer to the Le88010 DAA info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaLe88010Deinit( DAA_Le88010_DRV *pDev )
{
   /* Close the DAA */
   CloseDaa( pDev );
}


/*
*****************************************************************************
** FUNCTION:   daaLe88010IsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
static XDRV_BOOL daaLe88010IsOffhook( XDRV_DAA *pDrv )
{
   return ( zarlinkFxoLineIsOffhook(&pLineCtx[0]) );
}


/*
*****************************************************************************
** FUNCTION:   daaLe88010IsRingActive
**
** PURPOSE:    Determine if a channel's ring signal is active
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if ring active, XDRV_FALSE otherwise
**
**
*****************************************************************************
*/
static XDRV_BOOL daaLe88010IsRingActive( XDRV_DAA *pDrv )
{
   bool lineStatus = FALSE;
      
   if ( VpGetLineStatus( &pLineCtx[0], VP_INPUT_RINGING, &lineStatus ) == VP_STATUS_SUCCESS )
   {
      if ( lineStatus == TRUE )
      {
         return (XDRV_TRUE);
      }
      else
      {
         return (XDRV_FALSE);
      }
   }
   else
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR %s %d", __FUNCTION__, __LINE__));
   }
   
   return (XDRV_FALSE);
}

/*
*****************************************************************************
** FUNCTION:   daaLe88010GetLoopCurrent
**
** PURPOSE:    Determine the state of a channel's loop current
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_DAA_LOOP_OPEN if there is no loop current detected
**             XDRV_DAA_LOOP_CLOSED if loop current is detected
**
** NOTE:       No polarity reversal detection is implemented
**
*****************************************************************************
*/
static XDRV_DAA_LOOP daaLe88010GetLoopCurrent( XDRV_DAA *pDrv )
{
   VpLineStateType lineState;
   if(VpGetLineState(&pLineCtx[0], &lineState) != VP_STATUS_SUCCESS) 
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR %s %d", __FUNCTION__, __LINE__));
      return ( XDRV_DAA_LOOP_OPEN );
   }

   if(lineState == VP_LINE_FXO_LOOP_CLOSE || lineState == VP_LINE_FXO_TALK)
   {
      return ( XDRV_DAA_LOOP_CLOSED );
   } 
   else
   {
      return ( XDRV_DAA_LOOP_OPEN );
   }
}



/*
*****************************************************************************
** FUNCTION:   daaLe88010ModeControl
**
** PURPOSE:    Sets the DAA into one of the defined modes
**
** PARAMETERS: pDrv  - pointer to the DAA driver device structure
**             mode  - the mode to set the DAA into.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void daaLe88010ModeControl( XDRV_DAA *pDrv, XDRV_DAA_MODE mode )
{
   zarlinkFxoLineModeControl(&pLineCtx[0], mode);
}

/*
*****************************************************************************
** FUNCTION:   daaLe88010LedControl
**
** PURPOSE:    This function controls the on board Hook State LED
**
** PARAMETERS: pDrv  - pointer to the DAA driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    none
**
*****************************************************************************
*/
static void daaLe88010LedControl( XDRV_DAA *pDrv, int value )
{
    (void) pDrv;
    (void) value;
    /* Currently not applicable */
}


/*
*****************************************************************************
** FUNCTION:   CloseDaa
**
** PURPOSE:    Close the dual DAA
**
** PARAMETERS: pDev  - pointer to the Le88010 DAA info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void CloseDaa( DAA_Le88010_DRV *pDev )
{
   (void) pDev;
}


/*
*****************************************************************************
** FUNCTION:   daaLe88010GetLocale
**
** PURPOSE:    This function gets the WinSLAC profile for the
**             selected country.
**
** PARAMETERS: country - the country to select.
**
** RETURNS:    Pointer to the correct profile data.
**
*****************************************************************************
*/
static VpProfileDataType *daaLe88010GetLocale( VRG_COUNTRY country, VpProfileDataType** pFXOProfiles)
{
   VpProfileDataType *pCountryProfile = NULL;
   
   if( country > VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "daa6258Le88010GetLocale: ERROR! Unsupported locale %d", 
                country));
   }
   else
   {
      pCountryProfile = (VpProfileDataType *)pFXOProfiles[country];
   }

   return pCountryProfile;
}

#ifdef DYING_GASP_API
/*****************************************************************************
*
* FUNCTION:   daaLe88010Shutdown
*
* PURPOSE:    Shutdown the DAAs
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
void daaLe88010Shutdown ( XDRV_UINT16 rstGpioPin )
{
   kerSysSetGpio( rstGpioPin, kGpioActive );
}
#endif
