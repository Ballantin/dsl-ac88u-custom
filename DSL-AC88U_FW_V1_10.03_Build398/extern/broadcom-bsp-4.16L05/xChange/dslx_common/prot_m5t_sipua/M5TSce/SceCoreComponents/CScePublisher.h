//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
//==============================================================================
#ifndef MXG_CSCEPUBLISHER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEPUBLISHER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSCEBASECOMPONENT_H
#include "SceCoreComponents/CSceBaseComponent.h"
#endif

#ifndef MXG_ISCEPUBLISHER_H
#include "SceCoreComponents/IScePublisher.h"
#endif

#ifndef MXG_CHEADERLIST_H
#include "SipParser/CHeaderList.h"
#endif

#ifndef MXG_CNAMEADDR_H
#include "SipParser/CNameAddr.h"
#endif

#ifndef MXG_CSIPMESSAGEBODY_H
#include "SipParser/CSipMessageBody.h"
#endif

#ifndef MXG_ISIPPUBLISHMGR_H
#include "SipUserAgent/ISipPublishMgr.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CScePublisher
//==============================================================================
//
// Description:
//   This class implements the IScePublisher interface.  It handles generic
//   PUBLISH requests.
//
// Location:
//   SceCoreComponents/CScePublisher.h
//
// See Also:
//   IScePublisher
//
//==============================================================================
class CScePublisher : protected CSceBaseComponent,
                      protected IScePublisher,
                      protected ISipPublishMgr
{
//-- Friend Declarations
    friend class CSceCoreComponentsInitializer;

//-- New types within class namespace
public:
protected:

    // Description:
    //  This is the timer ID values used in this component.
    enum ETimerId
    {
        // Description:
        //  Timer to retry a publish.
        eTIMER_ID_RETRY_PUBLISH = CSceBaseComponent::eCHILD_COMPONENT_FIRST_TIMER_ID,
        // Description:
        //  A component inheriting from this object must start its timer values
        //  from this value.
        eTIMER_ID_UNUSED
    };

private:

    // Description:
    //  These are the publisher states.
    enum EState
    {
        // Description:
        //  Steady state. This is the initial state, the publisher is initialized
        //  but no PUBLISH has been sent yet. The publisher remains in this state
        //  until the first call to Publish() made by the application.
        eSTATE_INITIAL   = 0,

        // Description:
        //  This is a transient state, and the publisher remains here until the
        //  object is completely configured (when OnUserAgentServiceConfigured is called)
        //  after the first call to Publish() made by the application.
        eSTATE_CONFIGURING,

        // Description:
        //  This is a transient state, the publisher remains in this state when
        //  the initial PUBLISH has failed it is waiting for resend it.
        eSTATE_RETRYING_INITIAL,

        // Description:
        //  Transient state. The publisher remains here when it has sent an initial
        //  or a modify PUBLISH request and it is waiting for the response.
        eSTATE_PUBLISHING,

        // Description:
        //  Transient state. The publisher remains in it when it has sent a refresh
        //  PUBLISH request and it is waiting for the response.
        eSTATE_REFRESHING,

        // Description:
        //  This is a steady state, the publication is still active on the server.
        eSTATE_PUBLISHED,

        // Description:
        //  This is a steady state, the publication has been removed from the server.
        eSTATE_REMOVED,

        // Description:
        //  Final State. There was an unrecoverable error condition, the object will
        //  remains here until it is terminated.
        eSTATE_ERROR,

        // Description:
        //  Final state. The publication has been terminated and the publisher is
        //  currently terminating this component.
        eSTATE_TERMINATED
    };

    // Description:
    //  These values are set as the mxt_opaque or client PUBLISH requests.
    enum ETypeOfPublish
    {
        // Description:
        //  This is the initial PUBLISH.
        ePUBLISH_TYPE_INITIAL = 1,
        // Description:
        //  This is a refresh PUBLISH.
        ePUBLISH_TYPE_REFRESH,
        // Description:
        //  This is a modify PUBLISH.
        ePUBLISH_TYPE_MODIFY,
        // Description:
        //  This is a remove PUBLISH.
        ePUBLISH_TYPE_REMOVE
    };

//-- Published Interface
public:

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CScePublisher(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CScePublisher();


    //-- << Initialization mechanism >>
    static mxt_result InitializeCScePublisher();

    static void FinalizeCScePublisher();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << CSceBaseComponent >>
    virtual uint32_t GetSupportedServicesBitset() const;

    virtual void OnUserAgentServiceConfigured(IN mxt_result res,
                                              IN TO CTelUri* pTelUri,
                                              IN TO CList<CSipUri>* plstSipUris);

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    //-- << IScePublisher >>
    virtual mxt_result SetManager(IN IScePublisherMgr* pMgr);

    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual mxt_result SetEventType(ESipEventType eEventType);

    virtual ESipEventType GetEventType();

    virtual mxt_result SetPeerAddr(IN const CNameAddr& rPeerAddr);

    virtual mxt_result GetPeerAddr(OUT CNameAddr& rPeerAddr);

    virtual void SetPublicationRetryDelay(IN uint32_t uNbSeconds);

    virtual uint32_t GetPublicationRetryDelay() const;

    virtual void SetPublicationExpirationTime(IN uint32_t uNbSeconds);

    virtual uint32_t GetPublicationExpirationTime() const;

    virtual mxt_result Publish(IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               IN TO CSceCallerPreferences* pCallerPreferences);

    virtual mxt_result Terminate();

    virtual mxt_result Abort();

    //-- << ISipPublishMgr >>
    virtual void EvProgress(IN ISipPublishSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipPublishSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipPublishSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvExpiresIntervalTooBrief(IN ISipPublishSvc* pSvc,
                                           IN ISipClientEventControl* pClientEventCtrl,
                                           IN unsigned int uExpiresS,
                                           IN const CSipPacket& rResponse);

    virtual void EvConditionalRequestFailed(IN ISipPublishSvc* pSvc,
                                            IN ISipClientEventControl* pClientEventCtrl,
                                            IN unsigned int uExpiresS,
                                            IN const CSipPacket& rResponse);

    virtual void EvProtocolError(IN ISipPublishSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventCtrl,
                                 IN const CSipPacket& rResponse);

    virtual void EvExpiring(IN ISipPublishSvc* pSvc);

    virtual void EvExpired(IN ISipPublishSvc* pSvc);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CScePublisher(IN const CScePublisher& rSrc);

    // Summary:
    //  Assignment Operator.
    CScePublisher& operator=(IN const CScePublisher& rSrc);


    //-- << Helper methods >>

    // Summary:
    //  Creates and configure a context.
    mxt_result CreateContext(IN ISceUserConfig* pUserConfig);

    // Summary:
    //  Configures the services on the context.
    mxt_result ConfigureServices();

    // Summary:
    //  Setups a context to send PUBLISH requests.
    mxt_result PrepareToSendPublish(IN TOA CHeaderList* pExtraHeaders,
                                    IN TOA CSipMessageBody* pMessageBody);

    // Summary:
    //  Sends an PUBLISH request.
    mxt_result SendPublish(IN ETypeOfPublish eTypeOfPublish,
                           IN CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody);

    // Summary:
    //  Determines the retry time when a final negative response is received.
    uint32_t GetRetryTime(IN const CSipPacket& rResponse);

    // Summary:
    //  Reissues the PUBLISH request.
    mxt_result ReIssuePublish(IN ISipClientEventControl* pClientEventControl);

    // Summary:
    //  Process a PUBLISH failure that can be reissued.
    void ProcessFailureToReissue(IN ISipPublishSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventCtrl,
                                 IN const CSipPacket& rResponse);

    // Summary:
    //  Terminates this object.
    void Shutdown(IN bool bCauseIsError);

    // Summary:
    //  Completes the shut down process.
    void ShutdownSecondStep();

    // Summary:
    //  Removes the publication from the Event State Compositor.
    mxt_result RemovePublication();

    // Summary:
    //  Updates the headers used by the publication requests.
    mxt_result UpdateHeadersWithCapabilitiesAndCallerPref(IN CSceCallerPreferences* pCallerPreferences,
                                                          INOUT CHeaderList* pHeaders);
    // Summary:
    // Stop Timer if it is started and release internal context.
    void ReleaseCurrentContext();

//-- Hidden Data Members
protected:
private:

    // The manager of this object.
    IScePublisherMgr* m_pMgr;

    // Publisher state.
    EState m_eState;

    CNameAddr m_ResourceAor;

    ESipEventType m_eEventType;

    // This is the value set by SetPublicationRetryDelay.
    // Default is 60 seconds (1 minute).
    uint32_t m_uPublicationRetryDelayS;

    // This is the value set by SetPublicationExpirationTime.
    // Default is 3600 seconds (1 hour).
    uint32_t m_uPublicationExpirationTimeS;

    struct SDataToPublish
    {
        CHeaderList* m_pExtraHeaders;
        CSipMessageBody* m_pMessageBody;
        
        SDataToPublish(TOA CHeaderList* pExtraHeaders,
                       TOA CSipMessageBody* pMessageBody)
        :   m_pExtraHeaders(pExtraHeaders),
            m_pMessageBody(pMessageBody)
        {
        }

        ~SDataToPublish()
        {
            Empty();
        }

        void Empty()
        {
            MX_DELETE(m_pExtraHeaders);
            m_pExtraHeaders = NULL;

            MX_DELETE(m_pMessageBody);
            m_pMessageBody = NULL;
        }

        bool IsEmpty()
        {
            return m_pExtraHeaders == NULL &&
                   m_pMessageBody == NULL;
        }
    };

    // Data to publish during th asynchronous call to ConfigureUserAgentService.
    SDataToPublish m_stDataToPublish;

    // Data to publish given to the component while it was refreshing another
    // publication.
    SDataToPublish m_stPendingDataToPublish;

    // Headers needed when refreshing a publication or when removing it.
    CHeaderList* m_pHeaders;

    // Flag: a publication is pending. This flag is set ON when the application
    // demands a modify publication when the publisher is performing a refreshing,
    // so the modify must be sent later.
    bool m_bPublishIsPending;

    // Flag: the publisher has started a shutting down process.
    bool m_bPublisherIsShuttingDown;

    // Flag: the shutting down was triggered by an error.
    bool m_bShuttingDownByAnError;

    // Flag: the shutting down must be done the quickest possible (no message on
    // the network).
    bool m_bAborting;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CSCEPUBLISHER_H

//M5T_INTERNAL_USE_END
