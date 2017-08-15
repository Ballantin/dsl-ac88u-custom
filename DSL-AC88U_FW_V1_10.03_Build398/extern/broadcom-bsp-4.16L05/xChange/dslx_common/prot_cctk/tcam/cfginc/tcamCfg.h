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
**      This file contains the TCAM compile-time configuration headers.
**
***********************************************************************************/

#ifndef __TCAM_CFG__H__INCLUDED__
#define __TCAM_CFG__H__INCLUDED__

/* ---- Features ---------------------------------------------------------------- */


/* ---- Constants --------------------------------------------------------------- */

/*
** -- Thread related information --
**
** The following configuration elements can be used to customize the TCAM async
** thread.
**
** Note that it is *NOT* recommended to change those values unless you are well
** aware of the consequences since mocking up with this information may render
** the system useless and non functional.
*/
#define TCAMCFG_THREAD_NAME                                   "TCAM"
#define TCAMCFG_THREAD_STACK                                  (16 * 1024)
#define TCAMCFG_THREAD_PRI                                    0
#define TCAMCFG_THREAD_SLEEP                                  150
#define TCAMCFG_THREAD_QDEPTH                                 20 /* Matches BOS. */
#define TCAMCFG_THREAD_QNAME                                  "TC-Q"
#define TCAMCFG_THREAD_SEMNAME                                "TC-S"


/* ---- Variable Externs -------------------------------------------------------- */

/* The callback override that an application MUST define (and implement) for the
** TCAM layer to work.  This callback is associated with the configuration
** interface.
**
** The prototype for the callback override function can be found in the interface
** module definition: 'tcamIfcCfg.h'.
*/
#define TCAM_IFC__CFG_CALLBACK__IMPLEMENT


/* The callback override that an application MUST define (and implement) for the
** TCAM layer to work.  This callback is associated with the endpoint control
** interface.
**
** The prototype for the callback override function can be found in the interface
** module definition: 'tcamIfcEpt.h'.
*/
#define TCAM_IFC__EPT_CALLBACK__IMPLEMENT


/* The callback override that an application MUST define (and implement) for the
** TCAM layer to work.  This callback is associated with the messaging
** interface.
**
** The prototype for the callback override function can be found in the interface
** module definition: 'tcamIfcMsg.h'.
*/
#define TCAM_IFC__MSG_CALLBACK__IMPLEMENT


/* A magic cookie that may be used to prepend to the signaling interface for the
** outside world.  Only define if necessary.  If defined, the TCAM layer will only
** process messages prepended with the magic cookie and ignore messages otherwise.
** If defined, the TCAM layer will also preprend any signaling messages it issues
** with this magic.
**
** When defined, the magic cookie is expected to be a single ASCII character.
** Example of such magic cookie: '+', '$', etc...
*/
#define TCAM_COM__MAGIC_SIGNALING
/* Undef it right now since by default it should not be defined.
*/
#undef TCAM_COM__MAGIC_SIGNALING


/* ---- Function Prototypes ----------------------------------------------------- */

/* ---- Include Files ----------------------------------------------------------- */
/*
** CUSTOM CONFIGURATION - This must be included after all default build
** configuration options. Custom include files should undef default config options
** and then re-define them.
*/
#include <tcamCfgCustom.h>

/* Configuration Verification */
#include <tcamCfgChk.h>

#endif /* __TCAM_CFG__H__INCLUDED__ */
