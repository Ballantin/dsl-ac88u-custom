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
#ifndef MXG_IXCAPHTTPCLIENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXCAPHTTPCLIENT_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_XCAPCLIENTCFG_H
#include "Config/XcapClientCfg.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class IXcapHttpClientMgr;
class CMimeObject;
class CTlsContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IXcapHttpClient);
//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: IXcapHttpClient
//========================================
//<GROUP TID_PACKAGE_XCAP_HTTP_CLASSES>
//
//  Summary:
//      This interface defines a simple abstraction layer for XCAP to access an HTTP
//  stack.
//
// Description:
//  This interface defines a simple abstraction layer for XCAP to access an HTTP
//  stack.
//
//  It is expected that multiple instances of the component implementing this
//  interface can be used simultaneously.
//
//  The implementation must be asynchronous and must not block on the calls to
//  the various Send requests.
//
//  It is expected that the implementation will automatically manage the
//  following HTTP headers:
//
//  <B>Request Headers</B>
//      - Authorization - Automatically managed and inserted when Authenticate
//                        is called.
//      - Host - Automatically set when needed.
//      - Proxy-Authorization - Automatically managed and inserted when
//                              Authenticate is called.
//  <B>Response Headers</B>
//      - Proxy-Authenticate - Automatically interpreted. Implementation must
//                             remember challenges.
//      - WWW-Authenticate - Automatically interpreted. Implementation must
//                           remember challenges.
//
//  <B>Entity Headers</B>
//      - Content-Length - Automatically set in requests and used from responses
//                         when necessary.
//
//  <B>General Headers</B>
//      - Connection - Managed automatically. Connections SHOULD be reused.
//      - Trailer - Managed automatically.
//      - Transfer-Encoding - Managed automatically.
//
//  The implementation must not include any Accept header into the
//  request. This has the meaning that any media type is accepted in responses.
//
//  #Authentication Behaviour#
//  The HTTP implementation manages HTTP digest authentication to a certain
//  level. Whenever a 401 or a 407 response is received, the implementation
//  reports that event to its manager. It is up to the manager to get the
//  credentials from the user or from local storage. Once it has the
//  credentials, the manager must call Authenticate to configure the credentials
//  and then it should try to resend the original request that was challenged.
//  The HTTP client implmentation will not automatically resend the challenged
//  request once Authenticate is called, it is up to the manager to do so.
//
//  #Custom HTTP Error Codes#
//  This interface defines the following HTTP error codes that can be used to
//  report special error conditions when failing to receive an actual HTTP
//  response to a request.
//      - 701 - Unable to resolve host name.
//      - 702 - Unable to connect to server.
//      - 703 - Unable to verify server certificate.
//
// Location:
//   Http/IXcapHttpClient.h
//
// See Also:
//   IXcapHttpClientMgr
//
//
//==============================================================================
class IXcapHttpClient : public IEComUnknown
{
//M5T_INTERNAL_USE_BEGIN
//
// Analysis notes. Documentation must not be exported, but it can be useful to
// keep this information here for now.
//
// A next revision of this interface should eventually be defined. This new
// revision should be using a CHttpHeaderList (specialized instance from the
// TextMessage package) to allow passing all possible request headers to
// SendXyz methods, and also to receive all headers in the event reported by
// the implementation of this interface. With this new interface revision, we
// could think about having a better HTTP abstraction that would be useful for
// other uses beyond XCAP.
//
// The following HTTP headers should be configurable when the XCAP component
// sends a request:
//         - If-Match - Required for conditional access.
//         - If-None-Match - Required for conditional access.
//         - Content-Type - Specifies payload type.
//
// The following HTTP headers should be accessible when the XCAP component
// receives an HTTP response:
//         - ETag - Required for conditional access.
//         - Location - Could be useful, but not used in the current XCAP
//                      implementation.
//         - Content-Type - Specifies payload type.
//
//M5T_INTERNAL_USE_END

//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IXcapHttpClient);
    //M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  Activate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Initializes the HTTP stack instance and assigns it with a thread to
    //      use.
    //
    //  Parameters:
    //    pThread:
    //      Pointer to a servicing thread object that this instance should use
    //      to access messaging, timer, and socket services. It can be NULL, in
    //      which case a new servicing thread can be created.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Initializes the HTTP stack instance and assigns it with a thread to
    //      use.
    //
    //      It is mandatory to call this method before calling any other method
    //      from this interface.
    //
    //      This method provides to the HTTP Client implementation a hint
    //      regarding which thread it should use. Nothing prevents an
    //      implementation to always create a new thread even if a servicing
    //      thread is passed here. However, it is expected that the M5T
    //      implementation of this interface will use the passed thread to
    //      maximize resource sharing.
    //
    //==========================================================================
    virtual mxt_result Activate(IN IEComUnknown* pThread = NULL) = 0;



    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the HTTP manager.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Sets the the manager to which events are reported.
    //
    //  See Also:
    //      IXcapHttpClient
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IXcapHttpClientMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetProxy
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures a proxy server to use when sending HTTP requests.
    //
    //  Parameters:
    //    pszAddress:
    //      IP address or FQDN of the HTTP proxy. NULL means that no proxy is to
    //      be used.
    //
    //    uPort:
    //      Port where to establish the connection.
    //
    //    bUseTls:
    //      true to use TLS to connect to the proxy server, false otherwise.
    //
    //  Description:
    //      Configures a proxy server to use when sending HTTP requests.
    //
    //==========================================================================
    virtual void SetProxy(IN const char* pszAddress,
                          IN unsigned short uPort,
                          IN bool bUseTls = false) = 0;

