//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CMARSHALER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMARSHALER_H
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
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT, MXD_CMARSHALER_ENABLE_DEBUG
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CSHAREDPTR_H
#include "Basic/CSharedPtr.h"
#endif
#ifndef MXG_CPOOL_H
#include "Cap/CPool.h"
#endif

// Interface Realized & Parent

// BRCM_CUSTOM - [begin] - Add ECOS support.
#if defined( MXD_OS_BOS_ECOS )
#   include <bosSocket.h>
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.

//M5T_INTERNAL_USE_BEGIN
// Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
class CMarshaler;
mxt_result InitializeCMarshaler();
void FinalizeCMarshaler();
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
const uint32_t MARSHALER_FIRST_MEMORY_SEGMENT_SIZE = 64;
const uint32_t MARSHALER_FIRST_MEMORY_SEGMENT_SPACE = MARSHALER_FIRST_MEMORY_SEGMENT_SIZE - sizeof(ptrdiff_t);

#ifdef MXD_CMARSHALER_ENABLE_DEBUG
const uint8_t MARSHALING_TYPE_BOOL          = 1;
const uint8_t MARSHALING_TYPE_CHAR          = 2;
const uint8_t MARSHALING_TYPE_UCHAR         = 3;
const uint8_t MARSHALING_TYPE_SCHAR         = 4;
const uint8_t MARSHALING_TYPE_USHORT        = 5;
const uint8_t MARSHALING_TYPE_SSHORT        = 6;
const uint8_t MARSHALING_TYPE_UINT          = 7;
const uint8_t MARSHALING_TYPE_SINT          = 8;
const uint8_t MARSHALING_TYPE_ULONG         = 9;
const uint8_t MARSHALING_TYPE_SLONG         = 10;
const uint8_t MARSHALING_TYPE_ULONGLONG     = 11;
const uint8_t MARSHALING_TYPE_SLONGLONG     = 12;
const uint8_t MARSHALING_TYPE_FLOAT         = 13;
const uint8_t MARSHALING_TYPE_DOUBLE        = 14;
const uint8_t MARSHALING_TYPE_LONGDOUBLE    = 15;
const uint8_t MARSHALING_TYPE_POINTER       = 16;
const uint8_t MARSHALING_TYPE_UNKNOWN       = 17;
const uint8_t MARSHALING_TYPE_CSHAREDPTR    = 18;
// BRCM_CUSTOM - [begin] - Add ECOS support.
#   if defined( MXD_OS_BOS_ECOS )
#      if BOS_CFG_SOCKET_INTERFACE_SUPPORT
const UINT8 MARSHALING_TYPE_BOSSOCKET       = 19;
#      endif
#   endif
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif
//M5T_INTERNAL_USE_END


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
#if defined(_Type)
#error "_Type" is already defined as a MACRO. Cannot be used again in template definition.
#endif

// Function prototype

//<GROUP CMarshaler>
//<COMBINE CMarshaler::\<\<@IN bool>
template <class _Type>
CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const _Type* pPtr);

//<GROUP CMarshaler>
//<COMBINE CMarshaler::\>\>@OUT bool&>
template <class _Type>
CMarshaler& operator>> (IN CMarshaler& rMarshaler, OUT _Type*& pPtr);

//<GROUP CMarshaler>
//<COMBINE CMarshaler::\<\<@IN const CSharedPtr<_Type\>&>
template <class _Type>
CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CSharedPtr<_Type>& rstSharedPtr);

//<GROUP CMarshaler>
//<COMBINE CMarshaler::\>\>@OUT CSharedPtr<_Type\>&>
template <class _Type>
CMarshaler& operator>> (IN CMarshaler& rMarshaler, OUT CSharedPtr<_Type>& rstSharedPtr);

