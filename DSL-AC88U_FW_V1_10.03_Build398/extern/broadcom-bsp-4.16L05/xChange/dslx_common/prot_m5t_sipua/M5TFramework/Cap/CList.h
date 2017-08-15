//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CLIST_H
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

//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CListBASE_H
#include "Cap/CListBase.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAllocator;

template <class _Type, class _Container>
class CQueue;

template <class _Type, class _Container>
class CStack;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CList
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//  The CList class implements a double linked list of typed elements.
//
// Description:
//   The CList class implements a double linked list of typed elements.
//
//   The CList class has the following properties:
//     - Insert O(n)
//     - Erase O(n)
//     - Find O(n) (Search/sort algorithms implemented outside of the class
//       itself)
//
//   This template class implements a standard double linked list.
//
//   The CList class is a sequenced container that arranges elements of a given
//   type in a series of nodes that are chained together. Each node also
//   contains an element. Changing the linkage of a node does not change the
//   physical address of the associated element. An internal iterator is kept
//   to optimize the node research.
//
//   In summary, the CList:
//
//      - Offers sequential-access with an index.
//      - Manages its own memory buffer.
//      - Auto increases its capacity when needed.
//      - Reduces its capacity when requested.
//      - Keeps track of how many objects are currently stored.
//      - Calls element's construct automatically at construction.
//      - Calls element's destructor automatically at destruction.
//
// #When to choose CList:#
//
//   It is important to choose your container with care. CList is designed to
//   be efficient at insertion and deletion from the extremities. It does not
//   support random-access. Elements are accessed sequentially.
//
//   CList, CVector, and CVList offer the programmer different complexity
//   trade-offs and should be used accordingly. CVector is the type of sequence
//   that should be used by default, CVList should be used when there are
//   frequent insertions and deletions from the middle of the sequences, and
//   CList is the data structure of choice when most insertions and deletions
//   take place at the end of the sequence.
//
//   Choose CList when:
//      - You need to be able to insert a new element at an arbitrary position.
//      - Your elements need to be ordered in the container.
//      - You need to access each element through its index.
//      - It is important to avoid movement of existing contained elements when
//        insertions/deletions take place (see the WARNING section below).
//      - You want fast insertions/deletions at extremities of the container.
//
//   Avoid CList when:
//      - You need fast random-access with constant search time.
//      - Your internal data must be layout-compatible with C.
//
// #Reminders on how to use CList:#
//
//      1. When using a CList of pointers, do not forget to delete the pointers
//         before the container is destroyed. Otherwise you will produce memory
//         leaks because the CList only destroys the pointers and not what
//         they point to. Before deleting a CList of pointers, you should
//         always write something like this:
//
//            <CODE>
//
//                unsigned int uIndex;
//                unsigned int uSize = plist->GetSize();
//                for (uIndex = 0; uIndex < uSize; uIndex++)
//                {
//                    MX_DELETE((*plist)[uIndex]);
//                }
//                MX_DELETE(plist);
//                plist = NULL;
//
//             </CODE>
//
//      2. Use the method ReserveCapacity to avoid unnecessary reallocations.
//         If you neglect to do so, each new insertion on a list that is too
//         small makes it grow by 1 element, added to the unavoidable overhead.
//
//      3. Use the method GetCapacity to know exactly the number of objects
//         the list can store without reallocation.
//
//      4. Use the method GetSize to know the number of currently stored
//         elements.
//
//      5. When iterating by index and using the Erase method, you have to
//         make sure that the index is valid for the whole iteration scope.
//         Since the Erase method affects the size of the container, the
//         operator[] or the GetAt method should be used with caution. A
//         good practice is to use the GetSize method within the iteration
//         scope such as the following example:
//
//          <CODE>
//              CList<SomeStruct> lstSomeStruct;
//              unsigned int i = 0;
//              for ( i = 0 ; i < lstSomeStruct.GetSize(); i++ )
//              {
//                  if ( lstSomeStruct[i].bToErase )
//                  {
//                      lstSomeStruct.Erase(i);
//                      i--;
//                  }
//              }
//          </CODE>
//
//          Another good practice is to iterate from the end such as the
//          following example:
//
//          <CODE>
//              CList<SomeStruct> lstSomeStruct;
//              unsigned int i = 0;
//              for ( i = lstSomeStruct.GetSize(); i > 0; i-- )
//              {
//                  if ( lstSomeStruct[i-1].bToErase )
//                  {
//                      lstSomeStruct.Erase(i-1);
//                  }
//              }
//          </CODE>
//
//      6. CList fully supports incomparable types such as structures or complex
//         objects. However, the sort methods (InsertSorted, Sort, Find,
//         FindSorted, GetMinElementIndex, and GetMaxElementIndex) can not be
//         used except if the structure or the complex object implements the !=
//         and < operators. Alternatively, you can use the CUncmp template
//         helper and provide your own comparison methods. Please refer to the
//         CUncmp class documentation for more information and code examples.
//
// Warning:
//  This container is not thread safe.
//
// Location:
//   Cap/CList.h
//
// See also:
//   CVector, CList
//
//==EDOC========================================================================
template<class _Type>
class CList : protected CListBase
{
//M5T_INTERNAL_USE_BEGIN
    friend class CQueue<_Type, CList<_Type> >;
    friend class CStack<_Type, CList<_Type> >;
//M5T_INTERNAL_USE_END

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //     Constructor.
    //------------------
    CList(IN IAllocator* pAllocator = NULL);

    // Summary:
    //      Copy Constructor.
    //------------------------
    CList(IN const CList<_Type>& rList);

    // Summary:
    //     Destructor.
    //-----------------
    virtual ~CList();

    // Summary:
    //     Assignment operator.
    //--------------------------
    CList<_Type>& operator=(IN const CList<_Type>& rList);

