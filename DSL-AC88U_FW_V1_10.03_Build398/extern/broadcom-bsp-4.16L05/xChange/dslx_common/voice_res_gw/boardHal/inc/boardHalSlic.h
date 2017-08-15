/****************************************************************************
*
* Copyright (c) 2000-2011 Broadcom
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
*    Filename: boardHalSlic.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      Subscriber Line Interface Circuits (SLIC) present on a board.
*
****************************************************************************/

#ifndef BOARD_HAL_SLIC_H
#define BOARD_HAL_SLIC_H

#if __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgCountry.h>
#include <xdrvSlic.h>
#include <vrgTypes.h>
#include <board.h>
#include <vrgCountry.h>
#include <boardHalInit.h>

/* ---- Constants and Types ---------------------------------------------- */
#define LINE_OFFHOOK0         0x00000001
#define LINE_OFFHOOK1         0x00000020

/*
*****************************************************************************
** FUNCTION:   halSlicEnable
**
** PURPOSE:    Disable the SLIC interface on the line
**
** PARAMETERS: lineId      - physical interface to control
**             enableSlic  - controls whether to enable the SLIC i/f
**                           (1 = enable; 0 = disable )
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
typedef int (*halSlicEnable)( int lineId, VRG_BOOL enableSlic );

/*
*****************************************************************************
** FUNCTION:   halSlicReadWriteMemory
**
** PURPOSE:    Writes/reads to/from slic memory
**
** PARAMETERS: endpt - the line number ( 0 referenced ) 
**             addr - the address to read or write
**             data - the data to write to the address ( unused for read operations )
**             write - 0 for read operations, 1 for write operations
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
typedef int (*halSlicReadWriteMemory)( int endpt, int addr, int data, int write );


/*
*****************************************************************************
** FUNCTION:   halSlicForceHvgShutdown
**
** PURPOSE:    Forces the HVG shutdown regarless of the 3341 state
**             (tries to even before 3341 is initialized).
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
** NOTE:       This is intended to be used by the maintanace thread
**             for interroperability with home security systems.
**
*****************************************************************************
*/
typedef int (*halSlicForceHvgShutdown)( void );


/*
*****************************************************************************
** FUNCTION:   halSlicHvgRefresh
**
** PURPOSE:    Refreshes the HVG based on the current operating state
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
**  NOTES:     There are requirements for line voltage behaviour during different
**             states of MTA operation (board reset, voice only reset, connectivity
**             loss) to be programmable and assynchrounously controlled from a  
**             higher application layer (such as Maintanance Task). Satisfying these 
**             requirements may necessitate to shutdown the HVG even after the 
**             halInit has configured the HVG for the last time. 
**             This API allows the application to check if such late shutdown has
**             occured and revive the HVG module if needed.            
**
*****************************************************************************
*/
typedef int (*halSlicHvgRefresh)( void );


/*
*****************************************************************************
** FUNCTION:   halSlicGetDriver
**
** PURPOSE:    Get the SLIC driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the SLIC driver interface
**
*****************************************************************************
*/
typedef XDRV_SLIC* (*halSlicGetDriver)( int chan );

/*
*****************************************************************************
** FUNCTION:   halSlicSetAnalogLoopback
**
** PURPOSE:    Set enable/disable analog loopback.
**
** PARAMETERS: lineId - lineID to enable/disable analog loopback
**             enabled - 0 to disable, 1 to enable
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*halSlicSetAnalogLoopback)( int lineId, VRG_BOOL enabled );


/*
*****************************************************************************
** FUNCTION:   halSlicSetRingParms
**
** PURPOSE:    Inform board HAL of current ring settings
**
** PARAMETERS: chan - endpoint
**             ringVoltage - ringing voltage for this endpoint
**             ringOffset - ringing DC offset for this endpoint
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*halSlicSetRingParms)( int chan, int ringVoltage, int ringOffset );

/*
** Structure that contains all function pointers that define the API.
*/
typedef struct BOARD_HAL_SLIC_FUNCS
{
   halSlicEnable              slicEnable;
   halSlicReadWriteMemory     slicReadWriteMemory;
   halSlicForceHvgShutdown    slicForceHvgShutdown;
   halSlicHvgRefresh          slicHvgRefresh;
   halSlicGetDriver           slicGetDriver;
   halSlicSetAnalogLoopback   slicSetAnalogLoopback;
   halSlicSetRingParms        slicSetRingParms;

} BOARD_HAL_SLIC_FUNCS;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalSlicReset
**
** PURPOSE:    Reset the SLIC
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSlicReset( void );

