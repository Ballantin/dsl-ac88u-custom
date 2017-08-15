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
#ifndef MXG_CTXTHEADERLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTHEADERLIST_H
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

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CTxtHeader;
class CTxtToken;
class CBlob;
struct STxtHeaderDescriptor;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTxtHeaderList
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 9>
//
// Summary:
//   This class manages a list of CTxtHeaders.
//
// Description:
//   This class manages a list of CTxtHeaders. It is merely a container. It is
//   responsible for parsing and serializing multiple headers.
//
//   The CTxtHeaderList holds the headers in a vector, where each different
//   header type is assigned to a different index in the vector.
//
//   Since CTxtHeaders of the same type can be linked together to create a
//   linked-list, their use within the CTxtHeaderList effectively creates a two
//   dimensional array of headers, where the first dimension is the type of
//   header, and the second dimension is additional headers of the same type.
//
// Location:
//   TextMessage/CTxtHeaderList.h
//
//==============================================================================
class CTxtHeaderList
{
//-- Friend Declarations
    friend class CTxtBasePacket;

//-- New types definition
public:

    // Summary:
    //   This enum is used when creating and adding a new header to the list.
    //
    // Description:
    //   This enum is used when creating and adding a new header to the list.
    enum EInsertPosition
    {
        // Description:
        // Add the new header to the end of headers of the same type.
        eAT_END,

        // Description:
        // Add the new header at the beginning of headers of the same type.
        eAT_BEGINNING,
    };

//-- Published Interface
public:
    //-- << Constructors >>

    // Summary:
    //  Default Constructor.
    CTxtHeaderList(IN IAllocator* pAllocator = NULL, IN bool bParseExtensionHeaders = true);

    // Summary:
    //  Copy Constructor.
    CTxtHeaderList(IN const CTxtHeaderList& rSrc);

    //-- << Destructor >>

    // Summary:
    //  Destructor.
    virtual ~CTxtHeaderList();

    //-- << Operators >>

    // Summary:
    //  Assignment Operator.
    CTxtHeaderList& operator=(IN const CTxtHeaderList& rSrc);

    // Summary:
    //  Comparison operator.
    bool operator==(IN const CTxtHeaderList& rSrc) const;

    // Summary:
    //  Inequality operator.
    bool operator!=(IN const CTxtHeaderList& rSrc) const;

    // << Creator >>

    // Summary:
    //  Creates a new header and inserts it in the list.
    CTxtHeader* CreateNewTxtHeader(IN const char* pszHeaderName, IN EInsertPosition eInsertPos);

    //-- << Header list management >>

    // Summary:
    //  Adds the header list at the end of the list.
    mxt_result Append(IN const CTxtHeaderList& rHeaderList);

    // Summary:
    //  Appends the header list and replaces the headers that have the same
    //  type as the one in the passed list.
    void ReplaceHeaders(IN const CTxtHeaderList& rNewHeaderList);

    // Summary:
    //  Replaces all the headers of a type with a new header.
    mxt_result ReplaceHeaderTypeWith(IN const CTxtHeader& rHeader);

    // Summary:
    //  Removes the n'th header of with the specified name.
    mxt_result RemoveHeader(IN const char* pszName, IN unsigned int uIndex);

    // Summary:
    //  Removes all headers of the specified type.
    mxt_result RemoveHeaderType(IN const char* pszName);

    // Summary:
    //  Sorts the headers using the defined header order.
    void Sort() const;

    // Summary:
    //  Returns a CTxtHeader chain.
    const CTxtHeader* Get(IN const char* pszName) const;

    //<COMBINE CTxtHeaderList::Get@IN const char*@const>
    CTxtHeader* Get(IN const char* pszName);

    // Summary:
    //  Returns the number of header types that are available in the list.
    unsigned int GetHeaderTypesCount() const;

    // Summary:
    //  Returns true if the list is empty.
    bool IsEmpty() const;

    // Summary:
    //  Resets the header list to its initial state.
    void Reset();

    //-- << Unparsable Data Access >>

    // Summary:
    //  Returns the number of unparsable data that are currently available.
    unsigned int GetUnparsableHeadersCount() const;

