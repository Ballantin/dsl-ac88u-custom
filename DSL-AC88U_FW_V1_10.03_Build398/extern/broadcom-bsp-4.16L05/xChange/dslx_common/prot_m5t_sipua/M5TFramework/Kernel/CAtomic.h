//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CATOMIC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CATOMIC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: CAtomicOperations
//==============================================================================
//<GROUP KERNEL_TEMPLATES>
//
// Summary:
//  Encapsulates atomic operation of any types.
//
// Description:
//  CAtomicOperations is a templated class which encapsulates atomic operations
//  on any type. In it's generic implementation, atomic operations are enforced
//  using a mutex. Templated specialization of this class can be done for any
//  platform that natively supports atomic operations. It is currently
//  implemented natively for the Windows, Linux and Linux PPC platforms only.
//
//  This class is intended to be used directly in cases where native atomic
//  operations are not supported and the memory footprint of the code is a
//  constraint. By using this class, instead of the safer CAtomicValue you can
//  avoid having one mutex per synchronized value, and synchronize multiple
//  values with one mutex. In other cases, it is safer to use the CAtomicValue
//  class.
//
//  Template specialization of the class can be done for every platform
//  natively supporting atomic operations.
//
//  Current Native Support (OS / Compiler / Architecture):
//      - Windows / MSVC / x86
//          - int16_t
//          - uint16_t
//          - int32_t
//          - uint32_t
//      - Linux / GCC / x86
//          - int16_t
//          - uint16_t
//          - int32_t
//          - uint32_t
//      - Linux / GCC / PPC
//          - int32_t
//          - uint32_t
//
// See Also:
//  CAtomicValue
//
// Location:
//  Kernel/CAtomic.h
//
//==EDOC========================================================================
template<class _Type>
class CAtomicOperations
{
    //-- Published Interface
    //-----------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CAtomicOperations();

    // Summary:
    //  Specialized constructor used to pass in a shared mutex to use for the
    //  atomic operations.
    //------------------------------------------------------------------------
    CAtomicOperations(IN CMutex* pmutexShared);

    // Summary:
    //  Destructor.
    //--------------
    ~CAtomicOperations();

    //-- << Stereotype >>
    //--------------------

    // Summary:
    //  Increments the value in an atomic manner and returns it.
    //---------------------------------------------------------------------
    _Type Increment(INOUT _Type* pValue) const;

    // Summary:
    //  Decrements the value in an atomic manner and returns it.
    //---------------------------------------------------------------------
    _Type Decrement(INOUT _Type* pValue) const;

    // Summary:
    //  Exchanges the value in an atomic manner and returns the value before
    //  the operation was executed.
    //--------------------------------------------------------------------------
    _Type Exchange(IN _Type source, INOUT _Type* pDestination) const;

    // Summary:
    //  Retrieves the value in an atomic manner.
    //--------------------------------------------------
    _Type Retrieve(INOUT _Type const* pValue) const;

    //-- Hidden Methods
    //-------------------
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CAtomicOperations(IN const CAtomicOperations& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CAtomicOperations& operator=(IN const CAtomicOperations& rSrc);


    //-- Hidden Data Members
    //------------------------
private:

    // Mutex needed for the generic case of atomization.
    CMutex* m_pMutex;

    // Flag representing whether the mutex is a shared mutex or owned by the
    // CAtomicOperations class.
    bool m_bIsMutexOwner;
};

//==SDOC========================================================================
//== Class: CAtomicValue
//==============================================================================
//<GROUP KERNEL_TEMPLATES>
//
// Summary:
//  Represents an atomic value on which atomic operations can be performed.
//
// Description:
//  Class representing an atomic value on which multiple operations can be
//  performed. The class derives from CAtomicOperations to provide the atomic
//  functionality.
//
//  Note: CAtomicValue derives from CAtomicOperations instead of having a
//  CAtomicOperations member so that if a specialized templated version of
//  CAtomicOperations that doesn't contain any members is used, CAtomicValue
//  will not gain one byte in size for having an empty class member.
//
// Example:
//
// <CODE>
//    class RefCountedClass
//    {
//        RefCountedClass()
//            :   m_atomicRefCount(1)
//        {
//        }
//
//        uint32_t AddRef()
//        {
//            return m_atomicRefCount.Increment();
//        }
//
//        uint32_t Release()
//        {
//            uint32_t uResult = m_atomicRefCount.Decrement();
//            if (uResult < 1)
//            {
//                delete this;
//            }
//            return result;
//        }
//
//    protected:
//        virtual ~RefCountedClass()
//        {
//            MX_ASSERT(m_atomicRefCount.Get() == 0);
//        }
//
//    private:
//        CAtomicValue<uint32_t> m_atomicRefCount;
//    };
// </CODE>
//
// Location:
//  Kernel/CAtomic.h
//  CAtomicOperations
//
// See Also:
//  CMutex
//
//==EDOC========================================================================
template<class _Type>
class CAtomicValue : private CAtomicOperations<_Type>
{
public:
    // Summary:
    //  Default constructor.
    //-----------------------
    CAtomicValue();

