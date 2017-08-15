/****************************************************************************
*
* Copyright © 2013 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
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
*    Filename: boardHal63138.h
*
****************************************************************************
*    Description:
*
*     Private defines for the 63138 board HAL.
*
****************************************************************************/

#ifndef BOARD_HAL_63138_H
#define BOARD_HAL_63138_H

#ifdef __cplusplus
extern "C" {
#endif

#define USE_DLB_STUB          1

/* ---- Include Files ---------------------------------------------------- */
#include <vrgCountry.h>
#include <xdrvItpc.h>
#include <xdrvSpinLock.h>
#include <xdrvDebug.h>
#include <dlbStub.h>
#include <boardHalInit.h>
#include <board.h>
#include <lhapi.h>

/* ---- Constants and Types ---------------------------------------------- */
#define BOARD_HAL_NUM_SLIC_MAX      2
#define BOARD_HAL_NUM_RELAY_MAX     2

/*
** 63138vcm based reference design doesn't support hook state
** debouncing in hardware, so use only s/w debouncing.
*/
#define BOARD_HAL_63138VCM_HW_DEBOUNCE_MS  0    /* no h/w debouncing */
#define BOARD_HAL_63138VCM_SW_DEBOUNCE_MS  15   /* use only s/w debouncing */

/* ---- Variable Externs ------------------------------------------------- */

/* HAL SHIM configuration table */
extern LHAPI_HALSHIM_CONFIG     *gHalshimConfig;

/* Number of entries in the HAL shom config table */
extern XDRV_UINT32              gHalshimEntryCount;

/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalInit63138
**
** PURPOSE:    Initialize the HAL.
**
** PARAMETERS: country - current country code
**             initModules (not used)
**
** RETURNS:    0 on success, otherwise error code.
**
*****************************************************************************
*/
int boardHalInit63138( VRG_BOARDHAL_INIT_CFG *initCfg, int initModules );


/*
*****************************************************************************
** FUNCTION:   boardHalDeinit63138
**
** PURPOSE:    Uninitialize the HAL. Reset any local storage and
**             free all resources.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, otherwise error code.
**             This function should only return an error under catastrophic
**             circumstances. i.e. Something that cannot be fixed by re-invoking
**             the module initialization function.
**
*****************************************************************************
*/
int boardHalDeinit63138( void );


/*
*****************************************************************************
** FUNCTION:   boardHalDspInit
**
** PURPOSE:    To initialize the DSP, load an image, and start it running.
**
** PARAMETERS:
**
** RETURNS:    0 on success.
**
** NOTE:
*****************************************************************************
*/
int boardHalDspInit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalDspDeinit
**
** PURPOSE:
**
** PARAMETERS:
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void boardHalDspDeinit( void );


#ifdef DYING_GASP_API
void boardHalShutdown63138( void *context );
void boardHalDspShutdown( void *context );
#endif


#ifdef __cplusplus
    }
#endif

#endif  /* BOARD_HAL_63138_H  */
