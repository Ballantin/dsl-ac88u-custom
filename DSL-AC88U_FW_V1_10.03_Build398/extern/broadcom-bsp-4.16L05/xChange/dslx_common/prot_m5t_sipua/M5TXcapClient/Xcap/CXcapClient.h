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
#ifndef MXG_CXCAPCLIENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CXCAPCLIENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_IXCAPCLIENT_H
#include "Xcap/IXcapClient.h"
#endif

#ifndef MXG_IXCAPHTTPCLIENTMGR_H
#include "Http/IXcapHttpClientMgr.h"
#endif

#ifndef MXG_CTXTHOSTPORT_H
#include "TextMessage/CTxtHostPort.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CMarshaler;
class CTlsContext;
class CMimeObject;
class IXcapHttpClient;
class CTxtToken;
class IXmlDocument;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CXcapClient
//==============================================================================
//
// Description:
//   This class implements the XCAP client using the XCAP HTTP Client
//   abstraction layer. It reports events to its manager upon receiving HTTP
//   responses to the requests it sent.
//
// IMPLEMENTATION'S NOTES
//
//  The implementation is an event driven. It activates its HTTP session with
//  the same thread as the one it is currently using, thus there is no need to
//  synchronize the HTTP stack events.
//
//  The various operations on this class are performed synchronously
//  (Set methods) or asynchronously (Document methods) if called from a
//  different thread than the thread configured through the Activate method.
//  The operations are all performed synchronously when the methods are called
//  from the same thread (same execution context).
//
//  The various operations will fail to post a message using PostMessage if
//  the Activate method was not called. Therefore the Activate method MUST
//  be called prior to calling any other method on this object.
//
//
// Location:
//   Xcap/CXcapClient.h
//
// See Also:
//   IXcapClient
//
//==============================================================================
class CXcapClient : private CEComUnknown,
                    private CEventDriven,
                    private IXcapClient,
                    private IXcapHttpClientMgr
{
    //-- Friend Declarations

    friend class CXcapClientInitializer;

//-- Published Interface
public:
//-- Hidden Methods
protected:
private:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CXcapClient();

    // Summary:
    //  Destructor.
    virtual ~CXcapClient();

    //-- << Initialization mechanism >>
    static mxt_result InitializeCXcapClient();

    static void FinalizeCXcapClient();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_IECOMUNKNOWN

    //-- Inherited from CEComUnknown.

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    // Inherited from CEventDriven.

    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessageId,
                                           IN CMarshaler* pParameter);

    // Inherited from IXcapClient.

    virtual mxt_result Activate(IN IEComUnknown* pThread = NULL);

    virtual mxt_result SetHttpProxy(IN const CTxtHostPort& rHostPort,
                                    IN EUriType eUriType);

    virtual mxt_result SetXcapServer(IN const CTxtHostPort& rHostPort,
                                     IN EUriType eUriType);

    virtual mxt_result SetTlsContext(IN TOA CTlsContext* pContext);

    virtual void SetUserName(IN const char* pszUserName);

    virtual void GetUserName(OUT const char*& rpszUserName);

    virtual void SetDigestCredentials(IN const char* pszRealm,
                                      IN const char* pszUserName,
                                      IN const char* pszPassword);

    virtual mxt_result Get(IN IXcapRequestMgr* pMgr,
                           IN const char* pszAuid,
                           IN const char* pszDocName,
                           IN const char* pszNodeSelector,
                           IN mxt_opaque opqAppTransactionId,
                           IN const char* pszEntityTag,
                           IN EDocumentPath ePath);

    virtual mxt_result Put(IN IXcapRequestMgr* pMgr,
                           IN const char* pszAuid,
                           IN const char* pszDocName,
                           IN const char* pszNodeSelector,
                           IN TOA CMimeObject* pMimeObject,
                           IN mxt_opaque opqAppTransactionId,
                           IN const char* pszEntityTagMatch,
                           IN const char* pszEntityTagNoneMatch,
                           IN EDocumentPath ePath);

    virtual mxt_result Delete(IN IXcapRequestMgr* pMgr,
                              IN const char* pszAuid,
                              IN const char* pszDocName,
                              IN const char* pszNodeSelector,
                              IN mxt_opaque opqAppTransactionId,
                              IN const char* pszEntityTag,
                              IN EDocumentPath ePath);

    virtual void SetRedirectionFollowup(IN unsigned int uMaxNumRedirection);

    // Inherited from IXcapHttpClientMgr.

    virtual void EvGetResponse(IN mxt_opaque opqRequest,
                               IN unsigned int uResponse,
                               IN const char* pszETagHeaderValue,
                               IN TOA CMimeObject* pMimeObject);

    virtual void EvPutResponse(IN mxt_opaque opqRequest,
                               IN unsigned int uResponse,
                               IN const char* pszETagHeaderValue,
                               IN TOA CMimeObject* pMimeObject);

    virtual void EvDeleteResponse(IN mxt_opaque opqRequest,
                                  IN unsigned int uResponse,
                                  IN const char* pszETagHeaderValue,
                                  IN TOA CMimeObject* pMimeObject);

    virtual void EvAuthenticationRequired(IN const char* pszRealm);

    // Helper methods

    // Summary:
    //  Sets the HTTP Proxy on the XCAP HTTP Client.
    void InternalOnSetProxy(IN CMarshaler* pParameter);

    // Summary:
    //  Sets the XCAP server.
    void InternalOnSetServer(IN CMarshaler* pParameter);

    // Summary:
    //  Sets the TLS context on the XCAP HTTP Client.
    void InternalOnSetTlsContext(IN CMarshaler* pParameter);

    // Summary:
    //  Sets the digest credentials on XCAP HTTP Client.
    void InternalOnSetDigestCredentials(IN CMarshaler* pParameter);

    // Summary:
    //  Sets the redirection followup on XCAP HTTP Client.
    void InternalOnSetRedirectionFollowup(IN CMarshaler* pParameter);

    // Used to identify the asynchronous message posted internally by this event
    // driven.
    enum EMessage
    {
        eSET_MANAGER,
        eSET_PROXY,
        eSET_SERVER,
        eSET_TLS_CONTEXT,
        eSET_USERNAME,
        eGET_USERNAME,
        eSET_DIGEST_CREDENTIALS,
        eSET_REDIRECTION_FOLLOWUP,
        eGET_DOCUMENT,
        ePUT_DOCUMENT,
        eDELETE_DOCUMENT
    };

    // Summary:
    //  Treats the GET, PUT or DELETE HTTP request.
    void InternalOnRequest(IN EMessage eMessage,
                           IN CMarshaler* pParameter);

    // Summary:
    //  Sets the XCAP server data members.
    void SetServer(IN TOA CTxtHostPort* pHostPort,
                   IN EUriType eUriType);

    // Enum representing the requested HTTP operation (or method).
    enum EResponseEventType
    {
        eGET,
        ePUT,
        eDELETE
    };

    // Summary:
    //  Helper method that reports proper response event to the manager.
    void EvResponse(IN EResponseEventType eEventType,
                    IN mxt_opaque opqRequest,
                    IN unsigned int uResponse,
                    IN const char* pszETagHeaderValue,
                    IN TOA CMimeObject* pMimeObject);

    // Summary:
    //  Creates the XCAP URI using the server, the username and the provided
    //  parameters.
    void CreateUri(IN EDocumentPath ePath,
                   IN const char* pszAuid,
                   IN const char* pszDocName,
                   IN const char* pszNodeSelector,
                   INOUT CBlob& rBlobUri);

    // Summary:
    //  Parses the MIME object to find a xcap-error XML document.
    void Parse(IN const CMimeObject& rMimeObject,
               OUT GO IXmlDocument** ppXmlDocument);

    // Summary:
    //  Recursively finds a XCAP message in the MIME object.
    const CMimeObject* FindXcapMessage(IN const CMimeObject* pObject);

    // Summary:
    //  Requests a document (either GET, PUT or DELETE) on the XCAP HTTP Client.
    mxt_result Request(IN IXcapRequestMgr* pMgr,
                       IN EMessage eMessage,
                       IN bool bIsAsynchronous,
                       IN const char* pszAuid,
                       IN const char* pszDocName,
                       IN const char* pszNodeSelector,
                       IN mxt_opaque opqAppTransactionId,
                       IN const char* pszEntityTagMatch,
                       IN const char* pszEntityTagNoneMatch,
                       IN EDocumentPath ePath,
                       IN TOA CMimeObject* pMimeObject = NULL);

    // Summary:
    //  Requests a document by providing the complete URI.
    mxt_result Request(IN IXcapRequestMgr* pMgr,
                       IN EMessage eMessage,
                       IN bool bIsAsynchronous,
                       IN CBlob& rblobUri,
                       IN mxt_opaque opqAppTransactionId,
                       IN const char* pszEntityTagMatch,
                       IN const char* pszEntityTagNoneMatch,
                       IN TOA CMimeObject* pMimeObject = NULL);

    // This method will take the next request in the buffer and execute it.
    void NextRequest(IN unsigned int uLastResponseCode,
                     IN const char* pszEntityTag,
                     IN IXmlDocument* pErrorDocument);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CXcapClient(IN const CXcapClient& rSrc);

    // Summary:
    //  Assignment Operator.
    CXcapClient& operator=(IN const CXcapClient& rSrc);