//==SDOC========================================================================
//==
//==  Class: CMarshaler
//==
//==============================================================================
//<GROUP CAP_CLASSES>
//
//  Summary:
//      Class used to store objects.
//
//  Description:
//      The CMarshaller class is used to store objects. It is designed to
//      rapidly insert and extract objects stored within. All elements are
//      first in first out. It is up to the application to be prepared to
//      receive any kind of data when getting an object.
//
//      Objects stored inside the marshaler are put into a preallocated pool.
//
//==EDOC========================================================================
class CMarshaler
{
// Templated friends declarations are not supported in gcc 2.7.2.
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
    template <class _Type> friend CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const _Type* pPtr);
    template <class _Type> friend CMarshaler& operator>> (IN CMarshaler& rMarshaler, OUT _Type*& pPtr);
    template <class _Type> friend CMarshaler& operator<< (IN CMarshaler& rMarshaler,
                                                          IN const CSharedPtr<_Type>& rstSharedPtr);
    template <class _Type> friend CMarshaler& operator>> (IN CMarshaler& rMarshaler,
                                                          OUT CSharedPtr<_Type>& rstSharedPtr);
#endif

// Published Interface
//--------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CMarshaler();

    // Summary:
    //  Destructor.
    //---------------
    ~CMarshaler();

    // Summary:
    //  Copy constructor.
    //--------------------
    CMarshaler(const CMarshaler& rMarshaler);

    // << operators >>
    //-----------------

    // Summary:
    //  Assignment operator
    //----------------------
    CMarshaler& operator=(const CMarshaler& rFrom);

    // Summary:
    //  Clears the marshaler.
    //------------------------
    void Clear();

    // Summary:
    //  Checks whether or not the marshaler is empty.
    //------------------------------------------------
    bool IsEmpty() const;

    // << allocators >>
    //---------------------------
    //M5T_INTERNAL_USE_BEGIN
    void* operator new(size_t size);
    void* operator new(size_t size, void* pvoid);
    void  operator delete(void* pvoid);
    void  operator delete(void* pvoid, void*);
    //M5T_INTERNAL_USE_END

    // Storing Methods
    //-----------------

    //-- Bool
    // Summary:
    //  Inserts an object into the marshaler.
    //----------------------------------------
    CMarshaler& operator<< (IN bool data);

    //-- Char
    CMarshaler& operator<< (IN char data);
    CMarshaler& operator<< (IN unsigned char data);
    CMarshaler& operator<< (IN signed char data);

    //-- Short
    CMarshaler& operator<< (IN unsigned short data);
    CMarshaler& operator<< (IN signed short data);

    //-- Int
    CMarshaler& operator<< (IN unsigned int data);
    CMarshaler& operator<< (IN signed int data);

    //-- Long
    CMarshaler& operator<< (IN unsigned long data);
    CMarshaler& operator<< (IN signed long data);

#if defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    //-- Use 64 bits instead of long long since it is not supported.
    CMarshaler& operator<< (IN unsigned __int64 data);
    CMarshaler& operator<< (IN __int64 data);
#else
    //-- Long Long
    CMarshaler& operator<< (IN unsigned long long data);
    CMarshaler& operator<< (IN signed long long data);
#endif //defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)

    //-- Float
    CMarshaler& operator<< (IN const float& rData);

    //-- Double
    CMarshaler& operator<< (IN const double& rData);

    //-- Long Double
    CMarshaler& operator<< (IN const long double& rData);

// BRCM_CUSTOM - [begin] - Add ECOS support.
#if defined( MXD_OS_BOS_ECOS )
    //-- BOS_SOCKET
    CMarshaler& operator<< (IN const BOS_SOCKET rData);
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.

    // Summary:
    //  Stores variable length data into the marshaler.
    //--------------------------------------------------
    void Store(IN const void* pData, IN unsigned int uSize);


    // Loading Methods
    //-----------------

    //-- Bool
    // Summary:
    //  Extracts an object from the marshaler.
    //----------------------------------------
    CMarshaler& operator>> (OUT bool& rData);

    //-- Char
    CMarshaler& operator>> (OUT char& rData);
    CMarshaler& operator>> (OUT unsigned char& rData);
    CMarshaler& operator>> (OUT signed char& rData);

    //-- Short
    CMarshaler& operator>> (OUT unsigned short& rData);
    CMarshaler& operator>> (OUT signed short& rData);

    //-- Int
    CMarshaler& operator>> (OUT unsigned int& rData);
    CMarshaler& operator>> (OUT signed int& rData);

    //-- Long
    CMarshaler& operator>> (OUT unsigned long& rData);
    CMarshaler& operator>> (OUT signed long& rData);