    // Summary:
    //     Comparison operator.
    //--------------------------
    bool operator==(IN const CList<_Type>& rList) const;

    // Summary:
    //     Different than operator.
    //-------------------------------
    bool operator!=(IN const CList<_Type>& rList) const;

    // Summary:
    //     Less than operator.
    //-------------------------
    bool operator<(IN const CList<_Type>& rList) const;

    // Summary:
    //     Greater than operator.
    //----------------------------
    bool operator>(IN const CList<_Type>& rList) const;

    // Summary:
    //     Less than or equal to operator.
    //-------------------------------------
    bool operator<=(IN const CList<_Type>& rList) const;

    // Summary:
    //     Greater then or equal to.
    //-------------------------------
    bool operator>=(IN const CList<_Type>& rList) const;

    //  Summary:
    //      Sets the default comparison function for this container.
    //---------------------------------------------------------------
   mxt_result SetComparisonFunction(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                        IN const _Type& rOtherElement,
                                                        IN  mxt_opaque opq),
                                    IN  mxt_opaque opq);

    // Capacity (Max element count).
    //-------------------------------

    //  Summary:
    //      Returns the capacity.
    //----------------------------
    unsigned int GetCapacity() const;

    //  Summary:
    //      Returns the local capacity count.
    //----------------------------------------
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

    //  Summary:
    //      Gets the size.
    //---------------------
    unsigned int GetSize() const;

    //  Summary:
    //      Checks if the list is empty.
    //-----------------------------------
    bool IsEmpty() const;

    //  Summary:
    //      Returns if the list is full.
    //-----------------------------------
    bool IsFull() const;

    //  Summary:
    //      Gets the first index of the list.
    //----------------------------------------
    unsigned int GetFirstIndex() const;

    //  Summary:
    //      Gets the last used index.
    //--------------------------------
    unsigned int GetLastIndex() const;

    //  Summary:
    //      Gets the end index.
    //--------------------------
    unsigned int GetEndIndex() const;

    // Direct element access.
    //------------------------

    //  Summary:
    //      Gets the element at position x.
    //--------------------------------------
    const _Type& operator[](IN unsigned int uIndex) const;

    //<COMBINE CList::[]@IN unsigned int@const>
    _Type& operator[](IN unsigned int uIndex);

    //  Summary:
    //      Gets the element at position x.
    //--------------------------------------
    const _Type& GetAt(IN unsigned int uIndex) const;

    //<COMBINE CList::GetAt@IN unsigned int@const>
    _Type& GetAt(IN unsigned int uIndex);

    // List operations.
    //------------------

    //  Summary:
    //      Appends one element at the end index.
    //--------------------------------------------
    mxt_result Append(IN const _Type& rElement);

    //  Summary:
    //      Inserts one or more elements.
    //------------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount);

    //  Summary:
    //      Inserts one or more elements.
    //------------------------------------
    mxt_result Insert(IN unsigned int uIndex,
                      IN unsigned int uCount,
                      IN const _Type& rElement);

    //  Summary:
    //      Inserts the elements contained within another list.
    //----------------------------------------------------------
    mxt_result Insert(IN unsigned int uIndex, IN const CList& rList);

    //  Summary:
    //      Inserts one element, leaving the container sorted.
    //---------------------------------------------------------
    mxt_result InsertSorted(IN const _Type& rElement,
                            IN int (pfnCompare)(IN const _Type& rOneElement,
                                                IN const _Type& rOtherElement,
                                                IN mxt_opaque opq) = NULL,
                            IN mxt_opaque opq = 0);

    //  Summary:
    //      Erases one element.
    //--------------------------
    void Erase(IN unsigned int uIndex);

    //  Summary:
    //      Erases multiple elements.
    //--------------------------
    void Erase(IN unsigned int uIndex, IN unsigned int uCount);

    //  Summary:
    //      Erases all elements.
    //---------------------------
    void EraseAll();

    // Advanced operations.
    //----------------------

    //  Summary:
    //      Inserts one element but does not construct it.
    //-----------------------------------------------------
    void* Allocate(IN unsigned int uIndex);

    //  Summary:
    //      Moves all the elements of another list to a specific index.
    //------------------------------------------------------------------
    mxt_result Merge(IN unsigned int uIndex, INOUT CList& rList);

    //  Summary:
    //      Moves some elements from this list to another starting from a
    //      specific index.
    //--------------------------------------------------------------------
    mxt_result Split(IN unsigned int uIndex, OUT CList& rList);

    //  Summary:
    //      Swaps two elements.
    //--------------------------
    void Swap(IN unsigned int uFirstIndex, IN unsigned int uSecondIndex);

    //  Summary:
    //      Sorts the list.
    //----------------------
    void Sort(IN int (pfnCompare)(IN const _Type& rOneElement,
                                  IN const _Type& rOtherElement,
                                  IN  mxt_opaque opq) = NULL,
              IN  mxt_opaque opq = 0);

    //  Summary:
    //      Finds an element in the list.
    //------------------------------------
    unsigned int Find(IN unsigned int uStartIndex,
                      IN const _Type& rElement,
                      IN int (pfnCompare)(IN const _Type& rOneElement,
                                          IN const _Type& rOtherElement,
                                          IN  mxt_opaque opq) = NULL,
                      IN  mxt_opaque opq = 0) const;

    //  Summary:
    //      Finds an element in a sorted list.
    //-----------------------------------------
    unsigned int FindSorted(IN const _Type& rElement,
                            IN int (pfnCompare)(IN const _Type& rOneElement,
                                                IN const _Type& rOtherElement,
                                                IN  mxt_opaque opq) = NULL,
                            IN  mxt_opaque opq = 0) const;

    //  Summary:
    //      Gets the "smallest" element.
    //-----------------------------------
    unsigned int GetMinElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                        IN const _Type& rOtherElement,
                                                        IN  mxt_opaque opq) = NULL,
                                    IN  mxt_opaque opq = 0) const;

    //  Summary:
    //      Gets the "biggest" element.
    //----------------------------------
    unsigned int GetMaxElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                        IN const _Type& rOtherElement,
                                                        IN  mxt_opaque opq) = NULL,
                                    IN  mxt_opaque opq = 0) const;

    //M5T_INTERNAL_USE_BEGIN
    // List specific methods.
    // WARNING: Will be replaced by an iterator later when it is coded.
    //------------------------------------------------------------------
    void Erase(IN TOA SPtr st);
    const _Type& GetAt(IN SPtr st) const;
    _Type& GetAt(IN SPtr st);
    SPtr GetPointer(IN unsigned int uIndex) const;
    void MoveToFirst(IN SPtr st);
    void MoveToLast(IN SPtr st);
    //M5T_INTERNAL_USE_END


    // CMap operations.  Do not use outside the CMap class.
    //-----------------------------------------------------

    //  Summary:
    //      Allocates a memory zone in the proper location in the list.
    //------------------------------------------------------------------
    mxt_result AllocateSorted(IN const _Type& rElement, OUT void** ppAllocatedZone);

    //  Summary:
    //      This method erases an element in a sorted container.
    //-----------------------------------------------------------
    void EraseSorted(IN const _Type& rElement);

    //  Summary:
    //      Finds an element in a sorted container.
    //----------------------------------------------
    const _Type* FindPtrSorted(IN const _Type& rElement) const;

    //<COMBINE CList::FindPtrSorted@IN const _Type&@const>
    _Type* FindPtrSorted(IN const _Type& rElement);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // Stack operations.
    //-------------------

    // Summary:
    //     Retrieves the element at the top without erasing it.
    //----------------------------------------------------------
    const _Type& GetTop() const;

    // Summary:
    //     Retrieves the element at the top without erasing it.
    //----------------------------------------------------------
    _Type& GetTop();

    // Summary:
    //     Erases the element at the top.
    //------------------------------------
    void Pop();

    // Summary:
    //     Inserts an element at the top.
    //-------------------------------------
    mxt_result Push(IN const _Type& rElement);

    // Queue operations.
    //-------------------

    // Summary:
    //     Retrieves the element at the queue's first index without erasing it.
    //--------------------------------------------------------------------------
    const _Type& GetFront() const;

    // Summary:
    //     Retrieves the element at the queue's first index without erasing it.
    //--------------------------------------------------------------------------
    _Type& GetFront();

    // Summary:
    //     Erases the element at the queue's first index.
    //----------------------------------------------------
    void Dequeue();

    // Summary:
    //     Inserts the element at the queue's last index.
    //----------------------------------------------------
    mxt_result Enqueue(IN const _Type& rElement);

    virtual void Construct(OUT void* pvoid);
    virtual void ConstructFrom(OUT void* pvoidDestination,
                               IN const void* pvoidSource);
    virtual void Destruct(OUT void* pvoid);
    static int Compare(IN const void* pOneElement,
                       IN const void* pOtherElement,
                       IN mxt_opaque opq);

