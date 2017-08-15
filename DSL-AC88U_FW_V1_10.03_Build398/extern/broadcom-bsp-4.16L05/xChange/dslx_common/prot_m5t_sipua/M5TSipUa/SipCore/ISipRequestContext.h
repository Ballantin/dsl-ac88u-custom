//==============================================================================
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
//==============================================================================
#ifndef MXG_ISIPREQUESTCONTEXT_H
#define MXG_ISIPREQUESTCONTEXT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_ISIPTRANSACTIONMGR_H
#include "SipTransaction/ISipTransactionMgr.h" // ISipTransactionMgr::ERole
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHeaderList;
class CSipForkedDialogGrouper;
class CSipMessageBody;
class CSipPacket;
class CToken;
class IPrivateSipContext;
class ISipClientSvc;
class ISipContext;
class ISipForkedDialogGrouperMgr;
class ISipReqCtxConnectionSvc;
class ISipReqCtxCoreSvc;
class ISipTransactionStatistics;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipRequestContext);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipRequestContext
//========================================
//
// Description:
//   This interface represents a transaction client or server. It uses the core
//   and connection request context services to create and analyze requests and
//   their responses.
//
//   It is also responsible for managing the CANCEL associated with the
//   transaction it manages.
//
//   Services that handle incoming requests or generate outgoing requests are
//   responsible for creating it.
//
// Location:
//   SipCore/ISipRequestContext.h
//
//==============================================================================
class ISipRequestContext : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipRequestContext);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  Initialize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Initializes a newly created request context.
    //
    //  Parameters:
    //    pContext:
    //      The SIP context/service that created the request context. This
    //      parameter MUST NOT be NULL. ISipContext interface MUST be queryable
    //      on this pointer.
    //
    //  Returns:
    //      - Success: the initialization succeeded.
    //      - Failure: One of the following conditions occurred:
    //                 - pContext is NULL
    //                 - pContext does not offer the ISipContext interface.
    //                 - the request context has already been initialized.
    //
    //  Description:
    //      Performs the initialization of the request context.
    //
    //==========================================================================
    virtual mxt_result Initialize(IN IEComUnknown* pContext) = 0;

    //==========================================================================
    //==
    //==  UpdatePacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Scans through embedded services to create a packet to send.
    //
    //  Parameters:
    //    rPacket:
    //      The packet to send.
    //
    //  Returns:
    //      resS_OK:
    //          The packet has been successfully updated.
    //
    //      resSW_SIPCORE_WAIT_ASYNC_RESPONSE:
    //          Asynchronous processing is needed to query the application for
    //          packet modification.
    //
    //      resFE_INVALID_STATE:
    //          The packet cannot be sent by this request context. It occurs
    //          when trying to send more than one request or more than one final
    //          response with the same request context. It also occurs when
    //          trying to send requests on a server request context or responses
    //          on a client request context. Finally, it occurs if the packet is
    //          neither a request nor a response.
    //
    //  Description:
    //      Passes the packet through every interested request context core
    //      services so they can update it before it is sent on the network. The
    //      services can also update their state instead of updating the packet.
    //
    //     Note that it is possible to update more than one final postive
    //     response on the same request context. This is to allow proxies to
    //     forward multiple 2xx responses.
    //
    //  See Also:
    //      OnPacketUpdated, ISipReqCtxCoreSvc
    //
    //==========================================================================
    virtual mxt_result UpdatePacket(INOUT CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  OnPacketUpdated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies this request context that the packet has been updated.
    //
    //  Parameters:
    //    asyncResult:
    //      The result of the asynchronous update.
    //
    //    rPacket:
    //      The packet to send.
    //
    //  Description:
    //      Notifies the request context that the service is done updating the
    //      packet. The request context calls UpdatePacket on the remainder of
    //      the ISipReqCtxCoreSvc vector and, when it is finished, it calls
    //      OnPacketUpdated on its owner core service. The owner core service
    //      is then able to continue processing the packet.
    //
    //      If UpdatePacket on one of the ISipReqCtxCoreSvc returns that
    //      asynchronous processing is needed, the request context stores where
    //      in the ISipReqCtxCoreSvc vector it happened and waits for
    //      OnPacketUpdated to be called again.
    //
    //  See Also:
    //      UpdatePacket, ISipReqCtxCoreSvc
    //
    //==========================================================================
    virtual void OnPacketUpdated(IN mxt_result asyncResult,
                                 INOUT CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  EnableForkedDialogGrouper
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables a CSipForkedDialogGrouper to this ISipRequestContext.
    //
    //  Parameters:
    //    pForkedDialogGrouper:
    //      The CSipForkedDialogGrouper to use. MUST NOT be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: - EnableForkedDialogGrouper was already
    //                               called on this instance.
    //                             - Something was already sent or received on
    //                               this instance.
    //
    //      - resFE_INVALID_ARGUMENT: - The new CSipForkedDialogGrouper failed
    //                                  to be set on rOriginatorContext.
    //
    //      - resS_OK: The CSipForkedDialogGrouper has been enabled.
    //
    //  Description:
    //      Enables forking detection to this instance. This should be done
    //      before a request that can fork is sent through a transaction using
    //      this ISipRequestContext.
    //
    //      When the status of the request is received through
    //      EvRequestProgress, EvRequestSuccess and EvRequestFailed, it is
    //      passed to the CSipForkedDialogGrouper first so this object can
    //      dispatch the response to another SIP context if necessary. If the
    //      CSipForkedDialogGrouper does not handle the response, the response
    //      is passed back to the owner service.
    //
    //==========================================================================
    virtual mxt_result EnableForkedDialogGrouper(IN CSipForkedDialogGrouper* pForkedDialogGrouper) = 0;

    //==========================================================================
    //==
    //==  SendPacketThroughTransaction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends the packet through the transaction manager.
    //
    //  Parameters:
    //    rPacket:
    //      The packet to send.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The method has been called but the request
    //      context is already updating a packet to send.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: The packet has been blocked by one
    //      of the connection services.
    //
    //      - resSW_SIPCORE_WAIT_ASYNC_RESPONSE: One of the services waits an
    //      asynchronous response to update the packet.
    //
    //      - resFE_INVALID_ARGUMENT: Attempting to send a response through an
    //      invalid transaction.
    //
    //      - resFE_FAIL: Unable to activate the forked dialog grouper, to
    //      create the transaction to send the request or unable to send the
    //      response through the transaction that received the request.
    //
    //      - resS_OK: The packet has been successfully sent.
    //
    //  Description:
    //      Sends the packet with the transaction manager after presenting the
    //      packet to the connection services. If no transaction exists, the
    //      request context creates one. Otherwise, it uses the current
    //      transaction. If the packet is a request, the request context waits
    //      for the status of the request through EvRequestProgress,
    //      EvRequestSuccess, and EvRequestFailed. If the packet is a response,
    //      the request context waits for the EvResponseSuccess or
    //      EvResponseFailed event.
    //
    //      Before the packet is showed to anybody, the forked dialog grouper is
    //      activated if EnableForkedDialogGrouper has been called.
    //
    //      When this method fails, EnableForkedDialogGrouper has to be called
    //      again on this instance if a forkable request is tried again on this
    //      ISipRequestContext.
    //
    //  See Also:
    //      EvRequestProgress, EvRequestSuccess, EvRequestFailed,
    //      EvResponseSuccess, EvResponseFailed.
    //
    //==========================================================================
    virtual mxt_result SendPacketThroughTransaction(IN CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  SendPacketThroughTransport
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends the packet through the transport manager.
    //
    //  Parameters:
    //    rPacket:
    //      The packet to send.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The method has been called but the request
    //      context is already updating a packet to send.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: The packet has been blocked by one
    //      of the connection services.
    //
    //      - resSW_SIPCORE_WAIT_ASYNC_RESPONSE: One of the services waits an
    //      asynchronous response to update the packet.
    //
    //      - resFE_FAIL: Unable to send the packet through the transport
    //      manager.
    //
    //      - resS_OK: The packet has been successfully sent.
    //
    //  Description:
    //      Sends the packet with the transport manager after presenting the
    //      packet to the connection services. The status of the sent action
    //      is received through the EvCommandResult method.
    //
    //  See Also:
    //      EvCommandResult
    //
    //==========================================================================
    virtual mxt_result SendPacketThroughTransport(IN CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  SendRetransmissionThroughTransport
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      Sends the retransmitted packet through the transport manager.
    //
    //  Parameters:
    //    rPacket:
    //      The packet to send.
    //
    //  Returns:
    //      - resFE_FAIL: Unable to send the packet through the transport
    //      manager.
    //
    //      - resS_OK: The packet has been successfully sent.
    //
    //  Description:
    //      Sends the retransmitted packet with the transport manager. This
    //      packet will not be updated again. The status of the sent action
    //      is received through the EvCommandResult method.
    //
    //  See Also:
    //      EvCommandResult
    //
    //==========================================================================
    virtual mxt_result SendRetransmissionThroughTransport(IN CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  ProcessEvents
    //==
    //==========================================================================
    //
    //  Summary:
    //      Presents the received packet to the embedded services.
    //
    //  Parameters:
    //    rPacket:
    //      The received request.
    //
    //  Description:
    //      Called by the owner core service when receiving a request or a
    //      response for an outgoing request that has been forked. This can be
    //      the case when an INVITE response triggered the creation of a new
    //      dialog because the to-tag in it was different than the one in the
    //      original dialog.
    //
    //      This method presents the packet to each request context core service
    //      and at last to the owner service (through the OnPacketReceived
    //      method). It then calls the first event in the event list.
    //
    //  See Also:
    //      ISipReqCtxCoreSvc::OnPacketReceived
    //
    //==========================================================================
    virtual void ProcessEvents(IN const CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  CreateServerTransaction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Creates a server transaction on the transaction manager to handle
    //      the packet in parameter.
    //
    //  Parameters:
    //    eMergedRequestDetection:
    //      Whether or not the transaction should be created if a merged request
    //      exists.
    //
    //    rPacket:
    //      The received packet.
    //
    //  Returns:
    //    - resFE_DUPLICATE: The transaction has not been created because the
    //    packet is a retransmission of another packet. It can happen when two
    //    retransmissions are received fast and the second packet is put in the
    //    core processing queue before the first packet is processed.
    //
    //    - resFE_FAIL: The transaction has not been created for some other
    //    reason. It could be because this request context is already managing
    //    another transaction or because the transaction manager is overloaded,
    //    for instance.
    //
    //    - resS_OK: The transaction has been successfully created.
    //
    //  Description:
    //      Creates the transaction on the transaction manager to catch
    //      retransmissions and send responses.
    //
    //==========================================================================
    virtual mxt_result CreateServerTransaction(
                IN ISipTransactionMgr::ETransactionRole eMergedRequestDetection,
                IN const CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  SetOwner
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the owner request context core service for this request context.
    //
    //  Parameters:
    //    pOwner:
    //      The owner service for this request context. It can be NULL.
    //
    //  Description:
    //      Sets the owner service for this request context. This should be
    //      called at the beginning right after the creation of the request
    //      context. When the owner is done with the request context, it should
    //      call this method again, this time passing NULL as a parameter.
    //
    //      When the owner is NULL, packets received on the request context do
    //      not generate events to core nor connection services.
    //
    //==========================================================================
    virtual void SetOwner(IN ISipReqCtxCoreSvc* pOwner) = 0;

    //==========================================================================
    //==
    //==  StoreReIssueData
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the information used to retry the request in case of failure.
    //
    //  Parameters:
    //    rClientSvc:
    //      The service issuing the request.
    //
    //    rMethod:
    //      The method for the request.
    //
    //    pExtraHeaders:
    //      The additional headers to put in the request.
    //
    //    pMessageBody:
    //      The message-body to put in the request.
    //
    //    opqService:
    //      Opaque usable by the service sending the packet.
    //      This value is saved as part of the re-issue data and is
    //      passed back to ISipClientSvc::SendRequest if the request
    //      needs to be re-issued.
    //      It is possible this opaque will point to dynamically allocated
    //      memory. In this case, the memory can be freed when the
    //      re-issue data is no longer needed. For convenience, services
    //      can depend on ISipClientSvc::ClearReIssueData being called
    //      when this happens.
    //
    //  Description:
    //      Stores the data necessary to re-issue the request if it fails. This
    //      is called by the service that sends the request.
    //
    //      Clear or ReIssueRequest must be called for this data to be removed
    //      from memory.
    //
    //  See Also:
    //      ReIssueRequest, Clear
    //
    //==========================================================================
    virtual void StoreReIssueData(IN ISipClientSvc& rClientSvc,
                                  IN const CToken& rMethod,
                                  IN CHeaderList* pExtraHeaders,
                                  IN CSipMessageBody* pMessageBody,
                                  IN mxt_opaque opqService) = 0;

    //==========================================================================
    //==
    //==  GetCoreSvcVector
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a reference to the internal vector of request context core
    //      services of this request context.
    //
    //  Returns:
    //      A reference to the vector that contains the core services.
    //
    //  Description:
    //      This method is called by the owner service and it is called only to
    //      populate the vector with the interested request context core
    //      services.
    //
    //  See Also:
    //      GetConnectionSvcVector
    //
    //==========================================================================
    virtual CVector<ISipReqCtxCoreSvc*>& GetCoreSvcVector() = 0;

    //==========================================================================
    //==
    //==  GetConnectionSvcVector
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a reference to the internal vector of request context
    //      connection services of this request context.
    //
    //  Returns:
    //      A reference to the vector that contains the connection services.
    //
    //  Description:
    //      This method is called by the owner service and it is called only to
    //      populate the vector with the interested request context connection
    //      services.
    //
    //  See Also:
    //      GetCoreSvcVector
    //
    //==========================================================================
    virtual CVector<ISipReqCtxConnectionSvc*>& GetConnectionSvcVector() = 0;

    //==========================================================================
    //==
    //==  ClearEvents
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the request context to forget about events it still has.
    //
    //  Description:
    //      This method can be used to clear the event list. It is mainly used
    //      to terminate the request context that received an ACK request since
    //      no response is sent in this case.
    //
    //==========================================================================
    virtual void ClearEvents() = 0;

    //==========================================================================
    //==
    //==  MuteIncomingCancel
    //==
    //==========================================================================
    //
    //  Summary:
    //      Mutes any call to OnRequestCancelled to the owner service when
    //      a final response is being sent.
    //
    //  Parameters:
    //      bMuteIncomingCancel:
    //          True to mute, false otherwise.
    //
    //  Description:
    //      Mutes any call to OnRequestCancelled to the owner service when
    //      a final response is being sent.  The final response can be buffered
    //      in the case when the provisional response service is active and
    //      a PRACK has not been received for a 1xx response.
    //
    //      This keeps the behaviour of the SIP stack inline when a final
    //      response is being sent and when one glares the CANCEL request.
    //
    //==========================================================================
    virtual void MuteIncomingCancel(IN bool bMuteIncomingCancel) = 0;

    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the user-specified opaque parameter associated with this
    //      transaction.
    //
    //  Returns:
    //      The opaque parameter.
    //
    //  Description:
    //      Gets the user-specified opaque parameter associated with this
    //      transaction.
    //
    //      This is the opaque parameter that is passed to the function that
    //      has initiated this client transaction. The same opaque parameter can
    //      also be accessed through ISipClientTransaction::GetOpaque.
    //
    //      It can be changed after the client transaction is created by using
    //      SetOpaque or ISipClientTransaction::SetOpaque.
    //
    //  See Also:
    //      SetOpaque
    //      ISipClientTransaction::GetOpaque
    //      ISipClientTransaction::SetOpaque
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets a user-specified opaque parameter associated with this
    //      transaction.
    //
    //  Parameters:
    //    opq:
    //      The opaque value used to set the parameter.
    //
    //  Description:
    //      Sets a user-specified opaque parameter associated with this
    //      transaction.
    //
    //      This opaque parameter is the same as the one that can be configured
    //      through the ISipClientTransaction interface. Updating it from one
    //      interface has impact on the opaque parameter accessible by both
    //      interfaces, as they relate to the same transaction.
    //
    //  See Also:
    //      GetOpaque
    //      ISipClientTransaction::GetOpaque
    //      ISipClientTransaction::SetOpaque
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;

// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    virtual mxt_opaque GetGuidOpaque() = 0;

    virtual void SetGuidOpaque(IN mxt_opaque opq) = 0;
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
    virtual mxt_opaque GetTosOpaque() = 0;

    virtual void SetTosOpaque(IN mxt_opaque opq) = 0;
// BRCM_CUSTOM - [end] Add TOS config support

    //==========================================================================
    //==
    //==  SetTransactionStatistics
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the transaction statistics to be used.
    //
    //  Parameters:
    //    pTransStats:
    //      Pointer to the transaction statistics.
    //
    //  Description:
    //      Configures the transaction statistics for this request context.
    //
    //==========================================================================
    virtual void SetTransactionStatistics(
                                IN ISipTransactionStatistics* pTransStats) = 0;

    //==========================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates a request context.
    //
    //  Returns:
    //    - resS_OK: The request context will be terminated.
    //    - A failure result indicates that an error has occurred and that the
    //      request context won't be terminated.
    //
    //  Description:
    //      Terminates a request context immediately but as cleanly as possible.
    //      For server transactions, a request terminated response is sent.
    //      Client INVITE transactions are cancelled and client non-INVITE
    //      transaction are answered locally with a timeout response.
    //
    //==========================================================================
    virtual mxt_result Terminate() = 0;

    //==========================================================================
    //==
    //==  GetLastPacketSent
    //==
    //==========================================================================
    //
    //  Returns:
    //      - The last packet sent on the transaction. Can be NULL if no packet
    //        has been sent yet. When non-NULL, a reference is added on the
    //        returned packet and the caller MUST call CSipPacket::Release on
    //        the returned pointer when it is done with it.
    //
    //  Description:
    //      This method returns the last packet sent on a transaction by the 
    //      request context. If no packet has been sent NULL is returned.
    //
    //      When the return value is non-NULL, a reference is added on the
    //      returned packet and the caller MUST call CSipPacket::Release on the
    //      returned pointer when it is done with it.
    //
    //==========================================================================
    virtual const CSipPacket* GetLastPacketSent() const = 0;

    //==========================================================================
    //==
    //==  GetLastPacketReceived
    //==
    //==========================================================================
    //
    //  Parameters:
    //      rpLastPacketReceived:
    //       Set to the last packet received by the ISipRequestContext.
    //
    //       Can be NULL if no packet has been received yet. When non-NULL, a
    //       reference is added on the returned packet and the caller MUST call
    //       CSipPacket::Release on the returned pointer when it is done with
    //       it.
    //
    //  Description:
    //      This method returns the last packet received by the
    //      ISipRequestContext. If no packet has been sent NULL is returned.
    //
    //      When the return value is non-NULL, a reference is added on the
    //      returned packet and the caller MUST call CSipPacket::Release on the
    //      returned pointer when it is done with it.
    //
    //==========================================================================
    virtual void GetLastPacketReceived(OUT const CSipPacket*& rpLastPacketReceived) const = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipRequestContext() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipRequestContext() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipRequestContext(IN const ISipRequestContext& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipRequestContext& operator=(IN const ISipRequestContext& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREQUESTCONTEXT_H