#if defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    //-- Use 64 bits instead of long long since it is not supported.
    CMarshaler& operator>> (OUT unsigned __int64& rData);
    CMarshaler& operator>> (OUT __int64& rData);
#else
    //-- Long Long
    CMarshaler& operator>> (OUT unsigned long long& rData);
    CMarshaler& operator>> (OUT signed long long& rData);
#endif // defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)

    //-- Float
    CMarshaler& operator>> (OUT float& rData);

    //-- Double
    CMarshaler& operator>> (OUT double& rData);

    //-- Long Double
    CMarshaler& operator>> (OUT long double& rData);

// BRCM_CUSTOM - [begin] - Add ECOS support.
#if defined( MXD_OS_BOS_ECOS )
    //-- BOS_SOCKET
    CMarshaler& operator>> (OUT BOS_SOCKET& rData);
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.

    // Summary:
    //  Loads data from the marshaler.
    //---------------------------------
    bool Load(OUT void* pData, IN unsigned int uCapacity);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//---------------
private:

    // Data Initialization and release.
    void ResetMembers();
    void ReleaseSegments();

    // Static initialization mechanism.
    static mxt_result InitializeCMarshaler();
    static void FinalizeCMarshaler();

    // Friends declarations.
    friend mxt_result InitializeCMarshaler();
    friend void FinalizeCMarshaler();

#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
private:
#else
// Both Insert and Extract methods need to be defined public when built using
// gcc 2.7, since template resolution for shift operators is buggy in this
// compiler's version.
//----------------------------------------------------------------------------
public:
#endif

    // Insertion and extraction.
    void Insert(IN const void* pData, IN unsigned int uSize);
    void Extract(OUT void* pData, IN unsigned int uSize);

// Hidden Data Members
//--------------------
private:

    // Extraction and Insertion Pointers
    //-----------------------------------
    uint8_t* m_pInsertPosition;
    uint8_t* m_pInsertSegmentEnd;
    uint8_t* m_pExtractPosition;
    uint8_t* m_pExtractSegmentEnd;

    uint8_t m_pFirstSegment[MARSHALER_FIRST_MEMORY_SEGMENT_SIZE];
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  CMarshaler
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CMarshaler::CMarshaler()
{
    ResetMembers();
}

//==SDOC========================================================================
//==
//==  CMarshaler
//==
//==============================================================================
//
//  Parameters:
//      rMarshaler:
//          Reference to the CMarshaler object to copy.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CMarshaler::CMarshaler(const CMarshaler& rMarshaler)
{
    ResetMembers();
    *this = rMarshaler;
}

//==SDOC========================================================================
//==
//==  ~CMarshaler
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CMarshaler::~CMarshaler()
{
    MX_ASSERT(IsEmpty());
    ReleaseSegments();
}

//DOM-IGNORE-BEGIN
inline
void* CMarshaler::operator new(size_t /*size*/)
{
    // Allocate one Blob from the pool
    //-----------------------------------
    return CPool<CMarshaler>::Allocate();
}

inline
void* CMarshaler::operator new(size_t /*size*/, void* pvoid)
{
    return pvoid;
}

inline
void CMarshaler::operator delete(void* pvoid)
{
    // Release Blob to the pool
    //----------------------------
    CPool<CMarshaler>::Deallocate(pvoid);
}

