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
#ifndef MXG_CVECTOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CVECTOR_H
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
#ifndef MXG_CMARSHALER_H
#include "Cap/CMarshaler.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CVECTORBASE_H
#include "Cap/CVectorBase.h"
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
#if defined(_Type)
#error "_Type" is already defined as a MACRO. Cannot be used again in template definition.
#endif

template <class _Type>
class CVector;

//M5T_INTERNAL_USE_BEGIN
template <class _Type>
CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CVector<_Type>& rVector);

template <class _Type>
CMarshaler& operator>> (IN CMarshaler& rMarshaler, IN CVector<_Type>& rVector);
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//== Class: CVector
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//   Class implementing a standard vector.
//
// Description:
//
//   This template class implements a standard vector. It is the equivalent to
//   the STL container template class "vector" and its implementation adopts the
//   same philosophy.
//
//   The CVector class is a sequenced container that arranges elements of a
//   given type in a linear arrangement and allows fast random-access to any
//   element through the use of an index (the first element is at index 0). In
//   other words, the CVector keeps an array of elements contiguously in memory.
//   CVectors allow constant time insertions and deletions at the end of the
//   sequence. Inserting or deleting elements in the middle or at the start of
//   a vector requires linear time. The internal buffer grows as elements are
//   added to the container and when the vector is destroyed, its destructor
//   automatically destroys the elements in the container and deallocates the
//   memory holding those elements.
//
//   By managing its own memory, CVector eliminates the burdens of:
//
//      - Ensuring enough memory is allocated and reallocating memory when
//        needed.
//      - Ensuring the memory buffer and its internal elements are correctly
//        deleted.
//      - Ensuring the correct form of delete is used for single or array of
//        elements.
//      - Keeping track of how many elements are stored in memory.
//
//   In summary, the CVector:
//
//      - Offers random-access with index.
//      - Manages its own memory buffer.
//      - Auto increases its capacity when needed.
//      - Decreases its capacity when requested.
//      - Keeps track of how many objects are currently stored.
//      - Calls the element's construct automatically at construction.
//      - Calls the element's destructor automatically at destruction.
//
// #More on CVector memory management:#
//
//   CVector, like CString, is a contiguous-memory container compared to CList
//   that is a node-based container. As a contiguous-memory container (or if you
//   prefer an array-based container), CVector stores its elements in one block
//   of memory. This block of memory holds zero, one, or more elements (_Type).
//   If a new element is inserted or an existing element is erased, the other
//   elements in the memory block have to be shifted up or down to make room for
//   the new element or to fill the space previously occupied by the erased
//   element. This kind of movement affects both performance and exception
//   safety.
//
//   As an example, suppose a CVector "vec" with its Size() == 3 and its
//   Capacity() == 5 elements. As represented below, you can see that the 3
//   elements are contiguous in memory.
//
//  <CODE>
//
//             --------------------------------------------------------
//     vec ->  | element1 | element2 | element3 |   free   |   free   |
//             --------------------------------------------------------
//
//  </CODE>
//
//   Now if the method "vec.Insert(1, 1, element4)" is called, the elements at
//   index 2 and 3 are shifted up (to the right) and element4 is inserted in
//   the second position.
//
//  <CODE>
//
//             --------------------------------------------------------
//     vec ->  | element1 | element4 | element2 | element3 |   free   |
//             --------------------------------------------------------
//
//  </CODE>
//
//   At this point in time, all <B>references and pointers</B> to element 2 and
//   3 become invalid.
//
//   Full reallocation is also performed when the capacity of the internal
//   memory block is not sufficient. This reallocation always occurs when a
//   method must increase the sequence contained in the CVector object beyond
//   its current storage capacity.
//
//   In all such cases, references and pointers that point at altered portions
//   of the sequence become invalid. If no reallocation occurs, only references
//   and pointers before the insertion/deletion point remain valid.
//
// #Optimization:#
//
//   Depending on the template type used, it may be possible to optimize the
//   insertion and deletion operations. If the constructors and destructors are
//   of no use, simply using memcpy, memmove, and memset is faster. To force
//   this optimization, false must be passed at creation in the bUseVirtual
//   argument.
//
// #When to choose CVector:#
//
//   It is important to choose your container with care. CVector is designed to
//   be efficient at memory management and offers random-access, but it is not
//   always appropriate.
//
//   CList, CVector, and CVList offer the programmer different complexity
//   trade-offs and should be used accordingly. CVector is the type of sequence
//   that should be used by default, CVList should be used when there are
//   frequent insertions and deletions from the middle of the sequences, and
//   CList is the data structure of choice when most insertions and deletions
//   take place at the end of the sequence.
//
//   Choose CVector when:
//      - You need to be able to insert a new element at an arbitrary position.
//      - Your elements need to be ordered in the container.
//      - You need to access each element through its index.
//      - You need fast random-access with constant search time.
//      - Your internal data must be layout-compatible with C.
//
//   Avoid CVector when:
//      - It is important to avoid movement of existing contained elements when
//        insertions/deletions take place (see WARNING section below).
//      - You want fast insertions/deletions inside the container. If frequent
//        insertions/deletions are always at the end of the container, you can
//        consider CVector.
//
//
// #Reminders on how to use CVector:#
//
//      1. Avoid keeping references and pointers to internal elements. If a
//         reference/pointer needs to be given, make sure no reallocation is
//         performed by allocating sufficient capacity with Reserve() and by
//         making sure no insertion or deallocation is performed on the CVector
//         elements.
//
//      2. When using a CVector of pointers, don't forget to delete the pointers
//         before the container is destroyed. Otherwise you will produce memory
//         leaks because the CVector only destroys the pointers and not what
//         they point to. Before deleting a CVector of pointers, you should
//         always write something like this:
//
//            <CODE>
//
//                unsigned int uIndex;
//                unsigned int uSize = pvec->GetSize();
//                for (uIndex = 0; uIndex < uSize; uIndex++)
//                {
//                    MX_DELETE((*pvec)[uIndex]);
//                }
//                MX_DELETE(pvec);
//                pvec = NULL;
//
//             </CODE>
//
//      3. Use the method ReserveCapacity to avoid unnecessary reallocations.
//         If you neglect to do so, each new insertion on a vector that is too
//         small makes it grow by 1 element, added to the unavoidable
//         malloc/copy/free, which is somewhat heavy.
//
//      4. Use the method GetCapacity to know exactly the number of objects
//         the vector can store without reallocation.
//
//      5. Use the method GetSize to know the number of currently stored
//         elements.
//
//      6. When iterating by index and using the Erase method, you have
//         to make sure that the index is valid for the whole iteration scope.
//         Since the Erase method affects the size of the container, the
//         operator[] or the GetAt method should be use with caution. A good
//         practice is to use the GetSize method within the iteration scope
//         such as in the following example:
//
//          <CODE>
//              CVector<SomeStruct> vecSomeStruct;
//              unsigned int i = 0;
//              for ( i = 0 ; i < vecSomeStruct.GetSize(); i++ )
//              {
//                  if ( vecSomeStruct[i].bToErase )
//                  {
//                      vecSomeStruct.Erase(i);
//                      i--;
//                  }
//              }
//          </CODE>
//
//          Another good practice is to iterate from the end such as in the
//          following example:
//
//          <CODE>
//              CVector<SomeStruct> vecSomeStruct;
//              unsigned int i = 0;
//              for ( i = vecSomeStruct.GetSize(); i > 0; i-- )
//              {
//                  if ( vecSomeStruct[i-1].bToErase )
//                  {
//                      vecSomeStruct.Erase(i-1);
//                  }
//              }
//          </CODE>
//
//      7. CVector fully supports incomparable types such as structures or
//         complex objects. However, the sort methods (InsertSorted, Sort,
//         Find, FindSorted, GetMinElementIndex, and GetMaxElementIndex) can
//         not be used except if the structure or the complex object implements
//         the != and < operators. Alternatively, you can use the CUncmp
//         template helper and provide your own compare function. Please refer
//         to the CUncmp class documentation for more information and code
//         examples.
//
// Warning:
//  This container is not thread safe.
//
//  Since the CVector exports methods that reorder objects, and reordering means
//  to change the 'this' pointer of the object itself and of the aggregated
//  objects, problems of referencing will occur if special care is not taken to
//  update these references.
//
//  If someone however decides to store C++ objects in a CVector, and agrees to
//  never move them since they do not support moving, it must be well documented
//  since the potential for a problem to occur is still there. This practice is
//  not recommended.
//
//  For C++ objects, you should normally store the object's pointer in the
//  CVector (which yields to an access time of the same order), or use a CList
//  or CVList when appropriate.
//
// Location:
//   Cap/CVector.h
//
// See also:
//   CList, CVlist
//
//==EDOC========================================================================
template<class _Type>
class CVector : protected CVectorBase
{
//M5T_INTERNAL_USE_BEGIN
    friend class CQueue<_Type, CVector<_Type> >;
    friend class CStack<_Type, CVector<_Type> >;

#if defined (MXD_COMPILER_MS_VC) && (MXD_COMPILER_MS_VC < 1310)
    friend CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CVector<_Type>& rVector);
    friend CMarshaler& operator>> (IN CMarshaler& rMarshaler, IN CVector<_Type>& rVector);
