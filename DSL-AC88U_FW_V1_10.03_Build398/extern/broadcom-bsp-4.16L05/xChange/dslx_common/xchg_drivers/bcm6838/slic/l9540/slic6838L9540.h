/***************************************************************************
*    Copyright (c) 2008-2012 Broadcom                                        
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
*    Filename: slic6838L9540.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      Subscriber Line Interface Circuits (SLIC) API.
*
****************************************************************************/

#ifndef SLIC_6838_L9540_H
#define SLIC_6838_L9540_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <xdrvSlic.h>
#include <xdrvApm.h>
#include <bcm6838.h>


/*
** 6838 <-> L9540 SLIC connectivity configuration structure.
*/
typedef struct
{
   unsigned int enhancedControl;          /* Enable enhanced control */

} SLICCFG_6838_L9540_CONNECT_INFO;

/*
** 6838 L9540 SLIC driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_SLIC' base-class(structure).
** The base SLIC structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific SLIC drivers.
**
** When the 6838 SLIC driver initialization function is invoked, the base HPI
** structure function pointers will be intialized to a set of hardware specific
** SLIC driver functions. Therefore, given a pointer to the base SLIC driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific SLIC driver function.
**
** L9540 SLIC driver state that is specific to the 6838 should be stored in the
** 'SLIC_6838_L9540_DRV' structure. A pointer to 'XDRV_SLIC_FUNCS' must be the first
** member of the 'SLIC_6838_L9540_DRV' struct.
*/
typedef struct
{
   /* Must be first element of this struct. */
   const XDRV_SLIC_FUNCS            *pDrvFuncs;

   /* L9540 6838-specific state info. */
   int                              chan;                  /* SLIC channel ID              */
   XDRV_APM                        *pApmDrv;               /* Pointer to APM driver structure */
   SLICCFG_6838_L9540_CONNECT_INFO  slicCfgInfo;           /* SLIC conectivity configuration structure */
   XDRV_BOOL                        bRingDCoffsetEnabled;  /* ring DC offset enable flag (1=ON, 0=OFF) */
   XDRV_BOOL                        bDrvEnabled;           /* SLIC driver state (enabled/disabled) */
   XDRV_BOOL                        phaseReversalActive;   /* Phase Reversal Flag */
   XDRV_SLIC_MODE                   slicMode;              /* Current Slic Mode */
   XDRV_BOOL                        bOnBattery;            /* battery mode (true=on battery; false=on AC power) */
   XDRV_BOOL                        bBoostedLoopCurrent;   /* Boosted Loop Current Flag */
   XDRV_BOOL                        bTestLoad;             /* Test Load Flag */
   XDRV_BOOL                        bEnhancedControl;      /* Enhanced control supported */
   int                              ringVoltage;           /* Current Ring Voltage */
   int                              ringOffset;            /* Current Ring DC Offset */
   int                              ringOffsetCal;         /* Current Ring DC Offset Calibration */
   int                              ringFrequency;         /* Current Ring Frequency */
   int                              ringWaveshape;         /* Current Ring Waveshape */
   int                              pendingSlicTime;       /* Pending State Change Info */
   int                              pendingSlicMode;       /* Pending State Change Info */
   int                              pendingSlicState;      /* Pending State Change Info */

} SLIC_6838_L9540_DRV;


/*
*****************************************************************************
** FUNCTION:   slic6838L9540Init
**
** PURPOSE:    Initialize and open the L9540 SLIC
**
** PARAMETERS: pDev          - pointer to the L9540 SLIC info structure
**             chan          - channel number 
**             pSlicCfgCInfo - SLIC conectivity configuration structure 
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838L9540Init
(
   SLIC_6838_L9540_DRV              *pDev,
   int                              chan,
   const SLICCFG_6838_L9540_CONNECT_INFO *pSlicCfgCInfo,
   XDRV_APM                        *pApmDrv
);


/*
*****************************************************************************
** FUNCTION:   slic6838L9540Deinit
**
** PURPOSE:    Shutdown the L9540 SLIC
**
** PARAMETERS: pDev  - pointer to the L9540 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838L9540Deinit( SLIC_6838_L9540_DRV *pDev );


/*
*****************************************************************************
** FUNCTION:   slic6838L9540UpdateState
**
** PURPOSE:    Update L9540 SLIC state
**
** PARAMETERS: pDev - pointer to the L9540 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838L9540UpdateState( SLIC_6838_L9540_DRV *pDev );


/*
*****************************************************************************
** FUNCTION:   slic6838L9540SetTestLoad
**
** PURPOSE:    This function controls the SLIC test load
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void slic6838L9540SetTestLoad( XDRV_SLIC *pDrv, int value );


#if defined(__cplusplus)
}
#endif


#endif   /* SLIC_6838_L9540_H */
