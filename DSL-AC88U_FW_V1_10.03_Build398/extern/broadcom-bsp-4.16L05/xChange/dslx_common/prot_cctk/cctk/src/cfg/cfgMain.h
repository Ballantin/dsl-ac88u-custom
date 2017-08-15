/**********************************************************************************
** Copyright © 2007-2008 Broadcom
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
**      This file defines the Configuration Framework main.
**
***********************************************************************************/

#ifndef __CFG_MAIN__H__INCLUDED__
#define __CFG_MAIN__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cfgApi.h>
#include <cfgEngine.h>

/* ---- Constants and Types ----------------------------------------------------- */
#define CFM_CFG                        ( gCfmCtrlBlk.cfg )

/* User configuration */
typedef struct CFMUSR
{
   BOS_UINT32  uGuid;                  /* GUID associated to the user */
   CFGUSR      cfg;                    /* User specific configuration */
} CFMUSR;

/* Call configuration */
typedef struct CFMCALL
{
   BOS_UINT32  uGuid;                  /* GUID associated to the call */
   CFGCALL     cfg;                    /* Call configuration */
} CFMCALL;

/* Event subscription configuration */
typedef struct CFMEVT
{
   BOS_UINT32  uGuid;                  /* GUID associated to event subscription */
   CFGEVT      cfg;                    /* Event subscription configuration */
} CFMEVT;

/* Configuration block */
typedef struct CFMCFGBLK
{
   CFGSYS      *pSys;                  /* System configuration */
   CFMUSR      *pUsr[CFG_MAX_USR];     /* User configuration */
   CFMCALL     *pCall[CCTKCFG_MAX_CALL];
                                       /* Call configuration */
   CFMEVT      *pEvt[CCTKCFG_MAX_EVT]; /* Event subscription configuration */
} CFMCFGBLK;

/* Global config main control block */
typedef struct CFMCTRLBLK
{
   /* The following:
   ** 1) Can be "accessed" by both the application thread and the stack's core
   **    thread, and
   ** 2) Can ONLY be "modified" within the application task context, and
   ** 3) Can ONLY be "modified" when the stack's core thread no longer exists
   **    (i.e. no mutex protection needed).
   */
   MX_NS CfgEngine
               *pEngine;               /* Config engine handle (This must be the
                                       ** first field of CFMCTRLBLK).
                                       */

   /* The following can ONLY be "accessed"/"modified" by the stack's core thread
   ** (i.e. no mutex protected is needed).
   */
   CFMCFGBLK   cfg;                    /* Configuration block */ 
} CFMCTRLBLK;

/* ---- Variable Externs -------------------------------------------------------- */
/* Default system extended configuration */
extern CCTKEXTCFGMAP gCctkDftExtSysCfg[];

/* Default user extended configuration */
extern CCTKEXTCFGMAP gCctkDftExtUsrCfg[];

/* Config main control block */
extern CFMCTRLBLK gCfmCtrlBlk;

/* ---- Function Prototypes ----------------------------------------------------- */
/* Internal configuration manipulation routines */
void cfmClrUsrCfg( BOS_UINT32 uUsrIdx );

#endif /* __CFG_MAIN__H__INCLUDED__ */
