//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPTRANSACTION_H
#define MXG_CSIPTRANSACTION_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING
#endif

//-- Data Members
//-----------------
#ifndef MXG_ISIPTRANSACTIONMGR_H
#include "SipTransaction/ISipTransactionMgr.h" // ETransactionRole
#endif

#ifndef MXG_METHODS_H
#include "SipParser/Method.h" // ESipMethod
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_HEADERDEFINITION_H
#include "SipParser/HeaderDefinition.h" // ESipHeaderType
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPTRANSPORTUSER_H
#include "SipTransport/ISipTransportUser.h"
#endif

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING
    #ifndef MXG_CWATCHEDINSTANCE_H
    #include "SipTransport/CWatchedInstance.h"
    #endif
#endif

//-- Inline usage
//-----------------
#ifndef MXG_CSIPPACKET_H
#include "SipTransport/CSipPacket.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CMarshaler;
class CSipHeader;
class CSipTransactionMgr;
class CToken;
class ISipTransportMgr;
class ISipTransactionUser;
class ISipTransactionStatistics;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// <TITLE RFC 3261 Transaction Timers>
//
// Description:
//  Timer defined in Table 4 of RFC 3261.
//
//------------------------------------------------------------------------------
const unsigned int uDEFAULT_T1_MS = 500;

// <COMBINE uDEFAULT_T1_MS>
const unsigned int uDEFAULT_T2_MS = 4000;

// <COMBINE uDEFAULT_T1_MS>
const unsigned int uDEFAULT_T4_MS = 5000;


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: CSipTransaction
//========================================
//
// Description:
//   This class encapsulates a SIP transaction as defined in RFC 3261 or RFC
//   2543.
//
//   Some data members are kept in this class to be able to process incoming
//   requests constructed according to RFC 2543 rules.
//
//   The transaction creates, manages, and stores the key associated with
//   each transaction. The keys are generated with elements specific for each
//   type of transaction: client, server RFC 3261 compliant, and server RFC 2543
//   compliant.
//
//   The following are the keys content for every type of transaction:
//
//   Client Transaction:            top Via branch + CSeq method
//   RFC 3261 Server Transaction:   top Via branch + top Via sent-by
//   RFC 2543 Server Transaction:   Request-URI + from tag + Call-ID + top Via
//                                  + CSeq number
//
//   Note that the sent-by of the top Via is required in server transaction
//   matching process because there could be accidental or malicious duplication
//   of branch parameters from different clients. This is in accordance to RFC
//   3261 in section 17.2.3.
//
// Location:
//   SipTransaction/CSipTransaction.h
//
// See Also:
//   ITransactionMgr
//
//==EDOC========================================================================
class CSipTransaction : public ISipTransportUser,
                        public CEventDriven
#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING
                        , public CWatchedInstance<CSipTransaction>
#endif
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
#ifdef MXD_SIPSTACK_ENABLE_SIPTRANSACTION_INSTANCE_TRACKING
    friend class CInstanceTracker;      // To access the Dump method.
#endif
#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING
    friend class CDeprecatedInstanceTracker; // To access the Dump method.
#endif
    //M5T_INTERNAL_USE_END

//-- New types within class namespace
public:

    // Enum describing the Via header matching method for Via headers
    // that are RFC2543 compatible.
    enum ERfc2543ViaHeaderMatchingMethod
    {
        // The Via headers are compared byte by byte as recommended
        // by RFC2543.
        eSTANDARD_VIA_MATCHING_METHOD,
        // Only the sent-by parameters of the Via headers are compared.
        // This allows matching an incoming request even if some parameters
        // of the Via header have changed. For example, matching a CANCEL
        // or an ACK to a server INVITE.
        eSENTBY_ONLY_VIA_MATCHING_METHOD
    };

