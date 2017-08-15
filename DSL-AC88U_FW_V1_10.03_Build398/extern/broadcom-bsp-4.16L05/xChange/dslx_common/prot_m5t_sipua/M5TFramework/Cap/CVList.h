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
#ifndef MXG_CVLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CVLIST_H
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
#ifndef MXG_CVLISTBASE_H
#include "Cap/CVListBase.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class IAllocator;

template <class _Type, class _Container>
class CStack;

template <class _Type, class _Container>
class CQueue;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CVList
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//   Class implementing a standard vector list.
//
// Description:
//
//   The CVList class is a sequenced container that manages an array of pointers
//   to elements of the templated type and allows fast random-access to any
//   element through the use of an index (first element is at index 0). CVList
//   allows constant time insertions and deletions at the end of the sequence.
//   Inserting or deleting elements in the middle or at the start of a CVList
//   requires linear time.
//
//   By managing its own memory, CVList eliminates the burdens of:
//
//      - Ensuring enough memory is allocated and reallocating memory when
//        needed.
//      - Ensuring the memory buffer and its internal elements are correctly
//        deleted.
//      - Ensuring the correct form of delete is used for single or array of
//        elements.
//      - Keeping track of how many elements are stored in memory.
//
//   In summary, the CVList:
//
//      - Offers random-access with index.
//      - Manages its own memory.
//      - Auto increases its capacity when needed.
//      - Decreases its capacity when requested.
//      - Keeps track of how many objects are currently stored.
//      - Calls the element's construct automatically at construction.
//      - Calls the element's destructor automatically at destruction.
//
// #More on CVList memory management:#
//
//   In a CVList, each element is kept in a separate allocation block that is
//   sized for _Type. CVList manages an array of pointers to the elements. This
//   array maintains the ordering of the elements. If a new element is inserted
//   or an existing element is erased, other pointers to elements in the array
//   have to be shifted up or down to make room for the new element pointer or
//   to fill the space previously occupied by the erased element pointer. As you
//   can understand, this kind of movement affects performance.
//
//   As an example, suppose a CVList "vlist" with its Size() == 3 and
//   its Capacity() == 5 elements. As represented below, you can see that the 3
//   pointers to elements are contiguous in memory and that each element is
//   allocated in an individual memory block.
//
//  <CODE>
//
//               ---------- ---------- ----------
//               | elem 1 | | elem 2 | | elem 3 |
//               ---------- ---------- ----------
//                    ^          ^          ^
//               -----|----------|----------|----------------------------
//     vlist ->  |    |    |     |    |     |     |   NULL   |   NULL   |
//               --------------------------------------------------------
//
//
//  </CODE>
//
//   Now if the method "vlist.Insert(1, 1, element4)" is called, the pointer
//   to elements at index 2 and 3 are shifted up (to the right) and the pointer
//   to element4 is inserted in the second position.
//
//  <CODE>
//
//               ---------- ---------- ---------- ----------
//               | elem 1 | | elem 4 | | elem 2 | | elem 3 |
//               ---------- ---------- ---------- ----------
//                    ^          ^          ^          ^
//               -----|----------|----------|----------|----------------
//     vlist ->  |    |    |     |    |     |    |     |    |   NULL   |
//               -------------------------------------------------------
//
//  </CODE>
//
//   Full reallocation is also performed when the capacity of the internal
//   memory block used to keep the pointer to the elements is not sufficient.
//   This reallocation always occurs when a member method must increase the
//   sequence contained in the CVList object beyond its current storage
//   capacity.
//
//   In all such cases, references and pointers that point at elements remain
//   valid and may be reused later.
//
// #When to choose CVList:#
//
//   It is important to choose your container with care. CVList is designed to
//   be efficient at insertion, deletion and offers random-access, but it is
//   not always appropriate.
//
//   CList, CVector, and CVList offer the programmer different complexity
//   trade-offs and should be used accordingly. CVector is the type of sequence
//   that should be used by default, CVList should be used when there are
//   frequent insertions and deletions from the middle of the sequences, and
//   CList is the data structure of choice when most insertions and deletions
//   take place at the end of the sequence.
//
//   Choose CVList when:
//      - You need to be able to insert a new element at an arbitrary position.
//      - Your elements need to be ordered in the container.
//      - You need to access each element through its index.
//      - You need fast random-access with constant search time.
//      - It is important to avoid movement of existing contained elements when
//        insertions/deletions take place (see the WARNING section below).
//      - You want fast insertions/deletions inside the container. If frequent
//        insertions/deletions are always at the end of the container, you can
//        consider CVector.
//
//   Avoid CVList when:
//      - Your internal data must be layout-compatible with C;
//
// #Reminders on how to use CVList:#
//
//      1. When using a CVList of pointers, don't forget to delete the pointers
//         before the container is destroyed. Otherwise you will produce memory
//         leaks because the CVList only destroys the pointers and not what they
//         point to. Before deleting a CVList of pointers, you should always
//         write something like this:
//
//            <CODE>
//
//                unsigned int uIndex;
//                unsigned int uSize = pvlist->GetSize();
//                for (uIndex = 0; uIndex < uSize; uIndex++)
//                {
//                    MX_DELETE((*pvlist)[uIndex]);
//                }
//                MX_DELETE(pvlist);
//                pvlist = NULL;
//
//             </CODE>
//
//      3. Use the method ReserveCapacity to avoid unnecessary reallocations.
//         If you neglect to do so, each new insertion on a CVList that is too
//         small makes it grow by 1 element, added to the unavoidable
//         malloc/copy/free, which is somewhat heavy.
//
//      4. Use the method GetCapacity to know exactly the number of objects
//         the CVList can store without reallocation.
//
//      5. Use the method GetSize to know the number of elements currently
//         stored.
//
//      6. When iterating by index and using the Erase method, you have to
//         make sure that the index is valid for the whole iteration scope.
//         Since the Erase method affects the size of the container, the
//         operator[] or the GetAt method should be used with caution. A good
//         practice is to use the GetSize method within the iteration scope
//         such as in the following example:
//
//          <CODE>
//              CVList<SomeStruct> vlstSomeStruct;
//              unsigned int i = 0;
//              for (i = 0 ; i < vlstSomeStruct.GetSize(); i++ )
//              {
//                  if ( vlstSomeStruct[i].bToErase )
//                  {
//                      vlstSomeStruct.Erase(i);
//                      i--;
//                  }
//              }
//          </CODE>
//
//          Another good practice is to iterate from the end such as in the
//          following example:
//
//          <CODE>
//              CVList<SomeStruct> vlstSomeStruct;
//              unsigned int i = 0;
//              for ( i = vlstSomeStruct.GetSize(); i > 0; i-- )
//              {
//                  if ( vlstSomeStruct[i-1].bToErase )
//                  {
//                      vlstSomeStruct.Erase(i-1);
//                  }
//              }
//          </CODE>
//
//      7. CVList fully supports incomparable types such as structures or
//         complex objects. However, the sort methods (InsertSorted, Sort, Find,
//         FindSorted, GetMinElementIndex, and GetMaxElementIndex) can not be
//         used except if the structure or the complex object implements the !=
//         and < operators. Alternatively, you can use the CUncmp template
//         helper and provide your own compare function. Please refer to the
//         CUncmp class documentation for more information and code examples.
//
// Warning:
//  This container is not thread safe.
//
// Location:
//   Cap/CVList.h
//
// See also:
//   CList, CVector
//
//==EDOC========================================================================
template<class _Type>
class CVList : protected CVListBase
{
//M5T_INTERNAL_USE_BEGIN
    friend class CQueue<_Type, CVList<_Type> >;
    friend class CStack<_Type, CVList<_Type> >;
//M5T_INTERNAL_USE_END

// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //     Constructor.
    //------------------
    CVList(IN IAllocator* pAllocator = NULL);

