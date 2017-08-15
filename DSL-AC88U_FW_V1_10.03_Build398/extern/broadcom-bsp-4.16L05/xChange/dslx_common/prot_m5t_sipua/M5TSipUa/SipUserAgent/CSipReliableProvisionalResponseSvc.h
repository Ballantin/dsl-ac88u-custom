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
#ifndef MXG_CSIPRELIABLEPROVISIONALRESPONSESVC_H
#define MXG_CSIPRELIABLEPROVISIONALRESPONSESVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CHEADERLIST_H
#include "SipParser/CHeaderList.h"
#endif

#ifndef MXG_CSIPMESSAGEBODY_H
#include "SipParser/CSipMessageBody.h"
#endif


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif

#ifndef MXG_ISIPRELIABLEPROVISIONALRESPONSESVC_H
#include "SipUserAgent/ISipReliableProvisionalResponseSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPSERVEREVENTCONTROL_H
#include "SipCore/ISipServerEventControl.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_STATUSCODE_H
#include "SipParser/StatusCode.h" // ESipStatusClass
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
//== Class: CSipReliableProvisionalResponseSvc
//========================================
//
// Description:
//   This class is necessary to manage reliable provisional responses on an
//   INVITE request. It permits to transform provisional responses into reliable
//   provisional responses and to retransmit them until a corresponding PRACK
//   request is received. It also manages the corresponding PRACK requests. This
//   class is able to manage reliable provisional responses on UAS and UAC side.
//
// Location:
//   SipUserAgent/CSipReliableProvisionalResponseSvc.h
//
// See Also:
//   ISipReliableProvisionalResponseMgr.h
//
//==EDOC========================================================================
class CSipReliableProvisionalResponseSvc :
                        private CEventDriven,
                        private CSipClientSvc,
                        private ISipReliableProvisionalResponseSvc,
                        private CSipReqCtxCoreSvc,
                        private ISipServerEventControl,
                        private ISipCoreSvc
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

    static mxt_result InitializeCSipReliableProvisionalResponseSvc();

    static void FinalizeCSipReliableProvisionalResponseSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipReliableProvisionalResponseSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipReliableProvisionalResponseSvc();


    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << CSipClientSvc >>
    //--------------------------------------------

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext);


    //-- << ISipReliableProvisionalResponseSvc >>
    //--------------------------------------------

    // Summary:
    //  Configures the manager associated with this service.
    //-------------------------------------------------------
    virtual mxt_result SetManager(IN ISipReliableProvisionalResponseMgr* pMgr);

    // Summary:
    //  Configures whether or not reliable provisional responses are required
    //  for local INVITE requests.
    //------------------------------------------------------------------------
    virtual mxt_result LocalRequestsRequireReliability(IN bool bRequire);

    // Summary:
    //  Gives an ISipServerEventControl interface through which provisional
    //  responses are sent reliably.
    //----------------------------------------------------------------------
    virtual mxt_result MakeReliableServerEventControl(
                        IN ISipServerEventControl* pServerEventCtrl,
                        OUT ISipServerEventControl*& rpRelServerEventCtrl );

    // Summary:
    //  Sends a PRACK request to acknowledge a reliable provisional response
    //  received.
    //-----------------------------------------------------------------------
    virtual mxt_result Prack(IN mxt_opaque opqTransaction,
                             IN TO CHeaderList* pExtraHeaders,
                             IN TO CSipMessageBody*  pMessageBody,
                             OUT ISipClientTransaction*& rpTransaction);

    // Summary:
    //  Says whether or not the remote endpoint supports the 100rel extension.
    //------------------------------------------------------------------------
    virtual bool IsReliabilitySupportedByPeer();

    // Summary:
    //  Says whether or not the remote endpoint insists on reliable delivery of
    //  provisional response.
    //-------------------------------------------------------------------------
    virtual bool IsReliabilityRequiredByPeer();


    //-- << ISipReqCtxCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Manages the initial packet of a request context.
    //--------------------------------------------------
    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    // Summary:
    //  Informs the request context core service of a received packet.
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
    //  Informs the service that the application is done updating the packet.
    //-----------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    // Summary:
    //  Sends a response to a received request.
    //-----------------------------------------
    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket,
                                    IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TO CHeaderList*     pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Informs that a CANCEL has been received for a pending request.
    //----------------------------------------------------------------
    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket);

    // Summary:
    //  Lets the service call an event on the application.
    //----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    // Summary:
    //  Lets the service clear the state associated with an event in which the
    //  application has no interest.
    //------------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    // Summary:
    //  Adds a reference on this request context core service.
    //--------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Removes a reference on this request context core service.
    //-----------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


    //-- << ISipServerEventControl >>
    //--------------------------------------------

    // Summary:
    //  Sends a response for a request received on this transaction.
    //---------------------------------------------------------------
    virtual mxt_result SendResponse(IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TO CHeaderList* pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Gets the user-specified opaque parameter associated with this
    //  transaction.
    //----------------------------------------------------------------
    virtual mxt_opaque GetOpaque();

    // Summary:
    //  Sets the user-specified opaque parameter associated with this
    //  transaction.
    //----------------------------------------------------------------
    virtual void SetOpaque(IN mxt_opaque opq);

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void SetSendOption(IN bool bToNetwork);
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    //-- << ITimerServiceMgr >>
    //--------------------------------------------

    // Summary:
    //  Notifies the manager that a new timer elapsed or was stopped.
    //---------------------------------------------------------------
    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);


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
    //--------------------------------------------------------------------
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


    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  This method returns true if a header Require with a '100rel' option-tag
    //  is present in the packet.
    //--------------------------------------------------------------------------
    bool IsReliabilityRequired(IN const CSipPacket& rPacket);

    // Summary:
    //  This method sets the reliability level asked for an INVITE request.
    //----------------------------------------------------------------------
    void SetPeerReliabilityLevel(IN const CSipPacket& rPacket);

    // Summary:
    //  Saves the data for a reliable provisional response retransmission.
    //---------------------------------------------------------------------
    mxt_result SaveReIssueData(IN unsigned int uCode,
                               IN const char* szReason,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Deletes the re-issue data.
    //-----------------------------
    void DeleteReIssueData();

    // Summary:
    //  Sends the original reliable response from the re-issue data saved and
    //  starts the timer.
    //------------------------------------------------------------------------
    mxt_result SendOriginalReliableResponse(
                                        IN unsigned int uCode,
                                        IN const char* szReason,
                                        IN TO CHeaderList* pExtraHeaders,
                                        IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Sends the reliable response from the re-issue data saved.
    //------------------------------------------------------------
    mxt_result SendReliableResponse();

    // Summary:
    //  Stops the retransmission of the reliable provisional response.
    //-----------------------------------------------------------------
    void ReliableResponseTerminated( IN bool bStopTimer );

    // Summary:
    //  This method indicates if the service is ready to follow a new INVITE
    //  request's progress.
    //------------------------------------------------------------------
    bool IsReadyToFollowInvite();

    // Summary:
    //  Indicates if there was a reliable response sent that is waiting for a
    //  PRACK request.
    //-------------------------------------------------------------------------
    bool IsReliableResponseWaitingForPrack();

    // Summary:
    //  Terminates the reliable response processing when a final response
    //  to a received PRACK request has been sent.
    //--------------------------------------------------------------------
    void TerminateLastReliableResponseHelper();

    // Summary:
    //  Resets the state of the service when the last reliable response of an
    //  INVITE request received is terminated.
    //------------------------------------------------------------------------
    void LastReliableResponseTerminated();

    // Summary:
    //  Processes the events.
    //------------------------
    void ProcessEventHelper( IN ISipRequestContext& rRequestContext,
                             IN mxt_opaque opqEvent,
                             IN const CSipPacket* pPacket );

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipReliableProvisionalResponseSvc(
                            IN const CSipReliableProvisionalResponseSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipReliableProvisionalResponseSvc& operator=(
                            IN const CSipReliableProvisionalResponseSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    enum EReliabilityLevel
    {
        // The order of the declaration is important.
        //--------------------------------------------
        eRELIABILITY_NOT_SUPPORTED = 0,
        eRELIABILITY_SUPPORTED,
        eRELIABILITY_REQUIRED
    };

    enum ERelResRelatedInvite
    {
        // The order of the declaration is important.
        //--------------------------------------------
        eNO_REL_RES_PROCESSING = 0,
        eCURRENT_INVITE_UAS,
        ePREVIOUS_INVITE_UAS,
        ePREVIOUS_INVITE_UAC,
        eCURRENT_INVITE_UAC
    };

    // The enum of events affecting the service clear process.
    //---------------------------------------------------------
    enum EClearState
    {
        // Not updating a final response.
        eIDLE,
        // Updating a final response.
        eUPDATING,
        // Updating a final response and waiting to clear.
        eUPDATING_CLEAR
    };

    struct SReIssueData
    {
        // The code of the sent reliable provisional response.
        unsigned int m_uCode;
        // The reason phrase of the sent reliable provisional response. MAY be
        // NULL.
        CString m_strReason;
        // The extra headers included in the sent reliable provisional response.
        // NULL when there was no reliable provisional response sent.
        CHeaderList* m_pExtraHeaders;
        // The content included in the sent reliable provisional response. MAY
        // be NULL.
        CSipMessageBody* m_pMessageBody;

        SReIssueData( IN unsigned int uCode = 0,
                      IN const char* szReason = NULL,
                      IN TO CHeaderList* pExtraHeaders = NULL,
                      IN TO CSipMessageBody* pMessageBody = NULL )
        :   m_uCode( uCode ),
            m_strReason( szReason ),
            m_pExtraHeaders( pExtraHeaders ),
            m_pMessageBody( pMessageBody ){}

        ~SReIssueData()
        {
            MX_DELETE(m_pExtraHeaders);
            MX_DELETE(m_pMessageBody);
        }
    };

    // This is the manager of this service.
    //--------------------------------------
    ISipReliableProvisionalResponseMgr* m_pMgr;


    // UAS

    // The 2xx response packet (to INVITE request) that we delayed while waiting
    // for the completion of the reliable provisional response process.
    SReIssueData* m_pstDelayed2xxResponse;

    // Data needed to reissue the same reliable provisional response.
    //----------------------------------------------------------------
    SReIssueData* m_pstReIssueData;


    // UAC

    // This is the level of reliability needed for an outgoing INVITE request.
    //-------------------------------------------------------------------------
    EReliabilityLevel m_eLocalReliabilityLevel;

    // The request context managing the Client PRACK request.
    //--------------------------------------------------------
    ISipRequestContext* m_pPrackRequestContext;

    // Both UAS and UAC

    // This is the level of reliability needed for an incoming INVITE request.
    //-------------------------------------------------------------------------
    EReliabilityLevel m_ePeerReliabilityLevel;

    // The ISipRequestContext that manages the INVITE request in which the
    // application could send reliable responses.
    //---------------------------------------------------------------------
    ISipRequestContext* m_pInviteReqCtx;

    // The CSeq value of the current INVITE request.
    //-----------------------------------------------
    uint64_t m_uCurrentCSeq;

    // The RSeq that was used in the last reliable provisional response. SHOULD
    // be uREL_SVC_INVALID_RSEQ if there was no reliable provisional response
    // sent in this transaction.
    //--------------------------------------------------------------------------
    uint64_t m_uCurrentRSeq;

    // This tells if the current reliable provisional response processed is for
    // the current INVITE request, the last INVITE request received or the last
    // INVITE sent.
    //--------------------------------------------------------------------------
    ERelResRelatedInvite m_eRelResRelatedInvite;

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    bool m_bToNetwork;
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    // This is the CSeq value of the INVITE request related to the last
    // reliable provisional response sent or received.
    //------------------------------------------------------------------
    uint64_t m_uCSeqValueToUse;

    // The current clear process state.
    //----------------------------------
    EClearState m_eClearState;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  DeleteReIssueData
//==
//==============================================================================
//
//  Description:
//      Deletes all the informations kept for retransmission of reliable
//      provisional response.
//
//==EDOC========================================================================
inline void CSipReliableProvisionalResponseSvc::DeleteReIssueData()
{
    MX_DELETE(m_pstReIssueData);
    m_pstReIssueData = NULL;
}

//==SDOC========================================================================
//==
//==  IsReadyToFollowInvite
//==
//==============================================================================
//
//  Returns:
//      true if this service is in a state where it can follow a new INVITE
//          request's progress.
//
//      false otherwise.
//
//  Description:
//      This method tells if this service is ready to follow a new INVITE
//      request's progress.
//
//==EDOC========================================================================
inline bool CSipReliableProvisionalResponseSvc::IsReadyToFollowInvite()
{
    return m_pInviteReqCtx == NULL && // no pending INVITE
           m_pstDelayed2xxResponse == NULL; // no delayed INVITE response
}

//==SDOC========================================================================
//==
//==  IsReliableResponseWaitingForPrack
//==
//==============================================================================
//
//  Returns:
//      true if a reliable provisional response has been sent and a PRACK
//          request for that response has not been received.
//
//      false otherwise (a PRACK request is unexpected at the moment).
//
//  Description:
//      Tells if receiving a PRACK request at the moment would be correct (the
//      PRACK has chances to be accepted).
//
//==EDOC========================================================================
inline
    bool CSipReliableProvisionalResponseSvc::IsReliableResponseWaitingForPrack()
{
    return m_pstReIssueData != NULL &&
           m_pPrackRequestContext == NULL;
}

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
inline void CSipReliableProvisionalResponseSvc::SetSendOption(IN bool bToNetwork)
{
    m_bToNetwork = bToNetwork;
}
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPRELIABLEPROVISIONALRESPONSESVC_H

