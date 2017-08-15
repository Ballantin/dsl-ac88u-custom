//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_IUAIMPPXCAPCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPXCAPCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCEUAIMPPCFG_H
#include "Config/SceUaImppCfg.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CNameAddr;
class IUaImppXcapDocumentSubscriber;
class IUri;
class IXcapClient;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IUaImppXcapConfig);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IUaImppXcapConfig
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Description:
//  This configuration interface (which is part of the SCE user configuration
//  object) gives access to XCAP components that are associated with the user
//  represented by the configuration object. Two XCAP-related components are
//  managed by this configuration object:
//  - The IXcapClient, which is used to send XCAP requests. All XCAP requests
//    from this user are going through this IXcapClient instance, which ensures
//    that existing TCP connections are reused.
//  - The IUaImppXcapDocumentSubscriber, which is used to subscribe (with SIP)
//    to the state of documents held by an XCAP server. Again, the same
//    instance is used for all documents monitored by this user.
//
//#IXcapClient Management#
//  The IXcapClient instance is created and activated the first time 
//  GetXcapClient is called. It is also automatically configured with the 
//  following parameters:
//    - IXcapClient::SetUserName is set to the AOR of the user, which
//      corresponds to the URI found in ISceUserConfig::GetUserAddress;
//    - IXcapClient::SetRedirectionFollowup is set to 5;
//
//  The application MUST configure the following parameter from the IXcapClient
//  before it creates any component that requires the use of XCAP for this user:
//    - IXcapClient::SetXcapServer, with the address of the XCAP server that
//      is associated with this user.
//
//  The application can further customize the IXcapClient by using the following
//  APIs:
//      - IXcapClient::SetUserName to override the XCAP user name;
//      - IXcapClient::SetHttpProxy to set an HTTP proxy to use;
//      - IXcapClient::SetTlsContext to set trusted TLS certificates;
//      - IXcapClient::SetRedirectionFollowup to override the default number of 
//        times that an HTTP request can be redirected;
//
//  XCAP operations performed by the SIP Client Engine components can be
//  challenged using digest authentication. The application is notified of such
//  challenges through the ISceUserAuthenticationMgr interface, as it is the
//  case with SIP. The application can set credentials using the
//  ISceUserAuthentication interface of the user configuration object associated
//  with the user.
//
//#IUaImppXcapDocumentSubscriber Management#
//  The IUaImppXcapDocumentSubscriber instance is created the first time
//  GetXcapDocumentSubscriber is called. It is then initialized with the values
//  set in SetXcapNotifier and SetSubscriptionExpirationTime.
//
//  An application that for some reason needs to subscribe to more than one XCAP
//  server can do so using another instance of a IUaImppXcapDocumentSubscriber,
//  but it must override the server to subscribe to using
//  IUaImppXcapDocumentSubscriber::SetNotifier.
//
// Location:
//   SceUaImpp/IUaImppXcapConfig.h
//
//==EDOC========================================================================
class IUaImppXcapConfig : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppXcapConfig);
//M5T_INTERNAL_USE_END

