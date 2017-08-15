//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_CSTRING_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSTRING_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT, MXD_uCSTRING_BLOCK_LENGTH, MXD_STRING_DISABLE_REFCOUNT
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

// Interface Realized & Parent

// Data members
#ifndef MXG_CALIGNEDVARIABLESTORAGE_H
#include "Basic/CAlignedVariableStorage.h"
#endif

#ifndef MXG_MXSTRINGFORMAT_H
#include "Basic/MxStringFormat.h"
#endif

#ifndef MXG_CMARSHALER_H
#include "Cap/CMarshaler.h"
#endif

#ifndef MXG_CATOMIC_H
#include "Kernel/CAtomic.h"
#endif


// Forward Declarations Outside of Namespace

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations inside Namespace

//-- These forward declarations are strictly for documentation purpose
#if 0
//==SDOC========================================================================
//==
//==   operator<<
//==
//==============================================================================
//<GROUP CString>
//
//  Summary:
//   Marshaling input operator.
//
//  Parameters:
//      rMarshaler:  CMarshaler object into which CString is inserted.
//
//      rStr:  Left-hand CString to be inserted.
//
//  Returns:
//      Reference to the Cmarshaler object.
//
//  Description:
//      Inserts the CString "rStr" into the CMarshaler object "rMarshaler".
//
//==EDOC========================================================================
CMarshaler& operator<< ( IN CMarshaler& rMarshaler,
                         IN const CString& rStr );

//==SDOC========================================================================
//==
//==   operator>>
//==
//==============================================================================
//<GROUP CString>
//
//  Summary:
//   Marshaling output operator.
//
//  Parameters:
//      rMarshaler:  CMarshaler object containing a c-style string.
//
//      rStr:  CString object into which the c-style string from CMarshaler
//             is inserted.
//
//  Returns:
//      Reference to the CMarshaler object.
//
//  Description:
//      Extracts the c-style string from the CMarshaler object "rMarshaler"
//      and inserts it into the CString "rStr".
//
//==EDOC========================================================================
CMarshaler& operator>> ( IN CMarshaler& rMarshaler,
                         IN CString& rStr );

//==SDOC========================================================================
//==
//==   operator+
//==
//==============================================================================
//<GROUP CString>
//
//  Summary:
//   Appends a CString to the end of a c-style string.
//
//  Parameters:
//      pszLhs:  Left-hand c-style string.
//
//      strRhs:  Right-hand CString operand.
//
//  Returns:
//      A CString that is the result of the sum of both operands.
//
//  Description:
//      See \CString::operator+(IN const \CString&) const
//      for more details.
//
//  See Also:
//      <LINK CString::+@IN const CString&@const,
//            \CString::operator+(IN const \CString&) const>
//
//==EDOC========================================================================
CString operator+(IN const char* pszLhs, IN const CString& strRhs);

//==SDOC========================================================================
//==
//==   operator+
//==
//==============================================================================
//<GROUP CString>
//
//  Summary:
//   Appends a CString to the end of a character.
//
//  Parameters:
//      c:  Left-hand character.
//
//      strRhs:  Right-hand CString operand.
//
//  Returns:
//      A CString that is the result of the sum of both operands.
//
//  Description:
//      See \CString::operator+(IN const \CString&) const
//      for more details.
//
//  See Also:
//      <LINK CString::+@IN const CString&@const,
//            \CString::operator+(IN const \CString&) const>
//
//==EDOC========================================================================
CString operator+(IN const char c, IN const CString& strRhs);
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
const uint16_t uCSTRING_MAX_VALID_SIZE = 0xFFFF;

#ifndef MXD_uCSTRING_BLOCK_LENGTH
const uint8_t uCSTRING_BLOCK_LENGTH = 32;
#else
const uint8_t uCSTRING_BLOCK_LENGTH = MXD_uCSTRING_BLOCK_LENGTH;
#endif

