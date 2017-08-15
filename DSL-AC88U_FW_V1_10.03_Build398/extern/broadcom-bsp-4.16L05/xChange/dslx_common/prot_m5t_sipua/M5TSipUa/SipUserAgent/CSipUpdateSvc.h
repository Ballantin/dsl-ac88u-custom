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
#ifndef MXG_CSIPUPDATESVC_H
#define MXG_CSIPUPDATESVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_ISIPUSERAGENTSVC_H
#include "SipUserAgent/ISipUserAgentSvc.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif

#ifndef MXG_ISIPUPDATESVC_H
#include "SipUserAgent/ISipUpdateSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
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
class CHeaderList;
class CSipHeader;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipUpdateSvc
//========================================
//
// Description:
//   This class is used to manage the use of UPDATE requests and responses. It
//   permits to automatically reply to UPDATE requests received at a wrong time
//   and to the application to send UPDATE requests and be warned of the
//   responses. This class also permits to change the Request-URI of the
//   ISipUserAgentSvc when an UPDATE request succeeds.
//
// Location:
//   SipUserAgent/CSipUpdateSvc.h
//
// See Also:
//   ISipUpdateSvc, ISipUpdateMgr
//
//==EDOC========================================================================
class CSipUpdateSvc : private CSipClientSvc,
                      private ISipUpdateSvc,
                      private CSipReqCtxCoreSvc,
                      private ISipCoreSvc
{
//-- Hidden types
//------------------------
private:
    enum EUpdateEvent
    {
        // Reported to the application.
        //------------------------------
        ePROVISIONAL_RESPONSE,
        eSUCCESS_RESPONSE,
        eFAILURE_RESPONSE,
        // UPDATE request received when there is no processing UPDATE request.
        //---------------------------------------------------------------------
        eREQUEST_RECEIVED,

        // Errors where a reponse is automatically returned.
        //---------------------------------------------------

        // UPDATE request received with no or with an invalid Contact header
        // (responded with a 400).
        //-------------------------------------------------------------------
        eINVALID_CONTACT_HEADER,
        // UPDATE request received while there is no ISipUserAgentSvc attached
        // to the context (responded with 500).
        //---------------------------------------------------------------------
        eNO_USER_AGENT_SERVICE,
        // UPDATE request received while there is no dialog (responded with
        // 481).
        //------------------------------------------------------------------
        eCALL_DOES_NOT_EXIST,
        // UPDATE request received while there is a pending UPDATE request that
        // came from this UA (responded with 491).
        //----------------------------------------------------------------------
        eLOCAL_PENDING_REQUEST,
        // UPDATE request received while there is a pending UPDATE request that
        // came from the remote UA (responded with 500 with a Retry-After
        // header).
        //----------------------------------------------------------------------
        ePEER_PENDING_REQUEST,

        // Not reported to the application.
        //----------------------------------

        // UPDATE request received while there was no manager set to this
        // service (responded with 500).
        //----------------------------------------------------------------
        eMANAGER_NOT_SET
    };


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

    static mxt_result InitializeCSipUpdateSvc();

    static void FinalizeCSipUpdateSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //--------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipUpdateSvc(IN IEComUnknown&  rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipUpdateSvc();


    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISipUpdateSvc >>
    //--------------------------------------------

    // Summary:
    //  Configures the manager associated with this service.
    //-----------------------------------------------------
    virtual mxt_result SetManager(IN ISipUpdateMgr* pMgr);

    // Summary:
    //  Sends an UPDATE request.
    //---------------------------
    virtual mxt_result Update(IN mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              IN TO CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction);

    //-- << ISipReqCtxCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Manages the initial packet of a request context.
    //--------------------------------------------------
    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    // Summary:
    //  Notifies the request context core service of a received packet.
    //-----------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Notifies the service that the application is done updating the packet.
    //------------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    // Summary:
    //  Sends a response to a received request.
    //-----------------------------------------
    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket,
                                    IN unsigned int        uCode,
                                    IN const char*         szReason,
                                    IN TO CHeaderList*     pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

    // Summary:
    //  Notifies that a CANCEL has been received for a pending request.
    //-----------------------------------------------------------------
    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket);

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

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


    //-- << CSipClientSvc >>
    //--------------------------------------------

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext);


    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  Indicates if this service is in a state in which it can receive or send
    //  an UPDATE request.
    //-------------------------------------------------------------------------
    bool IsReadyToUpdate( ISipUserAgentSvc& rUserAgentSvc );

    // Summary:
    //  Performs the processing needed for the selected event.
    //---------------------------------------------------
    void DoEventProcessing( IN ISipRequestContext& rRequestContext,
                            IN EUpdateEvent eEvent,
                            IN const CSipPacket* pPacket );

    // Summary:
    //  Sends a 500 response with a Retry-After header.
    //--------------------------------------------------
    mxt_result Send500ResponseWithRetry( IN ISipRequestContext& rRequestContext,
                                         IN const CSipPacket& rPacket );

    // Summary:
    //  Returns the failure result to put in EvInvalidUpdate.
    //--------------------------------------------------------
    mxt_result GetErrorValue( IN EUpdateEvent eEvent );

    // Summary:
    //  Replaces the header by the passed parameter.
    //-----------------------------------------------
    mxt_result ReplaceHeaderType( IN TO CSipHeader* pHeaderToAdd,
                                  INOUT CHeaderList& rHeaderList );


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipUpdateSvc(IN const CSipUpdateSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipUpdateSvc& operator=(IN const CSipUpdateSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
    // This enum serves to determine from which UA the current request comes
    // from.
    //-----------------------------------------------------------------------
    enum EClientOfCurrentRequest
    {
        // The current UPDATE request was sent by the local UA.
        eLOCAL,
        // The current UPDATE request was received on the local UA.
        ePEER
    };

    // The manager of this service.
    //------------------------------
    ISipUpdateMgr* m_pMgr;

    // The request context used for the current UPDATE request (received from
    // or sent by the update service).
    //------------------------------------------------------------------------
    ISipRequestContext* m_pCurrentRequestContext;

    // Tells if the from where the request was sent.
    //-----------------------------------------------
    EClientOfCurrentRequest m_eClientOfRequest;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  IsReadyToUpdate
//==
//==============================================================================
//
//  Returns:
//      true if a UPDATE request can be received or sent at the moment.
//
//  Description:
//      Tells if the update service is in a state where an UPDATE request can be
//      received or sent.
//
//==EDOC========================================================================
inline bool CSipUpdateSvc::IsReadyToUpdate( ISipUserAgentSvc& rUserAgentSvc )
{
    return ( m_pMgr != NULL &&
             m_pCurrentRequestContext == NULL &&
             rUserAgentSvc.GetState() == ISipUserAgentSvc::eIN_DIALOG );
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPUPDATESVC_H

