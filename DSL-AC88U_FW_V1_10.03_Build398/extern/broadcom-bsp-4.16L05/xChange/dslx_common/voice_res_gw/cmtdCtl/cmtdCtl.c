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
*    Filename: cmtdCtl.c
*
****************************************************************************
*    Description:
*
*     CMTD related control
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "hdspCmd.h"
#include <hapi_cmtd.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include "cmtdCtl.h"
#include "cmtdCtlCountryProfile.h"

#include <vrgCountry.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* Stores the previously configured tone based on filter index */
static CMTD_TONE_TYPE configuredTone[CMTD_FILTER_MAX] =
{
   CMTD_TONE_TYPE_INVALID,   /* Defaults to invalid tone type */
   CMTD_TONE_TYPE_INVALID    /* Defaults to invalid tone type */
};

/* Mapping from CMTD_TONE_TYPE to character array names */
const char* cmtdToneMap[] =
{
   "Dial Tone",         /* CMTD_TONE_TYPE_DIAL */
   "Ring Tone",         /* CMTD_TONE_TYPE_RING */
   "Busy Tone",         /* CMTD_TONE_TYPE_BUSY */
   "Congestion Tone",   /* CMTD_TONE_TYPE_CONG */
   "Release Tone",      /* CMTD_TONE_TYPE_RELEASE */
   "Maximum"            /* CMTD_TONE_TYPE_MAX */
};

/* ---- Private Function Prototypes -------------------------------------- */
HAPIRESULT setFilterCoeff( VRG_UINT16 hdl, VRG_UINT16 filtSize, VRG_SINT16 *filtCoeffp, CMTD_FILTER_INDEX filterIndex );
HAPIRESULT setStmParam( VRG_UINT16 hdl, HAPI_CMTD_STM *stmParamp, CMTD_FILTER_INDEX filterIndex );
HAPIRESULT enblFiltStm( VRG_UINT16 hdl, VRG_UINT16 numFilt, VRG_UINT16 numStm );

/* ---- Functions -------------------------------------------------------- */

HAPIRESULT setFilterCoeff( VRG_UINT16 hdl, VRG_UINT16 filtSize, VRG_SINT16 *filtCoeffp, CMTD_FILTER_INDEX filterIndex )
{
   VRG_UINT16 filtCoefNum;
   VRG_UINT16 filtOffset;
   HAPI_CMTD_FILTER cmtdFilter;
    
   memset( &cmtdFilter, 0, sizeof( HAPI_CMTD_FILTER ) );
   cmtdFilter.index = (HAPI_UINT16) filterIndex;
   cmtdFilter.coefs.type = filtSize;
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "CMTD_CTL: setFilterCoeff: filtSize%d", filtSize ));
   cmtdFilter.coefs.minAttenuation = 65536/4;

   /* Check that the destination filter size is large enough */
   XCHG_ASSERT( filtSize <= HAPI_CMTD_FILTER_NUM_TAPS );

   filtOffset = ( HAPI_CMTD_FILTER_NUM_TAPS - filtSize ) / 2; /* need to "center" filter coefficients in array */
   for (filtCoefNum = 0; filtCoefNum < filtSize; filtCoefNum++)
   {
      cmtdFilter.coefs.filterTaps[ filtCoefNum + filtOffset ] = filtCoeffp[ filtCoefNum ];
   }
   return hdspSendCmdData( hdl, HAPI_CMTD_SETFILTER_CMD, HSCMDEXTDATA, sizeof(cmtdFilter), &cmtdFilter );
}

