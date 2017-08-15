//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef CXCAPCURLHTTPCLIENT_H
//M5T_INTERNAL_USE_BEGIN
#define CXCAPCURLHTTPCLIENT_H

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

#if defined(MXD_XCAP_LIBCURL_ENABLE_SUPPORT)

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef __CURL_CURL_H
#include "curl/curl.h"
#endif

#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_IXCAPHTTPCLIENT_H
#include "Http/IXcapHttpClient.h"
#endif

#ifndef MXG_CMIMEOBJECT_H
#include "MimeMessage/CMimeObject.h"
#endif

#ifndef HEADER_OPENSSL_TYPES_H
#include "openssl/ossl_typ.h"
#endif

#ifndef HEADER_SSL_H 
#include "openssl/ssl.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CTlsContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CXcapCurlHttpClient
//==============================================================================
//<GROUP CXcapHttpClient>
//
// Summary:
//   This class is the CURL implementation of the simple abstraction layer for
//   XCAP.
//
// Description:
//   This class is the CURL implementation of the interface defined in
//   IXcapHttpClient.h. When using this interface, XCAP will use the Curl HTTP
//   stack to send and receive HTTP requests.
//
// Location:
//   Http/CXcapCurlHttpClient.h
//
//==============================================================================
class CXcapCurlHttpClient : protected CEComUnknown,
                            protected CEventDriven,
                            protected IXcapHttpClient
{
//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:

    //-- << Initialization mechanism >>
    static mxt_result InitializeCXcapCurlHttpClient();
    static void FinalizeCXcapCurlHttpClient();

    //-- << Static public methods >>
    // Summary:
    //  Sets the certificates file to be used by Curl.
    static void SetTrustedCertificateFile(IN const char* pszCertificatePath);


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComUnknown >>
    MX_DECLARE_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    // Summary:
    //  This function gets called by libcurl as soon as there is data received
    //  that needs to be saved.
    static unsigned int CurlWriteCallback(IN void* pData,
                                          IN unsigned int uMaxSize,
                                          IN unsigned int uSizeMult,
                                          IN void* pStream);

    // Summary:
    //  This function gets called by libcurl as soon as it needs to read data in
    //  order to send it to the peer.
    static unsigned int CurlReadCallback(OUT void* pData,
                                         IN unsigned int uMaxSize,
                                         IN unsigned int uSizeMult,
                                         IN void* pStream);

    // Summary:
    //  This function gets called by libcurl as soon as it has received header
    //  data.
    static unsigned int CurlHeaderCallback(OUT void* pData,
                                           IN unsigned int uMaxSize,
                                           IN unsigned int uSizeMult,
                                           IN void* pStream);

    // Summary:
    //  This function gets called by libcurl just before the initialization of
    //  an SSL connection.
    static CURLcode CurlContextCallback(IN CURL *pCurlHandle,
                                        OUT void* pVoidSslCtx,
                                        IN void *pParam);


    //-- << IXCapHttpClient >>

    virtual mxt_result Activate(IN IEComUnknown* pThread);

    virtual mxt_result SetManager(IN IXcapHttpClientMgr* pMgr);

    virtual void SetProxy(IN const char* pszAddress,
                          IN unsigned short uPort,
                          IN bool bUseTls);

    virtual mxt_result SendGet(IN mxt_opaque opqRequest,
                               IN const char* pszAbsoluteUrl,
                               IN const char* pszIfNoneMatchHeaderValue);

    virtual mxt_result SendPut(IN mxt_opaque opqRequest,
                               IN const char* pszAbsoluteUrl,
                               IN TOA CMimeObject* pMimeObject,
                               IN const char* pszIfMatchHeaderValue,
                               IN const char* pszIfNoneMatchHeaderValue);

    virtual mxt_result SendDelete(IN mxt_opaque opqRequest,
                                  IN const char* pszAbsoluteUrl,
                                  IN const char* pszIfMatchHeaderValue);

    virtual mxt_result Authenticate(IN const char* pszRealm,
                                    IN const char* pszUserName,
                                    IN const char* pszPassword);

    virtual void SetTlsContext(IN TOA CTlsContext* pTlsContext);

    virtual void SetRedirectionFollowup(IN unsigned int uMaxNumRedirection);

    //-- << CEventDriven >>
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessageId,
                                           IN CMarshaler* pParameter);


