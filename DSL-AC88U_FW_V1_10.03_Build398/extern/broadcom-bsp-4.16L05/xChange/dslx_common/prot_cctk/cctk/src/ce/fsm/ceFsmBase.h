/**********************************************************************************
** Copyright (c) 2007-2011 Broadcom
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
**      This file defines the base class of the finite state machine object that
**      is used by the resources of the Call Engine.
**
***********************************************************************************/
#ifndef __CE_FSM_BASE__H__INCLUDED__
#define __CE_FSM_BASE__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Constants and Types ----------------------------------------------------- */
class CeResBase;

/* ---- Variable Externs -------------------------------------------------------- */

extern const BOS_UINT8 *gpcCeFsmName[];
extern const BOS_UINT8 *gpcCeFsmState[];
extern const BOS_UINT8 *gpcCeFsmEvent[];


/* ---- Class Definition -------------------------------------------------------- */

/*
** This class defines the base class for all different FSM..
*/
class CeFsmBase
{
   public:

      typedef void (*CEFSMVOIDFUNCTIONPTR)( CeResBase &rRes );

      /*
      ** This defines the different types of FSMs.
      */
      enum CEFSMTYPE
      {
         eCEFSMTYPE_IN,              /* Incoming call. */
         eCEFSMTYPE_OUT,             /* Outgoing call. */
         eCEFSMTYPE_CALL,            /* Active call. */
#if CCTKCFG_REMOTE_USER_CONTROL
         eCEFSMTYPE_PROXY_IN,        /* Incoming call from Remote user. */
         eCEFSMTYPE_PROXY_OUT,       /* Outgoing call to Remote user. */
#endif
         eCEFSMTYPE_HOLD,            /* Hold service. */
         eCEFSMTYPE_XFER,            /* Transfer service. */
         eCEFSMTYPE_PUB,             /* Publication service. */
         eCEFSMTYPE_SUB,             /* Subscription service. */
         eCEFSMTYPE_REG,             /* Registration service. */
         eCEFSMTYPE_MAX,             /* Counter - must be last. */
      };

      /*
      ** This defines the different states that can be managed by the FSMs.
      **
      ** Each FSM handles a specific set of states which are specific to its
      ** purpose.  The state set for a given FSM are enclosed within a XXX_SET_BEGIN
      ** and XXX_SET_FINISH markers and any new state to be handled by the specific
      ** FSM should be placed within those markers or it will be ignored.
      */
      enum CEFSMSTATE
      {
         eCEFSMSTATE_IDLE = 0,            /* Idle. */

         eCEFSMSTATE_REG_SET_BEGIN,       /* State marker REG FSM - start. */
         eCEFSMSTATE_REG_IDLE,
         eCEFSMSTATE_REG_REGING,
         eCEFSMSTATE_REG_REGED,
         eCEFSMSTATE_REG_SET_FINISH,      /* State marker REG FSM - end. */

         eCEFSMSTATE_CALLIN_SET_BEGIN,    /* State marker CALL-IN FSM - start. */
         eCEFSMSTATE_CALLIN_IDLE,
         eCEFSMSTATE_CALLIN_ALERT,
         eCEFSMSTATE_CALLIN_SET_FINISH,   /* State marker CALL-IN FSM - end. */

         eCEFSMSTATE_CALLOUT_SET_BEGIN,   /* State marker CALL-OUT FSM - start. */
         eCEFSMSTATE_CALLOUT_IDLE,
         eCEFSMSTATE_CALLOUT_ATTEMPT,
         eCEFSMSTATE_CALLOUT_ALERT,
         eCEFSMSTATE_CALLOUT_SET_FINISH,  /* State marker CALL-OUT FSM - end. */

         eCEFSMSTATE_CALL_SET_BEGIN,      /* State marker CALL FSM - start. */
         eCEFSMSTATE_CALL_IDLE,
         eCEFSMSTATE_CALL_CONNECT,
         eCEFSMSTATE_CALL_CLEAN,
         eCEFSMSTATE_CALL_SET_FINISH,     /* State marker CALL FSM - end. */

         eCEFSMSTATE_XFER_SET_BEGIN,      /* State marker XFER FSM - start. */
         eCEFSMSTATE_XFER_IDLE,
         eCEFSMSTATE_XFER_ACCEPT,
         eCEFSMSTATE_XFER_SET_FINISH,     /* State marker XFER FSM - end. */

         eCEFSMSTATE_HOLD_SET_BEGIN,      /* State marker HOLD FSM - start. */
         eCEFSMSTATE_HOLD_IDLE,
         eCEFSMSTATE_HOLD_PENDING,
         eCEFSMSTATE_HOLD_ACCEPT,
         eCEFSMSTATE_HOLD_SET_FINISH,     /* State marker HOLD FSM - end. */

         eCEFSMSTATE_SUB_SET_BEGIN,       /* State marker SUB FSM - start. */
         eCEFSMSTATE_SUB_IDLE,
         eCEFSMSTATE_SUB_SUBING,
         eCEFSMSTATE_SUB_UNSUBING,
         eCEFSMSTATE_SUB_SUBED,
         eCEFSMSTATE_SUB_SET_FINISH,      /* State marker SUB FSM - end. */

         eCEFSMSTATE_PUB_SET_BEGIN,       /* State marker PUB FSM - start. */
         eCEFSMSTATE_PUB_IDLE,
         eCEFSMSTATE_PUB_PUBING,
         eCEFSMSTATE_PUB_SET_FINISH,      /* State marker PUB FSM - end. */
      };

      CeFsmBase( CEFSMTYPE eType = eCEFSMTYPE_MAX );
      virtual ~CeFsmBase( void );


      virtual CEFSMTYPE              fsmType( void ) const;
      virtual void                   fsmRun( CeResBase &rRes ) = 0;

      static  void                   fsmNoOp( CeResBase &rRes );
      static  void                   fsmInvalid( CeResBase &rRes );
      static  void                   fsmError( CeResBase &rRes );

   private:

      CEFSMTYPE                meFsmType;
};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmBase::CeFsmBase
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              eType - the type of FSM to be created.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline CeFsmBase::CeFsmBase( CEFSMTYPE eType ) :
   meFsmType    ( eType )
{

}


/***********************************************************************************
**  FUNCTION:   CeFsmBase::~CeFsmBase
**
**  PURPOSE:    Class desctructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline CeFsmBase::~CeFsmBase( void )
{
   /* Nothing to be done.
   */
}


/***********************************************************************************
**  FUNCTION:   CeFsmBase::fsmType
**
**  PURPOSE:    Retrieves the FSM type.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    FSM type (see enum CEFSMTYPE)
**
**  NOTES:
***********************************************************************************/
inline CeFsmBase::CEFSMTYPE CeFsmBase::fsmType( void ) const
{
   return meFsmType;
}

#endif /* __CE_FSM_BASE__H__INCLUDED__ */
