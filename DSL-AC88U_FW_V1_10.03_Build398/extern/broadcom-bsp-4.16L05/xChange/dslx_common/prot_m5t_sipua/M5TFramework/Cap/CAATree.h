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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CAATREE_H
#define MXG_CAATREE_H
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
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT, MXD_CAATREE_ENABLE_DEBUG
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CAATREEBASE_H
#include "Cap/CAATreeBase.h"
#endif


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
//== Class: CAATree
//==============================================================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//  Implements an AA tree, which is a type of Binary Search Tree.
//
// Description:
//   The CAATree class provides an implementation of an AA tree, which is a type
//   of Binary Search Tree (BST).
//
//   The AA trees are similar in many ways to the Red-Black trees.  They are
//   almost as fast as Red-Black trees but a lot simpler to implement.  In this
//   implementation, the elements are linked together by nodes holding the
//   balancing information of the tree.  Changing the linkage of a node (by
//   inserting or deleting elements in the tree) does not change the physical
//   address of its associated element.
//
//   The CAATree class has the following properties:
//     - Insert: O(log n)
//     - Erase:  O(log n)
//     - Find:   O(log n)
//
//   It is possible to iterate over the tree using indexes. Inorder tree
//   traversal is used and an internal iterator is kept to improve performance.
//   Scanning a tree from its smallest to its biggest element (and vice versa)
//   is an O(n) operation.
//
//   In summary, the CAATree:
//      - Stores objects in an ordered way (defined by a comparison function).
//      - Allows fast searches among many objects.
//      - Does not modify the physical addresses of its elements after they are
//        inserted.
//      - Does not allow insertion of duplicate objects.
//      - Manages its own memory buffer.
//      - Increases its capacity when needed.
//      - Reduces its capacity when requested.
//      - Keeps track of how many objects are currently stored.
//      - Calls an element's constructor automatically at construction.
//      - Calls an element's destructor automatically at destruction.
//
// #When to choose CAATree:#
//
//   It is important to choose your container with care.  Trees are useful when
//   fast random access to a large amount of data is needed. They are also quite
//   efficient at insertion or deletion, but they do not support duplicate
//   values. One frequent use of trees is to implement an associative container
//   such as a map.
//
//   CAATree, CVector, CList, and CVList offer to the programmer different
//   complexity trade-offs and should be used accordingly.  For example, a
//   CVector is the type of sequence that should be used by default when data
//   needs to be accessed randomly by an index.  A CVList and a CList would
//   also be good choices in that case, offering more performance for insertions
//   and deletions but requiring a little more memory by element.  A CAATree
//   should be used when working with a large amount of data that needs to be
//   kept ordered or when a fast non-sequential access to this data is needed
//   without using an index.  Being the container needing the most memory by
//   element, other alternatives to the CAATree should be considered if memory
//   requirements are very tight.
//
//   Choose CAATree when:
//      - You work with a large amount of data and you need a fast random access
//        to them.
//      - Your elements need to be ordered in the container.
//      - It is important to avoid movement of existing contained elements when
//        insertions/deletions take place.
//
//   Avoid CAATree when:
//      - Elements cannot be compared between themselves.
//      - You need to access each element randomly through an index.
//      - Memory requirements are very tight.
//      - Your internal data must be layout-compatible with C.
//
// #Reminders on how to use CAATree:#
//
//      1. When using a CAATree of pointers, don't forget to delete the pointers
//         before the container is destroyed.  You will otherwise produce memory
//         leaks because the CAATree only destroys the pointers and not what
//         they point to. Before deleting a CAATree of pointers, you should
//         always write something like this:
//
//            <CODE>
//
//                unsigned int uIndex;
//                unsigned int uFirstIndex = pAATree->GetFirstIndex();
//                unsigned int uLastIndex = pAATree->GetLastIndex();
//                for (uIndex = uFirstIndex; uIndex <= uLastIndex; uIndex++)
//                {
//                    MX_DELETE((*pAATree)[uIndex]);
//                }
//                MX_DELETE(pAATree);
//                pAATree = NULL;
//
//             </CODE>
//
//      2. Use the method ReserveCapacity to avoid unnecessary reallocations.
//         If you neglect to do so, each new insertion in a CAATree that is too
//         small will make it grow by 1 element, added to the unavoidable
//         overhead.
//
//      3. The default comparison function of the CAATree uses the standard
//         operators between elements.  You must supply a custom comparison
//         function if the elements you store must be compared differently. You
//         must also supply a new comparison function (or override the standard
//         operators of the stored elements) if you want to search elements
//         using only a subset of their data.  Note that you have to supply a
//         comparison function to CAATrees of pointers if you do not want them
//         to be sorted by pointer values.
//
//      4. CAATrees do not support duplicate values. A possible solution for
//         this issue may be to use a container of elements in each node.
//
//      5. When iterating by index and using the Erase function, you have to
//         make sure that the index is valid for the whole iteration scope.
//         Since the Erase function affects the size of the container, the
//         operator[] or the GetAt function should be used with caution. A
//         good practice is to use the GetSize function within the iteration
//         scope such as in the following example:
//
//          <CODE>
//              CAATree<SomeStruct> treeSomeStruct;
//              unsigned int i = 0;
//              for (i = 0 ; i < treeSomeStruct.GetSize(); i++ )
//              {
//                  if ( treeSomeStruct[i].bToErase )
//                  {
//                      treeSomeStruct.Erase(i);
//                      i--;
//                  }
//              }
//          </CODE>
//
//          Another good practice is to iterate from the end such as in the
//          following example:
//
//          <CODE>
//              CAATree<SomeStruct> treeSomeStruct;
//              unsigned int i = 0;
//              for (i = treeSomeStruct.GetSize() ; i > 0 ; i-- )
//              {
//                  if ( treeSomeStruct[i-1].bToErase )
//                  {
//                      treeSomeStruct.Erase(i-1);
//                  }
//              }
//          </CODE>
//
//      6. CAATree fully supports incomparable types such as structures or
//         complex objects. However, since the != and < operators are required
//         by the CAATree class, a user can use the CUncmp template helper
//         rather than implement operators. Please refer to the CUncmp class
//         documentation for more information and code examples.
//
// Warning:
//   This container is not thread safe.
//
// Note:
//   AA trees have been developed by Mark Allen Weiss (see
//   http://www.cs.fiu.edu/~weiss/).
//
// Location:
//   Cap/CAATree.h
//
// See Also:
//   CVector, CList, CVList
//
//==EDOC========================================================================
template<class _Type>
class CAATree : protected CAATreeBase
{
//-- Published Interface
//------------------------
public:

