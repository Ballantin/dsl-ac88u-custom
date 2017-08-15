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
#ifndef MXG_ISIPTRANSACTIONCOMPLETIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPTRANSACTIONCOMPLETIONSVC_H
//M5T_INTERNAL_USE_END

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

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipTransactionCompletionMgr;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID( ISipTransactionCompletionSvc );

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipTransactionCompletionSvc
//==============================================================================
// <FLAG New in 4.1.4>
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   This service monitors ongoing transactions and indexes them by their type
//   of request. When all client and server transactions of a specific request
//   type are terminated, this service reports an event through its manager
//   interface. The application then knows that it can send a new request.
//
//   This service also allows to configure how the generic service behaves when
//   trying to initiate multiple transactions of the same request type in
//   parallel. By default, the generic service is able to manage multiple
//   parallel client and server transactions for the same type of request. By
//   using EnableParallelTransactions("method", false), the application 
//   prevents the generic service from creating a client transaction for a
//   specific request type when there is already an existing transaction (client
//   or server) for this request type. In other words, the default behaviour is
//   to allow parallel transactions for a specific request type, while calling
//   EnableParallelTransactions with false forces serializing the client
//   transactions at the application level.
//
//   This service generates events to the application through the
//   ISipTransactionCompletionMgr interface.
//
//   This service must be one of the last services to be attached to a context
//   in order to properly work. Only the following services can be attached to
//   a context after the ISipTransactionCompletedSvc:
//   ISipStatisticsSvc, ISipOutputControllingSvc, and ISipConnectionBlacklistSvc.
//
//#The ISipTransactionCompletionSvc is an ECOM object#
//
//  The ISipTransactionCompletionSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_ISipTransactionCompletionSvc</B>
//
//  <B>Interface Id: IID_ISipTransactionCompletionSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipTransactionCompletionSvc.h
//
// See Also:
//   ISipTransactionCompletionMgr
//
//==============================================================================
class ISipTransactionCompletionSvc : public IEComUnknown
{

//-- Published Interface
public:

    //-- << Stereotype >>

    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipTransactionCompletionSvc);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  EnableParallelTransactions
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets if parallel transactions are allowed for the specified method.
    //
    //  Parameters:
    //      rstrMethod:
    //       The method type to configure.
    //
    //      bEnable:
    //       - true to allow parallel transactions.
    //
    //       - false to disallow parallel transactions and force serial
    //         processing.
    //
    //  Description:
    //      Sets if the passed strMethod can have multiple parallel transactions.
    //      By default, a method allows parallel transactions. Note that this
    //      setting applies only to the ISipGenericSvc on the current
    //      ISipContext.
    //
    //==========================================================================
    virtual void EnableParallelTransactions(IN const CString& rstrMethod,
                                            IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the manager to notify of transaction completion.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager to notify of the transaction completion.
    //
    //  Description:
    //      Sets the transaction completion manager that is notified when
    //      all transactions for a specific request type are completed.
    //
    //==========================================================================
    virtual void SetManager(IN ISipTransactionCompletionMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  CanEstablishNewTransaction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells if a new transaction can be established.
    //
    //  Parameters:
    //      rstrMethod:
    //       The method that will create the transaction.
    //
    //  Returns:
    //      true if a new transaction based on the request strMethod can be
    //      established.
    //
    //  Description:
    //      Returns true if a new transaction based on the request strMethod
    //      is allowed to be created. A new transaction will be denied to be
    //      created only if there is already an ongoing transaction and that
    //      parallel transactions for this request type is set to false.
    //
    //==========================================================================
    virtual bool CanEstablishNewTransaction(IN const CString& rstrMethod) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipTransactionCompletionSvc() {};

    // Summary:
    //  Destructor.
    virtual ~ISipTransactionCompletionSvc() {};
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipTransactionCompletionSvc(IN const ISipTransactionCompletionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipTransactionCompletionSvc& operator=(IN const ISipTransactionCompletionSvc& rSrc);
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTRANSACTIONCOMPLETIONSVC_H

