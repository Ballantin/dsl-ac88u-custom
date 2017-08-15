//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CVLISTBASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CVLISTBASE_H

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

//-- Data Members
//-----------------
#ifndef MXG_CBLOCKALLOCATOR_H
#include "Cap/CBlockAllocator.h"
#endif
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CVListBase
//========================================
//
// Description:
//   The CVListBase class implements an untyped double linked list.  It is used
//   by the templated CVList as the base class for vector list management.
//
//   The CVListBase class has the following properties:
//       Insert O(1)
//       Erase O(1)
//       Find O(n) (Search/sort algorithms implemented outside of the class
//                  itself)
//
// Location:
//   Cap/CListBase.h
//
// See also:
//   CVectorBase, CVlistBase
//
//==EDOC========================================================================
class CVListBase
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CVListBase(IN unsigned int uSizeOfType, IN IAllocator* pAllocator);
    CVListBase(IN const CVListBase& rVListBase);
protected:
    CVListBase(IN unsigned int uSizeOfType, IN const CBlockAllocator& rBlockAllocator);

public:
    virtual ~CVListBase();

    CVListBase& operator=(IN const CVListBase& rVListBase);

    // Capacity (Max element count).
    unsigned int GetCapacity() const;
    unsigned int GetLockCapacity() const;
    void LockCapacity();
    mxt_result ReduceCapacity(IN unsigned int uDownToCapacity);
    mxt_result ReserveCapacity(IN unsigned int uUpToCapacity);
    void UnlockCapacity();

    // Size (Element count).
    unsigned int GetSize() const;
    bool IsEmpty() const;
    bool IsFull() const;

    // Index operations.
    unsigned int GetFirstIndex() const;
    unsigned int GetLastIndex() const;
    unsigned int GetEndIndex() const;

    // Direct element access.
    const void* operator[](IN unsigned int uIndex) const;
    void* operator[](IN unsigned int uIndex);
    const void* GetAt(IN unsigned int uIndex) const;
    void* GetAt(IN unsigned int uIndex);

    // List operations.
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount);
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount,
                      IN const void* pvoidContent);
    mxt_result Insert(IN unsigned int uIndex,
                      IN const CVListBase& rVListBase);
    mxt_result InsertSorted(IN const void* pvoidElement,
                            IN int (pfnCompare)(IN const void* pvoidOneElement,
                                                IN const void* pvoidOtherElement,
                                                IN mxt_opaque opq),
                            IN mxt_opaque opq);

    void Erase(IN unsigned int uIndex);
    void Erase(IN unsigned int uIndex, IN unsigned int uCount);
    void EraseAll();

    // Advanced operations.
    void* Allocate(IN unsigned int uIndex);
    mxt_result Merge(IN unsigned int uIndex, INOUT CVListBase& rVListBase);
    mxt_result Split(IN unsigned int uIndex, OUT CVListBase& rVListBase);
    void Swap(IN unsigned int uFirstIndex, IN unsigned int uSecondIndex);

    // Search/Sort Methods
    void Sort(IN int (pfnCompare)(IN const void* pOneElement,
                                  IN const void* pOtherElement,
                                  IN  mxt_opaque opq),
              IN mxt_opaque opq);

    unsigned int Find(IN unsigned int uStartIndex,
                      IN const void* pvoidElement,
                      IN int (pfnCompare)(IN const void* pOneElement,
                                          IN const void* pOtherElement,
                                          IN  mxt_opaque opq),
                      IN mxt_opaque opq) const;

    unsigned int FindSorted(IN const void* pvoidElement,
                            IN int (pfnCompare)(IN const void* pOneElement,
                                                IN const void* pOtherElement,
                                                IN  mxt_opaque opq),
                            IN mxt_opaque opq) const;

    unsigned int GetMinElementIndex(IN int (pfnCompare)(IN const void* pvoidOneElement,
                                                        IN const void* pvoidOtherElement,
                                                        IN  mxt_opaque opq),
                                    IN  mxt_opaque opq) const;

    unsigned int GetMaxElementIndex(IN int (pfnCompare)(IN const void* pvoidOneElement,
                                                        IN const void* pvoidOtherElement,
                                                        IN  mxt_opaque opq),
                                    IN  mxt_opaque opq) const;

    // CMap methods
    mxt_result AllocateSorted(IN const void* pvoidElement,
                              IN int (pfnCompare)(IN const void* pvoidOneElement,
                                                  IN const void* pvoidOtherElement,
                                                  IN  mxt_opaque opq),
                              IN  mxt_opaque opq,
                              OUT void** ppAllocatedZone);