inline
void CMarshaler::operator delete(void* pvoid, void*)
{
    // Release Blob to the pool
    //----------------------------
    CPool<CMarshaler>::Deallocate(pvoid);
}
//DOM-IGNORE-END

//==SDOC========================================================================
//==
//==  Clear
//==
//==============================================================================
//
//  Description:
//      Clears all items in the marshaler.
//
//==EDOC========================================================================
inline
void CMarshaler::Clear()
{
    ReleaseSegments();
    ResetMembers();
}

//==SDOC========================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True if the marshaler is empty, false otherwise.
//
//  Description:
//      Checks whether or not the CMarshaler object is empty.
//
//==EDOC========================================================================
inline
bool CMarshaler::IsEmpty() const
{
    return (m_pInsertPosition == m_pExtractPosition);
}

//==============================================================================
//====  INSERTION OPERATOR  ====================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  operator<<
//==
//==============================================================================
//
//  Parameters
//      data :
//          Object to insert into the marshaler.
//
//  Description:
//      Inserts an object into the marshaler. It is possible to marshal classes
//      into the CMarshaler object. To do this, the \<\< operators must be added
//      to the class to marshal for a CMarshaler. Please refer to the CBlob
//      implementation in CBlob.h and CBlob.cpp for an example of how to
//      implement the \<\< operator.
//
//      A templated version of this operator can marshal a pointer to any type.
//
//  See Also:
//      <LINK CMarshaler::\>\>@OUT bool&, \CMarshaler::operator\>\>>
//
//==EDOC========================================================================
inline CMarshaler& CMarshaler::operator<< (IN bool data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_BOOL, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(bool));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN char data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_CHAR, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(char));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN unsigned char data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_UCHAR, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(unsigned char));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN signed char data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_SCHAR, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(signed char));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN unsigned short data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_USHORT, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(unsigned short));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN signed short data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_SSHORT, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(signed short));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN unsigned int data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_UINT, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(unsigned int));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN signed int data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_SINT, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(signed int));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN unsigned long data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_ULONG, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(unsigned long));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN signed long data)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_SLONG, sizeof(uint8_t));
#endif

    Insert(&data, sizeof(signed long));
    return *this;
}

#if defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
//<COMBINE CMarshaler::\<\<@IN bool>
    inline
    CMarshaler& CMarshaler::operator<< (IN unsigned __int64 data)
    {
    #ifdef MXD_CMARSHALER_ENABLE_DEBUG
        Insert(&MARSHALING_TYPE_ULONGLONG, sizeof(uint8_t));
    #endif

        Insert(&data, sizeof(unsigned __int64));
        return *this;
    }

//<COMBINE CMarshaler::\<\<@IN bool>
    inline
    CMarshaler& CMarshaler::operator<< (IN __int64 data)
    {
    #ifdef MXD_CMARSHALER_ENABLE_DEBUG
        Insert(&MARSHALING_TYPE_SLONGLONG, sizeof(uint8_t));
    #endif

        Insert(&data, sizeof(__int64));
        return *this;
    }
#else

//<COMBINE CMarshaler::\<\<@IN bool>
    inline
    CMarshaler& CMarshaler::operator<< (IN unsigned long long data)
    {
    #ifdef MXD_CMARSHALER_ENABLE_DEBUG
        Insert(&MARSHALING_TYPE_ULONGLONG, sizeof(uint8_t));
    #endif

        Insert(&data, sizeof(unsigned long long));
        return *this;
    }

//<COMBINE CMarshaler::\<\<@IN bool>
    inline
    CMarshaler& CMarshaler::operator<< (IN signed long long data)
    {
    #ifdef MXD_CMARSHALER_ENABLE_DEBUG
        Insert(&MARSHALING_TYPE_SLONGLONG, sizeof(uint8_t));
    #endif

        Insert(&data, sizeof(signed long long));
        return *this;
    }
