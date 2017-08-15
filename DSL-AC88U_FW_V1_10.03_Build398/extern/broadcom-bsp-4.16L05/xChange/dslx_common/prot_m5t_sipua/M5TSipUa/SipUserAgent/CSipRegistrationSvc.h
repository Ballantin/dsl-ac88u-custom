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
//   form whatsoever without written prior approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
//==============================================================================
//
// Portions of this software Copyright (c) 2008-2012 Broadcom
// 
//==============================================================================
#ifndef MXG_CSIPREGISTRATIONSVC_H
#define MXG_CSIPREGISTRATIONSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT to be \
able to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------

#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif // MXG_CTOKEN_H

#ifndef MXG_METHOD_H
#include "SipParser/Method.h" // g_aszMETHOD_NAME
#endif // MXG_METHOD_H


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif // MXG_CSIPCLIENTSVC_H

#ifndef MXG_ISIPREGISTRATIONSVC_H
#include "SipUserAgent/ISipRegistrationSvc.h"
#endif // MXG_ISIPREGISTRATIONSVC_H

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif // MXG_CSIPREQCTXCORESVC_H

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif // MXG_CEVENTDRIVEN_H

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipCoreEventList;
class CSipPacket;
class ISipRequestContext;
class ISipRegistrationMgr;
class CHostPort;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipRegistrationSvc
//========================================
//
// Description:
//   This class is used to manage contacts registered to a registrar.  It
//   permits to add and remove registrations from the registrar and gives a
//   feedback to the ISipRegistrationMgr when the contacts are about to expire
//   and when they expire.
//
// Updated Behaviour:
//
//  The behaviour of the registration service has changed from MxSF v3.x.
//
//  In v4.0, the service does not support values other than an integral number
//  of seconds in the Expires header and in the expires parameter of a Contact
//  header received from the registrar, as defined in RFC 3261.
//
//  If no valid expiration value is found in the packet received from the
//  registrar, the default value of 3600 seconds (1 hour) is taken.
//
// Location:
//   SipUserAgent/CSipRegistrationSvc.h
//
// See Also:
//   ISipRegistrationSvc, ISipRegistrationMgr
//
//==EDOC========================================================================
class CSipRegistrationSvc : private CEventDriven,
                            private CSipClientSvc,
                            private ISipRegistrationSvc,
                            private CSipReqCtxCoreSvc,
                            private ISipCoreSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    friend class CUserAgentRegistrationSvcTest;
    //M5T_INTERNAL_USE_END
public:
    
    // Summary:
    //  Sets the default matching that will be used for all registration
    //  services.
    static mxt_result SetDefaultContactMatchingType(IN unsigned int uContactMatchingType);

//-- Hidden types
//------------------------
protected:

    struct SRegistration
    {
        // Timer ID used for this subscription.
        //----------------------------------------
        unsigned int m_uTimerId;

        // Indicates if the next timer event should be EvExpiring.
        // This member is not strictly necessary because we can
        // infer this information from the current time and
        // m_uExpirationTimeMs.
        // However, doing so could lead to EvExpiring being skipped
        // in certain situations, like in overload conditions, in
        // tests and when debugging (i.e., the overall behavior
        // becomes less sensitive to timings).
        //--------------------------------------------------------------
        bool m_bEvExpiring;

        // The contact(s) registered to the registrar.
        //------------------------------------------
        CSipHeader* m_pContact;

        // The time when this registration should be expired.
        //----------------------------------------------------
        uint64_t m_uExpirationTimeMs;

        // Says if there is a timer running related to this registration.
        //----------------------------------------------------------------
        bool m_bTimerStarted;

        // Expiration threshold.
        //----------------------------------------------------
        uint64_t m_uExpiringThresholdMs;

        SRegistration(IN unsigned int uTimerId,
                      IN uint64_t uExpiringThresholdMs)
        :   m_uTimerId(uTimerId),
            m_bEvExpiring(false),
            m_pContact( NULL ),
            m_uExpirationTimeMs(0),
            m_bTimerStarted( false ),
            m_uExpiringThresholdMs(uExpiringThresholdMs)
        {
        }
    };

    // This is used to know what type of request should be sent.
    //-----------------------------------------------------------
    enum ETypeOfRequest
    {
        eADD,
        eGET_LIST,
        eREMOVE,
        eCLEAR
    };


