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
#ifndef MXG_CTXTHEADER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTHEADER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CTXTTOKEN_H
#include "TextMessage/CTxtToken.h"
#endif

#ifndef MXG_CTXTHOSTPORT_H
#include "TextMessage/CTxtHostPort.h"
#endif

#ifndef MXG_CTXTNAMEADDR_H
#include "TextMessage/CTxtNameAddr.h"
#endif

#ifndef MXG_TXTHEADERGRAMMAR_H
#include "TextMessage/TxtHeaderGrammar.h"
#endif

#ifndef MXG_TXTMESSAGECONSTANTS_H
#include "TextMessage/TxtMessageConstants.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;
class CTxtParameter;
class CTxtParameterList;
class IAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//<GROUP TEXT_MESSAGE_VARIABLES>
//
// Summary:
//  Indicates that all next headers MUST be removed.
//
// Description:
//  Indicates that all next headers MUST be removed. Used with
//  RemoveNextHeaders.
const unsigned int g_uREMOVE_ALL = ~0u;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTxtHeader
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 8>
//
// Summary:
//  This class handles parsing and storage of a basic header structure.
//
// Description:
//
//  This class handles parsing and storage of a basic header structure. It
//  can store and understand all header constructs described in the
//  ETxtHeaderGrammar enumeration.
//
//  This class is meant to be the base class of more protocol specific classes.
//  For example, one shall implement a CSipHeader inheriting from this class.
//  Upon creation of a CSipHeader object, this class would receive an instance
//  of a STxtHeaderDescriptor structure. This structure shall describe the
//  construct of the header that this class is to support and will help in
//  parsing and serializing the header type.
//
//  The CTxtHeader class is also a singly linked list of headers. This linked
//  list is constructed for headers that can be combined or can appear multiple
//  times in a packet.
//
// Location:
//   TextMessage/CTxtHeader.h
//
//==============================================================================
class CTxtHeader
{
//-- Friend Declarations

    // This class is friend for memory management issue due to the allocator
    // feature.
    friend class CTxtHeaderList;

protected:
    // Summary:
    //  Indicates what to reset.
    //
    // Description:
    //  Indicates what to reset to the Reset method.
    enum EResetLevel
    {
        // Description:
        //  Resets the parameter list and the header values.
        eBASIC_RESET = 0x1000,

        // Description:
        //  Does basic reset in addition of resetting the m_pNextHeader member.
        eNEXT_HEADERS_RESET = ((int)eBASIC_RESET | 0x1),

        // Description:
        //  Resets all
        eCOMPLETE_RESET = eNEXT_HEADERS_RESET
    };

//M5T_INTERNAL_USE_BEGIN
    // The following are the supported header types.

    // Description:
    // This enumeration describes the data type held in the byte array.
    enum EStructureType
    {
        // Description:
        // The byte array is a SOneTokenForm structure.
        eST_ONE_TOKEN_FORM,

        // Description:
        // The byte array is a STwoTokensForm structure.
        eST_TWO_TOKENS_FORM,

        // Description:
        // The byte array is a SThreeTokensForm structure.
        eST_THREE_TOKENS_FORM,

        // Description:
        // The byte array is a SThreeTokensHostPortForm structure.
        eST_THREE_TOKENS_HOSTPORT_FORM,

        // Description:
        // The byte array is a SNameAddrForm structure.
        eST_NAME_ADDR_FORM,
    };

    // eTHDT_TOKEN.
    // eTHDT_STRING.
    // eTHDT_FREE_STRING.
    // eTHDT_TOKEN_AUTH.
    struct SOneTokenForm
    {
        CTxtToken m_tokFirst;

        SOneTokenForm(IN IAllocator* pAllocator)
        :   m_tokFirst(NULL, pAllocator)
        {};
    };

    // eTHDT_TWO_TOKENS.
    // eTHDT_TOKEN_SLASH_TOKEN.
    struct STwoTokensForm
    {
        CTxtToken m_tokFirst;
        CTxtToken m_tokSecond;

        STwoTokensForm(IN IAllocator* pAllocator)
        :   m_tokFirst(NULL, pAllocator),
            m_tokSecond(NULL, pAllocator)
        {};
    };

