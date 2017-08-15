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
#ifndef MXG_CSIPSESSIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSESSIONSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT to be able to use this class.
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPAUTOMATICANSWERERREQCTXSVCMGR_H
#include "SipUserAgent/ISipAutomaticAnswererReqCtxSvcMgr.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_ISIPFORKEDDIALOGGROUPERMGR_H
#include "SipCore/ISipForkedDialogGrouperMgr.h"
#endif

#ifndef MXG_ISIPSESSIONSVC_H
#include "SipUserAgent/ISipSessionSvc.h"
#endif

#ifndef MXG_ISIPSESSIONTRANSACTIONCONTROLLER_H
#include "SipUserAgent/ISipSessionTransactionController.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipSessionTransaction;
class ISipForkedDialogGrouper;
class ISipForkedDialogGrouperMgr;
class ISipSessionMgr;
class ISipTransactionStatistics;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipSessionSvc
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//   Front end of the session service as it manages the session state and the
//   session transactions.
//
// Description:
//  This class controls the objects involved in an INVITE session.
//
//  CSipSessionSvc is a ISipCoreSvc that sees incoming packets (such as
//  requests, 2xx retransmissions and forked responses) and determines how to
//  handle them. It also determines if outgoing requests such as INVITE, ACK or
//  BYE are allowed. It uses the session dialog state and a list of
//  transactions to determine if the packet can be sent or received.
//
//  Once the transaction is created, the CSipSessionSvc does not see packets
//  related to the transaction as they are handled directly by
//  CSipSessionTransaction instances.
//
//  It handles the high-level forking APIs by using ISipForkedDialogGrouper and
//  ISipForkedDialogGrouperMgr.
//
// Location:
//   SipUserAgent/CSipSessionSvc.h
//
// See Also:
//   ISipSessionSvc, ISipSessionMgr, CSipSessionTransaction
//
//==============================================================================
class CSipSessionSvc : private CEComDelegatingUnknown,
                       private ISipAutomaticAnswererReqCtxSvcMgr,
                       private ISipCoreSvc,
                       private ISipForkedDialogGrouperMgr,
                       private ISipSessionSvc,
                       private ISipSessionTransactionController
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipSessionSvc();

    static void FinalizeCSipSessionSvc();