/*
*****************************************************************************
** FUNCTION:   boardHalSlicInit
**
** PURPOSE:    Initialize the SLIC
**
** PARAMETERS: boardHalInitCfg - current init cfg
**             bHighVring - boolean indicating if this platform supports 
**                          high voltage ringing mode.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicInit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL bHighVring );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicInitApm
**
** PURPOSE:    Initialize the SLIC
**
** PARAMETERS: boardHalInitCfg   [IN]  -  Init configuration flags
**                                        (currently used only to fetch country code)
**             bHighVring        [IN]  -  NOT USED
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicInitApm( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL bHighVring );



/*
*****************************************************************************
** FUNCTION:   boardHalSlicDeinit
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS: board hal init config
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicDeinit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicDeinitApm
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS: board hal init config
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicDeinitApm( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicPowerSave
**
** PURPOSE:    Disable SLICs for power savings
**
** PARAMETERS: lineId      - physical interface to control
**             disableSlic - controls whether to enable power savings
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicPowerSave( int lineId, VRG_BOOL disableSlic );

/*
*****************************************************************************
** FUNCTION:   boardHalSlicEnable
**
** PURPOSE:    Disable the SLIC interface on the line
**
** PARAMETERS: lineId      - physical interface to control
**             enableSlic  - controls whether to enable the SLIC i/f
**                           (1 = enable; 0 = disable )
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicEnable( int lineId, VRG_BOOL enableSlic );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicEnableApm
**
** PURPOSE:    Disable the SLIC interface on the line
**
** PARAMETERS: lineId      - physical interface to control
**             enableSlic  - controls whether to enable the SLIC i/f
**                           (1 = enable; 0 = disable )
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/                 
int boardHalSlicEnableApm( int lineId, VRG_BOOL enableSlic );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicIsDisabled
**
** PURPOSE:    Query status of slic for a given endpt
**
** PARAMETERS: chan - endpt or line number (starting from 0)
**
** RETURNS:    VRG_TRUE if disabled, VRG_FALSE if enabled
**
*****************************************************************************
*/
VRG_BOOL boardHalSlicIsDisabled( int chan );

/*
*****************************************************************************
** FUNCTION:   boardHalSlicReadWriteMemory
**
** PURPOSE:    Writes/reads to/from slic memory
**
** PARAMETERS: endpt - the line number ( 0 referenced ) 
**             addr - the address to read or write
**             data - the data to write to the address ( unused for read operations )
**             write - 0 for read operations, 1 for write operations
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicReadWriteMemory( int endpt, int addr, int data, int write );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicForceHvgShutdown
**
** PURPOSE:    Forces the HVG shutdown regarless of the 3341 state
**             (tries to even before 3341 is initialized).
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
** NOTE:       This is intended to be used by the maintanace thread
**             for interroperability with home security systems.
**
*****************************************************************************
*/
int boardHalSlicForceHvgShutdown( void );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicHvgRefresh
**
** PURPOSE:    Refreshes the HVG based on the current operating state
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
**  NOTES:     There are requirements for line voltage behaviour during different
**             states of MTA operation (board reset, voice only reset, connectivity
**             loss) to be programmable and assynchrounously controlled from a  
**             higher application layer (such as Maintanance Task). Satisfying these 
**             requirements may necessitate to shutdown the HVG even after the 
**             boardHalInit has configured the HVG for the last time. 
**             This API allows the application to check if such late shutdown has
**             occured and revive the HVG module if needed.            
**
*****************************************************************************
*/
int boardHalSlicHvgRefresh( void );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicHvgRefreshApm
**
** PURPOSE:    Refreshes the HVG based on the current operating state
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
**  NOTES:     There are requirements for line voltage behaviour during different
**             states of MTA operation (board reset, voice only reset, connectivity
**             loss) to be programmable and assynchrounously controlled from a  
**             higher application layer (such as Maintanance Task). Satisfying these 
**             requirements may necessitate to shutdown the HVG even after the 
**             boardHalInit has configured the HVG for the last time. 
**             This API allows the application to check if such late shutdown has
**             occured and revive the HVG module if needed.            
**
*****************************************************************************
*/
int boardHalSlicHvgRefreshApm( void );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicGetDriver
**
** PURPOSE:    Get the SLIC driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the SLIC driver interface
**
*****************************************************************************
*/
XDRV_SLIC* boardHalSlicGetDriver( int chan );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicGetDriverApm
**
** PURPOSE:    Get the SLIC driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the SLIC driver interface
**
*****************************************************************************
*/
XDRV_SLIC* boardHalSlicGetDriverApm( int chan );


#ifdef DYING_GASP_API
void boardHalSlicShutdown( void *context );
#endif
/*
*****************************************************************************
** FUNCTION:   boardHalSetAnalogLoopback
**
** PURPOSE:    Set enable/disable analog loopback.
**
** PARAMETERS: lineId - lineID to enable/disable analog loopback
**             enabled - 0 to disable, 1 to enable
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSetAnalogLoopback( int lineId, VRG_BOOL enabled );


/*
*****************************************************************************
** FUNCTION:   boardHalSetAnalogLoopbackApm
**
** PURPOSE:    Set GPIO offhook pin to enable/disable analog loopback.
**
** PARAMETERS: lineId - lineID to enable/disable analog loopback
**             enabled - 0 to disable, 1 to enable
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSetAnalogLoopbackApm( int lineId, VRG_BOOL enabled );


/*
*****************************************************************************
** FUNCTION:   boardHalSlicSetRingParms
**
** PURPOSE:    Inform board HAL of current ring settings
**
** PARAMETERS: chan - endpoint
**             ringVoltage - ringing voltage for this endpoint
**             ringOffset - ringing DC offset for this endpoint
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSlicSetRingParms( int chan, int ringVoltage, int ringOffset );

/*****************************************************************************
** FUNCTION:   boardHalIsSlic
**
** PURPOSE:    Determine whether a particular line uses a Slic
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    XDRV_TRUE if DAA, XDRV_FALSE 
**
*****************************************************************************
*/
XDRV_BOOL boardHalIsSlic( int chan );

/*****************************************************************************
*
* FUNCTION:   boardHalResetGpios
*
* PURPOSE:    Set directions and mode for various GPIOs
*             involved with SLIC deinitialization (relay GPIOs, reset GPIO)
*
* PARAMETERS: none
*
* RETURNS:    0 on success, else 1
*
*****************************************************************************/
int boardHalResetGpios( void );

#if __cplusplus
}
#endif


#endif   /* BOARD_HAL_SLIC_H */