    // eTHDT_THREE_TOKENS.
    struct SThreeTokensForm
    {
        CTxtToken m_tokFirst;
        CTxtToken m_tokSecond;
        CTxtToken m_tokThird;

        SThreeTokensForm(IN IAllocator* pAllocator)
        :   m_tokFirst(NULL, pAllocator),
            m_tokSecond(NULL, pAllocator),
            m_tokThird(NULL, pAllocator)
        {};
    };

    // eTHDT_TOKEN_SLASH_TOKEN_SLASH_TOKEN_HOSTPORT
    struct SThreeTokensHostPortForm
    {
        CTxtToken m_tokFirst;
        CTxtToken m_tokSecond;
        CTxtToken m_tokThird;
        CTxtHostPort m_hostPort;

        SThreeTokensHostPortForm(IN IAllocator* pAllocator)
        :   m_tokFirst(NULL, pAllocator),
            m_tokSecond(NULL, pAllocator),
            m_tokThird(NULL, pAllocator),
            m_hostPort(pAllocator)
        {};
    };

    // eTHDT_NAMEADDR.
    // eTHDT_NAMEADDR_WILDCARD.
    struct SNameAddrForm
    {
        CTxtNameAddr m_nameAddr;
        bool m_bIsWildCard;

        SNameAddrForm(IN IAllocator* pAllocator)
        :   m_nameAddr(pAllocator),
            m_bIsWildCard(false)
        {}
    };

//M5T_INTERNAL_USE_END

//-- Published Interface
public:
    //-- << Constructors / Destructors >>

