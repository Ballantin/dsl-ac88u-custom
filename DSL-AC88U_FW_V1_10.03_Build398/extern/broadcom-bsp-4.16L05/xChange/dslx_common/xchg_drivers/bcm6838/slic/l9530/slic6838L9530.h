/***************************************************************************
*    Copyright © 2008 Broadcom                                  
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
*    Filename: slic6838L9530.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      Subscriber Line Interface Circuits (SLIC) API.
*
****************************************************************************/

#ifndef SLIC_6838_L9530_H
#define SLIC_6838_L9530_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <xdrvSlic.h>
#include <xdrvApm.h>
#include <bcm6838.h>


/*
** Pin layout.
** The host processor is connected to the SLIC through GPIOs,
** which control the SLIC operation or sense the inputs for hook state changes.
** The following pins of Le9530 are connected to the host processor:
**
**    B2, B1, B0  - These three pins are used to control the SLIC operating state
**    TLDEN       - This pin is used to switch in test load
**    NSTAT       - This pin is use for hook detection
**
*/
typedef struct
{
   XDRV_UINT32 B0_pin;
   XDRV_UINT32 B1_pin;
   XDRV_UINT32 B2_pin;
   XDRV_UINT32 NSTAT_pin;
   XDRV_UINT32 TLDEN_pin;

} SLICCFG_6838_L9530_PIN_INFO;


/*
** GPIO control pointers for B0, B1, B2, TLDEN, and NSTAT.
** xyzCtrlp refers to the GPIO direction control (32-bit)
** xyzDatap refers to the GPIO value (32-bit)
** xyzMask refers to the GPIO mask
*/
typedef struct
{
   volatile XDRV_UINT32 *slicIoCtrlp;        /* location to control SLIC I/O directions (for B0, B1, B2) */
   volatile XDRV_UINT32 *slicIoTldCtrlp;     /* location to control SLIC I/O directions (for TLDEN) */
   volatile XDRV_UINT32 *slicIoNstatCtrlp;   /* location to control SLIC I/O directions (for NSTAT) */

   volatile XDRV_UINT32 *slicIoDatap;        /* location to control SLIC I/O states */
   volatile XDRV_UINT32 *slicIoTldDatap;     /* location to control SLIC TLDEN */
   volatile XDRV_UINT32 *slicIoNstatDatap;   /* location to control SLIC NSTAT */

   XDRV_UINT32          slicIoStateMask;     /* Mask to control SLIC state pins - B2,B1,B0 */
   XDRV_UINT32          slicIoTldMask;       /* Mask for control of TLDEN */
   XDRV_UINT32          slicIoNstatMask;     /* Mask for control of NSTAT */

   unsigned int enhancedControl;          /* Enable enhanced control */

} SLICCFG_6838_L9530_CONNECT_INFO;


/*
** 6838 L9530 SLIC driver structure. Conceptually, this structure is similar to
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
** L9530 SLIC driver state that is specific to the 6838 should be stored in the
** 'SLIC_6838_L9530_DRV' structure. A pointer to 'XDRV_SLIC_FUNCS' must be the first
** member of the 'SLIC_6838_L9530_DRV' struct.
*/
typedef struct
{
   /* Must be first element of this struct. */
   const XDRV_SLIC_FUNCS            *pDrvFuncs;

   /* L9530 6838-specific state info. */
   int                              chan;                  /* SLIC channel ID              */
   XDRV_APM                        *pApmDrv;               /* Pointer to APM driver structure */
   SLICCFG_6838_L9530_PIN_INFO      pinInfo;               /* SLIC pin information */
   SLICCFG_6838_L9530_CONNECT_INFO  slicCfgInfo;           /* SLIC conectivity configuration structure */
   XDRV_BOOL                        bRingDCoffsetEnabled;  /* ring DC offset enable flag (1=ON, 0=OFF) */
   XDRV_BOOL                        bDrvEnabled;           /* SLIC driver state (enabled/disabled) */
   XDRV_BOOL                        phaseReversalActive;   /* Phase Reversal Flag */
   XDRV_SLIC_MODE                   slicMode;              /* Current Slic Mode */
   XDRV_BOOL                        bOnBattery;            /* battery mode (true=on battery; false=on AC power) */
   XDRV_BOOL                        bBoostedLoopCurrent;   /* Boosted Loop Current Flag */
   XDRV_BOOL                        bEnhancedControl;      /* Enhanced control supported */
   XDRV_BOOL                        bEnhancedControlEna;   /* Enhanced control enabled */
   int                              ringVoltage;           /* Current Ring Voltage */
   int                              ringOffset;            /* Current Ring DC Offset */
   int                              ringOffsetCal;         /* Current Ring DC Offset Calibration */
   int                              ringFrequency;         /* Current Ring Frequency */
   int                              ringWaveshape;         /* Current Ring Waveshape */
   int                              pendingSlicTime;       /* Pending State Change Info */
   int                              pendingSlicMode;       /* Pending State Change Info */
   int                              pendingSlicState;      /* Pending State Change Info */

} SLIC_6838_L9530_DRV;


/*
*****************************************************************************
** FUNCTION:   slic6838L9530Init
**
** PURPOSE:    Initialize and open the L9530 SLIC
**
** PARAMETERS: pDev           - pointer to the L9530 SLIC info structure
**             chan           - channel number 
**             pSlicPinInfo   - Information about GPIO pins used for SLIC control
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838L9530Init
(
   SLIC_6838_L9530_DRV              *pDev,
   int                              chan,
   const SLICCFG_6838_L9530_PIN_INFO *pSlicPinInfo,
   XDRV_APM                         *pApmDrv
);



/*
*****************************************************************************
** FUNCTION:   slic6838L9530Deinit
**
** PURPOSE:    Shutdown the L9530 SLIC
**
** PARAMETERS: pDev  - pointer to the L9530 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838L9530Deinit( SLIC_6838_L9530_DRV  *pDev );


#if defined(__cplusplus)
}
#endif


#endif   /* SLIC_6838_L9530_H */