#if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

    //==========================================================================
    //==
    //==  GetXcapClient
    //==
    //==========================================================================
    //
    //  Summary:
    //    Gets this configuration object's Xcap Client.
    //
    //  Parameters:
    //    ppXcapClient:
    //      Double pointer which is filled with the Xcap Client. ppXcapClient
    //      will point to an ECOM which is counted, the user is resposible for
    //      releasing this reference.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Gets this configuration object's Xcap Client.
    //
    //==========================================================================
    virtual mxt_result GetXcapClient(OUT IXcapClient** ppXcapClient) = 0;

    //==========================================================================
    //==
    //==  GetXcapDocumentSubscriber
    //==
    //==========================================================================
    //
    //  Summary:
    //    Gets this configuration object's Xcap Document Subscriber.
    //
    //  Parameters:
    //    ppComponent:
    //      Double pointer which is filled with the Xcap Document Subscriber.
    //      ppComponent will point to an ECOM which is counted, the user is
    //      resposible for releasing this reference.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Gets this configuration object's Xcap Document Subscriber.
    //
    //==========================================================================
    virtual mxt_result GetXcapDocumentSubscriber(OUT IUaImppXcapDocumentSubscriber** ppComponent) = 0;

    //==========================================================================
    //==
    //==  SetXcapNotifier
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the server where to subscribe for XCAP document state.
    //
    //  Parameters:
    //    pResourceAor:
    //      The address of the resource to which the component must subscribe.
    //      Will be used in the To header. Ownership is taken.
    //
    //    pNotifierRequestUri:
    //      The address of the resource to which the component must subscribe.
    //      Will be used in the request-URI. Can be NULL, in which case the URI
    //      found in pResourceAor will be used instead. Ownership is taken.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Sets the server to use to monitor XCAP documents. 
    //      
    //      This method can only be called before GetXcapDocumentSubscriber is
    //      called.
    //
    //==========================================================================
    virtual mxt_result SetXcapNotifier(IN TOA CNameAddr* pResourceAor,
                                       IN TOA IUri* pNotifierRequestUri) = 0;

    //==========================================================================
    //==
    //==  GetXcapNotifier
    //==
    //==========================================================================
    //
    //  Summary:
    //    Gets the server where to subscribe for XCAP document state.
    //
    //  Parameters:
    //    ppResourceAor:
    //      Double pointer which is filled with the address of the resource to
    //      which the component must subscribe. Will be used in the To header.
    //
    //    ppNotifierRequestUri:
    //      Double pointer which is filled with the address of the resource to
    //      which the component must subscribe. Will be used in the request-URI.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Gets the server to use to monitor XCAP documents. 
    //      
    //==========================================================================
    virtual mxt_result GetXcapNotifier(OUT const CNameAddr** ppResourceAor,
                                       OUT const IUri** ppNotifierRequestUri) = 0;


    //==========================================================================
    //==
    //==  SetSubscriptionExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the requested expiration time of subscriptions.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //          uNbSeconds is 0.
    //      - resS_OK:
    //          the requested subscription expiration time has been
    //          modified.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to set as expiration time XCAP document
    //      subscriptions (ie. Added into the Expires header in SUBSCRIBE
    //      requests). This parameter MUST be different than 0.
    //
    //  Description:
    //      Configures the subscription expiration time requested by the
    //      IUaImppXcapDocumentSubscriber when it subscribes to the state of one
    //      or more documents.
    //
    //      It is possible for a server to lower the proposed expiration time.
    //
    //      A server might also reject the expiration time if it is too short,
    //      in which case the subscription will be retried with the shortest
    //      possible value supported by the server.
    //
    //      This defaults to 3600 seconds (one hour).
    //
    //      Note that the change does not affect already established
    //      subscriptions.
    //
    //  See Also:
    //      GetSubscriptionExpirationTime
    //
    //==========================================================================
    virtual mxt_result SetSubscriptionExpirationTime(IN uint32_t uNbSeconds) = 0;

    //==========================================================================
    //==
    //==  GetSubscriptionExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configured expiration time for a document subscription.
    //
    //  Returns:
    //      The number of seconds used for expiration time of subscriptions,
    //      in seconds.
    //
    //  Description:
    //      Returns the configured expiration time for a subscription, in
    //      seconds.
    //
    //  See Also:
    //      SetSubscriptionExpirationTime
    //
    //==========================================================================
    virtual uint32_t GetSubscriptionExpirationTime() const = 0;

#endif // #if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)


//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Default Constructor.
    //-----------------------
    IUaImppXcapConfig(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUaImppXcapConfig(){}



private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUaImppXcapConfig(IN const IUaImppXcapConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUaImppXcapConfig& operator=(IN const IUaImppXcapConfig& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

//M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUAIMPPXCAPCONFIG_H

