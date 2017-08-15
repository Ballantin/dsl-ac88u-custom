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
#ifndef MXG_IXCAPREQUESTMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXCAPREQUESTMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_XCAPCLIENTCFG_H
#include "Config/XcapClientCfg.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CMimeObject;
class IXmlDocument;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IXcapRequestMgr
//========================================
//<GROUP TID_PACKAGE_XCAP_CLASSES>
//
// Summary:
//  This interface defines the events that can be reported to a user of the
//  IXcapClient.
//
// Description:
//  This interface defines the events that can be reported to the user of the
//  IXcapClient.
//
//  Note that if the implementation of this interface must somehow synchronize
//  access to its data if it does not run in the same thread as the thread that
//  was configured in IXcapClient::Activate, all events reported by the
//  IXcapClient are reported in the context of the thread configured in
//  Activate.
//
//  When perfoming an XCAP operation, the client can report existing HTTP
//  responses. Furthermore, the following response codes may be reported,
//  depending on the HTTP abstraction layer implementation:
//
//      - 701 - Unable to resolve the destination using DNS;
//      - 702 - Unable to connect to the resolved destination;
//      - 703 - Connected using HTTPS to the peer, but the provided certificate 
//              is not trusted.
//
// Location:
//   Xcap/IXcapRequestMgr.h
//
// See Also:
//   IXcapClient
//
//==============================================================================
class IXcapRequestMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //==
    //==  EvGetResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of a GET operation on the IXcapClient.
    //
    //  Parameters:
    //    opqAppTransactionId:
    //      The opaque parameter passed to the GET operation is returned through
    //      this parameter. This can serve as a transaction identifier.
    //
    //    uResponseCode:
    //      Response code for the GET request.
    //
    //    pMimeObject:
    //      Pointer to a MIME object that contains the document retrieved using
    //      the GET operation. It can be NULL if no document was available or if
    //      an error was returned.
    //
    //      Ownership of this pointer, when non NULL, must always be taken by
    //      the implementation of this event.
    //
    //    pszEntityTag:
    //      Pointer to the entity-tag received from the server, used to identify
    //      the version of the retrieved document. This entity tag can later be
    //      used to perform conditional operations on the document. It can be 
    //      NULL if the server returned an error or if the server did not return
    //      any entity tag.
    //
    //    pErrorDocument:
    //      Pointer to an IXmlDocument that holds an XCAP error document in the
    //      format "application/xcap-error+xml" defined in RFC 4825. This
    //      pointer should only be valid when eResult is eXCAP_ERROR_XCAP. This
    //      pointer can be NULL if no XCAP error document was sent along with
    //      the response.
    //
    //      The error document will remain valid for this event only and will be
    //      released by the IXcapClient after this call. If the application
    //      wants to keep this object for a longer period, it must add a
    //      reference to it.
    //
    //  Description:
    //      Reports the result of a GET operation on the IXcapClient.
    //
    //  See Also:
    //      IXcapClient::GetDocument
    //
    //==========================================================================
    virtual void EvGetResult(IN mxt_opaque opqAppTransactionId,
                             IN unsigned int uResponseCode,
                             IN TOA CMimeObject* pMimeObject,
                             IN const char* pszEntityTag,
                             IN IXmlDocument* pErrorDocument) = 0;

    //==========================================================================
    //==
    //==  EvPutResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of a GET operation on the IXcapClient.
    //
    //  Parameters:
    //    opqAppTransactionId:
    //      The opaque parameter passed to a PUT operation is returned through
    //      this parameter. This can serve as a transaction identifier.
    //
    //    uResponseCode:
    //      Response code for the PUT request.
    //
    //    pszEntityTag:
    //      Pointer to the entity-tag received from the server, used to
    //      identify the version of the updated or new document. This entity
    //      tag can later by used to perform conditional operations on the
    //      document. It can be NULL if the server returned an error or if the
    //      server simply did not return any entity tag.
    //
    //    pErrorDocument:
    //      Pointer to an IXmlDocument that holds an XCAP error document in the
    //      format "application/xcap-error+xml" defined in RFC 4825. This
    //      pointer should only be valid when eResult is eXCAP_ERROR_XCAP. This
    //      pointer can be NULL if no XCAP error document was sent along with
    //      the response.
    //
    //      The error document will remain valid for this event only and will be
    //      released by the IXcapClient after this call. If the application
    //      wants to keep this object for a longer period, it must add a
    //      reference to it.
    //
    //  Description:
    //      Reports the result of a PUT operation on the IXcapClient.
    //
    //  See Also:
    //      IXcapClient::PutDocument
    //
    //==========================================================================
    virtual void EvPutResult(IN mxt_opaque opqAppTransactionId,
                             IN unsigned int uResponseCode,
                             IN const char* pszEntityTag,
                             IN IXmlDocument* pErrorDocument) = 0;

    //==========================================================================
    //==
    //==  EvDeleteResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of a DELETE operation on the IXcapClient.
    //
    //  Parameters:
    //    opqAppTransactionId:
    //      The opaque parameter passed to the DELETE operation is returned
    //      through this parameter. This can serve as a transaction identifier.
    //
    //    uResponseCode:
    //      Response code for the DELETE request.
    //
    //    pszEntityTag:
    //      Pointer to the entity-tag received from the server, used to identify
    //      the version of the updated document. This entity tag can later by
    //      used to perform conditional operations on the document. It can be 
    //      NULL if the whole document was deleted, if the server returned an 
    //      error, or if the server simply did not return any entity tag.
    //
    //    pErrorDocument:
    //      Pointer to an IXmlDocument that holds an XCAP error document in the
    //      format "application/xcap-error+xml" defined in RFC 4825. This
    //      pointer should only be valid when eResult is eXCAP_ERROR_XCAP. This
    //      pointer can be NULL if no XCAP error document was sent along with
    //      the response.
    //
    //      The error document will remain valid for this event only and will be
    //      released by the IXcapClient after this call. If the application
    //      wants to keep this object for a longer period, it must add a
    //      reference to it.
    //
    //  Description:
    //      Reports the result of a DELETE operation on the IXcapClient.
    //
    //  See Also:
    //      IXcapClient::DeleteDocument
    //
    //==========================================================================
    virtual void EvDeleteResult(IN mxt_opaque opqAppTransactionId,
                                IN unsigned int uResponseCode,
                                IN const char* pszEntityTag,
                                IN IXmlDocument* pErrorDocument) = 0;


    //==========================================================================
    //==
    //==  EvAuthenticationRequired
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that authentication must be performed in order for an
    //      operation to be accepted by the server.
    //
    //  Parameters:
    //    pszRealm:
    //      The autorization realm returned by the server.
    //
    //  Description:
    //      Reports that authentication must be performed in order for an
    //      operation to be accepted by the server.
    //
    //      When this event is reported, the user of the IXcapClient must try to
    //      get a password from the user or from local storage and then call
    //      IXcapClient::SetDigestCredentials. This processing can be done
    //      asynchronously. Note that SetDigestCredentials MUST be called even
    //      if there is no password to send in order to release resources.
    //
    //  See Also:
    //      IXcapClient::SetDigestCredentials
    //
    //==========================================================================
    virtual void EvAuthenticationRequired(IN const char* pszRealm) = 0;


//-- Hidden Methods
protected:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IXcapRequestMgr(){};

    // Summary:
    //  Destructor.
    virtual ~IXcapRequestMgr(){};

private:
    // Summary:
    //  Copy Constructor.
    IXcapRequestMgr(IN const IXcapRequestMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IXcapRequestMgr& operator=(IN const IXcapRequestMgr& rSrc);


//-- Hidden Data Members
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IXCAPREQUESTMGR_H

