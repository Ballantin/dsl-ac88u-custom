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
#ifndef MXG_CCPIMMESSAGE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCPIMMESSAGE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CTXTBASEPACKET_H
#include "TextMessage/CTxtBasePacket.h"
#endif

#ifndef MXG_CCPIMHEADER_H
#include "SceUaImppCpim/CCpimHeader.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CCpimHeaderList;
class CMimeObject;
class CSipMessageBody;
class CTxtNameAddr;
class IUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
extern const char* const g_pszHDR_CPIM_FROM;
extern const char* const g_pszHDR_CPIM_TO;
extern const char* const g_pszHDR_CPIM_CC;
extern const char* const g_pszHDR_CPIM_DATE_TIME;
extern const char* const g_pszHDR_CPIM_SUBJECT;
extern const char* const g_pszHDR_CPIM_NS;
extern const char* const g_pszHDR_CPIM_REQUIRE;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
typedef CTxtNameAddr& ((CCpimHeader::*PFNGetNameAddrMethod) ());
//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: CCpimMessage
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//  This class represents a CPIM message.
//
// Description:
//  This class defines a CPIM message, as defined by RFC 3862.
//
//  A CPIM message encapsulates an IM message and provides basic information
//  about its sender and its recipients. The format is intended to facilitate
//  interoperability between different IM protocols.
//
//  The encapsulated IM message can be in any type of MIME format.
//
// Location:
//   SceUaImppCpim/CCpimMessage.h
//
// See Also:
//   CCpimHeader, CCpimHeaderList
//
//==============================================================================
class CCpimMessage : public CTxtBasePacket
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CCpimMessage();

    // Summary:
    //  Destructor.
    virtual ~CCpimMessage();

    // Summary:
    //  Copy Constructor.
    CCpimMessage(IN const CCpimMessage& rSrc);

    // Summary:
    //  Assignment Operator.
    CCpimMessage& operator=(IN const CCpimMessage& rSrc);

    // Summary:
    //  Creates a CPIM message with From and To headers along with a plain text
    //  message.
    void CreatePlainTextMessage(IN const CTxtNameAddr& rFrom,
                                IN const CTxtNameAddr& rTo,
                                IN const char* pszUtf8Message);

    // Summary:
    //  Sets the From header that identifies the sender of the message.
    void SetFrom(IN const CTxtNameAddr& rFrom);

    // Summary:
    //  Sets the From header that identifies the sender of the message.
    void SetFrom(IN const IUri* pUri,
                 IN const char* pszUtf8DisplayName);

    // Summary:
    //  Gets the From header that identifies the sender of the message.
    const CTxtNameAddr* GetFrom() const;

    // Summary:
    //  Adds a new To header that identifies an intended recipient of the
    //  message.
    void AddTo(IN const CTxtNameAddr& rFrom);

    // Summary:
    //  Adds a new To header that identifies an intended recipient of the
    //  message.
    void AddTo(IN const IUri* pUri,
               IN const char* pszUtf8DisplayName);


    // Summary:
    //  Gets the number of To headers found in the message.
    unsigned int GetNumTo() const;

    // Summary:
    //  Gets a To header by its index, which identifies one of the intended
    //  recipient.
    const CTxtNameAddr* GetTo(IN unsigned int uIndex) const;

    // Summary:
    //  Adds a new cc header that identifies a non-primary recipient to which a
    //  courtesy copy is sent (CC).
    void AddCC(IN const CTxtNameAddr& rCc);

    // Summary:
    //  Adds a new cc header that identifies a non-primary recipient to which a
    //  courtesy copy is sent (CC).
    void AddCC(IN const IUri* pUri,
               IN const char* pszUtf8DisplayName);

    // Summary:
    //  Gets the number of cc headers found in the message.
    unsigned int GetNumCC() const;

    // Summary:
    //  Gets a cc header by its index, which identifies a non-primary recipient
    //  to which a courtesy copy is sent (CC).
    const CTxtNameAddr* GetCC(IN unsigned int uIndex) const;

    // Summary:
    //  Sets a timestamp on the CPIM message in a DateTime header.
    void SetDateTime(IN uint16_t uYear,
                     IN uint16_t uMonth,
                     IN uint16_t uDay,
                     IN uint16_t uHour,
                     IN uint16_t uMinute,
                     IN uint16_t uSecond,
                     IN uint16_t* puMs);

    // Summary:
    //  Gets the timestamp found in the CPIM message in the form of a DateTime
    //  header.
    mxt_result GetDateTime(OUT uint16_t& ruYear,
                           OUT uint16_t& ruMonth,
                           OUT uint16_t& ruDay,
                           OUT uint16_t& ruHour,
                           OUT uint16_t& ruMinute,
                           OUT uint16_t& ruSecond,
                           OUT uint16_t& ruMs) const;


    // Summary:
    //  Gets the DateTime header from the CPIM message.
    const CCpimHeader* GetDateTime() const;


    // Summary:
    //  Adds a Subject header that describes the topic of the message.
    void AddSubject(IN const char* pszUtf8Subject,
                    IN const char* pszLanguage);

    // Summary:
    //  Gets the number of Subject headers found in the message.
    unsigned int GetNumSubject() const;


    // Summary:
    //  Gets a Subject header by its index, which describes the topic of the
    //  message.
    const char* GetSubject(IN unsigned int uIndex,
                           OUT const char*& rpszLanguage) const;


    // Summary:
    //  Adds a Namespace (NS) header used to declare a local namespace prefix.
    void AddNS(IN const CTxtNameAddr& rNamespace);

    // Summary:
    //  Adds a Namespace (NS) header used to declare a local namespace prefix.
    void AddNS(IN const IUri* pUri,
               IN const char* pszNamePrefix);

    // Summary:
    //  Gets the number of NS headers found in the message.
    unsigned int GetNumNS() const;

    // Summary:
    //  Gets a NS header by its index, which is used to declare a local
    //  namespace prefix.
    const CTxtNameAddr* GetNS(IN unsigned int uIndex) const;

    // Summary:
    //  Adds a Require header which is used to identify features that must be
    //  implemented by the receiver.
    void AddRequire(IN const char* pszHeaderName);

    // Summary:
    //  Gets the number of Require headers found in the message.
    unsigned int GetNumRequire() const;

    // Summary:
    //  Gets a Require header by its index.
    const char* GetRequire(IN unsigned int uIndex) const;

    // Summary:
    //  Adds an extension header which are application defined.
    void AddExtension(IN const char* pszHeaderName,
                      IN const char* pszHeaderValue);

    // Summary:
    //  Gets the number of name-matching Extension headers found
    //  in the message.
    unsigned int GetNumExtension(IN const char* pszHeaderName) const;

    // Summary:
    //  Gets a name-matching extension header by its index.
    const char* GetExtension(IN const char* pszHeaderName,
                             IN unsigned int uIndex) const;

    // Summary:
    //  Sets a "text/plain" message as a payload of the CPIM message.
    void SetPlainTextMessage(IN const CString& rstrUtf8Message);

    // Summary:
    //  Gets a "text/plain" payload from the CPIM message.
    mxt_result GetPlainTextMessage(OUT CString& rstrUtf8Message) const;

    // Summary:
    //  Sets a MIME payload to the CPIM message.
    void SetMimePayload(IN TOA CMimeObject* pMimeObject);

    // Summary:
    //  Gets the MIME payload associated with the CPIM message.
    const CMimeObject* GetMimePayload() const;

    // Summary:
    //  Resets the message's content.
    void Reset();

    // Summary:
    //  Finds and parses the CPIM found in the MIME object.
    mxt_result ParseCpim(IN const CMimeObject& rMimeObject);

    // Summary:
    //  Serializes the CPIM into a MIME object.
    void Serialize(OUT GO CMimeObject*& rpMimeObject);

    // Summary:
    //  Finds and parses the CPIM found in the MIME object.
    mxt_result ParseCpim(IN const CSipMessageBody& rMessageBody);

    // Summary:
    //  Serializes the CPIM into a CSipMessageBody object.
    void Serialize(OUT GO CSipMessageBody*& rpMessageBody);

    // Summary:
    //  Parses the CPIM from a pointer to the actual CPIM data.
    mxt_result ParseCpim(INOUT const char*& rpcPos, IN unsigned int uLength);

    // Summary:
    //  Serializes the CPIM in a blob.
    void Serialize(INOUT CBlob& rBlob);


    // Summary:
    //  Provides access to the list of CPIM headers for advanced use.
    CCpimHeaderList& GetHeaderList();

    // Summary:
    //  Provides access to the list of CPIM headers for advanced use.
    const CCpimHeaderList& GetHeaderList() const;

    //-- << CTxtBasePacket methods >>

    // Summary:
    //  Gets the type of packet as a string.
    virtual const char* GetPacketType() const;

