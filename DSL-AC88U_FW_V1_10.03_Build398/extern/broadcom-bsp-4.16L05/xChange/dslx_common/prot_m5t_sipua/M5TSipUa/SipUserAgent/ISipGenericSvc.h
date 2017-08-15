//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2001 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPGENERICSVC_H
#define MXG_ISIPGENERICSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT to be able \
to use this class.
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
class ISipClientTransaction;
class ISipGenericMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipGenericSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipGenericSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   The generic service offers the possibility to send and receive any type of
//   SIP request and their responses. It can be used to send multiple requests
//   and receive their responses simultaneously. Generic requests are requests
//   that follow the simple request/response pattern, as opposed to
//   request/response/ACK. This means it can be used to manage non-INVITE and
//   non-ACK requests.
//
//   It SHOULD NOT be used to manage INVITEs on the UAS side, the session
//   service SHOULD be preferred for such a task. The generic service is not
//   tailored to manage all the complexities of received INVITEs.
//
//   This service is used for sending requests that are not directly
//   supported by other services such as OPTIONS, INFO, MESSAGE, etc. The
//   generic service can also be used for managing other types of requests that
//   are already managed by an existing service, such as UPDATE, REFER, or
//   REGISTER. However this is not usually recommended, unless the user has an
//   in-depth knowledge of the requirements for the other types of requests.
//
//   Even though the BYE request fits the "non-INVITE" request definition, it
//   should only be sent through the ISipSessionSvc in order for the Session
//   Service to maintain proper dialog state.
//
//   The Generic Service <C>SendRequest</C> method must <B>NOT</B> be
//   used to send CANCEL requests. Use the
//   <C>ISipClientTransaction::CancelRequest</C> method from the transaction to
//   cancel instead.
//
//   This service generates events to the application through the
//   ISipGenericMgr interface.
//
//#The ISipGenericSvc is an ECOM object#
//
//  The ISipGenericSvc is an ECOM object that is created and accessed through
//  the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipGenericSvc</B>
//
//  <B>Interface Id: IID_ISipGenericSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipGenericSvc.h
//
// See Also:
//   ISipGenericMgr, ISipContext
//
//==EDOC========================================================================
class ISipGenericSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipGenericSvc);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager that will receive events from this service.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. Must not be NULL.
    //
    //  Returns:
    //    resFE_FAIL:
    //      pMgr is NULL.
    //
    //    resS_OK:
    //      Otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events generated
    //      by this service.
    //
    //      Note that an event manager MUST be associated with this service
    //      before it is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipGenericMgr* pMgr) = 0;

    //==SDOC====================================================================
    //==
    //==  SendRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends the specified request.
    //
    //  Parameters:
    //    szMethod:
    //      The SIP method to send.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      The SIP headers to add in the packet. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      The packet's body, aka payload. Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Reference to the created client transaction.
    //
    //  Returns:
    //    resS_OK:
    //      Request is being sent. Feedback is provided as a response to
    //      the request through the service manager.
    //
    //    resFE_FAIL:
    //      Unable to send the request. No other feedback is provided for
    //      this request.
    //
    //
    //  Description:
    //      Sends the request over the network with the specified SIP headers
    //      and payload when present.
    //
    //      The reference to ISipClientTransaction allows users to further
    //      control the transaction. It is possible to cancel a generic request
    //      by using ISipClientTransaction::CancelRequest; however, this is not
    //      recommended. RFCs 4320 and 4321 highlight the problems related to
    //      cancelling a non-INVITE request.
    //
    //  See Also:
    //      ISipGenericSvc::EvResponse, ISipClientTransaction,
    //
    //===EDOC===================================================================
    virtual mxt_result SendRequest(
                               IN    const char*             szMethod,
                               IN    mxt_opaque              opqTransaction,
                               IN TO CHeaderList*            pExtraHeaders,
                               IN TO CSipMessageBody*        pMessageBody,
                               OUT   ISipClientTransaction*& rpTransaction) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipGenericSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipGenericSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipGenericSvc(IN const ISipGenericSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipGenericSvc& operator=(IN const ISipGenericSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPGENERICSVC_H

