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
*    Filename: lhapiPrivate.h
*
****************************************************************************
*    Description:
*
*     Private interface for lhapi module.
*
****************************************************************************/

#ifndef LHAPI_PRIVATE_H
#define LHAPI_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchInit.
**
** PURPOSE:    Init lhapi thread dispatch sub-module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchInit( void );


/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchDeinit.
**
** PURPOSE:    De-initialize lhapi thread dispatch sub-module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchDeinit( void );


/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchHostOnly
**
** PURPOSE:    This function is called by hausware when it has threads that
**             are ready to run. It is the application's responsibility to
**             dispatch the threads by calling KNL_RunReadyThreads(). Note that
**             lhapiKnlScheduleReadyThreads may be called in ISR context. The
**             application must decide whether to invoke KNL_RunReadyThreads()
**             in ISR context or task context. If it invokes it in ISR context,
**             interrupts MUST be enabled prior to calling KNL_RunReadyThreads().
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchHostOnly( void );


/*
*****************************************************************************
** FUNCTION:   lhapiIsHauswareDistributed
**
** PURPOSE:    Indicates is hausware is running a distributed architecture.
**
** PARAMETERS: None.
**
** RETURNS:    TRUE or FALSE.
**
** NOTE:
*****************************************************************************
*/
VRG_BOOL lhapiIsHauswareDistributed( void );


/*
*****************************************************************************
** FUNCTION:   lhapiCritSecInit
**
** PURPOSE:    Init lhapi critical section primitive.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiCritSecInit( void );


/*
*****************************************************************************
** FUNCTION:   lhapiCritSecDeinit
**
** PURPOSE:    De-initialize lhapi critical section primitive.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiCritSecDeinit( void );

/*
*****************************************************************************
** FUNCTION:   lhapiOsSemDestroy
**
** PURPOSE:    Destroy all the semaphores and reset the counter
**
** PARAMETERS: none
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void lhapiOsSemDestroy( void );


#ifdef __cplusplus
    }
#endif

#endif  /* LHAPI_PRIVATE_H  */
