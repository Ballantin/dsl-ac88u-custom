//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CPOOL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPOOL_H
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

#ifndef MXG_MXTRACEFRAMEWORK_H
#include "Basic/MxTraceBasic.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CBLOCKALLOCATOR_H
#include "Cap/CBlockAllocator.h"
#endif
#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif
#ifndef MXG_ALLOCATOR_H
#include "Cap/IAllocator.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

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
//== Class: CPool
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//      The class CPool implements a typed element pool.
//
// Description:
//      The class CPool implements a typed element pool.
//      It must be initialized before use. Support for concurrency protection
//      is optional.
//
//      This class is meant to be used directly. It is not being inherited,
//      although it is possible.
//
// <CODE>
//
//      class CRtpPacket
//      {
//          ...
//      };
//
//      void SomeFunction(...)
//      {
//          // Constructs an element from the pool.
//          CRtpPacket* pRtpPacket = CPool<CRtpPacket>::New();
//
//          ...
//
//          // Returns the element to the pool.
//          CPool<CRtpPacket>::Delete(pRtpPacket);
//          pRtpPacket = NULL;
//      }
//
// </CODE>
//
// Location:
//   Cap/CPool.h
//
// See Also:
//
//==EDOC========================================================================
template<class _Type>
class CPool
{
//-- Published Interface
//------------------------
public:

