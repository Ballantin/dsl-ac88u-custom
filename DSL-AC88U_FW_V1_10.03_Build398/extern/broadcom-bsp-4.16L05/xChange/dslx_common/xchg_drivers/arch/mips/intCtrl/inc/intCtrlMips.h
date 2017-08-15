/****************************************************************************
*  
*  Copyright (c) 2008 Broadcom
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
*    Filename: intCtrlMips.h
*
****************************************************************************
*    Description:
*
*     Interface for MIPS interrupt control module.
*
*     This module allows clients to register interrupt service routines, for
*     each interrupt source supported by a specific MIPS core, including
*     individual peripheral interrupts.
*
*     User registered interrupt service routines are high level (C language)
*     functions. Low-level, architecture specific, processor context saves
*     will be performed prior to invoking registered handlers. Like-wise,
*     a context restore will be performed after the registered handler returns.
*
****************************************************************************/

#ifndef INT_CTRL_MIPS_H
#define INT_CTRL_MIPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvIntCtrl.h>


/* Chip-specific interrupt control definitions, e.g. peripheral interrupt sources. */
#include <intCtrlMipsCustom.h>


/* This macro must be defined in intCtrlMipsCustom.h. It indicates whether a
** specified interrupt id is a peripheral interrupt. */
#ifndef INT_CTRL_MIPS_IS_PERIPH_ID
#error "INT_CTRL_MIPS_IS_PERIPH_ID implementation required!"
#endif

/* This macro must be defined in intCtrlMipsCustom.h. It maps the peripheral
** interrupt id to a 0-indexed number. */
#ifndef INT_CTRL_MIPS_PERIPH_ID_TO_INDEX
#error "INT_CTRL_MIPS_PERIPH_ID_TO_INDEX implementation required!"
#endif


/* ---- Constants and Types ---------------------------------------------- */

/*
** List of standard MIPS core interrupt id's. These should be used to register
** user interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
**
** NOTE - the names used below are from the perspective of the MIPS core, NOT
** the Broadcom integrated processor. This is especially important for the
** external interrupts. One (or more) of these will be used as an internal
** peripheral interrupt source for the Broadcom processor. Typically this is
** MIPS core external interrupt0. In this case, the MIPS core external
** interrupt1 would map to the Broadcom external chip interrupt0. Also,
** some processors (dual-cores) may have more than one peripheral interrupt.
**
** The list of interrupt id's listed below can be extended to include
** peripheral interrupt sources. This should be provided in the chip-specific
** header file intCtrlMipsCustom.h. Note that the peripheral interrupts id's
** must be unique and not collide with those listed below.
*/

typedef enum INT_CTRL_MIPS_INTERRUPT_ID
{
   /* Software interrupts. */
   INT_CTRL_MIPS_SW_INTERRUPT_0     = 0,
   INT_CTRL_MIPS_SW_INTERRUPT_1,


   /* External interrupts. */
   INT_CTRL_MIPS_CORE_EXT_INTERRUPT_0,
   INT_CTRL_MIPS_CORE_EXT_INTERRUPT_1,
   INT_CTRL_MIPS_CORE_EXT_INTERRUPT_2,
   INT_CTRL_MIPS_CORE_EXT_INTERRUPT_3,
   INT_CTRL_MIPS_CORE_EXT_INTERRUPT_4,

   /* Timer interrupt. */
   INT_CTRL_MIPS_TIMER_INTERRUPT,

   /* This must be last. */
   INT_CTRL_MIPS_NUM_INTERRUPTS

} INT_CTRL_MIPS_INTERRUPT_ID;



/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   intCtrlInitMips
**
** PURPOSE:    Initialize MIPS interrupt control driver.
**
** PARAMETERS: periphIntId    (in)  Interrupt id of peripheral interrupt. On most
**                                  chips, this is INT_CTRL_MIPS_EXT_INTERRUPT_0.
**             intEnableMask  (in)  Peripheral interrupt mask register.
**             intState       (in)  Peripheral interrupt status register.
**             extIntConfig   (in)  External interrupt configuration register.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that interrupts (IE bit of status register) are
**             initially disabled prior to calling this function. They will
**             be enabled as part of this function.
*****************************************************************************
*/
void intCtrlInitMips
(
   INT_CTRL_MIPS_INTERRUPT_ID    periphIntId,
   volatile XDRV_UINT32         *intEnableMask,
   volatile XDRV_UINT32         *intStatus,
   volatile XDRV_UINT32         *extIntConfig
);


/*
*****************************************************************************
** FUNCTION:   intCtrlDeinitMips
**
** PURPOSE:    Deinitialize MIPS interrupt control driver.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void intCtrlDeinitMips( void );

void xdrvIntCtrlSoftwareInterrupt( int number );

#ifdef __cplusplus
    }
#endif

#endif  /* INT_CTRL_MIPS_H  */