    // Summary:
    //  Constructor.
    CTxtHeader(IN const STxtHeaderDescriptor* pProtocolHeaders,
               IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Copy Constructor.
    CTxtHeader(IN const CTxtHeader& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CTxtHeader();

    // << Operators >>

    // Summary:
    //  Assignment Operator.
    CTxtHeader& operator=(IN const CTxtHeader& rSrc);

    // Summary:
    //  Comparison operator.
    bool operator==(IN const CTxtHeader& rSrc) const;

    // Summary:
    //  Comparison operator.
    bool operator!=(IN const CTxtHeader& rSrc) const;

    // << Information providers >>

    // Summary:
    //  Retrieves the header name in long form.
    const char* GetHeaderLongName() const;

    // Summary:
    //  Retrieves the header name in short form.
    const char* GetHeaderShortName() const;

    // Summary:
    // Extracts the datatype from the header descriptor.
    ETxtHeaderGrammar GetHeaderDataType() const;

    // << Accessors >>

    // Summary:
    //  Returns the number of headers in the chain, but omits the current
    //  header.
    unsigned int GetNextHeadersCount() const;

    // Summary:
    //  Returns the number of headers in the chain, including this one.
    unsigned int GetHeadersCount() const;

    // Summary:
    //  Returns a pointer to the chained header at the given 0-based index.
    CTxtHeader* GetNextHeader(IN unsigned int uIndex = 0);

    //<COMBINE CTxtHeader::GetNextHeader@IN unsigned int>
    const CTxtHeader* GetNextHeader(IN unsigned int uIndex = 0) const;

    // Summary:
    //  Returns true if this header is empty, and can be empty as per the
    //  header definition table.
    bool IsEmptyHeader() const;

    // Summary:
    //  Returns true if the current instance is equivalent to the cited
    //  instance, without regard to the next headers.
    bool IsSingleHdrEquivalent(IN const CTxtHeader& rSrc) const;

    // Summary:
    //  Returns the header data as a SOneTokenForm structure.
    SOneTokenForm& GetOneTokenForm();

    //<COMBINE CTxtHeader::GetOneTokenForm>
    const SOneTokenForm& GetOneTokenForm() const;

    // Summary:
    //  Returns the header data as a STwoTokensForm structure.
    STwoTokensForm& GetTwoTokensForm();

    //<COMBINE CTxtHeader::GetTwoTokensForm>
    const STwoTokensForm& GetTwoTokensForm() const;

    // Summary:
    //  Returns the header data as a SThreeTokensForm structure.
    SThreeTokensForm& GetThreeTokensForm();

    //<COMBINE CTxtHeader::GetThreeTokensForm>
    const SThreeTokensForm& GetThreeTokensForm() const;

    // Summary:
    //  Returns the header data as a SThreeTokensHostPortForm structure.
    SThreeTokensHostPortForm& GetThreeTokensHostPortForm();

    //<COMBINE CTxtHeader::GetThreeTokensHostPortForm>
    const SThreeTokensHostPortForm& GetThreeTokensHostPortForm() const;

    // Summary:
    //  Returns the header data as a SNameAddrForm structure.
    SNameAddrForm& GetNameAddrForm();

    //<COMBINE CTxtHeader::GetNameAddrForm>
    const SNameAddrForm& GetNameAddrForm() const;

//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Indicates if this header contains unparsable data.
    bool IsUnparsable() const;

    // Summary:
    //  This method allows retrieval of unparsed data available in this header.
    const char* GetUnparsableData() const;
//M5T_INTERNAL_USE_END

    //-- << Parameters management >>

    // Summary:
    //  Creates an empty parameter list.
    virtual CTxtParameterList* CreateNewParameterList(IN bool bStartWithSeparator = true,
                                                      IN char cSeparator = g_cSEMI);

    // Summary:
    //  Removes the parameter list.
    void RemoveParameterList();

    // Summary:
    //  Returns the current parameter list.
    CTxtParameterList* GetParameterList();

    //<COMBINE CTxtHeader::GetParameterList>
    const CTxtParameterList* GetParameterList() const;

    // Summary:
    //  Fetches a parameter in the parameter list by name or index.
    CTxtParameter* GetParam(IN const char* pszParameterName);

    //<COMBINE CTxtHeader::GetParam@IN const char*>
    const CTxtParameter* GetParam(IN const char* pszParameterName) const;

    //<COMBINE CTxtHeader::GetParam@IN const char*>
    CTxtParameter* GetParam(IN unsigned int uParameterIndex);

    //<COMBINE CTxtHeader::GetParam@IN const char*>
    const CTxtParameter* GetParam(IN unsigned int uParameterIndex) const;

    // Summary:
    //  Fetches a parameter value in the list.
    CTxtToken* GetParamToken(IN const char* pszParameterName);

    //<COMBINE CTxtHeader::GetParamToken>
    const CTxtToken* GetParamToken(IN const char* pszParameterName) const;

    // Summary:
    //  Fetches a parameter value in the list.
    const char* GetParamString(IN const char* pszParameterName);

    //<COMBINE CTxtHeader::GetParamString>
    const char* GetParamString(IN const char* pszParameterName) const;

    //-- << Modifiers >>

    // Summary:
    //  Sets the token characteristics on the internal tokens.
    mxt_result SetTokenCharacteristics(IN unsigned int uTokenNumber,
                                       IN const CTxtToken::STokenCharacteristics* pstTokenCharacteristics);

    // Summary:
    //  Inserts a header at the end of the chain.
    mxt_result AppendNextHeader(IN const CTxtHeader& rNewHeader);

    // Summary:
    //  Inserts a header at the given 0-based index in the chain. Index 0
    //  refers to this CTxtHeader's first next header.
    mxt_result InsertNextHeader(IN unsigned int uIndex,
                                IN const CTxtHeader& rNewHeader);

    // Summary:
    //  Replaces the next headers with the headers in parameter.
    mxt_result ReplaceNextHeader(IN const CTxtHeader& rNewHeader);

    // Summary:
    //  Removes a header from the chain, at the given 0-based index. Index 0
    //  refers to this CTxtHeader's first next header.
    mxt_result RemoveNextHeaders(IN unsigned int uIndex,
                                 IN unsigned int uCount = g_uREMOVE_ALL);

    // Summary:
    //  Clears all parsed/Set data.
    virtual void Reset(IN EResetLevel eResetLevel = eCOMPLETE_RESET);

//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  This method allows the header to set data that fails to parse as its
    //  unparsed data.
    void SetUnparsableData(IN const char* pszUnparsableData, IN unsigned int uUnparsableDataLength);

    // Summary:
    //  This method releases any unparsable data that is set in the header.
    void ClearUnparsableData();
//M5T_INTERNAL_USE_END

    //-- << Parsing >>

    // Summary:
    //  Parses the provided text to find data for the current header type.
    mxt_result Parse(INOUT const char*& rpszHeaderText,
                     IN CTxtToken* ptokParsingToken = NULL);

    //-- << Serialization >>
    // Summary:
    //  Serializes the header values into the provided blob.
    void Serialize(INOUT CBlob& rBlob,
                   IN bool bFirstHeader = true,
                   IN CTxtToken* ptokSerializingToken = NULL) const;

    //-- << Helpers >>

    // Summary:
    //  Sets the header name for an extension header.
    void SetExtensionHeaderName(IN const char* pszExtensionHeaderName);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Helpers >>

    // Summary:
    //  Creates a new header of the same type as the current one. The child
    //  class are expected to overload this method.
    virtual GO CTxtHeader* CreateNewHeader();

    // Summary:
    //  Destroys a header created through the CreateNewHeader method.
    void ReleaseHeader(IN TOA CTxtHeader* pHeader);

    // Summary:
    //  Properly resets all header values.
    void ResetHeaderValues();

    // Summary:
    //  Compares two header chains.
    bool CompareHeaderChain(IN const CTxtHeader& rTxtHeaderChain1,
                            IN const CTxtHeader& rTxtHeaderChain2) const;

    // Summary:
    // Checks whether or not the given bit is set in the header descriptor.
    bool IsSet(IN uint32_t uBitmask) const;

    // Summary:
    // Allocates memory using the allocator if it is available.
    GO uint8_t* AllocateMemory(IN unsigned int uAllocationSize) const;

    // Summary:
    // Releases memory using the allocator is it is available.
    void ReleaseMemory(IN TOA uint8_t* pMemoryBlock) const;

    // Summary:
    // Creates the data structure to hold the header data.
    void CreateDataStructure();

    // Summary:
    // Destroys the data structure that holds the header data.
    void DestroyDataStructure();

    // Summary:
    //  Advances rpszHeaderText to the start of the next header.
    bool SkipToNextHeader(INOUT const char*& rpszHeaderText, IN bool bCheckComma);

    // Summary:
    //  Parses a single header from the text provided as parameter.
    mxt_result ParseSingleHeader(INOUT const char*& rpszHeaderText,
                                 IN CTxtToken* ptokParsingToken = NULL);

    // Summary:
    //  Serializes a token.
    void SerializeToken(IN const CTxtToken& rTokenToSerialize,
                        IN CTxtToken* pSerializingToken,
                        INOUT CBlob& rBlob) const;

    // Summary:
    //  Parses a token.
    mxt_result ParseToken(INOUT const char*& rpszTokenText,
                          INOUT CTxtToken& rParsingToken,
                          INOUT CTxtToken* pContainerToken = NULL);

    // Summary:
    //  Returns the header form encoded and held by m_puData.
    EStructureType GetHeaderForm() const;

private:
    //-- << Deactivated Constructors >>

    // Summary:
    //  Default constructor.
    CTxtHeader();

//-- Hidden Data Members
protected:

    // The header description.
    const STxtHeaderDescriptor* m_pstHeaderDescription;

    // This indicates if this is an extension header. This is required to
    // correctly manage extension header names.
    bool m_bExtensionHeader;

    // The header name when it is an extension.
    char* m_pszExtensionHeaderName;

    // The allocator to use for memory allocation.
    IAllocator* m_pAllocator;

    // Next header for multi-headers.
    CTxtHeader* m_pNextHeader;

    // The parameter list.
    CTxtParameterList* m_pParameterList;

    // This is the data holder, it must be accessed correctly according to the
    // header data type. This member will be as big as the structure it
    // contains + 4 bytes. The first four bytes of the array is an integer value
    // (from EStructureType) that indicates what is the structure that is held
    // in the byte array.
    uint8_t* m_puData;

    // This pointer contains the unparsable data associated to this header.
    // When this member is non-NULL, the header failed to parse and its data
    // structure pointed by m_puData is empty. m_puData is not NULL but the
    // data structure it contains holds no data. This string contains the
    // whole header value in its unparsed form.
    char* m_pszUnparsableData;

//-- Public constants
public:

    //-- These are helper bitmasks to set and extract information in/from the
    //-- SHeaderDescriptor::m_uBitset member.
    static const uint32_t ms_uDATATYPE_MASK;
    static const uint32_t ms_uCAN_BE_EMPTY_SET;
    static const uint32_t ms_uMULTIPLE_INSTANCE_SET;
    static const uint32_t ms_uCAN_BE_COMBINED_SET;
    static const uint32_t ms_uGENERATE_COMBINED_SET;
    static const uint32_t ms_uHAS_PARAMS_SET;
    static const uint32_t ms_uPARAMS_LOCATION_BEFORE_VALUE_SET;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  OPERATORS  =============================================================
//==============================================================================

//==============================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          The CTxtHeader to which this header is compared.
//
//  Returns:
//      True if the header chains are different.
//
//  Description:
//      Inequality operator.
//
//  See Also:
//      operator==
//
//==============================================================================
inline bool CTxtHeader::operator!=(IN const CTxtHeader& rSrc) const
{
    return !(*this == rSrc);
}

//==============================================================================
//====  INFORMATION PROVIDERS METHODS  =========================================
//==============================================================================

//==============================================================================
//==
//==  GetHeaderLongName
//==
//==============================================================================
//
//  Returns:
//      The header name in its long form. Can be NULL if the header is
//      ill-configured.
//
//  Description:
//      This method retrieves the header name in its long form.
//
//==============================================================================
inline const char* CTxtHeader::GetHeaderLongName() const
{
    MX_ASSERT(m_pstHeaderDescription != NULL);
    return m_pstHeaderDescription == NULL ?
               NULL :
               m_pstHeaderDescription->m_pszLongName == NULL ?
                   m_pszExtensionHeaderName :
                   m_pstHeaderDescription->m_pszLongName;
}

//==============================================================================
//==
//==  GetHeaderShortName
//==
//==============================================================================
//
//  Returns:
//      The header name in its short form. Can be NULL if the header is
//      ill-configured or the short form is unavailable.
//
//  Description:
//      This method retrieves the header name in its short form.
//
//==============================================================================
inline const char* CTxtHeader::GetHeaderShortName() const
{
    MX_ASSERT(m_pstHeaderDescription != NULL);
    return m_pstHeaderDescription == NULL ? NULL : m_pstHeaderDescription->m_pszShortName;
}

//==============================================================================
//==
//==  GetHeaderDataType
//==
//==============================================================================
//
//  Returns:
//      The header data type as defined in ETxtHeaderGrammar. Or eTHDT_UNKNOWN
//      when the header has no associated descriptor.
//
//  Description:
//      This method extracts the datatype from the header descriptor.
//
//==============================================================================
inline ETxtHeaderGrammar CTxtHeader::GetHeaderDataType() const
{
    MX_ASSERT(m_pstHeaderDescription != NULL);
    return m_pstHeaderDescription == NULL ? eTHDT_UNKNOWN :
               static_cast<ETxtHeaderGrammar>(static_cast<int>(m_pstHeaderDescription->m_uBitset & ms_uDATATYPE_MASK));
}

//==============================================================================
//==
//==  IsSet
//==
//==============================================================================
//
//  Parameters:
//    uBitmask:
//      The bitmask for which to look in the bitset of the header descriptor.
//
//  Returns:
//      - true: the bitmask is set in the bitset.
//      - false: otherwise.
//
//  Description:
//      This method returns whether or not the provided bitmask is currently
//      set in the header descriptor bitset.
//
//==============================================================================
inline bool CTxtHeader::IsSet(IN uint32_t uBitmask) const
{
    MX_ASSERT(m_pstHeaderDescription != NULL);
    return m_pstHeaderDescription == NULL ? false : (m_pstHeaderDescription->m_uBitset & uBitmask) == uBitmask;
}

//==============================================================================
//====  ACCESSORS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  GetHeadersCount
//==
//==============================================================================
//
//  Returns:
//      The number of headers in the chain, including the present instance.
//
//  Description:
//      This method counts the number of CTxtHeader pointers that are linked in
//      the chain including this instance.
//
//  See Also:
//      GetNextHeadersCount
//
//==============================================================================
inline unsigned int CTxtHeader::GetHeadersCount() const
{
    // "this" counts in the header list.
    return 1 + GetNextHeadersCount();
}

//--<COMBINE CTxtHeader::GetNextHeader@IN unsigned int>
inline const CTxtHeader* CTxtHeader::GetNextHeader(IN unsigned int uIndex) const
{
    return const_cast<CTxtHeader*>(this)->GetNextHeader(uIndex);
}

//==============================================================================
//==
//==  GetOneTokenForm
//==
//==============================================================================
//
//  Returns:
//      The SOneTokenForm structure used internally to hold the header data.
//
//  Description:
//      This method returns the header data as a SOneTokenForm structure. It
//      asserts that the internal data is a SOneTokenForm structure.
//
//==============================================================================
inline CTxtHeader::SOneTokenForm& CTxtHeader::GetOneTokenForm()
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_ONE_TOKEN_FORM ||
              GetHeaderForm() == eST_TWO_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_HOSTPORT_FORM);
    return *reinterpret_cast<SOneTokenForm*>(m_puData + sizeof(uint32_t));
}

//--<COMBINE CTxtHeader::GetOneTokenForm>
inline const CTxtHeader::SOneTokenForm& CTxtHeader::GetOneTokenForm() const
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_ONE_TOKEN_FORM ||
              GetHeaderForm() == eST_TWO_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_HOSTPORT_FORM);
    return *reinterpret_cast<SOneTokenForm*>(m_puData + sizeof(uint32_t));
}

