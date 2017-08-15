//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPPUBLISHSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPPUBLISHSVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
// MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT to be \
able to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_METHOD_H
#include "SipParser/Method.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPPUBLISHSVC_H
#include "SipUserAgent/ISipPublishSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipCoreEventList;
class CSipPacket;
class ISipRequestContext;
class ISipPublishMgr;
class CToken;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipPublishSvc
//========================================
//
// Description:
//  This class is used to manage all publications for an event name. It permits
//  operations such as an initial publication, refresh, modify, or remove. All
//  publication operations are exchanged between an EPA (Event Publication 
//  Agent) and ESC (Event State Compositor). This class implements only the 
//  behaviour of the EPA.
//
//  This service also triggers some events like Expiring, Expired timer, and
//  other events related to protocol errors. All events are dispatched through
//  the Publish Manager interface (ISipPublishMgr).
//
// Location:
//   SipUserAgentSvc/CSipPublishSvc.h
//
// See Also:
//   ISipPublishSvc, ISipPublishMgr
//
//==EDOC========================================================================
class CSipPublishSvc : private CEventDriven,
                       private CSipClientSvc,
                       private CSipReqCtxCoreSvc,
                       private ISipPublishSvc,
                       private ISipCoreSvc
{

//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- Hidden types
//------------------------
protected:

    // Type of Publish Operation sent.
    //--------------------------------
    enum ETypeOfRequest
    {
        eINITIAL,
        eMODIFY,
        eREFRESH,
        eREMOVE
    };

//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipPublishSvc();

    static void FinalizeCSipPublishSvc();


    //-- << ECOM mechanism >>
    //-------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << Constructors/destructors/operators >>
    //--------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipPublishSvc( IN IEComUnknown& rOuterIEComUnknown );

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSipPublishSvc();


    //-- << CSipClientSvc >>
    //--------------------------------------------

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext );


    //-- << ISipClientSvc methods >>
    //--------------------------------

    //-- << ISipPublishSvc >>
    //--------------------------------------------

    //  Summary:
    //   Configures the manager for this service.
    //-------------------------------------------
    virtual mxt_result SetManager(IN ISipPublishMgr* pMgr);

    //  Summary:
    //   Sets the delay between the EvExpiring and EvExpired event.
    //-------------------------------------------------------------
    virtual mxt_result SetExpiringThreshold(IN unsigned int uDelayInS);

    //  Summary:
    //   Sets the default expiration for a publication.
    //-------------------------------------------------
    virtual mxt_result SetDefaultExpiration(
                                         IN unsigned int uDefaultExpirationInS);


    //  Summary:
    //   Retrieves the entity-tag.
    //----------------------------
    virtual mxt_result GetEntityTag(OUT CString& rstrEntityTag);

    //  Summary:
    //   Sets the entity-tag.
    //-----------------------
    virtual mxt_result SetEntityTag(IN const CString& rstrEntityTag);

    //  Summary:
    //   Sends an Initial PUBLISH operation.
    //--------------------------------------
    virtual mxt_result Publish(IN mxt_opaque opqTransaction,
                               IN unsigned int uExpiresS,
                               IN const CString& rstrEvent,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction);

    //  Summary:
    //   Sends a Modify PUBLISH operation.
    //------------------------------------
    virtual mxt_result Modify(IN mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              IN TO CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction);
    //  Summary:
    //    Sends a Refresh PUBLISH operation.
    //---------------------------------------
    virtual mxt_result Refresh(IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               OUT ISipClientTransaction*& rpTransaction);
    //  Summary:
    //    Sends a Remove PUBLISH operation.
    //-------------------------------------
    virtual mxt_result Remove(IN mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              OUT ISipClientTransaction*& rpTransaction);

    //-- << ISipReqCtxCoreSvc >>
    //--------------------------------------------

    //  Summary:
    //    Informs the request context core service of a received packet.
    //------------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    //  Summary:
    //    Inform the service that the application is done updating the packet.
    //------------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    //  Summary:
    //    Let the service call an event on the application.
    //-----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    //  Summary:
    //    Let the service clear the state associated with an event in which the
    //    application has no interest.
    //-------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    //  Summary:
    //    Add a reference on this request context core service.
    //---------------------------------------------------------
    virtual unsigned int AddRcsRef();

    //  Summary:
    //    Remove a reference on this request context core service.
    //------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


    //-- << ISipCoreSvc >>
    //--------------------------------------------

    //  Summary:
    //    Obtain an instance of an ISipReqCtxCoreSvc to attach to a request
    //    context.
    //---------------------------------------------------------------------
    virtual
    ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                    IN const CSipPacket& rPacket,
                                    IN ISipCoreSvc::EDirection eDirection);

    //  Summary:
    //    Gives to which degree the service is a good candidate to own a
    //    request context.
    //------------------------------------------------------------------
    virtual
    ISipCoreSvc::EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                                IN const CSipPacket& rPacket);

    //  Summary:
    //    Return the ISipReqCtxCoreSvc instance to use as the owner for a
    //    request context.
    //-------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();


    //-- << ITimerServiceMgr >>
    //--------------------------------------------

    //  Summary:
    //    Notify the manager that a new timer elapsed or was stopped.
    //---------------------------------------------------------------
    virtual void EvTimerServiceMgrAwaken( IN bool bStopped,
                                          IN unsigned int uTimer,
                                          IN mxt_opaque opq );


    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  General function to send the PUBLISH operation.
    //-------------------------------------------------
    mxt_result SendPublish ( IN ETypeOfRequest eTypeOfRequest,
                             IN mxt_opaque opqTransaction,
                             IN unsigned int uExpiresS,
                             IN const CString& rstrEvent,
                             IN TO CHeaderList* pExtraHeaders,
                             OUT ISipClientTransaction*& rpTransaction,
                             IN TO CSipMessageBody* pMessageBody = NULL);

    // Summary:
    //  Start timers for a publication.
    //---------------------------------
    void StartPublicationTimer();

    // Summary:
    //  Clears the data member used to establish a PUBLISH operation.
    //---------------------------------------------------------------
    void  ResetPublication();

    // Summary:
    //  Gets from the 2xx response the entity tag and the expires value.
    //------------------------------------------------------------------
    void GetPublicationInformation(IN const CSipPacket& rPacket);

    // Summary:
    //  Processes state changes for CallEvent and ClearEvent methods.
    //----------------------------------------------------------------
    void ProcessEventHelper(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent,
                            IN const CSipPacket* pPacket );

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //   Copy Constructor.
    //--------------------
    CSipPublishSvc(IN const CSipPublishSvc& rSrc);

    // Summary:
    //   Assignment Operator.
    //-----------------------
    CSipPublishSvc& operator=(IN const CSipPublishSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:

    // Events to be called by the Publish service on its manager.
    //------------------------------------------------------------
    enum EPacketReceivedEvent
    {
        ePROGRESS_RESPONSE_RECEIVED,
        eSUCCESS_RESPONSE_RECEIVED,
        eFAILURE_EXPIRES_TOO_BRIEF,
        eFAILURE_CONDITIONAL_REQUEST,
        eFAILURE_PROTOCOL_ERROR,
        eFAILURE_RESPONSE_RECEIVED
    };

private:

    // The events are reported through this manager.
    //-----------------------------------------------
    ISipPublishMgr* m_pMgr;

    // The request context used for the current PUBLISH request transaction.
    //-----------------------------------------------------------------------
    ISipRequestContext* m_pCurrentRequestContext;

    // It contains the delay between the EvExpiring and EvExpired event
    // occurance.
    //------------------------------------------------------------------------
    unsigned int m_uExpiringThresholdInS;

    // Delay for which a publication is considered active. After an initial
    // publication, the delay is initialized with the Expires header found in a
    // 2xx response. This variable is used to verify if the stack can send
    // a Modify, Refresh or Remove operation. If this variable is equal to zero,
    // no Modify, Refresh or Remove operation is possible.
    //-------------------------------------------------------------------------
    unsigned int m_uExpirationTimeInS;

    // It contains the default expiration value for a publication. After a
    // publish operation, if the expires header in a 2xx response is invalid or
    // empty, this value is assumed to be the expiration value for the
    // published event.
    unsigned int m_uDefaultExpirationTimeInS;

    // It contains the delay of Expired event for an active Publication. Once
    // we receive a 2xx for a publication, the Expired timer value is computed.
    // When the Expiring event is fired the EvExpired event is started with this
    // value.
    //--------------------------------------------------------------------------
    unsigned int m_uExpiredTimeInS;

    // The last Publish operation type.
    //----------------------------------
    ETypeOfRequest m_eTypeOfRequest;

    // The entity tag.
    //-----------------
    CString m_strEntityTag;

    // The published event name.
    //---------------------------
    CString m_strEvent;

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPPUBLISHSVC_H
