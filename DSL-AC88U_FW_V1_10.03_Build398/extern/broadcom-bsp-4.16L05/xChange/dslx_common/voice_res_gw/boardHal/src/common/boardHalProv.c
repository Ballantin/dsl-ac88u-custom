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
*    Filename: boardHalProv.c
*
****************************************************************************
*    Description:
*
*      This file contains main entry points for board HAL provisioning
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgLog.h>
#include <vrgTypes.h>
#include <boardparms_voice.h>
#include <boardHalProv.h>
#include <xchgAssert.h>
#include <bcm_map.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define  BOARDHALPROV_DBG        0

/* ---- Private Variables ------------------------------------------------ */

static BOARDHALPROV_DEVICE_CHANNEL_MAP deviceChannelMap[BP_MAX_VOICE_CHAN];
static VOICE_BOARD_PARMS               voiceParams;
static VRG_BOOL                        voiceParamsObtained = VRG_FALSE;


/* ---- Private Function Prototypes -------------------------------------- */

static void bhpDumpVoiceParams( VOICE_BOARD_PARMS* voiceParams );
static void bhpCreateInternalProvTable( VOICE_BOARD_PARMS* voiceParams );
static int bhpGetDeviceIndex( VOICE_BOARD_PARMS* voiceParams, int endpt, unsigned int type );

/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalProvInit
**
** PURPOSE:    Initialize the board HAL provisioning:
**             Fetch voice-related parameters from the external provisioning
**             (e.g. boardparams tables) and set up internal board hal
**             provisioning tables for later use.
**
** PARAMETERS:
**
** RETURNS:
**
*****************************************************************************
*/
void boardHalProvInit( void )
{
   char boardIdStr[BP_BOARD_ID_LEN];
   char baseBoardIdStr[BP_BOARD_ID_LEN];

   memset ( &voiceParams, 0, sizeof(voiceParams) );

   if ( voiceParamsObtained == VRG_FALSE )
   {
      /* We don't have the voice parameters yet.
      ** First we need to get the board ID from the main BOARD_PARAMETERS table.
      ** Once we get the board ID we will use it as index to the voice tables */
      if ( BpGetVoiceBoardId(boardIdStr) == BP_SUCCESS )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Obtained Voice board id string (%s)", boardIdStr ));
      }
      else
      {
         VRG_LOG_CRIT(( VRG_LOG_MOD_BOARDHAL, "Failed to obtain Voice board id string!" ));
         VRG_LOG_CRIT(( VRG_LOG_MOD_BOARDHAL, "   *** Please verify that Voice board configuration id is properly set up on the CFE prompt" ));
         XCHG_ASSERT(0);
      }

      if ( BpGetBoardId(baseBoardIdStr) == BP_SUCCESS )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Obtained Base board id string (%s)", baseBoardIdStr ));
      }
      else
      {
         VRG_LOG_CRIT(( VRG_LOG_MOD_BOARDHAL, "Failed to obtain Base board id string!" ));
         VRG_LOG_CRIT(( VRG_LOG_MOD_BOARDHAL, "   *** Please verify that Base board configuration id is properly set up on the CFE prompt" ));
         XCHG_ASSERT(0);
      }

      /* Get the voice voice parameters based on the board id string and baseboard id string. */
      if ( BpGetVoiceParms( boardIdStr, &voiceParams, baseBoardIdStr ) == BP_SUCCESS )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Successfully obtained voice parameters" ));
         bhpDumpVoiceParams( &voiceParams );
      }
      else
      {
         VRG_LOG_CRIT(( VRG_LOG_MOD_BOARDHAL, "Failed to obtain voice parameters!, voice NOT configured" ));
      }
   }
   else
   {
      /* Nobody should call boardHalProvInit twice */
      XCHG_ASSERT(0);
   }

   #if defined(CONFIG_BCM96362) || defined(CONFIG_BCM963268) || defined(CONFIG_BCM963138)  || defined(CONFIG_BCM963148)
   if (OTP_GET_USER_BIT(OTP_DECT_DISABLE))
   {
      /* Force the number of DECT line to zero if DECT is disabled in the chip such as 6361 */
      voiceParams.numDectLines = 0;
   }
   #endif

   if ((NUM_IDECT_CHANNELS + NUM_EDECT_CHANNELS) == 0 )
   {
      voiceParams.numDectLines = 0;
   }

   /* Create internal provisioning helper table */
   bhpCreateInternalProvTable( &voiceParams );

   voiceParamsObtained = VRG_TRUE;
}


