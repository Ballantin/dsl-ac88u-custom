/****************************************************************************
*  
*  Copyright (c) 2000-2009 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
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
*    Filename: boardHalMain.h
*
****************************************************************************
*    Description:
*
*      This file contains main entry points for initialization/deinitialization
*      of the board as well as several necessary utility functions.
*
****************************************************************************/

#ifndef BOARD_HAL_MAIN_H
#define BOARD_HAL_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <boardparms_voice.h>
#include <vrgTypes.h>

/* ---- Constants and Types ---------------------------------------------- */

typedef struct BOARDHALPROV_DEVICE_CHANNEL_MAP
{
   /* Device type for the endpoint */
   BP_VOICE_DEVICE_TYPE    deviceType;

   /* Index for the device. If only one device of a certain type is used the '
   ** index for that device will be zero. If two devices of a same type are used
   ** the index will be 0 and 1 */
   int                     deviceIndex;

   /* Internal channel in the device (e.g. channel 0 is FXS and channel 1 is FXO on Si32178) */ 
   int                     deviceChannel;

   /* Type of channel (SLIC, DAA, DECT, etc.) */ 
   BP_VOICE_CHANNEL_TYPE   deviceChannelType;
   
   /* Freq range of channel ( Narrowband/Wideband ) */
   BP_VOICE_CHANNEL_FREQRANGE deviceChannelFreqRange;

   /* TX/RX timeslots for the channel */
   unsigned int            txTimeslot;
   unsigned int            rxTimeslot;

} BOARDHALPROV_DEVICE_CHANNEL_MAP;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


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
void boardHalProvInit( void );

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
void boardHalProvDeinit( void );

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
VOICE_BOARD_PARMS* boardHalProvGetVoiceParms( void );


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
BOARDHALPROV_DEVICE_CHANNEL_MAP* boardHalProvGetChannelMap( int endpt );


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
int boardHalGetNumEndpoints( void );


/*
*****************************************************************************
** FUNCTION:   boardHalGetNumFxoEndpoints
**
** PURPOSE:    Get the number of FXO endpoints in the system
**
** PARAMETERS: 
**
** RETURNS:    Number of FXO endpoints in the system
**
*****************************************************************************
*/
int boardHalGetNumFxoEndpoints( void );

/*
*****************************************************************************
** FUNCTION:   boardHalGetNumFxsEndpoints
**
** PURPOSE:    Get the number of FXS endpoints in the system
**
** PARAMETERS: 
**
** RETURNS:    Number of FXs endpoints in the system
**
*****************************************************************************
*/
int boardHalGetNumFxsEndpoints( void );


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
int boardHalGetNumDectEndpoints( void );


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
int boardHalGetNumFailoverRelays( void );


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
unsigned int boardHalProvGetFlags( void );


#ifdef __cplusplus
    }
#endif

#endif  /* BOARD_HAL_INIT_H  */
