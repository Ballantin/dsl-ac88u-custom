//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPTRANSFERSVC07_H
#define MXG_CSIPTRANSFERSVC07_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT to be \
able to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_ISIPNOTIFIERSVC_H
#include "SipUserAgent/ISipNotifierSvc.h"
#endif

#ifndef MXG_CNAMEADDR_H
#include "SipParser/CNameAddr.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPTRANSFERSVC07_H
#include "SipUserAgent/ISipTransferSvc07.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPREFEREEMGR_H
#include "SipUserAgent/ISipRefereeMgr.h"
#endif

#ifndef MXG_ISIPREFERRERMGR_H
#include "SipUserAgent/ISipReferrerMgr.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipTransferSvc07
//========================================
//
// Description:
//   This class is used to manage the use of REFER requests and responses to
//   attempt a transfer and the corresponding NOTIFY requests. The class can act
//   as a transferee and a transferor. It permits to automatically reply to
//   REFER and NOTIFY requests received at a wrong time. It also allows the
//   application to send REFER and NOTIFY requests and be warned of the
//   responses.
//
//  This class follows the rules of draft-ietf-sipping-cc-transfer-03. The class
//  numeration is incremental. Since the last corresponding version in v3.6 was
//  06, this one is called 07.
//
// Location:
//   SipUserAgent/CSipTransferSvc07.h
//
// See Also:
//   CSomethingElse, CSomething
//
//==EDOC========================================================================
class CSipTransferSvc07 : private CEComDelegatingUnknown,
                          private ISipTransferSvc07,
                          private ISipCoreSvc,
                          private CSipReqCtxCoreSvc,
                          private ISipRefereeMgr,
                          private ISipReferrerMgr
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipTransferSvc07();

    static void FinalizeCSipTransferSvc07();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipTransferSvc07(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipTransferSvc07();


    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);


    //-- << ISipTransferSvc07 >>
    //--------------------------------------------

    virtual mxt_result SetManager(IN ISipTransferMgr07* pMgr);

    virtual mxt_result Transfer(IN mxt_opaque opqTransaction,
                                IN const CNameAddr& rTarget,
                                IN TO CHeaderList* pExtraHeaders,
                                IN TO CSipMessageBody* pMessageBody,
                                OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result Transfer(IN mxt_opaque opqTransaction,
                                IN const ISipUserAgentSvc* pUaDialog,
                                IN TO CHeaderList* pExtraHeaders,
                                IN TO CSipMessageBody* pMessageBody,
                                OUT ISipClientTransaction*& rpTransaction,
                                IN bool bUseRemoteAddrInReferTo = true);

    virtual mxt_result ReportRefusal(IN mxt_opaque opqTransaction,
                                     IN ISipNotifierSvc::EReason eReason,
                                     IN unsigned int uRetryAfterSec,
                                     IN TO CHeaderList* pExtraHeaders,
                                     OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result ReportPending(IN mxt_opaque opqTransaction,
                                     IN unsigned int uExpirationInSec,
                                     IN TO CHeaderList* pExtraHeaders,
                                     OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result ReportFailure(IN mxt_opaque opqTransaction,
                                     IN TO CHeaderList* pExtraHeaders,
                                     OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result ReportProgress(
                                    IN mxt_opaque opqTransaction,
                                    IN unsigned int uExpirationInSec,
                                    IN const CSipStatusLine& rContentStatusLine,
                                    IN TO CHeaderList* pExtraHeaders,
                                    OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result ReportFinalStatus(
                                    IN mxt_opaque opqTransaction,
                                    IN const CSipStatusLine& rContentStatusLine,
                                    IN TO CHeaderList* pExtraHeaders,
                                    OUT ISipClientTransaction*& rpTransaction);

    virtual void ConfigureNotifyIdParameterUsage(IN ISipRefereeSvc::EIdParameterUsage eIdParamUsage);


    //-- << ISipReqCtxCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Notifies the request context core service of a received packet.
    //-----------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Modifies a packet about to be sent.
    //-------------------------------------
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Adds a reference on this request context core service.
    //--------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Removes a reference from this request context core service.
    //-----------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


    //-- << ISipCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Obtains an instance of an ISipReqCtxCoreSvc to attach to a request
    //  context.
    //--------------------------------------------------------------------
    virtual
    ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                IN EDirection eDirection);

    // Summary:
    //  Establishes the confidence level for which the service is a good
    //  candidate to own a request context.
    //-------------------------------------------------------------------
    virtual
    EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    // Summary:
    //  Returns the ISipReqCtxCoreSvc instance to use as the owner for a request
    //  context.
    //--------------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();


    //-- << ISipRefereeMgr >>
    //--------------------------------------------

    // Summary:
    //  A REFER request has been received.
    //--------------------------------
    virtual void EvReferred(IN ISipRefereeSvc* pSvc,
                            IN ISipServerEventControl* pServerEventControl,
                            IN mxt_opaque opqReferId,
                            IN const CNameAddr& rReferToAddr,
                            IN const CSipPacket& rRefer);
    // Summary:
    //  A SUBSCRIBE request to refresh an active subscription has been received.
    //-------------------------------------------------------------------------
    virtual void EvRefreshed(IN ISipRefereeSvc* pSvc,
                             IN ISipServerEventControl* pServerEventControl,
                             IN mxt_opaque opqReferId,
                             IN unsigned int uExpirationSec,
                             IN const CSipPacket& rSubscribe);

    // Summary:
    //  A SUBSCRIBE request to terminate an active/pending subscription has
    //  been received.
    //----------------------------------------------------------------------
    virtual void EvTerminated(IN ISipRefereeSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN mxt_opaque opqReferId,
                              IN const CSipPacket& rSubscribe);

    // Summary:
    //  A request has been automatically rejected by this service.
    //------------------------------------------------------------
    virtual void EvInvalidRequest(IN ISipRefereeSvc* pSvc,
                                  IN mxt_opaque opqApplicationData,
                                  IN const CSipPacket& rRequest,
                                  IN mxt_result reason);

    // Summary:
    //  A provisional response (1xx) has been received for a NOTIFY request sent
    //  by the ISipRefereeSvc.
    //-------------------------------------------------------------------------
    virtual void EvProgress(IN ISipRefereeSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN mxt_opaque opqReferId,
                            IN const CSipPacket& rResponse);

    // Summary:
    //  A success response (2xx) has been received for a NOTIFY request sent by
    //  the ISipRefereeSvc.
    //-------------------------------------------------------------------------
    virtual void EvSuccess(IN ISipRefereeSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN mxt_opaque opqReferId,
                           IN const CSipPacket& rResponse);

    // Summary:
    //  A failure (<= 3xx) has been received for a NOTIFY request sent by the
    //  ISipRefereeSvc.
    //-------------------------------------------------------------------
    virtual void EvFailure(IN ISipRefereeSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN mxt_opaque opqReferId,
                           IN const CSipPacket& rResponse);

    // Summary:
    //  The subscription is expired.
    //-------------------------------
    virtual void EvExpired(IN ISipRefereeSvc* pSvc,
                           IN mxt_opaque opqReferId);


    //-- << ISipReferrerMgr >>
    //--------------------------------------------

    // Summary:
    //  A provisional response (1xx) has been received for a REFER request sent
    //  by the ISipReferrerSvc.
    //------------------------------------------------------------------------
    virtual void EvReferProgress(IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse);

    // Summary:
    //  A success response (2xx) has been received for a REFER request sent by
    //  the ISipReferrerSvc.
    //------------------------------------------------------------------------
    virtual void EvReferSuccess(IN ISipReferrerSvc* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN mxt_opaque opqReferId,
                                IN const CSipPacket& rResponse);

    // Summary:
    //  A failure (<= 3xx) has been received for a REFER request sent by the
    //  ISipReferrerSvc.
    //------------------------------------------------------------------
    virtual void EvReferFailure(IN ISipReferrerSvc* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN mxt_opaque opqReferId,
                                IN const CSipPacket& rResponse);

    // Summary:
    //  A provisional response (1xx) has been received for a SUBSCRIBE request
    //  sent by the ISipReferrerSvc.
    //-------------------------------------------------------------------------
    virtual void EvSubscribeProgress(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse);

    // Summary:
    //  A success response (2xx) has been received for a SUBSCRIBE request sent
    //  by the ISipReferrerSvc.
    //------------------------------------------------------------------------
    virtual void EvSubscribeSuccess(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse);

    // Summary:
    //  An "Interval Too Small" (423) has been received for a SUBSCRIBE request
    //  sent by the ISipReferrerSvc.
    //--------------------------------------------------------------------------
    virtual void EvIntervalTooSmall(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN unsigned int uMinExpirationSec,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse);

    // Summary:
    //  A failure (<= 3xx) has been received for a SUBSCRIBE request sent by the
    //  ISipReferrerSvc.
    //----------------------------------------------------------------------
    virtual void EvSubscribeFailure(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse);

    // Summary:
    //  Notifies the application that a NOTIFY for a REFER sent has been
    //  received.
    //-------------------------------------------------------------------
    virtual void EvReferStatus(IN ISipReferrerSvc* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN mxt_opaque opqReferId,
                               IN EState eState,
                               IN const CSipStatusLine& rStatus,
                               IN const CHeaderList* pContentHeaders,
                               IN const CSipPacket& rNotify);

    // Summary:
    //  An invalid NOTIFY has been received.
    //--------------------------------------
    virtual void EvInvalidNotify(IN ISipReferrerSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rNotify,
                                 IN mxt_result reason);

    // Summary:
    //  The subscription is about to expire.
    //---------------------------------------
    virtual void EvExpiring(IN ISipReferrerSvc* pSvc,
                            IN mxt_opaque opqReferId);

    // Summary:
    //  The subscription is expired.
    //-------------------------------
    virtual void EvExpired(IN ISipReferrerSvc* pSvc,
                           IN mxt_opaque opqReferId);


    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  Indicates if a current transfer can be managed as a transferee.
    //-----------------------------------------------------------------
    bool IsTransfereeReadyForTransfer();

    // Summary:
    //  Tests if the URI type is supported by the transfer service 07.
    //-----------------------------------------------------------------
    bool IsSupportedUri(IN const CNameAddr& rAddrToTest);

    // Summary:
    //  Tests if the NOTIFY in parameter has a "terminated" value in its
    //  Subscription-State header.
    //-------------------------------------------------------------------
    bool IsFinalNotify(IN const CSipPacket& rPacket);

    // Summary:
    //  Sends a non-final NOTIFY request.
    //-----------------------------------
    mxt_result ReportProgressStatusHelper(
                                IN mxt_opaque opqTransaction,
                                IN ISipNotifierSvc::EState eSubscriptionState,
                                IN unsigned int uExpirationInSec,
                                IN const CSipStatusLine& rContentStatusLine,
                                IN TO CHeaderList* pExtraHeaders,
                                OUT ISipClientTransaction*& rpTransaction );

    // Summary:
    //  Sends a final NOTIFY request.
    //--------------------------------
    mxt_result ReportFinalStatusHelper(
                                    IN mxt_opaque opqTransaction,
                                    IN ISipNotifierSvc::EReason eReason,
                                    IN unsigned int uRetryAfterSec,
                                    IN const CSipStatusLine& rContentStatusLine,
                                    IN TO CHeaderList* pExtraHeaders,
                                    OUT ISipClientTransaction*& rpTransaction);


//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipTransferSvc07(IN const CSipTransferSvc07& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipTransferSvc07& operator=(IN const CSipTransferSvc07& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    // The manager of this service.
    //------------------------------
    ISipTransferMgr07* m_pMgr;

    // Aggregated ISipRefereeSvc. It will be responsible for sending NOTIFY
    // requests and receiving SUBSCRIBE and REFER requests.
    //----------------------------------------------------------------------
    IEComUnknown* m_pRefereeSvc;

    // Aggregated ISipReferrerSvc. It will be responsible for sending SUBSCRIBE
    // and REFER requests and receiving NOTIFY requests.
    //-------------------------------------------------------------------------
    IEComUnknown* m_pReferrerSvc;


    // TRANSFEROR SIDE

    // true if a current transfer is managed as a transferor.
    //--------------------------------------------------------
    bool m_bTransferorTransferActive;

    // TRANSFEREE SIDE

    // The request context for the current REFER request.
    //----------------------------------------------------
    ISipRequestContext* m_pTranfereeReferReqCtx;

    // true if a current transfer is managed as a transferee.
    //--------------------------------------------------------
    bool m_bTransfereeTransferActive;

    // The current refer ID that must be used to send NOTIFYs.
    //---------------------------------------------------------
    mxt_opaque m_opqCurrentTransfereeReferId;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  IsTransfereeReadyForTransfer
//==
//==============================================================================
//
//  Returns:
//      true if the service is ready to manage a transfer on the transferee side.
//
//  Description:
//      Tests if the service is ready to manage a transfer as a transferee.
//
//==EDOC========================================================================
inline bool CSipTransferSvc07::IsTransfereeReadyForTransfer()
{
    // We need to test m_pTranfereeReferReqCtx because a final NOTIFY may be
    // sent before the REFER is answered with a final response.
    //-----------------------------------------------------------------------
    return !m_bTransfereeTransferActive && m_pTranfereeReferReqCtx == NULL;
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPTRANSFERSVC07_H