// Hidden Members
//----------------
protected:

    struct SCompareParams
    {
        int (*m_pfnCompare)(IN const _Type& rOneElement,
                            IN const _Type& rOtherElement,
                            IN  mxt_opaque opq);
        mxt_opaque m_opq;
    };

    SCompareParams m_stCompareParams;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  CList
//==
//==============================================================================
//
//  Parameters:
//    pAllocator:
//      Allows to specify an allocator used to allocate and free the elements.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CList<_Type>::CList(IN IAllocator* pAllocator)
: CListBase(sizeof(_Type), pAllocator)
{
    m_stCompareParams.m_pfnCompare = NULL;
    m_stCompareParams.m_opq = MX_INT32_TO_OPQ(0);
}

//==SDOC========================================================================
//==
//==  CList
//==
//==============================================================================
//
//  Parameters:
//    rList:
//      A reference to the source list.
//
//  Description:
//      Copy Constructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CList<_Type>::CList(IN const CList<_Type>& rList)
: CListBase(rList.m_uSizeOfType, rList.m_blockAllocator)
{
    *this = rList;
}

//==SDOC========================================================================
//==
//==  ~CList
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CList<_Type>::~CList()
{
    EraseAll();
}

//==SDOC========================================================================
//==
//==  operator=
//==
//==============================================================================
//
//  Parameters:
//    rList:
//      Reference to the list to copy.
//  Returns:
//      A CList identical to the referenced one.
//
//  Description:
//      Creates a copy of the referenced CList.
//
//==EDOC========================================================================
template<class _Type>
inline
CList<_Type>& CList<_Type>::operator=(IN const CList<_Type>& rList)
{
    CListBase::operator=(rList);
    m_stCompareParams.m_pfnCompare = rList.m_stCompareParams.m_pfnCompare;
    m_stCompareParams.m_opq = rList.m_stCompareParams.m_opq;
    return *this;
}

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    rList:
//      Reference to the list to compare.
//  Returns:
//      Returns true if both lists are identical, otherwise returns false.
//
//  Description:
//      Verifies that the left hand list is identical to the right hand list.
//      This is done by comparing containers element by element and returns once
//      an element is different from the other. This behaviour is identical to
//      the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CList<_Type>::operator==(IN const CList<_Type>& rList) const
{
    return !(*this != rList);
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rList:
//      Reference to the list to compare.
//  Returns:
//      Returns true if both lists are different, otherwise returns false.
//
//  Description:
//      Verifies that the left hand list is different than the right hand list.
//      This is done by comparing containers element by element and returns once
//      an element is different from the other. This behaviour is identical to
//      the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
bool CList<_Type>::operator!=(IN const CList<_Type>& rList) const
{
    if(&rList == this)
    {
        return false;
    }

    if (m_uSizeOfList != rList.m_uSizeOfList)
    {
        return true;
    }

    unsigned int uIndex = 0;
    bool bNotEqual = false;

    while (uIndex < m_uSizeOfList && bNotEqual == false)
    {
        if (GetAt(uIndex) != rList.GetAt(uIndex))
        {
            bNotEqual = true;
        }

        uIndex++;
    }

    return bNotEqual;
}

//==SDOC========================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//    rList:
//      Reference to the list to compare.
//  Returns:
//      Returns true if the left hand list is smaller than the right hand list,
//      otherwise returns false.
//
//  Description:
//      Verifies if the left hand list is less than the right hand list. This is
//      done by comparing containers element by element and returns once an
//      element is different from the other. This behaviour is identical to the
//      one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
bool CList<_Type>::operator<(IN const CList<_Type>& rList) const
{
    if (&rList == this)
    {
        return false;
    }

    unsigned int uIndex = 0;
    bool bLessThan = false;
    unsigned int uSizeOfList = MX_MIN(GetSize(), rList.GetSize());

    while (uIndex < uSizeOfList && bLessThan == false)
    {
        if (GetAt(uIndex) < rList.GetAt(uIndex))
        {
            bLessThan = true;
        }

        uIndex++;
    }

    if(bLessThan == false)
    {
        bLessThan = GetSize() < rList.GetSize() ? true : false;
    }
    return bLessThan;
}

//==SDOC========================================================================
//==
//==  operator>
//==
//==============================================================================
//
//  Parameters:
//    rList:
//      Reference to the list to compare.
//  Returns:
//      Returns true if the left hand list is greater than the right hand list,
//      otherwise returns false.
//
//  Description:
//      Verifies if the left hand list is greater than the right hand list. This
//      is done by comparing containers element by element and returns once an
//      element is different from the other. This behaviour is identical to the
//      one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CList<_Type>::operator>(IN const CList<_Type>& rList) const
{
    return (!(*this < rList) && *this != rList);
}

//==SDOC========================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//    rList:
//      Reference to the list to compare.
//  Returns:
//      Returns true if the left hand list is smaller than or equal to the right
//      hand list, otherwise returns false.
//
//  Description:
//      Verifies if the left hand list is less than or equal to the right hand
//      list. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CList<_Type>::operator<=(IN const CList<_Type>& rList) const
{
    return !(*this > rList);
}

//==SDOC========================================================================
//==
//==  operator>=
//==
//==============================================================================
//
//  Parameters:
//    rList:
//      Reference to the list to compare.
//  Returns:
//      Returns true if the left hand list is greater than or equal to the right
//      hand list, otherwise returns false.
//
//  Description:
//      Verifies if the left hand list is greater than or equal to the right
//      hand list. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CList<_Type>::operator>=(IN const CList<_Type>& rList) const
{
    return !(*this < rList);
}

//==SDOC========================================================================
//==
//==  SetComparisonFunction
//==
//==============================================================================
//
//  Parameters:
//      pfnCompare:
//          Pointer to a comparison function.
//      opq:
//          Opaque comparison parameter.
//
//  Returns:
//      resS_OK
//
//  Description:
//      Sets the default comparison function to be used in Search/Sort
//      algorithms for this container instance.
//      The comparison function needs to return one of the following values:
//          < 0 if rOneElement is "smaller" than rOtherElement.
//          > 0 if rOneElement is "bigger" than rOtherElement.
//          0 if rOneElement and rOtherElement are equivalent.
//
//==EDOC========================================================================
template<class _Type>
mxt_result CList<_Type>::SetComparisonFunction(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                             IN const _Type& rOtherElement,
                                                             IN  mxt_opaque opq),
                                         IN  mxt_opaque opq)
{
    m_stCompareParams.m_pfnCompare = pfnCompare;
    m_stCompareParams.m_opq = opq;

    return resS_OK;
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
//      Returns the capacity.
//      The capacity is the number of elements already allocated that may or
//      may not be already in use.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CList<_Type>::GetCapacity() const
{
    return CListBase::GetCapacity();
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
//      Returns the local capacity count.
//      The lock capacity count is a counter that is increased each time
//      LockCapacity is called and decreased each time UnlockCapacity is
//      called. If the lock capacity count is greater than zero, this means
//      that any call to ReduceCapacity or ReserveCapacity fails.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CList<_Type>::GetLockCapacity() const
{
    return CListBase::GetLockCapacity();
}

//==SDOC========================================================================
//==
//==  LockCapacity
//==
//==============================================================================
//
//  Description:
//      Locks the capacity.
//      The lock capacity count is a counter that is increased each time
//      LockCapacity is called and decreased each time UnlockCapacity is
//      called. If the lock capacity count is greater than zero, this means
//      that any call to ReduceCapacity or ReserveCapacity fails.
//
//==EDOC========================================================================
template<class _Type>
inline
void CList<_Type>::LockCapacity()
{
    CListBase::LockCapacity();
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
//      Reduce the capacity.
//      The capacity is reduced to uDownToCapacity if uDownToCapacity is less
//      than the current capacity.
//      The method fails if the lock capacity count is not 0.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::ReduceCapacity(IN unsigned int uDownToCapacity)
{
    return CListBase::ReduceCapacity(uDownToCapacity);
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
//      Increases the capacity.
//      The capacity is increased to uUpToCapacity if uUpToCapacity is greater
//      than the current capacity.
//      The method fails if the lock capacity count is not 0 and the wanted
//      capacity is greater than the current capacity.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::ReserveCapacity(IN unsigned int uUpToCapacity)
{
    return CListBase::ReserveCapacity(uUpToCapacity);
}

//==SDOC========================================================================
//==
//==  UnlockCapacity
//==
//==============================================================================
//
//  Description:
//      Unlocks the capacity.
//      The lock capacity count is a counter that is increased each time
//      LockCapacity is called and decreased each time UnlockCapacity is
//      called. If the lock capacity count is greater than zero, this means
//      that any call to ReduceCapacity or ReserveCapacity fails.
//
//==EDOC========================================================================
template<class _Type>
inline
void CList<_Type>::UnlockCapacity()
{
    CListBase::UnlockCapacity();
}

//==SDOC========================================================================
//==
//==  GetSize
//==
//==============================================================================
//
//  Returns:
//      The size.
//
//  Description:
//      Returns the size.
//      This is the number of elements that are allocated and already in use.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CList<_Type>::GetSize() const
{
    return CListBase::GetSize();
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
//      Returns true if the size is 0; in other words, if no elements are
//      currently in use.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CList<_Type>::IsEmpty() const
{
    return CListBase::IsEmpty();
}

//==SDOC========================================================================
//==
//==  IsFull
//==
//==============================================================================
//
//  Returns:
//      True when the container is full according to its capacity.
//
//  Description:
//      This method returns true when the size of the container equals its
//      capacity, i.e., there's no more room to add new elements without
//      allocating more memory.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CList<_Type>::IsFull() const
{
    return CListBase::IsFull();
}

//==SDOC========================================================================
//==
//==  GetFirstIndex
//==
//==============================================================================
//
//  Returns:
//      The index of the first used element.
//
//  Description:
//      Returns the index of the first used element.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CList<_Type>::GetFirstIndex() const
{
    return CListBase::GetFirstIndex();
}

//==SDOC========================================================================
//==
//==  GetLastIndex
//==
//==============================================================================
//
//  Returns:
//      The index of the last used element.
//
//  Description:
//      Returns the index of the last used element.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CList<_Type>::GetLastIndex() const
{
    return CListBase::GetLastIndex();
}

//==SDOC========================================================================
//==
//==  GetEndIndex
//==
//==============================================================================
//
//  Returns:
//      The index of the first unused element.
//
//  Description:
//      Returns the index of the first unused element.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CList<_Type>::GetEndIndex() const
{
    return CListBase::GetEndIndex();
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex.
//      Returns an invalid element if uIndex is equal to or greater than the
//      size.
//
//==EDOC========================================================================
template<class _Type>
inline
const _Type& CList<_Type>::operator[](IN unsigned int uIndex) const
{
    return *reinterpret_cast<const _Type *>(CListBase::operator[](uIndex));
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex.
//      Returns an invalid element if uIndex is equal to or greater than the
//      size.
//
//==EDOC========================================================================
template<class _Type>
inline
_Type& CList<_Type>::operator[](IN unsigned int uIndex)
{
    return *reinterpret_cast<_Type *>(CListBase::operator[](uIndex));
}

//==SDOC========================================================================
//==
//==  GetAt
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//       Index of an element in the list.
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex.
//      Returns an invalid element if uIndex is equal to or greater than the
//      size.
//
//==EDOC========================================================================
template<class _Type>
inline
const _Type& CList<_Type>::GetAt(IN unsigned int uIndex) const
{
    return *reinterpret_cast<const _Type *>(CListBase::GetAt(uIndex));
}

//==SDOC========================================================================
//==
//==  GetAt
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//       Index of an element in the list.
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex.
//      Returns an invalid element if uIndex is equal to or greater than the
//      size.
//
//==EDOC========================================================================
template<class _Type>
inline
_Type& CList<_Type>::GetAt(IN unsigned int uIndex)
{
    return *reinterpret_cast<_Type *>(CListBase::GetAt(uIndex));
}

//==SDOC========================================================================
//==
//==  Append
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      A reference to an element used to construct a new one in the CList.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts one or more elements and constructs them.
//
//  See also: Insert
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::Append(IN const _Type& rElement)
{
    return Insert(GetEndIndex(), 1, rElement);
}

//==SDOC========================================================================
//==
//==  Insert
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to insert the elements.
//
//    uCount:
//      The number of elements to insert.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts one or more elements and constructs them.
//      The capacity is increased if there are not enough allocated elements
//      to accommodate the insertion of the new elements. The lock capacity
//      count is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::Insert(IN unsigned int uIndex,
                                IN unsigned int uCount)
{
    return CListBase::Insert(uIndex, uCount);
}

//==SDOC========================================================================
//==
//==  Insert
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to insert the elements.
//
//    uCount:
//      The number of elements to insert.
//
//    rElement:
//      A reference to an element used to construct the elements.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts one or more elements and constructs them from rElement.
//      The capacity is increased if there are not enough allocated elements
//      to accommodate the insertion of the new elements. The lock capacity
//      count is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::Insert(IN unsigned int uIndex,
                                IN unsigned int uCount,
                                IN const _Type& rContent)
{
    return CListBase::Insert(uIndex, uCount, &rContent);
}

//==SDOC========================================================================
//==
//==  Insert
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to insert the elements.
//
//    rList:
//      A list that contains the elements to insert.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts the elements contained within another list to a specified
//      index.
//      The capacity is increased if there are not enough allocated elements
//      to accommodate the insertion of the new elements. The lock capacity
//      count is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::Insert(IN unsigned int uIndex,
                                IN const CList& rList)
{
    return CListBase::Insert(uIndex, rList);
}

//==SDOC========================================================================
//==
//==  InsertSorted
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      A reference to an element used to construct the new element.
//
//    pfnCompare:
//      An optional _Type comparison function or NULL.
//
//    opq:
//      An opaque to be passed to the pfnCompare. Ignored if pfnCompare is NULL.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts one element and constructs it from rElement, leaving the
//      container sorted.
//      The capacity is increased if there are not enough allocated elements
//      to accommodate the insertion of the new element. The lock capacity
//      count is still enforced and if it is not zero, the insertion fails.
//      The pfnCompare pointer can be used to change the algorithm behaviour.
//
//==EDOC========================================================================
template<class _Type>
mxt_result CList<_Type>::InsertSorted(IN const _Type& rElement,
                                      IN int (pfnCompare)(IN const _Type& rOneElement,
                                                          IN const _Type& rOtherElement,
                                                          IN mxt_opaque opq),
                                      IN mxt_opaque opq)
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CListBase::InsertSorted(&rElement,
                                       CList<_Type>::Compare,
                                       MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CListBase::InsertSorted(&rElement,
                                       CList<_Type>::Compare,
                                       MX_VOIDPTR_TO_OPQ(&m_stCompareParams));
    }
}

//==SDOC========================================================================
//==
//==  Erase
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the element to erase.
//
//  Description:
//      Erases one element at position uIndex.
//
//==EDOC========================================================================
template<class _Type>
inline
void CList<_Type>::Erase(IN unsigned int uIndex)
{
    CListBase::Erase(uIndex);
}

//==SDOC========================================================================
//==
//==  Erase
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the first element to erase.
//
//    uCount:
//      The number of elements to erase.
//
//  Description:
//      Erases multiple elements, beginning with the element at a specific
//      index.
//
//==EDOC========================================================================
template<class _Type>
inline
void CList<_Type>::Erase(IN unsigned int uIndex, IN unsigned int uCount)
{
    CListBase::Erase(uIndex, uCount);
}

//==SDOC========================================================================
//==
//==  EraseAll
//==
//==============================================================================
//
//  Description:
//      Erases all elements.
//
//==EDOC========================================================================
template<class _Type>
inline
void CList<_Type>::EraseAll()
{
    CListBase::EraseAll();
}

//==SDOC========================================================================
//==
//==  Pop
//==
//==============================================================================
//
//  Description:
//      Erases the element at the top.
//
//==EDOC========================================================================
template<class _Type>
inline
void CList<_Type>::Pop()
{
    Erase(GetLastIndex());
}

//==SDOC========================================================================
//==
//==  Push
//==
//==============================================================================
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts an element at the top.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::Push(IN const _Type& rElement)
{
    return Insert(GetEndIndex(), 1, rElement);
}

//M5T_INTERNAL_USE_BEGIN
//==SDOC========================================================================
//==
//==  GetTop
//==
//==============================================================================
//
//  Returns:
//      The element at the top.
//
//  Description:
//      Retrieves the element at the top without erasing it.
//
//==EDOC========================================================================
template<class _Type>
inline
const _Type& CList<_Type>::GetTop() const
{
    return GetAt(GetLastIndex());
}

//==SDOC========================================================================
//==
//==  GetTop
//==
//==============================================================================
//
//  Returns:
//      The element at the top.
//
//  Description:
//      Retrieves the element at the top without erasing it.
//
//==EDOC========================================================================
template<class _Type>
inline
_Type& CList<_Type>::GetTop()
{
    return GetAt(GetLastIndex());
}

//==SDOC========================================================================
//==
//==  Dequeue
//==
//==============================================================================
//
//  Description:
//      Erases the element at the queue's first index.
//
//==EDOC========================================================================
template<class _Type>
inline
void CList<_Type>::Dequeue()
{
    Erase(GetFirstIndex());
}

//==SDOC========================================================================
//==
//==  Enqueue
//==
//==============================================================================
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts the element at the queue's last index.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::Enqueue(IN const _Type& rElement)
{
    return Insert(GetEndIndex(), 1, rElement);
}

//==SDOC========================================================================
//==
//==  GetFront
//==
//==============================================================================
//
//  Returns:
//      The element at the queue's first index.
//
//  Description:
//      Retrieves the element at the queue's first index without erasing it.
//
//==EDOC========================================================================
template<class _Type>
inline
const _Type& CList<_Type>::GetFront() const
{
    return GetAt(GetFirstIndex());
}

//==SDOC========================================================================
//==
//==  GetFront
//==
//==============================================================================
//
//  Returns:
//      The element at the queue's first index.
//
//  Description:
//      Retrieves the element at the queue's first index without erasing it.
//
//==EDOC========================================================================
template<class _Type>
inline
_Type& CList<_Type>::GetFront()
{
    return GetAt(GetFirstIndex());
}
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//==
//==  Allocate
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to insert the element.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Acts like an insert but does not call the constructor. The user is
//      then free to use the pointer to do whatever it wishes.
//      The capacity is increased if there are not enough allocated elements
//      to accommodate the insertion of the new element. The lock capacity
//      count is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
void* CList<_Type>::Allocate(IN unsigned int uIndex)
{
    return CListBase::Allocate(uIndex);
}

//==SDOC========================================================================
//==
//==  Merge
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to move the elements.
//
//    rList:
//      The source list.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Moves all the elements of another list to a specific index.
//      The capacity is increased if there are not enough allocated elements
//      to accommodate the insertion of the new elements. The lock capacity
//      count is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::Merge(IN unsigned int uIndex, INOUT CList& rList)
{
    return CListBase::Merge(uIndex, rList);
}

//==SDOC========================================================================
//==
//==  Split
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to move the elements.
//
//    rList:
//      The destination list.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Moves some elements from this list to another starting from a specific
//      index. The destination list is emptied first. The capacity of the target
//      list is increased if there are not enough allocated elements to
//      accommodate the insertion of the new elements. The lock capacity count
//      is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::Split(IN unsigned int uIndex, OUT CList& rList)
{
    return CListBase::Split(uIndex, rList);
}

//==SDOC========================================================================
//==
//==  Swap
//==
//==============================================================================
//
//  Parameters:
//    uFirstIndex:
//      The index of the first element to swap.
//
//    uSecondIndex:
//      The index of the second element to swap.
//
//  Description:
//      Exchanges the position of two elements.
//
//==EDOC========================================================================
template<class _Type>
inline
void CList<_Type>::Swap(IN unsigned int uFirstIndex,
                        IN unsigned int uSecondIndex)
{
    CListBase::Swap(uFirstIndex, uSecondIndex);
}

//M5T_INTERNAL_USE_BEGIN
template<class _Type>
inline
void CList<_Type>::Erase(IN TOA SPtr st)
{
    CListBase::Erase(st);
}

template<class _Type>
inline
const _Type& CList<_Type>::GetAt(IN SPtr st) const
{
    return *reinterpret_cast<const _Type*>(CListBase::GetAt(st));
}

template<class _Type>
inline
_Type& CList<_Type>::GetAt(IN SPtr st)
{
    return *reinterpret_cast<_Type*>(CListBase::GetAt(st));
}

template<class _Type>
inline
CListBase::SPtr CList<_Type>::GetPointer(IN unsigned int uIndex) const
{
    return CListBase::GetPointer(uIndex);
}

template<class _Type>
inline
void CList<_Type>::MoveToFirst(IN SPtr st)
{
    CListBase::MoveToFirst(st);
}

template<class _Type>
inline
void CList<_Type>::MoveToLast(IN SPtr st)
{
    CListBase::MoveToLast(st);
}

template<class _Type>
void CList<_Type>::Construct(OUT void* pvoid)
{
    new (pvoid) _Type;
}

template<class _Type>
void CList<_Type>::ConstructFrom(OUT void* pvoidDestination,
                                 IN const void* pvoidSource)
{
    new (pvoidDestination) _Type(*reinterpret_cast<const _Type*>(pvoidSource));
}

template<class _Type>
void CList<_Type>::Destruct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);

    reinterpret_cast<_Type*>(pvoid)->~_Type();
}

template<class _Type>
int CList<_Type>::Compare(IN const void* pOneElement,
                          IN const void* pOtherElement,
                          IN mxt_opaque opq)
{
    int nResult = 0;
    if ((reinterpret_cast<SCompareParams*>(opq))->m_pfnCompare != NULL)
    {
        nResult = reinterpret_cast<SCompareParams*>(opq)->m_pfnCompare(*reinterpret_cast<const _Type*>(pOneElement),
                                                                       *reinterpret_cast<const _Type*>(pOtherElement),
                                                                       reinterpret_cast<SCompareParams*>(opq)->m_opq);
    }
    else if(*reinterpret_cast<const _Type*>(pOneElement) <
            *reinterpret_cast<const _Type*>(pOtherElement))
    {
        nResult = -1;
    }
    else if(*reinterpret_cast<const _Type*>(pOneElement) !=
            *reinterpret_cast<const _Type*>(pOtherElement))
    {
        nResult = 1;
    }

    return nResult;
}
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//==
//==  Sort
//==
//==============================================================================
//
//  Parameters:
//    pfnCompare:
//      An optional _Type comparison function or NULL.
//
//    opq:
//      An opaque to be passed to the pfnCompare. Ignored if pfnCompare is NULL.
//
//  Description:
//      Sorts the elements in the container by using the shellsort algorithm.
//      The pfnCompare pointer can be used to change the algorithm behaviour.
//
//==EDOC========================================================================
template<class _Type>
void CList<_Type>::Sort(IN int (pfnCompare)(IN const _Type& rOneElement,
                                            IN const _Type& rOtherElement,
                                            IN  mxt_opaque opq),
                        IN  mxt_opaque opq)
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        CListBase::Sort(CList<_Type>::Compare, MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        CListBase::Sort(CList<_Type>::Compare, MX_VOIDPTR_TO_OPQ(&m_stCompareParams));
    }
}