// Hidden Methods
//----------------
protected:

    virtual void Construct(OUT void* pvoid);

    virtual void ConstructFrom(OUT void* pvoidDestination,
                               IN const void* pvoidSource);

    virtual void Destruct(OUT void* pvoid);

    static mxt_result Copy(INOUT CVListBase* pDestinationVListBase,
                           IN unsigned int uDestinationIndex,
                           IN const CVListBase* pSourceVListBase,
                           IN unsigned int uSourceIndex,
                           IN unsigned int uCount);
    static mxt_result Move(INOUT CVListBase* pDestinationVListBase,
                           IN unsigned int uDestinationIndex,
                           INOUT CVListBase* pSourceVListBase,
                           IN unsigned int uSourceIndex,
                           IN unsigned int uCount);
private:

    unsigned int InternalFindSorted(IN const void* pvoidElement,
                                    IN int (pfnCompare)(IN const void* pOneElement,
                                                        IN const void* pOtherElement,
                                                        IN  mxt_opaque opq),
                                    IN mxt_opaque opq,
                                    OUT bool& rbFound) const;

// Hidden Data Members
//---------------------
protected:

    CBlockAllocator m_blockAllocator;

    unsigned int m_uSizeOfType;

    CVector<void*> m_vecpvoidElements;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
void CVListBase::Construct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);
    // BRCM_CUSTOM - [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM - [end] - Suppress warning
}

inline
void CVListBase::ConstructFrom(OUT void* pvoidDestination,
                               IN const void* pvoidSource)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoidDestination);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoidSource);
    // BRCM_CUSTOM - [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM - [end] - Suppress warning
}

inline
void CVListBase::Destruct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);
    // BRCM_CUSTOM - [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM [end] - Suppress warning
}

inline
unsigned int CVListBase::GetCapacity() const
{
    return m_vecpvoidElements.GetCapacity();
}

inline
unsigned int CVListBase::GetLockCapacity() const
{
    return m_vecpvoidElements.GetLockCapacity();
}

inline
void CVListBase::LockCapacity()
{
    m_vecpvoidElements.LockCapacity();
}

inline
void CVListBase::UnlockCapacity()
{
    m_vecpvoidElements.UnlockCapacity();
}

inline
unsigned int CVListBase::GetSize() const
{
    return m_vecpvoidElements.GetSize();
}

inline
bool CVListBase::IsEmpty() const
{
    return m_vecpvoidElements.IsEmpty();
}

inline
bool CVListBase::IsFull() const
{
    return(GetSize() == GetCapacity());
}

inline
unsigned int CVListBase::GetFirstIndex() const
{
    return 0;
}

inline
unsigned int CVListBase::GetLastIndex() const
{
    return GetSize() - 1;
}

inline
unsigned int CVListBase::GetEndIndex() const
{
    return GetSize();
}

inline
const void* CVListBase::operator[](IN unsigned int uIndex) const
{
    return GetAt(uIndex);
}

inline
void* CVListBase::operator[](IN unsigned int uIndex)
{
    return GetAt(uIndex);
}

inline
const void* CVListBase::GetAt(IN unsigned int uIndex) const
{
    return m_vecpvoidElements.GetAt(uIndex);
}

inline
void* CVListBase::GetAt(IN unsigned int uIndex)
{
    return m_vecpvoidElements.GetAt(uIndex);
}

inline
mxt_result CVListBase::Insert(IN unsigned int uIndex,
                              IN unsigned int uCount)
{
    return Insert(uIndex, uCount, NULL);
}

inline
void CVListBase::Erase(IN unsigned int uIndex)
{
    Erase(uIndex, 1);
}

inline
void CVListBase::EraseAll()
{
    Erase(0, GetSize());
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_END

#endif // MXG_CVLISTBASE_H