const unsigned int uCOPY_ALL = uCSTRING_MAX_VALID_SIZE;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//==  Class: CString
//=================================
//<GROUP CAP_CLASSES>
//
// Summary:
//  Class used to encapsulate a c-style string.
//
// Description:
//  The CString class is used to encapsulate a c-style string. It has a
//  resizable buffer to hold the string. The buffer's capacity can be modified
//  automatically (insert, append, etc.) or explicitly (ReduceCapacity and
//  ReserveCapacity).
//
//  Internal buffer struct diagram
//
//  <CODE>
//  +----------+----------+------+-----------------+----+-=-=-=-=-+
//  | RefCount | Capacity | Size | Array of chars  | \\0 | ....... |
//  +----------+----------+------+-----------------+----+-=-=-=-=-+
//   2/4 Bytes   2 Bytes  2 Bytes     n Bytes
//  </CODE>
//
//  Note:
//        - The RefCount size depends on whether
//          MXD_ATOMIC_NATIVE_ENABLE_SUPPORT is enabled and the
//          support for 16 bit native atomic operations on the current
//          platform/architecture. See CAtomicOperations for more information.
//        - The minimal size for a new CString object is 7/9 bytes + the size of
//          the internal pointer.
//        - The maximal size for a CString object is uCSTRING_MAX_VALID_SIZE
//          bytes + the size of the internal pointer.
//        - The size for a CString copy (refCount scheme enabled) is the size of
//          the internal pointer.
//
//
//  Memory management:
//  The memory used to store the characters in a CString is allocated with
//  MX_NEW_ARRAY. By default, MX_NEW_ARRAY redirects the allocation to standard
//  new operator. If a more efficient memory allocation algorithm is required,
//  refer to the documentation related to MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT.
//
//  Note: The default capacity of a CString is 0 bytes. The CString is autogrow,
//  but to improve performance it is recommended to call ReserveCapacity with
//  the right capacity before use.
//
//  The following two sections describe the reference counting mechanism
//  available within the CString class.
//
//  #Synchronized Reference Counting:#
//  When MXD_STRING_DISABLE_REFCOUNT is NOT defined, the reference count scheme
//  is enabled.  Reference Counting is used when multiple CStrings can refer to
//  the same string.  For example, the two code lines below take advantage of
//  the RefCount scheme to save memory space.
//
//  CString strCopy1 = strInitial;
//  CString strCopy2(strInitial);
//
//  In this example, no new memory is allocated for strCopy1 and strCopy2,
//  except for the four bytes required for the pointer data member.  The final
//  RefCount value is three, so any of the three CString objects are aware that
//  they are sharing an internal buffer with other CString instances. The access
//  to the reference counting is synchronized with a single static mutex.  When
//  the Ref count is locked for a CString, no other CString can modify its
//  internal buffer.  This way, there is no problem when copying a string from
//  one thread to another. THIS DOES NOT MEAN THAT IT IS SAFE TO SHARE POINTERS
//  TO STRINGS BETWEEN THREADS! It only means that the reference counting scheme
//  will not cause any problems when a string is shared between two or more
//  threads by using the "=" operator or the copy constructor.
//
//  Note: The RefCount value is equal to "0" for an uninitialized CString
//        object.
//
//  #No Reference Counting:#
//  When MXD_STRING_DISABLE_REFCOUNT is defined, the reference counting scheme
//  is disabled. Any call to the copy constructor or the "=" operator (the two
//  places where the RefCount can be increased) allocates a memory storage for
//  the new CString object. Thus, the RefCount data member always has a value of
//  "0".
//
// See Also:
//  MXD_STRING_DISABLE_REFCOUNT, MXD_uCSTRING_BLOCK_LENGTH, CAtomicOperations
//
//==EDOC========================================================================
class CString
{
//M5T_INTERNAL_USE_BEGIN
    // Friends declarations
    //----------------------
    friend CMarshaler& operator<< ( IN CMarshaler& rMarshaler,
                                    IN const CString& rStr );

    friend CMarshaler& operator>> ( IN CMarshaler& rMarshaler,
                                    IN CString& rStr );

    friend CString operator+(IN const char* pszLhs, IN const CString& strRhs);
    friend CString operator+(IN const char c, IN const CString& strRhs);
//M5T_INTERNAL_USE_END

// Published Interface
//--------------------
public:

    // Summary:
    //  Default constructor.
    //---------------------------
    CString();

    // Summary:
    //  Constructor. Constructs CString from a char buffer.
    //----------------------------------------------------------
    CString(IN const char* pszRhs);

    // Summary:
    //  Constructor. Constructs CString from a char buffer of a specified length.
    //--------------------------------------------------------------------------
    CString(IN const char* pszRhs, IN unsigned int uSize);

    // Summary:
    //  Copy constructor.
    //---------------------------
    CString(IN const CString& rstrRhs);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CString();

    //  Summary:
    //   Checks whether or not two CString objects are identical.
    //------------------------------------------------------------
    bool operator==(IN const CString& rstrRhs) const;

    //  Summary:
    //   If a c-string object is identical with a c-style string.
    //------------------------------------------------------------
    bool operator==(IN const char* pszRhs) const;

    //  Summary:
    //   Checks whether or not two CString objects are different.
    //------------------------------------------------------------
    bool operator!=(IN const CString& rstrRhs) const;

    //  Summary:
    //   Checks whether or not a CString and a c-style string are different.
    //-----------------------------------------------------------------------
    bool operator!=(IN const char* pszRhs) const;

    //  Summary:
    //      Checks whether or not the left-hand CString is lower than the
    //      right-hand CString.
    //--------------------------------------------------------------------
    bool operator<(IN const CString& rstrRhs) const;

    //  Summary:
    //      Checks whether or not the left-hand CString is lower than the
    //      right-hand c-style string.
    //--------------------------------------------------------------------
    bool operator<(IN const char* pszRhs) const;

    //  Summary:
    //      Checks whether or not the left-hand CString is greater than the
    //      right-hand CString.
    //----------------------------------------------------------------------
    bool operator>(IN const CString& rstrRhs) const;

    //  Summary:
    //      Checks whether or not the left-hand CString is greater than the
    //      right-hand c-style string.
    //----------------------------------------------------------------------
    bool operator>(IN const char* pszRhs) const;

    //  Summary:
    //      Checks whether or not the left-hand CString is less than or equal
    //      to the right-hand CString.
    //------------------------------------------------------------------------
    bool operator<=(IN const CString& rstrRhs) const;

    //  Summary:
    //      Checks whether or not the left-hand CString is less than or equal
    //      to the right-hand c-style string.
    //------------------------------------------------------------------------
    bool operator<=(IN const char* pszRhs) const;

    //  Summary:
    //      Checks whether or not the left-hand CString is greater than or equal
    //      to the right-hand CString.
    //--------------------------------------------------------------------------
    bool operator>=(IN const CString& rstrRhs) const;

    //  Summary:
    //      Checks whether or not the left-hand CString is greater than or equal
    //      to the right-hand c-style string.
    //--------------------------------------------------------------------------
    bool operator>=(IN const char* pszRhs) const;

    //  Summary:
    //      Assigns a CString to the current CString object.
    //-------------------------------------------------------
    CString& operator= (IN const CString& rstrRhs);

    //  Summary:
    //      Assigns a c-style string to the current CString object.
    //--------------------------------------------------------------
    CString& operator= (IN const char* pszRhs);

    //  Summary:
    //      Assigns a character to the current CString object.
    //---------------------------------------------------------
    CString& operator= (IN const char c);

    //  Summary:
    //      Appends the CString's content to the end of the current CString.
    //-----------------------------------------------------------------------
    CString operator+(IN const CString& strRhs) const;

    //  Summary:
    //      Appends a c-style string's content to the end of the current
    //      CString.
    //------------------------------------------------------------------
    CString operator+(IN const char* pszRhs) const;

    //  Summary:
    //      Appends a character to the end of the current CString.
    //-------------------------------------------------------------
    CString operator+(IN const char c) const;

    //  Summary:
    //      Appends a CString to the end of the current CString.
    //-----------------------------------------------------------
    CString& operator+=(IN const CString& rstrRhs);

    //  Summary:
    //      Appends a character to the end of the current CString.
    //-------------------------------------------------------------
    CString& operator+=(IN const char* pszRhs);

    //  Summary:
    //      Appends a character to the end of the current CString.
    //-------------------------------------------------------------
    CString& operator+=(IN char c);

    //  Summary:
    //      Appends a c-style sub string to the end of the current CString.
    //---------------------------------------------------------------------
    mxt_result Append( IN const char* szSrc, IN unsigned int uSize);

    //  Summary:
    //      Case insensitive comparison between two CStrings.
    //--------------------------------------------------------
    int CaseInsCmp(IN const CString& rstrRhs) const;

    //  Summary:
    //      Case insensitive comparison between a c-style string and the current
    //      CString.
    //--------------------------------------------------------------------------
    int CaseInsCmp(IN const char* pszCmp) const;

    //  Summary:
    //      Converts to lowercase the contents of the current CString.
    //-----------------------------------------------------------------
    void ToLowerCase();

    //  Summary:
    //      Converts to uppercase the contents of the current CString.
    //-----------------------------------------------------------------
    void ToUpperCase();

