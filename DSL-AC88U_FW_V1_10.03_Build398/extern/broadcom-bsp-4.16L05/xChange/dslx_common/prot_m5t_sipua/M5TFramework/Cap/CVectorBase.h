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
#ifndef MXG_CVECTORBASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CVECTORBASE_H

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
//== Class: CVectorBase
//========================================
//
// Description:
//   The CVectorBase class implements an untyped array of elements.  It is used
//   by the templated CVector as the base class for array management.
//
//   The CVectorBase class has the following properties:
//       Insert O(1)
//       Erase O(1)
//       Find O(n) (Search/sort algorithms implemented outside of the class
//                  itself)
//
// Location:
//   Cap/CVectorBase.h
//
// See also:
//   CListBase, CVlistBase
//
//==EDOC========================================================================
class CVectorBase
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CVectorBase(IN unsigned int uSizeOfType, IN IAllocator* pAllocator = NULL);
    CVectorBase(IN unsigned int uSizeOfType,
                IN bool bUseVirtual,
                IN IAllocator* pAllocator = NULL);
    CVectorBase(IN const CVectorBase& rVectorBase);
    virtual ~CVectorBase();

    CVectorBase& operator=(IN const CVectorBase& rVectorBase);
    bool operator==(IN const CVectorBase& rVectorBase) const;
    bool operator!=(IN const CVectorBase& rVectorBase) const;
    bool operator<(IN const CVectorBase& rVectorBase) const;

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
                      IN const CVectorBase& rVector);
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
    mxt_result Merge(IN unsigned int uIndex, INOUT CVectorBase& rVectorBase);
    mxt_result Split(IN unsigned int uIndex, OUT CVectorBase& rVectorBase);
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

    static void Copy(INOUT CVectorBase* pDestinationVectorBase,
                     IN unsigned int uDestinationIndex,
                     IN const CVectorBase* pSourceVectorBase,
                     IN unsigned int uSourceIndex,
                     IN unsigned int uCount);
    static void Move(INOUT CVectorBase* pDestinationVectorBase,
                     IN unsigned int uDestinationIndex,
                     INOUT CVectorBase* pSourceVectorBase,
                     IN unsigned int uSourceIndex,
                     IN unsigned int uCount);

    const void* GetAtInCapacity(IN unsigned int uIndex) const;
    void* GetAtInCapacity(IN unsigned int uIndex);
    mxt_result Resize(IN unsigned int uSize);

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

    IAllocator* m_pAllocator;

    unsigned int m_uCapacityOfVector;
    unsigned int m_uLockCapacity;
    unsigned int m_uSizeOfType;
    unsigned int m_uSizeOfVector;

    // Use virtual Construct/ConstructFrom/Destruct method instead of memcpy.
    bool m_bUseVirtual;

    uint8_t* m_pauType;
    uint8_t* m_puVector;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
void CVectorBase::Construct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);
    // BRCM_CUSTOM - [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM - [end] - Suppress warning
}

inline
void CVectorBase::ConstructFrom(OUT void* pvoidDestination,
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
void CVectorBase::Destruct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);
    // BRCM_CUSTOM [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM [end] - Suppress warning
}

inline
unsigned int CVectorBase::GetCapacity() const
{
    return m_uCapacityOfVector;
}

inline
unsigned int CVectorBase::GetLockCapacity() const
{
    return m_uLockCapacity;
}

inline
void CVectorBase::LockCapacity()
{
    m_uLockCapacity++;
}

inline
void CVectorBase::UnlockCapacity()
{
    MX_ASSERT(m_uLockCapacity);

    if (m_uLockCapacity)
    {
        m_uLockCapacity--;
    }
}

inline
unsigned int CVectorBase::GetSize() const
{
    return m_uSizeOfVector;
}

inline
bool CVectorBase::IsEmpty() const
{
    return m_uSizeOfVector == 0;
}

inline
bool CVectorBase::IsFull() const
{
    return(GetSize() == GetCapacity());
}

inline
unsigned int CVectorBase::GetFirstIndex() const
{
    return 0;
}

inline
unsigned int CVectorBase::GetLastIndex() const
{
    return m_uSizeOfVector - 1;
}

inline
unsigned int CVectorBase::GetEndIndex() const
{
    return m_uSizeOfVector;
}

inline
const void* CVectorBase::operator[](IN unsigned int uIndex) const
{
    return GetAt(uIndex);
}

inline
void* CVectorBase::operator[](IN unsigned int uIndex)
{
    return GetAt(uIndex);
}

inline
const void* CVectorBase::GetAt(IN unsigned int uIndex) const
{
    return uIndex < m_uSizeOfVector ? &m_puVector[uIndex * m_uSizeOfType] : NULL;
}

inline
void* CVectorBase::GetAt(IN unsigned int uIndex)
{
    return uIndex < m_uSizeOfVector ? &m_puVector[uIndex * m_uSizeOfType] : NULL;
}


inline
const void* CVectorBase::GetAtInCapacity(IN unsigned int uIndex) const
{
    return uIndex < m_uCapacityOfVector ? &m_puVector[uIndex * m_uSizeOfType] : NULL;
}

inline
void* CVectorBase::GetAtInCapacity(IN unsigned int uIndex)
{
    return uIndex < m_uCapacityOfVector ? &m_puVector[uIndex * m_uSizeOfType] : NULL;
}

inline
mxt_result CVectorBase::Resize(IN unsigned int uSize)
{
    if (uSize > m_uCapacityOfVector)
    {
        return resFE_INVALID_ARGUMENT;
    }
    m_uSizeOfVector = uSize;
    return resS_OK;
}

inline
mxt_result CVectorBase::Insert(IN unsigned int uIndex,
                               IN unsigned int uCount)
{
    return Insert(uIndex, uCount, NULL);
}

inline
void CVectorBase::Erase(IN unsigned int uIndex)
{
    Erase(uIndex, 1);
}

inline
void CVectorBase::EraseAll()
{
    Erase(0, m_uSizeOfVector);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CVECTORBASE_H

//M5T_INTERNAL_USE_END