/*
*****************************************************************************
** FUNCTION:   setStmParam
**
** PURPOSE:    To set CMTD state machine parameters.
**
** PARAMETERS: active filter (in), minTimeMs (in).
**
** RETURNS:
**
** NOTE:
*****************************************************************************
*/
HAPIRESULT setStmParam( VRG_UINT16 hdl, HAPI_CMTD_STM *stmParamp, CMTD_FILTER_INDEX filterIndex )
{
   HAPIRESULT result;
   HAPI_CMTD_STM localStmParam;
   VRG_UINT16 i;

   /* Create a local copy of HAPI_CMTD_STM */
   memcpy( &localStmParam, stmParamp, sizeof( HAPI_CMTD_STM  ) );

   localStmParam.index = (HAPI_UINT16) filterIndex;

   /* Needs to re-adjust the configured active filter index */
   for ( i = 0; i < HAPI_CMTD_NUM_SUBSTATES; i++ )
   {
      /* Filter index must be changed if its not the silence filter */
      if ( localStmParam.stmParams.substate[i].activeFilter != CMTD_SILENCE_FILTER )
      {
         /* Increment the activeFilter value by filterIndex (either 0 or 1) */
         localStmParam.stmParams.substate[i].activeFilter += (HAPI_UINT16) filterIndex;
      }
   }

   result = hdspSendCmdData( hdl, HAPI_CMTD_SETSTM_CMD, HSCMDEXTDATA, sizeof( HAPI_CMTD_STM ), ( VRG_UINT16 * )&localStmParam );
   if ( result != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "CMTD_CTL: setStmParam FAILED!!!" ));
      return result;
   }

   return HAPISUCCESS;
}

/*
*****************************************************************************
** FUNCTION:   enblFiltStm
**
** PURPOSE:    To enable CMTD STM/filter pair.
**
** PARAMETERS: VHD handle (in), STM/filter index (in).
**
** RETURNS:
**
** NOTE:
*****************************************************************************
*/
HAPIRESULT enblFiltStm( VRG_UINT16 hdl, VRG_UINT16 numFilt, VRG_UINT16 numStm )
{
   return hdspSendCmd( hdl,
       HAPI_CMTD_ENABLE_CMD,
       HSCMDDATA,
       (VRG_UINT16)HAPI_CMTD_ENABLE_OP1_ENA_FILTERS_BIT |
       (VRG_UINT16)HAPI_CMTD_ENABLE_OP1_ENA_STMS_BIT | 
       (VRG_UINT16)HAPI_CMTD_ENABLE_OP1_ENA_SRV_BIT,
       numFilt | ( numStm << HAPI_CMTD_ENABLE_OP2_STM_BITSHIFT ) );
}

/*
*****************************************************************************
** FUNCTION:   cmtdCtlSetFilterCoeff
**
** PURPOSE:    To set CMTD filter coefficients.
**
** PARAMETERS: locale (in), region (in).
**
** RETURNS:
**
** NOTE:
*****************************************************************************
*/
HAPIRESULT cmtdCtlConfigFilter( VRG_UINT16 hdl, VRG_COUNTRY locale, CMTD_TONE_TYPE toneType, CMTD_FILTER_INDEX filterIndex )
{
   CMTD_STMPARAMS *filtParamp = NULL;
   HAPIRESULT result;

   /* Check to see if locale value is out of bounds */
   XCHG_ASSERT( locale < VRG_COUNTRY_MAX );

   /* Check to see if toneType value is out of bounds */
   XCHG_ASSERT( toneType < CMTD_TONE_TYPE_MAX );

   /* Check to see if the filterIndex is out of bounds */
   XCHG_ASSERT( filterIndex < CMTD_FILTER_MAX );

   /* Check if the tone profile for the country locale exists */
   if ( cmtdCallingToneProfiles[locale] != NULL )
   {
      /* Allocate the proper filter parameter configuration */
      filtParamp = cmtdCallingToneProfiles[locale][toneType];
   }
   else
   {      
      VRG_LOG_INFO((VRG_LOG_MOD_EPT,  "CMTD_CTL: Configured country profile does not exist" ));
   }

   /* 
   ** At this point, filtParamp may still be NULL. Check if it is, then assign
   ** a default value for it.
   */   
   if ( filtParamp == NULL )
   {
      VRG_LOG_INFO((VRG_LOG_MOD_EPT,  "CMTD_CTL: Using default settings" ));
      switch ( toneType )
      {
         case CMTD_TONE_TYPE_DIAL:
         {
            filtParamp = ( CMTD_STMPARAMS * )&northAmericaDialToneParam;
         }
         break;
         case CMTD_TONE_TYPE_RING:
         {
            filtParamp = ( CMTD_STMPARAMS * )&northAmericaRingbackParam;
         }
         break;
         case CMTD_TONE_TYPE_BUSY:
         {
            filtParamp = ( CMTD_STMPARAMS * )&northAmericaBusyToneParam;
         }
         break;
         case CMTD_TONE_TYPE_CONG:
         {
            filtParamp = ( CMTD_STMPARAMS * )&northAmericaCongToneParam;
         }
         break;
         case CMTD_TONE_TYPE_RELEASE:
         {
            /* 
            ** Release Tone does not exist for that country. To prevent
            ** accidental detection we should not configure a filter at
            ** all. Simply return.
            */

            return HAPISUCCESS;
         }
         break;
         default:
         {
            /* default to North America dial tone */
            filtParamp = ( CMTD_STMPARAMS * )&northAmericaDialToneParam;
         }
         break;
      }
   }

   result = setFilterCoeff( hdl, filtParamp->filtSize, filtParamp->filtCoeffp, filterIndex );
   if ( result != HAPISUCCESS )
   {
       return ( result );
   }

   result = setStmParam( hdl, &filtParamp->stmParam, filterIndex );
   if ( result != HAPISUCCESS )
   {
       return ( result );
   }

   /* Enable the CMTD service, filter(s) and cadence state machine(s) */
   result = enblFiltStm( hdl, filterIndex, filterIndex );      /* Assume filter to state machine mapping is 1-to-1 */

   /* Check if the filter has been successfully configured */
   if ( result == HAPISUCCESS )
   {
      /* Keep track of the currently configured tone */
      configuredTone[filterIndex] = toneType;
   }
    
   return ( result );
}

