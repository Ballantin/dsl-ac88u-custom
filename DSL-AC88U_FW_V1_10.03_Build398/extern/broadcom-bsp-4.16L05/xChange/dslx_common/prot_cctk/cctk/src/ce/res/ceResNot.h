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
**      This file defines the notifier class of the resource object that is managed
**      by the Call Engine.
**
***********************************************************************************/
#ifndef __CE_RES_NOT__H__INCLUDED__
#define __CE_RES_NOT__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceResBase.h>


/* ---- Constants and Types ----------------------------------------------------- */
#define CE_RES_NOT__VQ_RTCP_XR_SUBTYPE    "vq-rtcpxr"
#define CE_RES_NOT__XML_SMML_SUBTYPE      "xml-smml"
#define CE_RES_NOT__AUDIO_TYPE            "audio"
#define CE_RES_NOT__TEL_EVT_SUBTYPE       "telephone-event"

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

/*
** This class defines the user resource.
*/
class CeResNot: virtual public CeResBase
{
   public:

      CeResNot( void );
      ~CeResNot( void );

      BOS_BOOL                         resourceCreate( BOS_UINT32 uCfgGuid,
                                                       CCTKPUBLISH eType );
      inline CCTKPUBLISH               resourceType( void );
      BOS_BOOL                         resourceTerminate( void );

      BOS_BOOL                         resourceOodEqual(
                                          MXD_GNS::ISceOodRequestSender *pOod );

      MXD_GNS::ISceOodRequestSender *  resourceNetOodHdl( void );

      void                             resourcePushData( CEEVT eEvt,
                                                         void *pData );

      inline BOS_UINT8 *               resourceGetPeer( void )
                                                      { return mpPubParty; };
      BOS_BOOL                         resourceBasicCtlEqual( 
                                             IN MXD_GNS::ISceBasicExtensionControl
                                                                   *pBasicExtCtl );
      inline void                      resourceSetCall( BOS_UINT32 uCallGuid )
                                                       { muCallGuid = uCallGuid; };
      inline BOS_UINT32                resourceGetCall( void )
                                                      { return muCallGuid; };

      BOS_UINT32                       resourceMetrics( BOS_UINT8 *puData,
                                                        BOS_UINT32 uSize );

      inline BOS_UINT32                resourceGetFinalRsp( void )
                                                      { return mFinalRsp; };

      MXD_GNS::ISipServerEventControl *resourceNetEvtCtlHdl( void );
      void                             resourceNetEvtCtlHdl(
                                          MXD_GNS::ISipServerEventControl
                                                                  *pNetEvtCtlHdl );

      void                             resourceSetBody( BOS_UINT8 *pBody,
                                                        BOS_UINT32 uSize );
      inline BOS_UINT8 *               resourceGetBody( void )
                                                      { return mpBody; };
      inline BOS_UINT32                resourceGetSize( void )
                                                      { return muSize; };

      /* Base class virtual method overrides.
      */
      virtual void                     resourceShow( void );

   private:

      CCTKPUBLISH                      mePubType;

      BOS_UINT32                       muCallGuid;

      MXD_GNS::ISceOodRequestSender    *mpSceOodReqSnd;

      BOS_UINT8                        *mpPubParty;

      BOS_UINT32                       mFinalRsp;

      MXD_GNS::ISipServerEventControl  *mpSrvEvtCtl;

      BOS_UINT8                        *mpBody;
      BOS_UINT32                       muSize;
};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeResNot::CeResNot
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
inline CeResNot::CeResNot( void ) :
   CeResBase      ( CeResBase::eCERESTYPE_NOT ),
   mePubType      ( eCCTKPUBLISH_MAX ),
   muCallGuid     ( guCeInvalidGuid ),
   mpSceOodReqSnd ( NULL ),
   mpPubParty     ( NULL ),
   mFinalRsp      ( 0 ),
   mpSrvEvtCtl    ( NULL ),
   mpBody         ( NULL ),
   muSize         ( 0 )
{
   resourceSetAddress( (void *) this );
}


