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
#ifndef MXG_CCPIMHEADERLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCPIMHEADERLIST_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CTXTHEADERLIST_H
#include "TextMessage/CTxtHeaderList.h"
#endif

#ifndef MXG_CCPIMHEADER_H
#include "SceUaImppCpim/CCpimHeader.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CCpimHeaderList
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//  This class represents a CPIM header list.
//
// Description:
//   This class manages a list of headers. It is merely a container. It is
//   responsible for parsing and serializing multiple headers.
//
// Location:
//   SceUaImppCpim/CCpimHeaderList.h
//
//==============================================================================
class CCpimHeaderList : public CTxtHeaderList
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CCpimHeaderList(IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Copy Constructor.
    CCpimHeaderList(IN const CCpimHeaderList& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CCpimHeaderList();

    //-- << Operators >>

    // Summary:
    //  Assignment Operator.
    CCpimHeaderList& operator=(IN const CCpimHeaderList& rSrc);

    // << Creator >>

    // Summary:
    //  Creates a new header and inserts it in the list.
    CCpimHeader* CreateNewCpimHeader(IN const char* pszHeaderName,
                                     IN EInsertPosition eInsertPos);

    // Summary:
    //  Creates a new header and inserts it in the list.
    CCpimHeader* CreateNewCpimHeader(IN ECpimHeaderType eHeaderType,
                                     IN EInsertPosition eInsertPos);

    // Summary:
    //  Returns a CCpimHeader chain.
    CCpimHeader* Get(IN const char* pszName);

    // Summary:
    //  Returns a CCpimHeader chain.
    const CCpimHeader* Get(IN const char* pszName) const;

    // Summary:
    //  Returns a CCpimHeader chain.
    CCpimHeader* Get(IN ECpimHeaderType eHeaderType);

    // Summary:
    //  Returns a CCpimHeader chain.
    const CCpimHeader* Get(IN ECpimHeaderType eHeaderType) const;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods.
protected:

    // Summary:
    //  Creates a new header using the allocator or MX_NEW.
    virtual GO CTxtHeader* InternalCreateNewHeader(IN const char* pszHeaderName) const;

private:

//-- Hidden Data Members.
protected:
private:

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  CreateNewCpimHeader
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
inline CCpimHeader* CCpimHeaderList::CreateNewCpimHeader(IN const char* pszHeaderName,
                                                         IN EInsertPosition eInsertPos)
{
    return static_cast<CCpimHeader*>(CreateNewTxtHeader(pszHeaderName, eInsertPos));
}

//==============================================================================
//==
//==  CreateNewCpimHeader
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
inline CCpimHeader* CCpimHeaderList::CreateNewCpimHeader(IN ECpimHeaderType eHeaderType,
                                                         IN EInsertPosition eInsertPos)
{
    return static_cast<CCpimHeader*>(CreateNewTxtHeader(g_astCpimHeaderDescriptor[eHeaderType].m_pszLongName,
                                                        eInsertPos));
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
//  Returns:
//      A pointer to the header chain. It points on a CCpimHeader chain if the
//      method is successful (*pres == resS_OK).
//
//  Description:
//      This method is used to retrieve a CPIM header chain.
//
//==============================================================================
inline const CCpimHeader* CCpimHeaderList::Get(IN const char* pszName) const
{
    return static_cast<const CCpimHeader*>(CTxtHeaderList::Get(pszName));
}

// <COMBINE CCpimHeaderList::Get@IN const char*@const>
inline CCpimHeader* CCpimHeaderList::Get(IN const char* pszName)
{
    return static_cast<CCpimHeader*>(CTxtHeaderList::Get(pszName));
}

//==============================================================================
//==
//==  Get
//==
//==============================================================================
//
//  Parameters:
//      eHeaderType:
//          Type of the headers to parse and return.
//
//  Returns:
//      A pointer to the header chain. It points on a CCpimHeader chain if the
//      method is successful (*pres == resS_OK).
//
//  Description:
//      This method is used to retrieve a CPIM header chain.
//
//==============================================================================
inline const CCpimHeader* CCpimHeaderList::Get(IN ECpimHeaderType eHeaderType) const
{
    return static_cast<const CCpimHeader*>(CTxtHeaderList::Get(g_astCpimHeaderDescriptor[eHeaderType].m_pszLongName));
}

// <COMBINE CCpimHeaderList::Get@IN ECpimHeaderType@const>
inline CCpimHeader* CCpimHeaderList::Get(IN ECpimHeaderType eHeaderType)
{
    return static_cast<CCpimHeader*>(CTxtHeaderList::Get(g_astCpimHeaderDescriptor[eHeaderType].m_pszLongName));
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CCPIMHEADER_H

