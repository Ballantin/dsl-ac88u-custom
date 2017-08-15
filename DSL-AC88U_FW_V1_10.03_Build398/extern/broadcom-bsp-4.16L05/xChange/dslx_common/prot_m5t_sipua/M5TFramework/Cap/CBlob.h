//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CBLOB_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CBLOB_H
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
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

// Interface Realized & Parent
#ifndef MXG_CVECTORBASE_H
#include "Cap/CVectorBase.h"
#endif

// Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAllocator;
class CMarshaler;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN

// Macro used to copy all of the blob's data instead of using getsize methods.
#define uBLOB_COPY_ALL (~0U)

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CBlob
//==============================================================================
//<GROUP CAP_CLASSES>
//
//  Summary:
//      This class is defined as a Binary Large OBject (Blob).
//
//  Description:
//      This class is defined as a Binary Large OBject (Blob). The Blob holds an
//      array of bytes and offers some operations to manipulate this buffer
//      safely. The security of the blob can be bypassed for performance
//      purposes. To do so, the client can call GetFirstIndexPtr or
//      GetEndIndexPtr and use the pointer to the array of bytes directly.
//
//      The blob inherits all of CVectorBase's capabilities where the size of
//      each element is one byte. The blob totally relies on CVectorBase for all
//      standard vector operations.
//
//      The index in the array of bytes is always zero-based.
//
//  Location:
//      Cap/CBlob.h
//
//==============================================================================
class CBlob : protected CVectorBase
{
// Friend Declaration
//--------------------
    //==SDOC====================================================================
    //==
    //==   operator<<
    //==
    //==========================================================================
    //
    //  Summary:
    //      Marshaling input operator.
    //
    //  Parameters:
    //      rMarshaler:
    //          CMarshaler object into which CBlob is inserted.
    //
    //      rBlob:
    //          Left-hand CBlob to be inserted.
    //
    //  Returns:
    //      Reference to the CMarshaler object.
    //
    //  Description:
    //      Inserts the CBlob "rBlob" into the CMarshaler object "rMarshaler".
    //
    //==EDOC====================================================================
    friend CMarshaler& operator<< (IN CMarshaler& rMarshaler,
                                   IN const CBlob& rBlob);


    //==SDOC====================================================================
    //==
    //==   operator>>
    //==
    //==========================================================================
    //
    //  Summary:
    //      Marshaling output operator.
    //
    //  Parameters:
    //      rMarshaler:
    //          CMarshaler object containing a blob.
    //
    //      rBlob:
    //          CBlob object into which the blob from CMarshaler is inserted.
    //
    //  Returns:
    //      Reference to the CMarshaler object.
    //
    //  Description:
    //      Extracts the blob from the CMarshaler object "rMarshaler" and
    //      inserts it into the CBlob "rBlob".
    //
    //==EDOC====================================================================
    friend CMarshaler& operator>> (IN CMarshaler& rMarshaler,
                                   IN CBlob& rBlob);

// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------------

    // Summary:
    //     Default constructor.
    //---------------------------
    CBlob(IN IAllocator* pAllocator = NULL);

    // Summary:
    //     Constructor. Allocates capacity.
    CBlob(IN unsigned int uCapacity,
          IN IAllocator* pAllocator = NULL);

    // Summary:
    //     Constructor. Allocates capacity and inserts data.
    CBlob(IN const uint8_t* puData,
          IN unsigned int uDataSize,
          IN unsigned int uCapacity,
          IN IAllocator* pAllocator = NULL);

    // Summary:
    //     Copy constructor.
    //------------------------
    CBlob(IN const CBlob& rBlob);

    // Summary:
    //     Destructor.
    //------------------
    virtual ~CBlob();

    // << Operators >>
    //----------------------------------------

    // Summary:
    //     Assignment operator.
    //---------------------------
    CBlob& operator=(IN const CBlob& rBlob);

    // Summary:
    //     Comparison operator.
    //---------------------------
    bool operator==(IN const CBlob& rBlob) const;

    // Summary:
    //     Different than operator.
    //-------------------------------
    bool operator!=(IN const CBlob& rBlob) const;

    // Summary:
    //     Less than operator.
    //--------------------------
    bool operator<(IN const CBlob& rBlob) const;

