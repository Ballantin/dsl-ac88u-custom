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
#ifndef MXG_IXCAPCLIENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXCAPCLIENT_H
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
//-------------------------
class IXcapRequestMgr;
class CTxtHostPort;
class CTlsContext;
class CMimeObject;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IXcapClient);
//M5T_INTERNAL_USE_END



//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IXcapClient
//========================================
//<GROUP TID_PACKAGE_XCAP_CLASSES>
//
// Summary:
//  This interface defines the basic operations that can be done on a XCAP
//  client implementation.
//
// Description:
//  This interface defines the basic operations that can be done on a XCAP
//  client implementation.
//
//  This XCAP client interface is able to manage documents for a single user on
//  a single server.
//
//  The XCAP client interface supports conditional operation on XCAP resources.
//  This is done by using the <I>pszEntityTag</I> parameter in the methods that
//  support this. Conditional operations allow the user of this interface to
//  conditionally get an XCAP resource only if the resource was changed, and
//  also update or delete a resource only if it was not changed on the server.
//
//  The various Set methods are synchronous. This way, the configuration is
//  synchronously updated before it is used (for example, calling SetXcapServer
//  before calling Get).
//
//  Activate MUST be called before using any method on this interface.
//
// Location:
//   Xcap/IXcapClient.h
//
// See Also:
//   IXcapRequestMgr
//
//==============================================================================
class IXcapClient : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:

