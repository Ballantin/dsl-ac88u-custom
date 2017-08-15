//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPTRANSACTIONMGR_H
#define MXG_ISIPTRANSACTIONMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================



MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipTransactionUser;
class ISipTransportMgr;
class ISipTransportUser;
class ISipTransactionStatistics;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipTransactionMgr
//========================================
//
// Description:
//   The interface through which the Transaction User (SipCore) has to interact
//   in order to manage the transactions list. The Transaction User creates
//   transaction via the method CreateTransaction. It can also send responses
//   to any server transaction it previously created by using the SendResponse 
//   method.
//
//   Transaction events are reported via the interface ISipTransactionUser. When
//   creating a transaction, the SipCore has to provide a pointer to such
//   interface so it can be notified of future events happening on this
//   transaction.
//
// Location:
//   SipTransaction/ISipTransactionMgr.h
//
// See Also:
//   ISipTransactionUser
//
//==EDOC========================================================================
class ISipTransactionMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    // The role of the transaction.
    //------------------------------
    enum ETransactionRole
    {
        // Transaction initiated on client side: Sending a request.
        //----------------------------------------------------------
        eCLIENT,
        // Transaction initiated on server side: Receiving a request. If the
        // request is a merged request compared with another transaction that
        // has merged request detection enabled, it will be rejected with a 482
        // Loop Detected response. UA services should use this mode.
        //----------------------------------------------------------------------
        eSERVER_WITH_MERGED_REQUEST_DETECTION,
        // Transaction initiated on server side: Receiving a request. If the
        // request is a merged request of another transaction, it will
        // nevertheless create the new transaction and will not automatically
        // reject it with a 482. This is useful for proxy services that MUST
        // NOT act upon merged requests and must forward them anyway. Note that
        // if a merged request corresponding to a transaction created with this
        // mode is received later, it will not be considered as a merged
        // request even if it is created with
        // eSERVER_WITH_MERGED_REQUEST_DETECTION. This feature enable mixing
        // proxy and ua services in the same application. Merged requests can
        // only be detected when the transaction for BOTH requests were created
        // using eSERVER_WITH_MERGED_REQUEST_DETECTION.
        //----------------------------------------------------------------------
        eSERVER_WITHOUT_MERGED_REQUEST_DETECTION
    };

    // Transaction timers struct
    struct STransactionsTimers
    {
        unsigned int m_uT1Ms;
        unsigned int m_uT2Ms;
        unsigned int m_uT4Ms;
        unsigned int m_uTDMs;
        unsigned int m_uTBMs;

        STransactionsTimers(unsigned int uT1Ms, unsigned int uT2Ms, unsigned int uT4Ms, unsigned int uTDMs, unsigned int uTBMs) 
        {
            m_uT1Ms = uT1Ms;
            m_uT2Ms = uT2Ms;
            m_uT4Ms = uT4Ms;
            m_uTDMs = uTDMs;
            m_uTBMs = uTBMs;
        }
    };

    //-- << Stereotype >>
    //--------------------------------------------

    //-- << Initialization and Termination>>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  Initialize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the transport manager.
    //
    //  Parameters:
    //    pTransportMgr:
    //      The transport manager.
    //
    //  Returns:
    //      resS_OK:
    //          Success is always returned.
    //
    //  Description:
    //      Sets the transport manager used to send requests over the network.
    //
    //==EDOC====================================================================
    virtual mxt_result Initialize(IN ISipTransportMgr& rTransportMgr) = 0;

    //==SDOC====================================================================
    //==
    //==  ShutdownA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes all the active transactions asynchronously.
    //
    //  Parameters:
    //    rUser:
    //      The user on which EvCommandResult is called when the shutdown is
    //      terminated.
    //
    //    opq:
    //      The opaque data that is passed into EvCommandResult when called.
    //
    //  Description:
    //      Removes all the active transactions from the list asynchronously.
    //
    //==EDOC====================================================================
    virtual void ShutdownA(IN ISipTransportUser& rUser,
                           IN mxt_opaque opq) = 0;



    //-- << Transaction Management >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  ForceTransactionTimeout
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the specified transaction from the active transactions list.
    //
    //  Parameters:
    //    opqTransaction:
    //      The handle of the transaction.
    //
    //  Returns:
    //      resS_OK:
    //          Everything went OK.
    //
    //      resFE_FAIL:
    //          Transaction removal did not work. Transaction handle is probably
    //          invalid.
    //
    //  Description:
    //      This method retrieves the transaction associated with the specified
    //      handle and removes it from the active transactions list. This allows
    //      the Transaction User, when a timeout occurs, to terminate a client
    //      INVITE transaction for which no response higher than 2xx has been
    //      received following one or several 1xx responses. Not calling this
    //      method would leave the INVITE client transaction in a proceeding
    //      state for this specific case.
    //
    //      It does not generate any event on the Transaction User after this
    //      has been called.
    //
    //==EDOC====================================================================
    virtual mxt_result ForceTransactionTimeout(IN mxt_opaque opqTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  AbandonLastResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the specified transaction from the active transactions list.
    //
    //  Parameters:
    //    opqTransaction:
    //      The handle of the transaction.
    //
    //  Returns:
    //      resS_OK:
    //          Everything went OK.
    //
    //      resFE_FAIL:
    //          Transaction removal did not work. Transaction handle is probably
    //          invalid.
    //
    //  Description:
    //      This method retrieves the transaction associated with the specified
    //      handle and removes it from the active transactions list. This allows
    //      the Transaction User to terminate the transaction because of a
    //      failure to send a response. The Transaction User must call this
    //      method prior to trying other targets directly through the
    //      SipTransport.
    //
    //      It does not generate any event on the Transaction User after this
    //      has been called.
    //
    //==EDOC====================================================================
    virtual mxt_result AbandonLastResponse(IN mxt_opaque opqTransaction) = 0;

    //==========================================================================
    //==
    //==  CreateTransaction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Creates a server or client transaction.
    //
    //  Parameters:
    //    eRole:
    //      The role of the transaction: Client or server with merged request
    //      detection or server without merged request detection.
    //
    //    rPacket:
    //      The packet. Must be a request.
    //
    //    pUser:
    //      Interface through which events are reported. It can be NULL.
    //
    //    pTransStats:
    //      Pointer to the transaction statistics.
    //
    //    ropqTransaction:
    //      The handle of the transaction.
    //
    //    pstTransactionTimers
    //      Pointer to the timers to use per transaction.
    //
    //  Returns:
    //      resS_OK:
    //          The transaction has been created successfully.
    //
    //      resFE_FAIL:
    //          The creation because rPacket is not a request.
    //
    //      resFE_DUPLICATE:
    //          The creation failed because there is already an existing
    //          transaction with the same attributes.
    //
    //      It can also return any status code returned by the CSipHeader::Parse
    //      method or the CSipTransaction::Send method when the request for
    //      client transaction is being sent.
    //
    //      It can also return any status code returned by the CSipHeader::Parse
    //      method or the CSipTransaction::Send method when the request for
    //      client transaction is being sent.
    //
    //  Description:
    //      Creates a server or client transaction. If it is a client
    //      transaction, it sends the request over the network. The transaction
    //      manager also does the retransmissions as needed but silently 
    //      receives responses for it.
    //
    //      In case of a server transaction, the Transaction User that called
    //      this method must send a final response for that transaction but is
    //      not informed whether or not the response failed. If it is an INVITE
    //      server transaction, it possibly sends a 100 Trying over the network
    //      if the Transaction User did not send any response within 200 ms.
    //
    //      If merged request detection is used, the created transaction 
    //      automatically answers a 482 Loop Detect when the transaction user
    //      attempts to create a new transaction with merged request detection
    //      with a merged request. When it is disabled, the transaction user
    //      can create a new transaction with a merged request later. A merged
    //      request is a request that has taken two different paths to arrive at
    //      the same endpoint. This is recognized by the fact that both requests
    //      have no To tag, the same From tag, the same Call-Id, the same CSeq,
    //      and the same Request-Uri user part. Typically, proxy services should
    //      create transactions that do not detect merged requests and 
    //      user-agent services should create transactions that do detect merged
    //      requests.
    //
    //      In case pUser parameter is NULL, the Transaction User is not 
    //      notified of any event regarding the transaction's progress.
    //
    //  See Also:
    //      ISipTransactionUser, SendResponse
    //
    //==========================================================================
    virtual mxt_result
        CreateTransaction(IN  ETransactionRole           eRole,
                          IN  const CSipPacket&          rPacket,
                          IN  ISipTransactionUser*       pUser,
                          IN  ISipTransactionStatistics* pTransStats,
// BRCM_CUSTOM - [add] Add CCTK GUID opaque
                          IN  mxt_opaque                 opqGuid,
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
                          IN  mxt_opaque                 opqTos,
// BRCM_CUSTOM - [end] Add TOS config support
                          OUT mxt_opaque&                ropqTransaction,
                          IN  const STransactionsTimers* pstTransactionTimers)= 0;


    //==SDOC====================================================================
    //==
    //==  SendResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a response on the specified transaction.
    //
    //  Parameters:
    //    rPacket:
    //      The packet to send. It must be a response.
    //
    //    opqTransaction:
    //      The handle of the transaction.
    //
    //  Returns:
    //      resS_OK :
    //          Everything went OK.
    //
    //      resFE_FAIL:
    //          Returned when the packet is a request.
    //
    //      It can also return any status code returned by the CSipHeader::Parse
    //      or CSipTransaction::Send methods.
    //
    //  Description:
    //      Sends a packet on the specified transaction. This could be used to
    //      send any response to an incoming server transaction.
    //
    //==EDOC====================================================================
    virtual mxt_result SendResponse(IN const CSipPacket&    rPacket,
                                    IN mxt_opaque           opqTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTransactionStatistics
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the transaction statistics to be used.
    //
    //  Parameters:
    //    opqTransaction:
    //      Handle of the transaction to configure.
    //
    //    pTransStats:
    //      Pointer to the transaction statistics.
    //
    //  Description:
    //      Configures the transaction statistics for the specified transaction.
    //
    //==EDOC====================================================================
    virtual void SetTransactionStatistics(
                                IN mxt_opaque opqTransaction,
                                IN ISipTransactionStatistics* pTransStats) = 0;

    //==SDOC====================================================================
    //==
    //==  StopRetransmission
    //==
    //==========================================================================
    //
    //  Summary:
    //    Stops the retransmissions.
    //
    //  Parameters:
    //    opqTransaction:
    //      Handle of the transaction.
    //
    //  Returns:
    //      resFE_FAIL
    //          Retransmissions have not been stopped.
    //      resSW_NOTHING_DONE
    //          Transaction uses reliable transport, therefore timer is not 
    //          running and was not stopped.
    //      resFE_INVALID_STATE
    //          Transaction is not in the appropriate state.
    //      Otherwise
    //          The packet retransmissions were correctly stopped.
    //
    //  Description:
    //    Stops the retransmissions of the given transaction.
    //
    //  MXI_DESIGN_NOTES: This method is currently only supported for UAC 
    //    INVITE transactions as no other useful use is known when writing this.
    //
    //==EDOC====================================================================
    virtual mxt_result StopRetransmission(IN mxt_opaque opqTransaction) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipTransactionMgr(){};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipTransactionMgr(){};

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipTransactionMgr(IN const ISipTransactionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipTransactionMgr& operator=(IN const ISipTransactionMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTRANSACTIONMGR_H