    // Summary:
    //     Greater than operator.
    //-----------------------------
    bool operator>(IN const CBlob& rBlob) const;

    // Summary:
    //     Less than or equal to operator.
    //--------------------------------------
    bool operator<=(IN const CBlob& rBlob) const;

    // Summary:
    //     Greater than or equal to operator.
    //-----------------------------------------
    bool operator>=(IN const CBlob& rBlob) const;

    //  Summary:
    //      Returns a pointer to the byte at index zero.
    //---------------------------------------------------
    uint8_t* GetFirstIndexPtr();

    // <COMBINE GetFirstIndexPtr>
    const uint8_t* GetFirstIndexPtr() const;

    //  Summary:
    //      Returns a pointer to the byte at the end index.
    //------------------------------------------------------
    uint8_t* GetEndIndexPtr();

    // <COMBINE GetEndIndexPtr>
    const uint8_t* GetEndIndexPtr() const;

    //  Summary:
    //      Inserts bytes at the specified index.
    //--------------------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN const uint8_t* puData,
                      IN unsigned int uDataSize);

    //  Summary:
    //      Appends bytes at the end index.
    //--------------------------------------
    mxt_result Append(IN const char* pszString);

    //  Summary:
    //      Appends a byte at the end index.
    //---------------------------------------
    mxt_result Append(IN uint8_t uByte);

    //  Summary:
    //      Appends bytes at the end index.
    //--------------------------------------
    mxt_result Append(IN const uint8_t* puData,
                      IN unsigned int uSize);

    //  Summary:
    //      Appends the content of another blob at the end index.
    //------------------------------------------------------------
    mxt_result Append(IN const CBlob& rBlob,
                      IN unsigned int uSrcBlobIndex = 0,
                      IN unsigned int uSize = uBLOB_COPY_ALL);

    //  Summary:
    //      Reads bytes starting at the current read index.
    //------------------------------------------------------
    uint8_t* Read(OUT uint8_t* pReadDest, IN unsigned int uSize );

    //  Summary:
    //      Reads bytes starting at the current read index.
    //------------------------------------------------------
    const uint8_t* ReadNoCopy(IN unsigned int uSize );

    //  Summary:
    //      Gets the remaining number of bytes to read from the blob.
    //----------------------------------------------------------------
    unsigned int GetUnreadSize();

    //  Summary:
    //      Sets the current read index.
    //-----------------------------------
    mxt_result SetReadIndex(IN unsigned int uIndex);

    // << Bits operations >>
    // ----------------------------

    //  Summary:
    //      Appends bits at the end of the blob.
    //-------------------------------------------
    void AppendBits(IN const uint8_t* puData,
                    IN unsigned int uSizeInBits,
                    IN unsigned int uStartingBit = 0);

    //  Summary:
    //      Reads bits at the current read index.
    //--------------------------------------------
    void ReadBits(OUT uint8_t* puReadDest,
                  IN unsigned int uSizeInBits,
                  IN unsigned int uStartingBit = 0);

    //  Summary:
    //      Pads with cleared ('0') bits at the end of the blob.
    //-----------------------------------------------------------
    void PadBits(IN unsigned int uSizeInBits);

    //  Summary:
    //      Skips bits before reading.
    //---------------------------------
    void SkipBits(IN unsigned int uSizeInBits);

    //  Summary:
    //      Increments (if needed) the read index so that it is a multiple of
    //      the given number of bits.
    //------------------------------------------------------------------------
    void RealignReadBits(IN unsigned int uBitsMultiple = 8);

    //  Summary:
    //      Pads the needed number of bits at the end of the blob so that it is
    //      realigned on a multiple of the given number of bits.
    //--------------------------------------------------------------------------
    void RealignAppendBits(IN unsigned int uBitsMultiple = 8);

    //  Summary:
    //      Gets the number of unread bits.
    //--------------------------------------
    unsigned int GetUnreadBits();

    // ===============================================
    // << CVectorBase class operations >>
    // ===============================================

    // Capacity (Max element count).
    //-------------------------------

    //  Summary:
    //      Returns the capacity.
    //----------------------------
    unsigned int GetCapacity() const;

    //  Summary:
    //      Returns the lock capacity count.
    //---------------------------------------
    unsigned int GetLockCapacity() const;

    //  Summary:
    //      Locks the capacity.
    //--------------------------
    void LockCapacity();

    //  Summary:
    //      Reduces the capacity.
    //----------------------------
    mxt_result ReduceCapacity(IN unsigned int uDownToCapacity);

    //  Summary:
    //      Increases the capacity.
    //------------------------------
    mxt_result ReserveCapacity(IN unsigned int uUpToCapacity);

    //  Summary:
    //      Unlocks the capacity.
    //----------------------------
    void UnlockCapacity();

    // Size (Element count).
    //-----------------------

    // Summary:
    //     Gets the size.
    //---------------------
    unsigned int GetSize() const;

    // Summary:
    //     Checks if the blob is empty.
    //----------------------------------
    bool IsEmpty() const;

    // Summary:
    //     Checks if the blob is full.
    //---------------------------------
    bool IsFull() const;

    // Summary:
    //     Resizes the blob.
    //------------------------
    mxt_result Resize(IN unsigned int uSize);

    // Summary:
    //     Resizes the blob.
    //------------------------
    mxt_result Resize(IN uint8_t* pEnd);

    // Index operations.
    //-------------------

    // Summary:
    //     Returns the index of the first used byte.
    //------------------------------------------------------------
    unsigned int GetFirstIndex() const;

    // Summary:
    //     Returns the index of the last used byte.
    //-----------------------------------------------
    unsigned int GetLastIndex() const;

    // Summary:
    //      Returns the index of the first unused byte.
    //--------------------------------------------------
    unsigned int GetEndIndex() const;

    // Direct element access.
    //------------------------

    // Summary:
    //      Returns a reference to the byte at uIndex.
    //-------------------------------------------------
    const uint8_t& operator[](IN unsigned int uIndex) const;

    //<COMBINE CBlob::[]@IN unsigned int@const>
    uint8_t& operator[](IN unsigned int uIndex);

    // Summary:
    //      Returns a reference to the byte at uIndex.
    //-------------------------------------------------
    uint8_t& GetAt(IN unsigned int uIndex);

    //<COMBINE CBlob::GetAt@IN unsigned int>
    const uint8_t& GetAt(IN unsigned int uIndex) const;


    // List operations.
    //------------------

    //  Summary:
    //      Inserts one or more bytes.
    //---------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount);

    //  Summary:
    //      Inserts one or more bytes.
    //---------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount,
                      IN uint8_t uValue );

    //  Summary:
    //      Inserts the bytes contained in another blob.
    //---------------------------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN const CBlob& rBlob,
                      IN unsigned int uSrcBlobIndex = 0,
                      IN unsigned int uSize = uBLOB_COPY_ALL);

    // Summary:
    //      Erases the byte at uIndex from the blob.
    //-----------------------------------------------
    void Erase(IN unsigned int uIndex);

    // Summary:
    //      Erases uCount bytes at uIndex from the blob.
    //---------------------------------------------------
    void Erase(IN unsigned int uIndex, IN unsigned int uCount);

    // Summary:
    //      Erases all the bytes from the blob.
    //------------------------------------------
    void EraseAll();

    // Advanced operations.
    //----------------------

    //  Summary:
    //      Moves all the byte of another blob to a specific index.
    //--------------------------------------------------------------
    mxt_result Merge(IN unsigned int uIndex, INOUT CBlob& rBlob);

    //  Summary:
    //      Moves some bytes from this blob to another starting from a
    //      specific index.
    //-----------------------------------------------------------------
    mxt_result Split(IN unsigned int uIndex, OUT CBlob& rBlob);

    // Summary:
    //      This method exchanges the position of two bytes.
    //-------------------------------------------------------
    void Swap(IN unsigned int uFirstIndex, IN unsigned int uSecondIndex);

