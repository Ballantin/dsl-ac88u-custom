/**********************************************************************************
** Copyright © 2007 - 2010 Broadcom
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
**      This file defines the subscriber finite state machine class.
**
***********************************************************************************/
#ifndef __CE_FSM_SUB__H__INCLUDED__
#define __CE_FSM_SUB__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceResBase.h>
#include <ceFsmBase.h>


/* ---- Constants and Types ----------------------------------------------------- */


/* ---- Variable Externs -------------------------------------------------------- */


/* ---- Class Definition -------------------------------------------------------- */

class CeFsmSub : virtual public CeFsmBase
{
   public:

      CeFsmSub( void );

      void               fsmRun( CeResBase &rRes );

      static void        fsmSubFail( CeResBase &rRes );
      static void        fsmSubEnd( CeResBase &rRes );
      static void        fsmSubTry( CeResBase &rRes );
      static void        fsmSub( CeResBase &rRes );
      static void        fsmSubAttempt( CeResBase &rRes );
      static void        fsmSubTerm( CeResBase &rRes );
      
      static void        fsmSubAccept( CeResBase &rRes );
      static void        fsmSubReject( CeResBase &rRes );
      static void        fsmSubEnded( CeResBase &rRes );
      static void        fsmSubAdvert( CeResBase &rRes );
      static void        fsmSubUnsub( CeResBase &rRes );
      static void        fsmSubPending( CeResBase &rRes );

      static void        fsmUnsolNtfy( CeResBase &rRes );
      static void        fsmSolNtfy( CeResBase &rRes );
      static void        fsmRejNtfy( CeResBase &rRes );

   private:

};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmSub::CeFsmSub
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
inline CeFsmSub::CeFsmSub( void ) :
   CeFsmBase( CeFsmBase::eCEFSMTYPE_SUB )
{

}

#endif /* __CE_FSM_SUB__H__INCLUDED__ */
