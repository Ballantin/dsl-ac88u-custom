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
#ifndef MXG_CMIMEHEADER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMIMEHEADER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CTXTHEADER_H
#include "TextMessage/CTxtHeader.h"
#endif

#ifndef MXG_MIMEHEADERGRAMMAR_H
#include "MimeMessage/MimeHeaderGrammar.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CMimeHeader
//========================================
//<GROUP MIME_MESSAGE_CLASSES>
//<TOPICORDER 0>
//
// Summary:
//  This class represents a MIME header.
//
// Description:
//  This class represents a MIME header. It supports a set of MIME headers, with
//  each header type mapped to a value defined in EMimeHeaderType. The following
//  MIME headers are supported:
//      - Content-Description, mapped to eHDR_CONTENT_DESCRIPTION;
//      - Content-Disposition, mapped to eHDR_CONTENT_DISPOSITION;
//      - Content-Encoding, mapped to eHDR_CONTENT_ENCODING;
//      - Content-Id, mapped to eHDR_CONTENT_ID;
//      - Content-Language, mapped to eHDR_CONTENT_LANGUAGE;
//      - Content-Length, mapped to eHDR_CONTENT_LENGTH;
//      - Content-Transfer-Encoding, mapped to eHDR_CONTENT_TRANSFER_ENCODING;
//      - Content-Type, mapped to eHDR_CONTENT_TYPE;
//
// Location:
//   MimeMessage/CMimeHeader.h
//
//==============================================================================
class CMimeHeader : public CTxtHeader
{
//-- Friend Declarations


//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CMimeHeader(IN EMimeHeaderType eHeaderType,
                IN IAllocator* pAllocator = NULL);

//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Constructor reserved for internal use only.
    CMimeHeader(IN const STxtHeaderDescriptor* pProtocolHeaders,
                IN IAllocator* pAllocator = NULL);