//M5T_INTERNAL_BEGIN

// Hidden Methods
//---------------------
private:
    void CopyMembers(IN const CBlob& rBlob);

// Hidden Data Members
//---------------------
private:

    // Index to the next byte available for reading.
    // (Used with bytes and bits)
    unsigned int m_uReadIndex;

    // # bits already read in the byte pointed to by m_uReadIndex-1.
    // (Used with bits only)
    unsigned int m_uBitsRead;

    // Index to the byte where bits are inserted.
    // (Used with bits only)
    unsigned int m_uWriteIndex;

    // # bits written in the byte pointed to by m_uWriteIndex.
    // (Used with bits only)
    unsigned int m_uBitsWritten;

//M5T_INTERNAL_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      A reference to the source blob.
//
//  Returns:
//      True if the two blobs are equal, false otherwise
//
//  Description:
//      Verifies whether or not the left hand blob is equal to the right hand
//      blob. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
inline
bool CBlob::operator==(IN const CBlob& rBlob) const
{
    return CVectorBase::operator==(rBlob);
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      A reference to the source blob.
//
//  Returns:
//      True if the two blobs are different, false otherwise
//
//  Description:
//      Verifies whether or not the left hand blob is equal to the right hand
//      blob. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
inline
bool CBlob::operator!=(IN const CBlob& rBlob) const
{
    return CVectorBase::operator!=(rBlob);
}

//==SDOC========================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      A reference to the source blob.
//
//  Returns:
//      True if the left hand blob is less than the right hand blob, false
//      otherwise
//
//  Description:
//      Verifies that the left hand blob is less than the right hand blob. This
//      is done by comparing containers element by element and returns once an
//      element is different from the other. This behaviour is identical to the
//      one used in strcmp.
//
//==EDOC========================================================================
inline
bool CBlob::operator<(IN const CBlob& rBlob) const
{
    return CVectorBase::operator<(rBlob);
}

//==SDOC========================================================================
//==
//==  operator>
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      A reference to the source blob.
//
//  Returns:
//      True if the left hand blob is greater than the right hand blob, false
//      otherwise
//
//  Description:
//      Verifies that the left hand blob is greater than the right hand blob.
//      This is done by comparing containers element by element and returns once
//      an element is different from the other. This behaviour is identical to
//      the one used in strcmp.
//
//==EDOC========================================================================
inline
bool CBlob::operator>(IN const CBlob& rBlob) const
{
    return (!(*this < rBlob) && *this != rBlob);
}

//==SDOC========================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      A reference to the source blob.
//
//  Returns:
//      True if the left hand blob is less than or equal to the right hand blob,
//      false otherwise
//
//  Description:
//      Verifies that the left hand blob is less than or equal to the right hand
//      blob. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour
//      is identical to the one used in strcmp.
//
//==EDOC========================================================================
inline
bool CBlob::operator<=(IN const CBlob& rBlob) const
{
    return !(*this > rBlob);
}

//==SDOC========================================================================
//==
//==  operator>=
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      A reference to the source blob.
//
//  Returns:
//      True if the left hand blob is greater than or equal to the right hand
//      blob, false otherwise
//
//  Description:
//      Verifies that the left hand blob is greater than or equal to the right
//      hand blob. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
inline
bool CBlob::operator>=(IN const CBlob& rBlob) const
{
    return !(*this < rBlob);
}

//==SDOC========================================================================
//==
//==  GetFirstIndexPtr
//==
//==============================================================================
//
//  Returns:
//      NULL if the capacity is 0, a pointer to the byte at index zero
//      otherwise.
//
//  Description:
//      This method returns a pointer to the byte at index zero in the blob's
//      internal buffer. This method is used when the client wants to modify,
//      insert, or remove data manually.
//
//  Warning:
//      - Never delete a pointer acquired through GetFirstIndexPtr. The
//        blob is always the owner of the buffer.
//
//      - If bytes of data are inserted or removed using this approach, care
//        must be taken to avoid buffer overflow.
//
//      - Avoid keeping the pointer returned by this method. Any operation on
//        the blob that can lead to memory allocation makes such a pointer
//        obsolete.
//
//  See also: GetEndIndexPtr, Resize, GetSize, GetCapacity
//
//==EDOC========================================================================
inline
uint8_t* CBlob::GetFirstIndexPtr()
{
    return reinterpret_cast<uint8_t*>(GetAtInCapacity(GetFirstIndex()));
}

inline
const uint8_t* CBlob::GetFirstIndexPtr() const
{
    return reinterpret_cast<const uint8_t*>(GetAtInCapacity(GetFirstIndex()));
}

//==SDOC========================================================================
//==
//==  GetEndIndexPtr
//==
//==============================================================================
//
//  Returns:
//      NULL if the size equals the capacity, a pointer to the byte at end
//      index otherwise.
//
//  Description:
//      This method returns a pointer to the byte at end index in the blob's
//      internal buffer. The memory location returned is one past the last valid
//      byte in the buffer. This method is used when the client wants to modify,
//      insert, or remove data manually.
//
//  Warning:
//      - Never delete a pointer acquired through GetEndIndexPtr. The blob is
//        always the owner of the buffer.
//
//      - If bytes of data are inserted or removed using this approach, care
//        must be taken to avoid buffer overflow.
//
//      - Avoid keeping the pointer returned by this method. Any operation on
//        the blob that can lead to memory allocation makes such a pointer
//        obsolete.
//
//  See also: GetFirstIndexPtr, Resize, GetSize, GetCapacity
//
//==EDOC========================================================================
inline
uint8_t* CBlob::GetEndIndexPtr()
{
    return reinterpret_cast<uint8_t*>(GetAtInCapacity(GetEndIndex()));
}

inline
const uint8_t* CBlob::GetEndIndexPtr() const
{
    return reinterpret_cast<const uint8_t*>(GetAtInCapacity(GetEndIndex()));
}

//==SDOC========================================================================
//==
//==  Append
//==
//==============================================================================
//
//  Parameters:
//    pszString:
//      A pointer to the c-style string buffer to insert.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_ARGUMENT
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method appends a c-style string AS BYTES at the end index. The
//      string to be inserted is pointed by pszString. The terminating '\0'
//      character is not inserted.
//
//      The parameter pszString MUST be null-terminated, otherwise the result is
//      unpredictable. The method uses strlen to get the number of bytes to
//      append.
//
//  See also: Insert
//
//==EDOC========================================================================
inline
mxt_result CBlob::Append(IN const char* pszString)
{
    return Append(reinterpret_cast<const uint8_t*>(pszString), strlen(pszString));
}

//==SDOC========================================================================
//==
//==  Append
//==
//==============================================================================
//
//  Parameters:
//    uByte:
//      Byte to append to the buffer.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method appends a single byte at the end index.
//
//  See also:
//      Insert
//
//==EDOC========================================================================
inline
mxt_result CBlob::Append(IN uint8_t uByte)
{
    return Append(&uByte, 1);
}

//==SDOC========================================================================
//==
//==  Append
//==
//==============================================================================
//
//  Parameters:
//    puData:
//      A pointer to the bytes to insert.
//
//    uSize:
//      The number of bytes to insert.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_ARGUMENT
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method appends uSize bytes of data at the end index. The bytes
//      to be inserted are pointed by puData.
//
//  See also: Insert
//
//==EDOC========================================================================
inline
mxt_result CBlob::Append(IN const uint8_t* puData,
                         IN unsigned int uSize )
{
    return Insert(GetEndIndex(), puData, uSize );
}

//==SDOC========================================================================
//==
//==  Append
//==
//==============================================================================
//
//  Parameters:
//    rBlob:
//      A reference to the blob to be appended.
//
//    uSrcBlobIndex:
//      Index of the first byte to append from rBlob ( default is 0 ).
//
//    uSize:
//      Size in bytes to append from rBlob ( default is entire blob ).
//
//  Returns:
//      resS_OK
//      resFE_INVALID_ARGUMENT
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method partially or fully appends the content of rBlob to the
//      current blob. When uSrcBlobIndex and uSize are left to the default
//      value, the content of rBlob is completely appended. When
//      uSrcBlobIndex and uSize are specified, uSize bytes of rBlob starting
//      at uSrcBlobIndex are appended.
//
//  See also: Insert
//
//==EDOC========================================================================
inline
mxt_result CBlob::Append(IN const CBlob& rBlob,
                         IN unsigned int uSrcBlobIndex,
                         IN unsigned int uSize)
{
    return Insert(GetEndIndex(), rBlob, uSrcBlobIndex, uSize);
}

//==SDOC========================================================================
//==
//==  GetUnreadSize
//==
//==============================================================================
//
//  Returns:
//      The remaining number of bytes to read.
//
//  Description:
//      This method simply returns the remaining number of bytes to read from
//      the blob.
//
//  See also: Read, ReadNoCopy
//
//==EDOC========================================================================
inline
unsigned int CBlob::GetUnreadSize()
{
    return (GetEndIndex() - m_uReadIndex);
}


//==============================================================================
//====  CVectorBase operations =================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetCapacity
//==
//==============================================================================
//
//  Returns:
//      The capacity.
//
//  Description:
//      This method returns the capacity of the blob. The capacity is the
//      number of bytes allocated that are available for use.
//
//==EDOC========================================================================
inline
unsigned int CBlob::GetCapacity() const
{
    return CVectorBase::GetCapacity();
}

//==SDOC========================================================================
//==
//==  GetLockCapacity
//==
//==============================================================================
//
//  Returns:
//      The lock capacity count.
//
//  Description:
//      This method returns the lock capacity count. The lock capacity count
//      is a counter that is increased each time LockCapacity is called and
//      decreased each time UnlockCapacity is called. If the lock capacity
//      count is greater than zero, this means that any call to ReduceCapacity
//      or ReserveCapacity fails.
//
//==EDOC========================================================================
inline
unsigned int CBlob::GetLockCapacity() const
{
    return CVectorBase::GetLockCapacity();
}

//==SDOC========================================================================
//==
//==  LockCapacity
//==
//==============================================================================
//
//  Description:
//      This method locks the capacity. The lock capacity count is a counter
//      that is increased each time LockCapacity is called and decreased each
//      time UnlockCapacity is called. If the lock capacity count is greater
//      than zero, this means that any call to ReduceCapacity or ReserveCapacity
//      fails.
//
//==EDOC========================================================================
inline
void CBlob::LockCapacity()
{
    CVectorBase::LockCapacity();
}

//==SDOC========================================================================
//==
//==  ReduceCapacity
//==
//==============================================================================
//
//  Parameters:
//    uDownToCapacity:
//      The wanted capacity.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_STATE
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method reduces the capacity of the blob. The capacity is reduced to
//      uDownToCapacity only if uDownToCapacity is below the current capacity.
//
//      The method fails if the lock capacity count is not 0.
//
//==EDOC========================================================================
inline
mxt_result CBlob::ReduceCapacity(IN unsigned int uDownToCapacity)
{
    return CVectorBase::ReduceCapacity(uDownToCapacity);
}

//==SDOC========================================================================
//==
//==  ReserveCapacity
//==
//==============================================================================
//
//  Parameters:
//    uUpToCapacity:
//      The wanted capacity.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_STATE
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method increases the capacity of the blob. The capacity is
//      increased to uUpToCapacity only if uUpToCapacity is greater than the
//      current capacity. The method fails if the lock capacity count is not 0
//      and the wanted capacity is greater than the current capacity.
//
//==EDOC========================================================================
inline
mxt_result CBlob::ReserveCapacity(IN unsigned int uUpToCapacity)
{
    return CVectorBase::ReserveCapacity(uUpToCapacity);
}

//==SDOC========================================================================
//==
//==  UnlockCapacity
//==
//==============================================================================
//
//  Description:
//      This method unlocks the capacity of the blob. The lock capacity count is
//      a counter that is increased each time LockCapacity is called and
//      decreased each time UnlockCapacity is called. If the lock capacity count
//      is greater than zero, this means that any call to ReduceCapacity or
//      ReserveCapacity fails.
//
//==EDOC========================================================================
inline
void CBlob::UnlockCapacity()
{
    CVectorBase::UnlockCapacity();
}

//==SDOC========================================================================
//==
//==  GetSize
//==
//==============================================================================
//
//  Returns:
//      The size of the blob.
//
//  Description:
//      This method returns the size of the blob. The size is the number of
//      bytes that are already in use in the blob.
//
//==EDOC========================================================================
inline
unsigned int CBlob::GetSize() const
{
    return CVectorBase::GetSize();
}

//==SDOC========================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True if the size is 0.
//
//  Description:
//      This method returns true if the size is 0; in other words, if no bytes
//      are currently used in the blob.
//
//==EDOC========================================================================
inline
bool CBlob::IsEmpty() const
{
    return CVectorBase::IsEmpty();
}

//==SDOC====================================================================
//==
//==  IsFull
//==
//==========================================================================
//
//  Returns:
//      True when the blob is full according to its capacity.
//
//  Description:
//      This method returns true when the size of the blob equals its
//      capacity, i.e., there is no more room to add new data to it without
//      allocating more memory.
//
//==EDOC====================================================================
inline
bool CBlob::IsFull() const
{
    return CVectorBase::IsFull();
}

//==SDOC========================================================================
//==
//==  Resize
//==
//==============================================================================
//
//  Parameters:
//    uSize:
//      The size representing used bytes in the blob.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      This method resizes to uSize the number of bytes in use in the blob. The
//      parameter uSize must fit into the blob's capacity. This method is useful
//      when the blob's internal buffer is used directly.
//
//  See also: GetFirstIndexPtr, GetEndIndexPtr
//
//==EDOC========================================================================
inline
mxt_result CBlob::Resize(IN unsigned int uSize)
{
    return CVectorBase::Resize(uSize);
}

//==SDOC========================================================================
//==
//==  GetFirstIndex
//==
//==============================================================================
//
//  Returns:
//      The index of the first used byte.
//
//  Description:
//      This method returns the index of the first used byte.
//
//==EDOC========================================================================
inline
unsigned int CBlob::GetFirstIndex() const
{
    return CVectorBase::GetFirstIndex();
}

//==SDOC====================================================================
//==
//==  GetLastIndex
//==
//==========================================================================
//
//  Returns:
//      The index of the last used byte.
//
//  Description:
//      This method returns the index of the last used byte.
//
//==EDOC====================================================================
inline
unsigned int CBlob::GetLastIndex() const
{
    return CVectorBase::GetLastIndex();
}

//==SDOC====================================================================
//==
//==  GetEndIndex
//==
//==========================================================================
//
//  Returns:
//      The index of the first unused byte.
//
//  Description:
//      This method returns the index of the first unused byte. This is one
//      byte passed the last used byte in the blob.
//
//==EDOC====================================================================
inline
unsigned int CBlob::GetEndIndex() const
{
    return CVectorBase::GetEndIndex();
}

//==SDOC====================================================================
//==
//==  Insert
//==
//==========================================================================
//
//  Parameters:
//    uIndex:
//      The index where to insert the byte(s).
//
//    uCount:
//      The number of bytes to insert.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method inserts uCount uninitialized bytes at uIndex in the blob.
//      The capacity is increased if not big enough to accommodate the insertion
//      of the new bytes. If the lock capacity count is non-zero, the insertion
//      fails.
//
//  See also: Append, Erase, EraseAll
//
//==EDOC====================================================================
inline
mxt_result CBlob::Insert(IN unsigned int uIndex,
                         IN unsigned int uCount)
{
    return CVectorBase::Insert(uIndex,uCount,NULL);
}

//==SDOC========================================================================
//==
//==  Insert
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to insert the byte(s).
//
//    uCount:
//      The number of bytes to insert.
//
//    uValue:
//      The value with which to initialize inserted bytes.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method inserts uCount bytes initialized to uValue at uIndex in the
//      blob. The capacity is increased if not big enough to accommodate the
//      insertion of the new bytes. If the lock capacity count is non-zero, the
//      insertion fails.
//
//  See also: Append, Erase, EraseAll
//
//==EDOC========================================================================
inline
mxt_result CBlob::Insert(IN unsigned int uIndex,
                         IN unsigned int uCount,
                         IN uint8_t uValue)
{
    return CVectorBase::Insert(uIndex,uCount,&uValue);
}

//==SDOC========================================================================
//==
//==  Erase
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the byte to erase.
//
//  Description:
//      This method erases from the blob the byte at uIndex.
//
//  See also: Insert
//
//==EDOC========================================================================
inline
void CBlob::Erase(IN unsigned int uIndex)
{
    CVectorBase::Erase(uIndex);
}

//==SDOC========================================================================
//==
//==  Erase
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the first byte to erase.
//
//    uCount:
//      The number of bytes to erase.
//
//  Description:
//      This method erases from the blob uCount bytes at uIndex.
//
//  See also: Insert
//
//==EDOC========================================================================
inline
void CBlob::Erase(IN unsigned int uIndex, IN unsigned int uCount)
{
    CVectorBase::Erase(uIndex,uCount);
}

//==SDOC========================================================================
//==
//==  EraseAll
//==
//==============================================================================
//
//  Description:
//      This method erases all the bytes from the blob.
//
//  See also: Insert
//
//==EDOC========================================================================
inline
void CBlob::EraseAll()
{
    m_uReadIndex = 0;
    m_uBitsRead = 0;
    m_uWriteIndex = 0;
    m_uBitsWritten = 0;

    CVectorBase::EraseAll();
}

//==SDOC====================================================================
//==
//==  operator[]
//==
//==========================================================================
//
//  Parameters:
//      uIndex:
//       The index of a byte.
//
//  Returns:
//      The byte at uIndex.
//
//  Description:
//      This method returns a reference to the byte at uIndex. It returns a
//      reference to an invalid byte if uIndex is greater than or equal to the
//      size.
//
//  See also: GetAt
//
//==EDOC====================================================================
inline
const uint8_t& CBlob::operator[](IN unsigned int uIndex) const
{
    return *reinterpret_cast<const uint8_t*>(CVectorBase::operator[](uIndex));
}

inline
uint8_t& CBlob::operator[](IN unsigned int uIndex)
{
    return *reinterpret_cast<uint8_t*>(CVectorBase::operator[](uIndex));
}

//==SDOC========================================================================
//==
//==  GetAt
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the byte to retrieve.
//
//  Returns:
//      The byte at uIndex.
//
//  Description:
//      This method returns a reference to the byte at uIndex. It returns a
//      reference to an invalid byte if uIndex is greater than or equal to the
//      size.
//
//  See also:
//      operator[]
//
//==EDOC========================================================================
inline
uint8_t& CBlob::GetAt(IN unsigned int uIndex)
{
    return *reinterpret_cast<uint8_t*>(CVectorBase::GetAt(uIndex));
}

inline
const uint8_t& CBlob::GetAt(IN unsigned int uIndex) const
{
    return *reinterpret_cast<const uint8_t*>(CVectorBase::GetAt(uIndex));
}

//==SDOC========================================================================
//==
//==  Merge
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to move the blob.
//
//    rBlob:
//      The source blob.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method moves all the bytes of another blob to a specific index.
//      The capacity is increased if not big enough to accommodate the insertion
//      of the new bytes. If the lock capacity count is non-zero, the insertion
//      fails.
//
//  See also: Insert, Split
//
//==EDOC========================================================================
inline
mxt_result CBlob::Merge(IN unsigned int uIndex, INOUT CBlob& rBlob)
{
    return CVectorBase::Merge(uIndex, rBlob);
}

//==SDOC====================================================================
//==
//==  Split
//==
//==========================================================================
//
//  Parameters:
//    uIndex:
//      The index where to move the bytes.
//
//    rBlob:
//      The destination blob.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      This method moves some bytes from this blob to another starting from
//      a specific index. The destination blob is emptied first. The capacity is
//      increased if not big enough to accommodate the insertion of the new
//      bytes. If the lock capacity count is non-zero, the insertion fails.
//
//  See also: Insert, Merge
//
//==EDOC====================================================================
inline
mxt_result CBlob::Split(IN unsigned int uIndex, OUT CBlob& rBlob)
{
    return CVectorBase::Split(uIndex, rBlob);
}

//==SDOC========================================================================
//==
//==  Swap
//==
//==============================================================================
//
//  Parameters:
//    uFirstIndex:
//      The index of the first byte to swap.
//
//    uSecondIndex:
//      The index of the second byte to swap.
//
//  Description:
//      This method exchanges the position of two bytes.
//
//==EDOC========================================================================
inline
void CBlob::Swap(IN unsigned int uFirstIndex, IN unsigned int uSecondIndex)
{
    CVectorBase::Swap(uFirstIndex, uSecondIndex );
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // #ifndef MXG_CBLOB_H

