/**********************************************************************************
** Copyright © 2008 Broadcom Corporation
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file implements the Utility main.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <utilApi.h>
#include <utilMem.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/* ---- "C" API (START OF) ------------------------------------------------------ */
/***********************************************************************************
**  FUNCTION:   utilInit
**
**  PURPOSE:    External API to initialize (and launch) the Utility module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      This API must be invoked prior to any further Utility work
**              is possible.
***********************************************************************************/
UTILSTATUS utilInit( void )
{
   CCTK_TRACED0( "util - utilInit()" );

   UTILSTATUS eStatus = eUTILSTATUS_SUCCESS;

#if CCTKCFG_MEMORY_MONITORING
   /* Initialize Memory Management module */
   eStatus = utilMemInit();
   if ( eStatus != eUTILSTATUS_SUCCESS )
   {
      CCTK_TRACEI0( "util - utilInit: Error initializing Memory Management." );
   }
#endif

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   utilDeInit
**
**  PURPOSE:    External API to de-initialize the Utility module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      After this API has been invoked, there is no further processing
**              possible from the Utility module.
***********************************************************************************/
UTILSTATUS utilDeInit( void )
{
   CCTK_TRACED0( "util - utilDeInit()" );

   UTILSTATUS eStatus = eUTILSTATUS_SUCCESS;

#if CCTKCFG_MEMORY_MONITORING
   /* De-initialize Memory Management module */
   eStatus = utilMemDeInit();
   if ( eStatus != eUTILSTATUS_SUCCESS )
   {
      CCTK_TRACEI0( "util - utilDeInit: Error de-initializing Memory Management." );
   }
#endif

   return eStatus;
}

/* ---- "C" API (END OF) -------------------------------------------------------- */
