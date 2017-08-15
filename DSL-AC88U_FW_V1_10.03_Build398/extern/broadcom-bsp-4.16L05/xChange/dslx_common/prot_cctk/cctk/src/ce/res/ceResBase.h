/**********************************************************************************
** Copyright © 2007 - 2009 Broadcom
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
**      This file defines the base class of the resource object that is managed
**      by the Call Engine.
**
***********************************************************************************/
#ifndef __CE_RES_BASE__H__INCLUDED__
#define __CE_RES_BASE__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceApi.h>
#include <ceFsmBase.h>


/* ---- Constants and Types ----------------------------------------------------- */
static const BOS_UINT32 guCeResMaxForkedCall = 4;

#define CERESBASE_BOOL_VALUE( a )      (a) ? 'Y' : 'N'

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

/*
** This class defines the base class for all different types of resource class.
*/
class CeResBase
{
   public:

      friend class CeFsmBase;

      /*
      ** This defines the different types of resource managed by the Call Engine.
      */
      enum CERESTYPE
      {
         eCERESTYPE_CALL,            /* Call.          */
         eCERESTYPE_SUB,             /* Subscription.  */
         eCERESTYPE_NOT,             /* Notification.  */
         eCERESTYPE_USR,             /* User.          */
         eCERESTYPE_MAX,             /* Counter.       */
      };

      CeResBase( CERESTYPE  eType     = eCERESTYPE_MAX );

      virtual ~CeResBase( void );

      virtual BOS_UINT32             resourceGuid( void ) const;

      virtual CERESTYPE              resourceGender( void ) const;

      virtual BOS_BOOL               resourceCreate( BOS_UINT32 uCfgGuid );
      virtual BOS_BOOL               resourceTerminate( void );

      virtual BOS_UINT32             resourceParent( void ) const;
      virtual void                   resourceAllocated( BOS_UINT32 uGuid,
                                                        BOS_UINT32 uParent );

      virtual CeFsmBase::CEFSMTYPE   resourceFsm( void ) const;
      virtual void resourcePushFsm( CeFsmBase::CEFSMTYPE eNewFsm );

      virtual CeFsmBase::CEFSMSTATE  resourceState( void ) const;
      virtual void resourcePushState( CeFsmBase::CEFSMSTATE eNewState );

      virtual CEEVT                  resourceEvent( void ) const;
      virtual void                   resourcePushEvent( CEEVT eNewEvent );

      virtual void                   resourcePushData( CEEVT eEvt,
                                                       void *pData );

      virtual void                   resourceInit( void );

      virtual void                   resourceLiberate( void );

      virtual BOS_BOOL               resourceBasicCtlEqual( 
                                        IN MXD_GNS::ISceBasicExtensionControl
                                                                    *pBasicExtCtl );

      virtual void                   resourceShow( void );

      void *                         resourceAddress( void );

      void                           resourceSetAddress( void *pAddr );

   private:

      CERESTYPE                meResourceGender;
      BOS_UINT32               muParentGuid;
      BOS_UINT32               muGuid;
      void *                   mpResource;