    // Summary:
    //  Initializes the pool.
    static void Initialize(IN bool bEnableThreadProtection = true,
                           IN unsigned int uInitialCapacity = 0,
                           IN bool bInitialCapacityLocked = false,
                           IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Uninitializes the pool.
    static void Uninitialize();

    // Summary:
    //  Allocates and constructs a new element by calling its default
    //  constructor.
    static _Type* GO New();

    // Summary:
    //  Allocates and constructs a new element by calling its copy constructor.
    static _Type* GO New(IN _Type& rTypeFrom);

    // Summary:
    //  Destructs an element and returns it to the Pool.
    static void Delete(TOA IN _Type* pType);

    // Summary:
    //  Allocates a new element without calling its constructor.
    static void* GO Allocate();

    // Summary:
    //  Deallocates an element without calling its destructor.
    static void Deallocate(TOA IN void* pvoid);

    // Summary:
    //  Returns the capacity.
    static unsigned int GetCapacity();

    // Summary:
    //  Returns the lock capacity.
    static unsigned int GetLockCapacity();

    // Summary:
    //  Locks the capacity.
    static void LockCapacity();

    // Summary:
    //  Reduces the capacity.
    static mxt_result ReduceCapacity(IN unsigned int uDownToCapacity);

    // Summary:
    //  Increases the capacity.
    static mxt_result ReserveCapacity(IN unsigned int uUpToCapacity);

    // Summary:
    //  Unlocks the capacity.
    static void UnlockCapacity();

//M5T_INTERNAL_USE_BEGIN
    static mxt_result InitializeCPool();
    static void FinalizeCPool();
//M5T_INTERNAL_USE_END

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors >>
    //-------------------------------------

    CPool() {}
    ~CPool() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    CPool(IN const CPool& rSrc);
    CPool& operator=(IN const CPool& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:

    static CBlockAllocator* ms_pBlockAllocator;
    static unsigned int ms_uAllocatedCount;
    static unsigned int ms_uCapacity;
    static unsigned int ms_uLockCapacity;

    static CMutex* ms_pmutex;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  Initialize
//==
//==============================================================================
//
//  Parameters:
//      bEnableThreadProtection:
//          True if this object is to have thread protection enabled.
//
//      uInitialCapacity:
//          Sets the initial number of objects CPool can hold on start.
//
//      bInitialCapacityLocked:
//          True if the capacity set is the maximal capacity allowed.
//
//      pAllocator:
//          Pointer to an IAllocator.
//
//  Description:
//      Initializes the pool. This method must be called once per class that
//      inherits from CPool.
//
//==EDOC========================================================================
template<class _Type>
void CPool<_Type>::Initialize(IN bool bEnableThreadProtection,
                              IN unsigned int uInitialCapacity,
                              IN bool bInitialCapacityLocked,
                              IN IAllocator* pAllocator)
{
    if (ms_pBlockAllocator == NULL)
    {
        ms_pBlockAllocator = MX_NEW(CBlockAllocator)(sizeof(_Type), pAllocator);
        ms_uAllocatedCount = 0;
        ms_uCapacity = uInitialCapacity;
        ms_uLockCapacity = bInitialCapacityLocked ? 1 : 0;
        ms_pmutex = bEnableThreadProtection ? MX_NEW(CMutex) : NULL;

        if (ms_uCapacity)
        {
            ms_pBlockAllocator->Reserve(ms_uCapacity);
        }
    }
}

//==SDOC========================================================================
//==
//==  Uninitialize
//==
//==============================================================================
//
//  Description:
//      Uninitializes the pool. This method must be called once per class that
//      inherits from CPool once the objects are no longer required.
//
//==EDOC========================================================================
template<class _Type>
void CPool<_Type>::Uninitialize()
{
    if (ms_pBlockAllocator != NULL)
    {
        MX_ASSERT(ms_uAllocatedCount == 0);

        MX_DELETE(ms_pBlockAllocator);
        ms_pBlockAllocator = NULL;
        ms_uAllocatedCount = 0;
        ms_uCapacity = 0;
        ms_uLockCapacity = 0;;
        MX_DELETE(ms_pmutex);
    }
}

//==SDOC========================================================================
//==
//==  New
//==
//==============================================================================
//
//  Returns:
//      The newly allocated element.
//
//  Description:
//      Allocates and constructs a new element by calling its default
//      constructor. This method may return NULL if the pool is empty and the
//      capacity is locked.
//
//==EDOC========================================================================
template<class _Type>
_Type* GO CPool<_Type>::New()
{
    void* pvoid = Allocate();

    if (pvoid)
    {
        new (pvoid) _Type();
    }

    return reinterpret_cast<_Type*>(pvoid);
}

//==SDOC========================================================================
//==
//==  New
//==
//==============================================================================
//
//  Parameters:
//    rTypeFrom:
//      A reference to the element to be copied into the new element.
//
//  Returns:
//      The newly allocated element.
//
//  Description:
//      Allocates and constructs a new element by calling its copy constructor.
//      This method may return NULL if the pool is empty and the capacity is
//      locked.
//
//==EDOC========================================================================
template<class _Type>
_Type* GO CPool<_Type>::New(IN _Type& rTypeFrom)
{
    void* pvoid = Allocate();

    if (pvoid)
    {
        new (pvoid) _Type(rTypeFrom);
    }

    return reinterpret_cast<_Type*>(pvoid);
}

//==SDOC========================================================================
//==
//==  Delete
//==
//==============================================================================
//
//  Parameters:
//    pType:
//      The element to delete.
//
//  Description:
//      Deletes an element and returns it to the Pool.
//
//==EDOC========================================================================
template<class _Type>
void CPool<_Type>::Delete(TOA IN _Type* pType)
{
    if (pType != NULL)
    {
        pType->~_Type();
        Deallocate(pType);
    }
}

//==SDOC========================================================================
//==
//==  Allocate
//==
//==============================================================================
//
//  Returns:
//      The newly allocated element.
//
//  Description:
//      Allocates a new element without calling its constructor. The placement
//      new operator should probably be called by using the returned element.
//
//==EDOC========================================================================
template<class _Type>
void* CPool<_Type>::Allocate()
{
    void* pvoid = NULL;

    if (ms_pmutex)
    {
        ms_pmutex->Lock();
    }
    /**/
    /**/    MX_ASSERT(ms_pBlockAllocator != NULL);
    /**/
    /**/    if (ms_uAllocatedCount < ms_uCapacity)
    /**/    {
    /**/        pvoid = GO ms_pBlockAllocator->Pop();
    /**/        ms_uAllocatedCount++;
    /**/    }
    /**/    // If we are or will be overallocated, we must also update the
    /**/    // capacity. This is only possible if the capacity is unlocked.
    /**/    else if (ms_uLockCapacity == 0)
    /**/    {
    /**/        pvoid = GO ms_pBlockAllocator->Pop();
    /**/        ms_uAllocatedCount++;
    /**/        ms_uCapacity++;
    /**/    }
    /**/
    if (ms_pmutex)
    {
        ms_pmutex->Unlock();
    }

    if (pvoid == NULL)
    {
        MX_TRACE4(0,
                  g_stFrameworkBasic,
                  "CPool()::Allocate-Pool is empty.");
    }

    return pvoid;
}

//==SDOC========================================================================
//==
//==  Deallocate
//==
//==============================================================================
//
//  Parameters:
//      pvoid:
//       Pointer to the element to be deallocated.
//
//  Description:
//      Deallocates an element.
//
//==EDOC========================================================================
template<class _Type>
void CPool<_Type>::Deallocate(TOA IN void* pvoid)
{
    if (ms_pmutex)
    {
        ms_pmutex->Lock();
    }
    /**/
    /**/    MX_ASSERT(ms_pBlockAllocator != NULL);
    /**/    MX_ASSERT(ms_uAllocatedCount != 0);
    /**/
    /**/    GO ms_pBlockAllocator->Push(GO pvoid);
    /**/
    /**/    // If we are overallocated, we just try to respect the capacity
    /**/    // by reducing the size of the block allocator to 0.
    /**/    // We will then be one element closer to the expected capacity.
    /**/    // This is possible when ReduceCapacity is called with a
    /**/    // uDownToCapacity that is smaller than the number of allocated
    /**/    // element.
    /**/    if (ms_uAllocatedCount > ms_uCapacity)
    /**/    {
    /**/        ms_pBlockAllocator->Reduce(0);
    /**/    }
    /**/
    /**/    ms_uAllocatedCount--;
    /**/
    if (ms_pmutex)
    {
        ms_pmutex->Unlock();
    }
}

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
//      Returns the capacity. The capacity is the number of objects already
//      allocated that may or may not be already in use.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CPool<_Type>::GetCapacity()
{
    unsigned int uCapacity = 0;

    if (ms_pmutex)
    {
        ms_pmutex->Lock();
    }
    /**/
    /**/    uCapacity = ms_uCapacity;
    /**/
    if (ms_pmutex)
    {
        ms_pmutex->Unlock();
    }

    return uCapacity;
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
//      Returns the lock capacity count. The lock capacity count is a counter
//      that is increased each time LockCapacity is called and decreased each
//      time UnlockCapacity is called. If the lock capacity count is greater
//      than zero, this means that any call to ReduceCapacity or ReserveCapacity
//      fails.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CPool<_Type>::GetLockCapacity()
{
    unsigned int uLockCapacity = 0;

    if (ms_pmutex)
    {
        ms_pmutex->Lock();
    }
    /**/
    /**/    uLockCapacity = ms_uLockCapacity;
    /**/
    if (ms_pmutex)
    {
        ms_pmutex->Unlock();
    }

    return uLockCapacity;
}

//==SDOC========================================================================
//==
//==  LockCapacity
//==
//==============================================================================
//
//  Description:
//      Locks the capacity. The lock capacity count is a counter that is
//      increased each time LockCapacity is called and decreased each time
//      UnlockCapacity is called. If the lock capacity count is greater than
//      zero, this means that any call to ReduceCapacity or ReserveCapacity
//      fails.
//
//==EDOC========================================================================
template<class _Type>
void CPool<_Type>::LockCapacity()
{
    if (ms_pmutex)
    {
        ms_pmutex->Lock();
    }
    /**/
    /**/     ms_uLockCapacity++;
    /**/
    if (ms_pmutex)
    {
        ms_pmutex->Unlock();
    }
}

//==SDOC========================================================================
//==
//==  ReduceCapacity
//==
//==============================================================================
//
//  Parameters:
//    uDownToCapacity:
//      The requested capacity.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_STATE
//
//  Description:
//      Reduces the capacity. The capacity is reduced to uDownToCapacity if
//      uDownToCapacity is less than the current capacity. If there are more
//      objects currently in use (a new was called on them) than the requested
//      capacity, the capacity is reduced but the objects remain in the pool.
//      The free capacity is 0 until there are enough objects deleted from the
//      pool. The method fails if the lock capacity count is not 0.
//
//==EDOC========================================================================
template<class _Type>
mxt_result CPool<_Type>::ReduceCapacity(IN unsigned int uDownToCapacity)
{
    mxt_result res = resS_OK;

    if (ms_pmutex)
    {
        ms_pmutex->Lock();
    }
    /**/
    /**/    if (ms_uLockCapacity)
    /**/    {
    /**/        res = resFE_INVALID_STATE;
    /**/    }
    /**/    else if (ms_uCapacity > uDownToCapacity)
    /**/    {
    /**/        ms_uCapacity = uDownToCapacity;
    /**/
    /**/        if (ms_uAllocatedCount > ms_uCapacity)
    /**/        {
    /**/            ms_pBlockAllocator->Reduce(0);
    /**/        }
    /**/        else if (ms_uAllocatedCount + ms_pBlockAllocator->GetBlockCount() > ms_uCapacity)
    /**/        {
    /**/            ms_pBlockAllocator->Reduce(ms_uCapacity - ms_uAllocatedCount);
    /**/        }
    /**/    }
    /**/
    if (ms_pmutex)
    {
        ms_pmutex->Unlock();
    }

    return res;
}

//==SDOC========================================================================
//==
//==  ReserveCapacity
//==
//==============================================================================
//
//  Parameters:
//    uUpToCapacity:
//      The requested capacity.
//
//  Returns:
//      resS_OK
//      resFE_INVALID_STATE
//
//  Description:
//      Increases the capacity. The capacity is increased to uUpToCapacity if
//      uUpToCapacity is greater than the current capacity. The method fails if
//      the lock capacity count is not 0 and the wanted capacity is greater than
//      the current capacity.
//
//==EDOC========================================================================
template<class _Type>
mxt_result CPool<_Type>::ReserveCapacity(IN unsigned int uUpToCapacity)
{
    mxt_result res = resS_OK;

    if (ms_pmutex)
    {
        ms_pmutex->Lock();
    }
    /**/
    /**/    if (ms_uLockCapacity && uUpToCapacity > GetCapacity())
    /**/    {
    /**/        res = resFE_INVALID_STATE;
    /**/    }
    /**/    else  if (ms_uCapacity < uUpToCapacity)
    /**/    {
    /**/        ms_uCapacity = uUpToCapacity;
    /**/
    /**/        if (ms_uAllocatedCount + ms_pBlockAllocator->GetBlockCount() < ms_uCapacity)
    /**/        {
    /**/            ms_pBlockAllocator->Reserve(ms_uCapacity - ms_uAllocatedCount);
    /**/        }
    /**/    }
    /**/
    if (ms_pmutex)
    {
        ms_pmutex->Unlock();
    }

    return res;
}

//==SDOC========================================================================
//==
//==  UnlockCapacity
//==
//==============================================================================
//
//  Description:
//      Unlocks the capacity. The lock capacity count is a counter that is
//      increased each time LockCapacity is called and decreased each time
//      UnlockCapacity is called. If the lock capacity count is greater than
//      zero, this means that any call to ReduceCapacity or ReserveCapacity
//      fails.
//
//==EDOC========================================================================
template<class _Type>
void CPool<_Type>::UnlockCapacity()
{
    if (ms_pmutex)
    {
        ms_pmutex->Lock();
    }
    /**/
    /**/    MX_ASSERT(ms_uLockCapacity);
    /**/
    /**/    if (ms_uLockCapacity)
    /**/    {
    /**/        ms_uLockCapacity--;
    /**/    }
    /**/
    if (ms_pmutex)
    {
        ms_pmutex->Unlock();
    }
}

template<class _Type>
mxt_result CPool<_Type>::InitializeCPool()
{
    ms_pBlockAllocator = NULL;
    ms_uAllocatedCount = 0;
    ms_uCapacity = 0;
    ms_uLockCapacity = 0;
    ms_pmutex = NULL;

    return resS_OK;
}

template<class _Type>
void CPool<_Type>::FinalizeCPool()
{
}

template<class _Type>
mxt_result InitializeCPool()
{
    return CPool<_Type>::InitializeCPool();
}

template<class _Type>
void FinalizeCPool()
{
    CPool<_Type>::FinalizeCPool();
}

//===========================================================================
//====  STATIC MEMBERS INITIALIZATION  ======================================
//===========================================================================
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && \
    MXD_COMPILER_GNU_GCC != MXD_COMPILER_GNU_GCC_2_7 && \
    MXD_COMPILER_GNU_GCC != MXD_COMPILER_GNU_GCC_2_96)

    template<class _Type>
    CBlockAllocator* CPool<_Type>::ms_pBlockAllocator = NULL;

    template<class _Type>
    unsigned int CPool<_Type>::ms_uAllocatedCount = 0;

    template<class _Type>
    unsigned int CPool<_Type>::ms_uCapacity = 0;

    template<class _Type>
    unsigned int CPool<_Type>::ms_uLockCapacity = 0;

    template<class _Type>
    CMutex* CPool<_Type>::ms_pmutex = NULL;
#endif

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CPOOL_H

