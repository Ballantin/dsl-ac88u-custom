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
*    Filename: daaLe88010.h
*
****************************************************************************
*    Description:
*
*      This file contains function declarations for the control of the
*      Le88010 Data Access Arrangement circuit (DAA).
*
****************************************************************************/

#ifndef DAA_Le88010_H
#define DAA_Le88010_H

#if __cplusplus
extern "C" {
#endif

#include <xdrvDaa.h>
#include <bcmChip.h>
#include "daaLe88010Defs.h"
#include <vrgCountry.h>
#include <board.h>

/*
**  Le88010 DAA driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_DAA' base-class(structure).
** The base DAA structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific DAA drivers.
**
** When the  DAA driver initialization function is invoked, the base HPI
** structure function pointers will be intialized to a set of hardware specific
** DAA driver functions. Therefore, given a pointer to the base DAA driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific DAA driver function.
**
** Le88010 DAA driver state that is specific to the  should be stored in the
** 'DAA_Le88010_DRV' structure.  Note that since Le88010 is a single-channel
** DAA, one instance of 'DAA_Le88010_DRV' contains information for 1
** physical FXO (PSTN) endpoints.  Information specific to each endpoint is saved inside
** the 'DAA_Le88010_CHAN' structure.
*/

/* 
 * Le88010 Single-channel information
 */
typedef struct DAA_Le88010_CHAN
{
   /* Must be first element of this struct */
   const XDRV_DAA_FUNCS  *pDrvFuncs;

   /* Le88010 state info */
   int                    chan;

} DAA_Le88010_CHAN;

/* 
 * Le88010 Dual-DAA information
 */
typedef struct DAA_Le88010_DRV
{
   /* Two channels per Le88010 DAA */
   DAA_Le88010_CHAN        daaInfo;
   int                     daaId;
   VRG_COUNTRY             daaLocale;
   int                     mspiId;       /* device ID on MSPI */
   int                     pcmMode;       /* 16 bit linear, 8 bit u/A law, or wideband */
   int                     txTimeSlot;    /* pcm highway transmit timeslot */
   int                     rxTimeSlot;    /* pcm highway receive timeslot */

} DAA_Le88010_DRV;


/*
   Le88010 Register Value Definition.
*/

typedef struct
{
   XDRV_UINT8    regWR;
   XDRV_UINT8    regLen;
   XDRV_UINT8    regVal[LE88010_MAX_CMD_LENGTH];

} LE88010_REGISTER_VALUE;

typedef enum LE88010_PCM_MODE
{
   LE88010_PCM_MODE_LINEAR,
   LE88010_PCM_MODE_ULAW,
   LE88010_PCM_MODE_ALAW
} LE88010_PCM_MODE;

/*
*****************************************************************************
** FUNCTION:   daaLe88010Init
**
** PURPOSE:    Initialize and open the Le88010 DAA
**
** PARAMETERS: locale      - locale/country enum
**             chan        - the line number ( 0 referenced )
**             pDev        - pointer to the Le88010 DAA info structure
**             rstGpioPin  - GPIO pin used to reset the DAA
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaLe88010Init
(
   VRG_COUNTRY             locale,
   int                     chan,
   DAA_Le88010_DRV*        pDev, 
   XDRV_UINT16             rstGpioPin
);

/*****************************************************************************
*
* FUNCTION:   daaLe88010Reset
*
* PURPOSE:    Reset the DAAs
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
void daaLe88010Reset
(
   XDRV_UINT16 rstGpioPin
);

/*
*****************************************************************************
** FUNCTION:   daaLe88010Deinit
**
** PURPOSE:    Shutdown the Le88010 DAA
**
** PARAMETERS: pDev  - pointer to the Le88010 DAA info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaLe88010Deinit( DAA_Le88010_DRV *pDev );

#ifdef DYING_GASP_API
void daaLe88010Shutdown ( XDRV_UINT16 rstGpioPin );
#endif

#if __cplusplus
}
#endif


#endif   /* DAA_Le88010_H */