//-- Hidden Methods.
protected:
    //-- << CTxtBasePacket methods >>

    // Summary:
    //  Creates a new CTxtHeaderList object.
    virtual void CreateHeaderList(OUT GO CTxtHeaderList*& rpHeaderList) const;

private:
    //-- << Helpers >>

    // Summary:
    //  Parses a date/time number from the string.
    mxt_result ParseDateTime(IN unsigned int uSize,
                             IN const char cSeparator,
                             INOUT const char*& rpcBuffer,
                             OUT uint16_t& ruResult) const;

    // Summary:
    //  Finds a CPIM message in the MIME object.
    const CMimeObject* FindCpimMessage(IN const CMimeObject* pObject);

    // Summary:
    //  Find a CPIM message in the CSipMessageBody object.
    const CSipMessageBody* FindCpimMessage(IN const CSipMessageBody* pObject);

    // Summary:
    //  Adds a new header and return its value container.
    CTxtNameAddr& AddHeaderHelper(IN const char* pszName,
                                  IN PFNGetNameAddrMethod pfnGet);

    // Summary:
    //  Adds a new header.
    CCpimHeader* AddHeaderHelper(IN const char* pszName);

    // Summary:
    //  Gets a header's value container.
    CTxtNameAddr* GetHelper(IN const char* pszName,
                            IN unsigned int uIndex,
                            IN PFNGetNameAddrMethod pfnGet) const;

    // Summary:
    //  Gets a header.
    CCpimHeader* GetHeaderHelper(IN const char* pszName,
                                 IN unsigned int uIndex) const;

    // Summary:
    //  Get the number of headers.
    unsigned int GetNumHeaderHelper(IN const char* pszName) const;


