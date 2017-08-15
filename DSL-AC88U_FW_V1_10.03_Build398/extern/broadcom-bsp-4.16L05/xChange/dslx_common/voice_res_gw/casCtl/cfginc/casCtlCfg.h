/***************************************************************************
Copyright © 2000-2007 Broadcom

This program  is the  proprietary software  of Broadcom  Corporation and/or  its
licensors, and may only be used, duplicated, modified or distributed pursuant to
the  terms and  conditions of  a separate,  written license  agreement executed
between you and Broadcom (an "Authorized  License").  Except as set forth in  an
Authorized License, Broadcom  grants no license  (express or implied),  right to
use, or waiver of any kind with respect to the Software, and Broadcom  expressly
reserves all rights in and to the Software and all intellectual property  rights
therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
USE OF THE SOFTWARE.  
 
Except as expressly set forth in the Authorized License,
 
1.      This  program,  including  its  structure,  sequence  and  organization,
constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
information only  in connection  with your  use of  Broadcom integrated  circuit
products.
 
2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
PERFORMANCE OF THE SOFTWARE.

3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
REMEDY.
****************************************************************************
*
*    Filename: casCtlCfg.h
*
****************************************************************************
*    Description:
*
*     CAS control default build options.  To override defaults in
*     casCtlCfg.h make a file called casCtlCfgCustom.h in your build
*     directory.
*
****************************************************************************/

#ifndef CAS_CTL_CFG_H
#define CAS_CTL_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <casCtlCfgCustom.h>


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/

/* Task priority for CAS */
#ifndef CAS_CTL_CFG_TASK_PRIORITY
#define CAS_CTL_CFG_TASK_PRIORITY                  BOS_TASK_CLASS_HIGH
#endif


/* The CAS task will run every 10ms */
#ifndef CAS_CTL_CFG_TASK_RATE_MS
#define CAS_CTL_CFG_TASK_RATE_MS                   10
#endif


/* CAS debounce period during ringing */
#ifndef CAS_CTL_CFG_RING_TRIP_REMOVE
#define CAS_CTL_CFG_RING_TRIP_REMOVE              100
#endif


/* Maximum number of supported endpoints */
#ifndef CAS_CTL_CFG_NUM_ENDPTS
#define CAS_CTL_CFG_NUM_ENDPTS                     1
#endif


/* 1 if Staggered ringing is supported. Cuts peak ringing power by 1/2 */
#ifndef CAS_CTL_CFG_STAGGER_RING_SUPPORT
#define CAS_CTL_CFG_STAGGER_RING_SUPPORT           1
#endif


/* 1 to dynamically setup the staggered ringing state machine based on ring
** cadence.  Staggered ringing needs to be enabled as well. */
#ifndef CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP
#define CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP     0
#endif


/* Verify data coherence. */
#if CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP
#   if !CAS_CTL_CFG_STAGGER_RING_SUPPORT
#      error "CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP cannot be set if CAS_CTL_CFG_STAGGER_RING_SUPPORT is not set"
#   endif
#endif

/* Adjustment to use for debouncing time on SLIC */
#ifndef CAS_CTL_DEFAULT_FXO_DEBOUNCE_MSEC
#define CAS_CTL_DEFAULT_FXO_DEBOUNCE_MSEC         0
#endif

#ifdef __cplusplus
    }
#endif

#endif  /* CAS_CTL_CFG_H  */