//-- Hidden Methods
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CXcapCurlHttpClient(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CXcapCurlHttpClient();

    //-- << Other methods >>

    // Summary:
    //  Sets the last received E-Tag.
    void SetReceivedETag(const char* pszETag);

    // Summary:
    //  Gets the MIME object used to store the received MIME headers and
    //  content.
    CMimeObject*& GetReceivedMimeObject();

    // Summary:
    //  Gets the MIME object to send.
    CMimeObject*& GetMimeObjectToSend();

    // Summary:
    //  Gets the TLS context.
    CTlsContext* GetTlsContext();

    // Summary:
    //  Sets the challenge realm.
    void SetChallengeRealm(IN const char* pszrealm);

    // Summary:
    //  Displays the last OpenSsl error.
    void DisplayErrors();

    // Summary:
    //  Function set in SSL_CTX_set_verify.
    static int VerifyCallback(IN int nOk, IN X509_STORE_CTX* pStore);

    // Summary:
    //  Notifies the manager of curl_easy_perform failure and HTTP answers.
    void ProcessResponse(IN CURLcode code, IN CMarshaler* pParameter);

    // Summary:
    //  Notifies the manager of curl_easy_perform failure and HTTP answers.
    void NotifyManager(IN unsigned int uRequestType,
                       IN mxt_opaque opqRequest,
                       IN unsigned int uResponseCode);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CXcapCurlHttpClient(IN const CXcapCurlHttpClient& rSrc);

    // Summary:
    //  Assignment Operator.
    CXcapCurlHttpClient& operator=(IN const CXcapCurlHttpClient& rSrc);


//-- Hidden Data Members
protected:
private:
    // Enum representing the requested HTTP operation (or method).
    enum ERequestType
    {
        eHTTP_REQUEST_GET,
        eHTTP_REQUEST_PUT,
        eHTTP_REQUEST_DELETE
    };

    // This instance manager.
    IXcapHttpClientMgr* m_pMgr;

    // Curl handle.
    CURL* m_pCurl;

    // Last received E-Tag.
    CString m_strLastETagReceived;

    // Received MIME object.
    CMimeObject* m_pMimeObjectReceived;

    // MIME object to send in the next request.
    CMimeObject* m_pNextMimeObjectToSend;

    // TLS context.
    CTlsContext* m_pTlsContext;

    // The path to the trusted certificate file.
    static const char* ms_pszTrustedCertificate;

    // Struct representing a challenge.
    struct SChallenge
    {
        // Challenge code (401 or 407).
        unsigned int m_uCode;

        // The type of request that generated the challenge.
        unsigned int m_uRequestType;

        // Realm of the challenge.
        char* m_pszRealm;

        // If this was a Digest challenge.
        bool m_bIsDigest;

        // The opaque of request that generated the challenge.
        mxt_opaque m_opqRequest;
    };

    SChallenge m_stChallenge;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SetReceivedETag
//==
//==============================================================================
//
//  Parameters:
//      pszETag:
//       Received E-Tag header value.
//
//  Description:
//      Sets the last received E-Tag.
//
//==============================================================================
inline void CXcapCurlHttpClient::SetReceivedETag(const char* pszETag)
{
    m_strLastETagReceived = pszETag;
}

//==============================================================================
//==
//==  GetReceivedMimeObject
//==
//==============================================================================
//
//  Returns:
//      Pointer reference to the MIME object used to store received MIME header
//      and content.
//
//  Description:
//      Gets the MIME object used to store the received MIME headers and
//      content.
//
//==============================================================================
inline CMimeObject*& CXcapCurlHttpClient::GetReceivedMimeObject()
{
    if (m_pMimeObjectReceived == NULL)
    {
        m_pMimeObjectReceived = MX_NEW(CMimeObject)(false);
    }

    return m_pMimeObjectReceived;
}

//==============================================================================
//==
//==  GetMimeObjectToSend
//==
//==============================================================================
//
//  Returns:
//      Pointer reference to the MIME object to send in the next request.
//
//  Description:
//      Gets the MIME object to send in the next request.
//
//==============================================================================
inline CMimeObject*& CXcapCurlHttpClient::GetMimeObjectToSend()
{
    return m_pNextMimeObjectToSend;
}

//==============================================================================
//==
//==  GetTlsContext
//==
//==============================================================================
//
//  Returns:
//      Pointer to the TLS context used by this object.
//
//  Description:
//      Gets the TLS context used by this object's instance.
//
//==============================================================================
inline CTlsContext* CXcapCurlHttpClient::GetTlsContext()
{
    return m_pTlsContext;
}

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #if defined(MXD_XCAP_LIBCURL_ENABLE_SUPPORT)

#endif //-- #ifndef CXCAPCURLHTTPCLIENT_H