//-- Hidden Data Members.
protected:
private:

    // The packet type descriptor for the CPIM packet.
    static const char* const ms_pszPACKET_TYPE;

    // The payload in the form of a MIME object.
    CMimeObject* m_pPayload;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetFrom
//==
//==============================================================================
//
//  Returns:
//      Pointer to a CTxtNameAddr containing the information about the sender of
//      the message. Can be NULL if the message has no From header.
//
//  Description:
//      Gets the address and display name of the sender. This information is
//      taken from the "From" CPIM header.
//
//  See Also:
//      SetFrom, GetTo, GetCC
//
//==============================================================================
inline const CTxtNameAddr* CCpimMessage::GetFrom() const
{
    return GetHelper(g_pszHDR_CPIM_FROM, 0, &CCpimHeader::GetFrom);
}


//==============================================================================
//==
//==  AddTo
//==
//==============================================================================
//
//  Parameters:
//    rTo:
//      Reference to a CTxtNameAddr that contains the optional display name and
//      URI of the recipient to add.
//
//  Description:
//      Adds an intended recipient to the recipient list. This is encoded into a
//      "To" CPIM header.
//
//  See Also:
//      GetTo, SetFrom, AddCC
//
//==============================================================================
inline void CCpimMessage::AddTo(IN const CTxtNameAddr& rTo)
{
    AddHeaderHelper(g_pszHDR_CPIM_TO, &CCpimHeader::GetTo) = rTo;
}


//==============================================================================
//==
//==  AddTo
//==
//==============================================================================
//
//  Parameters:
//    pUri:
//      Pointer to the URI of the intended recipient.
//
//    pszUtf8DisplayName:
//      Pointer to an UTF-8 encoded display name of the intended recipient. Can
//      be NULL, in which case no display-name is set.
//
//  Description:
//      Adds an intended recipient to the recipient list. This is encoded into a
//      "To" CPIM header.
//
//  See Also:
//      GetTo, SetFrom, AddCC, GetNumTo
//
//==============================================================================
inline void CCpimMessage::AddTo(IN const IUri* pUri,
                                IN const char* pszUtf8DisplayName)
{
    AddHeaderHelper(g_pszHDR_CPIM_TO, &CCpimHeader::GetTo).SetUri(pUri, pszUtf8DisplayName);
}


//==============================================================================
//==
//==  GetNumTo
//==
//==============================================================================
//
//  Returns:
//      The number of recipient found in the message.
//
//  Description:
//      Gets the number of intended recipients found in the message. At a lower
//      level, this corresponds to the number of "To" headers found in the
//      message.
//
//  See Also:
//      AddTo, GetTo, GetFrom, GetCC
//
//==============================================================================
inline unsigned int CCpimMessage::GetNumTo() const
{
    return GetNumHeaderHelper(g_pszHDR_CPIM_TO);
}


//==============================================================================
//==
//==  GetTo
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Index of the intended recipient to return.
//
//  Returns:
//      Pointer to a CTxtNameAddr that identifies the intended recipient found
//      at uIndex. Can be NULL if the index is out of range.
//
//  Description:
//      Gets the intended recipient found at the given index. A CPIM message can
//      encode multiple intended recipients.
//
//      The intended recipient corresponds to the "To" CPIM header.
//
//  See Also:
//      GetNumTo, AddTo, GetFrom, GetCC
//
//==============================================================================
inline const CTxtNameAddr* CCpimMessage::GetTo(IN unsigned int uIndex) const
{
    return GetHelper(g_pszHDR_CPIM_TO, uIndex, &CCpimHeader::GetTo);
}


//==============================================================================
//==
//==  AddCC
//==
//==============================================================================
//
//  Parameters:
//    rCc:
//      Reference to a CTxtNameAddr that contains the optional display name and
//      URI of the non-primary recipient.
//
//  Description:
//      Adds a recipient to the list of non-primary recipients to which a
//      courtesy copy is sent. This is encoded as a "cc" CPIM header.
//
//  See Also:
//      SetFrom, AddTo, GetCC, GetNumCC
//
//==============================================================================
inline void CCpimMessage::AddCC(IN const CTxtNameAddr& rCc)
{
    AddHeaderHelper(g_pszHDR_CPIM_CC, &CCpimHeader::GetCc) = rCc;
}


//==============================================================================
//==
//==  AddCC
//==
//==============================================================================
//
//  Parameters:
//    pUri:
//      Pointer to the URI of the recipient.
//
//    pszUtf8DisplayName:
//      Pointer to an UTF-8 encoded display name of the recipient. Can
//      be NULL, in which case no display-name is set.
//
//  Description:
//      Adds a recipient to the list of non-primary recipients to which a
//      courtesy copy is sent. This is encoded as a "cc" CPIM header.
//
//  See Also:
//      SetFrom, AddTo, GetCC, GetNumCC
//
//==============================================================================
inline void CCpimMessage::AddCC(IN const IUri* pUri,
                                IN const char* pszUtf8DisplayName)
{
    AddHeaderHelper(g_pszHDR_CPIM_CC, &CCpimHeader::GetCc).SetUri(pUri, pszUtf8DisplayName);
}