    //  Summary:
    //      Direct Access operator (read only).
    //------------------------------------------
    const char& operator[](IN unsigned int uIndex) const;

    //  Summary:
    //      Gets one character from the current CString.
    //---------------------------------------------------
    const char& GetAt(IN unsigned int uIndex) const;

    //  Summary:
    //      Sets one character from the current CString.
    //---------------------------------------------------
    void SetAt(IN unsigned int uIndex, IN char c);

    //  Summary:
    //      Returns a pointer to CString's internal buffer.
    //------------------------------------------------------
    char* GetBuffer();

    //  Summary:
    //      Resizes the CString.
    //---------------------------
    void Resize(IN unsigned int uSize);

    //  Summary:
    //      Reduces the capacity of CString's internal buffer.
    //---------------------------------------------------------
    mxt_result ReduceCapacity(IN uint16_t uDownToCapacity);

    //  Summary:
    //      Augments the capacity of CString's internal buffer.
    //----------------------------------------------------------
    mxt_result ReserveCapacity(IN uint16_t uUpToCapacity);

    //  Summary:
    //      Returns the capacity of the current CString.
    //---------------------------------------------------
    uint16_t GetCapacity() const;

    //  Summary:
    //      Returns the size of the current CString.
    //-----------------------------------------------
    uint16_t GetSize() const;

    //  Summary:
    //      Checks whether or not CString's internal buffer is empty.
    //----------------------------------------------------------------
    bool IsEmpty() const;

    //  Summary:
    //      Inserts a CString into the current CString.
    //--------------------------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount,
                      IN const CString& rstrSrc,
                      IN unsigned int uSrcIndex,
                      IN unsigned int uSize);

    //  Summary:
    //      Inserts a c-style string into the current CString.
    //---------------------------------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount,
                      IN const char* pszRhs,
                      IN unsigned int uSize = uCOPY_ALL);

    //  Summary:
    //      Inserts a character into the current CString.
    //----------------------------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount,
                      IN char c);

    //  Summary:
    //      Erases portions of the current CString.
    //----------------------------------------------
    void Erase(IN unsigned int uIndex, IN unsigned int uSize);

    //  Summary:
    //      Erases one character from the current CString.
    //-----------------------------------------------------
    void Erase(IN unsigned int uIndex);

    //  Summary:
    //      Erases all the characters from the current CString object.
    //-----------------------------------------------------------------
    void EraseAll();

    //  Summary:
    //      Removes all the leading characters of the specified type.
    //----------------------------------------------------------------
    void TrimLeftSide(IN char c = ' ');

    //  Summary:
    //      Removes all the trailing characters of the specified type.
    //-----------------------------------------------------------------
    void TrimRightSide(IN char c = ' ');

    //  Summary:
    //      Removes all the leading and trailing characters of the specified
    //      type.
    //------------------------------------------------------------------------
    void TrimBothSide(IN char c = ' ');

    //  Summary:
    //      Finds the occurrence of a CString in the current CString.
    //----------------------------------------------------------------
    unsigned int FindSubstring (IN unsigned int uStartIndex,
                                IN const CString& rstrSubstring) const;

    //  Summary:
    //      Finds the occurrence of a c-style string in the current CString.
    //-----------------------------------------------------------------------
    unsigned int FindSubstring (IN unsigned int uStartIndex,
                                IN const char* pszSubstring) const;

    //  Summary:
    //      Returns a pointer to the internal C-style string.
    //--------------------------------------------------------
    const char* CStr() const;

    //  Summary:
    //      Formats arguments following pszFormat specifications.
    //------------------------------------------------------------
    mxt_result Format(IN unsigned int uIndex, IN const char *pszFormat, ...);

    //  Summary:
    //      Formats a variable argument list following pszFormat specifications.
    //---------------------------------------------------------------------------
    mxt_result FormatV(IN unsigned int uIndex, IN const char *pszFormat, va_list args);

//M5T_INTERNAL_USE_BEGIN
private:

    // Hidden Methods
    //--------------------

    uint16_t Release();

    uint16_t AddRef();

    void CreateCopy();

    const char* MakeNullSafe(IN const char* pszRhs) const;

    static mxt_result InitializeCString();
    static void FinalizeCString();

// Friends declarations
//----------------------
    friend mxt_result InitializeCString();
    friend void FinalizeCString();