#else
    friend CMarshaler& operator<< <_Type>(IN CMarshaler& rMarshaler, IN const CVector<_Type>& rVector);
    friend CMarshaler& operator>> <_Type>(IN CMarshaler& rMarshaler, IN CVector<_Type>& rVector);
#endif
//M5T_INTERNAL_USE_END

// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //     Default constructor.
    //------------------
    CVector(IN IAllocator* pAllocator = NULL);

    // Summary:
    //     Constructor.
    //------------------
    CVector(IN bool bUseVirtual, IN IAllocator* pAllocator = NULL);

    // Summary:
    //     Copy Constructor.
    //-----------------------
    CVector(IN const CVector<_Type>& rVector);

    // Summary:
    //     Destructor.
    //-----------------
    virtual ~CVector();

    // Summary:
    //     Assignment operator.
    //--------------------------
    CVector<_Type>& operator=(IN const CVector<_Type>& rVector);

    // Summary:
    //     Comparison operator.
    //--------------------------
    bool operator==(IN const CVector<_Type>& rVector) const;

    // Summary:
    //     Different than operator.
    //------------------------------
    bool operator!=(IN const CVector<_Type>& rVector) const;

    // Summary:
    //     Less than operator.
    //-------------------------
    bool operator<(IN const CVector<_Type>& rVector) const;

    // Summary:
    //     Greater than operator.
    //----------------------------
    bool operator>(IN const CVector<_Type>& rVector) const;

    // Summary:
    //     Less than or equal to operator.
    //----------------------------------------
    bool operator<=(IN const CVector<_Type>& rVector) const;

    // Summary:
    //     Greater than or equal to operator.
    //----------------------------------------
    bool operator>=(IN const CVector<_Type>& rVector) const;

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
    //     Gets the size of the vector.
    //----------------------------------
    unsigned int GetSize() const;

    // Summary:
    //     Checks if the vector is empty.
    //------------------------------------
    bool IsEmpty() const;

    // Summary:
    //     Checks if the vector is full.
    //-----------------------------------
    bool IsFull() const;

    // Index operations.
    //-------------------

    // Summary:
    //     Gets the first used index.
    //--------------------------------
    unsigned int GetFirstIndex() const;

    // Summary:
    //     Gets the last used element.
    //---------------------------------
    unsigned int GetLastIndex() const;

    // Summary:
    //     Gets the index of the first unused element.
    unsigned int GetEndIndex() const;

    // Direct element access.
    //------------------------

    // Summary:
    //     Gets the element at the specified index(const).
    //-----------------------------------------------------
    const _Type& operator[](IN unsigned int uIndex) const;

    // Summary:
    //     Gets the element at the specified index.
    //----------------------------------------------
    _Type& operator[](IN unsigned int uIndex);

    // Summary:
    //     Gets the element at the specified index(const).
    //-----------------------------------------------------
    const _Type& GetAt(IN unsigned int uIndex) const;

    //<COMBINE CVector::GetAt@IN unsigned int@const>
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
    //      Inserts the element contained within another vector.
    //-----------------------------------------------------------
    mxt_result Insert(IN unsigned int uIndex, IN const CVector& rVector);

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
    //------------------------------------------------
    void Erase(IN unsigned int uIndex);

    // Summary:
    //     Erases a number of elements at the specified index.
    //---------------------------------------------------------
    void Erase(IN unsigned int uIndex, IN unsigned int uCount);

    // Summary:
    //     Erases all elements.
    //-------------------------
    void EraseAll();

    // Advanced operations.
    //----------------------

    // Summary:
    //     Inserts one element but does not construct it.
    //-----------------------------------------------------
    void* Allocate(IN unsigned int uIndex);

    //  Summary:
    //      Moves all the elements of another vector to a specific index.
    //--------------------------------------------------------------------
    mxt_result Merge(IN unsigned int uIndex, INOUT CVector& rVector);

    //  Summary:
    //      Moves some elements from this vector to another starting from a
    //      specific index.
    //----------------------------------------------------------------------
    mxt_result Split(IN unsigned int uIndex, OUT CVector& rVector);

    // Summary:
    //     Exchanges the position of two elements.
    //---------------------------------------------
    void Swap(IN unsigned int uFirstIndex, IN unsigned int uSecondIndex);

    // Summary:
    //      Sorts the elements in the container using the shellsort algorithm.
    //-------------------------------------------------------------------------
    void Sort(IN int (pfnCompare)(IN const _Type& rOneElement,
                                  IN const _Type& rOtherElement,
                                  IN  mxt_opaque opq) = NULL,
              IN  mxt_opaque opq = 0);

    // Summary:
    //     Performs a sequential search for an element.
    //--------------------------------------------------
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
    //     Performs a sequential search for the "biggest" element.
    //-------------------------------------------------------------
    unsigned int GetMaxElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                        IN const _Type& rOtherElement,
                                                        IN  mxt_opaque opq) = NULL,
                                    IN  mxt_opaque opq = 0) const;


    // CMap operations.  Do not use outside the CMap class.
    //-----------------------------------------------------

    //  Summary:
    //      Allocates a memory zone in the proper location in the list.
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

    //<COMBINE CVector::FindPtrSorted@IN const _Type&@const>
    _Type* FindPtrSorted(IN const _Type& rElement);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // Stack operations.
    //-------------------

    // Summary:
    //      Retrieves the element at the top without erasing it.
    //-----------------------------------------------------------
    const _Type& GetTop() const;

    // Summary:
    //     Retrieves the element at the top without erasing it.
    //----------------------------------------------------------
    _Type& GetTop();

    // Summary:
    //      Erases the element at the top.
    //-------------------------------------
    void Pop();

    // Summary:
    //      Inserts an element at the top.
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
//==  CVector
//==
//==============================================================================
//
//  Parameters:
//    pAllocator:
//      Allows to specify an allocator that is used to allocate and free the
//      internal buffer.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CVector<_Type>::CVector(IN IAllocator* pAllocator)
: CVectorBase(sizeof(_Type), pAllocator)
{
    m_stCompareParams.m_pfnCompare = NULL;
    m_stCompareParams.m_opq = MX_INT32_TO_OPQ(0);
}