protected:

    // Enum describing the RFC compatibility of the transaction.
    //-----------------------------------------------------------
    enum ETransactionRfcCompatibility
    {
        // RFC 3261 Compatible, contains magic cookie.
        //---------------------------------------------
        eRFC3261_COMPATIBILITY,
        // RFC 2543 Compatible, no magic cookie.
        //---------------------------------------
        eRFC2543_COMPATIBILITY,
        // Could not find compatibility, no Via header.
        //----------------------------------------------
        eUNKNOWN_COMPATIBILITY
    };


    struct SAsyncTransportData
    {
        const CSipPacket*   m_pSentPacket;
        bool                m_bNotifyTransactionUser;
        bool                m_bIsRetransmission;
        bool                m_bSourceIsTransaction;
        bool                m_bNotifyTransaction;

        SAsyncTransportData()
            :m_pSentPacket(NULL),
             m_bNotifyTransactionUser(false),
             m_bIsRetransmission(false),
             m_bSourceIsTransaction(false),
             m_bNotifyTransaction(true)
            {}
        ~SAsyncTransportData()
        {
            if (m_pSentPacket != NULL)
            {
                m_pSentPacket->Release();
                m_pSentPacket = NULL;
            }
        }
    };


//-- Published Interface
//------------------------
public:

    //-- << Configuration >>
    //--------------------------------------------

    // Summary:
    //  Sets the global T1 timer value.
    //---------------------------------
    static void SetGlobalT1(IN unsigned int uT1Ms);

    // Summary:
    //  Gets the global T1 timer value.
    //--------------------------------
    static unsigned int GetGlobalT1();

    // Summary:
    //  Sets the global T2 timer value.
    //---------------------------------
    static void SetGlobalT2(IN unsigned int uT2Ms);

    // Summary:
    //  Gets the global T2 timer value.
    //---------------------------------
    static unsigned int GetGlobalT2();

    // Summary:
    //  Sets the global T4 timer value.
    //---------------------------------
    static void SetGlobalT4(IN unsigned int uT4Ms);

    // Summary:
    //  Gets the global T4 timer value.
    //--------------------------------
    static unsigned int GetGlobalT4();

    // Summary:
    //  Sets the timeout timer.
    //--------------------------
    static void SetTimeoutTimer(IN unsigned int uTimeoutMs);

    // Summary:
    //  Gets the timeout timer.
    //--------------------------
    static unsigned int GetTimeoutTimer();

    // Summary:
    //  Sets the RFC 2543 Via header matching method.
    static void SetRfc2543ViaHeaderMatchingMethod(
                            IN ERfc2543ViaHeaderMatchingMethod eMatchingMethod);

    //-- << Stereotype >>
    //--------------------------------------------

    // Summary:
    //  Returns the last packet sent. It may be NULL if no packet
    //  was sent.
    //-------------------------------------------------------------------
    const CSipPacket* GetLastPacketSent() const;

    // Summary:
    //  Tells if the packet matches the transaction according to RFC 2543 or
    //  3261 matching rules.
    //-----------------------------------------------------------------------
    bool Match(IN const CSipPacket& rPacket, IN ESipMethod eMethod);

    // Summary:
    //  Verifies if the request is a merged request.
    bool IsMergedRequest(IN const CSipPacket& rPacket);

    // Summary:
    //  Tells if a CANCEL request matches the transaction.
    //-----------------------------------------------------
    bool MatchCancel(IN const CSipPacket& rPacket, IN ESipMethod eMethod);

    // Summary:
    //  Computes the transaction according to the packet.
    //----------------------------------------------------
    static unsigned int CreateKey(IN const CSipPacket& rPacket,
                                  IN ISipTransactionMgr::ETransactionRole eRole,
                                  IN ESipMethod eMethod);
    // Summary:
    //  Calling this method SHOULD result in a (self-) state transition.
    //-------------------------------------------------------------------
    void EvProcessPacketA(IN const CSipPacket& rPacket);

    // Summary:
    //  Sets the original state for a server transaction.
    //----------------------------------------------------
    virtual void ProcessOriginalRequest(IN const CSipPacket& rPacket);

    // Summary:
    //  Calls asynchronously EvRequestCancelled on the Transaction User.
    //-------------------------------------------------------------------
    void EvRequestCancelledA(IN const CSipPacket& rPacket);

    //  Summary:
    //   Sends the pPacket to the ISipTransportMgr.
    //----------------------------------------------
    virtual mxt_result Send(IN const CSipPacket& rPacket,
                            IN bool bNotifyTransactionUser,
                            IN bool bRetransmission,
                            IN bool bSourceIsTransaction);

    // Summary:
    //  Sends the packet without updating the last packet sent.
    //----------------------------------------------------------
    mxt_result StatelessSend(IN const CSipPacket& rPacket,
                             IN bool bNotifyTransactionUser,
                             IN bool bRetransmission,
                             IN bool bSourceIsTransaction,
                             IN bool bNotifyTransaction);

    // Summary:
    //  Tells the transaction that an event should no longer occur on the
    //  Transaction User.
    //---------------------------------------------------------------------
    void InvalidateTransactionUser();

    // Summary:
    //   Gets the associated Key for this transaction.
    //-------------------------------------------------
    unsigned int GetKey();


    // Summary:
    //  Sets the transport and transaction managers.
    //-----------------------------------------------
    static void SetManagers(IN ISipTransportMgr& rTransportMgr,
                            IN CSipTransactionMgr& rTransactionMgr);

    // Summary:
    //  Sets the transaction statistics on the transaction.
    //------------------------------------------------------
    void SetTransactionStatistics(IN ISipTransactionStatistics* pTransStats);

// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    void SetGuidOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
    void SetTosOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add TOS config support

    // Summary:
    //  Stops the retransmissions.
    virtual mxt_result StopRetransmission();

    //==SDOC========================================================================
    //==
    //==  EvTransportError
    //==
    //==============================================================================
    //
    //  Description:
    //    Called when a transport error occurs that affect this transaction.
    //    A transport error affects a transaction if its last packet
    //    sent has the same destination and transport as the
    //    error.
    //
    //==EDOC========================================================================
    virtual void EvTransportError(IN mxt_result res) = 0;

    //-- << ISipTransportUser >>
    //---------------------------

    // Summary:
    //  Gives feedback to the transaction once the sending result is known.
    //----------------------------------------------------------------------
    virtual void EvCommandResult(IN mxt_result res,
                                 IN mxt_opaque opq);

    //-- << IMessageServiceMgr >>
    //-----------------------------

    // Summary:
    //  Dispatches messages.
    //-----------------------
    void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                   IN unsigned int uMessage,
                                   IN CMarshaler* pParameter);

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipTransaction(IN ISipTransactionUser* pTransactionUser,
                    IN const CSipPacket& rPacket,
                    IN ISipTransactionMgr::ETransactionRole eRole,
                    IN ESipMethod eMethod,
                    IN ISipTransactionStatistics* pTransStats,
                    IN const ISipTransactionMgr::STransactionsTimers* pstTransactionTimers);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipTransaction();

    //-- << CEventDriven >>
    //-------------------------------------------------
    virtual void ReleaseInstance();

    //-- << Helpers >>
    //-------------------------------------------------

    // Summary:
    //  Determines if the packet is RFC 3261 or RFC 2543 compatible and returns
    //  the Via branch if it is RFC 3261 compatible.
    //--------------------------------------------------------------------------
    static ETransactionRfcCompatibility
        GetRfcCompatibilityHelper(IN    const CSipPacket& rPacket,
                                  INOUT const CString*&   rpstrTopViaBranch);

    // Summary:
    //  Gets the Via sent-by value. Used for RFC 3261 server transactions.
    //---------------------------------------------------------------------
    static CString& GetViaSentByHelper(IN    const CSipPacket& rPacket,
                                       INOUT CString&          rstrViaSentBy);

    // Summary:
    //  Gets the Request-URI value. Used for RFC 2543 server transactions.
    //---------------------------------------------------------------------
    static CString& GetRequestUriHelper(IN    const CSipPacket& rPacket,
                                        INOUT CString&          rstrRequestUri);

    // Summary:
    //  Gets the tag parameter for the specified header: To or From. Only
    //  useful for RFC 2543 server transactions.
    //--------------------------------------------------------------------
    static CString& GetTagParamHelper(IN    const CSipPacket& rPacket,
                                      IN    ESipHeaderType    eType,
                                      INOUT CString&          rstrTagParam);

    // Summary:
    //  Tests the tag equivalence.
    //-----------------------------
    static bool IsTagEquivalent(IN const CString&    rstrTag,
                                IN const CSipPacket& rPacket,
                                IN ESipHeaderType    eType);

    // Summary:
    //  Gets the Call-ID value. Used for RFC 2543 server transactions.
    //-----------------------------------------------------------------
    static CString& GetCallIdHelper(IN    const CSipPacket& rPacket,
                                    INOUT CString&          rstrCallId);

    // Summary:
    //  Tests the Call-ID equivalence.
    //---------------------------------
    static bool IsCallIdEquivalent(INOUT CString&          rstrCallId,
                                   IN    const CSipPacket& rPacket);

    // Summary:
    //  Gets the Top Via Header value. Used for RFC 2543 server transactions.
    //------------------------------------------------------------------------
    static CString& GetTopViaHelper(IN    const CSipPacket& rPacket,
                                    INOUT CString&          rstrTopVia);

    // Summary:
    //  Hash top Via branch and CSeq method.
    //---------------------------------------
    static unsigned int CreateKeyClientRfc3261Helper(
                                                   IN const CString& rstrTopViaBranch,
                                                   IN ESipMethod eMethod);

    // Summary:
    //  Hash top Via branch and top Via sent-by.
    //-------------------------------------------
    static unsigned int CreateKeyServerRfc3261Helper(
                                                  IN const CSipPacket& rPacket,
                                                  IN const CString& rstrTopViaBranch);

    // Summary:
    //  Hash Request-URI, From Tag, Call-ID, top Via, and CSeq number.
    //-----------------------------------------------------------------
    static unsigned int CreateKeyServerRfc2543Helper(
                                                  IN const CSipPacket& rPacket);

    // Summary:
    //  Called when a packet is processed asynchronously. Defined by child
    //  classes.
    //----------------------------------------------------------------------
    virtual void InternalEvProcessPacketA(IN const CSipPacket& rPacket) = 0;

    // Summary:
    //  Called asynchronously upon a CANCEL request.
    //-----------------------------------------------
    void InternalEvRequestCancelledA(IN const CSipPacket& rPacket);

    // Summary:
    //  Called asynchronously upon feedback received from the transport.
    //-------------------------------------------------------------------
    virtual void InternalEvCommandResultA(IN mxt_result res,
                                          IN const SAsyncTransportData* pAsyncData) = 0;

    //  Summary:
    //   Retransmits the last packet that was sent.
    //----------------------------------------------
    mxt_result Retransmit();

    // Summary:
    //  Generates a response from the last packet sent. Used by client state
    //  machines to generate a final negative local response.
    //-----------------------------------------------------------------------
    void GenerateFinalResponseLocally(IN unsigned int uCode,
                                      IN  const char* szReason,
                                      IN mxt_result res);

    // Summary:
    //  Tells whether or not the packet matches the server transaction
    //  according to RFC 2543 matching rules.
    //-----------------------------------------------------------------
    virtual bool MatchRfc2543Helper(IN const CSipPacket& rPacket);

    // Summary:
    //  Tells whether or not the packet matches the server transaction
    //  according to RFC 3261 matching rules.
    //-----------------------------------------------------------------
    virtual bool MatchRfc3261ServerHelper(IN const CSipPacket& rPacket,
                                          IN const CString* pstrTopViaBranch);

    // Summary:
    //  Called when the transaction is about to terminate.
    //-----------------------------------------------------
    void TerminateTransaction();

    // Summary:
    //  Called by ReleasionFirstStep when the client transaction is about to be
    //  released.
    //--------------------------------------------------------------------------
    void ClientRelease();

    // Summary:
    //  Called by ReleasionFirstStep when the server transaction is about to be
    //  released.
    //--------------------------------------------------------------------------
    void ServerRelease();

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING
    //-- << CWatchedInstance Mechanism >>

    // Summary:
    //  Gets a CBlob containing information about the CSipTransaction still
    //  allocated.
    static void Dump(OUT CBlob& rblobTransactionDump);
