//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_UAIMPPPAGESENDER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_UAIMPPPAGESENDER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSceCallerPreferences;
class CSipMessageBody;
class IUaImppPageSenderMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IUaImppPageSender);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppPageSender
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//    The UA IMPP Page Sender component allows an application to send page mode
//    IM requests.
//
// Description:
//   The UA IMPP Page Sender component allows an application to send page mode
//   IM requests. More specifically, for the SIP protocol, this component sends
//   SIP MESSAGE requests.
//
//   The SIP MESSAGE request allows page mode instant messaging only. Thus, a
//   SIP session is not established between the two peers when they exchange
//   such instant messages. This means that two MESSAGE requests sent to the
//   same destination may end up reaching, and being handled by, different
//   endpoints. This behavior depends on the network configuration and on how
//   proxies in the path handle the request.
//
//   This component allows the application to send MESSAGE requests to a single
//   SIP user at a time. This destination is set using the SetPeerAddr() API and
//   can be changed provided that final responses were received to all previous
//   requests sent.
//
//   The SendMessage API allows the application to send any type of payload. It
//   is expected that applications that want to offer a rich IM experience will
//   use this API to send multipart messages that contains CPIM and quite
//   possibly pictures or images to display along with the message.
//
//   An instance of this component should only be released when responses to all
//   sent requests have been received.
//
// Location:
//   SceUaImpp/IUaImppPageSender.h
//
//==============================================================================
class IUaImppPageSender : public IEComUnknown
{
//-- Friend Declarations


//-- Published Interface
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppPageSender);
    //M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which this object reports events.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. Can be NULL.
    //
    //  Returns:
    //      - resS_OK: Manager set correctly.
    //
    //  Description:
    //      Configures the manager to which this object reports events. A
    //      manager must be configured before a page message can be sent with
    //      this object.
    //
    //      If set to NULL no more events will be reported.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IUaImppPageSenderMgr* pMgr) = 0;


    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the page sender object with a user configuration object.
    //
    //  Parameters:
    //      pConfig:
    //       Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resS_OK: Configuration correctly set.
    //      - resFE_INVALID_ARGUMENT: Configuration is NULL or invalid.
    //      - resFE_INVALID_STATE: Configuration is already set.
    //
    //  Description:
    //      Configures the page sender object with a user configuration object.
    //      A user configuration object must be configured before this component
    //      is used to send a page message.
    //      
    //==========================================================================
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig) = 0;

    //==========================================================================
    //==
    //==  GetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the page sender's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the page sender.
    //      
    //==========================================================================
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig) = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque paramerer to the page sender.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the page sender. It can be used by
    //      the application to store any information.
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
    //      Gets the opaque parameter to the page sender.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the page sender. It can be used by
    //      the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;


    //==========================================================================
    //==
    //==  SetPeerAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the target of page messages sent with this component.
    //
    //  Parameters:
    //    rPeerAddr:
    //      The address where to send future IM messages.
    //
    //  Returns:
    //      - resS_OK: the peer address was properly set.
    //      - Any other result means an error occured.
    //
    //  Description:
    //      Sets the destination where to send future IM requests.
    //
    //      This address can be changed during the life of the component.
    //      However, there must be no pending request (request for which
    //      EvResponse was not called yet) for the peer
    //      address to be changed. If a message must be sent to another
    //      target before a pending request terminates, a new
    //      IUaImppPageSender instance must be created to send that
    //      request.
    //
    //      Note that when this address is changed, it will reset the underlying
    //      SIP stack context. This means that it will reset authentication
    //      state, redirection state and any state that was associated with the
    //      underlying context (CSeq, Call-ID, From tag will also change).
    //
    //  See Also:
    //      SendMessage
    //
    //==========================================================================
    virtual mxt_result SetPeerAddr(IN const CNameAddr& rPeerAddr) = 0;


    //==========================================================================
    //==
    //==  SendMessage
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends an IM MESSAGE request to the configured peer with an
    //      application specified payload.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application transaction identifier.
    //
    //    pMessageBody:
    //      The message body to send. Ownership is taken. Ownership is always
    //      taken.
    //
    //    pCallerPreferences:
    //      The caller preferences to include in the request. Ownership is
    //      always taken. May be NULL.
    //
    //  Returns:
    //      - resS_OK: The request was successfully sent.
    //      - resFE_INVALID_STATE: The manager, the user configuration or the
    //        peer address is not set.
    //      - resFE_FAIL: The request could not be sent by the component.
    //
    //  Description:
    //      Sends a page mode IM request to the configured peer. This request is
    //      a MESSAGE request and is sent out-of-dialog. The component simply
    //      uses pMessageBody as the payload of the request.
    //
    //      If the method succeeds, the final responses for this request will be
    //      reported through IUaImppPageSenderMgr::EvResponse. The opaque
    //      transaction passed to EvResponse will be the same opaque as the
    //      opqTransaction passed to SendMessage. Also note that it is
    //      guaranteed that a final response will be reported for the request if
    //      this method succeeds.
    //
    //      It is possible to send an IM request without having received
    //      response to a previous request. The application must use
    //      opqTransaction to find out which response relates to which request,
    //      as there are no guarantees that the MESSAGE requests are delivered
    //      in the order they were sent.
    //
    //==========================================================================
    virtual mxt_result SendMessage(IN mxt_opaque opqTransaction,
                                   IN TO CSipMessageBody* pMessageBody,
                                   IN TO CSceCallerPreferences* pCallerPreferences) = 0;


//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaImppPageSender() {};

    // Summary:
    //  Destructor.
    virtual ~IUaImppPageSender() {};


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaImppPageSender(IN const IUaImppPageSender& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaImppPageSender& operator=(IN const IUaImppPageSender& rSrc);


//-- Hidden Data Members
protected:
private:

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_UAIMPPPAGESENDER_H