    // Summary:
    //     Copy Constructor.
    //------------------------
    CVList(IN const CVList<_Type>& rVList);

    // Summary:
    //     Destructor.
    //------------------
    virtual ~CVList();

    // Summary:
    //     Assignment operator.
    //--------------------------
    CVList<_Type>& operator=(IN const CVList<_Type>& rvlstList);

    // Summary:
    //     Comparison operator.
    //---------------------------
    bool operator==(IN const CVList<_Type>& rvlstList) const;

    // Summary:
    //     Different than operator.
    //------------------------------
    bool operator!=(IN const CVList<_Type>& rvlstList) const;

    // Summary:
    //     Less than operator.
    //-------------------------
    bool operator<(IN const CVList<_Type>& rvlstList) const;

    // Summary:
    //     Greater than operator.
    //----------------------------
    bool operator>(IN const CVList<_Type>& rvlstList) const;

    // Summary:
    //     Less than or equal to operator.
    //-------------------------------------
    bool operator<=(IN const CVList<_Type>& rvlstList) const;

    // Summary:
    //     Greater than or equal to operator.
    //----------------------------------------
    bool operator>=(IN const CVList<_Type>& rvlstList) const;

    //  Summary:
    //      Sets the default comparison function for this container.
    //----------------------------------------------------------------
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
    //     Gets the size of the CVList.
    //----------------------------------
    unsigned int GetSize() const;