#endif // #if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING

private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipTransaction();

    static void FinalizeCSipTransaction();


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipTransaction(IN const CSipTransaction& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipTransaction& operator=(IN const CSipTransaction& rSrc);


//-- Hidden Data Members
//------------------------
protected:
    // This is the interface with the SipTransport, from the packets are sent,
    // and network event are publicised.
    //-------------------------------------------------------------------------
    static ISipTransportMgr* ms_pTransportMgr;

    // This is the interface to which the state machines notify their
    // termination.
    //----------------------------------------------------------------
    static CSipTransactionMgr* ms_pTransactionMgr;

    // The global value used for T1.
    //--------------------------------
    static unsigned int ms_uGlobalT1Ms;

    // The global value used for T2.
    //--------------------------------
    static unsigned int ms_uGlobalT2Ms;

    // The global value used for T4.
    //--------------------------------
    static unsigned int ms_uGlobalT4Ms;

    // The current value used to timeout transactions.
    //-------------------------------------------------
    static unsigned int ms_uTimeoutTimerMs;

    // Indicates which Via header matching method to use.
    static ERfc2543ViaHeaderMatchingMethod ms_eRfc2543ViaHeaderMatchingMethod;

    // This is the interface used by state machines to inform the RFC-3261
    // Transaction User.
    //---------------------------------------------------------------------
    ISipTransactionUser* m_pTransactionUser;

    // The transport used for sending the packet. Used mostly to know if the
    // transport is connection oriented or not.
    //-----------------------------------------------------------------------
    ESipTransport m_eTransport;

    // This is the last packet that was sent by the state machine.
    //-------------------------------------------------------------
    const CSipPacket* m_pLastPacketSent;

    // This tells the state machine for how many EvCommandResult from the
    // SipTransport. This is zero when we are not waiting for any SipTransport's
    // callback. Otherwise, this is non-zero. It is incremented when SendA is
    // called on the transport without returning any error. It is decremented in
    // InternalEvCommandResultA.
    //--------------------------------------------------------------------------
    unsigned int m_uPendingResults;

    // The transaction key.
    //----------------------
    unsigned int m_uKey;

    // The role of this transaction: client or server.
    //-------------------------------------------------
    ISipTransactionMgr::ETransactionRole m_eRole;

    // The SIP method found in the CSeq header. This will be used to match
    // incoming requests (INVITE or non-INVITE constructed according to RFC
    // 2543 and RFC 3261 CANCEL) to server transactions.
    //----------------------------------------------------------------------
    ESipMethod m_eMethod;

    // The peer IP and port address. This will be used only to match CANCEL and
    // ACK requests.
    //--------------------------------------------------------------------------
    CSocketAddr m_peerAddr;

    // The RFC compatibility: RFC 3261 or 2543.
    //------------------------------------------
    ETransactionRfcCompatibility m_eRfcCompatibility;

    // The top Via branch parameter. Used to match RFC 3261 server and client
    // transaction.
    //------------------------------------------------------------------------
    CString m_strViaBranch;

    // The top Via sent-by parameter. Used to match RFC 3261 server
    // transaction.
    //--------------------------------------------------------------
    CString m_strViaSentby;

    // The Request-URI, used to match RFC 2543 server transaction.
    //-------------------------------------------------------------
    CString m_strRequestUri;

    // The To Tag of this transaction. This is used only to be backward
    // compatible with RFC2543 to match incoming requests (INVITE or non-INVITE
    // constructed according to RFC 2543) to server transactions. This is also
    // set when the Transaction User sends a >= 3xx response containing a To
    // Tag so it can later be used to match incoming ACK requests (constructed
    // according to RFC 2543) to server transactions.
    //--------------------------------------------------------------------------
    CString m_strToTag;

    // The From tag, used to match RFC 2543 server transaction.
    //----------------------------------------------------------
    CString m_strFromTag;

    // The Call-ID, used to match RFC 2543 server transaction.
    //---------------------------------------------------------
    CString m_strCallId;

    // The top Via header, used to match RFC 2543 server transaction.
    //----------------------------------------------------------------
    CString m_strTopVia;

    // The CSeq number, used to match RFC 2543 server transaction.
    //-------------------------------------------------------------
    CString m_strCseqNumber;

    // The username of the received SIP request-URI if merge detection is
    // enabled.
    //--------------------------------------------------------------------
    CToken* m_pUser;

    // A copy of the received CSeq header merge detection is enabled.
    //----------------------------------------------------------------
    CSipHeader* m_pHdrCSeq;

    // Pointer to the transaction statistic.
    //---------------------------------------
    ISipTransactionStatistics* m_pTransStats;
   
    // The current value used for T1.
    //-------------------------------
    unsigned int m_uT1Ms;

    // The current value used for T2.
    //-------------------------------
    unsigned int m_uT2Ms;

    // The current value used for T4.
    //-------------------------------
    unsigned int m_uT4Ms;

    // Tells if a failure must be notified for the current packet. This member
    // makes sure that we do not report a failure twice for the same packet.
    //-------------------------------------------------------------------------
    bool m_bNotifySendingFailure;

// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    // This opaque stores the GUID of a CCTK resource.
    //-------------------------------------------------
    mxt_opaque m_opqGuid;
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
    // This opaque stores the TOS value associated to this transaction.
    //------------------------------------------------------------------
    mxt_opaque m_opqTos;
// BRCM_CUSTOM - [end] Add TOS config support

private:
    // Message IDs.
    //--------------
    enum EMessage
    {
        eEVPROCESSPACKET,
        eEVREQUESTCANCELLED,
        eEVCOMMANDRESULT
    };

    // This tells if we must release the transaction when the last result is
    // received from the transport.
    //-----------------------------------------------------------------------
    bool m_bReleaseWaitingForPendingResults;

    // Indicates if a non-trying response has been sent to transport layer.
    //----------------------------------------------------------------------
    bool m_bNonTryingResponseBeingSent; 
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==SDOC========================================================================
//==
//==  SetGlobalT1
//==
//==============================================================================
//
//  Parameters:
//    uT1Ms:
//      The initial retransmission interval to use.
//
//  Description:
//      Changes the initial retransmission interval for unreliable transports.
//      It corresponds to the T1 timer defined in section A at the end of RFC
//      3261.
//
//      By default, it is set to uDEFAULT_T1_MS ms. You can change this value
//      when you know that the RTT is larger than the default value for your
//      network. Note that RFC 3261 allows implementations to lower or increase
//      this value though it does not recommend lowering it.
//
//      Note that when modifying this value, you also need to call SetGlobalT2 and
//      SetTimeoutTimer unless you want to affect the number of
//      retransmissions.
//
//      Note also that the value for the T1 timer is an unsigned 32 bits
//      integer. This is to be uniform with the timeout timer that
//      eventually needs 32 bits to represent its value.
//
//  See Also:
//      SetGlobalT2, SetTimeoutTimer
//
//==EDOC========================================================================
inline void CSipTransaction::SetGlobalT1(IN unsigned int uT1Ms)
{
    ms_uGlobalT1Ms = uT1Ms;
}

//==SDOC========================================================================
//==
//==  GetGlobalT1
//==
//==============================================================================
//
//  Returns:
//      The value of the retransmission interval to use.
//
//  Description:
//      Gets the initial retransmission interval for unreliable transports. It
//      corresponds to the T1 timer defined in section A at the end of RFC 3261.
//
//      By default, it is set to uDEFAULT_T1_MS ms.
//
//  See Also:
//      SetGlobalT1
//
//==EDOC========================================================================
inline unsigned int CSipTransaction::GetGlobalT1()
{
    return ms_uGlobalT1Ms;
}

//==SDOC========================================================================
//==
//==  SetGlobalT2
//==
//==============================================================================
//
//  Parameters:
//    uT2Ms:
//      The maximum retransmission interval.
//
//  Description:
//      Changes the maximum retransmission interval for unreliable transports.
//      It corresponds to the T2 timer defined in section A at the end of RFC
//      3261.
//
//      By default, it is set to uDEFAULT_T2_MS ms. Retransmissions of certain
//      types of SIP packets (namely final responses to INVITE and requests
//      other than INVITE or ACK) are retransmitted at an initial rate of T1 ms,
//      doubling each time until the interval reaches T2 ms. Then, the interval
//      stays at T2 ms until the timeout timer starts or an ACK or response is
//      received.
//
//      Note that when modifying this value, you also need to call SetGlobalT1
//      and SetTimeoutTimer unless you want to affect the number of
//      retransmissions.
//
//      Note also that the value for the T2 timer is an unsigned 32 bits
//      integer. This is to be uniform with the timeout timer that
//      eventually needs 32 bits to represent its value.
//
//  See Also:
//      SetGlobalT1, SetTimeoutTimer
//
//==EDOC========================================================================
inline void CSipTransaction::SetGlobalT2(IN unsigned int uT2Ms)
{
    ms_uGlobalT2Ms = uT2Ms;
}

//==SDOC========================================================================
//==
//==  GetGlobalT2
//==
//==============================================================================
//
//  Returns:
//      The value of the maximum retransmission interval to use.
//
//  Description:
//      Gets the maximum retransmission interval for unreliable transports. It
//      corresponds to the T2 timer defined in section A at the end of RFC 3261.
//
//      By default, it is set to uDEFAULT_T2_MS ms.
//
//  See Also:
//      SetGlobalT2
//
//==EDOC========================================================================
inline unsigned int CSipTransaction::GetGlobalT2()
{
    return ms_uGlobalT2Ms;
}

//==SDOC========================================================================
//==
//==  SetGlobalT4
//==
//==============================================================================
//
//  Parameters:
//    uT4Ms:
//      The maximum duration a message remains in the network.
//
//  Description:
//      Changes the maximum duration a message remains in the network for 
//      unreliable transports. It corresponds to the T4 timer defined in section 
//      A at the end of RFC 3261.
//
//      Note also that the value for the T4 timer is an unsigned 32 bits
//      integer. This is to be uniform with the timeout timer that
//      eventually needs 32 bits to represent its value.
//
//  See Also:
//      SetGlobalT4
//
//==EDOC========================================================================
inline void CSipTransaction::SetGlobalT4(IN unsigned int uT4Ms)
{
    ms_uGlobalT4Ms = uT4Ms;
}

//==SDOC========================================================================
//==
//==  GetGlobalT4
//==
//==============================================================================
//
//  Returns:
//      The value of the maximum duration a message remains in the network.
//
//  Description:
//      Gets the maximum duration a message remains in the network for 
//      unreliable transports. It corresponds to the T4 timer defined in 
//      section A at the end of RFC 3261.
//
//      By default, it is set to uDEFAULT_T4_MS ms.
//
//  See Also:
//      SetGlobalT4
//
//==EDOC========================================================================
inline unsigned int CSipTransaction::GetGlobalT4()
{
    return ms_uGlobalT4Ms;
}

//==SDOC========================================================================
//==
//==  SetTimeoutTimer
//==
//==============================================================================
//
//  Parameters:
//    uTimeoutMs:
//      The timeout timer.
//
//  Description:
//      Changes the time to wait for a response or an ACK before considering
//      a transaction timed out. It corresponds to timer B, F, and H for all
//      transport protocols and timer J for UDP. These timers are defined in
//      section A at the end of RFC 3261.
//
//      By default, it is set to 64*T1.
//
//      Note that when modifying this value, you also need to call SetGlobalT1 and
//      SetGlobalT2 unless you want to affect the number of retransmissions.
//
//      Note also that the value for the timeout timer is an unsigned 32 bits
//      integer. Its value can be considerably high so eventually, it must
//      be represented on 32 bits.
//
//  See Also:
//      SetGlobalT1, SetGlobalT2
//
//==EDOC========================================================================
inline void CSipTransaction::SetTimeoutTimer(IN unsigned int uTimeoutMs)
{
    ms_uTimeoutTimerMs = uTimeoutMs;
}

//==SDOC========================================================================
//==
//==  GetTimeoutTimer
//==
//==============================================================================
//
//  Returns:
//      The value of the timeout timer.
//
//  Description:
//      Gets the time to wait for a response or an ACK before considering
//      a transaction timed out. It corresponds to timer B, F, and H for all
//      transport protocols and timer J for UDP. These timers are defined in
//      section A at the end of RFC 3261.
//
//      By default, it is set to 64*T1.
//
//  See Also:
//      SetTimeoutTimer
//
//==EDOC========================================================================
inline unsigned int CSipTransaction::GetTimeoutTimer()
{
    return ms_uTimeoutTimerMs;
}

//==SDOC========================================================================
//==
//==  SetRfc2543ViaHeaderMatchingMethod
//==
//==============================================================================
//
//  Parameters:
//    eRfc2543ViaHeaderMatchingMethod:
//      The RFC 2543 Via header matching method.
//
//  Description:
//      Sets the RFC 2543 Via header matching method.
//
//==EDOC========================================================================
inline void CSipTransaction::SetRfc2543ViaHeaderMatchingMethod(
             IN ERfc2543ViaHeaderMatchingMethod eRfc2543ViaHeaderMatchingMethod)
{
    ms_eRfc2543ViaHeaderMatchingMethod = eRfc2543ViaHeaderMatchingMethod;
}

//==SDOC========================================================================
//==
//==  InvalidateTransactionUser
//==
//==============================================================================
//
//  Description:
//      Tells the transaction that an event should no longer occur on the 
//      Transaction User because the SipCore asked to terminate the transaction.
//
//==EDOC========================================================================
inline void CSipTransaction::InvalidateTransactionUser()
{
    m_pTransactionUser = NULL;
}

//==SDOC========================================================================
//==
//==  ProcessOriginalRequest
//==
//==============================================================================
//
//  Parameters:
//      rPacket:
//       The received packet that needs to be dealt within the state machine.
//
//  Returns:
//      The result of the packet processing (implementation dependent).
//
//  Description:
//      Sets the original state for a server transaction.
//
//==EDOC========================================================================
inline void CSipTransaction::ProcessOriginalRequest(
                                                   IN const CSipPacket& rPacket)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(rPacket);
}

