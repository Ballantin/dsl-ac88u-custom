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
#ifndef MXG_ISIPSESSIONTRANSACTION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSESSIONTRANSACTION_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_METHOD_H
#include "SipParser/Method.h"
#endif

#ifndef MXG_ISIPSESSIONTRANSACTIONCONTROLLER_H
// ISipSessionTransactionController::ESessionDialogTerminationType
#include "SipUserAgent/ISipSessionTransactionController.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipPacket;
class ISipClientTransaction;
class ISipForkedDialogGrouperMgr;
class ISipReqCtxCoreSvc;
class ISipRequestContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISipSessionTransaction);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipSessionTransaction
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//  The interface through which the session service communicates with its
//  transactions.
//
// Description:
//  This interface is used by the session service to fetch information from a
//  transaction. Such informations tells the session if the transaction allows
//  to send a request or receive a packet. The session service is able to
//  determine the outcome of an outgoing request on an incoming packet based
//  on the feedback provided by the transactions.
//
//  This is a generic interface that defines the methods common to all
//  transaction that are handled by the session service:
//  - Client Invite
//  - Server Invite
//  - Client Bye
//  - Server Bye
//
//  This interface is internal to the stack and MUST NOT be used outside of the
//  SIP-UA.
//
// Location:
//   SipUserAgent/ISipSessionTransaction.h
//
// See Also:
//   CSipSessionTransaction, CSipSessionSvc
//
//==============================================================================
class ISipSessionTransaction : public IEComUnknown
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    MX_DECLARE_ECOM_GETIID(ISipSessionTransaction);

    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  SetController
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the transaction controller.
    //
    //  Parameters:
    //      pSessionTransactionController:
    //       The transaction controller.
    //
    //  Description:
    //      Sets the transaction controller.
    //
    //      The transaction can use this pointer to query other ECOM interfaces
    //      like ISipUserAgentSvc and IPrivateSipContext.
    //
    //      This method MUST be called. And MUST NOT be called on a
    //      ISipSessionTransaction that already has a
    //      ISipSessionTransactionController.
    //
    //==========================================================================
    virtual void SetController(IN ISipSessionTransactionController* pSessionTransactionController) = 0;

    // The session dialog state.
    enum ESessionDialogState
    {
        // The session dialog is not established yet.
        eSESSION_DIALOG_NOT_ESTABLISHED,
        // The session dialog is established.
        eSESSION_DIALOG_ESTABLISHED        
    };

    //==========================================================================
    //==
    //==  SetSessionDialogState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the session dialog state.
    //
    //  Parameters:
    //      eSessionDialogState:
    //       The session dialog state.
    //
    //  Description:
    //      Sets the session dialog state.
    //
    //      It could be used to determine the event to report on the session
    //      manager or the method to call on the user agent service.
    //
    //      The default value is eSESSION_DIALOG_NOT_ESTABLISHED.
    //
    //==========================================================================
    virtual void SetSessionDialogState(IN ESessionDialogState eSessionDialogState) = 0;

    //==========================================================================
    //==
    //==  GetReqCtxCoreSvc
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the request context core service.
    //
    //  Parameters:
    //      rpReqCtxCoreSvc:
    //       The request context core service.
    //
    //       A reference is counted.
    //
    //  Description:
    //      Gets the request context core service implemented by the session
    //      transaction.
    //
    //      This method is needed because ISipReqCtxCoreSvc is not an ECOM
    //      interface.
    //
    //==========================================================================
    virtual void GetReqCtxCoreSvc(OUT ISipReqCtxCoreSvc*& rpReqCtxCoreSvc) = 0;

    //==========================================================================
    //==
    //==  AllowSendingRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells if this instance currently allows the sending of a request.
    //
    //  Parameters:
    //      eMethod:
    //       The method of the request to send. Can be INVITE or BYE.
    //
    //  Returns:
    //      - resS_OK: The state of this instance allows the requested request
    //                  to be sent.
    //      - resFE_REQUEST_PENDING: The state of this instance forbids the
    //                               requested request to be sent at this time.
    //      - Other failures: The state of this instance forbids the requested
    //                        request to be sent at any time.
    //
    //  Description:
    //      Tells the caller if the current ISipSessionTransaction allows to
    //      send the specified request or not.
    //
    //      This method is used by the ISipSessionTransactionController to know
    //      if a new client transaction can be initiated.
    //
    //==========================================================================
    virtual mxt_result AllowSendingRequest(IN ESipMethod eMethod) = 0;

    //==========================================================================
    //==
    //==  CanSendRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells if this instance can send a request.
    //
    //  Parameters:
    //      eMethod:
    //       The method type to send. Will be ACK.
    //
    //  Returns:
    //      - true: The request can be sent by this instance.
    //      - false: The method cannot be sent by this instance.
    //
    //  Description:
    //      This method tells if the specified request can be sent by this
    //      instance.
    //
    //      This method is used by the ISipSessionTransactionController to know
    //      which ISipSessionTransaction must be used to send an ACK request.
    //
    //==========================================================================
    virtual bool CanSendRequest(IN ESipMethod eMethod) = 0;

    //==========================================================================
    //==
    //==  AllowReceivingRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells if the transaction currently allows the reception of that
    //      packet.
    //
    //  Parameters:
    //      eMethod:
    //       The method of the request.
    //
    //      rRequest:
    //       The request. Can be an INVITE or a BYE.
    //
    //      ruResponseRejectedCode:
    //       The final response code to send to reject the request. MUST be set
    //       when a failure is returned.
    //
    //      ppResponseExtraHeaders:
    //       A pointer to a CHeaderList pointer to use when this method returns
    //       a failure.
    //
    //       The pointed pointer will be set when additional headers must be
    //       added to the response.
    //
    //       Even if the method returns a failure, the pointed pointer can be
    //       NULL when there is no additional headers to add to the response.
    //
    //       Pass NULL when this information is not useful.
    //
    //  Returns:
    //      - resS_OK: the state of the ISipSessionTransaction allows the
    //                 received request to be processed.
    //      - Failure: the state of the ISipSessionTransaction forbids the
    //                 request to be received. It should be rejected by the
    //                 ruResponseRejectedCode response code.
    //
    //  Description:
    //      This method tells the caller if this ISipSessionTransaction is in a
    //      state where it is possible to accept a received request.
    //
    //      Used by the ISipSessionTransactionController to determine if a
    //      received request can be received at this moment.
    //
    //      This is used when a new BYE or INVITE request is received through
    //      ISipCoreSvc::GetOwnerConfidenceLevel and then
    //      ISipCoreSvc::GetOwnerInstance (implemented by the controller).
    //
    //==========================================================================
    virtual mxt_result AllowReceivingRequest(IN ESipMethod eMethod,
                                             IN const CSipPacket& rRequest,
                                             OUT uint16_t& ruResponseRejectedCode,
                                             OUT GO CHeaderList** ppResponseExtraHeaders) = 0;

    //==========================================================================
    //==
    //==  CanHandlePacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells if the transaction currently allows the reception of that
    //      packet.
    //
    //  Parameters:
    //      eMethod:
    //       The method of the packet. If rPacket is a response, the method is
    //       taken from the CSeq header in rPacket.
    //
    //      rPacket:
    //       The packet. It can be an INVITE response or an ACK request.
    //
    //  Returns:
    //      - true: this ISipSessionTransaction instance can handle the packet.
    //      - false: the packet cannot be processed by the
    //               ISipSessionTransaction.
    //
    //  Description:
    //      This method tells the caller if this instance is in a state
    //      to handle the received packet by itself.
    //
    //      Used by the ISipSessionTransactionController to determine which
    //      ISipSessionTransaction can handle a received packet.
    //
    //      This is used when a packet is not reported directly through
    //      ISipReqCtxSvc::OnPacketReceived but instead the packet is reported
    //      on the ISipContext which uses ISipCoreSvc::GetOwnerConfidenceLevel
    //      and then ISipCoreSvc::GetOwnerInstance (implemented by the
    //      ISipSessionTransactionController).
    //
    //==========================================================================
    virtual bool CanHandlePacket(IN ESipMethod eMethod,
                                 IN const CSipPacket& rPacket) = 0;

    //==========================================================================
    //==
    //==  IsSessionDialogNeeded
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells if the transaction currently needs the session dialog to
    //      complete.
    //
    //  Parameters:
    //      eType:
    //       The parameter that has been passed to
    //       ISipSessionTransactionController::EvTerminateSessionDialog if this
    //       parameter is eFINAL_TERMINATION, the same value must be passed when
    //       this object calls
    //       ISipSessionTransactionController::EvTerminateSessionDialog again.
    //
    //  Returns:
    //      - true: this ISipSessionTransaction instance needs the session
    //              dialog to complete its processing. When this object does not
    //              need the session dialog anymore, it will call
    //              ISipSessionTransactionController::EvTerminateSessionDialog.
    //      - false: this ISipSessionTransaction instance does not need the
    //               session dialog to complete.
    //
    //  Description:
    //      This method tells if the transaction currently needs the session
    //      dialog to complete. If it needs it (e.g.: returns true), the session
    //      dialog MUST NOT be terminated. An instance returning true to this
    //      method MUST call
    //      ISipSessionTransactionController::EvTerminateSessionDialog when it
    //      does not need the session dialog.
    //
    //==========================================================================
    virtual bool IsSessionDialogNeeded(IN ISipSessionTransactionController::ESessionDialogTerminationType eType) = 0;

    //==========================================================================
    //==
    //==  Invite
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends an INVITE.
    //
    //  Parameters:
    //      pForkedDialogGrouperMgr:
    //       The manager of the forked dialog grouper.
    //
    //       Can be NULL, in which case forking for the sent INVITE will not be
    //       supported.
    //
    //       pForkedDialogGrouper MUST not be NULL when an initial INVITE is
    //       sent on a context for which no dialog is established yet.
    //
    //    opqTransaction:
    //      Application data to associate with the INVITE transaction (i.e. the
    //      request context). This is opaque to the service.
    //
    //    pExtraHeaders:
    //      The extra SIP headers to add to the packet.
    //
    //    pMessageBody:
    //      The packet's body, aka payload.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      - resS_OK: The INVITE is sent.
    //      - resFE_INVALID_STATE: The transaction cannot send the INVITE.
    //      - Failure: Other return code.
    //
    //  Description:
    //      Sends an INVITE or a reINVITE.
    //
    //      If the INVITE can fork as it could be an initial INVITE sent on a
    //      non-established dialog then pForkedDialogGrouperMgr must be
    //      specified.
    //
    //==========================================================================
    virtual mxt_result Invite(IN ISipForkedDialogGrouperMgr* pForkedDialogGrouperMgr,
                              IN mxt_opaque opqTransaction,
                              IN TOA CHeaderList* pExtraHeaders,
                              IN TOA CSipMessageBody* pMessageBody,
                              OUT ISipClientTransaction*& rpTransaction) = 0;

    //==========================================================================
    //==
    //==  Ack
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends an ACK.
    //
    //  Parameters:
    //    pExtraHeaders:
    //      The extra SIP headers to add to the packet.
    //
    //    pMessageBody:
    //      The packet's body, aka payload.
    //
    //  Returns:
    //      - resS_OK: The ACK is sent.
    //      - resFE_INVALID_STATE: The transaction cannot send the ACK.
    //      - Failure: Other return code.
    //
    //  Description:
    //      Sends an ACK.
    //
    //==========================================================================
    virtual mxt_result Ack(IN TOA CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody) = 0;

    //==========================================================================
    //==
    //==  Bye
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a BYE.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      The extra SIP headers to add to the packet.
    //
    //    pMessageBody:
    //      The packet's body, aka payload.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      - resS_OK: The BYE is sent.
    //      - resFE_INVALID_STATE: The transaction cannot send the BYE.
    //      - Failure: Other return code.
    //
    //  Description:
    //      Sends a BYE.
    //
    //==========================================================================
    virtual mxt_result Bye(IN mxt_opaque opqTransaction,
                           IN TOA CHeaderList* pExtraHeaders,
                           IN TOA CSipMessageBody* pMessageBody,
                           OUT ISipClientTransaction*& rpTransaction) = 0;

    //==========================================================================
    //==
    //==  Clear
    //==
    //==========================================================================
    //
    //  Summary:
    //      Called when the context is cleared.
    //
    //  Description:
    //      The method is called when the application has cleared the context.
    //      The implementation should release any references it has on a client
    //      request context and stop timers.
    //
    //  See Also:
    //      ISipCoreSvc::ClearCoreSvc
    //
    //==========================================================================
    virtual void Clear() = 0;


//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipSessionTransaction(){};

    // Summary:
    //  Destructor.
    virtual ~ISipSessionTransaction(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipSessionTransaction(IN const ISipSessionTransaction& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipSessionTransaction& operator=(IN const ISipSessionTransaction& rSrc);


//-- Hidden Data Members
protected:
private:
};


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_ISIPSESSIONTRANSACTION_H