//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IXcapClient);
//M5T_INTERNAL_USE_END


    //  Summary:
    //      The different types of URI used.
    //
    // Description:
    //  Defines the type of URI used when contacting an XCAP server.
    enum EUriType
    {
        // Description:
        //  Uses an HTTP URI when contacting the XCAP server. This means that
        //  the data transported between the server and the client is not
        //  encrypted and the server is not authenticated.
        eURI_HTTP,

        // Description:
        //  Uses an HTTPS URI when contacting the XCAP server. This means that
        //  the data transported between the server and the client is
        //  encrypted. Furthermore, the server certificate is verified
        //  when establishing the connection, ensuring that the client is
        //  connected to the proper server.
        eURI_HTTPS
    };


    //==========================================================================
    //==
    //==  Activate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Initializes the XCAP client instance and assigns it with a thread to
    //      use.
    //
    //  Parameters:
    //    pThread:
    //      Pointer to a servicing thread object that this instance must use to
    //      access messaging, timer, and socket services. It can be NULL, in
    //      which case a new servicing thread (and thus a new thread) is created.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Initializes the XCAP client instance and assigns it with a thread to
    //      use.
    //
    //      It is mandatory to call this method before calling any other method
    //      from this interface. Not calling Activate will yield to crashes if
    //      other methods of this interface are called.
    //
    //==========================================================================
    virtual mxt_result Activate(IN IEComUnknown* pThread = NULL) = 0;


    //==========================================================================
    //==
    //==  SetHttpProxy
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the HTTP proxy to use to access the XCAP server.
    //
    //  Parameters:
    //    rHostPort:
    //      Reference to a CTxtHostPort that identifies the address of the
    //      proxy server along with the port to which connect.
    //
    //    eUriType:
    //      Defines whether the client must connect using HTTP or HTTPS.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Sets the HTTP proxy to use to access the XCAP server on which the
    //      configuration operations will be performed. This is a setting for
    //      the HTTP stack.
    //
    //==========================================================================
    virtual mxt_result SetHttpProxy(IN const CTxtHostPort& rHostPort,
                                    IN EUriType eUriType) = 0;

    //==========================================================================
    //==
    //==  SetXcapServer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the XCAP server to which communicate.
    //
    //  Parameters:
    //    rHostPort:
    //      Reference to a CTxtHostPort that identifies the address of the
    //      server along with the port to which connect.
    //
    //    eUriType:
    //      Defines whether the client must connect using HTTP or HTTPS.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Sets the XCAP server on which the configuration operations will be
    //      performed.
    //
    //==========================================================================
    virtual mxt_result SetXcapServer(IN const CTxtHostPort& rHostPort,
                                     IN EUriType eUriType) = 0;

    //==========================================================================
    //==
    //==  SetTlsContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the TLS context to use when establishing a TLS connection
    //      to the XCAP server or HTTP proxy server.
    //
    //  Parameters:
    //    pContext:
    //      Pointer to the TLS context to use if establishing an HTTPS
    //      connection. It must not be NULL. Ownership is always taken if the
    //      pointer is not NULL.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Configures the TLS context to use when establishing a TLS connection
    //      to the XCAP server or HTTP proxy server.
    //
    //==========================================================================
    virtual mxt_result SetTlsContext(IN TOA CTlsContext* pContext) = 0;

    //==========================================================================
    //==
    //==  SetUserName
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the user name used when communicating with the XCAP
    //      server.
    //
    //  Parameters:
    //    pszUserName:
    //      Pointer to a c-style string containing the user name to be used on
    //      the XCAP server.
    //
    //  Description:
    //      Sets the name of the user that this IXcapClient instance represents.
    //      This user name is later used to build the URI when accessing
    //      documents that are in the user path (EDocumentPath::ePATH_USER). The
    //      username is also known as the XUI (XCAP User Identifier). It is
    //      recommended for SIP applications that the XUI is the
    //      address-of-record (AOR).
    //
    //      It is not mandatory to configure a user name if this IXcapClient
    //      instance is used only to access the global path using
    //      EDocumentPath::ePATH_GLOBAL as the ePath parameter of the various
    //      Get, Put, and Delete methods of this class.
    //
    //  See Also:
    //      EDocumentPath, GetUserName
    //
    //==========================================================================
    virtual void SetUserName(IN const char* pszUserName) = 0;

    //==========================================================================
    //==
    //==  GetUserName
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured the user name used when communicating with the
    //      XCAP server.
    //
    //  Parameters:
    //    rpszUserName:
    //      Pointer to a c-style string containing the configured user name used
    //      by the XCAP server.
    //
    //  Description:
    //      Gets the name of the user that this IXcapClient instance represents.
    //      The username is also known as the XUI (XCAP User Identifier).
    //
    //  See Also:
    //      EDocumentPath, SetUserName
    //
    //==========================================================================
    virtual void GetUserName(OUT const char*& rpszUserName) = 0;

    //==========================================================================
    //==
    //==  SetDigestCredentials
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the credentials of the user to access the server.
    //
    //  Parameters:
    //    pszRealm:
    //      Pointer to a c-style string containing the realm for which the
    //      credentials are created.
    //
    //    pszUserName:
    //      Pointer to a c-style string containing the user name. It can be NULL,
    //      in which case the implementation assumes that no credentials can be
    //      set for this realm.
    //
    //    pszPassword:
    //      Pointer to a c-style string containing the password associated with
    //      the user name.
    //
    //      This parameter can be NULL if the user of the IXcapClient does not
    //      want or cannot provide authentication information.
    //
    //  Description:
    //      Configures the credentials of the user for accessing the XCAP
    //      server.
    //
    //==========================================================================
    virtual void SetDigestCredentials(IN const char* pszRealm,
                                      IN const char* pszUserName,
                                      IN const char* pszPassword) = 0;

    //  Summary:
    //      The different document's path type.
    //
    // Description:
    //  This enumeration defines the path of the document.
    enum EDocumentPath
    {
        // Description:
        //  Corresponds to a document that is found under the user's directory
        //  and is usually private to this user.
        ePATH_USER,

        // Description:
        //  Corresponds to a document that is found under the global directory,
        //  which is accessible to all users.
        ePATH_GLOBAL
    };


    //==========================================================================
    //==
    //==  Get
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets a document or element from the XCAP server.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager for this request. Must not be NULL.
    //
    //    pszAuid:
    //      Pointer to a c-style string specifying the application usage ID.
    //      It must not be NULL.
    //
    //    pszDocName:
    //      Pointer to a c-style string specifying the name of the document.
    //      It must not be NULL.
    //
    //    pszNodeSelector:
    //      Pointer to a c-style string specifying the node selector. Can be
    //      NULL, in which case the operation applies to the whole document.
    //
    //    opqAppTransactionId:
    //      Application-provided opaque transaction identifier. This parameter
    //      is not used by the XCAP client and it simply returns this value when
    //      it reports an event pertaining to this operation.
    //
    //    pszEntityTag:
    //      Pointer to a c-style string specifying the current entity-tag
    //      received from a previous event. In the context of GetDocument, this
    //      value is used to conditionally get a document only if the local
    //      version and the server version are not the same.
    //
    //      It can be NULL, in which case the document is unconditionally 
    //      fetched from the server.
    //
    //      This entity-tag is used as an If-None-Match HTTP header.
    //
    //    ePath:
    //      Location of the document in the document tree.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Gets a document from the XCAP server. The document is fetched
    //      asynchronously and the EvGetResult event is reported.
    //
    //  See Also:
    //      IXcapRequestMgr::EvGetResult
    //
    //==========================================================================
    virtual mxt_result Get(IN IXcapRequestMgr* pMgr,
                           IN const char* pszAuid,
                           IN const char* pszDocName,
                           IN const char* pszNodeSelector,
                           IN mxt_opaque opqAppTransactionId,
                           IN const char* pszEntityTag = NULL,
                           IN EDocumentPath ePath = ePATH_USER) = 0;


    //==========================================================================
    //==
    //==  Put
    //==
    //==========================================================================
    //
    //  Summary:
    //      Places or updates a document or element on the XCAP server.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager for this request. Must not be NULL.
    //
    //    pszAuid:
    //      Pointer to a c-style string specifying the application usage ID.
    //      It must not be NULL.
    //
    //    pszDocName:
    //      Pointer to a c-style string specifying the name of the document.
    //      It must not be NULL.
    //
    //    pszNodeSelector:
    //      Pointer to a c-style string specifying the node selector. Can be
    //      NULL, in which case the operation applies to the whole document.
    //
    //    pMimeObject:
    //      Pointer to the MIME object that holds the document to send to the
    //      server. It must not be NULL.
    //
    //    opqAppTransactionId:
    //      Application-provided opaque transaction identifier. This parameter
    //      is not used by the XCAP client and it simply returns this value when
    //      it reports an event pertaining to this operation.
    //
    //    pszEntityTagMatch:
    //      Pointer to a c-style string specifying the current entity-tag
    //      received from a previous event. In the context of PutDocument, this
    //      value is used to conditionally update an existing document only if
    //      the local version and the server version are the same.
    //
    //      It can be NULL, in which case the document is unconditionally 
    //      uploaded on the server and possibly replaces any existing document 
    //      of the same name.
    //
    //      This parameter is used as an If-Match HTTP header.
    //
    //    pszEntityTagNoneMatch:
    //      Pointer to a c-style string specifying the entity-tag that the file
    //      held by the server must not match in order to accept the file.
    //      
    //      Can be NULL, in which case this parameter is not used.
    //
    //      This parameter is used as an If-None-Match HTTP header.
    //      
    //    ePath:
    //      Where the document is located in the document tree.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Places a document on the XCAP server. This method can be used to
    //      either create a new document or replace an existing document.
    //
    //      pszEntityTag can be used to prevent the replacement of an up-to-date
    //      document with outdated information.
    //
    //  See Also:
    //      IXcapRequestMgr::EvPutResult
    //
    //==========================================================================
    virtual mxt_result Put(IN IXcapRequestMgr* pMgr,
                           IN const char* pszAuid,
                           IN const char* pszDocName,
                           IN const char* pszNodeSelector,
                           IN TOA CMimeObject* pMimeObject,
                           IN mxt_opaque opqAppTransactionId,
                           IN const char* pszEntityTagMatch = NULL,
                           IN const char* pszEntityTagNoneMatch = NULL,
                           IN EDocumentPath ePath = ePATH_USER) = 0;


    //==========================================================================
    //==
    //==  Delete
    //==
    //==========================================================================
    //
    //  Summary:
    //      Deletes a document or element from the XCAP server.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager for this request. Must not be NULL.
    //
    //    pszAuid:
    //      Pointer to a c-style string specifying the application usage ID.
    //      It must not be NULL.
    //
    //    pszDocName:
    //      Pointer to a c-style string specifying the name of the document.
    //      It must not be NULL.
    //
    //    pszNodeSelector:
    //      Pointer to a c-style string specifying the node selector. Can be
    //      NULL, in which case the operation applies to the whole document.
    //
    //    opqAppTransactionId:
    //      Application-provided opaque transaction identifier. This parameter
    //      is not used by the XCAP client and it simply returns this value when
    //      it reports an event pertaining to this operation.
    //
    //    pszEntityTag:
    //      Pointer to a c-style string specifying the current entity-tag
    //      received from a previous event. In the context of DeleteDocument,
    //      this value is used to conditionally delete an existing document only
    //      if the local version and the server version are the same.
    //
    //      It can be NULL, in which case the document is unconditionally 
    //      deleted from the server.
    //
    //      This parameter is used as an If-Match HTTP header.
    //
    //    ePath:
    //      Location of the document in the document tree.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Deletes a document from the XCAP server.
    //
    //  See Also:
    //      IXcapRequestMgr::EvDeleteResult
    //
    //==========================================================================
    virtual mxt_result Delete(IN IXcapRequestMgr* pMgr,
                              IN const char* pszAuid,
                              IN const char* pszDocName,
                              IN const char* pszNodeSelector,
                              IN mxt_opaque opqAppTransactionId,
                              IN const char* pszEntityTag = NULL,
                              IN EDocumentPath ePath = ePATH_USER) = 0;

    //==========================================================================
    //==
    //==  SetRedirectionFollowup
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the maximum number of times a request can be redirected.
    //
    //  Parameters:
    //      uMaxNumRedirection:
    //       The maximum number of times a request is redirected before an
    //       event is reported to the manager. To disable automatic redirection,
    //       call this method with 0. The default value is 5.
    //
    //  Description:
    //      The XCAP HTTP client can try the next location when a GET HTTP
    //      operation is redirected by the XCAP server using a 3xx response
    //      class.
    //
    //      A redirected HTTP request is tried to the next location (using the
    //      Location header) when this method is called with uMaxNumRedirection
    //      greater than 0.
    //
    //==========================================================================
    virtual void SetRedirectionFollowup(IN unsigned int uMaxNumRedirection = 5) = 0;


//-- Hidden Methods
protected:

    // Summary:
    //  Default Constructor.
    IXcapClient() {};

    // Summary:
    //  Destructor.
    virtual ~IXcapClient() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IXcapClient(IN const IXcapClient& rSrc);

    // Summary:
    //  Assignment Operator.
    IXcapClient& operator=(IN const IXcapClient& rSrc);


//-- Hidden Data Members
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IXCAPCLIENT_H