//==SDOC========================================================================
//==
//==  Find
//==
//==============================================================================
//
//  Parameters:
//    uStartIndex:
//      Where to start the search.
//
//    rElement:
//      The element to find.
//
//    pfnCompare:
//      An optional _Type comparison function or NULL.
//
//    opq:
//      An opaque to be passed to the pfnCompare. Ignored if pfnCompare is NULL.
//
//  Returns:
//      When found, the index of the element, the index of the first unused
//      element otherwise.
//
//  Description:
//      Performs a sequential search for rElement, starting at uStartIndex,
//      returning the index of the first occurrence. The pfnCompare pointer can
//      be used to change the algorithm behaviour.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CList<_Type>::Find(IN unsigned int uStartIndex,
                                IN const _Type& rElement,
                                IN int (pfnCompare)(IN const _Type& rOneElement,
                                                    IN const _Type& rOtherElement,
                                                    IN  mxt_opaque opq),
                                IN  mxt_opaque opq) const
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CListBase::Find(uStartIndex,
                               &rElement,
                               CList<_Type>::Compare,
                               MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CListBase::Find(uStartIndex,
                               &rElement,
                               CList<_Type>::Compare,
                               MX_VOIDPTR_TO_OPQ(
                                   const_cast<SCompareParams*>(&m_stCompareParams)));
    }
}