    //==========================================================================
    //==
    //==  SendGet
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a GET HTTP request.
    //
    //  Parameters:
    //    opqRequest:
    //      Opaque parameter that is returned to the caller when the HTTP
    //      implementation returns an event. It can be used to identify this
    //      transaction.
    //
    //    pszAbsoluteUrl:
    //      The complete HTTP URI, including the scheme. It cannot be NULL. The
    //      URI must have previously been escaped.
    //
    //    pszIfNoneMatchHeaderValue:
    //      The value to set into the If-None-Match header. It can be NULL, in
    //      which case the header is not sent.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Sends a GET request to the specified HTTP URI.
    //
    //      This method returns resS_OK to inform the caller that the request is
    //      processing asynchronously. The response to the GET request is
    //      returned through the IXcapHttpClientMgr manager.
    //
    //      Note that a response is returned in any case, even if the
    //      implementation was not able to resolve the URI or connect to the
    //      server. Under such conditions, a response is generated locally.
    //
    //==========================================================================
    virtual mxt_result SendGet(IN mxt_opaque opqRequest,
                               IN const char* pszAbsoluteUrl,
                               IN const char* pszIfNoneMatchHeaderValue = NULL) = 0;

    //==========================================================================
    //==
    //==  SendPut
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a PUT HTTP request.
    //
    //  Parameters:
    //    opqRequest:
    //      Opaque parameter that is returned to the caller when the HTTP
    //      implementation returns an event. It can be used to identify this
    //      transaction.
    //
    //    pszAbsoluteUrl:
    //      The complete HTTP URI, including the scheme. It cannot be NULL. The
    //      URI must have previously been escaped.
    //
    //    pMimeObject:
    //      Pointer to a CMimeObject that holds the content to send along with
    //      the content headers to use in the request. The Content-Type header
    //      is the only mandatory header to be included in the CMimeObject.
    //
    //    pszIfMatchHeaderValue:
    //      The value to set into the If-Match header. It can be NULL, in which
    //      case the header is not sent.
    //
    //    pszIfNoneMatchHeaderValue:
    //      The value to set into the If-None-Match header. It can be NULL, in
    //      which case the header is not sent.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Sends a PUT request to the specified HTTP URI.
    //
    //      This method returns resS_OK to inform the caller that the request is
    //      processing asynchronously. The response to the PUT request is
    //      returned through the IXcapHttpClientMgr manager.
    //
    //      Note that a response is returned in any case, even if the
    //      implementation was not able to resolve the URI or if it was not able
    //      to connect to the server. Under such conditions, a response is
    //      generated locally.
    //
    //==========================================================================
    virtual mxt_result SendPut(IN mxt_opaque opqRequest,
                               IN const char* pszAbsoluteUrl,
                               IN TOA CMimeObject* pMimeObject,
                               IN const char* pszIfMatchHeaderValue = NULL,
                               IN const char* pszIfNoneMatchHeaderValue = NULL) = 0;


