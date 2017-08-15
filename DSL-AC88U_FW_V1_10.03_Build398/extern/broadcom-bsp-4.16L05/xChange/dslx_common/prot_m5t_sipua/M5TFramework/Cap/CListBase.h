//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CLISTBASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CLISTBASE_H

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

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IALLOCATOR_H
#include "Cap/IAllocator.h"
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
//== Class: CListBase
//========================================
//
// Description:
//   The CListBase class implements an untyped double linked list. It is used by
//   the templated CList as its base class for list management.
//
//   The CListBase class has the following properties:
//       Insert O(c)
//       Erase O(c)
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
class CListBase
{
protected:

    struct SElement
    {
        SElement* pstPreviousElement;
        SElement* pstNextElement;
    };

// Published Struct
//------------------
public:

    struct SPtr
    {
        SElement* pstElement;
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CListBase(IN unsigned int uSizeOfType, IN IAllocator* pAllocator);
    CListBase(IN const CListBase& rListBase);
protected:
    CListBase(IN unsigned int uSizeOfType, IN const CBlockAllocator& rBlockAllocator);

public:
    virtual ~CListBase();

    CListBase& operator=(IN const CListBase& rListBase);

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
                      IN const CListBase& rListBase);
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
    mxt_result Merge(IN unsigned int uIndex, INOUT CListBase& rListBase);
    mxt_result Split(IN unsigned int uIndex, OUT CListBase& rVistBase);
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

    // List specific methods.
    // WARNING: Will be replaced by an iterator later when it is coded.
    //------------------------------------------------------------------
    void Erase(IN TOA SPtr st);
    const void* GetAt(IN SPtr st) const;
    void* GetAt(IN SPtr st);
    SPtr GetPointer(IN unsigned int uIndex) const;
    void MoveToFirst(IN SPtr st);
    void MoveToLast(IN SPtr st);

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

    SElement* Find(IN unsigned int uIndex) const;

    virtual void Construct(OUT void* pvoid);

    virtual void ConstructFrom(OUT void* pvoidDestination,
                               IN const void* pvoidSource);

    virtual void Destruct(OUT void* pvoid);

    static mxt_result Copy(INOUT CListBase* pDestinationListBase,
                           IN unsigned int uDestinationIndex,
                           IN const CListBase* pSourceListBase,
                           IN unsigned int uSourceIndex,
                           IN unsigned int uCount);
    static mxt_result Move(INOUT CListBase* pDestinationListBase,
                           IN unsigned int uDestinationIndex,
                           INOUT CListBase* pSourceListBase,
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

    unsigned int m_uLockCapacity;
    unsigned int m_uSizeOfList;
    unsigned int m_uSizeOfType;

    mutable unsigned int m_uCurrentIndex;

    mutable SElement* m_pstCurrentElement;
    SElement* m_pstFirstElement;
    SElement* m_pstLastElement;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
void CListBase::Construct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);
    // BRCM_CUSTOM - [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM - [end] - Suppress warning
}

inline
void CListBase::ConstructFrom(OUT void* pvoidDestination,
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
void CListBase::Destruct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);
    // BRCM_CUSTOM - [begin] - Suppress warning
    int value = 0;
    MX_ASSERT(value);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(value);
    // BRCM_CUSTOM - [end] - Suppress warning
}

inline
unsigned int CListBase::GetCapacity() const
{
    return m_uSizeOfList + m_blockAllocator.GetBlockCount();
}

inline
unsigned int CListBase::GetLockCapacity() const
{
    return m_uLockCapacity;
}

inline
void CListBase::LockCapacity()
{
    m_uLockCapacity++;
}

inline
void CListBase::UnlockCapacity()
{
    MX_ASSERT(m_uLockCapacity);

    if (m_uLockCapacity)
    {
        m_uLockCapacity--;
    }
}

inline
unsigned int CListBase::GetSize() const
{
    return m_uSizeOfList;
}

inline
bool CListBase::IsEmpty() const
{
    return m_uSizeOfList == 0;
}

inline
bool CListBase::IsFull() const
{
    return(GetSize() == GetCapacity());
}

inline
unsigned int CListBase::GetFirstIndex() const
{
    return 0;
}

inline
unsigned int CListBase::GetLastIndex() const
{
    return m_uSizeOfList - 1;
}

inline
unsigned int CListBase::GetEndIndex() const
{
    return m_uSizeOfList;
}

inline
const void* CListBase::operator[](IN unsigned int uIndex) const
{
    SElement* pstElement = Find(uIndex);
    return pstElement ? pstElement + 1 : NULL;
}

inline
void* CListBase::operator[](IN unsigned int uIndex)
{
    SElement* pstElement = Find(uIndex);
    return pstElement ? pstElement + 1 : NULL;
}

inline
const void* CListBase::GetAt(IN unsigned int uIndex) const
{
    SElement* pstElement = Find(uIndex);
    return pstElement ? pstElement + 1 : NULL;
}

inline
void* CListBase::GetAt(IN unsigned int uIndex)
{
    SElement* pstElement = Find(uIndex);
    return pstElement ? pstElement + 1 : NULL;
}

inline
mxt_result CListBase::Insert(IN unsigned int uIndex,
                             IN unsigned int uCount)
{
    return Insert(uIndex, uCount, NULL);
}

inline
void CListBase::Erase(IN unsigned int uIndex)
{
    Erase(uIndex, 1);
}

inline
void CListBase::EraseAll()
{
    Erase(0, m_uSizeOfList);
}

inline
const void* CListBase::GetAt(IN CListBase::SPtr st) const
{
    return st.pstElement + 1;
}

inline
void* CListBase::GetAt(IN CListBase::SPtr st)
{
    return st.pstElement + 1;
}

inline
CListBase::SPtr CListBase::GetPointer(IN unsigned int uIndex) const
{
    SPtr st = { Find(uIndex) };
    return st;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CLISTBASE_H

//M5T_INTERNAL_USE_END

