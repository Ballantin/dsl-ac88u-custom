/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom
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
****************************************************************************
*
*    Filename: slicSi3216.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      Subscriber Line Interface Circuits (SLIC) API.
*
****************************************************************************/

#ifndef SLIC_SI3216_H
#define SLIC_SI3216_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <vrgCountry.h>
#include <xdrvSlic.h>
#include <board.h>


/*
**  Si3216 SLIC driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_SLIC' base-class(structure).
** The base SLIC structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific SLIC drivers.
**
** When the  SLIC driver initialization function is invoked, the base HPI
** structure function pointers will be intialized to a set of hardware specific
** SLIC driver functions. Therefore, given a pointer to the base SLIC driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific SLIC driver function.
*/


/* 
** Si3216 Driver information
*/
typedef struct SLIC_SI3216_DRV
{
   /* Must be first element of this struct */
   const XDRV_SLIC_FUNCS *pDrvFuncs;

   /* Si3216  Specific state info */
   int                    chan;        /* SLIC channel ID            */
   XDRV_BOOL              bDrvEnabled; /* SLIC driver state (enabled/disabled) */
   
   /*  Si3216  Specific config info */
   int pcmMode;                        /* pcm mode 16 bit linear, 8 bit u/A law */
#define SI3216_LINEAR       1
#define SI3216_ULAW         2
#define SI3216_ALAW         3
#define SI3216_WIDEBAND     4

   int txTimeSlot;                     /* pcm highway transmit timeslot */
   int rxTimeSlot;                     /* pcm highway receive timeslot */
   int mspiDevId;                      /* Device ID on MSPI bus  */   
   VRG_COUNTRY country;                /* locale */

} SLIC_SI3216_DRV;

typedef struct
{
   XDRV_UINT8    regWR;    /* register number */
   XDRV_UINT8    regDir;   /* Is a direct register, 1 = direct register, 0 = indirect*/   	
   XDRV_UINT16   regVal;   /* register value */

} SI3216_REGISTER_VALUE;


/*
*****************************************************************************
** FUNCTION:   slicSi3216Init
**
** PURPOSE:    Initialize and open the Si3216 SLIC
**
** PARAMETERS: chan      - the line number ( 0 referenced )
**             pDev      - pointer to the Si3216 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSi3216Init
(
   int                     chan,
   SLIC_SI3216_DRV   *pDev
);


/*
*****************************************************************************
** FUNCTION:   slicSi3216Deinit
**
** PURPOSE:    Shutdown the Si3216 SLIC
**
** PARAMETERS: pDev  - pointer to the Si3216 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSi3216Deinit( SLIC_SI3216_DRV *pDev );

#ifdef DYING_GASP_API
/*
*****************************************************************************
** FUNCTION:   slicSi3216Shutdown
**
** PURPOSE:    Shutdown the Si3216 SLIC
**
** PARAMETERS: rstGpioPin  - reset pin
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSi3216Shutdown( int mspiDevId );
#endif

#if __cplusplus
}
#endif

#endif   /* SLIC_SI3216_H */