    // Summary:
    //     Checks if the CVList is empty.
    //------------------------------------
    bool IsEmpty() const;

    // Summary:
    //     Checks if the CVList is full.
    //-----------------------------------
    bool IsFull() const;

    // Index operations.
    //-------------------

    // Summary:
    //      Returns the index of the first used element.
    //---------------------------------------------------
    unsigned int GetFirstIndex() const;

    // Summary:
    //     Returns the index of the last used element.
    //-------------------------------------------------
    unsigned int GetLastIndex() const;

    // Summary:
    //      Returns the index of the first unused element.
    //-----------------------------------------------------
    unsigned int GetEndIndex() const;

    // Direct element access.
    //------------------------

    // Summary:
    //     Returns the element at the specified index.
    //-------------------------------------------------
    const _Type& operator[](IN unsigned int uIndex) const;

    // Summary:
    //     Returns the element at the specified index.
    //-------------------------------------------------
    _Type& operator[](IN unsigned int uIndex);

    // Summary:
    //     Returns the element at the specified index.
    //-------------------------------------------------
    const _Type& GetAt(IN unsigned int uIndex) const;

    // Summary:
    //     Returns the element at the specified index.
    //-------------------------------------------------
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
    //      Inserts the elements contained within another CVList.
    //------------------------------------------------------------
    mxt_result Insert(IN unsigned int uIndex, IN const CVList& rVList);

    //  Summary:
    //      Inserts one element, leaving the container sorted.
    //---------------------------------------------------------
    mxt_result InsertSorted(IN const _Type& rElement,
                            IN int (pfnCompare)(IN const _Type& rOneElement,
                                                IN const _Type& rOtherElement,
                                                IN mxt_opaque opq) = NULL,
                            IN mxt_opaque opq = 0);

    // Summary:
    //     Erases the element at the specified index.
    //-------------------------------------------------
    void Erase(IN unsigned int uIndex);

    // Summary:
    //     Erases multiple elements, beginning with the element at the index.
    //------------------------------------------------------------------------
    void Erase(IN unsigned int uIndex, IN unsigned int uCount);

    // Summary:
    //     Erases all elements.
    //--------------------------
    void EraseAll();

    // Advanced operations.
    //----------------------

    //  Summary:
    //      Inserts one element but does not construct it.
    //-----------------------------------------------------
    void* Allocate(IN unsigned int uIndex);

    //  Summary:
    //      Moves all the elements of another VList to a specific index.
    //-------------------------------------------------------------------
    mxt_result Merge(IN unsigned int uIndex, INOUT CVList& rVList);

    //  Summary:
    //      Moves some elements from this VList to another starting from a
    //      specific index.
    //---------------------------------------------------------------------
    mxt_result Split(IN unsigned int uIndex, OUT CVList& rVList);

    // Summary:
    //     Exchanges the position of two elements.
    //---------------------------------------------
    void Swap(IN unsigned int uFirstIndex, IN unsigned int uSecondIndex);

