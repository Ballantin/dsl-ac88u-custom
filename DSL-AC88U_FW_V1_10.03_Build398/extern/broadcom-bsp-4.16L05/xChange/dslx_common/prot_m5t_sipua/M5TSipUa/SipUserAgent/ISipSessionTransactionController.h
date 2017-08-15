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
#ifndef MXG_ISIPSESSIONTRANSACTIONCONTROLLER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSESSIONTRANSACTIONCONTROLLER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_METHOD_H
#include "SipParser/Method.h" // ESipMethod
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_ISIPUSERAGENTSVC_H
#include "SipUserAgent/ISipUserAgentSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipPacket;
class ISipRequestContext;
class ISipSessionMgr;
class ISipSessionTransaction;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(ISipSessionTransactionController);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipSessionTransactionController
//==============================================================================
//<GROUP SipUserAgent>
//
// Summary:
//   The interface through which the session transaction communicates with the
//   session.
//
// Description:
//  It is used by the transaction to fetch informations related to the session
//  or to inform the session about events that could trigger the session to
//  change its state.
//
//  This interface is internal to the stack and MUST NOT be used outside of
//  the SIP-UA.
//
// Location:
//   SipUserAgent/ISipSessionTransactionMgr.h
//
// See Also:
//   CSipSessionTransaction, CSipSessionSvc
//
//==============================================================================
class ISipSessionTransactionController : public IEComUnknown
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    MX_DECLARE_ECOM_GETIID(ISipSessionTransactionController);

    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  GetSessionMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the session service manager.
    //
    //  Returns:
    //      The session service manager.
    //
    //      Can be NULL.
    //
    //  Description:
    //      Gets the session service manager.
    //
    //      The manager can change during the service lifetime. It could be the
    //      application, the context terminator or it can even be NULL if the
    //      SIP context has been cleared.
    //
    //==========================================================================
    virtual ISipSessionMgr* GetSessionMgr() = 0;

    //==========================================================================
    //==
    //==  AllowSendingRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Asks the session if it permits sending the specified request.
    //
    //  Parameters:
    //      pSessionTransaction:
    //       The ISipSessionTransaction that wants to send the request.
    //
    //      eMethod:
    //       The method.
    //
    //  Returns:
    //      - true: The request can be sent.
    //      - false: The request cannot be sent.
    //
    //  Description:
    //      This method is used by the transaction when a request is re-issued.
    //      The transaction asks the session if it is currently in a state where
    //      the request can be re-issued.
    //
    //      There are cases where a re-issue must fail as a request of the same
    //      type could have already been sent. Such case occurs when an INVITE
    //      is either challenged or redirected and the application calls
    //      ISipSessionSvc::Invite before that ReIssueRequest is called.
    //
    //      The client transactions should call AllowSendingRequest in their
    //      GetRequestContext implementation and if it returns true
    //      GetRequestContext should return a NULL request context.
    //
    //  See Also:
    //      ISipClientSvc::GetRequestContext,
    //      ISipClientEventControl::ReIssueRequest
    //
    //==========================================================================
    virtual bool AllowSendingRequest(IN ISipSessionTransaction* pSessionTransaction,
                                     IN ESipMethod eMethod) = 0;

    //==========================================================================
    //==
    //==  EvEstablishSessionDialog
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells the controller that packet establishing a session dialog was
    //      processed.
    //
    //  Parameters:
    //      pSessionTransaction:
    //       The ISipSessionTransaction on which the session dialog has been
    //       terminated.
    //
    //      rPacket:
    //       The packet that has been processed.
    //
    //      eDirection:
    //       eSENT when the packet has been sent to a remote peer, eRECEIVED
    //       when the packet has been received from a remote peer.
    //
    //  Description:
    //      Informs the controller that packet establishing a session dialog has
    //      been processed. This packet can be a 1xx or 2xx response to an
    //      initial INVITE was either sent or received.
    //
    //      This allows the controller to establish a dialog on the
    //      ISipUserAgentSvc and to transit its state from eNOT_ESTABLISHED
    //      to eESTABLISHED.
    //
    //==========================================================================
    virtual void EvEstablishSessionDialog(IN ISipSessionTransaction* pSessionTransaction,
                                          IN const CSipPacket& rPacket,
                                          IN ISipUserAgentSvc::EPacketDirection eDirection) = 0;

    // The type of termination.
    enum ESessionDialogTerminationType
    {
        // Description:
        //  The session dialog is terminated but another could be started again
        //  by the controller (to re-issue a failed request for example).
        eREUSABLE_TERMINATION,
        // Description:
        //  The termination of this session dialog is final. The controller must
        //  not try to start another session dialog.
        eFINAL_TERMINATION
    };

    //==========================================================================
    //==
    //==  EvTerminateSessionDialog
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells the controller that a packet terminating the session dialog
    //      has been processed.
    //
    //  Parameters:
    //      pSessionTransaction:
    //       The ISipSessionTransaction on which the session dialog has been
    //       terminated.
    //
    //      eType:
    //       The type of termination.
    //
    //  Description:
    //      Informs the controller that a packet terminating the session dialog
    //      has been processed. This packet is a 2xx response to a BYE request
    //      that was either sent or received.
    //
    //      This allows the controller to, when applicable, terminate the dialog
    //      on the ISipUserAgentSvc and to transit its state to eTERMINATED.
    //
    //==========================================================================
    virtual void EvTerminateSessionDialog(IN ISipSessionTransaction* pSessionTransaction,
                                          IN ESessionDialogTerminationType eType) = 0;

    //==========================================================================
    //==
    //==  EvTransactionTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells the session about the transaction termination.
    //
    //  Parameters:
    //      pSessionTransaction:
    //       The transaction.
    //
    //  Description:
    //      Reports to the session that the transaction is about to be
    //      terminated.
    //
    //      The session MUST remove the terminated transaction from its active
    //      transactions list.
    //      
    //      Before calling this event the transaction should add a reference on
    //      itself as the session removes the reference it has on the
    //      transaction.
    //
    //==========================================================================
    virtual void EvTransactionTerminated(IN ISipSessionTransaction* pSessionTransaction) = 0;

    //==========================================================================
    //==
    //==  EvNewSessionTransaction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs that a new ISipSessionTransaction has been created.
    //
    //  Parameters:
    //      pNewSessionTransaction:
    //       The new ISipSessionTransaction.
    //
    //       MUST NOT be NULL.
    //
    //  Description:
    //      Reports to the controller that a new ISipSessionTransaction has been
    //      created.
    //
    //      The implementation has the choice to configure the
    //      ISipSessionTransaction as if it would be new (SetController,
    //      SetSessionDialogState, etc.) or doing nothing on it.
    //
    //      If no ISipSessionTransactionController is set on
    //      pSessionTransaction, it will be released.
    //
    //      The implementation of this method MUST be synchronous.
    //
    //==========================================================================
    virtual void EvNewSessionTransaction(IN ISipSessionTransaction* pNewSessionTransaction) = 0;


//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipSessionTransactionController(){};

    // Summary:
    //  Destructor.
    virtual ~ISipSessionTransactionController(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipSessionTransactionController(IN const ISipSessionTransactionController& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipSessionTransactionController& operator=(IN const ISipSessionTransactionController& rSrc);


//-- Hidden Data Members
protected:
private:
};


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_ISIPSESSIONTRANSACTIONCONTROLLER_H

