/****************************************************************************
*
* Copyright © 2008 Broadcom
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
*    Filename: slic6828Si3239.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      Subscriber Line Interface Circuits (SLIC) API.
*
****************************************************************************/

#ifndef SLIC_6828_SI3239_H
#define SLIC_6828_SI3239_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <xdrvSlic.h>
#include <xdrvApm.h>
#include <xdrvItpc.h>
#include <bcm6828.h>

/*
** Si3239 special mode flags
*/
typedef enum
{
   SLIC_6828_SI3239_SPECIAL_DISABLE_RING_TRANISTION_MODE

} SLIC_6828_SI3239_SPECIAL_MODE;
/*
** 6828 <-> Si3239 SLIC configuration structure.
*/
typedef struct
{

   unsigned int enhancedControl;   /* Enable enhanced control */

} SLICCFG_6828_SI3239_INFO;

/*
** 6828 Si3239 SLIC driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_SLIC' base-class(structure).
** The base SLIC structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific SLIC drivers.
**
** When the 6828 SLIC driver initialization function is invoked, the base HPI
** structure function pointers will be intialized to a set of hardware specific
** SLIC driver functions. Therefore, given a pointer to the base SLIC driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific SLIC driver function.
*/


/*
** Si3239 Driver information
*/
typedef struct SLIC_6828_SI3239_DRV
{
   /* Must be first element of this struct */
   const XDRV_SLIC_FUNCS *pDrvFuncs;

   /* Si3239 6828 Specific state info */
   int                        chan;                  /* SLIC channel ID */
   XDRV_APM                  *pApmDrv;               /* Pointer to APM driver structure */
   XDRV_ITPC                 *pItpcDrv;              /* Pointer to ITPC driver structure */   
   XDRV_BOOL                  bDrvEnabled;           /* SLIC driver state (enabled/disabled) */
   XDRV_BOOL                  bRingDCoffsetEnabled;  /* SLIC driver DC offset (enabled/disabled) */
   XDRV_BOOL                  phaseReversalActive;   /* Phase Reversal Flag */
   XDRV_SLIC_MODE             slicMode;              /* Current Slic Mode */
   XDRV_BOOL                  bOnBattery;            /* Battery mode */
   XDRV_BOOL                  bBoostedLoopCurrent;   /* Boosted Loop Current Flag */
   XDRV_BOOL                  bEnhancedControl;      /* Enable enhanced control */
   int                        ringVoltage;           /* Current Ring Voltage */
   int                        ringOffset;            /* Current Ring DC Offset */
   int                        ringOffsetCal;         /* Current Ring DC Offset Calibration */
   int                        ringFrequency;         /* Current Ring Frequency */
   int                        ringWaveshape;         /* Current Ring Waveshape */
   
   int                        pendingSlicTime;       /* Pending State Change Info */
   int                        pendingSlicMode;       /* Pending State Change Info */
   int                        pendingSlicState;      /* Pending State Change Info */
   int                        disableRingMode;       /* Block Pending States */   

} SLIC_6828_SI3239_DRV;


/*
*****************************************************************************
** FUNCTION:   slic6828Si3239Init
**
** PURPOSE:    Initialize and open the Si3239 SLIC
**
** PARAMETERS: chan         - the line number ( 0 referenced )
**             pDev         - pointer to the Si3239 SLIC info structure
**             pSlicCfgInfo - SLIC configuration structure
**             pApmDrv      - pointer to APM driver associate with this slic
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6828Si3239Init
(
   int                   chan,
   SLIC_6828_SI3239_DRV *pDev,
   const SLICCFG_6828_SI3239_INFO *pSlicCfgInfo,
   XDRV_APM             *pApmDrv
);


/*
*****************************************************************************
** FUNCTION:   slic6828Si3239Deinit
**
** PURPOSE:    Shutdown the Si3239 SLIC
**
** PARAMETERS: pDev  - pointer to the Si3239 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6828Si3239Deinit( SLIC_6828_SI3239_DRV *pDev );


/*
******************************************************************************
** FUNCTION:  slic6828Si3239RegisterRead
**
** PURPOSE:   Read from a memory address via MSPI
**
** PARAMETERS:
**            chan     - the line number ( 0 referenced )
**            addr     - address of the control register
**            data     - data read from the control register
**            indirect - allows read from indirect memory
**
** RETURNS:   none
**
*****************************************************************************
*/
void slic6828Si3239RegisterRead( int chan, XDRV_UINT8 addr, XDRV_UINT16 *data );


/*
******************************************************************************
** FUNCTION:  slic6828Si3239RegisterWrite
**
** PURPOSE:   Write to a memory address via MSPI
**
** PARAMETERS:
**            chan     - the line number ( 0 referenced )
**            addr     - address of the control register
**            data     - data to write to the control register
**            indirect - allows read from indirect memory
**
** RETURNS:   none
**
*****************************************************************************
*/
void slic6828Si3239RegisterWrite( int chan, XDRV_UINT8 addr, XDRV_UINT16 data );

/*
******************************************************************************
** FUNCTION:  slic6828Si3239RegisterMaskWrite
**
** PURPOSE:   Write to the masked location of a memory address via MSPI
**            and verify the write if requested to
**
** PARAMETERS:
**            chan - the line number ( 0 referenced )
**            addr - memory address to be written to
**            data - 8-bit data to be written
**            mask - mask to the data to be written
**            verify - bool flag to verify the write
**
** RETURNS:   0 on success, 1 otherwise
**
*****************************************************************************
*/
void slic6828Si3239RegisterMaskWrite( int chan, XDRV_UINT8 addr, XDRV_UINT16 data, XDRV_UINT16 mask, int verify );


#if __cplusplus
}
#endif

#endif   /* SLIC_6828_SI3239_H */