/*
*****************************************************************************
** FUNCTION:   boardHalProvDeinit
**
** PURPOSE:    Deinitialize the board HAL provisioning:
**
** PARAMETERS:
**
** RETURNS:
**
*****************************************************************************
*/
void boardHalProvDeinit( void )
{
   voiceParamsObtained = VRG_FALSE;
}

/*
*****************************************************************************
** FUNCTION:   boardHalProvGetVoiceParms
**
** PURPOSE:    Get the voice parameters based on the board hal provisioning
**             For example the provisioning can be performed through
**             CommEngine's boardparams structure, which would provide all
**             the necessary parameters for initializing the board HAL.**
** PARAMETERS:
**
** RETURNS:    Pointer to voice parameters
**
*****************************************************************************
*/
VOICE_BOARD_PARMS* boardHalProvGetVoiceParms( void )
{
   if ( voiceParamsObtained == VRG_TRUE )
   {
      return (&voiceParams );
   }
   else
   {
      /* Nobody should call this before successful retrieval
      ** of voice parameters by boardHalProvInit */
      XCHG_ASSERT( 0 );
   }

   return( NULL );
}


/*
*****************************************************************************
** FUNCTION:   boardHalProvGetChannelMap
**
** PURPOSE:    Get the device channel map
**
** PARAMETERS: endpt    -  [IN]  endpoint id
**
** RETURNS:    Pointer to device channel map
**
*****************************************************************************
*/
BOARDHALPROV_DEVICE_CHANNEL_MAP* boardHalProvGetChannelMap( int endpt )
{
   if ( voiceParamsObtained == VRG_TRUE )
   {
      return ( &deviceChannelMap[endpt] );
   }
   else
   {
      /* Nobody should call this before successful boardHalProvInit */
      XCHG_ASSERT( 0 );
   }

   return ( NULL );
}

/*
*****************************************************************************
** FUNCTION:   boardHalGetNumEndpoints
**
** PURPOSE:    Get the number of total endpoints in the system
**             (this includes FXS, FXO, DECT endpoints)
**
** PARAMETERS:
**
** RETURNS:    Total number of endpoints in the system
**
*****************************************************************************
*/
int boardHalGetNumEndpoints( void )
{
   return ( voiceParams.numFxsLines + voiceParams.numFxoLines + voiceParams.numDectLines );
}


/*
*****************************************************************************
** FUNCTION:   boardHalGetNumFxsEndpoints
**
** PURPOSE:    Get the number of total endpoints in the system
**             (this includes FXS, FXO, DECT endpoints)
**
** PARAMETERS:
**
** RETURNS:    Total number of endpoints in the system
**
*****************************************************************************
*/
int boardHalGetNumFxsEndpoints( void )
{
   return ( voiceParams.numFxsLines );
}



/*
*****************************************************************************
** FUNCTION:   boardHalGetNumFxoEndpoints
**
** PURPOSE:    Get the number of total endpoints in the system
**             (this includes FXS, FXO, DECT endpoints)
**
** PARAMETERS:
**
** RETURNS:    Total number of endpoints in the system
**
*****************************************************************************
*/
int boardHalGetNumFxoEndpoints( void )
{
   return ( voiceParams.numFxoLines );
}


/*
*****************************************************************************
** FUNCTION:   boardHalGetNumDectEndpoints
**
** PURPOSE:    Get the number of DECT endpoints in the system
**
** PARAMETERS:
**
** RETURNS:    Number of DECT endpoints in the system
**
*****************************************************************************
*/
int boardHalGetNumDectEndpoints( void )
{
   return ( voiceParams.numDectLines );
}


