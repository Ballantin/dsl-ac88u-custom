/****************************************************************************
*
* Copyright © 2000-2009 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
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
*    Filename: hdspInit.c
*
****************************************************************************
*    Description:
*
*     This file implements the HDSP module initialization.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>
#include <hapi.h>
#include <hdspCfg.h>
#include <hdspInit.h>
#include <hdspTone.h>
#include <hdspCmd.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:      hdspInit
**
** PURPOSE:       Initialization of the HDSP module. This includes
**                initialization of HHAPI.
**
** PARAMETERS:    eventCallback - event callback for DSP's resource manager
**                dspNum - the DSP number to initialize
**
** RETURNS:       HAPISUCCESS or HAPIFAILURE
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspInit
(
   HAPICALLBKP    eventCallback,
   int dspNum
)
{
   /*
   ** Get the initialization status
   */
   if( hapiInit( eventCallback, dspNum ) )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ENDPT: ERROR hapiInit FAILED" ));
      return ( HAPIFAILURE );
   }

#if HDSP_ERR_RECOVERY_MECHANISM
   /*
   ** Initialize the HDSP error module to track DSP errors
   */
   hdspErrorInit( eventCallback );
#endif

   return ( HAPISUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   hdspDeinit
**
** PURPOSE:    HDSP module deinit - call once during system shutdown.
**             This will shutdown the HDSP inteface (including HHAPI) and
**             free all resources used by the HDSP module. (i.e. this function
**             should free all resources allocated in hdspInit()).
**
**
** PARAMETERS: none
**
** RETURNS:    HAPISUCCESS or HAPIFAILURE
**             This function should only return an error under catastrophic
**             circumstances. i.e. Something that cannot be fixed by re-invoking
**             the module initialization function.
**
** NOTE:       It is assumed that this function is only called after the HHAPI
**             task has been notified of a pending application reset, and it
**             has acknowledged the notification. This implies that the HHAPI
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization functions.
**
*****************************************************************************
*/
VRG_SINT16 hdspDeinit( void )
{
#if HDSP_ERR_RECOVERY_MECHANISM
   hdspErrorDeinit();
#endif

   return ( HAPISUCCESS );
}
