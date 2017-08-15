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
#ifndef MXG_ISCEOODREQUESTSENDER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEOODREQUESTSENDER_H
//M5T_INTERNAL_USE_END


//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_SIPMETHODTYPES_H
#include "SceSipTypes/SipMethodTypes.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CHeaderList;
class CNameAddr;
class CSceCallerPreferences;
class CSipMessageBody;
class ISceOodRequestSenderMgr;
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceOodRequestSender);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceOodRequestSender
//==============================================================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   The Out of Dialog Request Sender component can be used to send out of
//   dialog requests.
//
// Description:
//   The Out of Dialog Request Sender component can be used to send out of
//   dialog requests. Requests sent through this component must not create a
//   dialog. This component cannot receive requests. Out of dialog requests are
//   received by the default handler instead.
//
// Location:
//   SceCoreComponents/ISceOodRequestSender.h
//
//==============================================================================
class ISceOodRequestSender : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceOodRequestSender);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetOodRequestSenderManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the manager for this component.
    //
    //  Parameters:
    //    pMgr:
    //      The manager for this component. Must not be NULL.
    //
    //  Returns:
    //      - resS_OK: the manager is properly updated.
    //      - resFE_INVALID_ARGUMENT: the pMgr parameter is NULL.
    //      - resFE_INVALID_STATE: the manager is already set.
    //
    //  Description:
    //      Sets the manager where this component will report responses received
    //      for requests sent through SendOodRequest.
    //
    //  See Also:
    //      SendOodRequest, ISceOodRequestSenderMgr
    //
    //==========================================================================
    virtual mxt_result SetOodRequestSenderManager(IN ISceOodRequestSenderMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the user configuration for this component.
    //
    //  Parameters:
    //    pConfig:
    //      The user configuration.
    //
    //  Returns:
    //      - resS_OK: The user configuration is properly set.
    //      - resFE_INVALID_ARGUMENT: the pConfig parameter is NULL.
    //      - resFE_INVALID_STATE: the user configuration is already set.
    //
    //  Description:
    //      Sets the user configuration for this component.
    //
    //  See Also:
    //      ISceUserConfig
    //
    //==========================================================================
    virtual mxt_result SetConfig(IN ISceUserConfig* pConfig) = 0;

    //==========================================================================
    //==
    //==  GetConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the components's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the components.
    //      
    //==========================================================================
    virtual void GetConfig(OUT ISceUserConfig*& rpConfig) = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque paramerer to the component.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the component. It can be used by the
    //      application to store any information.
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;


    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the opaque parameter to the component.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the component. It can be used by
    //      the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

// BRCM_CUSTOM - [mod] Add force destination cfg support
    //==========================================================================
    //==
    //==  SetPeerAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the peer address for the requests to send with this component.
    //
    //  Parameters:
    //    rPeerAddr:
    //      The peer address where to send future requests.  This updates the
    //      Request-URI and To header of future requests.
    //
    //  Returns:
    //      - resS_OK: the peer address is properly set.
    //      - Any other result means an error occurred.
    //
    //  Description:
    //      Sets the peer address for this component. This must be
    //      called before SendOodRequest is called.
    //
    //      This address can be changed during the life of the component.
    //      However, there must be no pending request (request for which
    //      EvOodResponse with a final response was not called yet) for the peer
    //      address to be changed. If an ood request must be sent to another
    //      target before a pending ood request terminates, a new
    //      ISceOodRequestSender instance must be created to send that
    //      request.
    //
    //      Note that when this address is changed, it will reset the underlying
    //      context. This means that it will reset the authentication state,
    //      redirection state and any state that was associated with the
    //      underlying context (CSeq, Call-ID, From tag will also change).
    //
    //  See Also:
    //      SetTargetAddr, SendOodRequest
    //
    //==========================================================================
    virtual mxt_result SetPeerAddr(IN const CNameAddr& rPeerAddr) = 0;
// BRCM_CUSTOM - [end] Add force destination cfg support

// BRCM_CUSTOM - [add] Add force destination cfg support
    //==========================================================================
    //==
    //==  SetTargetAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the target address for the requests to send with this component.
    //      This does not change the To or the Request-URI fields.
    //
    //  Parameters:
    //    rTargetAddr:
    //      The target address where to send future requests.
    //
    //  Returns:
    //      - resS_OK: the target address is properly set.
    //      - Any other result means an error occurred.
    //
    //  Description:
    //      Sets the destination where to send future requests. This must be
    //      called before SendOodRequest is called.
    //
    //      This address can be changed during the life of the component.
    //      However, there must be no pending request (request for which
    //      EvOodResponse with a final response was not called yet) for the target
    //      address to be changed. If an ood request must be sent to another
    //      target before a pending ood request terminates, a new
    //      ISceOodRequestSender instance must be created to send that
    //      request.
    //
    //      Note that if the target address is not configured via this function, 
    //      future requests will be sent to the current proxy.
    //
    //  See Also:
    //      SetPeerAddr, SendOodRequest
    //
    //==========================================================================
    virtual mxt_result SetTargetAddr(IN const CNameAddr& rTargetAddr) = 0;
// BRCM_CUSTOM - [end] Add force destination cfg support

    //==========================================================================
    //==
    //==  SendOodRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends an out of dialog request.
    //
    //  Parameters:
    //    eMethodType:
    //      The method type of the request to send.
    //
    //    opqTransaction:
    //      Application data that is opaque to this component. This data will be
    //      given back with responses to this request in the
    //      ISceOodRequestSenderMgr::EvOodResponse event.
    //
    //    pExtraHeaders:
    //      Additional headers to put in the request to send.
    //
    //    pMessageBody:
    //      Message body information for the request to send.
    //
    //    pCallerPreferences:
    //      The caller preferences to include in the request. Ownership is taken.
    //      May be NULL.
    //
    //  Returns:
    //      - resS_OK: The request is successfully sent. Responses to this
    //        request will be reported through the
    //        ISceOodRequestSenderMgr::EvOodResponse() event.
    //      - resFE_INVALID_STATE: The manager, the user configuration or the
    //        peer address is not set.
    //      - resFE_FAIL: the request could not be sent by the stack.
    //
    //  Description:
    //      This method sends an out of dialog request. That request must have
    //      no impact on dialog management. In other words, the method must not
    //      be INVITE, BYE, CANCEL, UPDATE or SUBSCRIBE. The component only
    //      generates headers that are independent of the method. If there are
    //      headers that are mandatory for the specific method type the
    //      application sends through this method, they must be passed to the
    //      extra headers parameter by the application. For instance, the
    //      'Event' and the 'Subscription-State' headers for NOTIFY requests
    //      must be passed through the extra headers parameter.
    //
    //      If the method succeeds, the responses (0 to many provisional and 1
    //      final) for this request are reported to the
    //      ISceOodRequestSenderMgr::EvOodResponse method. The opaque
    //      transaction passed to this method is given back to the event.
    //
    //  See Also:
    //      ISceOodRequestSenderMgr::EvOodResponse
    //
    //==========================================================================
    virtual mxt_result SendOodRequest(IN ESipMethodType eMethodType,
                                      IN mxt_opaque opqTransaction,
                                      IN TO CHeaderList* pExtraHeaders,
                                      IN TO CSipMessageBody* pMessageBody,
                                      IN TO CSceCallerPreferences* pCallerPreferences) = 0;

protected:
    //M5T_INTERNAL_USE_BEGIN
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceOodRequestSender() {}

    // Summary:
    //  Destructor.
    virtual ~ISceOodRequestSender() {}
    //M5T_INTERNAL_USE_END
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEOODREQUESTSENDER_H