//==SDOC========================================================================
//==
//==  FindSorted
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      The element to find.
//
//    pfnCompare:
//      An optional _Type comparison function or NULL.
//
//    opq:
//      An opaque to be passed to the pfnCompare. Ignored if pfnCompare is NULL.
//
//  Returns:
//      When found, the index of the element, the index of the first unused
//      element otherwise.
//
//  Description:
//      Performs a binary search for rElement on a sorted container returning
//      the index of the first occurrence. The pfnCompare pointer can be used to
//      change the algorithm behaviour. The behaviour is undefined if the
//      container has not been sorted.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CList<_Type>::FindSorted(IN const _Type& rElement,
                                      IN int (pfnCompare)(IN const _Type& rOneElement,
                                                          IN const _Type& rOtherElement,
                                                          IN  mxt_opaque opq),
                                      IN  mxt_opaque opq) const
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CListBase::FindSorted(&rElement,
                                     CList<_Type>::Compare,
                                     MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CListBase::FindSorted(&rElement,
                                     CList<_Type>::Compare,
                                     MX_VOIDPTR_TO_OPQ(
                                         const_cast<SCompareParams*>(&m_stCompareParams)));
    }
}

//==SDOC========================================================================
//==
//==  GetMinElementIndex
//==
//==============================================================================
//
//  Parameters:
//    pfnCompare:
//      An optional _Type comparison function or NULL.
//
//    opq:
//      An opaque to be passed to the pfnCompare. Ignored if pfnCompare is NULL.
//
//  Returns:
//      The index of the "smallest" element found in the container. An empty
//      container always returns 0.
//
//  Description:
//      Performs a sequential search for the "smallest" element of an unsorted
//      container. The pfnCompare pointer can be used to change the algorithm
//      behaviour.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CList<_Type>::GetMinElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                                  IN const _Type& rOtherElement,
                                                                  IN  mxt_opaque opq),
                                              IN  mxt_opaque opq) const
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CListBase::GetMinElementIndex(CList<_Type>::Compare,
                                             MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CListBase::GetMinElementIndex(CList<_Type>::Compare,
                                             MX_VOIDPTR_TO_OPQ(
                                                 const_cast<SCompareParams*>(&m_stCompareParams)));
    }
}