#if defined(MXD_ATOMIC_NATIVE_ENABLE_SUPPORT) && \
    defined(MXD_OS_LINUX) && defined(MXD_COMPILER_GNU_GCC) && defined(MXD_ARCH_PPC)
    // On PCC architectures, only 32 bit native atomic operations are
    // supported. As such, to improve greatly the performance of CString
    // we use a 32 bit ref-count on the PCC architecture.
    typedef uint32_t RefCountType;
#else
    typedef uint16_t RefCountType;
#endif

    // Hidden Data Members
    //--------------------
    struct SStringInfo
    {
        // Number of CString objects referencing this particular string.
        RefCountType m_uRefCount;

        // Amount of memory (in bytes) allocated to store the string
        uint16_t m_uCapacity;

        // Actual length of the string (in characters)
        uint16_t m_uSize;

        // Internal string buffer (will at least contain a '\0')
#if defined(MXD_CSTRING_MINIMUM_BUFFER)
        // Static buffer. If the string is too big to fit in this buffer, we
        // fall back to dynamic allocation.
        char     m_pszString[uCSTRING_BLOCK_LENGTH + 1];
#else
        // This variable is used to identify the beginning of the allocated memory
        // that will hold the string. This one byte array is mandatory to
        // provide room for the string terminating character ('\0') which is
        // not handled by ReserveCapacity. For example, when
        // ReserveCapacity(uSize) is invoked and uSize is equal to
        // uCSTRING_BLOCK_LENGTH, the size of the allocated memory is uSize +
        // sizeof(SStringInfo). Consequently, the room for the '\0' character
        // must be handled by SStringInfo. At creation, this buffer is also
        // initialized to '\0' to represent an empty string.
        char     m_pszString[1];
#endif

#if defined(MXD_CSTRING_MINIMUM_BUFFER)
        SStringInfo() :
            m_uRefCount(0),
            m_uCapacity(uCSTRING_BLOCK_LENGTH),
            m_uSize(0)
        {
            m_pszString[0] = 0;
        }
#endif
    };
    // Pointer to internal buffer structure
    SStringInfo* m_pstStringInfo;

#ifndef MXD_STRING_DISABLE_REFCOUNT
    // Buffer that will be used as storage for the in-place new'ed static
    // instance of CAtomicOperations<RefCountType> object.
    static CAlignedVariableStorage<CAtomicOperations<RefCountType> > ms_atomicOps;
#endif

    // Default buffer shared by all non-initialized CString instances
#if defined(MXD_CSTRING_MINIMUM_BUFFER)
    SStringInfo m_stStringInfo;
#else
    static CAlignedVariableStorage<SStringInfo> ms_emptyStringInfo;
#endif
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==   operator!=
//==
//==============================================================================
//
//  Parameters:
//      rstrRhs: The CString object at the right-hand side of the comparison.
//
//  Returns:
//      True if the size OR content of the CString internal buffer differs,
//      false otherwise.
//
//  Description:
//      Verifies that the right-hand CString is different than the left-hand
//      CString. Only the internal string of both objects are compared
//      (the capacity and ref count values can be different).
//
//  Warning:
//      The comparison is case sensitive.
//
//==EDOC========================================================================
inline
bool CString::operator!=(IN const CString& rstrRhs) const
{
    return !(*this == rstrRhs);
}

//==SDOC====================================================================
//==
//==   operator!=
//==
//==========================================================================
//
//  Parameters:
//      pszRhs: The CString object at the right-hand side of the comparison.
//
//  Returns:
//      True if the size OR content of the current CString object differs
//      from "pszRhs", false otherwise.
//
//  Description:
//      Verifies that the right-hand c-style string is different than the
//      left-hand CString.
//
//  Warning:
//      The comparison is case sensitive.
//
//==EDOC====================================================================
inline
bool CString::operator!=(IN const char* pszRhs) const
{
    return !(*this == pszRhs);
}

//==SDOC========================================================================
//==
//==   operator<
//==
//==============================================================================
//
//  Parameters:
//      rstrRhs:  The CString object at the right-hand side of the comparison.
//
//  Returns:
//      True if the left-hand operand is lexically less than the right-hand
//      operand.
//
//  Description:
//      Verifies that a CString is less than another CString.
//      The comparison is case sensitive and based on the "strcmp" function.
//
//==EDOC========================================================================
inline
bool CString::operator<(IN const CString& rstrRhs) const
{
    return *this < rstrRhs.CStr();
}