//==============================================================================
//==
//==  GetTwoTokensForm
//==
//==============================================================================
//
//  Returns:
//      The STwoTokensForm structure used internally to hold the header data.
//
//  Description:
//      This method returns the header data as a STwoTokensForm structure. It
//      asserts that the internal data is a STwoTokensForm structure.
//
//==============================================================================
inline CTxtHeader::STwoTokensForm& CTxtHeader::GetTwoTokensForm()
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_TWO_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_HOSTPORT_FORM);
    return *reinterpret_cast<STwoTokensForm*>(m_puData + sizeof(uint32_t));
}

//--<COMBINE CTxtHeader::GetTwoTokensForm>
inline const CTxtHeader::STwoTokensForm& CTxtHeader::GetTwoTokensForm() const
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_TWO_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_HOSTPORT_FORM);
    return *reinterpret_cast<STwoTokensForm*>(m_puData + sizeof(uint32_t));
}

//==============================================================================
//==
//==  GetThreeTokensForm
//==
//==============================================================================
//
//  Returns:
//      The SThreeTokensForm structure used internally to hold the header data.
//
//  Description:
//      This method returns the header data as a SThreeTokensForm structure. It
//      asserts that the internal data is a SThreeTokensForm structure.
//
//==============================================================================
inline CTxtHeader::SThreeTokensForm& CTxtHeader::GetThreeTokensForm()
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_THREE_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_HOSTPORT_FORM);
    return *reinterpret_cast<SThreeTokensForm*>(m_puData + sizeof(uint32_t));
}