//-- Hidden Methods
protected:
private:
    // << ECom Pattern >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    // Inherited from CEComDelegatingUnknown.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Constructors / Destructors / Operators >>

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif
//M5T_INTERNAL_USE_END

    // Summary:
    //  Default Constructor.
    CSipSessionSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipSessionSvc();

    //-- << ISipAutomaticAnswererReqCtxSvcMgr >>

    virtual void EvResponseSent(IN ISipReqCtxCoreSvc* pReqCtxCoreSvc,
                                IN ISipRequestContext* pRequestContext,
                                IN const CSipPacket& rRequest,
                                IN uint16_t uResponseCode,
                                IN const char* const pszReason,
                                IN mxt_opaque opqTransaction);

    virtual void EvResponseNotSent(IN ISipReqCtxCoreSvc* pReqCtxCoreSvc,
                                   IN ISipRequestContext* pRequestContext,
                                   IN const CSipPacket& rRequest,
                                   IN uint16_t uResponseCode,
                                   IN const char* const pszReason,
                                   IN mxt_opaque opqTransaction);

    //-- << ISipCoreSvc >>

    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN ISipCoreSvc::EDirection eDirection);

    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    virtual mxt_result ClearCoreSvc();

    //-- << ISipForkedDialogGrouperMgr >>

    virtual void EvNewDialogNeeded(IN ISipForkedDialogGrouper& rGrouper,
                                   IN const CSipPacket& rPacket,
                                   IN ISipContext& rOriginator);

    //-- << ISipSessionSvc >>

    virtual mxt_result SetManager(IN ISipSessionMgr* pMgr);

    virtual mxt_result Invite(IN mxt_opaque opqTransaction,
                              IN TOA CHeaderList* pExtraHeaders,
                              IN TOA CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result Ack(IN TOA CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody);

    virtual mxt_result Bye(IN mxt_opaque opqTransaction,
                           IN TOA CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody,
                           OUT ISipClientTransaction*& rpTransaction);

    virtual mxt_result HandleOriginalInviteResponseNewSession(IN mxt_opaque opqTransaction,
                                                              IN ISipSessionSvc* pNewSessionSvc,
                                                              IN const CSipPacket& rPacket);

    //-- << ISipSessionTransactionController >>

    virtual ISipSessionMgr* GetSessionMgr();

    virtual bool AllowSendingRequest(IN ISipSessionTransaction* pSessionTransaction,
                                     IN ESipMethod eMethod);

    virtual void EvEstablishSessionDialog(IN ISipSessionTransaction* pSessionTransaction,
                                          IN const CSipPacket& rPacket,
                                          IN ISipUserAgentSvc::EPacketDirection eDirection);

    virtual void EvTerminateSessionDialog(IN ISipSessionTransaction* pSessionTransaction,
                                          IN ESessionDialogTerminationType eType);

    virtual void EvTransactionTerminated(IN ISipSessionTransaction* pSessionTransaction);

    virtual void EvNewSessionTransaction(IN ISipSessionTransaction* pNewSessionTransaction);

    //-- << Helpers >>

    // Summary:
    //  Tells if sending such request is allowed.
    mxt_result IsSendingRequestAllowed(IN ESipMethod eMethod) const;

    // Summary:
    //  Tells if receiving such request is allowed.
    mxt_result IsReceivingRequestAllowed(IN ESipMethod eMethod,
                                         IN const CSipPacket& rRequest,
                                         OUT uint16_t& ruResponseCode,
                                         OUT const char*& rpszReasonPhrase,
                                         OUT GO CHeaderList** ppResponseExtraHeaders);

    // Summary:
    //  Tells if such request is allowed based on the session state.
    mxt_result IsRequestAllowedBySessionState(IN ESipMethod eMethod) const;

    // Summary:
    //  Helper for GetOwnerConfidenceLevel and GetOwnerInstance.
    ISipCoreSvc::EOwnerConfidenceLevel HandlePacket(IN const CSipPacket& rPacket,
                                                    IN bool bHandle,
                                                    OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Creates and initializes a ISipSessionTransaction.
    void CreateSessionTransaction(IN const mxt_clsid clsid,
                                  OUT ISipSessionTransaction*& rpSessionTransaction);

    // Summary:
    //  Initializes a ISipSessionTransaction and keeps it in the list.
    void ConfigureAndKeepSessionTransaction(IN ISipSessionTransaction* pSessionTransaction);

    // Summary:
    //  Releases the CSipAutomaticAnswererReqCtxSvc reference we own.
    void ReleaseAutomaticAnswerer(IN ISipReqCtxCoreSvc* pReqCtxCoreSvc);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipSessionSvc(IN const CSipSessionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipSessionSvc& operator=(IN const CSipSessionSvc& rSrc);


//-- Hidden Data Members
protected:
    // The session service event manager implemented by the application.
    ISipSessionMgr* m_pMgr;

    struct SForkedDialogGrouperData
    {
        SForkedDialogGrouperData(IN ISipForkedDialogGrouper& rForkedDialogGrouper,
                                 IN const CSipPacket& rRelatedPacket);

        ~SForkedDialogGrouperData();

        ISipForkedDialogGrouper* m_pForkedDialogGrouper;

        // The packet related to m_pForkedDialogGrouper. This serves as the key
        // to find the right m_pForkedDialogGrouper.
        const CSipPacket* m_pRelatedPacket;
    };

    // The forked dialog grouper received in EvNewDialogNeeded and their related
    // packet. This is kept until HandleOriginalInviteResponseNewSession is
    // called for the packet. It is only used when a forked response has been
    // received but not managed yet. There could be many different
    // ISipForkedDialogGrouper instances if this transaction is ended with a 2xx
    // and the dialog is terminated on this context but the context is not
    // cleared yet. In this case, the additional forked 2xx responses would not
    // be received on the same context so the ISipForkedDialogGrouper instance
    // would be different.
    CList<SForkedDialogGrouperData*>* m_plststForkedDialogGrouper;

    // Summary:
    //  This enum contains the states of the session.
    //
    // Description:
    //  The states of the session.
    enum ESessionSvcDialogState
    {
        // Description:
        //  The session dialog was not established. No 1xx or 2xx to initial
        //  INVITE has been sent/received or we are in a state where it is
        //  possible to re-issue the original request (a session dialog could
        //  have been established by a 1xx and terminated by final negative
        //  response to a previous INVITE).
        eNOT_ESTABLISHED,
        // Description:
        //  Established. The 1xx or 2xx to initial INVITE has been sent or
        //  received.
        eESTABLISHED,
        // Description:
        //  Terminated. A 2xx to a BYE request was sent/received or an incoming
        //  initial INVITE was rejected.
        eTERMINATED
    };

    // The state of the session.
    ESessionSvcDialogState m_eState;

    // This tells if the session dialog is still active after an
    // ISipSessionTransaction delayed its termination. This information is
    // complementary to m_eState because there could be many states resulting
    // when the session dialog is terminated.
    bool m_bDelayedSessionDialogTermination;

    // The transactions of this session.
    CVector<ISipSessionTransaction*> m_vecpSessionTransaction;

    // This member is only kept when assertions are enabled for these reasons:
    //  - The pointers are only kept to be released.
    //  - We will always get an event from CSipAutomaticAnswererReqCtxSvc. We
    //    could add a reference at creation and release it when a final event is
    //    called.
    //  - When assertions are enabled, this member is used to assert that all
    //    CSipAutomaticAnswererReqCtxSvc instances are always released before
    //    termination.
    MX_ASSERT_ONLY(CList<ISipReqCtxCoreSvc*>* m_plstpAutomaticAnswerer;)
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  IsRequestAllowedBySessionState
//==
//==============================================================================
//
//  Summary:
//      Tells if such request is allowed based on the session dialog state.
//
//  Parameters:
//      eMethod:
//       The SIP method.
//
//  Returns:
//      - resS_OK: The request is allowed.
//      - resFE_INVALID_STATE: The request is not allowed.
//
//  Description:
//      Uses the session state to determine if the request is allowed. This is
//      used both for sending and receiving INVITE or BYE requests.
//
//      This method is used in addition to the input from the
//      CSipSessionTransaction.
//
//==============================================================================
inline mxt_result CSipSessionSvc::IsRequestAllowedBySessionState(IN ESipMethod eMethod) const
{
    // This method must be used for INVITE and BYE requests as the ACK is a
    // special case.
    MX_ASSERT(eMethod == eSIP_METHOD_INVITE ||
              eMethod == eSIP_METHOD_BYE);

    // Rules:
    // - Allow INVITE (reception and sending) until the session dialog is
    //   eTERMINATED.
    // - Allow BYE (reception and sending) if the session dialog is
    //   eESTABLISHED.
    //
    // State/method | eNOT_ESTABLISHED | eESTABLISHED | eTERMINATED
    // INVITE       | true             | true         | false
    // BYE          | false            | true         | false

    mxt_result res = resFE_INVALID_STATE;

    if (// INVITE
        (m_eState != eTERMINATED &&
         (eMethod == eSIP_METHOD_INVITE ||
          // BYE
          m_eState == eESTABLISHED)))
    {
        res = resS_OK;
    }

    return res;
}

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSIPSESSIONSVC_H