    // Summary:
    //     Sorts the elements in the container using the shellsort algorithm.
    //------------------------------------------------------------------------
    void Sort(IN int (pfnCompare)(IN const _Type& rOneElement,
                                  IN const _Type& rOtherElement,
                                  IN  mxt_opaque opq) = NULL,
              IN  mxt_opaque opq = 0);

    // Summary:
    //      Performs a sequential search for an element starting at the index.
    //-------------------------------------------------------------------------
    unsigned int Find(IN unsigned int uStartIndex,
                      IN const _Type& rElement,
                      IN int (pfnCompare)(IN const _Type& rOneElement,
                                          IN const _Type& rOtherElement,
                                          IN  mxt_opaque opq) = NULL,
                      IN  mxt_opaque opq = 0) const;

    // Summary:
    //     Performs a binary search for an element on a sorted container.
    //--------------------------------------------------------------------
    unsigned int FindSorted(IN const _Type& rElement,
                            IN int (pfnCompare)(IN const _Type& rOneElement,
                                                IN const _Type& rOtherElement,
                                                IN  mxt_opaque opq) = NULL,
                            IN  mxt_opaque opq = 0) const;

    // Summary:
    //     Performs a sequential search for the "smallest" element.
    //--------------------------------------------------------------
    unsigned int GetMinElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                        IN const _Type& rOtherElement,
                                                        IN  mxt_opaque opq) = NULL,
                                    IN  mxt_opaque opq = 0) const;

    // Summary:
    //      Performs a sequential search for the "biggest" element.
    //--------------------------------------------------------------
    unsigned int GetMaxElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                        IN const _Type& rOtherElement,
                                                        IN  mxt_opaque opq) = NULL,
                                    IN  mxt_opaque opq = 0) const;


    // CMap operations.  Do not use outside the CMap class.
    //-----------------------------------------------------

    //  Summary:
    //      Allocates a memory zone in the proper location in the CVList.
    //--------------------------------------------------------------------
    mxt_result AllocateSorted(IN const _Type& rElement, OUT void** ppAllocatedZone);

    //  Summary:
    //      Erases an element in a sorted container.
    //-----------------------------------------------
    void EraseSorted(IN const _Type& rElement);

    //  Summary:
    //      Finds an element in a sorted container.
    //----------------------------------------------
    const _Type* FindPtrSorted(IN const _Type& rElement) const;

    //<COMBINE CVList::FindPtrSorted@IN const _Type&@const>
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
    //      Inserts an element at the top.
    //-------------------------------------
    mxt_result Push(IN const _Type& rElement);

    // Queue operations.
    //-------------------

    // Summary:
    //      Retrieves the element at the queue's first index without erasing it.
    //---------------------------------------------------------------------------
    const _Type& GetFront() const;

    // Summary:
    //      Retrieves the element at the queue's first index without erasing it.
    //---------------------------------------------------------------------------
    _Type& GetFront();

    // Summary:
    //      Erases the element at the queue's first index.
    //-----------------------------------------------------
    void Dequeue();

    // Summary:
    //      Inserts the element at the queue's last index.
    //-----------------------------------------------------
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
//==  CVList
//==
//==============================================================================
//
//  Parameters:
//    pAllocator:
//      Allows to specify an allocator that is used to allocate and free the
//      individual elements.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CVList<_Type>::CVList(IN IAllocator* pAllocator)
: CVListBase(sizeof(_Type), pAllocator)
{
    m_stCompareParams.m_pfnCompare = NULL;
    m_stCompareParams.m_opq = MX_INT32_TO_OPQ(0);
}

//==SDOC========================================================================
//==
//==  CVList
//==
//==============================================================================
//
//  Parameters:
//    rVList:
//      A reference to the source VList.
//
//  Description:
//      Copy Constructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CVList<_Type>::CVList(IN const CVList<_Type>& rVList)
: CVListBase(rVList.m_uSizeOfType,rVList.m_blockAllocator)
{
    *this = rVList;
}