/*
*****************************************************************************
** FUNCTION:   cmtdCtlDisable
**
** PURPOSE:    Disables CMTD service.
**
** PARAMETERS: hdl (in) - VHD handle
**
** RETURNS:
**
** NOTE:
*****************************************************************************
*/
HAPIRESULT cmtdCtlDisable( VRG_UINT16 hdl )
{
   VRG_UINT16 i;

   /* Reset the currently configured tone information */
   for ( i = 0; i < CMTD_FILTER_MAX; i++ )
   {
      configuredTone[i] = CMTD_TONE_TYPE_MAX; 
   }

   /* Disable CMTD service */
   return hdspSendCmd( hdl,
       HAPI_CMTD_ENABLE_CMD,
       HSCMDDATA,
       (VRG_UINT16)HAPI_CMTD_ENABLE_OP1_DIS_SRV_BIT,
       0 );
}

/*
*****************************************************************************
** FUNCTION:   cmtdCtlGetConfigTone
**
** PURPOSE:    Gets the currently configured tone type based on filter index
**
** PARAMETERS: filterIndex (in)
**
** RETURNS:    Currently configured tone type
**
** NOTE:
*****************************************************************************
*/
CMTD_TONE_TYPE cmtdCtlGetConfigTone( CMTD_FILTER_INDEX filterIndex )
{
   /* Check if filter index is out of bounds */
   if ( filterIndex >= CMTD_FILTER_MAX )
   {
      /* Return an out of bound tone type */
      return CMTD_TONE_TYPE_MAX;
   }

   /* Return the currently configured tone */
   return ( configuredTone[ filterIndex ] );
}

/*
*****************************************************************************
** FUNCTION:   cmtdCtlMapToneType
**
** PURPOSE:    Maps from CMTD_TONE_TYPE to character array representation
**
** PARAMETERS: filterIndex (in)
**
** RETURNS:    Name of the currently configured tone
**
** NOTE:
*****************************************************************************
*/
const char* cmtdCtlMapToneType( CMTD_TONE_TYPE tone )
{
   static const char* invalidToneName = "Invalid Tone";

   /* Check if the tone index is out of bounds */
   if ( tone > CMTD_TONE_TYPE_MAX )
   {
      /* Return an out of bound tone type */
      return ( cmtdToneMap[CMTD_TONE_TYPE_MAX] );
   }
   else if ( tone < 0 )
   {
      return ( invalidToneName );
   }

   /* Return the mapped char array */
   return ( cmtdToneMap[tone] );
}