//--<COMBINE CTxtHeader::GetThreeTokensForm>
inline const CTxtHeader::SThreeTokensForm& CTxtHeader::GetThreeTokensForm() const
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_THREE_TOKENS_FORM ||
              GetHeaderForm() == eST_THREE_TOKENS_HOSTPORT_FORM);
    return *reinterpret_cast<SThreeTokensForm*>(m_puData + sizeof(uint32_t));
}

//==============================================================================
//==
//==  GetThreeTokensHostPortForm
//==
//==============================================================================
//
//  Returns:
//      The SThreeTokensHostPortForm structure used internally to hold the
//      header data.
//
//  Description:
//      This method returns the header data as a SThreeTokensHostPortForm
//      structure. It asserts that the internal data is a
//      SThreeTokensHostPortForm structure.
//
//==============================================================================
inline CTxtHeader::SThreeTokensHostPortForm& CTxtHeader::GetThreeTokensHostPortForm()
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_THREE_TOKENS_HOSTPORT_FORM);
    return *reinterpret_cast<SThreeTokensHostPortForm*>(m_puData + sizeof(uint32_t));
}

//--<COMBINE CTxtHeader::GetThreeTokensHostPortForm>
inline const CTxtHeader::SThreeTokensHostPortForm& CTxtHeader::GetThreeTokensHostPortForm() const
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_THREE_TOKENS_HOSTPORT_FORM);
    return *reinterpret_cast<SThreeTokensHostPortForm*>(m_puData + sizeof(uint32_t));
}