/*
*****************************************************************************
** FUNCTION:   boardHalGetNumFailoverRelays
**
** PURPOSE:    Get the number of PSTN failover relays
**
** PARAMETERS:
**
** RETURNS:    Total number of relays in the system
**
*****************************************************************************
*/
int boardHalGetNumFailoverRelays( void )
{
   return ( voiceParams.numFailoverRelayPins );
}


/*
*****************************************************************************
** FUNCTION:   boardHalProvGetFlags
**
** PURPOSE:    Get the general-purpose flags from voice board parameters
**
** PARAMETERS:
**
** RETURNS:    32-bit value of voice board general-purpose flags
**
*****************************************************************************
*/
unsigned int boardHalProvGetFlags( void )
{
   return ( voiceParams.flags );
}


/*
*****************************************************************************
** FUNCTION:   bhpDumpVoiceParams
**
** PURPOSE:    Dump the voice parameters
**
** PARAMETERS:
**
** RETURNS:
**
*****************************************************************************
*/
static void bhpDumpVoiceParams( VOICE_BOARD_PARMS* voiceParams )
{
   int deviceId;

   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "---------- Voice board configuration ----------" ));

   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Number of FXS lines          =  %d", voiceParams->numFxsLines ));
   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Number of FXO lines          =  %d", voiceParams->numFxoLines ));
   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Number of DECT lines         =  %d", voiceParams->numDectLines ));
   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Number of failover relay pins=  %d", voiceParams->numFailoverRelayPins ));

   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "" ));
   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      int chan;

      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Voice device %d: ", deviceId ));
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "" ));
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Device type      = %d", voiceParams->voiceDevice[deviceId].voiceDeviceType ));
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Requires reset   = %d", voiceParams->voiceDevice[deviceId].requiresReset ));
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Reset GPIO       = 0x%08X", voiceParams->voiceDevice[deviceId].resetGpio ));
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "SPI device Id    = %d", voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId ));
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "SPI GPIO         = 0x%08X", voiceParams->voiceDevice[deviceId].spiCtrl.spiGpio ));
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "" ));

      for ( chan = 0; chan < BP_MAX_CHANNELS_PER_DEVICE; chan++ )
      {
         if ( voiceParams->voiceDevice[deviceId].channel[chan].status == BP_VOICE_CHANNEL_ACTIVE )
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "\t Channel %d active", chan ));
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "\t Freqrange     =  %d", voiceParams->voiceDevice[deviceId].channel[chan].freqRange ));
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "\t EndianMode    =  %d", voiceParams->voiceDevice[deviceId].channel[chan].endianMode ));
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "\t SampleSize    =  %d", voiceParams->voiceDevice[deviceId].channel[chan].sampleSize ));
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "\t PCM comp mode =  %d", voiceParams->voiceDevice[deviceId].channel[chan].pcmCompMode ));
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "\t TX timeslot   =  %d", voiceParams->voiceDevice[deviceId].channel[chan].txTimeslot ));
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "\t RX timeslot   =  %d", voiceParams->voiceDevice[deviceId].channel[chan].rxTimeslot ));
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "" ));
         }
         else
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "\t Channel %d inactive ", chan));
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "" ));
         }
      }
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "----------" ));
   }
}