/***********************************************************************************
**  FUNCTION:   CeResNot::~CeResNot
**
**  PURPOSE:    Class destructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline CeResNot::~CeResNot( void )
{
   if ( resourceNetOodHdl() )
   {
      mpSceOodReqSnd->ReleaseIfRef();
      mpSceOodReqSnd = NULL;
   }

   if ( resourceNetEvtCtlHdl() )
   {
      mpSrvEvtCtl->ReleaseIfRef();
      mpSrvEvtCtl = NULL;
   }

   mePubType = eCCTKPUBLISH_MAX;
   muCallGuid = guCeInvalidGuid;
   muSize = 0;

   if ( mpPubParty )
   {
      BRCM_DELETE_ARRAY( mpPubParty );
      mpPubParty = NULL;
   }

   if ( mpBody )
   {
      BRCM_DELETE_ARRAY( mpBody );
      mpBody = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceTerminate
**
**  PURPOSE:    Terminates a resouce usage.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    BOS_TRUE (always...)
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResNot::resourceTerminate( void )
{
   if ( resourceNetOodHdl() )
   {
      mpSceOodReqSnd->ReleaseIfRef();
      mpSceOodReqSnd = NULL;
   }

   if ( resourceNetEvtCtlHdl() )
   {
      mpSrvEvtCtl->ReleaseIfRef();
      mpSrvEvtCtl = NULL;
   }

   if ( mpPubParty )
   {
      BRCM_DELETE_ARRAY( mpPubParty );
      mpPubParty = NULL;
   }

   if ( mpBody )
   {
      BRCM_DELETE_ARRAY( mpBody );
      mpBody = NULL;
   }

   resourceLiberate();

   return BOS_TRUE;
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceOodEqual
**
**  PURPOSE:    Checks whether this is the resource we are looking for based on
**              the UA handle (unique for a given resource).
**
**  PARAMETERS:
**              pOod   - the out-of-dialog handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResNot::resourceOodEqual( MXD_GNS::ISceOodRequestSender *pOod )
{
   return (( mpSceOodReqSnd == pOod ) ? BOS_TRUE : BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceNetOodHdl
**
**  PURPOSE:    Returns the handle to the associated network resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The handle of interest (warning: may be NULL).
**
**  NOTES:
***********************************************************************************/
inline MXD_GNS::ISceOodRequestSender *CeResNot::resourceNetOodHdl( void )
{
   return mpSceOodReqSnd;
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceNetEvtCtlHdl
**
**  PURPOSE:    Returns the handle to the associated network resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The handle of interest (warning: may be NULL).
**
**  NOTES:
***********************************************************************************/
inline MXD_GNS::ISipServerEventControl *CeResNot::resourceNetEvtCtlHdl( void )
{
   return mpSrvEvtCtl;
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceNetEvtCtlHdl
**
**  PURPOSE:    Assigns the handle to the server event control resource that we
**              need to keep track of.
**
**  PARAMETERS:
**              pNetEvtCtlHdl - the handle to keep track of.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResNot::resourceNetEvtCtlHdl( MXD_GNS::ISipServerEventControl
                                                                  *pNetEvtCtlHdl )
{
   /* Free up previous resource handle in case we kept track of it.
   */
   if ( mpSrvEvtCtl )
   {
      mpSrvEvtCtl->ReleaseIfRef();
      mpSrvEvtCtl = NULL;
   }

   mpSrvEvtCtl = pNetEvtCtlHdl;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceType
**
**  PURPOSE:    Gets the resource type associated with this notification.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The notification (publication) type.
**
**  NOTES:
***********************************************************************************/
inline CCTKPUBLISH CeResNot::resourceType( void )
{
   return mePubType;
}

#endif /* __CE_RES_NOT__H__INCLUDED__ */
 