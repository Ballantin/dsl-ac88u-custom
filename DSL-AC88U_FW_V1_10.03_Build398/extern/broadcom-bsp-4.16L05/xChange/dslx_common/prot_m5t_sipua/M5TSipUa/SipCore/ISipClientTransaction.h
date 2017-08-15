//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_ISIPCLIENTTRANSACTION_H
#define MXG_ISIPCLIENTTRANSACTION_H

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


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHeaderList;
class CSipMessageBody;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipClientTransaction);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipClientTransaction
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//  This interface represents an ongoing client transaction within the stack.
//
//  The application can associate an opaque data with each transaction it
//  creates. This opaque data, which is configured when calling the API that
//  creates and sends a request, is also accessible through this interface.
//
//  A client transaction can be cancelled at any time before a final response is
//  received to the request. Simply use the CancelRequest API of this interface
//  to cancel a client transaction.
//
//  This interface is an ECOM object and as such is reference counted.
//
//  The stack user that will never cancel a transaction is not required to keep
//  a reference to a client transaction. For instance, any request other than
//  INVITE requests should never be cancelled. Thus it is possible for the stack
//  user to never keep a reference on the client transaction for all requests,
//  except INVITE requests.
//
//  If the stack user did keep a reference to the client transaction, then it
//  must release its reference when the transaction completes, that is when a
//  final response is received.
//
//  Refer to the M5T SIP Stack Programmer's Guide for more information on how to
//  use this interface for managing client transactions.
//
// Location:
//   SipCore/ISipClientTransaction.h
//
// See Also:
//   ISipClientEventControl
//
//==EDOC========================================================================
class ISipClientTransaction: public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:

    // Description:
    //  Used to specify how cancel commands should behave when called before
    //  reception of a provisionnal response.
    enum ECancelRequestBehavior
    {
        // Description:
        //   Buffer the CANCEL until reception of a response.
        eCANCELBEHAVIOR_RFC3261_COMPLIANT,

        // Description:
        //   Same behavior as RFC3261 except that retransmissions of INVITE
        //   requests are stopped if no answers were received. Only has effect
        //   on UAC INVITE transactions.
        eCANCELBEHAVIOR_STOP_RETRANSMISSION_WHEN_BUFFERING_CANCEL
    };

    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipClientTransaction);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  CancelRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Attempts to cancel client transaction.
    //
    //  Parameters:
    //    pExtraHeaders:
    //      Additional headers to put in the CANCEL request. It must be NULL to
    //      be RFC 3261 compliant. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      Message-body information to put in the CANCEL request. It must be
    //      NULL to be RFC 3261 compliant. Ownership is TAKEN.
    //
    //  Returns:
    //      - resS_OK: The Cancel request is being sent or has been buffered
    //                 until a 1xx class response is received.
    //      - resFE_FAIL: Invalid state for sending a CANCEL request.
    //
    //  Description:
    //      Attempts to cancel the request sent on the client transaction by
    //      creating a CANCEL request corresponding to that original request.
    //      The CANCEL is sent as soon as a provisional response to that
    //      original request is received. If a provisional response has already
    //      been received, the CANCEL is sent immediately.
    //
    //      This method can only be called if no final response has been
    //      received for the original transaction.
    //
    //      Since CANCEL is a 'hop by hop' method, additional headers
    //      and content information that the application put in this method only
    //      reach the next hop. The application is not informed of the status of
    //      the CANCEL request. Instead, it is informed of the status of the
    //      original response. Note that the original request can still obtain a
    //      success response in the case where the success response was sent by
    //      the remote party before it received the CANCEL request.
    //
    //      It is not recommended to use CANCEL to cancel requests other than
    //      INVITE.
    //
    //      The ISipClientTransaction properly interops with the RFC 2543 style
    //      CANCEL, where a final response to the original request is never
    //      sent. In this case, the stack automaticaly generates a final
    //      response after a certain amount of time (64*T1).
    //
    //==EDOC====================================================================
    virtual mxt_result CancelRequest(IN TO CHeaderList* pExtraHeaders,
                                     IN TO CSipMessageBody*  pMessageBody) = 0;

    //==SDOC====================================================================
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
    //      also be accessed through ISipClientEventControl::GetOpaque.
    //
    //      It can be changed after the client transaction is created by using
    //      SetOpaque or ISipClientEventControl::SetOpaque.
    //
    //  See Also:
    //      SetOpaque
    //      ISipClientEventControl::GetOpaque
    //      ISipClientEventControl::SetOpaque
    //
    //==EDOC====================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==SDOC====================================================================
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
    //      through the ISipClientEventControl interface. Updating it from one
    //      interface impacts the opaque parameter accessible by both
    //      interfaces, as they relate to the same transaction.
    //
    //  See Also:
    //      GetOpaque
    //      ISipClientEventControl::GetOpaque
    //      ISipClientEventControl::SetOpaque
    //
    //==EDOC====================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;

    //==SDOC====================================================================
    //==
    //==  SetCancelRequestBehavior
    //==
    //==========================================================================
    //
    //  Description:
    //    Sets the behavior of the CancelRequest() method.
    //
    //  Parameters:
    //    eCancelBehavior:
    //      Compliant to RFC3261 if not specified. If set to
    //      eCANCELBEHAVIOR_STOP_RETRANSMISSION_WHEN_BUFFERING_CANCEL, the
    //      retransmission of INVITE are stopped when a CANCEL cannot be sent
    //      because no answers were received for the request. Only stops INVITE
    //      retransmissions.
    //
    //==EDOC====================================================================
    virtual void SetCancelRequestBehavior(IN ECancelRequestBehavior eCancelBehavior) = 0;


//-- Hidden Methods
//-------------------
protected:

    //-- << Protected Constructors / Destructors >>
    //-------------------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipClientTransaction(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipClientTransaction(){}



private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipClientTransaction(IN const ISipClientTransaction& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipClientTransaction& operator=(IN const ISipClientTransaction& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCLIENTTRANSACTION_H

