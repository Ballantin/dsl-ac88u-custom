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
#ifndef MXG_CMIMEHEADERLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMIMEHEADERLIST_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CMIMEHEADER_H
#include "MimeMessage/CMimeHeader.h"
#endif

#ifndef MXG_CTXTHEADERLIST_H
#include "TextMessage/CTxtHeaderList.h"
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
//== Class: CMimeHeaderList
//========================================
//<GROUP MIME_MESSAGE_CLASSES>
//<TOPICORDER 1>
//
// Summary:
//  This class represents a MIME header list.
//
// Description:
//   This class manages a list of headers. It is merely a container. It is
//   responsible for parsing and serializing multiple headers.
//
// Location:
//   MimeMessage/CMimeHeaderList.h
//
//==============================================================================
class CMimeHeaderList : public CTxtHeaderList
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CMimeHeaderList(IN IAllocator* pAllocator = NULL, IN bool bParseExtensionHeaders = true);

    // Summary:
    //  Copy Constructor.
    CMimeHeaderList(IN const CMimeHeaderList& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CMimeHeaderList();

    //-- << Operators >>

    // Summary:
    //  Assignment Operator.
    CMimeHeaderList& operator=(IN const CMimeHeaderList& rSrc);

    // << Creator >>

    // Summary:
    //  Creates a new header and inserts it in the list.
    CMimeHeader* CreateNewMimeHeader(IN const char* pszHeaderName,
                                     IN EInsertPosition eInsertPos);

    // Summary:
    //  Creates a new header and inserts it in the list.
    CMimeHeader* CreateNewMimeHeader(IN EMimeHeaderType eHeaderType,
                                     IN EInsertPosition eInsertPos);

    // Summary:
    //  Removes the n'th header of the specified type.
    mxt_result RemoveHeader(IN EMimeHeaderType eHeaderType, IN unsigned int uIndex);

    // Summary:
    //  Removes all headers of the specified type.
    mxt_result RemoveHeaderType(IN EMimeHeaderType eHeaderType);

    // Summary:
    //  Returns a CMimeHeader chain.
    CMimeHeader* Get(IN const char* pszName);

    //<COMBINE CMimeHeaderList::Get@IN const char*>
    const CMimeHeader* Get(IN const char* pszName) const;

    //<COMBINE CMimeHeaderList::Get@IN const char*>
    CMimeHeader* Get(IN EMimeHeaderType eHeaderType);

    //<COMBINE CMimeHeaderList::Get@IN const char*>
    const CMimeHeader* Get(IN EMimeHeaderType eHeaderType) const;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    // Summary:
    //  Creates a new header using the allocator or MX_NEW.
    virtual GO CTxtHeader* InternalCreateNewHeader(IN const char* pszHeaderName) const;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  CreateNewMimeHeader
//==
//==============================================================================
//
//  Parameters:
//      pszHeaderName:
//          The header name of the header to create. This shall help the
//          creation process in allowing to find the correct header descriptor
//          for this header name. MUST NOT be NULL.
//      eInsertPos:
//          The insertion position in the list. Either eAT_BEGINNING or eAT_END.
//
//  Returns:
//      The created header. May return NULL, if the header failed to be added
//      to the list.
//
//  Description:
//      This method is a helper that allows a user of the header list to easily
//      create a new header, add it in the header list and get a pointer to make
//      modifications to the created header.
//
//  #Note:#
//      - The header ownership is kept by the header list.
//
//==============================================================================
inline CMimeHeader* CMimeHeaderList::CreateNewMimeHeader(IN const char* pszHeaderName,
                                                         IN EInsertPosition eInsertPos)
{
    return static_cast<CMimeHeader*>(CreateNewTxtHeader(pszHeaderName, eInsertPos));
}

//==============================================================================
//==
//==  CreateNewMimeHeader
//==
//==============================================================================
//
//  Parameters:
//      eHeaderType:
//          The header type of the header to create. This shall help the
//          creation process in allowing to find the correct header descriptor
//          for this header name. MUST NOT be NULL.
//
//      eInsertPos:
//          The insertion position in the list. Either eAT_BEGINNING or eAT_END.
//
//  Returns:
//      The created header. May return NULL, if the header failed to be
//      added to the list.
//
//  Description:
//      This method is a helper that allows a user of the header list to easily
//      create a new header, add it in the header list and get a pointer to make
//      modifications to the created header.
//
//  #Note:#
//      - The header ownership is kept by the header list.
//
//==============================================================================
inline CMimeHeader* CMimeHeaderList::CreateNewMimeHeader(IN EMimeHeaderType eHeaderType, IN EInsertPosition eInsertPos)
{
    return static_cast<CMimeHeader*>(CreateNewTxtHeader(g_astMimeHeaderDescriptor[eHeaderType].m_pszLongName,
                                                        eInsertPos));
}

//==============================================================================
//==
//==  RemoveHeader
//==
//==============================================================================
//
//  Parameters:
//      eHeaderType:
//          Type of the header to remove.
//
//      uIndex:
//          Index of the header to remove. The index is zero based.
//
//  Returns:
//      - resS_OK: Headers have been removed.
//      - failure: Could not find headers with the specified name or there is no
//                 header at the given index.
//
//  Description:
//      Removes the n'th header of the specified type.
//
//==============================================================================
inline mxt_result CMimeHeaderList::RemoveHeader(IN EMimeHeaderType eHeaderType, IN unsigned int uIndex)
{
    return CTxtHeaderList::RemoveHeader(g_astMimeHeaderDescriptor[eHeaderType].m_pszLongName, uIndex);
}

//==============================================================================
//==
//==  RemoveHeaderType
//==
//==============================================================================
//
//  Parameters:
//      eHeaderType:
//          Type of the headers to remove.
//
//  Returns:
//      - resS_OK: Headers has been removed.
//      - failure: Could not find headers of the specified type.
//
//  Description:
//      Removes all headers of the specified type.
//
//==============================================================================
inline mxt_result CMimeHeaderList::RemoveHeaderType(IN EMimeHeaderType eHeaderType)
{
    return CTxtHeaderList::RemoveHeaderType(g_astMimeHeaderDescriptor[eHeaderType].m_pszLongName);
}

//==============================================================================
//==
//==  Get
//==
//==============================================================================
//
//  Parameters:
//      pszName:
//          Name of the headers to parse and return.
//
//      eHeaderType:
//          Type of the headers to parse and return.
//
//  Returns:
//      A pointer to the header chain.
//
//  Description:
//      This method is used to retrieve a MIME header chain.
//
//==============================================================================
inline CMimeHeader* CMimeHeaderList::Get(IN const char* pszName)
{
    return static_cast<CMimeHeader*>(CTxtHeaderList::Get(pszName));
}

//--<COMBINE CMimeHeaderList::Get@IN const char*>
inline const CMimeHeader* CMimeHeaderList::Get(IN const char* pszName) const
{
    return static_cast<const CMimeHeader*>(CTxtHeaderList::Get(pszName));
}

//--<COMBINE CMimeHeaderList::Get@IN const char*>
inline const CMimeHeader* CMimeHeaderList::Get(IN EMimeHeaderType eHeaderType) const
{
    return static_cast<const CMimeHeader*>(CTxtHeaderList::Get(g_astMimeHeaderDescriptor[eHeaderType].m_pszLongName));
}

//--<COMBINE CMimeHeaderList::Get@IN const char*>
inline CMimeHeader* CMimeHeaderList::Get(IN EMimeHeaderType eHeaderType)
{
    return static_cast<CMimeHeader*>(CTxtHeaderList::Get(g_astMimeHeaderDescriptor[eHeaderType].m_pszLongName));
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CMIMEHEADER_H