//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipRegistrationSvc();

    static void FinalizeCSipRegistrationSvc();


    //-- << ECOM mechanism >>
    //-------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors/destructors/operators >>
    //--------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipRegistrationSvc( IN IEComUnknown& rOuterIEComUnknown );

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSipRegistrationSvc();


    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  Adds the expires param to 0.
    //-------------------------------
    void SetExpiresParamToZero( INOUT CSipHeader& rContact );

    // Summary:
    //  Sends a REGISTER request with multiple Contact headers.
    //----------------------------------------------------------
    mxt_result RegisterContacts( IN ETypeOfRequest eTypeOfRequest,
                                 IN TO CSipHeader* pContacts,
                                 IN mxt_opaque opqTransaction,
                                 IN TO CHeaderList* pExtraHeaders,
                                 IN TO CSipMessageBody* pMessageBody,
                                 OUT ISipClientTransaction*& rpTransaction);

    // Summary:
    //  Sends the register request.
    //------------------------------------------------------
    mxt_result SendRegister( IN mxt_opaque opqTransaction,
                             IN TO CHeaderList* pExtraHeaders,
                             IN TO CSipMessageBody* pMessageBody,
                             OUT ISipClientTransaction*& rpTransaction );

    // Summary:
    //  Sets the expires value of the contacts included in the last request to
    //  uDefaultExpiresValueInS and adds them in a registration.
    mxt_result ContactsAddedIntoRegistrar(IN uint32_t uDefaultExpiresValueInS,
                                          IN TO CSipHeader* pContacts);

    // Summary:
    //  Removes the contacts included in the last REGISTER request.
    //-------------------------------------------------------------------
    mxt_result ContactsRemovedFromRegistrar(IN const CSipHeader& rContacts);

    // Summary:
    //  Updates the contacts that have been received in the success response.
    //-----------------------------------------------------------------------
    mxt_result UpdateContacts( IN const CHeaderList& rHeaderList,
                               IN uint32_t uDefaultExpiresValueInS,
                               IN uint64_t uActualTimeMs);

    // Summary:
    //  Finds a contact in the managed registrations.
    //-----------------------------------------------
    bool GetRegistrationIndex( IN const CSipHeader& rContact,
                               OUT unsigned int& ruVectorIndex,
                               OUT int& rnHeaderIndex );

    // Summary:
    //  Erases the contact at the given position from the registrations.
    //-------------------------------------------------------------------
    GO CSipHeader* UnlinkContact( IN unsigned int uVectorIndex,
                                  IN int nHeaderIndex );

    // Summary:
    //   Delete contacts from all registrations.
    //-----------------------------------------------------
    void DeleteContacts(IN const CSipHeader* pContacts);

    // Summary:
    //  Empties m_vecRegisteredContacts and stops all the timers.
    //------------------------------------------------------------
    void DeleteAllRegistrations();

    // Summary:
    //  Remove m_vecRegisteredContacts entry and stops its
    //  associated timer.
    //------------------------------------------------------------
    void DeleteRegistration(IN unsigned int uIndex);

    // Summary:
    //  Sets the address field of the contacts to the given address.
    //----------------------------------------------------------------
    mxt_result SetAddress(IN const CHostPort& rAddress, INOUT CSipHeader* pContacts);

    // Summary:
    //  Verifies that all contacts sent in the REGISTER request are received in
    //  the answer. Caution, there are special cases.
    //--------------------------------------------------------------------------
    mxt_result VerifyResponseContacts(IN const CSipPacket& rPacket,
                                      IN const CSipHeader* pLastContacts);

    bool MatchContact(IN const CSipHeader* pHeaderToMatch,
                      IN const CSipHeader* pHeaderToBeMatched);

    //-- << CSipClientSvc >>
    //--------------------------------------------

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext );


    //-- << ISipClientSvc methods >>
    //--------------------------------

    //  Summary:
    //      Sends a request.
    //----------------------
    virtual mxt_result SendRequest(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   IN TO CHeaderList* pExtraHeaders,
                                   IN TO CSipMessageBody* pMessageBody,
                                   IN mxt_opaque opqService,
                                   OUT ISipClientTransaction*& rpClientTransaction,
                                   OUT CSipPacket** ppPacket,
                                   IN ISipForkedDialogGrouperMgr* pGrouperMgr = NULL,
                                   IN const CToken* pCreatingDialogRequestMethod = NULL);

    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);


    //-- << ISipRegistrationSvc >>
    //--------------------------------------------

    //  Summary:
    //      Configures the manager of this service.
    //----------------------------------------------
    virtual mxt_result SetManager(IN ISipRegistrationMgr* pMgr);

    //  Summary:
    //      Sets the delay before expiration at which the EvExpiring event
    //      fires.
    //--------------------------------------------------------------------
    virtual mxt_result SetExpiringThreshold(IN unsigned int uDelayInS,
                                            IN EThresholdType eThresholdType);

    //  Summary:
    //      Sends a REGISTER request with multiple Contact headers.
    //--------------------------------------------------------------
    virtual mxt_result Add(IN TO CSipHeader* pContacts,
                           IN mxt_opaque opqTransaction,
                           IN TO CHeaderList* pExtraHeaders,
                           IN TO CSipMessageBody* pMessageBody,
                           OUT ISipClientTransaction*& rpTransaction);

    //  Summary:
    //      Sends a REGISTER request with multiple Contact headers whose
    //      local address field will be filled automatically, based
    //      on the actual interface used to send the packet.
    //--------------------------------------------------------------------
    virtual mxt_result AddLocalRegistration(IN TO CSipHeader* pContacts,
                                            IN mxt_opaque opqTransaction,
                                            IN TO CHeaderList* pExtraHeaders,
                                            IN TO CSipMessageBody* pMessageBody,
                                            OUT ISipClientTransaction*& rpTransaction);

    //  Summary:
    //      Removes one or more registration bindings from the registrar
    //      server.
    //-------------------------------------------------------------------
    virtual mxt_result Remove(IN TO CSipHeader* pContacts,
                              IN mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              IN TO CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction);

    //  Summary:
    //      Removes all registration bindings from the registrar server.
    //-------------------------------------------------------------------
    virtual mxt_result Clear(IN mxt_opaque opqTransaction,
                             IN TO CHeaderList* pExtraHeaders,
                             IN TO CSipMessageBody* pMessageBody,
                             OUT ISipClientTransaction*& rpTransaction);

    //  Summary:
    //      Requests the list of registered contacts from the registrar server.
    //-------------------------------------------------------------------------
    virtual mxt_result GetList(IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result UpdateContact(IN TO CSipHeader* pContactHdr,
                                     IN unsigned int uNewExpirationS);

    virtual mxt_result SetContactMatchingType(IN unsigned int uContactMatchingType);

    virtual unsigned int GetContactMatchingType() const;

    virtual mxt_result SetRandomReReg(IN bool bRandomReReg);

    // Summary:
    //  Sets to true to ignore parameters in the contact validation of the
    //  REGISTER's response.
    virtual void SetIgnoreContactParamInSuccessResponse(IN bool bIgnoreParam);

    //  Summary:
    //      Sets the reg-id parameter used in Contact headers.
    virtual void SetRegId(IN unsigned int uRegId);

    //  Summary:
    //      Gets the reg-id parameter used in Contact headers.
    virtual unsigned int GetRegId() const;

    //  Summary:
    //      Returns the amount of time before the registration is considered 
    //      expiring.
    virtual unsigned int GetRegistrationExpiration() const;

    //  Summary:
    //      Returns the amount of time before the re-registration takes place
    virtual unsigned int GetReRegistrationTime() const;

    //-- << ISipReqCtxCoreSvc >>
    //--------------------------------------------

    //  Summary:
    //      Informs the request context core service of a received packet.
    //---------------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    //  Summary:
    //      Modifies a packet about to be sent.
    //----------------------------------------
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    //  Summary:
    //      Informs the service that the application is done updating the packet.
    //--------------------------------------------------------------------------
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

    //  Summary:
    //      Lets the service call an event on the application.
    //--------------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    //  Summary:
    //      Lets the service clear the state associated with an event in which
    //      the application has no interest.
    //----------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    //  Summary:
    //      Adds a reference on this request context core service.
    //---------------------------------------------------------------
    virtual unsigned int AddRcsRef();

    //  Summary:
    //      Removes a reference on this request context core service.
    //---------------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


    //-- << ISipCoreSvc >>
    //--------------------------------------------

    //  Summary:
    //      Obtains an instance of an ISipReqCtxCoreSvc to attach to a request
    //      context.
    //------------------------------------------------------------------------
    virtual
    ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                    IN const CSipPacket& rPacket,
                                    IN ISipCoreSvc::EDirection eDirection);

    //  Summary:
    //      Establishes the confidence level for which the service is a good
    //      candidate to own a request context.
    //-----------------------------------------------------------------------
    virtual
    ISipCoreSvc::EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                                IN const CSipPacket& rPacket);

    //  Summary:
    //      Returns the ISipReqCtxCoreSvc instance to use as the owner for a
    //      request context.
    //----------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();


    //-- << ITimerServiceMgr >>
    //--------------------------------------------

    //  Summary:
    //      Notifies the manager that a new timer elapsed or was stopped.
    //------------------------------------------------------------------
    virtual void EvTimerServiceMgrAwaken( IN bool bStopped,
                                          IN unsigned int uTimer,
                                          IN mxt_opaque opq );


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipRegistrationSvc(IN const CSipRegistrationSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipRegistrationSvc& operator=(IN const CSipRegistrationSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:

    // Enum kept to manage events. Those events are : the reception of
    // progress, success and failure responses.
    //-----------------------------------------------------------------
    enum EPacketReceivedEvent
    {
        ePROGRESS_RESPONSE_RECEIVED,
        eSUCCESS_RESPONSE_RECEIVED,
        eFAILURE_RESPONSE_RECEIVED
    };

private:

    // Enum to identify how contacts are managed.
    //---------------------------------------------
    enum EContactManagement {
        eUNDEFINED,
        eREMOTE,
        eLOCAL
    };

    // The events will be sent to this manager
    //-----------------------------------------
    ISipRegistrationMgr* m_pMgr;

    // The request context used to begin the last transaction
    //--------------------------------------------------------
    ISipRequestContext* m_pCurrentRequestContext;

    // The delay before expiration at which the EvExpiring event will fire.
    //----------------------------------------------------------------------
    uint64_t m_uExpiringThresholdMs;

    // The threshold type.
    //---------------------
    EThresholdType m_eThresholdType;

    // A chain of the contacts used to do the last request.
    // (For clarity, this member should only be directly used in
    // SendRequest and OnPacketReceived, except in asserts).
    //------------------------------------------------------------------
    CSipHeader* m_pLastContacts;

    // A vector of the contacts used to do the last request.
    //-------------------------------------------------------
    CVector<SRegistration*> m_vecpstRegisteredContacts;

    // Says if the last contacts are to be removed.
    //-----------------------------------------------
    bool m_bRemoving;

    // Identifies how contacts are managed (local or remote.)
    //-------------------------------------------------------
    EContactManagement m_eContactManagement;

    // Next timer ID to be used for registrations.
    //------------------------------------------------
    unsigned int m_uNextTimerId;

    // Indicates the matching behaviour for contacts received from the server.
    unsigned int m_uContactMatchingType;

    // Indicates the default matching behaviour for contacts.
    static unsigned int ms_uDefaultContactMatchingType;

    // Indicates the use of randomized re-registration time
    bool m_bRandomReReg;

    // The reg-id parameter used in Contact headers.
    unsigned int m_uRegId;

    // The amount of time before the registration is considered expired
    //----------------------------------------------------------------------
    uint64_t m_uExpirationTimeMs;

    // The amount of time before the re-registration happens
    //----------------------------------------------------------------------
    uint64_t m_uReregistrationTimeMs;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  SendRegister
//==
//==============================================================================
//  Summary:
//      Sends a REGISTER request to the parent CSipClientSvc.
//
//  Parameters:
//    opqTransaction:
//      The opaque that is associated with the transaction created.
//
//    pExtraHeaders:
//      A pointer to the extra headers to add to the request.
//      It can be NULL. Ownership is TAKEN.
//
//    pMessageBody:
//      The message-body of the packet to be sent. Ownership is TAKEN.
//
//    rpTransaction:
//      The transaction that is created, to which opqTransaction is attached.
//
//  Returns:
//      See CSipClientSvc::SendRequest
//
//  Description:
//      If there is one or more expires parameter in the Contact header, its
//      value is changed to 0.  if the parameter is not present, it is added to
//      the header.
//
//  See Also:
//      CSipClientSvc::SendRequest
//
//==EDOC========================================================================
inline mxt_result CSipRegistrationSvc::SendRegister(
                                    IN mxt_opaque opqTransaction,
                                    IN TO CHeaderList* pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody,
                                    OUT ISipClientTransaction*& rpTransaction )
{
    CToken tokRegister(CToken::eCS_SIP_HEADER,
                       g_aszMETHOD_NAME[eSIP_METHOD_REGISTER]);

    return SendRequest(IN opqTransaction,
                       IN tokRegister,
                       IN TO pExtraHeaders,
                       IN TO pMessageBody,
                       IN NULL,
                       OUT rpTransaction,
                       OUT NULL);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREGISTRATIONSVC_H
