/***************************************************************************
*    Copyright � 2013 Broadcom Corporation
*
*    This program is the proprietary software of Broadcom Corporation and/or
*    its licensors, and may only be used, duplicated, modified or
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized
*    License").  Except as set forth in an Authorized License, Broadcom
*    grants no license (express or implied), right to use, or waiver of any
*    kind with respect to the Software, and Broadcom expressly reserves all
*    rights in and to the Software and all intellectual property rights
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.
*
*
*  Except as expressly set forth in the Authorized License,
*
*    1.     This program, including its structure, sequence and
*    organization, constitutes the valuable trade secrets of Broadcom, and
*    you shall use all reasonable efforts to protect the confidentiality
*    thereof, and to use this information only in connection with your use
*    of Broadcom integrated circuit products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT
*    OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL,
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: boardHalDsp63148.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent DSP interface for
*     the 63148.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stddef.h>
#include <xchgAssert.h>
#include <boardHalDspTask.h>
#include <boardHalInit.h>
#include <boardHal63148.h>
#include <halshim.h>
#include <vrgLog.h>

/* Contains "DSP" image capability and configuration info. */
#include <dspAppNondistArchive.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
#define BOARDHAL_DSP_INIT_SUCCESS  0
#define BOARDHAL_DSP_INIT_ERROR    -1

/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalDspInit
**
** PURPOSE:    To initialize the DSP, load an image, and start it running.
**
** PARAMETERS: None.
**
** RETURNS:    0 on success.
**
** NOTE:
*****************************************************************************
*/
int boardHalDspInit( void )
{
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "DSP build date: %s", VRG_DSP_DATE_STR ));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalDspDeinit
**
** PURPOSE:
**
** PARAMETERS: Nothing.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void boardHalDspDeinit( void )
{
   /* Not valid for non-dist */
}

#ifdef DYING_GASP_API
void boardHalDspShutdown( void *context )
{
   (void)context;
}
#endif

/*
*****************************************************************************
** FUNCTION:   boardHalDspRegisterCallback
**
** PURPOSE:    Registers callback with the HAL that should be invoked
**             periodically in order to invoke the kernel of the voice
**             processing stack. The timer implementation is HAL specific
**             since it may have dependencies on APM/TDM drivers, or require
**             communication with the DSP HAL.
**
** PARAMETERS: Callback to invoke periodically.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void boardHalDspRegisterCallback( BOARD_HAL_DSP_TASK_FUNC dspTaskFunc )
{
   boardHalDspTaskRegisterCallback( dspTaskFunc );
}


/*
*****************************************************************************
** FUNCTION:   dspImageArchiveGet
**
** PURPOSE:    Retrieve DSP capabilities from the image archive
**
** PARAMETERS: none
**
** RETURNS:    pointer to DSP capabilities
**
*****************************************************************************
*/
const DSP_IMAGE_ARCHIVE *dspImageArchiveGet( void )
{
   return( &dspAppNondistArchive );
}

/*
*****************************************************************************
** FUNCTION:   boardHalDspGetDebugDriver
**
** PURPOSE:    Get the dsp debug driver.
**
** PARAMETERS: None
**
** RETURNS:    Pointer to the Debug driver interface
**
*****************************************************************************
*/
XDRV_DEBUG* boardHalDspGetDebugDriver( void )
{
   /* Not valid for non-dist */
   return ( NULL );
}

void boardHalDspEgressProcess( void )
{
   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "******************* ENTERED boardHalDspEgressProcess ******************* "));
}

void boardHalDspIngressProcess( void )
{
   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "******************* ENTERED boardHalDspIngressProcess ******************* "));
}

/*
*****************************************************************************
** FUNCTION:   boardHalDspIsNarrowbandHal
**
** PURPOSE:    Checks if the specified HAL device is configured to run in
**             narrow band mode.
**
** PARAMETERS: devIndex (in) HAL device index in config entry table.
**
** RETURNS:    XDRV_TRUE if HAL device is narrowband.
**             XDRV_FALSE if HAL device is wideband.
**
*****************************************************************************
*/
XDRV_BOOL boardHalDspIsNarrowbandHal( XDRV_UINT16 devIndex )
{
   XCHG_ASSERT( devIndex < gHalshimEntryCount );

   return (gHalshimConfig[devIndex].halShimMode == HALSHIM_NARROWBAND_MODE) ? XDRV_TRUE : XDRV_FALSE;
}