//==SDOC========================================================================
//==
//==   operator<
//==
//==============================================================================
//
//  Parameters:
//      pszRhs:  The c-style string at the right-hand side of the comparison.
//
//  Returns:
//      True if the left-hand operand is lexically less than the right-hand
//      operand.
//
//  Description:
//      Verifies that a CString is less than a c-style string.
//      The comparison is case sensitive and based on the "strcmp" function.
//
//==EDOC========================================================================
inline
bool CString::operator<(IN const char* pszRhs) const
{
    return strcmp(CStr(), MakeNullSafe(pszRhs)) < 0;
}

//==SDOC========================================================================
//==
//==   operator>
//==
//==============================================================================
//
//  Parameters:
//      rstrRhs:  The CString object at the right-hand side of the comparison.
//
//  Returns:
//      True if the left-hand operand is lexically greater than the right-hand
//      operand.
//
//  Description:
//      Verifies that a CString is greater than another CString.
//      Comparison is case sensitive and based on the "strcmp" function.
//
//==EDOC========================================================================
inline
bool CString::operator>(IN const CString& rstrRhs) const
{
    return *this > rstrRhs.CStr();
}

//==SDOC========================================================================
//==
//==   operator>
//==
//==============================================================================
//
//  Parameters:
//      pszRhs:  The c-style string at the right-hand side of the comparison.
//
//  Returns:
//      True if the left-hand operand is lexically greater than the right-hand
//      operand.
//
//  Description:
//      Verifies that a CString is higher than a c-style string.
//      The comparison is case sensitive and based on the "strcmp" function.
//
//==EDOC========================================================================
inline
bool CString::operator>(IN const char* pszRhs) const
{
    return (!(*this < pszRhs) && (*this != pszRhs));
}

//==SDOC========================================================================
//==
//==   operator<=
//==
//==============================================================================
//
//  Parameters:
//      rstrRhs:  The CString object at the right-hand side of the comparison.
//
//  Returns:
//      True if the left-hand operand is lexically less than or equal to the
//      right-hand operand.
//
//  Description:
//      Verifies that a CString is less than or equal to another CString.
//      The comparison is case sensitive and based on the "strcmp" function.
//
//==EDOC========================================================================
inline
bool CString::operator<=(IN const CString& rstrRhs) const
{
    return *this <= rstrRhs.CStr();
}

//==SDOC========================================================================
//==
//==   operator<=
//==
//==============================================================================
//
//  Parameters:
//      pszRhs:  The c-style string at the right-hand side of the comparison.
//
//  Returns:
//      True if the left-hand operand is lexically less than or equal to the
//      right-hand operand.
//
//  Description:
//      Verifies that a CString is less than or equal to a c-style string.
//      The comparison is case sensitive and based on the "strcmp" function.
//
//==EDOC========================================================================
inline
bool CString::operator<=(IN const char* pszRhs) const
{
    return !(*this > pszRhs);
}

//==SDOC========================================================================
//==
//==   operator>=
//==
//==============================================================================
//
//  Parameters:
//      rstrRhs:  The CString object at the right-hand side of the comparison.
//
//  Returns:
//      True if the left-hand operand is lexically greater than or equal to the
//      right-hand operand.
//
//  Description:
//      Verifies that a CString is greater than or equal to another CString.
//      The comparison is case sensitive and based on the "strcmp" function.
//
//==EDOC========================================================================
inline
bool CString::operator>=(IN const CString& rstrRhs) const
{
    return *this >= rstrRhs.CStr();
}

//==SDOC========================================================================
//==
//==   operator>=
//==
//==============================================================================
//
//  Parameters:
//      pszRhs:  The c-style string at the right-hand side of the comparison.
//
//  Returns:
//      True if the left-hand operand is lexically greater than or equal to the
//      right-hand operand.
//
//  Description:
//      Verifies that a CString is greater than or equal to a c-style string.
//      The comparison is case sensitive and based on the "strcmp" function.
//
//==EDOC========================================================================
inline
bool CString::operator>=(IN const char* pszRhs) const
{
    return !(*this < pszRhs);
}

//==SDOC========================================================================
//==
//==   operator+=
//==
//==============================================================================
//
//  Parameters:
//      rstrRhs: CString to append.
//
//  Returns:
//      A reference to this CString instance.
//
//  Description:
//      Appends the right-hand CString to the end of the left-hand CString.
//      A reference to the modified CString object is returned.
//
//==EDOC========================================================================
inline
CString& CString::operator+=(IN const CString& rstrRhs)
{
    Insert(GetSize(), 1, rstrRhs.CStr(), rstrRhs.GetSize());
    return *this;
}

