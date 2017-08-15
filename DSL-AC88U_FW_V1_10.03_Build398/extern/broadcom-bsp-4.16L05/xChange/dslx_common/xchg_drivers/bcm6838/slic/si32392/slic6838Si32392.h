/****************************************************************************
*
* Copyright (c) 2008-2012 Broadcom
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
*    Filename: slic6838Si32392.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      Subscriber Line Interface Circuits (SLIC) API.
*
****************************************************************************/

#ifndef SLIC_6838_SI32392_H
#define SLIC_6838_SI32392_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <xdrvSlic.h>
#include <xdrvApm.h>
#include <xdrvItpc.h>
#include <bcm6838.h>

/*
** Si32392 special mode flags
*/
typedef enum
{
   SLIC_6838_SI32392_SPECIAL_MODE_SHORT,
   SLIC_6838_SI32392_SPECIAL_MODE_TIP_SHORT,
   SLIC_6838_SI32392_SPECIAL_MODE_RING_SHORT,
   SLIC_6838_SI32392_SPECIAL_DISABLE_RING_TRANISTION_MODE

} SLIC_6838_SI32392_SPECIAL_MODE;

/*
** 6838 <-> Si32392 SLIC configuration structure.
*/
typedef struct
{

   unsigned int enhancedControl;   /* Enable enhanced control */
   int          spiDevId;          /* SPI device id for communicating */   
} SLICCFG_6838_SI32392_INFO;

/*
** 6838 Si32392 SLIC driver structure. Conceptually, this structure is similar to
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
*/


/*
** Si32392 Driver information
*/
typedef struct SLIC_6838_SI32392_DRV
{
   /* Must be first element of this struct */
   const XDRV_SLIC_FUNCS *pDrvFuncs;

   /* Si32392 6838 Specific state info */
   int                        chan;                  /* SLIC channel ID */
   int                        spiDevId;              /* SPI device id for communicating */   
   XDRV_APM                  *pApmDrv;               /* Pointer to APM driver structure */
   XDRV_ITPC                 *pItpcDrv;              /* Pointer to ITPC driver structure */   
   XDRV_BOOL                  bDrvEnabled;           /* SLIC driver state (enabled/disabled) */
   XDRV_BOOL                  bRingDCoffsetEnabled;  /* SLIC driver DC offset (enabled/disabled) */
   XDRV_BOOL                  phaseReversalActive;   /* Phase Reversal Flag */
   XDRV_SLIC_MODE             slicMode;              /* Current Slic Mode */
   XDRV_BOOL                  bOnBattery;            /* Battery mode */
   XDRV_BOOL                  bBoostedLoopCurrent;   /* Boosted Loop Current Flag */
   XDRV_BOOL                  bEnhancedControl;      /* Enable enhanced control supported */
   int                        ringVoltage;           /* Current Ring Voltage */
   int                        ringOffset;            /* Current Ring DC Offset */
   int                        ringOffsetCal;         /* Current Ring DC Offset Calibration */
   int                        ringFrequency;         /* Current Ring Frequency */
   int                        ringWaveshape;         /* Current Ring Waveshape */
   XDRV_BOOL                  bOffhook;              /* Current hookstate status */
   XDRV_BOOL                  bOffhookStateChange;   /* Hookstate status update occurred */
   int                        offhookCounter;        /* Offhook state counter */
   int                        pendingSlicTime;       /* Pending State Change Info */
   int                        pendingSlicMode;       /* Pending State Change Info */
   int                        pendingSlicState;      /* Pending State Change Info */
   int                        disableRingMode;       /* Block Pending States */   

} SLIC_6838_SI32392_DRV;


/*
*****************************************************************************
** FUNCTION:   slic6838Si32392Init
**
** PURPOSE:    Initialize and open the Si32392 SLIC
**
** PARAMETERS: chan         - the line number ( 0 referenced )
**             pDev         - pointer to the Si32392 SLIC info structure
**             pSlicCfgInfo - SLIC configuration structure
**             pApmDrv      - pointer to APM driver associate with this slic
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838Si32392Init
(
   int                   chan,
   SLIC_6838_SI32392_DRV *pDev,
   const SLICCFG_6838_SI32392_INFO *pSlicCfgInfo,
   XDRV_APM             *pApmDrv
);


/*
*****************************************************************************
** FUNCTION:   slic6838Si32392Deinit
**
** PURPOSE:    Shutdown the Si32392 SLIC
**
** PARAMETERS: pDev  - pointer to the Si32392 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838Si32392Deinit( SLIC_6838_SI32392_DRV *pDev );

/*
*****************************************************************************
** FUNCTION:   slic6838Si32392UpdateState
**
** PURPOSE:    Update SI32392 SLIC state
**
** PARAMETERS: pDev - pointer to the SI32392 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838Si32392UpdateState( SLIC_6838_SI32392_DRV *pDev );

/*
******************************************************************************
** FUNCTION:  slic6838Si32392RegisterRead
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
void slic6838Si32392RegisterRead( SLIC_6838_SI32392_DRV *pDev, XDRV_UINT8 addr, XDRV_UINT16 *data );


/*
******************************************************************************
** FUNCTION:  slic6838Si32392RegisterWrite
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
void slic6838Si32392RegisterWrite( SLIC_6838_SI32392_DRV *pDev, XDRV_UINT8 addr, XDRV_UINT16 data );

/*
******************************************************************************
** FUNCTION:  slic6838Si32392RegisterMaskWrite
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
void slic6838Si32392RegisterMaskWrite( SLIC_6838_SI32392_DRV *pDev, XDRV_UINT8 addr, XDRV_UINT16 data, XDRV_UINT16 mask, int verify );

/*
*****************************************************************************
** FUNCTION:   slic6838Si32392UpdateOffhookStatus
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDev - pointer to the SLIC driver device structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838Si32392UpdateOffhookStatus( SLIC_6838_SI32392_DRV *pDev );


/*
*****************************************************************************
** FUNCTION:   slic6838Si32392SetSpecialMode
**
** PURPOSE:    This function notifies the SLIC to enable or disable
**             various special modes
**
** PARAMETERS: pDrv   - pointer to the SLIC driver device structure
**             mode   - special mode to enable or disable
**             enable - enable or disable
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
int slic6838Si32392SetSpecialMode( SLIC_6838_SI32392_DRV *pDev, SLIC_6838_SI32392_SPECIAL_MODE mode, XDRV_BOOL enable );


#if __cplusplus
}
#endif

#endif   /* SLIC_6838_SI32392_H */