//==SDOC========================================================================
//==
//==  GetMaxElementIndex
//==
//==============================================================================
//
//  Parameters:
//    pfnCompare:
//      An optional _Type comparison function or NULL.
//
//    opq:
//      An opaque to be passed to the pfnCompare. Ignored if pfnCompare is NULL.
//
//  Returns:
//      The index of the "biggest" element found in the container. An empty
//      container always returns 0.
//
//  Description:
//      Performs a sequential search for the "biggest" element of an unsorted
//      container. The pfnCompare pointer can be used to change the algorithm
//      behaviour.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CList<_Type>::GetMaxElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                                  IN const _Type& rOtherElement,
                                                                  IN  mxt_opaque opq),
                                              IN  mxt_opaque opq) const
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CListBase::GetMaxElementIndex(CList<_Type>::Compare,
                                             MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CListBase::GetMaxElementIndex(CList<_Type>::Compare,
                                             MX_VOIDPTR_TO_OPQ(
                                                 const_cast<SCompareParams*>(&m_stCompareParams)));
    }
}

//==SDOC========================================================================
//==
//==  AllocateSorted
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      Reference to the element to allocate.
//
//    ppAllocatedZone:
//      Pointer to hold the pointer to the allocated zone.
//
//  Returns:
//      resS_OK on success, error code otherwise.
//
//  Description:
//      This method allocates a memory zone at the right position in the list by
//      using rElement. The allocated zone then needs to be initialized by the
//      user. This method is useful to the map because the comparison function
//      only compares the keys, which allows to leave uninitialized the second
//      element of the pair given to this function.
//
//  NOTES:
//      This method is needed to provide the CMap class a uniform interface
//      between containers. It must NEVER be used outside of the CMap class.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CList<_Type>::AllocateSorted(IN const _Type& rElement,
                                        OUT void** ppAllocatedZone)
{
    return CListBase::AllocateSorted(&rElement,
                                     CList<_Type>::Compare,
                                     MX_VOIDPTR_TO_OPQ(&m_stCompareParams),
                                     ppAllocatedZone);
}