//==SDOC========================================================================
//==
//==   operator+=
//==
//==============================================================================
//
//  Parameters:
//      pszRhs: The c-style string to append.
//
//  Returns:
//      A reference to this CString instance.
//
//  Description:
//      Appends the right-hand c-style string to the end of the left-hand
//      CString. A reference to the modified CString object is returned.
//
//==EDOC========================================================================
inline
CString& CString::operator+=(IN const char* pszRhs)
{
    Insert(GetSize(), 1, pszRhs, uCOPY_ALL);
    return *this;
}

//==SDOC========================================================================
//==
//==   operator+=
//==
//==============================================================================
//
//  Parameters:
//      c: character to append.
//
//  Returns:
//      A reference to this CString instance.
//
//  Description:
//      Appends the right-hand character to the end of the left-hand CString. A
//      reference to the modified CString object is returned.
//
//==EDOC========================================================================
inline
CString& CString::operator+=(IN char c)
{
    Insert(GetSize(), 1, c);
    return *this;
}

//==SDOC========================================================================
//==
//==   Append
//==
//==============================================================================
//
//  Parameters:
//      szSrc: The c-style string to append.
//
//      uSize: Number of characters to append.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_ARGUMENT
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Appends "uSize" characters from the "szSrc" string at the end of the
//      current CString.
//
//==EDOC========================================================================
inline
mxt_result CString::Append(IN const char* szSrc, IN unsigned int uSize)
{
    return Insert(GetSize(),1,szSrc,uSize);
}

//==SDOC========================================================================
//==
//==  CaseInsCmp
//==
//==============================================================================
//
//  Parameters:
//      rstrRhs: Reference to the CString instance with which to compare.
//
//  Returns:
//      * 0     if equal
//      * < 0   if the current instance or CString is "smaller/shorter" than
//              rstrCmp
//      * > 0   if the current instance or CString is "greater/longer" than
//              rstrCmp
//
//  Description:
//      These methods are used to compare CString's internal buffer with
//      another CString or a c-style string.  The return values are based
//      on the "strcmp" function.
//
//==EDOC========================================================================
inline
int CString::CaseInsCmp(IN const CString& rstrRhs) const
{
    return CaseInsCmp(rstrRhs.CStr());
}

//==SDOC========================================================================
//==
//==   operator[]
//==
//==============================================================================
//
//  Parameters:
//      uIndex: The index of the character to access in the string.
//
//  Returns:
//      A constant reference to a constant character.
//
//  Description:
//      Returns a reference to the character located at the index "uIndex"
//      of the CString. The character returned is not modifiable, instead
//      use the SetAtmethod to modify a single character from the current
//      CString.
//
//  Warning:
//      In the previous implementation of the CString class, the operator[]
//      returned a modifiable character reference.
//
//==EDOC========================================================================
inline
const char& CString::operator[](IN unsigned int uIndex) const
{
    return GetAt(uIndex);
}

//==SDOC========================================================================
//==
//==   GetAt
//==
//==============================================================================
//
//  Parameters:
//      uIndex: The index of the character to access in the string.
//              The value must be: 0 <= uIndex <= GetSize.
//
//  Returns:
//      A constant reference to a constant character.
//
//  Description:
//      Returns a reference to the character located at the index "uIndex" of
//      the CString.
//
//==EDOC========================================================================
inline
const char& CString::GetAt(IN unsigned int uIndex) const
{
    MX_ASSERT(uIndex <= GetSize());
    return m_pstStringInfo->m_pszString[uIndex];
}

//==SDOC========================================================================
//==
//==   GetCapacity
//==
//==============================================================================
//
//  Returns:
//      The memory currently allocated (in bytes) to store the string.
//
//  Description:
//      The capacity of the current Cstring's internal buffer is returned. If
//      the current CString is uninitialized, GetCapacity returns "0".
//
//==EDOC========================================================================
inline
uint16_t CString::GetCapacity() const
{
    return m_pstStringInfo->m_uCapacity;
}

//==SDOC========================================================================
//==
//==   GetSize
//==
//==============================================================================
//
//  Returns:
//      The size of the current CString internal string (number of characters).
//
//  Description:
//      The size of the current CString's internal string is returned. If the
//      current CString is uninitialized, GetSize returns "0".
//
//==EDOC========================================================================
inline
uint16_t CString::GetSize() const
{
    return m_pstStringInfo->m_uSize;
}

