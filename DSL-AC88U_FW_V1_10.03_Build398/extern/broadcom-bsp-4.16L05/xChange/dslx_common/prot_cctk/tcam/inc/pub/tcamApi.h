/**********************************************************************************
** Copyright © 2009 Broadcom
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
**      This file contains the Telephony Client Application Manager API.
**
***********************************************************************************/

#ifndef __TCAM_API__H__INCLUDED__
#define __TCAM_API__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <tcamCfg.h>
#include <cmgrApi.h>


/* ---- Constants and Types ----------------------------------------------------- */

/* Associate the TCAM invalid handle to be the CMGR invalid handle.
** This is extremely useful for the TCAM <-> CMGR API functionality.
**
** WARNING: Changing this association may render the system useless and will
**          be root cause for unexplained errors.
*/
#define TCAM_INVALID_HANDLE                        CMGR_INVALID_HANDLE

#define TCAM_IP_ADDRESS_STRING_LEN                 BOS_IP_ADDRESS_MAX_STR_LEN_V6
#define TCAM_MAX_CALL_RESOURCES                    CMGRCFG_MEDSES_MAX
#define TCAM_USR_IDENTIFIER_LEN                    CMGR_USER_IDENTIFIER_LEN
#define TCAM_PCSCF_IDENTIFIER_LEN                  127


/* ---- Function Prototypes ----------------------------------------------------- */

#if defined(__cplusplus)
extern "C"
{
#endif

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
void tcamInit ( void );


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
void tcamDeinit ( void );


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
void tcamCmd ( BOS_UINT8 *pCmdStr );


/***********************************************************************************
** FUNCTION:   tcamGetAppIdent
**
** PURPOSE:    Retrieve an application identifier from TCAM module.
**
** PARAMETERS: uIdent - the internal identifier used by the local application
**                      to be mapped to the outside world identifier.
**
** RETURNS:    The application identifier or NULL if none can be mapped.
**
** NOTE:       TCAM is the placeholder for the identifier mapping.
**
***********************************************************************************/
BOS_UINT8 *tcamGetAppIdent ( BOS_UINT32 uIdent );


#if defined(__cplusplus)
}
#endif

#endif /* __TCAM_API__H__INCLUDED__ */

