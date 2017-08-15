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
#ifndef MXG_CMIMEOBJECT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMIMEOBJECT_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CMIMEHEADERLIST_H
#include "MimeMessage/CMimeHeaderList.h"
#endif

#ifndef MXG_MIMEHEADERGRAMMAR_H
#include "MimeMessage/MimeHeaderGrammar.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CMimeHeader;
class CSubAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CMimeObject
//========================================
//<GROUP MIME_MESSAGE_CLASSES>
//<TOPICORDER 2>
//
// Summary:
//  This class represents a MIME object that can be transported by various IETF
//  protocols, like SIP, HTTP or RTSP.
//
// Description:
//  This class represents a MIME object that can be transported by various IETF
//  protocols, like SIP, HTTP or RTSP.
//
//  This class supports single body parts and multipart MIME bodies. When
//  managing multipart mime bodies, this class will automatically set the
//  boundary parameter to properly separate the parts.
//
//  When using this class to build multipart bodies, keep in mind that "leaf"
//  parts of the multipart tree are either CMimeObject that were configured
//  with a blob content (using SetMimeContent) or other objects inheriting from
//  this class that can serialize their content into a blob.
//
//  Example:
//
//   <CODE>
//   multipart/mixed (multipart)
//   \|
//   \+ application/sdp (leaf, blob)
//     \|
//     \+ multipart/signed (multipart)
//       \|
//       \+ message/sipfrag (leaf, blob)
//         \|
//         \+ application/pkcs7-signature (leaf, blob)
//   </CODE>
//
//   The following API calls would have to be done on different CMimeObject
//   in order to build the above multipart object.
//
//   1. Create a new CMimeObject (pMainObject), call
//      AddHeader(eHDR_MIME_CONTENT_TYPE) and set the content-type to
//      "multipart/mixed".
//   <CODE>
//      CMimeObject* pMainObject = MX_NEW(CMimeObject);
//      CMimeHeader* pMimeHeader = pMainObject->AddHeader(eHDR_MIME_CONTENT_TYPE);
//      pMimeHeader->GetContentTypeMSubType() = "mixed";
//      pMimeHeader->GetContentTypeMType() = "multipart";
//   </CODE>
//
//   2. Create a new CMimeObject (pSdpObject) and call
//      AddHeader(eHDR_MIME_CONTENT_TYPE) and set the content-type to
//      "application/sdp". Further serialize the SDP information into a blob and
//      set the blob in pSdpObject using SetMimeContent.
//   <CODE>
//      CMimeObject* pSdpObject = MX_NEW(CMimeObject);
//      CMimeHeader* pMimeHeader = pSdpObject->AddHeader(eHDR_MIME_CONTENT_TYPE);
//      pMimeHeader->GetContentTypeMSubType() = "sdp";
//      pMimeHeader->GetContentTypeMType() = "application";
//      CBlob* pBlob = MX_NEW(CBlob);
//      mxt_result res = pBlob->Append("Sdp Object Content");
//      res = pSdpObject->SetMimeContent(TOA pBlob);
//      pBlob = NULL;
//   </CODE>
//
//   3. Create a new CMimeObject (pSignedObject) and call
//      AddHeader(eHDR_MIME_CONTENT_TYPE) and set the content-type to
//      "multipart/signed".
//   <CODE>
//      CMimeObject* pSignedObject = MX_NEW(CMimeObject);
//      CMimeHeader* pMimeHeader = pSignedObject->AddHeader(eHDR_MIME_CONTENT_TYPE);
//      pMimeHeader->GetContentTypeMSubType() = "signed";
//      pMimeHeader->GetContentTypeMType() = "multipart";
//   </CODE>
//
//   4. Create a new CMimeObject (pSipfragObject) and call
//      AddHeader(eHDR_MIME_CONTENT_TYPE) and set the content-type to
//      "message/sipfrag". Further serialize the SIP information into a blob and
//      set the blob in pSipfragObject using SetMimeContent.
//   <CODE>
//      CMimeObject* pSipfragObject = MX_NEW(CMimeObject);
//      CMimeHeader* pMimeHeader = pSipfragObject->AddHeader(eHDR_MIME_CONTENT_TYPE);
//      pMimeHeader->GetContentTypeMSubType() = "sipfrag";
//      pMimeHeader->GetContentTypeMType() = "message";
//      CBlob* pBlob = MX_NEW(CBlob);
//      mxt_result res = pBlob->Append("Sipfrag Object Content");
//      res = pSipfragObject->SetMimeContent(TOA pBlob);
//      pBlob = NULL;
//   </CODE>
//
//   5. Create a new CMimeObject (pSignatureObject) and call
//      AddHeader(eHDR_MIME_CONTENT_TYPE) and set the content-type to
//      "application/pkcs7-signature". Further serialize the signature
//      information into a blob and set the blob in pSignatureObject using SetMimeContent.
//   <CODE>
//      CMimeObject* pSignatureObject = MX_NEW(CMimeObject);
//      CMimeHeader* pMimeHeader = pSignatureObject->AddHeader(eHDR_MIME_CONTENT_TYPE);
//      pMimeHeader->GetContentTypeMSubType() = "pkcs7-signature";
//      pMimeHeader->GetContentTypeMType() = "application";
//      CBlob* pBlob = MX_NEW(CBlob);
//      mxt_result res = pBlob->Append("Signature Object Content");
//      res = pSignatureObject->SetMimeContent(TOA pBlob);
//      pBlob = NULL;
//   </CODE>
//
//   6. Build pSignedObject by configuring its two parts. Call
//      AddInnerPart(pSipfragObject) and then AddInnerPart(pSignatureObject) on
//      it.
//   <CODE>
//     pSignedObject->AddInnerPart(TOA pSipfragObject);
//     pSipfragObject = NULL;
//     pSignedObject->AddInnerPart(TOA pSignatureObject);
//     pSignatureObject = NULL;
//   </CODE>
//   
//   7. Build pMainObject by configuring its two parts. Call
//      AddInnerPart(pSdpObject) and then AddInnerPart(pSignedObject) on it.
//   <CODE>
//     pMainObject->AddInnerPart(TOA pSdpObject);
//     pSdpObject = NULL;
//     pMainObject->AddInnerPart(TOA pSignedObject);
//     pSignedObject = NULL;
//   </CODE>
//
// Location:
//   MimeMessage/CMimeObject.h
//
// See Also:
//      CMimeHeaderList, CMimeHeader
//
//==============================================================================
class CMimeObject
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CMimeObject(IN bool bParseExtensionHeaders = true);

    // Summary:
    //  Copy Constructor.
    CMimeObject(IN const CMimeObject& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CMimeObject();

    // Summary:
    //  Assignment Operator.
    CMimeObject& operator=(IN const CMimeObject& rSrc);

    // Summary:
    //  Gets a header from the header list.
    CMimeHeader* GetHeader(IN EMimeHeaderType eHeaderType);

    //<COMBINE CMimeObject::GetHeader@IN EMimeHeaderType>
    const CMimeHeader* GetHeader(IN EMimeHeaderType eHeaderType) const;

    // Summary:
    //  Adds a header to the header list or replaces an existing header if
    //  multiple header instances are not permitted.
    CMimeHeader* AddHeader(IN EMimeHeaderType eHeaderType);

    // Summary:
    //  Gets the MIME headers for this object.
    const CMimeHeaderList& GetHeaderList() const;

    //<COMBINE CMimeObject::GetHeaderList@const>
    CMimeHeaderList& GetHeaderList();

    // Summary:
    //  Sets the MIME content of a non multipart MIME object.
    mxt_result SetMimeContent(IN TOA CBlob* pBlobContent);

    // Summary:
    //  Gets the MIME content of a non multipart MIME object.
    const CBlob* GetMimeContent() const;

    // Summary:
    //  Appends an inner part to a multipart MIME object.
    mxt_result AddInnerPart(IN TOA CMimeObject* pInnerPart);

    // Summary:
    //  Gets the number of parts held by a multipart MIME object.
    unsigned int GetNumberOfParts() const;

    // Summary:
    //  Gets an inner part of a multipart MIME object.
    const CMimeObject* GetInnerPart(IN unsigned int uIndex) const;

    // Summary:
    //  Parses the MIME headers and the MIME content.
    virtual mxt_result Parse(INOUT const char*& rpcPos, IN unsigned int uLength);

    // Summary:
    //  Parses a single MIME header.
    virtual mxt_result ParseHeader(INOUT const char*& rpcPos);

    // Summary:
    //  Parses the MIME content.
    virtual mxt_result ParseContent(INOUT const char*& rpcPos, IN unsigned int uLength);

    // Summary:
    //  Serializes the headers and the content into a blob.
    virtual void Serialize(INOUT CBlob& rBlob);

    // Summary:
    // Serializes the MIME headers into a blob.
    virtual void SerializeHeaders(INOUT CBlob& rBlob);

    // Summary:
    // Serializes the MIME content into a blob.
    virtual void SerializeContent(INOUT CBlob& rBlob);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    // Summary:
    //  Sets the boundary to use for this level.
    void SetBoundary(IN const CTxtToken& rTokBoundary);

    // Summary:
    //  Sets the boundary to use for this level. Also sets boundaries for inner
    //  parts.
    void SetBoundary(INOUT unsigned int& ruItem);

    // Summary:
    //  Gets the boundary used for this level.
    const CTxtToken& GetBoundary();

    // Summary:
    //  Finds the first occurrence of the boundary.
    mxt_result FindBoundaryDelimiterHelper(IN CMimeObject* pCurrentMimeObject,
                                           IN const char* pszPayload,
                                           IN unsigned int uLength,
                                           INOUT unsigned int& ruIndex,
                                           OUT unsigned int& ruSizeToRemove,
                                           OUT bool& rbFoundTerminalHyphens,
                                           OUT bool& rbBoundaryIsRightAfter);

    // Summary:
    //  Internal serialization method. Serializes the headers and the content
    //  into a blob.
    void InternalSerialize(INOUT CBlob& rBlob, IN bool bIsRoot = false);

    // Summary:
    // Serializes the MIME headers into a blob.
    virtual void InternalSerializeHeaders(INOUT CBlob& rBlob, IN bool bIsRoot = false);

    // Summary:
    // Serializes the MIME content into a blob.
    virtual void InternalSerializeContent(INOUT CBlob& rBlob, IN bool bIsRoot = false);

private:
    // Comparison function.
    unsigned int FindStringInBlob(IN const char* pszStringToFind,
                                  IN const CBlob& rBlob);

//-- Hidden Data Members
protected:

    // The suballocator. It is protected so child classes can access it.
    CSubAllocator* m_pAllocator;

    // The headerlist used by this object.
    CMimeHeaderList* m_pMimeHeaderList;

    // The list of inner parts. If size is non-zero, it means that this MIME
    // object has one or multiple inner parts.
    CVector<CMimeObject*> m_vecpInnerParts;

    // The content of this message, i.e the raw data. If NULL, it means that
    // this message have inner parts. In that case, the size of m_vecpInnerParts
    // is greater than zero. Otherwise, it has no inner parts and the
    // application is responsible to parse this content.
    CBlob* m_pBlobContent;

    // The boundary of this message-body.
    CTxtToken m_tokBoundary;
private:

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetHeader
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      Then type of header to get.
//
//  Returns:
//      A pointer to the selected MIME header. Can be NULL if the specified
//      header cannot be found in the header list.
//
//  Description:
//      Gets a MIME header from the header list.
//
//      Use AddHeader to add a header to the header list, or directly access the
//      CMimeHeaderList using GetHeaderList.
//
//  See Also:
//      AddHeader, GetHeaderList
//
//==============================================================================
inline CMimeHeader* CMimeObject::GetHeader(IN EMimeHeaderType eHeaderType)
{
    return m_pMimeHeaderList->Get(eHeaderType);
}


//--<COMBINE CMimeObject::GetHeader@IN EMimeHeaderType>
inline const CMimeHeader* CMimeObject::GetHeader(IN EMimeHeaderType eHeaderType) const
{
    return m_pMimeHeaderList->Get(eHeaderType);
}


//==============================================================================
//==
//==  AddHeader
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      The MIME header type to add.
//
//  Returns:
//      A pointer to the newly created header. Can be NULL if the header could
//      not be created.
//
//  Description:
//      Creates a new CMimeHeader according to the eHeaderType parameter. The
//      header is automatically stored in the header list held by this mime
//      object.
//
//  See Also:
//      GetHeader
//
//==============================================================================
inline CMimeHeader* CMimeObject::AddHeader(IN EMimeHeaderType eHeaderType)
{
    return m_pMimeHeaderList->CreateNewMimeHeader(eHeaderType, CMimeHeaderList::eAT_END);
}


//==============================================================================
//==
//==  GetHeaderList
//==
//==============================================================================
//
//  Returns:
//      A reference to the header list held by this mime object.
//
//  Description:
//      Provides access to the CMimeHeaderList held by this mime object.
//
//==============================================================================
inline const CMimeHeaderList& CMimeObject::GetHeaderList() const
{
    return *m_pMimeHeaderList;
}


//--<COMBINE CMimeObject::GetHeaderList@const>
inline CMimeHeaderList& CMimeObject::GetHeaderList()
{
    return *m_pMimeHeaderList;
}

//==============================================================================
//==
//==  GetMimeContent
//==
//==============================================================================
//
//  Returns:
//      A pointer to the configured mime content, as a blob. Can be NULL if
//      there are no mime content available for this part. Note that internal
//      parts may provide access to mime content.
//
//  Description:
//      Returns the parsed or configured mime content as a blob.
//
//      This method will usually return a NULL pointer for multipart
//      content-types. Sub-parts of a multipart object can be accessed using
//      GetInnerPart.
//
//==============================================================================
inline const CBlob* CMimeObject::GetMimeContent() const
{
    return m_pBlobContent;
}

//==============================================================================
//==
//==  GetNumberOfParts
//==
//==============================================================================
//
//  Returns:
//      The number of inner parts held by this multipart object.
//
//  Description:
//      Returns the number of parts held by this multipart object.
//
//  See Also:
//      AddInnerPart, GetInnerPart
//
//==============================================================================
inline unsigned int CMimeObject::GetNumberOfParts() const
{
    return m_vecpInnerParts.GetSize();
}

//==============================================================================
//==
//==  ParseHeader
//==
//==============================================================================
//
//  Parameters:
//    rpcPos:
//      Reference to a pointer to the start of the header to parse. It is
//      updated to point to the byte following the last byte parsed when this
//      method returns.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      This method verifies that rpcPos points to the start of a MIME header
//      and then parses it. rpcPos is not updated if it does not point to the
//      start of a recognized MIME header.
//
//  See Also:
//      Parse, ParseContent
//
//==============================================================================
inline mxt_result CMimeObject::ParseHeader(INOUT const char*& rpcPos)
{
    return m_pMimeHeaderList->ParseSingleHeaderLine(rpcPos);
}

//==============================================================================
//==
//==  SerializeHeaders
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      Reference to a blob into which the headers are serialized.
//
//  Description:
//      Serializes the mime headers held by this mime object into rBlob.
//
//  #Notes:#
//      This function adds a temporary Content-Length header with its value set
//      to 0 followed by 12 spaces. Calls to SerializeContent will update the
//      value with the real one.
//
//  See Also:
//      Serialize, SerializeContent
//
//==============================================================================
inline void CMimeObject::SerializeHeaders(INOUT CBlob& rBlob)
{
    InternalSerializeHeaders(INOUT rBlob, true);
}

//==============================================================================
//==
//==  SerializeContent
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      Reference to a blob into which the content is serialized.
//
//  Description:
//      Serializes the the content or the inner parts held by this mime object
//      into rBlob.
//
//  See Also:
//      Serialize, SerializeHeaders
//
//==============================================================================
inline void CMimeObject::SerializeContent(INOUT CBlob& rBlob)
{
    InternalSerializeContent(INOUT rBlob, true);
}

//M5T_INTERNAL_USE_BEGIN
//==============================================================================
//==
//==  GetBoundary
//==
//==============================================================================
//
//  Returns:
//      The boundary, can be empty if not previously set.
//
//  Description:
//      Gets the boundary used for this level. This method has no use for the
//      application, it should not be called.
//
//==============================================================================
inline const CTxtToken& CMimeObject::GetBoundary()
{
    return m_tokBoundary;
}
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CMIMEOBJECT_H