//==============================================================================
//==
//==  GetNameAddrForm
//==
//==============================================================================
//
//  Returns:
//      The SNameAddrForm structure used internally to hold the header data.
//
//  Description:
//      This method returns the header data as a SNameAddrForm structure. It
//      asserts that the internal data is a SNameAddrForm structure.
//
//==============================================================================
inline CTxtHeader::SNameAddrForm& CTxtHeader::GetNameAddrForm()
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_NAME_ADDR_FORM);
    return *reinterpret_cast<SNameAddrForm*>(m_puData + sizeof(uint32_t));
}

//--<COMBINE CTxtHeader::GetNameAddrForm>
inline const CTxtHeader::SNameAddrForm& CTxtHeader::GetNameAddrForm() const
{
    MX_ASSERT(m_puData != NULL);
    MX_ASSERT(GetHeaderForm() == eST_NAME_ADDR_FORM);
    return *reinterpret_cast<SNameAddrForm*>(m_puData + sizeof(uint32_t));
}

//M5T_INTERNAL_USE_BEGIN

//==============================================================================
//==
//==  IsUnparsable
//==
//==============================================================================
//
//  Returns:
//      - true: If the header contains unparsable data.
//      - false: otherwise.
//
//  Description:
//      This method indicates if the header has its unparsable data set. It
//      indicates that a header failed to parse the data returned by
//      GetUnparsableData.
//
//==============================================================================
inline bool CTxtHeader::IsUnparsable() const
{
    return m_pszUnparsableData != NULL;
}