/*
*****************************************************************************
** FUNCTION:   bhpCreateInternalProvTable
**
** PURPOSE:    Create internal table tracking associations between
**             global channels that reference the endpoint to
**             device type and internal device channels.
**
** PARAMETERS: voiceParams    -  [IN]  Pointer to retrieved VOICE_BOARD_PARMS
**
** RETURNS:
**
*****************************************************************************
*/
static void bhpCreateInternalProvTable( VOICE_BOARD_PARMS* voiceParams )
{
   int endpt = 0;
   int deviceId;
   int deviceChan;

   /*
   ** Cycle through all the devices and create table mapping for device type,
   ** active device channel, and endpoint channels
   */
   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      if ( (voiceParams->numDectLines == 0) &&
           ((voiceParams->voiceDevice[deviceId].voiceDeviceType == BP_VD_IDECT1) ||
            (voiceParams->voiceDevice[deviceId].voiceDeviceType == BP_VD_EDECT1)) )
      {
         continue;
      }

      for ( deviceChan = 0; deviceChan < BP_MAX_CHANNELS_PER_DEVICE; deviceChan++ )
      {
         if ( voiceParams->voiceDevice[deviceId].channel[deviceChan].status == BP_VOICE_CHANNEL_ACTIVE )
         {
            /* Set the device type for the endpoint */
            deviceChannelMap[endpt].deviceType = voiceParams->voiceDevice[deviceId].voiceDeviceType;
            deviceChannelMap[endpt].deviceChannel = deviceChan;
            deviceChannelMap[endpt].deviceIndex = bhpGetDeviceIndex( voiceParams, endpt, voiceParams->voiceDevice[deviceId].voiceDeviceType );
            deviceChannelMap[endpt].deviceChannelType = voiceParams->voiceDevice[deviceId].channel[deviceChan].type;
            deviceChannelMap[endpt].deviceChannelFreqRange = voiceParams->voiceDevice[deviceId].channel[deviceChan].freqRange;
            deviceChannelMap[endpt].txTimeslot = voiceParams->voiceDevice[deviceId].channel[deviceChan].txTimeslot;
            deviceChannelMap[endpt].rxTimeslot = voiceParams->voiceDevice[deviceId].channel[deviceChan].rxTimeslot;
            VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, " deviceChannelMap[%d].txTimeslot = %d ", endpt, deviceChannelMap[endpt].txTimeslot));
            endpt++;
         }
      }
   }

   /* End the table with BP_VD_NONE for the device type */
   deviceChannelMap[endpt].deviceType = BP_VD_NONE;

#if BOARDHALPROV_DBG
   for ( endpt = 0; deviceChannelMap[endpt].deviceType != BP_VD_NONE; endpt++ )
   {
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "----------" ));
      VRG_LOG_NOTICE((VRG_LOG_MOD_BOARDHAL, "deviceChannelMap[%d].deviceType      = %d ", endpt, deviceChannelMap[endpt].deviceType));
      VRG_LOG_NOTICE((VRG_LOG_MOD_BOARDHAL, "deviceChannelMap[%d].deviceChannel   = %d ", endpt, deviceChannelMap[endpt].deviceChannel));
      VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "----------" ));
   }
#endif /* BOARDHALPROV_DBG */
}


/*
*****************************************************************************
** FUNCTION:   bhpGetDeviceIndex
**
** PURPOSE:    Get the device index based on the endpoint number
**             NOTE: This is the index of a same-type device, not the index
**             of the device in the VOICE_BOARD_PARMS table. For example,
**             if the devices are X, Y, Y the index for the first Y device
**             will be 0 and for the second Y device will be 1.
**
**
** PARAMETERS: endpt    -  [IN]  Endpoint/Line id
**
** RETURNS:
**
*****************************************************************************
*/
static int bhpGetDeviceIndex( VOICE_BOARD_PARMS* voiceParams, int endpt, unsigned int type )
{
   int deviceId;
   int deviceChan;
   int endptCount = 0;
   int indexCount = -1;

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      /* If no dect lines then skip all dect devices */
      if ( (voiceParams->numDectLines == 0) &&
           ((voiceParams->voiceDevice[deviceId].voiceDeviceType == BP_VD_IDECT1) ||
            (voiceParams->voiceDevice[deviceId].voiceDeviceType == BP_VD_EDECT1)) )
      {
         continue;
      }

      /* Increment indexCount whenever a device of specified type is encountered. */

      if ( voiceParams->voiceDevice[deviceId].voiceDeviceType == type )
      {
         indexCount++;
      }

      for ( deviceChan = 0; deviceChan < BP_MAX_CHANNELS_PER_DEVICE; deviceChan++ )
      {
         if ( voiceParams->voiceDevice[deviceId].channel[deviceChan].status == BP_VOICE_CHANNEL_ACTIVE )
         {
            if ( endptCount == endpt )
            {
               VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "endpt %d deviceId %d indexCount %d ", endpt, deviceId, indexCount ));
               /* Return index of device in which this channel resides */
               return ( indexCount );
            }

            /* Endptcount must be incremented whenever we encounter a deviceChan */
            endptCount++;
         }
      }
   }

   return ( 0 );
}