//==SDOC========================================================================
//==
//==  CVector
//==
//==============================================================================
//
//  Parameters:
//    bUseVirtual:
//      True if the constructor and destructor should always be called.
//      False if memcpy, memmove, and memset may be used instead. Set to TRUE
//      by default.
//
//    pAllocator:
//      Allows to specify an allocator that is used to allocate and free the
//      internal buffer.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CVector<_Type>::CVector(IN bool bUseVirtual, IN IAllocator* pAllocator)
: CVectorBase(sizeof(_Type), bUseVirtual, pAllocator)
{
    m_stCompareParams.m_pfnCompare = NULL;
    m_stCompareParams.m_opq = MX_INT32_TO_OPQ(0);
}

//==SDOC========================================================================
//==
//==  CVector
//==
//==============================================================================
//
//  Parameters:
//    rVector:
//      A reference to the source vector.
//
//  Description:
//      Copy Constructor.
//
//==EDOC========================================================================
template<class _Type>
inline
CVector<_Type>::CVector(IN const CVector<_Type>& rVector)
: CVectorBase(rVector.m_uSizeOfType,rVector.m_bUseVirtual,rVector.m_pAllocator)
{
    *this = rVector;
}

//==SDOC====================================================================
//==
//==  ~CVector
//==
//==========================================================================
//
//  Description:
//      Destructor.
//
//==EDOC====================================================================
template<class _Type>
inline
CVector<_Type>::~CVector()
{
    EraseAll();
}

