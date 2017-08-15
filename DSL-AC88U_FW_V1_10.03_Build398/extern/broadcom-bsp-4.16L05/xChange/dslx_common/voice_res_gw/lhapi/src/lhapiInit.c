/****************************************************************************
*  
*  Copyright (c) 2000-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
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
*    Filename: lhapiInit.c
*
****************************************************************************
*    Description:
*
*     Implements the "Low-level HAPI" API that is required to interface
*     to LDX. Conceptually, LDX has multiple interface "levels". At the top
*     level, clients interface to HAPI - hausware API. This interface is used
*     to open/close/control LDX (voice) channels. In addition, in a distributed
*     LDX architecture, there is a "low-level" interface that allow for buffers
*     to be passed between the distributed processors. LDX provides a generic
*     API that must be implemented by clients (since LDX has no understanding
*     of how buffers are exchanged between processors).
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgLog.h>
#include <vrgTypes.h>
#include "lhapiExtension.h"
#include "lhapiPrivate.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/*
** Boolean indicating whether hausware runs a distributed configuration
** across host and DSP. Or whether it only runs on the host processor.
*/
static VRG_BOOL gIsHauswareDistributed;


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Private Constants ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   lhapiInit
**
** PURPOSE:    Initialize the low-level HAPI module.
**
** PARAMETERS: isHauswareDistributed (in) Boolean indicating whether hausware
**                                        runs a distributed configuration
**                                        across host and DSP. Or whether it
**                                        only runs on the host processor.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void lhapiInit( VRG_BOOL isHauswareDistributed )
{
   gIsHauswareDistributed = isHauswareDistributed;

   lhapiCritSecInit();

   lhapiThreadDispatchInit();
}


/*
*****************************************************************************
** FUNCTION:   lhapiDeinit
**
** PURPOSE:    De-init the low-level module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiDeinit( void )
{
   lhapiThreadDispatchDeinit();

   lhapiCritSecDeinit();

#ifdef BRCM_NONDIST_BUILD
   lhapiOsSemDestroy();
#endif


#ifdef FEAT_HAL_CAPTURE_TO_FILE 
   /* Destroy Capture Thread */
   lhapiHalCaptureThreadDeInit();
   
   /* Deinit capture module */
   lhapiHalCaptureDeInit();
#endif /* FEAT_HAL_CAPTURE_TO_FILE  */
   
}


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
VRG_BOOL lhapiIsHauswareDistributed( void )
{
   return ( gIsHauswareDistributed );
}