//==SDOC========================================================================
//==
//==  EraseSorted
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      Reference to the element to erase.
//
//  Description:
//      This method erases an element in a sorted container.
//
//  NOTES:
//      This method is needed to provide the CMap class a uniform interface
//      between containers. It must NEVER be used outside of the CMap class.
//
//==EDOC========================================================================
template<class _Type>
void CList<_Type>::EraseSorted(IN const _Type& rElement)
{
    unsigned int uIndex = FindSorted(rElement);
    if (uIndex != GetEndIndex())
    {
        Erase(uIndex);
    }
}

//==SDOC========================================================================
//==
//==  FindPtrSorted
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      Reference to the element to find.
//
//  Description:
//      This method finds an element in a sorted container and returns a pointer
//      to it.
//
//  NOTES:
//      This method is needed to provide the CMap class a uniform interface
//      between containers. It must NEVER be used outside of the CMap class.
//
//==EDOC========================================================================
template<class _Type>
const _Type* CList<_Type>::FindPtrSorted(IN const _Type& rElement) const
{
    const _Type* pType = NULL;
    unsigned int uIndex = FindSorted(rElement);
    if (uIndex != GetEndIndex())
    {
        pType = &(GetAt(uIndex));
    }

    return (pType);
}

template<class _Type>
inline
_Type* CList<_Type>::FindPtrSorted(IN const _Type& rElement)
{
    _Type* pType = NULL;
    unsigned int uIndex = FindSorted(rElement);
    if (uIndex != GetEndIndex())
    {
        pType = &(GetAt(uIndex));
    }

    return (pType);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CLIST_H

