/***************************************************************************
*
*    Copyright © 2000-2008 Broadcom                                        
*                                                                           
*    This program is the proprietary software of Broadcom and/or
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
*    Except as expressly set forth in the Authorized License,               
*                                                                           
*    1.     This program, including its structure, sequence and             
*    organization, constitutes the valuable trade secrets of Broadcom, and  
*    you shall use all reasonable efforts to protect the confidentiality    
*    thereof, and to use this information only in connection with your use  
*    of Broadcom integrated circuit products.                               
*                                                                           
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
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
*    Filename: boardHalApm.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      Analog Processing Module (APM).
*
****************************************************************************/

#ifndef BOARD_HAL_APM_H
#define BOARD_HAL_APM_H

#if __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvApm.h>
#include <vrgTypes.h>
#include <vrgCountry.h>
#include <boardHalInit.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalApmInit
**
** PURPOSE:    Initialize the APM
**
** PARAMETERS: boardHalInitCfg   - current init cfg
**             widebandMode      - 0=8kHz sampling; 1=16kHz sampling
**             packetRateSamples - packet rate in 8kHz samples
**             useRingGen        - use APM ring generator
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmInit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL widebandMode,
                     int packetRateSamples, int useRingGen );


/*
*****************************************************************************
** FUNCTION:   boardHalApmDeinit
**
** PURPOSE:    Shutdown the APM
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmDeinit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalApmGetDriver
**
** PURPOSE:    Get the APM driver
**
** PARAMETERS: dspNum - the DSP number to get the APM driver for.
**
** RETURNS:    pointer to the APM driver interface
**
*****************************************************************************
*/
XDRV_APM* boardHalApmGetDriver( int dspNum );

/*
*****************************************************************************
** FUNCTION:   boardHalApmSyncClkSource
**
** PURPOSE:    Select internal NTP clock or sync to TRC clock for APM module
**
** PARAMETERS: FALSE - change to use internal NTP
**             TRUE - revert back to original settings 
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalApmSyncClkSource( int clkSource );


/*
*****************************************************************************
** FUNCTION:   boardHalApmGetSlicType
**
** PURPOSE:    Get slic type from APM Flexicalc Cfg
**
** PARAMETERS: none
**
** RETURNS:    slic type
**
*****************************************************************************
*/
XDRV_UINT32 boardHalApmGetSlicType( void );

/*
*****************************************************************************
** FUNCTION:   boardHalApmHPFControl
**
** PURPOSE:    Determines if HPF control is available in the APM
**
** PARAMETERS: none
**
** RETURNS:    True if available, false otherwise
**
*****************************************************************************
*/
VRG_BOOL boardHalApmHPFControl( void );

/*
*****************************************************************************
** FUNCTION:   boardHalApmGetChanSwapStatus
**
** PURPOSE:    Determines if channel swap is set in the APM
**
** PARAMETERS: none
**
** RETURNS:    True if set, false otherwise
**
*****************************************************************************
*/
int boardHalApmGetChanSwapStatus( void );

#if __cplusplus
}
#endif


#endif   /* BOARD_HAL_APM_H */
