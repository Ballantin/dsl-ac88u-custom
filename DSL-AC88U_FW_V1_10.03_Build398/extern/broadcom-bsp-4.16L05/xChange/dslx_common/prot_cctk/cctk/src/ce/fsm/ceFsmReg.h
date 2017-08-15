/**********************************************************************************
** Copyright (c) 2007-2012 Broadcom
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
**      This file defines the registrar finite state machine class.
**
***********************************************************************************/
#ifndef __CE_FSM_REG__H__INCLUDED__
#define __CE_FSM_REG__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceResBase.h>
#include <ceFsmBase.h>


/* ---- Constants and Types ----------------------------------------------------- */


/* ---- Variable Externs -------------------------------------------------------- */


/* ---- Class Definition -------------------------------------------------------- */

class CeFsmReg : virtual public CeFsmBase
{
   public:

      CeFsmReg( void );

      void               fsmRun( CeResBase &rRes );

      static void        fsmReg( CeResBase &rRes );
      static void        fsmTrying( CeResBase &rRes );
      static void        fsmUnReg( CeResBase &rRes );
      static void        fsmReged( CeResBase &rRes );
      static void        fsmRereged( CeResBase &rRes );
      static void        fsmRegEnd( CeResBase &rRes );
      static void        fsmRegFail( CeResBase &rRes );
      static void        fsmRegTimeOut( CeResBase &rRes );
      static void        fsmNoSvc( CeResBase &rRes );

      static void        fsmAuthRej( CeResBase &rRes );
      static void        fsmAuthAck( CeResBase &rRes );
      static void        fsmAuthSrvFail( CeResBase &rRes );
      static void        fsmAuthNextNonce( CeResBase &rRes );

      static void        fsmOptions( CeResBase &rRes );

      static void        fsmKaFail( CeResBase &rRes );
      static void        fsmKaSuccess( CeResBase &rRes );
      static void        fsmKaRecovery( CeResBase &rRes );

      static void        fsmPublish( CeResBase &rRes );
      static void        fsmMsg( CeResBase &rRes );

      static void        fsmRefEnded( CeResBase &rRes );
      static void        fsmRefRej( CeResBase &rRes );
      static void        fsmRefer( CeResBase &rRes );

      static void        fsmPauId( CeResBase &rRes );

      static void        fsmUsrAdded( CeResBase &rRes );

      static void        fsmSvcEna( CeResBase &rRes );
      static void        fsmSvcDis( CeResBase &rRes );
      static void        fsmTargetFail( CeResBase &rRes );

      static void        fsmRemUsrAdd( CeResBase &rRes );
      static void        fsmRemUsrUpdate( CeResBase &rRes );
      static void        fsmRemUsrRemove( CeResBase &rRes );

      static void        fsmRegInbound ( CeResBase &rRes );

   private:

};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmReg::CeFsmReg
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline CeFsmReg::CeFsmReg( void ) :
   CeFsmBase( CeFsmBase::eCEFSMTYPE_REG )
{

}

#endif /* __CE_FSM_REG__H__INCLUDED__ */