    // Summary:
    //  Returns an unparsable header.
    const CTxtHeader& GetUnparsableHeader(IN unsigned int uIndex) const;

    //-- << Parsing >>

    // Summary:
    //  Creates a headerlist by parsing the content of the provided text.
    mxt_result Parse(INOUT const char*& rpszHeaderListText,
                     IN CTxtToken* ptokParsingToken = NULL);

    // Summary:
    //  Parses a single header line.
    mxt_result ParseSingleHeaderLine(INOUT const char*& rpszHeaderLineText,
                                     IN CTxtToken* ptokParsingToken = NULL);

    //-- << Serialization >>

    // Summary:
    //  Outputs the headers.
    void Serialize(INOUT CBlob& rBlob, IN CTxtToken* ptokSerializingToken = NULL) const;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    //-- << Helpers >>

    // Summary:
    //  Returns the header type index.
    mxt_result FindTypeIndex(IN const char* pszSearchedName, OUT unsigned int& ruIndex) const;

    // Summary:
    //  Inserts a header at the specified position in the chain.
    mxt_result Insert(IN TOA CTxtHeader* pNewHeader, IN EInsertPosition eInsertPosition);

    // Summary:
    //  Removes a header from the specified position in the chain.
    mxt_result Remove(IN unsigned int uTypeIndex, IN unsigned int uHeaderIndex);

    // Summary:
    //  Performs a dichotomic search for the provided header name in the
    //  provided header descriptors array.
    const STxtHeaderDescriptor* FindHeaderDescriptor(IN const STxtHeaderDescriptor* pstHeaderDescriptors,
                                                     IN unsigned int uArraySize,
                                                     IN const char* pszHeaderName,
                                                     OUT int* pnIndex = NULL) const;

    // Summary:
    //  Creates a new header using the allocator or MX_NEW. Child classes are
    //  expected to override this method.
    virtual GO CTxtHeader* InternalCreateNewHeader(IN const char* pszHeaderName) const;

    // Summary:
    //  Destroys a header created through the InternalCreateNewHeader method.
    void ReleaseHeader(IN TOA CTxtHeader* pHeader);

    // Summary:
    //  Allocates uSize bytes of memory using the allocator or not.
    GO uint8_t* AllocateMemory(IN unsigned int uSize) const;

    // Summary:
    //  Releases a memory block allocated with AllocateMemory.
    void ReleaseMemory(IN TOA uint8_t* pBuffer) const;

    //-- << Sorter >>

    // Summary:
    //  This method this method sorts the header vector according to their
    //  relative order as defined in their descriptors.
    static int SortCompare(IN CTxtHeader* const& rpOneElement,
                           IN CTxtHeader* const& rpOtherElement,
                           IN mxt_opaque opq);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

//-- Hidden Data Members
protected:

    // The allocator to use when allocating headers.
    // This is protected so child classes can access it.
    IAllocator* m_pAllocator;

private:

    // The list of headers. Each index contains a pointer to a CTxtHeader which
    // is a chain of headers of the same type.
    //
    // mutable because the Sort method can be called on const object.
    mutable CVector<CTxtHeader*> m_vecpHeaders;

    // The list of unparsable headers. Each index contains a pointer to a
    // CTxtHeader that failed to parse, links in those headers MUST be ignored
    // and each index is a single header not a header type.
    CVector<CTxtHeader*> m_vecpUnparsableHeaders;