//==SDOC========================================================================
//==
//==  GetKey
//==
//==============================================================================
//
//  Returns:
//      The transaction key.
//
//  Description:
//      Gets the key.
//
//==EDOC========================================================================
inline
unsigned int CSipTransaction::GetKey()
{
    return m_uKey;
}

//==SDOC========================================================================
//==
//==  SetManagers
//==
//==============================================================================
//
//  Parameters:
//    rTransportMgr:
//      The transport manager.
//
//    rTransactionMgr:
//      The transaction manager.
//
//  Description:
//      Sets the transport and transaction managers.
//
//==EDOC========================================================================
inline
void CSipTransaction::SetManagers(IN ISipTransportMgr& rTransportMgr,
                                  IN CSipTransactionMgr& rTransactionMgr)
{
    ms_pTransportMgr = &rTransportMgr;
    ms_pTransactionMgr = &rTransactionMgr;
}

//==SDOC========================================================================
//==
//==  GetLastPacketSent
//==
//==============================================================================
//
//  Returns:
//    The last packet sent. It may be NULL if no packet was sent.
//
//  Description:
//    Returns the last packet sent.
//
//==EDOC========================================================================
inline
const CSipPacket* CSipTransaction::GetLastPacketSent() const
{
    return m_pLastPacketSent;
}

// BRCM_CUSTOM - [add] Add CCTK GUID opaque
inline
void CSipTransaction::SetGuidOpaque(IN mxt_opaque opq)
{
    m_opqGuid = opq;
}
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
inline
void CSipTransaction::SetTosOpaque(IN mxt_opaque opq)
{
    m_opqTos = opq;
}
// BRCM_CUSTOM - [end] Add TOS config support

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPTRANSACTION_H