    // Constructors / Destructors / Operators
    //-------------------------------------------------

    // Summary:
    //     Default constructor.
    CAATree(IN IAllocator* pAllocator = NULL);

    // Summary:
    //     Copy constructor.
    //-----------------------
    CAATree(IN const CAATree<_Type>& rAATree);

    // Summary:
    //     Default constructor.
    //--------------------------
    virtual ~CAATree();

    // Summary:
    //     Assignment operator.
    //---------------------------
    CAATree<_Type>& operator=(IN const CAATree<_Type>& rAATree);

    //  Summary:
    //      Sets the comparison function for this container.
    //-------------------------------------------------------
    mxt_result SetComparisonFunction(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                         IN const _Type& rOtherElement,
                                                         IN mxt_opaque opq),
                                     IN  mxt_opaque opq);


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

    //  Summary:
    //      Gets the size of the container.
    //--------------------------------------
    unsigned int GetSize() const;

    //  Summary:
    //      Indicates whether or not the container is empty.
    //-------------------------------------------------------
    bool IsEmpty() const;

    //  Summary:
    //      Indicates whether or not the container is full.
    //------------------------------------------------------
    bool IsFull() const;


    // Index operations.
    //-------------------

    //  Summary:
    //      Gets the index of the first element of the tree.
    //-------------------------------------------------------
    unsigned int GetFirstIndex() const;

    //  Summary:
    //      Gets the index of the last element of the tree.
    //------------------------------------------------------
    unsigned int GetLastIndex() const;

    //  Summary:
    //      Gets the index of the first unused element.
    //--------------------------------------------------
    unsigned int GetEndIndex() const;

    //  Summary:
    //      Gets the index of the smallest element.
    //----------------------------------------------
    unsigned int GetMinElementIndex() const;

    //  Summary:
    //      Gets the index of the biggest element.
    //---------------------------------------------
    unsigned int GetMaxElementIndex() const;


    // Direct element access.
    //------------------------

    //  Summary:
    //      Gets the element at the specified index.
    //-----------------------------------------------
    _Type& GetAt(IN unsigned int uIndex);

    // <COMBINE CAATree::GetAt@IN unsigned int>
    const _Type& GetAt(IN unsigned int uIndex) const;

    //  Summary:
    //      Gets the element at the specified index.
    //-----------------------------------------------
    _Type& operator[](IN unsigned int uIndex);

    // <COMBINE CAATree::[]@IN unsigned int>
    const _Type& operator[](IN unsigned int uIndex) const;


    // Insert/Erase
    //--------------------------------------------

    //  Summary:
    //      Inserts one element in the tree.
    //---------------------------------------
    mxt_result Insert(IN const _Type& rElement);

    //  Summary:
    //      Erases one element of the tree.
    //--------------------------------------
    void EraseElement(IN const _Type& rElement);

    //  Summary:
    //      Erases one element of the tree.
    //--------------------------------------
    void Erase(IN unsigned int uIndex);

    //  Summary:
    //      Erases all the elements of the tree.
    //-------------------------------------------
    void EraseAll();


    // Advanced operations.
    //----------------------

    //  Summary:
    //      Allocates one element but does not construct it.
    //-------------------------------------------------------
    GO void* Allocate();

    //  Summary:
    //      Inserts a previously allocated element in the tree.
    //----------------------------------------------------------
    mxt_result InsertAllocated(IN TOA void* pElement);

    //  Summary:
    //      Erases an element previously allocated by Allocate.
    //----------------------------------------------------------
    void EraseAllocated(IN TOA void* pElement);

    //  Summary:
    //      Finds an element in the tree.
    //------------------------------------
    const _Type* FindPtr(IN const _Type& rElement) const;

    // <COMBINE CAATree::FindPtr@IN const _Type&@const>
    _Type* FindPtr(IN const _Type& rElement);

    // CMap operations.  Do not use outside the CMap class.
    //-----------------------------------------------------

    //  Summary:
    //      Allocates a memory zone in the proper location in the tree.
    //------------------------------------------------------------------
    mxt_result AllocateSorted(IN const _Type& rElement, OUT void** ppAllocatedZone);

    //  Summary:
    //      Erases an element in a sorted container.
    //-----------------------------------------------
    void EraseSorted(IN const _Type& rElement);

    //  Summary:
    //      Finds an element in a sorted container.
    //----------------------------------------------
    const _Type* FindPtrSorted(IN const _Type& rElement) const;

    //<COMBINE CAATree::FindPtrSorted@IN const _Type&@const>
    _Type* FindPtrSorted(IN const _Type& rElement);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

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


