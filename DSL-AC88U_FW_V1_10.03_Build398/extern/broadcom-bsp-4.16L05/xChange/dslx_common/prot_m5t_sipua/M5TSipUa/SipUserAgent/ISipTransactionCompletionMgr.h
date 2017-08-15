//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPTRANSACTIONCOMPLETIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPTRANSACTIONCOMPLETIONMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CString;
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipTransactionCompletionMgr
//==============================================================================
// <FLAG New in 4.1.4>
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//    The transaction completion manager is the interface through which the
//    ISipTransactionCompletionSvc reports its events to the application. It is
//    through this interface that the M5T SIP-UA informs the application
//    that a transaction is completed.
//
// Location:
//   SipUserAgent/ISipTransactionCompletionMgr.h
//
// See Also:
//   ISipTransactionCompletionSvc
//
//==============================================================================
class ISipTransactionCompletionMgr
{
//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvTransactionCompleted
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies that all transactions for a specific request type have been
    //      completed.
    //
    //  Parameters:
    //      rstrMethod:
    //       The method of the request for which all transactions were
    //       completed.
    //
    //      pContext:
    //       Pointer to the context associated with the transaction. Cannot be
    //       NULL.
    //
    //  Description:
    //      Reports that all transactions of the method type identified by
    //      rstrMethod are terminated on the context. This means that the stack
    //      is now ready to send another request that may have been buffered by
    //      the application.
    //
    //      This event is only reported when it is okay for the application to
    //      send a new request. When a request is sent or received and a final
    //      response that terminates the transaction is received or sent, the
    //      transaction count becomes zero and an asynchronous message is posted
    //      (this will eventually report EvTransactionCompleted when the message
    //      is processed). When the asynchronous message is processed, if the
    //      transaction count is still zero, then the event is reported. For some
    //      reasons, it is possible that the transaction count is not zero when
    //      the asynchronous message is processed. A request could either be
    //      received or sent while there is an asynchronous message in the queue
    //      of the transaction completion service. That explains why the event
    //      is not always reported after a transaction has terminated.
    //
    //      For example, an UPDATE request is sent and a final response is
    //      received, then an asynchronous message is posted. Before the
    //      asynchronous is processed, another UPDATE is received (the 
    //      transaction count is incremented). Hence when the asynchronous 
    //      message is processed, the event is not reported.
    //
    //  See Also:
    //      ISipTransactionCompletionSvc
    //
    //==========================================================================
    virtual void EvTransactionCompleted(IN const CString& rstrMethod,
                                        IN ISipContext* pContext) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipTransactionCompletionMgr() {};

    // Summary:
    //  Destructor.
    virtual ~ISipTransactionCompletionMgr() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipTransactionCompletionMgr(IN const ISipTransactionCompletionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipTransactionCompletionMgr& operator=(IN const ISipTransactionCompletionMgr& rSrc);

};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTRANSACTIONCOMPLETIONMGR_H
