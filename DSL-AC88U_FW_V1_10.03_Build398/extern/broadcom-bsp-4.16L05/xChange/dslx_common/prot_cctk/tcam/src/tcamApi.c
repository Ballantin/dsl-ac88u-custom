/**********************************************************************************
** Copyright © 2009 Broadcom Corporation
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
**      This file implements the Telephony Client Application Manager API.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <tcamInc.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   tcamInit
**
** PURPOSE:    Init the Telephony Client Application Manager stack.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       When using TCAM, this will also start up the entire CMGR/CCTK
**             underlying application.
**
***********************************************************************************/
void tcamInit ( void )
{
   /* Launch the TCAM.
   */
   tcamMainInit ();
}

/***********************************************************************************
** FUNCTION:   tcamDeinit
**
** PURPOSE:    Shuts down the Telephony Client Application Manager stack.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       When using TCAM, this will terminate the entire CMGR/CCTK
**             underlying application.
**
***********************************************************************************/
void tcamDeinit ( void )
{
   /* Terminate the TCAM.
   */
   tcamMainDeinit ();
}


/***********************************************************************************
** FUNCTION:   tcamCmd
**
** PURPOSE:    Pass a command to the TCAM module.
**
** PARAMETERS: pCmdStr - the command string.  The command accepted by the TCAM
**                       module can be found in the 'tcamCom' module parser.
**
** RETURNS:    None.
**
** NOTE:       A command that is not parsed properly will be ignored, there is
**             no status associated with this action in such case.
**
***********************************************************************************/
void tcamCmd ( BOS_UINT8 *pCmdStr )
{
   if ( pCmdStr )
   {
      /* Invoke the 'communication' module with the command.  The 'com' module
      ** will parse the command and transfer it to the rest of the application
      ** as necessary.
      */
      tcamCom ( pCmdStr );
   }
}