//-- Diagnostic methods
//------------------------------------------------
#ifdef MXD_CAATREE_ENABLE_DEBUG
public:
    void PrintTree();
protected:
private:
#endif //-- MXD_CAATREE_ENABLE_DEBUG

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  CAATree
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
CAATree<_Type>::CAATree(IN IAllocator* pAllocator)
: CAATreeBase(sizeof(_Type),
              CAATree<_Type>::Compare,
              MX_VOIDPTR_TO_OPQ(&m_stCompareParams),
              pAllocator)
{
    m_stCompareParams.m_pfnCompare = NULL;
    m_stCompareParams.m_opq = MX_INT32_TO_OPQ(0);
}

//==SDOC========================================================================
//==
//==  CAATree
//==
//==============================================================================
//
//  Parameters:
//    rAATree:
//      A reference to the source AA tree.
//
//  Description:
//      Copy Constructor.  Uninitialized elements allocated by the Allocate
//      method are not copied.
//
//==EDOC========================================================================
template<class _Type>
inline
CAATree<_Type>::CAATree(IN const CAATree<_Type>& rAATree)
: CAATreeBase(rAATree.m_uSizeOfType,
              rAATree.m_stCompareVoidParams.m_pfnCompare,
              rAATree.m_stCompareVoidParams.m_opq,
              rAATree.m_blockAllocator)
{
    *this = rAATree;
}