    // This flag indicates if extension headers are to be parsed or not. When
    // false, parsing fails automatically in ParseSingleHeaderLine when an extension
    // header is encountered.
    bool m_bParseExtensionHeaders;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  HEADER LIST MANAGEMENT METHODS  ========================================
//==============================================================================

//==============================================================================
//==
//==  Sort
//==
//==============================================================================
//
//  Description:
//      This method sorts the contained headers by using the predefined header
//      order.
//
//  See Also:
//      STxtHeaderDescriptor::m_uSerializationOrder
//
//  Example:
//    This example is derived from SIP headers "Via", "From", "To" and
//    "Contact". It uses the hypothesis that these header are to be serialized
//    in the following order: "Via", "From", "To", "Contact".
//
//  #Initial list order:#
//  <CODE>
//  Via1
//  To
//  Contact1
//  Via2
//  From
//  Contact2
//  </CODE>
//
//  #After calling Sort:#
//  <CODE>
//  Via1
//  Via2
//  From
//  To
//  Contact1
//  Contact2
//  </CODE>
//
//==============================================================================
inline void CTxtHeaderList::Sort() const
{
    m_vecpHeaders.Sort(NULL, 0);
}

//==============================================================================
//==
//==  Get
//==
//==============================================================================
//
//  Parameters:
//      pszName:
//          Name of the headers to get.
//
//  Returns:
//      A pointer to the first header with pszName as a header name. Additional
//      headers of the same name (if any) are linked to this header and can be
//      accessed using CTxtHeader::GetNextHeader. Can return NULL if no such
//      header exists within the header list.
//
//  Description:
//      This method is used to retrieve a header chain of the specified type.
//
//==============================================================================
inline const CTxtHeader* CTxtHeaderList::Get(IN const char* pszName) const
{
    // Use a const cast to reuse existing code in the non-const Get method.
    return const_cast<CTxtHeaderList*>(this)->Get(pszName);
}

//==============================================================================
//==
//==  GetHeaderTypesCount
//==
//==============================================================================
//
//  Returns:
//      Number of different header types.
//
//  Description:
//      Counts the number of header types that are in the list. Each header type
//      have different header names.
//
//==============================================================================
inline unsigned int CTxtHeaderList::GetHeaderTypesCount() const
{
    return m_vecpHeaders.GetSize();
}

//==============================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      - true: The list is empty.
//      - false: The list is NOT empty.
//
//  Description:
//      This method checks if the header list is empty.
//
//==============================================================================
inline bool CTxtHeaderList::IsEmpty() const
{
    return m_vecpHeaders.IsEmpty();
}

//==============================================================================
//====  UNPARSABLE DATA ACCESS METHODS  ========================================
//==============================================================================

//==============================================================================
//==
//==  GetUnparsableHeadersCount
//==
//==============================================================================
//
//  Returns:
//      The number of unparsable headers in the header list.
//
//  Description:
//      This method returns the number of unparsable headers in the header list.
//
//==============================================================================
inline unsigned int CTxtHeaderList::GetUnparsableHeadersCount() const
{
    return m_vecpUnparsableHeaders.GetSize();
}

//==============================================================================
//==
//==  GetUnparsableHeader
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          The 0-based index of the header to retrieved. MUST be equal or
//          greater than 0 and lower than GetUnparsableHeadersCount.
//
//  Returns:
//      The unparsable header at index uIndex.
//
//  Description:
//      This method returns the unparsable header at index uIndex. Absolutely
//      no validation is performed on uIndex for validity. The caller has the
//      responsibility to validate that uIndex is within the bounds of the
//      GetUnparsableHeadersCount returned value.
//
//  Warning:
//      Calling this method on a non-const object:
//          - that has been constructed through parsing AND
//          - where the parsing returned a failure AND
//          - from which unparsable headers have been removed by
//              - CTxtHeader::ReplaceNextHeader
//              - CTxtHeader::RemoveNextHeaders
//              - CTxtHeader::Reset
//              - CTxtHeaderList::ReplaceHeaders
//              - CTxtHeaderList::ReplaceHeaderTypeWith
//              - CTxtHeaderList::RemoveHeader
//              - CTxtHeaderList::RemoveHeaderType
//
//      CAN provoke a crash. This method is based on a vector that only
//      contains a copy of the pointer to a header object that is somewhere
//      in the main header list vector or header chains. If any of the above
//      methods are used, the copied pointer MAY become invalid. <B>This method
//      SHOULD NOT be used when a non-const header list has been modified.</B>
//
//==============================================================================
inline const CTxtHeader& CTxtHeaderList::GetUnparsableHeader(IN unsigned int uIndex) const
{
    return *m_vecpUnparsableHeaders[uIndex];
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTHEADERLIST_H