//==SDOC====================================================================
//==
//==  operator=
//==
//==========================================================================
//
//  Parameters:
//    rVector:
//      Reference to the list to compare.
//  Returns:
//      Reference to the assigned vector.
//
//  Description:
//      Assigns the right hand CVector to the left hand one.
//
//==EDOC====================================================================
template<class _Type>
inline
CVector<_Type>& CVector<_Type>::operator=(IN const CVector<_Type>& rVector)
{
    CVectorBase::operator=(rVector);
    m_stCompareParams.m_pfnCompare = rVector.m_stCompareParams.m_pfnCompare;
    m_stCompareParams.m_opq = rVector.m_stCompareParams.m_opq;
    return *this;
}

//==SDOC====================================================================
//==
//==  operator==
//==
//==========================================================================
//
//  Parameters:
//    rVector:
//      Reference to the list to compare.
//  Returns:
//      Returns true if both vectors are identical, otherwise returns false.
//
//  Description:
//      Verifies that the left hand vector is identical to the right hand
//      vector. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour
//      is identical to the one used in strcmp.
//
//==EDOC====================================================================
template<class _Type>
inline
bool CVector<_Type>::operator==(IN const CVector<_Type>& rVector) const
{
    return !(*this != rVector);
}

//==SDOC====================================================================
//==
//==  operator!=
//==
//==========================================================================
//
//  Parameters:
//    rVector:
//      Reference to the list to compare.
//  Returns:
//      Returns true if both vectors are different, otherwise returns false.
//
//  Description:
//      Verifies that the left hand vector is different than the right hand
//      vector. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour
//      is identical to the one used in strcmp.
//
//==EDOC====================================================================
template<class _Type>
bool CVector<_Type>::operator!=(IN const CVector<_Type>& rVector) const
{
    if (this == &rVector)
    {
        return false;
    }

    if (!m_bUseVirtual)
    {
        return CVectorBase::operator!=(rVector);
    }

    if (m_uSizeOfType   != rVector.m_uSizeOfType ||
        m_uSizeOfVector != rVector.m_uSizeOfVector)
    {
        return true;
    }

    unsigned int uIndex = 0;
    bool bNotEqual = false;

    while (uIndex < m_uSizeOfVector && bNotEqual == false)
    {
        if (!(GetAt(uIndex) == rVector.GetAt(uIndex)))
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
//    rVector:
//      Reference to the list to compare.
//  Returns:
//      Returns true if the left hand vector is less than the right hand
//      vector, otherwise returns false.
//
//  Description:
//      Verifies that the left hand vector is less than the right hand
//      vector. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour
//      is identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
bool CVector<_Type>::operator<(IN const CVector<_Type>& rVector) const
{
    if (this == &rVector)
    {
        return false;
    }

    if (!m_bUseVirtual)
    {
        return CVectorBase::operator<(rVector);
    }

    unsigned int uIndex = 0;
    bool bLessThan = false;
    unsigned int uSizeOfList = MX_MIN(GetSize(), rVector.GetSize());

    while (uIndex < uSizeOfList && bLessThan == false)
    {
        if (GetAt(uIndex) < rVector.GetAt(uIndex))
        {
            bLessThan = true;
        }

        uIndex++;
    }

    if(bLessThan == false)
    {
        bLessThan = GetSize() < rVector.GetSize() ? true : false;
    }
    return bLessThan;
}

//==SDOC========================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//    rVector:
//      Reference to the list to compare.
//  Returns:
//      Returns true if the left hand vector is greater than the right hand
//      vector, otherwise returns false.
//
//  Description:
//      Verifies that the left hand vector is greater than the right hand
//      vector. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour
//      is identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVector<_Type>::operator>(IN const CVector<_Type>& rVector) const
{
    return (!(*this < rVector) && *this != rVector);
}

//==SDOC========================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//    rVector:
//      Reference to the list to compare.
//  Returns:
//      Returns true if the left hand vector is less than or equal to the right
//      hand vector, otherwise returns false.
//
//  Description:
//      Verifies that the left hand vector is less than or equal to the right
//      hand vector. This is done by comparing containers element by element
//      and returns once an element is different from the other. This behaviour
//      is identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVector<_Type>::operator<=(IN const CVector<_Type>& rVector) const
{
    return !(*this > rVector);
}

//==SDOC========================================================================
//==
//==  operator>=
//==
//==============================================================================
//
//  Parameters:
//    rVector:
//      Reference to the list to compare.
//  Returns:
//      Returns true if the left hand vector is greater than or equal to the
//      right hand vector, otherwise returns false.
//
//  Description:
//      Verifies that the left hand vector is greater than or equal to the right
//      hand vector. This is done by comparing containers element by element and
//      returns once an element is different from the other. This behaviour is
//      identical to the one used in strcmp.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVector<_Type>::operator>=(IN const CVector<_Type>& rVector) const
{
    return !(*this < rVector);
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
mxt_result CVector<_Type>::SetComparisonFunction(IN int (pfnCompare)(IN const _Type& rOneElement,
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
unsigned int CVector<_Type>::GetCapacity() const
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
//      Returns the lock capacity count. The lock capacity count is a counter
//      that is increased each time LockCapacity is called and decreased each
//      time UnlockCapacity is called. If the lock capacity count is greater
//      than zero, this means that any call to ReduceCapacity or ReserveCapacity
//      fails.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CVector<_Type>::GetLockCapacity() const
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
//      Locks the capacity. The lock capacity count is a counter that is
//      increased each time LockCapacity is called and decreased each time
//      UnlockCapacity is called. If the lock capacity count is greater than
//      zero, this means that any call to ReduceCapacity or ReserveCapacity
//      fails.
//
//==EDOC========================================================================
template<class _Type>
inline
void CVector<_Type>::LockCapacity()
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
//      Reduces the capacity. The capacity is reduced to uDownToCapacity if the
//      size of the CVector is below uDownToCapacity and if uDownToCapacity is
//      below the current capacity. This method fails if the lock capacity count
//      is not 0.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVector<_Type>::ReduceCapacity(IN unsigned int uDownToCapacity)
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
//      Increases the capacity. The capacity is increased to uUpToCapacity if
//      uUpToCapacity is greater than the current capacity. This method fails if
//      the lock capacity count is not 0 and the wanted capacity is greater than
//      the current capacity.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVector<_Type>::ReserveCapacity(IN unsigned int uUpToCapacity)
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
//      Unlocks the capacity. The lock capacity count is a counter that is
//      increased each time LockCapacity is called and decreased each time
//      UnlockCapacity is called. If the lock capacity count is greater than
//      zero, this means that any call to ReduceCapacity or ReserveCapacity
//      fails.
//
//==EDOC========================================================================
template<class _Type>
inline
void CVector<_Type>::UnlockCapacity()
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
//      The size.
//
//  Description:
//      Returns the size, which is the number of elements that are allocated and
//      already in use.
//
//==EDOC========================================================================
template<class _Type>
inline
unsigned int CVector<_Type>::GetSize() const
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
//      Returns true if the size is 0; in other words, if no elements are
//      currently in use.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CVector<_Type>::IsEmpty() const
{
    return CVectorBase::IsEmpty();
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
bool CVector<_Type>::IsFull() const
{
    return CVectorBase::IsFull();
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
unsigned int CVector<_Type>::GetFirstIndex() const
{
    return CVectorBase::GetFirstIndex();
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
unsigned int CVector<_Type>::GetLastIndex() const
{
    return CVectorBase::GetLastIndex();
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
unsigned int CVector<_Type>::GetEndIndex() const
{
    return CVectorBase::GetEndIndex();
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
const _Type& CVector<_Type>::operator[](IN unsigned int uIndex) const
{
    return *reinterpret_cast<const _Type *>(CVectorBase::operator[](uIndex));
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
_Type& CVector<_Type>::operator[](IN unsigned int uIndex)
{
    return *reinterpret_cast<_Type *>(CVectorBase::operator[](uIndex));
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
const _Type& CVector<_Type>::GetAt(IN unsigned int uIndex) const
{
    return *reinterpret_cast<const _Type *>(CVectorBase::GetAt(uIndex));
}

template<class _Type>
inline
_Type& CVector<_Type>::GetAt(IN unsigned int uIndex)
{
    return *reinterpret_cast<_Type *>(CVectorBase::GetAt(uIndex));
}

//==SDOC========================================================================
//==
//==  Append
//==
//==============================================================================
//
//  Parameters:
//    rElement:
//      A reference to an element that is used to construct a new one in the
//      CVector.
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
mxt_result CVector<_Type>::Append(IN const _Type& rElement)
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
//      Inserts one or more elements and constructs them. The capacity is
//      increased if there are not enough allocated elements to accommodate the
//      insertion of the new elements. The lock capacity count is still enforced
//      and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVector<_Type>::Insert(IN unsigned int uIndex,
                                  IN unsigned int uCount)
{
    return CVectorBase::Insert(uIndex, uCount);
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
mxt_result CVector<_Type>::Insert(IN unsigned int uIndex,
                                  IN unsigned int uCount,
                                  IN const _Type& rElement)
{
    return CVectorBase::Insert(uIndex, uCount, &rElement);
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
//    rVector:
//      A vector that contains the elements to insert.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Inserts the elements contained within another vector to a specified
//      index. The capacity is increased if there are not enough allocated
//      elements to accommodate the insertion of the new elements. The lock
//      capacity count is still enforced and if it is not zero, the insertion
//      fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVector<_Type>::Insert(IN unsigned int uIndex,
                                  IN const CVector& rVector)
{
    return CVectorBase::Insert(uIndex, rVector);
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
mxt_result CVector<_Type>::InsertSorted(IN const _Type& rElement,
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
        return CVectorBase::InsertSorted(&rElement,
                                         CVector<_Type>::Compare,
                                         MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVectorBase::InsertSorted(&rElement,
                                         CVector<_Type>::Compare,
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
void CVector<_Type>::Erase(IN unsigned int uIndex)
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
//      The index of the first element to erase.
//
//    uCount:
//      The number of elements to erase.
//
//  Description:
//      Erases multiple elements, beginning with element at uIndex.
//
//==EDOC========================================================================
template<class _Type>
inline
void CVector<_Type>::Erase(IN unsigned int uIndex, IN unsigned int uCount)
{
    CVectorBase::Erase(uIndex, uCount);
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
void CVector<_Type>::EraseAll()
{
    CVectorBase::EraseAll();
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
void CVector<_Type>::Pop()
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
mxt_result CVector<_Type>::Push(IN const _Type& rElement)
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
const _Type& CVector<_Type>::GetTop() const
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
_Type& CVector<_Type>::GetTop()
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
void CVector<_Type>::Dequeue()
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
//      Inserts an element at the queue's last index.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVector<_Type>::Enqueue(IN const _Type& rElement)
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
const _Type& CVector<_Type>::GetFront() const
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
_Type& CVector<_Type>::GetFront()
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
//      to accommodate the insertion of the new elements. The lock capacity
//      count is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
void* CVector<_Type>::Allocate(IN unsigned int uIndex)
{
    return CVectorBase::Allocate(uIndex);
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
//    rVector:
//      The source vector.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Moves all the elements of another vector to uIndex. The capacity is
//      increased if there are not enough allocated elements to accommodate the
//      insertion of the new elements. The lock capacity count is still enforced
//      and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVector<_Type>::Merge(IN unsigned int uIndex, INOUT CVector& rVector)
{
    return CVectorBase::Merge(uIndex, rVector);
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
//    rVector:
//      The destination vector.
//
//  Returns:
//      resS_OK
//      resFE_OUT_OF_MEMORY
//
//  Description:
//      Moves some elements from this vector to another starting from a specific
//      index. The destination vector is emptied first. The capacity of the
//      target vector is increased if there are not enough allocated elements to
//      accommodate the insertion of the new elements. The lock capacity count
//      is still enforced and if it is not zero, the insertion fails.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVector<_Type>::Split(IN unsigned int uIndex, OUT CVector& rVector)
{
    return CVectorBase::Split(uIndex, rVector);
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
void CVector<_Type>::Swap(IN unsigned int uFirstIndex,
                          IN unsigned int uSecondIndex)
{
    CVectorBase::Swap(uFirstIndex, uSecondIndex);
}

//M5T_INTERNAL_USE_BEGIN
template<class _Type>
void CVector<_Type>::Construct(OUT void* pvoid)
{
    new (pvoid) _Type;
}

template<class _Type>
void CVector<_Type>::ConstructFrom(OUT void* pvoidDestination,
                                   IN const void* pvoidSource)
{
    new (pvoidDestination) _Type(*reinterpret_cast<const _Type*>(pvoidSource));
}

template<class _Type>
void CVector<_Type>::Destruct(OUT void* pvoid)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pvoid);

    reinterpret_cast<_Type*>(pvoid)->~_Type();
}

template<class _Type>
int CVector<_Type>::Compare(IN const void* pOneElement,
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
//      Sorts the elements in the container using the shellsort algorithm.
//      The pfnCompare pointer can be used to change the algorithm behaviour.
//
//==EDOC========================================================================
template<class _Type>
void CVector<_Type>::Sort(IN int (pfnCompare)(IN const _Type& rOneElement,
                                              IN const _Type& rOtherElement,
                                              IN  mxt_opaque opq),
                          IN  mxt_opaque opq)
{
    if(pfnCompare !=NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        CVectorBase::Sort(CVector<_Type>::Compare, MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        CVectorBase::Sort(CVector<_Type>::Compare, MX_VOIDPTR_TO_OPQ(&m_stCompareParams));
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
//      Performs a sequencial search for rElement, starting at uStartIndex
//      and returns the index of the first occurrence. The pfnCompare pointer
//      can be used to change the algorithm behaviour.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CVector<_Type>::Find(IN unsigned int uStartIndex,
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
        return CVectorBase::Find(uStartIndex,
                                 &rElement,
                                 CVector<_Type>::Compare,
                                 MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVectorBase::Find(uStartIndex,
                                 &rElement,
                                 CVector<_Type>::Compare,
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
//      Performs a binary search for rElement on a sorted container and returns
//      the index of the first occurrence. The pfnCompare pointer can be used to
//      change the algorithm behaviour. The behaviour is undefined if the
//      container has not been sorted.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CVector<_Type>::FindSorted(IN const _Type& rElement,
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
        return CVectorBase::FindSorted(&rElement,
                                       CVector<_Type>::Compare,
                                       MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVectorBase::FindSorted(&rElement,
                                       CVector<_Type>::Compare,
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
unsigned int CVector<_Type>::GetMinElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                                    IN const _Type& rOtherElement,
                                                                    IN  mxt_opaque opq),
                                                IN  mxt_opaque opq) const
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CVectorBase::GetMinElementIndex(CVector<_Type>::Compare,
                                               MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVectorBase::GetMinElementIndex(CVector<_Type>::Compare,
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
//      The index of the "biggest" element found in the container.  An empty
//      container always returns 0.
//
//  Description:
//      Performs a sequential search for the "biggest" element of an unsorted
//      container. The pfnCompare pointer can be used to change the algorithm
//      behaviour.
//
//==EDOC========================================================================
template<class _Type>
unsigned int CVector<_Type>::GetMaxElementIndex(IN int (pfnCompare)(IN const _Type& rOneElement,
                                                                    IN const _Type& rOtherElement,
                                                                    IN  mxt_opaque opq),
                                                IN  mxt_opaque opq) const
{
    if(pfnCompare != NULL)
    {
        SCompareParams stCompareParams;
        stCompareParams.m_pfnCompare = pfnCompare;
        stCompareParams.m_opq = opq;
        return CVectorBase::GetMaxElementIndex(CVector<_Type>::Compare,
                                               MX_VOIDPTR_TO_OPQ(&stCompareParams));
    }
    else
    {
        return CVectorBase::GetMaxElementIndex(CVector<_Type>::Compare,
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
//     This method allocates a memory zone at the right position in the vector
//     using rElement.  The allocated zone then needs to be initialized by the
//     user. This method is useful for the map because the comparison function
//     only compares the keys, so the second element of the pair given to this
//     function can be left uninitialized.
//
//  NOTES:
//      This method is needed to provide the CMap class a uniform interface
//      between containers. It must NEVER be used outside of the CMap class.
//
//==EDOC========================================================================
template<class _Type>
inline
mxt_result CVector<_Type>::AllocateSorted(IN const _Type& rElement,
                                          OUT void** ppAllocatedZone)
{
    return CVectorBase::AllocateSorted(&rElement,
                                       CVector<_Type>::Compare,
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
void CVector<_Type>::EraseSorted(IN const _Type& rElement)
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
const _Type* CVector<_Type>::FindPtrSorted(IN const _Type& rElement) const
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
_Type* CVector<_Type>::FindPtrSorted(IN const _Type& rElement)
{
    _Type* pType = NULL;
    unsigned int uIndex = FindSorted(rElement);
    if (uIndex != GetEndIndex())
    {
        pType = &(GetAt(uIndex));
    }

    return (pType);
}

//M5T_INTERNAL_USE_BEGIN
template < class _Type >
inline
CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CVector< _Type >& rVector)
{
    rMarshaler << rVector.GetCapacity();
    rMarshaler << rVector.GetSize();

    // Marshal each element.
    unsigned int uElementIndex = 0;
    for(; uElementIndex < rVector.GetSize(); uElementIndex++)
    {
        rMarshaler << rVector[uElementIndex];
    }

    return rMarshaler;
}

template < class _Type >
inline
CMarshaler& operator>> (IN CMarshaler& rMarshaler, IN CVector< _Type >& rVector)
{
    // Remove all elements from the CVector.
    rVector.EraseAll();

    // Get the requested capacity, and reserve space in the CVector.
    unsigned int uCapacity = 0;
    rMarshaler >> uCapacity;
    rVector.ReserveCapacity(uCapacity);

    // Get all the elements.
    unsigned int uElementCount = 0;
    rMarshaler >> uElementCount;
    uint32_t uElementIndex = 0;
    for(; uElementIndex < uElementCount; uElementIndex++)
    {
        // Call the constructor (with the placement new)
        // to initialize the vector element.
        rVector.Insert(uElementIndex,1);

        // Unmarshal element directly onto the vector element.
        rMarshaler >> rVector.GetAt(uElementIndex);
    }

    return rMarshaler;
}
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CVECTOR_H