// Friends declarations

//-- Hidden Data Members
protected:
private:
    // The XCAP http client on which requests are posted.
    IXcapHttpClient* m_pXcapHttpClient;

    // The XCAP request manager on which events (related to the xcap http
    // abstraction layer) are reported to the application. This is the
    // manager of the current request.
    IXcapRequestMgr* m_pMgr;

    // The attributes (the host and the URI type) used to set the XCAP HTTP
    // Server.
    CTxtHostPort* m_pServerHostPort;
    EUriType m_eServerUriType;

    // Username used when communicating with the XCAP server.
    CTxtToken* m_ptokUserName;

    // Structure that hold the differents options for a request.
    struct SRequestInfo
    {
        // The manager of this request
        IXcapRequestMgr* m_pMgr;

        // The operation this request will make
        EMessage m_eMessage;

        // The URI of the request
        CBlob m_blobUri;

        // The id of the transaction
        mxt_opaque m_opqAppTransactionId;

        // The entity tag of the document
        CString m_pstrEntityTagMatch;

        // The entity tag of the document
        CString m_pstrEntityTagNoneMatch;

        // The mime object that will be send for a put request
        CMimeObject* m_pMimeObject;
    };

    // Vector buffering request that need to be send next.
    CVector<SRequestInfo*> m_vecpBuffer;

    // Bool indicating if we are currently waiting for a request to terminate.
    bool m_bIsWaitingOnRequest;
};

//==============================================================================
//====  Helper METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SetServer
//==
//==============================================================================
//
//  Summary:
//      Sets the XCAP server attributes.
//
//  Parameters:
//    pHostPort:
//      Pointer to a CTxtHostPort that identifies the address of the
//      server along with the port to connect to.
//
//    eUriType:
//      Defines whether the client must connect using http or https.
//
//  Description:
//      Helper method to set the XCAP server attributes.
//
//==============================================================================
inline void CXcapClient::SetServer(IN TOA CTxtHostPort* pHostPort,
                                   IN EUriType eUriType)
{
    if (m_pServerHostPort != NULL)
    {
        MX_DELETE(m_pServerHostPort);
    }
    m_pServerHostPort = TOA pHostPort;
    m_eServerUriType = eUriType;
}

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CXCAPCLIENT_H