      CeFsmBase::CEFSMTYPE     meResourceFsm[CCTKCFG_RES_FSM_DEPTH];
      CeFsmBase::CEFSMSTATE    meResourceState[CCTKCFG_RES_STATE_DEPTH];
      CEEVT                    meResourceEvent[CCTKCFG_RES_STATE_DEPTH]
                                                   [CCTKCFG_RES_EVENT_DEPTH];
};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeResBase::~CeResBase
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
inline CeResBase::~CeResBase( void )
{
   /* Nothing to be done.
   */
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceCreate
**
**  PURPOSE:    Invoked when creating a resource.  The derived class should take
**              care of this as it is used to create the corresponding network
**              resource when invoking the creation of such resource.
**
**  PARAMETERS:
**              uCfgGuid - the GUID to locate the configuration associated with
**                         this resource.
**
**
**  RETURNS:    BOS_FALSE - Always because the derived class should implement it.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResBase::resourceCreate( BOS_UINT32 uCfgGuid )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uCfgGuid );

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceTerminate
**
**  PURPOSE:    Invoked when deleting a resource.  The derived class should take
**              care of this as it is used to delete the corresponding network
**              resource when invoking the deletion of such resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    BOS_FALSE - Always because the derived class should implement it.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResBase::resourceTerminate( void )
{
   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceGuuid
**
**  PURPOSE:    Get the resource 'globally unique identifier' associated with this
**              resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The GUID.
**
**  NOTES:
***********************************************************************************/
inline BOS_UINT32 CeResBase::resourceGuid( void ) const
{
   return muGuid;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceGender
**
**  PURPOSE:    Get the resource gender associated with this resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The GUID.
**
**  NOTES:
***********************************************************************************/
inline CeResBase::CERESTYPE CeResBase::resourceGender( void ) const
{
   return meResourceGender;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceParent
**
**  PURPOSE:    Get the parent 'globally unique identifier' associated with this
**              resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The parent GUID.
**
**  NOTES:
***********************************************************************************/
inline BOS_UINT32 CeResBase::resourceParent( void ) const
{
   return muParentGuid;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceAllocated
**
**  PURPOSE:    Set the parent 'globally unique identifier' associated with this
**              resource.  Setting a parent is equivalent to allocating the resource
**              for a given purpose.  Resource with no parent are free resources.
**
**  PARAMETERS:
**              uGuid   - the GUID for this resource.
**              uParent - the GUID of this resource's parent.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResBase::resourceAllocated( BOS_UINT32 uGuid, BOS_UINT32 uParent )
{
   muGuid = uGuid;
   muParentGuid = uParent;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceLiberate
**
**  PURPOSE:    Resets the GUIDs associated with the resource, this essentially
**              pushes the resource back into the free pool.  This must be used
**              with caution to avoid leakage.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResBase::resourceLiberate( void )
{
   /* Detach the resource from the backup and recovery module, this is done
   ** in order to free up the backup and recovery resource block that was
   ** associated with this resource (if any).
   */
   nsBrDetach( muGuid );

   muGuid = guCeInvalidGuid;
   muParentGuid = guCeInvalidGuid;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceInit
**
**  PURPOSE:    Initializes the information about the resource FSM(s), state(s) and
**              event(s) kept within the resource object.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    FSM type (see enum CEFSMTYPE)
**
**  NOTES:
***********************************************************************************/
inline void CeResBase::resourceInit( void )
{
   BOS_UINT32 uIx, uJx;

   for ( uIx = 0 ; uIx < CCTKCFG_RES_FSM_DEPTH ; uIx++ )
   {
      meResourceFsm[uIx] = CeFsmBase::eCEFSMTYPE_MAX;
   }
   for ( uIx = 0 ; uIx < CCTKCFG_RES_STATE_DEPTH ; uIx++ )
   {
      meResourceState[uIx] = CeFsmBase::eCEFSMSTATE_IDLE;
      for ( uJx = 0 ; uJx < CCTKCFG_RES_EVENT_DEPTH ; uJx++ )
      {
         meResourceEvent[uIx][uJx] = eCEEVT_INV;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceFsm
**
**  PURPOSE:    Retrieves the current resource FSM type associated with the resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    FSM type (see enum CEFSMTYPE)
**
**  NOTES:
***********************************************************************************/
inline CeFsmBase::CEFSMTYPE CeResBase::resourceFsm( void ) const
{
   return meResourceFsm[0];
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourcePushFsm
**
**  PURPOSE:    Setups a new FSM for this resource.
**
**  PARAMETERS:
**              newFsm - the new FSM type to be associated with the resource.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline void CeResBase::resourcePushFsm( CeFsmBase::CEFSMTYPE eNewFsm )
{
   BOS_UINT32 uIx;

   /* Push the information down the stack by one.
   */
   for ( uIx = CCTKCFG_RES_FSM_DEPTH - 1 ; uIx >= 1 ; uIx-- )
   {
      meResourceFsm[uIx] = meResourceFsm[uIx - 1];
   }

   /* Set the new FSM information on top.
   */
   meResourceFsm[0] = eNewFsm;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceState
**
**  PURPOSE:    Retrieves the current resource state associated with the resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Current state information (see enum CEFSMSTATE)
**
**  NOTES:
***********************************************************************************/
inline CeFsmBase::CEFSMSTATE CeResBase::resourceState( void ) const
{
   return meResourceState[0];
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourcePushState
**
**  PURPOSE:    Associates a new state to this resource.
**
**  PARAMETERS:
**              newState - the new state to be associated with the resource.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline void CeResBase::resourcePushState( CeFsmBase::CEFSMSTATE eNewState )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;

   /* Push the information down the stack by one.  Push both the state
   ** and the event information (since event information is kept per state
   ** for better accuracy).
   */
   for ( uIx = CCTKCFG_RES_STATE_DEPTH - 1 ; uIx >= 1 ; uIx-- )
   {
      meResourceState[uIx] = meResourceState[uIx - 1];
      for ( uJx = 0 ; uJx < CCTKCFG_RES_EVENT_DEPTH ; uJx++ )
      {
         meResourceEvent[uIx][uJx] = meResourceEvent[uIx - 1][uJx];
      }
   }

   /* Set the new state information on top.
   */
   meResourceState[0] = eNewState;
   for ( uJx = 0 ; uJx < CCTKCFG_RES_EVENT_DEPTH ; uJx++ )
   {
      meResourceEvent[0][uJx] = eCEEVT_INV;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourceEvent
**
**  PURPOSE:    Retrieves the current resource event associated with the resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Current event information (see enum CEEVT)
**
**  NOTES:
***********************************************************************************/
inline CEEVT CeResBase::resourceEvent( void ) const
{
   return meResourceEvent[0][0];
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourcePushEvent
**
**  PURPOSE:    Associates a new event to this resource.
**
**  PARAMETERS:
**              newEvent - the new event to be associated with the resource.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline void CeResBase::resourcePushEvent( CEEVT eNewEvent )
{
   BOS_UINT32 uIx;

   /* Push the information down the stack by one.
   */
   for ( uIx = CCTKCFG_RES_EVENT_DEPTH - 1 ; uIx >= 1 ; uIx-- )
   {
      meResourceEvent[0][uIx] = meResourceEvent[0][uIx - 1];
   }

   /* Set the new event information on top.
   */
   meResourceEvent[0][0] = eNewEvent;
}


/***********************************************************************************
**  FUNCTION:   CeResBase::resourcePushData
**
**  PURPOSE:    Pushes some information to the resource, information which is to
**              be used in order to properly apply the intended behavior.
**
**  PARAMETERS:
**              eEvt - the event associated with the data.
**              pData - the data of interest.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      The data is associated with a specific event.  This function is
**              to be overwritten by the resource class since events are per
**              resource and each resource may require something specific.
***********************************************************************************/
inline void CeResBase::resourcePushData( CEEVT eEvt, void *pData )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( eEvt );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );
}

#endif /* __CE_RES_BASE__H__INCLUDED__ */