//==============================================================================
//==
//==  GetNumCC
//==
//==============================================================================
//
//  Returns:
//      The number of non-primary recipients found in the message.
//
//  Description:
//      Gets the number of non-primary recipients found in the message. At a
//      lower level, this corresponds to the number of "cc" headers found in the
//      message.
//
//  See Also:
//      AddCC, GetCC
//
//==============================================================================
inline unsigned int CCpimMessage::GetNumCC() const
{
    return GetNumHeaderHelper(g_pszHDR_CPIM_CC);
}


//==============================================================================
//==
//==  GetCC
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Index of the non-primary recipient to return.
//
//  Returns:
//      Pointer to a CTxtNameAddr that identifies the non-primary recipient
//      found at uIndex. Can be NULL if the index is out of range.
//
//  Description:
//      Gets the non-primary recipient found at the given index. A CPIM message
//      can encode multiple non-primary recipients.
//
//      The non-primary recipient corresponds to the "cc" CPIM header.
//
//  See Also:
//      GetNumCC, GetFrom, GetTo, AddCC
//
//==============================================================================
inline const CTxtNameAddr* CCpimMessage::GetCC(IN unsigned int uIndex) const
{
    return GetHelper(g_pszHDR_CPIM_CC, uIndex, &CCpimHeader::GetCc);
}


//==============================================================================
//==
//==  GetNumSubject
//==
//==============================================================================
//
//  Returns:
//      The number of subject headers encoded into the message.
//
//  Description:
//      Returns the number of "Subject" headers encoded into the message.
//
//      Each subject added to a single message should all have different
//      pszLanguage parameters. Thus, all subject configured in a message should
//      represent the same subject in different languages.
//
//  See Also:
//      AddSubject, GetSubject
//
//==============================================================================
inline unsigned int CCpimMessage::GetNumSubject() const
{
    return GetNumHeaderHelper(g_pszHDR_CPIM_SUBJECT);
}


//==============================================================================
//==
//==  AddNS
//==
//==============================================================================
//
//  Parameters:
//    rNamespace:
//      Reference to the namespace to declare, encoded into a CTxtNameAddr.
//
//  Description:
//      Declares a new namespace as part of the message. Namespaces are used to
//      uniquely identify new features developped over the CPIM message format.
//
//  See Also:
//      GetNumNS, GetNS
//
//==============================================================================
inline void CCpimMessage::AddNS(IN const CTxtNameAddr& rNamespace)
{
    AddHeaderHelper(g_pszHDR_CPIM_NS, &CCpimHeader::GetNs) = rNamespace;
}


//==============================================================================
//==
//==  AddNS
//==
//==============================================================================
//
//  Parameters:
//    pUri:
//      Pointer to the namespace URI.
//
//    pszNamePrefix:
//      Pointer to the namespace prefix associated with the URI. Can be NULL, in
//      which case this declares a default namespace.
//
//  Description:
//      Declares a new namespace as part of the message. Namespaces are used to
//      uniquely identify new features developped over the CPIM message format.
//
//  See Also:
//      GetNumNS, GetNS
//
//==============================================================================
inline void CCpimMessage::AddNS(IN const IUri* pUri,
                                IN const char* pszNamePrefix)
{
    AddHeaderHelper(g_pszHDR_CPIM_NS, &CCpimHeader::GetNs).SetUri(pUri, pszNamePrefix);
}