//==SDOC========================================================================
//==
//==   IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True if current CString object is uninitialized or has a size of 0,
//      false otherwise.
//
//  Description:
//      Checks whether or not the internal string from the current CString
//      object has a size equal to "0".
//
//==EDOC========================================================================
inline
bool CString::IsEmpty() const
{
    return m_pstStringInfo->m_uSize == 0;
}

//==SDOC========================================================================
//==
//==   Insert
//==
//==============================================================================
//
//  Parameters:
//      uIndex: The index of the character where the insertion takes place.
//
//      uCount: The number of characters to insert.
//
//      c:  The character to insert.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_ARGUMENT
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts "uCount" times the character "c" at the index "uIndex" of the
//      current CString.
//
//==EDOC========================================================================
inline
mxt_result CString::Insert(IN unsigned int uIndex,
                           IN unsigned int uCount,
                           IN char c)
{
    return Insert(uIndex, uCount, &c, 1);
}

//==SDOC========================================================================
//==
//==   Erase
//==
//==============================================================================
//
//  Parameters:
//      uIndex: The index of the character to be erased.
//
//  Description:
//      Erases one character from the current CString object at the index
//      "uIndex". The size value is automatically modified.  The capacity is
//      unchanged.
//
//==EDOC========================================================================
inline
void CString::Erase(IN unsigned int uIndex)
{
    Erase(uIndex, 1);
}

//==SDOC========================================================================
//==
//==   EraseAll
//==
//==============================================================================
//
//  Description:
//      A '\0' character is put at the index 0 in CString's internal buffer.
//      The size is set to "0" and the capacity is unchanged.
//
//==EDOC========================================================================
inline
void CString::EraseAll()
{
    Erase(0, GetSize());
}

//==SDOC====================================================================
//==
//==   TrimBothSide
//==
//==========================================================================
//
//  Parameters:
//      c: The type of character to trim.
//
//  Description:
//      Removes all the leading AND trailing "c" characters from the string.
//      (default is " ").  The size value is automatically modified.
//
//==EDOC====================================================================
inline
void CString::TrimBothSide(IN char c)
{
    TrimRightSide(c);
    TrimLeftSide(c);
}

//==SDOC========================================================================
//==
//==  FindSubstring
//==
//==============================================================================
//
//  Parameters:
//      uStartIndex: The index from the current CString where the search begins.
//
//      rstrSubstring: The sub string to find.
//
//  Returns:
//      The index of the first occurrence if it is found or the size of the
//      string if it is not found.
//
//  Description:
//      Starting at the index "uStartIndex", this method finds the first
//      occurrence of "strSubstring" in the current CString and returns the
//      first index where a match occurred. The comparison is case sensitive.
//
//==EDOC========================================================================
inline
unsigned int CString::FindSubstring (IN unsigned int uStartIndex,
                                     IN const CString& rstrSubstring) const
{
    return FindSubstring(uStartIndex, rstrSubstring.CStr());
}

//==SDOC========================================================================
//==
//==   CStr
//==
//==============================================================================
//
//  Returns:
//      A const pointer to the beginning of CString's internal buffer.
//
//  Description:
//      Returns a pointer to a non modifiable C-style string.
//
//  Warning:
//      -Further calls to any method affecting CString's internal buffer may
//       cause a memory reallocation. When this reallocation occurs, any pointer
//       returned by a preceding CStr call becomes invalid.
//
//==EDOC========================================================================
inline
const char* CString::CStr() const
{
    return m_pstStringInfo->m_pszString;
}


//==SDOC========================================================================
//==
//==   MakeNullSafe
//==
//==============================================================================
//
//  Parameters:
//      pszRhs: A c-style string (possibly NULL) used as the return value.
//
//  Returns:
//      A pointer to a valid c-style string.
//
//  Description:
//      This method is used where support must be granted for NULL c-style
//      strings. When pszRhs is NULL, an empty string (i.e "") is returned.
//      Otherwise, pszRhs is returned unchanged.
//
//==EDOC========================================================================
inline
const char* CString::MakeNullSafe(IN const char* pszRhs) const
{
    return (pszRhs == NULL ? g_szEMPTY_STRING : pszRhs);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif //MXG_CSTRING_H