//==SDOC====================================================================
//==
//==  ~CVList
//==
//==========================================================================
//
//  Description:
//      Destructor.
//
//==EDOC====================================================================
template<class _Type>
inline
CVList<_Type>::~CVList()
{
    EraseAll();
}

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    rvlstList:
//      Reference to the CVList to assign.
//
//  Returns:
//      The assigned CVlist.
//
//  Description:
//      Assigns the right hand CVList to the left hand CVList.
//
//==EDOC========================================================================
template<class _Type>
inline
CVList<_Type>& CVList<_Type>::operator=(IN const CVList<_Type>& rvlstList)
{
    CVListBase::operator=(rvlstList);
    m_stCompareParams.m_pfnCompare = rvlstList.m_stCompareParams.m_pfnCompare;
    m_stCompareParams.m_opq = rvlstList.m_stCompareParams.m_opq;
    return *this;
}

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    rvlstList:
//      Reference to the CVList to compare.
//
//  Returns:
//      Returns true if both CVList are identical, otherwise returns false.
//
//  Description:
//      Verifies that the left hand CVList is identical to the right hand
//      CVList. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour
//      is identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVList<_Type>::operator==(IN const CVList<_Type>& rvlstList) const
{
    return !(*this != rvlstList);
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rvlstList:
//      Reference to the CVList to compare.
//
//  Returns:
//      Returns true if both CVList are different, otherwise returns false.
//
//  Description:
//      Verifies that the left hand CVList is different than the right hand
//      CVList. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour
//      is identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
bool CVList<_Type>::operator!=(IN const CVList<_Type>& rvlstList) const
{
    if (&rvlstList == this)
    {
        return false;
    }

    if (GetSize() != rvlstList.GetSize())
    {
        return true;
    }

    unsigned int uIndex = 0;
    bool bNotEqual = false;
    unsigned int uSizeOfList = GetSize();

    while (uIndex < uSizeOfList && bNotEqual == false)
    {
        if (GetAt(uIndex) != rvlstList.GetAt(uIndex))
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
//    rvlstList:
//      Reference to the CVList to compare.
//
//  Returns:
//      Returns true if the left hand CVList is smaller than the right hand
//      CVList, otherwise returns false.
//
//  Description:
//      Verifies if the left hand CVList is less than the right hand CVList.
//      This is done by comparing containers element by element and returns
//      once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
bool CVList<_Type>::operator<(IN const CVList<_Type>& rvlstList) const
{
    if (&rvlstList == this)
    {
        return false;
    }

    unsigned int uIndex = 0;
    bool bLessThan = false;
    unsigned int uSizeOfList = MX_MIN(GetSize(), rvlstList.GetSize());

    while (uIndex < uSizeOfList && bLessThan == false)
    {
        if (GetAt(uIndex) < rvlstList.GetAt(uIndex))
        {
            bLessThan = true;
        }

        uIndex++;
    }

    if(bLessThan == false)
    {
        bLessThan = GetSize() < rvlstList.GetSize() ? true : false;
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
//    rvlstList:
//      Reference to the CVList to compare.
//
//  Returns:
//      Returns true if the left hand CVList is greater than the right hand
//      CVList, otherwise returns false.
//
//  Description:
//      Verifies if the left hand CVList is greater than the right hand CVList.
//      This is done by comparing containers element by element and returns
//      once an element is different from the other. This behaviour is identical
//      to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVList<_Type>::operator>(IN const CVList<_Type>& rvlstList) const
{
    return (!(*this < rvlstList) && *this != rvlstList);
}

//==SDOC========================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//    rvlstList:
//      Reference to the CVList to compare.
//
//  Returns:
//      Returns true if the left hand CVList is smaller than or equal to the
//      right hand CVList, otherwise returns false
//
//  Description:
//      Verifies if the left hand CVList is less than or equal to the right hand
//      CVList. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVList<_Type>::operator<=(IN const CVList<_Type>& rvlstList) const
{
    return !(*this > rvlstList);
}

//==SDOC========================================================================
//==
//==  operator>=
//==
//==============================================================================
//
//  Parameters:
//    rvlstList:
//      Reference to the CVList to compare.
//
//  Returns:
//      Returns true if the left hand CVList is greater than or equal to the
//      right hand CVList, otherwise returns false.
//
//  Description:
//      Verifies if the left hand CVList is less than or equal to the right hand
//      CVList. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVList<_Type>::operator>=(IN const CVList<_Type>& rvlstList) const
{
    return !(*this < rvlstList);
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
mxt_result CVList<_Type>::SetComparisonFunction(IN int (pfnCompare)(IN const _Type& rOneElement,
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
unsigned int CVList<_Type>::GetCapacity() const
{
    return CVListBase::GetCapacity();
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
unsigned int CVList<_Type>::GetLockCapacity() const
{
    return CVListBase::GetLockCapacity();
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
inline
void CVList<_Type>::LockCapacity()
{
    CVListBase::LockCapacity();
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
//      Reduces the capacity. The capacity is reduced to uDownToCapacity if the
//      size of the CVList is less than uDownToCapacity and if uDownToCapacity
//      is less than the current capacity. This method fails if the lock
//      capacity count is not 0.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVList<_Type>::ReduceCapacity(IN unsigned int uDownToCapacity)
{
    return CVListBase::ReduceCapacity(uDownToCapacity);
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
//      uUpToCapacity is greater than the current capacity. This method fails if
//      the lock capacity count is not 0 and the wanted capacity is greater than
//      the current capacity.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVList<_Type>::ReserveCapacity(IN unsigned int uUpToCapacity)
{
    return CVListBase::ReserveCapacity(uUpToCapacity);
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
void CVList<_Type>::UnlockCapacity()
{
    CVListBase::UnlockCapacity();
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
//      Returns the size, which is the number of elements that are allocated and
//      already in use.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CVList<_Type>::GetSize() const
{
    return CVListBase::GetSize();
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
bool CVList<_Type>::IsEmpty() const
{
    return CVListBase::IsEmpty();
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
//      capacity, i.e. there's no more room to add new elements without
//      allocating more memory.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVList<_Type>::IsFull() const
{
    return CVListBase::IsFull();
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
unsigned int CVList<_Type>::GetFirstIndex() const
{
    return CVListBase::GetFirstIndex();
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
unsigned int CVList<_Type>::GetLastIndex() const
{
    return CVListBase::GetLastIndex();
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
unsigned int CVList<_Type>::GetEndIndex() const
{
    return CVListBase::GetEndIndex();
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          Index of the element to return.
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex. Returns an invalid element if uIndex is
//      equal to or greater than the size.
//
//==EDOC========================================================================
template<class _Type>
inline
const _Type& CVList<_Type>::operator[](IN unsigned int uIndex) const
{
    return *reinterpret_cast<const _Type *>(CVListBase::operator[](uIndex));
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          Index of the element to return.
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex. Returns an invalid element if uIndex is
//      equal to or greater than the size.
//
//==EDOC========================================================================
template<class _Type>
inline
_Type& CVList<_Type>::operator[](IN unsigned int uIndex)
{
    return *reinterpret_cast<_Type *>(CVListBase::operator[](uIndex));
}

//==SDOC========================================================================
//==
//==  GetAt
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          Index of the element to return.
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex. Returns an invalid element if uIndex is
//      equal to or greater than the size.
//
//==EDOC========================================================================
template<class _Type>
inline
const _Type& CVList<_Type>::GetAt(IN unsigned int uIndex) const
{
    return *reinterpret_cast<const _Type *>(CVListBase::GetAt(uIndex));
}

//==SDOC========================================================================
//==
//==  GetAt
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          Index of the element to return.
//
//  Returns:
//      The element at uIndex.
//
//  Description:
//      Returns the element at uIndex. Returns an invalid element if uIndex is
//      equal to or greater than the size.
//
//==EDOC========================================================================
template<class _Type>
inline
_Type& CVList<_Type>::GetAt(IN unsigned int uIndex)
{
    return *reinterpret_cast<_Type *>(CVListBase::GetAt(uIndex));
}

//==SDOC========================================================================
//==
//==  Append
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      A reference to an element that is used to contruct the new one in the
//      CVList.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Appends one element and constructs it.
//
//  See also: Insert
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVList<_Type>::Append(IN const _Type& rElement)
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
//      The index where to insert the first element.
//
//    uCount:
//      The number of elements to insert.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts one or more elements and constructs them. The capacity is
//      increased if there are not enough allocated elements to accommodate the
//      insertion of the new elements. The lock capacity count is still enforced
//      and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVList<_Type>::Insert(IN unsigned int uIndex,
                                 IN unsigned int uCount)
{
    return CVListBase::Insert(uIndex, uCount);
}

//==SDOC========================================================================
//==
//==  Insert
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      The index where to insert the first element.
//
//    uCount:
//      The number of elements to insert.
//
//    rElement:
//      A reference to an element that is used to construct the elements.
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
mxt_result CVList<_Type>::Insert(IN unsigned int uIndex,
                                 IN unsigned int uCount,
                                 IN const _Type& rContent)
{
    return CVListBase::Insert(uIndex, uCount, &rContent);
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
//    rVList:
//      A VList that contains the elements to insert.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts the elements contained within another CVList to a specified
//      index. The capacity is increased if there are not enough allocated
//      elements to accommodate the insertion of the new elements. The lock
//      capacity count is still enforced and if it is not zero, the insertion
//      fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVList<_Type>::Insert(IN unsigned int uIndex,
                                 IN const CVList& rVList)
{
    return CVListBase::Insert(uIndex, rVList);
}

//==SDOC========================================================================
//==
//==  InsertSorted
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      A reference to an element that is used to construct the new element.
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
//      container sorted. The capacity is increased if there are not enough
//      allocated elements to accommodate the insertion of the new element. The
//      lock capacity count is still enforced and if it is not zero, the
//      insertion fails. The pfnCompare pointer can be used to change the
//      algorithm behaviour.
//
//==EDOC========================================================================
template<class _Type>
mxt_result CVList<_Type>::InsertSorted(IN const _Type& rElement,
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
        return CVListBase::InsertSorted(&rElement,
                                        CVList<_Type>::Compare,
                                        MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVListBase::InsertSorted(&rElement,
                                        CVList<_Type>::Compare,
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
//      Erases one element.
//
//==EDOC========================================================================
template<class _Type>
inline
void CVList<_Type>::Erase(IN unsigned int uIndex)
{
    CVListBase::Erase(uIndex);
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
//      Erases multiple elements, beginning with the element at uIndex.
//
//==EDOC========================================================================
template<class _Type>
inline
void CVList<_Type>::Erase(IN unsigned int uIndex, IN unsigned int uCount)
{
    CVListBase::Erase(uIndex, uCount);
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
void CVList<_Type>::EraseAll()
{
    CVListBase::EraseAll();
}

//M5T_INTERNAL_USE_BEGIN
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
void CVList<_Type>::Pop()
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
mxt_result CVList<_Type>::Push(IN const _Type& rElement)
{
    return Insert(GetEndIndex(), 1, rElement);
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
const _Type& CVList<_Type>::GetTop() const
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
_Type& CVList<_Type>::GetTop()
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
void CVList<_Type>::Dequeue()
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
mxt_result CVList<_Type>::Enqueue(IN const _Type& rElement)
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
const _Type& CVList<_Type>::GetFront() const
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
_Type& CVList<_Type>::GetFront()
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
//      Acts like an insert but does not call the constructor. The user is then
//      free to use the pointer to do whatever he wishes. The capacity is
//      increased if there are not enough allocated elements to accommodate the
//      insertion of the new elements. The lock capacity count is still enforced
//      and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
void* CVList<_Type>::Allocate(IN unsigned int uIndex)
{
    return CVListBase::Allocate(uIndex);
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
//    rVList:
//      The source CVList.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Moves all the elements of another CVList to uIndex. The capacity is
//      increased if there are not enough allocated elements to accommodate the
//      insertion of the new elements. The lock capacity count is still enforced
//      and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVList<_Type>::Merge(IN unsigned int uIndex, INOUT CVList& rVList)
{
    return CVListBase::Merge(uIndex, rVList);
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
//    rVList:
//      The destination CVList.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Moves some elements from this CVList to another starting from a specific
//      index. The destination vector is emptied first. The capacity of the
//      target vector is increased if there are not enough allocated elements to
//      accommodate the insertion of the new elements. The lock capacity count
//      is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVList<_Type>::Split(IN unsigned int uIndex, OUT CVList& rVList)
{
    return CVListBase::Split(uIndex, rVList);
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
void CVList<_Type>::Swap(IN unsigned int uFirstIndex,
                         IN unsigned int uSecondIndex)
{
    CVListBase::Swap(uFirstIndex, uSecondIndex);
}

//M5T_INTERNAL_USE_BEGIN
template<class _Type>
void CVList<_Type>::Construct(OUT void* pvoid)
{
    new (pvoid) _Type;
}

template<class _Type>
void CVList<_Type>::ConstructFrom(OUT void* pvoidDestination,
                                  IN const void* pvoidSource)
{
    new (pvoidDestination) _Type(*reinterpret_cast<const _Type*>(pvoidSource));
}

template<class _Type>
void CVList<_Type>::Destruct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);

    reinterpret_cast<_Type*>(pvoid)->~_Type();
}

template<class _Type>
int CVList<_Type>::Compare(IN const void* pOneElement,
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
//      Sorts the elements in the container using the shellsort algorithm. The
//      pfnCompare pointer can be used to change the algorithm behaviour.
//
//==EDOC========================================================================
template<class _Type>
void CVList<_Type>::Sort(IN int (pfnCompare)(IN const _Type& rOneElement,
                                             IN const _Type& rOtherElement,
                                             IN  mxt_opaque opq),
                         IN  mxt_opaque opq)
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        CVListBase::Sort(CVList<_Type>::Compare, MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        CVListBase::Sort(CVList<_Type>::Compare, MX_VOIDPTR_TO_OPQ(&m_stCompareParams));
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
unsigned int CVList<_Type>::Find(IN unsigned int uStartIndex,
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
        return CVListBase::Find(uStartIndex,
                                &rElement,
                                CVList<_Type>::Compare,
                                MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVListBase::Find(uStartIndex,
                                &rElement,
                                CVList<_Type>::Compare,
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
unsigned int CVList<_Type>::FindSorted(IN const _Type& rElement,
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
        return CVListBase::FindSorted(&rElement,
                                      CVList<_Type>::Compare,
                                      MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVListBase::FindSorted(&rElement,
                                      CVList<_Type>::Compare,
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
//      Performs a sequential search for the "smallesr" element of an unsorted
//      container. The pfnCompare pointer can be used to change the algorithm
//      behaviour.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CVList<_Type>::GetMinElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                                   IN const _Type& rOtherElement,
                                                                   IN  mxt_opaque opq),
                                               IN  mxt_opaque opq) const
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CVListBase::GetMinElementIndex(CVList<_Type>::Compare,
                                              MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVListBase::GetMinElementIndex(CVList<_Type>::Compare,
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
unsigned int CVList<_Type>::GetMaxElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                                   IN const _Type& rOtherElement,
                                                                   IN  mxt_opaque opq),
                                               IN  mxt_opaque opq) const
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CVListBase::GetMaxElementIndex(CVList<_Type>::Compare,
                                              MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVListBase::GetMaxElementIndex(CVList<_Type>::Compare,
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
//      This method allocates a memory zone at the right position in the CVList
//      using rElement.  The allocated zone then needs to be initialized by the
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
mxt_result CVList<_Type>::AllocateSorted(IN const _Type& rElement,
                                         OUT void** ppAllocatedZone)
{
    return CVListBase::AllocateSorted(&rElement,
                                      CVList<_Type>::Compare,
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
void CVList<_Type>::EraseSorted(IN const _Type& rElement)
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
//      This method finds an element in a sorted container and returns a
//      pointer to it.
//
//  NOTES:
//      This method is needed to provide the CMap class a uniform interface
//      between containers. It must NEVER be used outside of the CMap class.
//
//==EDOC========================================================================
template<class _Type>
const _Type* CVList<_Type>::FindPtrSorted(IN const _Type& rElement) const
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
_Type* CVList<_Type>::FindPtrSorted(IN const _Type& rElement)
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

#endif // MXG_CVLIST_H