//==============================================================================
//==
//==  GetNumNS
//==
//==============================================================================
//
//  Returns:
//      The number of namespaces declared into the message.
//
//  Description:
//      Returns the number of namespaces declared into the message. At a lower
//      level, this corresponds to the number of "NS" headers found in the
//      message.
//
//  See Also:
//      AddNS, GetNS
//
//==============================================================================
inline unsigned int CCpimMessage::GetNumNS() const
{
    return GetNumHeaderHelper(g_pszHDR_CPIM_NS);
}


//==============================================================================
//==
//==  GetNS
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the namespace to retrieve.
//
//  Returns:
//      Pointer to a CTxtNameAddr that identifies the namespace found at uIndex.
//      Can be NULL if the index is out of range.
//
//  Description:
//      Gets the namespace found at the given index. A CPIM message can encode
//      multiple namespaces.
//
//      The namespace corresponds to the "NS" CPIM header.
//
//==============================================================================
inline const CTxtNameAddr* CCpimMessage::GetNS(IN unsigned int uIndex) const
{
    return GetHelper(g_pszHDR_CPIM_NS, uIndex, &CCpimHeader::GetNs);
}


//==============================================================================
//==
//==  AddRequire
//==
//==============================================================================
//
//  Parameters:
//    pszHeaderName:
//      Pointer to a c-style string that identifies the name of the header that
//      must be understood.
//
//  Description:
//      Adds a Require header to the CPIM message, which informs the recipient
//      that it MUST understand the extension header specified in pszHeaderName
//      in order to correctly process the message.
//
//  See Also:
//      GetNumRequire, GetRequire
//
//==============================================================================
inline void CCpimMessage::AddRequire(IN const char* pszHeaderName)
{
    AddHeaderHelper(g_pszHDR_CPIM_REQUIRE)->GetRequire() = pszHeaderName;
}


//==============================================================================
//==
//==  GetNumRequire
//==
//==============================================================================
//
//  Returns:
//      The number of require specified in the message.
//
//  Description:
//      Gets the number of extensions that must be understood by the message
//      recipients.
//
//  See Also:
//      AddRequire, GetRequire
//
//==============================================================================
inline unsigned int CCpimMessage::GetNumRequire() const
{
    return GetNumHeaderHelper(g_pszHDR_CPIM_REQUIRE);
}


//==============================================================================
//==
//==  AddExtension
//==
//==============================================================================
//
//  Parameters:
//    pszHeaderName:
//      Pointer to the header name. Must not be NULL.
//
//    pszHeaderValue:
//      Pointer to the header value. Can be NULL, in which case no value is
//      associated with the header.
//
//  Description:
//      Adds an extension header to the CPIM message.
//
//  See Also:
//      GetNumExtension, GetExtension
//
//==============================================================================
inline void CCpimMessage::AddExtension(IN const char* pszHeaderName,
                                       IN const char* pszHeaderValue)
{
    AddHeaderHelper(pszHeaderName)->GetOneTokenForm().m_tokFirst = pszHeaderValue;
}


//==============================================================================
//==
//==  GetNumExtension
//==
//==============================================================================
//
//  Parameters:
//    pszHeaderName:
//      Pointer to the header name. Must not be NULL.
//
//  Returns:
//      The number of extension headers in this message.
//
//  Description:
//      Gets the number of extension headers matching the given name in the
//      CPIM message.
//
//  See Also:
//      AddExtension, GetExtension
//
//==============================================================================
inline unsigned int CCpimMessage::GetNumExtension(IN const char* pszHeaderName) const
{
    return GetNumHeaderHelper(pszHeaderName);
}


//==============================================================================
//==
//==  GetMimePayload
//==
//==============================================================================
//
//  Returns:
//      Pointer to the MIME payload that contains the encapsulated IM message.
//      Can be NULL if no payload has been set.
//
//  Description:
//      Returns a pointer to the MIME payload.
//
//  See Also:
//      SetMimePayload, SetPlainTextMessage
//
//==============================================================================
inline const CMimeObject* CCpimMessage::GetMimePayload() const
{
    return m_pPayload;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CCPIMMESSAGE_H