    //==========================================================================
    //==
    //==  SendDelete
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a DELETE HTTP request.
    //
    //  Parameters:
    //    opqRequest:
    //      Opaque parameter that is returned to the caller when the HTTP
    //      implementation returns an event. It can be used to identify this
    //      transaction.
    //
    //    pszAbsoluteUrl:
    //      The complete HTTP URI, including the scheme. It cannot be NULL. The
    //      URI must have previously been escaped.
    //
    //    pszIfMatchHeaderValue:
    //      The value to set into the If-Match header. It can be NULL, in which
    //      case the header is not sent.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Sends a PUT request to the specified HTTP URI.
    //
    //      This method returns resS_OK to inform the caller that the request is
    //      processing asynchronously. The response to the DELETE request is
    //      returned through the IXcapHttpClientMgr manager.
    //
    //      Note that a response is returned in any case, even if the
    //      implementation was not able to resolve the URI or connect to the
    //      server. Under such conditions, a response is generated locally.
    //
    //==========================================================================
    virtual mxt_result SendDelete(IN mxt_opaque opqRequest,
                                  IN const char* pszAbsoluteUrl,
                                  IN const char* pszIfMatchHeaderValue = NULL) = 0;

    //==========================================================================
    //==
    //==  Authenticate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures authentication information for this user.
    //
    //  Parameters:
    //    pszRealm:
    //      The realm for which the user is authenticating. It canot be NULL.
    //
    //    pszUserName:
    //      The user name. It can be NULL if the user wants to inform the HTTP
    //      implementation that it has no credentials for the specified realm.
    //
    //    pszPassword:
    //      The password associated with the user name. It can be NULL if
    //      pszUserName is NULL.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Configures the authentication information for the specified user and
    //      realm.
    //
    //      This method configures the future credentials to use for the
    //      specified realm. It also automatically resends the challenged
    //      requests.
    //
    //==========================================================================
    virtual mxt_result Authenticate(IN const char* pszRealm,
                                    IN const char* pszUserName,
                                    IN const char* pszPassword) = 0;

    //==========================================================================
    //==
    //==  SetTlsContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the TLS context to use when establishing HTTPS
    //      connections.
    //
    //  Parameters:
    //    pTlsContext:
    //      Pointer to the TLS context. Ownership is always taken.
    //
    //  Description:
    //      Configures the TLS context to use when establishing HTTPS
    //      connections.
    //
    //===========================================================================
    virtual void SetTlsContext(IN TOA CTlsContext* pTlsContext) = 0;

    //==========================================================================
    //==
    //==  SetRedirectionFollowup
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the recursivity when HTTP requests are redirected by 3xx
    //      responses.
    //
    //  Parameters:
    //      uMaxNumRedirection:
    //       The maximum number of times a request is redirected before an
    //       event is reported to the manager. To disable automatic redirection,
    //       call this method with 0. The default value is 5.
    //
    //  Description:
    //      The XCAP HTTP client can try the next location when a PUT, GET or
    //      DELETE HTTP operation is redirected by the XCAP server using a 3xx
    //      response class.
    //
    //      A redirected HTTP request is tried to the next location (using the
    //      Location header) when this method is called with bRecurse set to
    //      true.
    //
    //==========================================================================
    virtual void SetRedirectionFollowup(IN unsigned int uMaxNumRedirection = 5) = 0;


//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IXcapHttpClient(){};

    // Summary:
    //  Destructor.
    virtual ~IXcapHttpClient(){};


protected:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IXcapHttpClient(IN const IXcapHttpClient& rSrc);

    // Summary:
    //  Assignment Operator.
    IXcapHttpClient& operator=(IN const IXcapHttpClient& rSrc);


//-- Hidden Data Members
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IXCAPHTTPCLIENT_H