    // Summary:
    //  Specialized constructor that initializes the constructed CAtomicValue
    //  to the specified value.
    //------------------------------------------------------------------------
    CAtomicValue(_Type const& initialValue);

    // Summary:
    //  Increments the value in an atomic manner and returns it.
    //-----------------------------------------------------------
    _Type Increment();

    // Summary:
    //  Decrements the value in an atomic manner and returns it.
    //-----------------------------------------------------------
    _Type Decrement();

    // Summary:
    //  Exchanges the value in an atomic manner and returns the old value before
    //  the operation was executed.
    //--------------------------------------------------------------------------
    _Type Exchange(_Type const& newValue = _Type());

    // Summary:
    //  Retrieves the value in an atomic manner.
    //------------------------------------------------
    _Type Retrieve() const;

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CAtomicValue(IN const CAtomicValue& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CAtomicValue& operator=(IN const CAtomicValue& rSrc);

    //-- Hidden Data Members
    //------------------------
private:

    // The value which holds the atomic type.
    _Type m_value;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  CAtomicOperations METHODS  =============================================
//==============================================================================

//==SDOC========================================================================
//==
//== CAtomicOperations
//==
//==============================================================================
//
// Description:
//  Default constructor for the CAtomicOperations class that creates a mutex
//  owned by this instance of the CAtomicOperations class.
//
//==EDOC========================================================================
template<class _Type>
inline CAtomicOperations<_Type>::CAtomicOperations()
:   m_pMutex(MX_NEW(CMutex)),
    m_bIsMutexOwner(true)
{
}

//==SDOC========================================================================
//==
//== CAtomicOperations
//==
//==============================================================================
//
//  Parameters:
//      pmutexShared:
//       Parameter that allows specifcation of a shared mutex to be used for
//       synchronizing the atomic operations. Note that if a mutex is
//       specified, it is not owned by the CAtomicOperations class and needs to be
//       valid for the lifetime of the CAtomicOperations instance.
//
//  Description:
//      Specialized constructor for the CAtomicOperations class that allows
//      specification of a shared mutex that will be used to synchronize the
//      atomic operations.
//
//==EDOC========================================================================
template<class _Type>
inline CAtomicOperations<_Type>::CAtomicOperations(IN CMutex* pmutexShared)
{
    if (pmutexShared != NULL)
    {
        m_bIsMutexOwner = false;
        m_pMutex = pmutexShared;
    }
    else
    {
        m_bIsMutexOwner = true;
        m_pMutex = MX_NEW(CMutex);
    }
}


//==SDOC========================================================================
//==
//== ~CAtomicOperations
//==
//==============================================================================
//
// Description:
//  Destructor for the CAtomicOperations class that destroys the mutex if it is
//  owned by this instance of the CAtomicOperations class.
//
//==EDOC========================================================================
template<class _Type>
inline CAtomicOperations<_Type>::~CAtomicOperations()
{
    if (m_bIsMutexOwner)
    {
        MX_DELETE(m_pMutex);
    }
}

//==SDOC========================================================================
//==
//== Increment
//==
//==============================================================================
//
//  Parameters:
//      pValue:
//       A pointer to the value that must be atomically incremented.
//
//  Returns:
//      The incremented value.
//
//  Description:
//      Method that performs an atomic increment on the value pointed to by the
//      passed pointer.
//
//==EDOC========================================================================
template<class _Type>
inline _Type CAtomicOperations<_Type>::Increment(INOUT _Type* pValue) const
{
    _Type result;
    m_pMutex->Lock();
    /**/    result = ++(*pValue);
    m_pMutex->Unlock();
    return result;
}

//==SDOC========================================================================
//==
//== Decrement
//==
//==============================================================================
//
//  Parameters:
//      pValue:
//       A pointer to the value that must atomically decremented.
//
//  Returns:
//      The decremented value.
//
//  Description:
//      Method that performs an atomic decrement on the value pointed to by
//      the passed pointer.
//
//==EDOC========================================================================
template<class _Type>
inline _Type CAtomicOperations<_Type>::Decrement(INOUT _Type* pValue) const
{
    _Type result;
    m_pMutex->Lock();
    /**/    result = --(*pValue);
    m_pMutex->Unlock();
    return result;
}

//==SDOC========================================================================
//==
//== Exchange
//==
//==============================================================================
//
//  Parameters:
//      source:
//       The new value to be exchanged with the value pointed to by the
//       pDestination parameter.
//
//      pDestination:
//       Pointer to the value to be exchanged with the source.
//
//  Returns:
//      The value of pDestination before the atomic exchange operation is done.
//
//  Description:
//      Method that atomically exchanges the existing value pointed to by the
//      specified destination with the new value specified in the source.
//
//==EDOC========================================================================
template<class _Type>
inline _Type CAtomicOperations<_Type>::Exchange(IN _Type source, INOUT _Type* pDestination) const
{
    _Type result;
    m_pMutex->Lock();
    /**/    result = *pDestination;
    /**/    *pDestination = source;
    m_pMutex->Unlock();
    return result;
}

//==SDOC========================================================================
//==
//== Retrieve
//==
//==============================================================================
//
//  Parameters:
//      pValue:
//       A pointer to the value that should atomically retrieved.
//
//  Returns:
//      The retrieved value.
//
//  Description:
//      Method that atomically retrieves the value pointed to by the passed
//      pointer.
//
//==EDOC========================================================================
template<class _Type>
inline _Type CAtomicOperations<_Type>::Retrieve(INOUT _Type const* pValue) const
{
    _Type result;
    m_pMutex->Lock();
    /**/ result = *pValue;
    m_pMutex->Unlock();
    return result;
}

//==============================================================================
//====  CAtomicValue METHODS  ==================================================
//==============================================================================

//==SDOC========================================================================
//==
//== CAtomicValue
//==
//==============================================================================
//
// Description:
//  Default constructor for the CAtomicValue class. The default constructor
//  will initialize the value to the default constructed _Type type.
//
//==EDOC========================================================================
template<class _Type>
inline CAtomicValue<_Type>::CAtomicValue()
:   CAtomicOperations<_Type>(),
    m_value(_Type())
{
}

//==SDOC========================================================================
//==
//== CAtomicValue
//==
//==============================================================================
//
//  Parameters:
//      initialValue:
//       The initial value.
//
//  Description:
//      Specialized constructor that allows the specification of an initial
//      value.
//
//==EDOC========================================================================
template<class _Type>
inline CAtomicValue<_Type>::CAtomicValue(_Type const& initialValue)
:   CAtomicOperations<_Type>(),
    m_value(initialValue)
{
}

//==SDOC========================================================================
//==
//== Increment
//==
//==============================================================================
//
// Returns:
//     The incremented value of this CAtomicValue object.
//
// Description:
//     Method that performs an atomic increment.
//
//==EDOC========================================================================
template<class _Type>
inline _Type CAtomicValue<_Type>::Increment()
{
    return CAtomicOperations<_Type>::Increment(&m_value);
}

//==SDOC========================================================================
//==
//== Decrement
//==
//==============================================================================
//
// Returns:
//     The decremented value of this CAtomicValue object.
//
// Description:
//     Method that performs an atomic decrement.
//
//==EDOC========================================================================
template<class _Type>
inline _Type CAtomicValue<_Type>::Decrement()
{
    return CAtomicOperations<_Type>::Decrement(&m_value);
}

//==SDOC========================================================================
//==
//== Exchange
//==
//==============================================================================
//
//  Parameters:
//      newValue:
//       The new value.
//
//  Returns:
//      The original value of this CAtomicValue object.
//
//  Description:
//      Method that atomically exchanges the existing value with a new one.
//
//==EDOC========================================================================
template<class _Type>
inline _Type CAtomicValue<_Type>::Exchange(_Type const& newValue)
{
    return CAtomicOperations<_Type>::Exchange(newValue, &m_value);
}

//==SDOC========================================================================
//==
//== Retrieve
//==
//==============================================================================
//
// Returns:
//     The current value of this CAtomicValue object.
//
// Description:
//     Method that atomically retrieves the internal value of this CAtomicValue
//     object. Note that this value simply represents a snapshot of this
//     CAtomicValue in it's lifetime. It is dangerous to use the Retrieve method
//     to perform synchronization as it will lead to race conditions.
//
//==EDOC========================================================================
template<class _Type>
inline _Type CAtomicValue<_Type>::Retrieve() const
{
    return CAtomicOperations<_Type>::Retrieve(&m_value);
}

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_ATOMIC_NATIVE_ENABLE_SUPPORT)

#define MXD_ATOMIC_NATIVE_OPERATIONS_IMPL

// Include specialization of the atomic operations for each OS to add native
// atomic support which will improve performance greatly.
#if defined(MXD_COMPILER_MS_VC) && defined(MXD_OS_WINDOWS) && defined(MXD_ARCH_IX86)
    #include "Kernel/CAtomicWin32.h"
#elif defined(MXD_OS_LINUX) && \
      defined(MXD_COMPILER_GNU_GCC) && \
      (defined(MXD_ARCH_IX86) || defined(MXD_ARCH_AMD64))
    #include "Kernel/CAtomicLinuxi386.h"
#elif defined(MXD_OS_LINUX) && defined(MXD_COMPILER_GNU_GCC) && defined(MXD_ARCH_PPC)
    #include "Kernel/CAtomicLinuxPPC.h"
#endif

#undef MXD_ATOMIC_NATIVE_OPERATIONS_IMPL

#endif // #if defined (MXD_ATOMIC_NATIVE_ENABLE_SUPPORT)
//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CATOMIC_H