#endif // defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN const float& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_FLOAT, sizeof(uint8_t));
#endif

    Insert(&rData, sizeof(float));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN const double& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_DOUBLE, sizeof(uint8_t));
#endif

    Insert(&rData, sizeof(double));
    return *this;
}

//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& CMarshaler::operator<< (IN const long double& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_LONGDOUBLE, sizeof(uint8_t));
#endif

    Insert(&rData, sizeof(long double));
    return *this;
}

template<class _Type>
//<COMBINE CMarshaler::\<\<@IN bool>
inline
CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const _Type* pPtr)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    rMarshaler.Insert(&MARSHALING_TYPE_POINTER, sizeof(uint8_t));
#endif

    rMarshaler.Insert(&pPtr, sizeof(const _Type*));
    return rMarshaler;
}

template<class _Type>
//<COMBINE CMarshaler::\<\<@IN const CSharedPtr<_Type>&>
inline
CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CSharedPtr<_Type>& rstSharedPtr)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    rMarshaler.Insert(&MARSHALING_TYPE_CSHAREDPTR, sizeof(uint8_t));
#endif

    _Type* pInterface = rstSharedPtr.Get();

    if (pInterface != NULL)
    {
        pInterface->AddIfRef();
    }

    rMarshaler.Insert(&pInterface, sizeof(pInterface));

    return rMarshaler;
}

// BRCM_CUSTOM - [begin] - Add ECOS support.
//<COMBINE CMarshaler::\<\<@IN bool>
#if defined( MXD_OS_BOS_ECOS )
inline
CMarshaler& CMarshaler::operator<< (IN BOS_SOCKET data)
{
#   ifdef MXD_CMARSHALER_ENABLE_DEBUG
    Insert(&MARSHALING_TYPE_BOSSOCKET, sizeof(uint8_t));
#   endif

    Insert(&data, sizeof(BOS_SOCKET));
    return *this;
}
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.

//==============================================================================
//====  EXTRACTION OPERATOR ====================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  operator>>
//==
//==============================================================================
//
//  Parameters
//      rData :
//          Object extracted from the marshaler.
//
//  Description:
//      Extracts an object from the marshaler. It is possible to marshal classes
//      into the CMarshaler object. A mashalled class must be extracted at some
//      point this is the operator used to do this.
//
//      A templated version of this operator can marshal a pointer to any type.
//
//  See Also:
//      <LINK CMarshaler::\<\<@IN bool, \CMarshaler::operator\<\<>
//
//==EDOC========================================================================
inline
CMarshaler& CMarshaler::operator>> (OUT bool& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_BOOL);
#endif

    Extract(&rData, sizeof(bool));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT char& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_CHAR);
#endif

    Extract(&rData, sizeof(char));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT unsigned char& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_UCHAR);
#endif

    Extract(&rData, sizeof(unsigned char));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT signed char& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_SCHAR);
#endif

    Extract(&rData, sizeof(signed char));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT unsigned short& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_USHORT);
#endif

    Extract(&rData, sizeof(unsigned short));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT signed short& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_SSHORT);
#endif

    Extract(&rData, sizeof(signed short));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT unsigned int& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_UINT);
#endif

    Extract(&rData, sizeof(unsigned int));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT signed int& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_SINT);
#endif

    Extract(&rData, sizeof(signed int));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT unsigned long& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_ULONG);
#endif

    Extract(&rData, sizeof(unsigned long));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT signed long& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_SLONG);
#endif

    Extract(&rData, sizeof(signed long));
    return *this;
}