//==SDOC========================================================================
//==
//==  ~CAATree
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CAATree<_Type>::~CAATree()
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
//    rAATree:
//      A reference to the source CAATree.
//
//  Returns:
//      A reference to this CAATree instance.
//
//  Description:
//      Assignment operator.
//
//==EDOC========================================================================
template<class _Type>
inline
CAATree<_Type>& CAATree<_Type>::operator=(IN const CAATree<_Type>& rAATree)
{
    CAATreeBase::operator=(rAATree);
    m_stCompareParams.m_pfnCompare = rAATree.m_stCompareParams.m_pfnCompare;
    m_stCompareParams.m_opq = rAATree.m_stCompareParams.m_opq;
    return *this;
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
//      Sets the comparison function to be used by the algorithms for this
//      container instance.
//      The comparison function needs to return one of the following values:
//          < 0 if rOneElement is "smaller" than rOtherElement.
//          > 0 if rOneElement is "bigger" than rOtherElement.
//          0 if rOneElement and rOtherElement are equivalent.
//
//   WARNING: 
//       This function MUST be called only when the tree is empty, otherwise
//       it will break the tree content.
//
//==EDOC========================================================================
template<class _Type>
mxt_result CAATree<_Type>::SetComparisonFunction(IN int (pfnCompare)(IN const _Type& rOneElement,
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
//      Returns the capacity, which is the number of elements already allocated
//      that may or may not be already in use.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CAATree<_Type>::GetCapacity() const
{
    return CAATreeBase::GetCapacity();
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
inline
unsigned int CAATree<_Type>::GetLockCapacity() const
{
    return CAATreeBase::GetLockCapacity();
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
//     fails.
//
//==EDOC========================================================================
template<class _Type>
inline
void CAATree<_Type>::LockCapacity()
{
    CAATreeBase::LockCapacity();
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
//
//  Description:
//      Reduces the capacity. The capacity is reduced if uDownToCapacity is
//      below the current capacity. The method fails if the lock capacity count
//      is not 0.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CAATree<_Type>::ReduceCapacity(IN unsigned int uDownToCapacity)
{
    return CAATreeBase::ReduceCapacity(uDownToCapacity);
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
//      Increases the capacity. The capacity is increased to uUpToCapacity if
//      uUpToCapacity is greater than the current capacity. The method fails if
//      the lock capacity count is not 0 and the wanted capacity is greater than
//      the current capacity.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CAATree<_Type>::ReserveCapacity(IN unsigned int uUpToCapacity)
{
    return CAATreeBase::ReserveCapacity(uUpToCapacity);
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
inline
void CAATree<_Type>::UnlockCapacity()
{
    CAATreeBase::UnlockCapacity();
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
//      Returns the size, which is the number of elements currently used by the
//      tree.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CAATree<_Type>::GetSize() const
{
    return CAATreeBase::GetSize();
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
bool CAATree<_Type>::IsEmpty() const
{
    return CAATreeBase::IsEmpty();
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
//      capacity, i.e., there is no more room to add new elements without
//      allocating more memory.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CAATree<_Type>::IsFull() const
{
    return CAATreeBase::IsFull();
}

//==SDOC========================================================================
//==
//==  GetFirstIndex
//==
//==============================================================================
//
//  Returns:
//      The index of the first element of the tree.
//
//  Description:
//      Returns the index of the first element of the tree. In this
//      implementation of a tree, the first index corresponds to the smallest
//      element according to the comparison function. Note that inorder tree
//      traversal is used when the index is incremented.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CAATree<_Type>::GetFirstIndex() const
{
    return CAATreeBase::GetFirstIndex();
}

//==SDOC========================================================================
//==
//==  GetLastIndex
//==
//==============================================================================
//
//  Returns:
//      The index of the last element of the tree.
//
//  Description:
//      Returns the index of the last element of the tree. In this
//      implementation of a tree, the last index corresponds to the biggest
//      element according to the comparison function. Note that inorder tree
//      traversal is used when the index is incremented.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CAATree<_Type>::GetLastIndex() const
{
    return CAATreeBase::GetLastIndex();
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
//      Returns the index of the first unused element (i.e., the first invalid
//      index).
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CAATree<_Type>::GetEndIndex() const
{
    return CAATreeBase::GetEndIndex();
}

//==SDOC========================================================================
//==
//==  GetMinElementIndex
//==
//==============================================================================
//
//  Returns:
//      The index of the "smallest" element found in the container. An empty
//      container always returns 0.
//
//  Description:
//      Returns the index of the smallest element of the tree according to the
//      comparison function (which is 0). This method updates the current index
//      of the tree.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CAATree<_Type>::GetMinElementIndex() const
{
    return CAATreeBase::GetMinElementIndex();
}

//==SDOC========================================================================
//==
//==  GetMaxElementIndex
//==
//==============================================================================
//
//  Returns:
//      The index of the "biggest" element found in the container. An empty
//      container always returns 0.
//
//  Description:
//      Returns the index of the biggest element of the tree according to the
//      comparison function (which corresponds to last index). This method
//      updates the current index of the tree.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CAATree<_Type>::GetMaxElementIndex() const
{
    return CAATreeBase::GetMaxElementIndex();
}

//==SDOC========================================================================
//==
//==  GetAt
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the wanted element.
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex.  A crash occurs if uIndex is greater than
//      the last index.  This method sets the current index to uIndex.
//
//==EDOC========================================================================
template<class _Type>
inline
_Type& CAATree<_Type>::GetAt(IN unsigned int uIndex)
{
    return *reinterpret_cast<_Type*>(CAATreeBase::GetAt(uIndex));
}

template<class _Type>
inline
const _Type& CAATree<_Type>::GetAt(IN unsigned int uIndex) const
{
    return *reinterpret_cast<const _Type*>(CAATreeBase::GetAt(uIndex));
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the wanted element.
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex. A crash occurs if uIndex is greater than
//      the last index. This method sets the current index to uIndex.
//
//==EDOC========================================================================
template<class _Type>
inline
_Type& CAATree<_Type>::operator[](IN unsigned int uIndex)
{
    return *reinterpret_cast<_Type*>(CAATreeBase::operator[](uIndex));
}

template<class _Type>
inline
const _Type& CAATree<_Type>::operator[](IN unsigned int uIndex) const
{
    return *reinterpret_cast<const _Type*>(CAATreeBase::operator[](uIndex));
}

//==SDOC========================================================================
//==
//==  Insert
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      The element to insert.
//
//  Returns:
//      resS_OK
//      resFE_DUPLICATE
//      resFE_INVALID_STATE
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts the given element in the tree.  If the element to insert is a
//      duplicate, then this method returns an error code to indicate the
//      problem.
//
//      The capacity is increased if there are not enough allocated elements to
//      accommodate the insertion of the new elements. The lock capacity count
//      is still enforced and, if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CAATree<_Type>::Insert(IN const _Type& rElement)
{
    return CAATreeBase::Insert(&rElement);
}

//==SDOC========================================================================
//==
//==  EraseElement
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      A reference on an element equal to the one to remove from the tree.
//
//  Description:
//      Erases from the tree the element equal to rElement (rElement does not
//      need to be contained in the tree).  If the element to remove is not
//      found, then this method does nothing at all.
//
//==EDOC========================================================================
template<class _Type>
inline
void CAATree<_Type>::EraseElement(IN const _Type& rElement)
{
    CAATreeBase::EraseElement(&rElement);
}

//==SDOC========================================================================
//==
//==  Erase
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index of the element to remove from the tree.
//
//  Description:
//      Erases one element from the tree. This method asserts if uIndex is
//      bigger than the last index of the tree.
//
//==EDOC========================================================================
template<class _Type>
inline
void CAATree<_Type>::Erase(IN unsigned int uIndex)
{
    CAATreeBase::Erase(uIndex);
}

//==SDOC========================================================================
//==
//==  EraseAll
//==
//==============================================================================
//
//  Description:
//      Erases all the elements of the tree.  It is more efficient to use
//      EraseAll than an Erase for each element.  This method does not erase
//      the uninitialized elements allocated by the Allocate method (you must
//      use EraseAllocated for these elements).
//
//==EDOC========================================================================
template<class _Type>
inline
void CAATree<_Type>::EraseAll()
{
    CAATreeBase::EraseAll();
}

//==SDOC========================================================================
//==
//==  Allocate
//==
//==============================================================================
//
//  Returns:
//      A pointer on an allocated but uninitialized element.  NULL is returned
//      when the allocation fails.
//
//  Description:
//      Allocates an element inside the tree but does not initialize it (no
//      constructor is called).  The user is then free to do whatever it wants
//      with the returned pointer.
//
//      Once the user has initialized the element referenced by the returned
//      pointer (by a "placement new" for example), it must call the
//      InsertAllocated method to insert it in the tree.  This step is necessary
//      because it is impossible for a tree to insert an uninitialized value in
//      its structure. The following is an example of the use of Allocate and
//      InsertAllocated:
//
//        <CODE>
//
//            void* pElem = AATree.Allocate();
//            int* pnValue = new (pElem) int;
//            AATree.InsertAllocated(pElem);
//
//         </CODE>
//
//      The capacity is increased if there are not enough allocated elements to
//      accommodate the insertion of the new elements. The lock capacity count
//      is still enforced and, if it is not zero, the allocate fails and returns
//      NULL.
//
//      Uninitialized elements are not accessible by indexes until they are
//      inserted.  They must also be inserted or erased before the destruction
//      of the tree (the behaviour is otherwise undefined).
//
//==EDOC========================================================================
template<class _Type>
inline
GO void* CAATree<_Type>::Allocate()
{
    return CAATreeBase::Allocate();
}

//==SDOC========================================================================
//==
//==  InsertAllocated
//==
//==============================================================================
//
//  Parameters:
//    pElement:
//      A pointer on an element allocated by Allocate.
//
//  Returns:
//      resS_OK
//      resFE_DUPLICATE
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      Inserts in the tree an element previously allocated by the Allocate
//      method. The element must be initialized by the user between the call to
//      Allocate and the call to InsertAllocated.
//
//        <CODE>
//
//            void* pElem = AATree.Allocate();
//            int* pnValue = new (pElem) int;
//            AATree.InsertAllocated(pElem);
//
//         </CODE>
//
//      This method can fail with the return code resFE_DUPLICATE if the given
//      element has been initialized with a value already in the tree. The
//      EraseAllocated method can then be used to free the element.
//
//      A crash is likely to occur if you use a pointer not allocated by
//      Allocate with this method.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CAATree<_Type>::InsertAllocated(IN TOA void* pElement)
{
    return CAATreeBase::InsertAllocated(pElement);
}

//==SDOC========================================================================
//==
//==  EraseAllocated
//==
//==============================================================================
//
//  Parameters:
//    pElement:
//      A pointer on an element allocated by Allocate.
//
//  Description:
//      Erases an element allocated by Allocate. No destructor is called.
//
//      A crash is likely to occur if you use a pointer not allocated by
//      Allocate with this method.
//
//==EDOC========================================================================
template<class _Type>
inline
void CAATree<_Type>::EraseAllocated(IN TOA void* pElement)
{
    CAATreeBase::EraseAllocated(pElement);
}

//==SDOC========================================================================
//==
//==  FindPtr
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      The element to find.
//
//  Returns:
//      A pointer on the element found, NULL otherwise.
//
//  Description:
//      This method searches the AA tree for the given element and returns
//      a pointer to it if it is found.
//
//==EDOC========================================================================
template<class _Type>
inline
const _Type* CAATree<_Type>::FindPtr(IN const _Type& rElement) const
{
    return reinterpret_cast<const _Type *>(CAATreeBase::FindPtr(&rElement));
}

template<class _Type>
inline
_Type* CAATree<_Type>::FindPtr(IN const _Type& rElement)
{
    return reinterpret_cast<_Type *>(CAATreeBase::FindPtr(&rElement));
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
//      This method allocates a memory zone at the right position in the tree
//      using rElement. The allocated zone then needs to be initialized by the
//      user. This method is useful to the map because the comparison function
//      only compares the keys, so the second element of the pair given to this
//      function can be left uninitialized.
//
//  NOTES:
//      This method is needed to provide the CMap class a uniform interface
//      between containers. It must NEVER be used outside of the CMap class.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CAATree<_Type>::AllocateSorted(IN const _Type& rElement,
                                          OUT void** ppAllocatedZone)
{
    return CAATreeBase::AllocateSorted(&rElement, ppAllocatedZone);
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
inline
void CAATree<_Type>::EraseSorted(IN const _Type& rElement)
{
    EraseElement(rElement);
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
//      This method finds an element in a sorted container and returns a
//      pointer to it.
//
//  NOTES:
//      This method is needed to provide the CMap class a uniform interface
//      between containers. It must NEVER be used outside of the CMap class.
//
//==EDOC========================================================================
template<class _Type>
inline
const _Type* CAATree<_Type>::FindPtrSorted(IN const _Type& rElement) const
{
    return FindPtr(rElement);
}

template<class _Type>
inline
_Type* CAATree<_Type>::FindPtrSorted(IN const _Type& rElement)
{
    return FindPtr(rElement);
}

template<class _Type>
void CAATree<_Type>::ConstructFrom(OUT void* pvoidDestination,
                                   IN const void* pvoidSource)
{
    new (pvoidDestination) _Type(*reinterpret_cast<const _Type*>(pvoidSource));
}

template<class _Type>
void CAATree<_Type>::Destruct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);

    reinterpret_cast<_Type*>(pvoid)->~_Type();
}

template<class _Type>
int CAATree<_Type>::Compare(IN const void* pOneElement,
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

//M5T_INTERNAL_USE_BEGIN
// Methods used to debug
#ifdef MXD_CAATREE_ENABLE_DEBUG
template<class _Type>
inline
void CAATree<_Type>::PrintTree()
{
    CAATreeBase::PrintTree();
}
#endif //-- MXD_CAATREE_ENABLE_DEBUG
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CAATREE_H