//==============================================================================
//==
//==  GetUnparsableData
//==
//==============================================================================
//
//  Returns:
//      The unparsable data. NULL if none has been set.
//
//  Description:
//      This method allows retrieval of unparsed data available in this header.
//
//      Unparsable data is defined has a header value that failed at some point
//      in the parsing process. It contains all the header value has it appeared
//      in the original packet. The end of the data is marked by a '\0'
//      character. The application can retry parsing that data using its own
//      technics.
//
//==============================================================================
inline const char* CTxtHeader::GetUnparsableData() const
{
    return m_pszUnparsableData;
}

//==============================================================================
//==
//==  GetHeaderForm
//==
//==============================================================================
//
//  Returns:
//      The header form that is held and pointed by the m_puData member.
//
//  Description:
//      This method returns the header form encoded and held by m_puData.
//
//==============================================================================
inline CTxtHeader::EStructureType CTxtHeader::GetHeaderForm() const
{
    MX_ASSERT(m_puData != NULL);
    return static_cast<EStructureType>(*reinterpret_cast<uint32_t*>(m_puData));
}

//M5T_INTERNAL_USE_END

//==============================================================================
//====  PARAMETERS MANAGEMENT  =================================================
//==============================================================================

//==============================================================================
//==
//==  GetParameterList
//==
//==============================================================================
//
//  Returns:
//      The current parameter list. NULL if no parameter list is set.
//
//  Description:
//      This method returns the current parameter list.
//
//==============================================================================
inline CTxtParameterList* CTxtHeader::GetParameterList()
{
    return m_pParameterList;
}

//--<COMBINE CTxtHeader::GetParameterList>
inline const CTxtParameterList* CTxtHeader::GetParameterList() const
{
    return m_pParameterList;
}

//--<COMBINE CTxtHeader::GetParam@IN const char*>
inline const CTxtParameter* CTxtHeader::GetParam(IN const char* pszParameterName) const
{
    return const_cast<CTxtHeader*>(this)->GetParam(pszParameterName);
}

//--<COMBINE CTxtHeader::GetParam@IN const char*>
inline const CTxtParameter* CTxtHeader::GetParam(IN unsigned int uParameterIndex) const
{
    return const_cast<CTxtHeader*>(this)->GetParam(uParameterIndex);
}

//--<COMBINE CTxtHeader::GetParamToken>
inline const CTxtToken* CTxtHeader::GetParamToken(IN const char* pszParameterName) const
{
    return const_cast<CTxtHeader*>(this)->GetParamToken(pszParameterName);
}

//--<COMBINE CTxtHeader::GetParamString>
inline const char* CTxtHeader::GetParamString(IN const char* pszParameterName) const
{
    return const_cast<CTxtHeader*>(this)->GetParamString(pszParameterName);
}

//==============================================================================
//====  HELPERS METHODS  =======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTHEADER_H