#if defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    //<COMBINE CMarshaler::\>\>@OUT bool&>
    inline
    CMarshaler& CMarshaler::operator>> (OUT unsigned __int64& rData)
    {
    #ifdef MXD_CMARSHALER_ENABLE_DEBUG
        uint8_t bDataType = 0;
        Extract(&bDataType, sizeof(uint8_t));
        MX_ASSERT(bDataType == MARSHALING_TYPE_ULONGLONG);
    #endif

        Extract(&rData, sizeof(unsigned __int64));
        return *this;
    }

    //<COMBINE CMarshaler::\>\>@OUT bool&>
    inline
    CMarshaler& CMarshaler::operator>> (OUT __int64& rData)
    {
    #ifdef MXD_CMARSHALER_ENABLE_DEBUG
        uint8_t bDataType = 0;
        Extract(&bDataType, sizeof(uint8_t));
        MX_ASSERT(bDataType == MARSHALING_TYPE_SLONGLONG);
    #endif

        Extract(&rData, sizeof(__int64));
        return *this;
    }
#else
    //<COMBINE CMarshaler::\>\>@OUT bool&>
    inline
    CMarshaler& CMarshaler::operator>> (OUT unsigned long long& rData)
    {
    #ifdef MXD_CMARSHALER_ENABLE_DEBUG
        uint8_t bDataType = 0;
        Extract(&bDataType, sizeof(uint8_t));
        MX_ASSERT(bDataType == MARSHALING_TYPE_ULONGLONG);
    #endif

        Extract(&rData, sizeof(unsigned long long));
        return *this;
    }

    //<COMBINE CMarshaler::\>\>@OUT bool&>
    inline
    CMarshaler& CMarshaler::operator>> (OUT signed long long& rData)
    {
    #ifdef MXD_CMARSHALER_ENABLE_DEBUG
        uint8_t bDataType = 0;
        Extract(&bDataType, sizeof(uint8_t));
        MX_ASSERT(bDataType == MARSHALING_TYPE_SLONGLONG);
    #endif

        Extract(&rData, sizeof(signed long long));
        return *this;
    }
#endif //defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT float& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_FLOAT);
#endif

    Extract(&rData, sizeof(float));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT double& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_DOUBLE);
#endif

    Extract(&rData, sizeof(double));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
inline
CMarshaler& CMarshaler::operator>> (OUT long double& rData)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_LONGDOUBLE);
#endif

    Extract(&rData, sizeof(long double));
    return *this;
}

//<COMBINE CMarshaler::\>\>@OUT bool&>
template<class _Type>
inline
CMarshaler& operator>> (IN CMarshaler& rMarshaler, OUT _Type*& pPtr)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    rMarshaler.Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_POINTER);
#endif

    rMarshaler.Extract(&pPtr, sizeof(const _Type*));
    return rMarshaler;
}

//<COMBINE CMarshaler::\>\>@OUT CSharedPtr<_Type>&>
template<class _Type>
CMarshaler& operator>> (IN CMarshaler& rMarshaler, OUT CSharedPtr<_Type>& rstSharedPtr)
{
#ifdef MXD_CMARSHALER_ENABLE_DEBUG
    uint8_t bDataType = 0;
    rMarshaler.Extract(&bDataType, sizeof(uint8_t));
    MX_ASSERT(bDataType == MARSHALING_TYPE_CSHAREDPTR);
#endif

    _Type* pInterface = NULL;

    rMarshaler.Extract(&pInterface, sizeof(pInterface));

    // We must use the cast operator to get access to the internal pointer so that we can bypass the AddIfRef call.
    *static_cast<_Type**>(rstSharedPtr) = pInterface;

    return rMarshaler;
}
// BRCM_CUSTOM - [begin] - Add ECOS support.
//<COMBINE CMarshaler::\>\>@OUT bool&>
#if defined( MXD_OS_BOS_ECOS )
inline
CMarshaler& CMarshaler::operator>> (OUT BOS_SOCKET& rData)
{
#   ifdef MXD_CMARSHALER_ENABLE_DEBUG
     uint8_t bDataType = 0;
     Extract(&bDataType, sizeof(uint8_t));
     MX_ASSERT(bDataType == MARSHALING_TYPE_BOSSOCKET);
#   endif

    Extract(&rData, sizeof(BOS_SOCKET));
    return *this;
}
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.


MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // #ifndef MXG_CMARSHALER_H