//M5T_INTERNAL_USE_END

    // Summary:
    //  Copy Constructor.
    CMimeHeader(IN const CMimeHeader& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CMimeHeader();

    // Summary:
    //  Assignment Operator.
    CMimeHeader& operator=(IN const CMimeHeader& rSrc);

    // Summary:
    //  Gets the header type.
    EMimeHeaderType GetHeaderType() const;

    // Summary:
    //  Provides access to the Content-Description header's text construct.
    const CTxtToken& GetContentDescription() const;

    //<COMBINE CMimeHeader::GetContentDescription@const>
    CTxtToken& GetContentDescription();

    // Summary:
    //  Provides access to the Content-Disposition header's disp-type
    //  construct.
    const CTxtToken& GetContentDisposition() const;

    //<COMBINE CMimeHeader::GetContentDisposition@const>
    CTxtToken& GetContentDisposition();

    // Summary:
    //  Provides access to the Content-Encoding header's content-coding
    //  construct.
    const CTxtToken& GetContentEncoding() const;

    //<COMBINE CMimeHeader::GetContentEncoding@const>
    CTxtToken& GetContentEncoding();

    // Summary:
    //  Provides access to the Content-Id.
    const CTxtNameAddr& GetContentId() const;

    //<COMBINE CMimeHeader::GetContentId@const>
    CTxtNameAddr& GetContentId();

    // Summary:
    //  Provides access to the Content-Language header's language-tag
    //  construct.
    const CTxtToken& GetContentLanguage() const;

    //<COMBINE CMimeHeader::GetContentLanguage@const>
    CTxtToken& GetContentLanguage();

    // Summary:
    //  Provides access to the Content-Length header's 1*DIGIT construct.
    const CTxtToken& GetContentLength() const;

    //<COMBINE CMimeHeader::GetContentLength@const>
    CTxtToken& GetContentLength();

    // Summary:
    //  Provides access to the Transfer-Encoding header's mechanism construct.
    const CTxtToken& GetContentTransferEncoding() const;

    //<COMBINE CMimeHeader::GetContentTransferEncoding@const>
    CTxtToken& GetContentTransferEncoding();

    // Summary:
    //  Provides access to the Content-Type header's m-type construct.
    const CTxtToken& GetContentTypeMType() const;

    //<COMBINE CMimeHeader::GetContentTypeMType@const>
    CTxtToken& GetContentTypeMType();

    // Summary:
    //  Provides access to the Content-Type header's m-subtype construct.
    const CTxtToken& GetContentTypeMSubType() const;

    //<COMBINE CMimeHeader::GetContentTypeMSubType@const>
    CTxtToken& GetContentTypeMSubType();

    // Summary:
    //  Returns a pointer to the chained header at the given 0-based index.
    CMimeHeader* GetNextMimeHeader(IN unsigned int uIndex = 0);

    //<COMBINE CMimeHeader::GetNextMimeHeader@IN unsigned int>
    const CMimeHeader* GetNextMimeHeader(IN unsigned int uIndex = 0) const;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:

    virtual GO CTxtHeader* CreateNewHeader();

//-- Hidden Data Members
protected:
private:
    // The type of header.
    EMimeHeaderType m_eHeaderType;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetHeaderType
//==
//==============================================================================
//
//  Returns:
//      The header's type.
//
//  Description:
//      Gets the header type.
//
//==============================================================================
inline EMimeHeaderType CMimeHeader::GetHeaderType() const
{
    return m_eHeaderType;
}

//==============================================================================
//==
//==  GetContentDescription
//==
//==============================================================================
//
//  Returns:
//      The content description.
//
//  Description:
//      Provides access to the Content-Description header's text construct.
//
//==============================================================================
inline const CTxtToken& CMimeHeader::GetContentDescription() const
{
    return GetOneTokenForm().m_tokFirst;
}

//--<COMBINE CMimeHeader::GetContentDescription@const>
inline CTxtToken& CMimeHeader::GetContentDescription()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetContentDisposition
//==
//==============================================================================
//
//  Returns:
//      The content disposition.
//
//  Description:
//      Provides access to the Content-Disposition header's disp-type
//      construct.
//
//==============================================================================
inline const CTxtToken& CMimeHeader::GetContentDisposition() const
{
    return GetOneTokenForm().m_tokFirst;
}

//--<COMBINE CMimeHeader::GetContentDisposition@const>
inline CTxtToken& CMimeHeader::GetContentDisposition()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetHeaderType
//==
//==============================================================================
//
//  Returns:
//      The content encoding.
//
//  Description:
//      Provides access to the Content-Encoding header's content-coding
//      construct.
//
//==============================================================================
inline const CTxtToken& CMimeHeader::GetContentEncoding() const
{
    return GetOneTokenForm().m_tokFirst;
}

//--<COMBINE CMimeHeader::GetContentEncoding@const>
inline CTxtToken& CMimeHeader::GetContentEncoding()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetContentId
//==
//==============================================================================
//
//  Returns:
//      The cootent id.
//
//  Description:
//      Provides access to the Content-Id.
//
//==============================================================================
inline const CTxtNameAddr& CMimeHeader::GetContentId() const
{
    return GetNameAddrForm().m_nameAddr;
}

//--<COMBINE CMimeHeader::GetContentId@const>
inline CTxtNameAddr& CMimeHeader::GetContentId()
{
    return GetNameAddrForm().m_nameAddr;
}

//==============================================================================
//==
//==  GetContentLanguage
//==
//==============================================================================
//
//  Returns:
//      The content language.
//
//  Description:
//      Provides access to the Content-Language header's language-tag
//      construct.
//
//==============================================================================
inline const CTxtToken& CMimeHeader::GetContentLanguage() const
{
    return GetOneTokenForm().m_tokFirst;
}

//--<COMBINE CMimeHeader::GetContentLanguage@const>
inline CTxtToken& CMimeHeader::GetContentLanguage()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetContentLength
//==
//==============================================================================
//
//  Returns:
//      The content length.
//
//  Description:
//      Provides access to the Content-Length header's 1*DIGIT construct.
//
//==============================================================================
inline const CTxtToken& CMimeHeader::GetContentLength() const
{
    return GetOneTokenForm().m_tokFirst;
}

//--<COMBINE CMimeHeader::GetContentLength@const>
inline CTxtToken& CMimeHeader::GetContentLength()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetContentTransferEncoding
//==
//==============================================================================
//
//  Returns:
//      The content transfer encoding.
//
//  Description:
//      Provides access to the Content-Transfer-Encoding header's mechanism
//      construct.
//
//==============================================================================
inline const CTxtToken& CMimeHeader::GetContentTransferEncoding() const
{
    return GetOneTokenForm().m_tokFirst;
}

//--<COMBINE CMimeHeader::GetContentTransferEncoding@const>
inline CTxtToken& CMimeHeader::GetContentTransferEncoding()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetContentTypeMType
//==
//==============================================================================
//
//  Returns:
//      The content type MType.
//
//  Description:
//      Provides access to the Content-Type header's m-type construct.
//
//==============================================================================
inline const CTxtToken& CMimeHeader::GetContentTypeMType() const
{
    return GetTwoTokensForm().m_tokFirst;
}

//--<COMBINE CMimeHeader::GetContentTypeMType@const>
inline CTxtToken& CMimeHeader::GetContentTypeMType()
{
    return GetTwoTokensForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetContentTypeMSubType
//==
//==============================================================================
//
//  Returns:
//      The content type MSubType.
//
//  Description:
//      Gets the content type MSubType.
//
//==============================================================================
inline const CTxtToken& CMimeHeader::GetContentTypeMSubType() const
{
    return GetTwoTokensForm().m_tokSecond;
}

//--<COMBINE CMimeHeader::GetContentTypeMSubType@const>
inline CTxtToken& CMimeHeader::GetContentTypeMSubType()
{
    return GetTwoTokensForm().m_tokSecond;
}

//==============================================================================
//==
//==  GetNextMimeHeader
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          0-based index of the header to find in the chain.
//
//  Returns:
//      Pointer to the header at the specified index, or NULL if out of range.
//
//  Description:
//      This method finds the header at the specified index in the chain and
//      returns it. This method accesses the list of next headers, which does
//      not include the current instance of CMimeHeader.
//
//==============================================================================
inline CMimeHeader* CMimeHeader::GetNextMimeHeader(IN unsigned int uIndex)
{
    return static_cast<CMimeHeader*>(GetNextHeader(uIndex));
}

//--<COMBINE CTxtHeader::GetNextMimeHeader@IN unsigned int>
inline const CMimeHeader* CMimeHeader::GetNextMimeHeader(IN unsigned int uIndex) const
{
    return static_cast<const CMimeHeader*>(GetNextHeader(uIndex));
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CMIMEHEADER_H
