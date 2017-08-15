//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CQUEUE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CQUEUE_H
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

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
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

//@@CQueue
//==SDOC========================================================================
//== Class: CQueue
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//   Interface for basic containers so they can be used as a standard queue.
//
// Description:
//
//   This template class implements an interface to a basic container (CList,
//   CVList, or CVector) to be used in a standard "queue" (FIFO) manner. It is
//   provided for code readability reasons. Therefore, when the required
//   container needs to be used as a queue, the CQueue interface should be
//   preferred to the direct access to the basic container interface.
//
//   Memory management is left to the container.
//
//   The initialization of a CQueue based on a CList looks as follows:
//      <CODE>
//          CQueue<int, CList<int> > queue;
//      </CODE>
//
//   The initialization of a CQueue with the default container looks as follows:
//      <CODE>
//          CQueue<int> queue;
//      </CODE>
//
//   The signature of a method receiving a queue and an array of elements to be
//   inserted into this queue would look as follows (see CQueue test case for
//   details)
//      <CODE>
//          template <class _Queue, class _Type>
//          void TestAll(IN _Queue queue, IN _Type* pDataArray)
//      </CODE>
//
//   The CQueue class currently supports the CVector, CList, and CVList
//   containers.  CList has been chosen as the default container, since it is
//   the container of choice for fast insertion/suppression at the container's
//   extremities and this is precisely where the queue implementation performs
//   its enqueue and dequeue operations on a CList.
//
// Warning:
//  This class is not thread safe and has the same features and limitations as
//  the container used to implement it.  Refer to the basic container's
//  documentation to know which one to use.
//
//  Make sure to reserve enough memory before you begin inserting elements
//  into a queue object.  This is done with a call to the ReserveCapacity
//  method. Otherwise, if you build up a queue of N elements with consecutive
//  calls such as the following:
//      <CODE>
//          queue.Enqueue(element);
//      </CODE>
//  you will cause N memory reallocations!  Thus, a queue built with calls to
//  Push without any previous memory allocation requires a lot more computation
//  than building the same queue after a call to ReserveCapacity.
//
// Location:
//   Cap/Cqueue.h
//
// See also:
//   CVList, CList, CVector, CStack
//
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
// GCC 2.7.2 does not support default parameters in templates,
// in which case all the parameters must be specified.
template<class _Type, class _Container>
#else
//M5T_INTERNAL_USE_END
template<class _Type, class _Container = CList<_Type> >
//M5T_INTERNAL_USE_BEGIN
#endif
//M5T_INTERNAL_USE_END
class CQueue
{
// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CQueue
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Parameters:
    //    pAllocator:
    //      Allows to specify an allocator that is used to allocate and free the
    //      individual elements.
    //
    //  Description:
    //      Constructor.
    //
    //==EDOC====================================================================
    CQueue(IN IAllocator* pAllocator = NULL);

    //==SDOC====================================================================
    //==
    //==  CQueue
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~CQueue();

    //==SDOC====================================================================
    //==
    //==  CQueue
    //==
    //==========================================================================
    //
    //  Summary:
    //      Copy Constructor.
    //
    //  Parameters:
    //    rQueue:
    //      A reference to the source queue.
    //
    //  Description:
    //      Copy Constructor.
    //
    //==EDOC====================================================================
    CQueue(IN const CQueue<_Type, _Container>& rQueue);

    //==SDOC====================================================================
    //==
    //==  operator=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Assignment operator.
    //
    //  Parameters:
    //    rQueue:
    //      Reference to the CQueue object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      A reference to this CQueue instance.
    //
    //  Description:
    //      Assignment operator.
    //
    //==EDOC====================================================================
    CQueue<_Type, _Container>& operator=
                                   (IN const CQueue<_Type, _Container>& rQueue);

    //==SDOC====================================================================
    //==
    //==  operator==
    //==
    //==========================================================================
    //
    //  Summary:
    //      Comparison operator.
    //
    //  Parameters:
    //    rQueue:
    //      Reference to the CQueue object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if both queues are equal in size and their internal
    //      content is also equal, else returns false.
    //
    //  Description:
    //      Verifies that both queues are equal. This is done by comparing each
    //      container element by element and returns once an element is
    //      different from the other. This behaviour is identical to the one
    //      used in strcmp.
    //
    //==EDOC====================================================================
    bool operator== (IN const CQueue<_Type, _Container>& rQueue) const;

    //==SDOC====================================================================
    //==
    //==  operator!=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Different from operator.
    //
    //  Parameters:
    //    rQueue:
    //      Reference to the CQueue object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if either the size or the content of the queues are
    //      different, else returns false
    //
    //  Description:
    //      Verifies that the left hand queue is different from the right hand
    //      queue. This is done by comparing containers element by element and
    //      returns once an element is different from the other. This behaviour
    //      is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator!= (IN const CQueue<_Type, _Container>& rQueue) const;

    //==SDOC====================================================================
    //==
    //==  operator<
    //==
    //==========================================================================
    //
    //  Summary:
    //      Less than operator.
    //
    //  Parameters:
    //    rQueue:
    //      Reference to the CQueue object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if left hand queue is smaller than the right hand
    //      queue, else returns false.
    //
    //  Description:
    //      Verifies that the left hand queue is less than the right hand queue.
    //      This is done by comparing containers element by element and returns
    //      once an element is different from the other. This behaviour is
    //      identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator< (IN const CQueue<_Type, _Container>& rQueue) const;

    //==SDOC====================================================================
    //==
    //==  operator>
    //==
    //==========================================================================
    //
    //  Summary:
    //      Greater than operator.
    //
    //  Parameters:
    //    rQueue:
    //      Reference to the CQueue object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand queue is greater than the right hand
    //      queue, else returns false.
    //
    //  Description:
    //      Verifies that the left hand queue is greater than the right hand
    //      queue. This is done by comparing containers element by element and
    //      returns once an element is different from the other. This behaviour
    //      is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator> (IN const CQueue<_Type, _Container>& rQueue) const;

    //==SDOC====================================================================
    //==
    //==  operator<=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Less than or equal to operator.
    //
    //  Parameters:
    //    rQueue:
    //      Reference to the CQueue object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand queue is less than or equal to the
    //      right hand queue, else returns false.
    //
    //  Description:
    //      Verifies that the left hand queue is less than or equal to the right
    //      hand queue. This is done by comparing containers element by element
    //      and returns once an element is different from the other. This
    //      behaviour is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator<= (IN const CQueue<_Type, _Container>& rQueue) const;

    //==SDOC====================================================================
    //==
    //==  operator>=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Greater than or equal to operator.
    //
    //  Parameters:
    //    rQueue:
    //      Reference to the CQueue object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand queue is greater than or equal to the
    //      right hand queue, else returns false.
    //
    //  Description:
    //      Verifies that the left hand queue is greater than or equal to the
    //      right hand queue. This is done by comparing containers element by
    //      element and returns once an element is different from the other.
    //      This behaviour is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator>= (IN const CQueue<_Type, _Container>& rQueue) const;

    // Capacity operations (Max element count)
    //----------------------------------------

    //==SDOC====================================================================
    //==
    //==  GetCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the capacity.
    //
    //  Returns:
    //      The capacity.
    //
    //  Description:
    //      Returns the capacity. The capacity is the number of elements already
    //      allocated that may or may not be already in use.
    //
    //==EDOC====================================================================
    unsigned int GetCapacity() const;

    //==SDOC====================================================================
    //==
    //==  GetLockCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the lock capacity count.
    //
    //  Returns:
    //      The lock capacity count.
    //
    //  Description:
    //      Returns the lock capacity count. The lock capacity count is a
    //      counter that is increased each time LockCapacity is called and
    //      decreased each time UnlockCapacity is called. As long as the lock
    //      capacity count is greater than zero, any call to ReduceCapacity or
    //      ReserveCapacity fails.
    //
    //==EDOC====================================================================
    unsigned int GetLockCapacity() const;

    //==SDOC====================================================================
    //==
    //==  LockCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Locks the capacity.
    //
    //  Description:
    //      Locks the capacity. The lock capacity count is a counter that is
    //      increased each time LockCapacity is called and decreased each time
    //      UnlockCapacity is called. As long as the lock capacity count is
    //      greater than zero, any call to ReduceCapacity or ReserveCapacity
    //      fails.
    //
    //==EDOC====================================================================
    void LockCapacity();

    //==SDOC====================================================================
    //==
    //==  ReduceCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reduces the capacity.
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
    //      Reduces the capacity to uDownToCapacity if its value is below the
    //      current capacity. If uDownToCapacity is lower than the current queue
    //      size, the capacity is reduced down to the queue size (no elements
    //      are lost).
    //
    //  Warning:
    //      The method fails if the lock capacity count is not 0.
    //
    //==EDOC====================================================================
    mxt_result ReduceCapacity(IN unsigned int uDownToCapacity);

    //==SDOC====================================================================
    //==
    //==  ReserveCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Increases the capacity.
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
    //      Increases the capacity. The capacity is increased to uUpToCapacity
    //      if uUpToCapacity is greater than the current capacity.
    //
    //  Warning:
    //      The method fails if the lock capacity count is not 0 and the wanted
    //      capacity is greater than the current capacity.
    //
    //==EDOC====================================================================
    mxt_result ReserveCapacity(IN unsigned int uUpToCapacity);

    //==SDOC====================================================================
    //==
    //==  UnlockCapacity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Unlocks the capacity.
    //
    //  Description:
    //      Unlocks the capacity. The lock capacity count is a counter that is
    //      increased each time LockCapacity is called and decreased each time
    //      UnlockCapacity is called. If the lock capacity count is greater than
    //      zero, this means that any call to ReduceCapacity or ReserveCapacity
    //      fails.
    //
    //==EDOC====================================================================
    void UnlockCapacity();

    // Size operations (Element count)
    //--------------------------------

    //==SDOC====================================================================
    //==
    //==  GetSize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the size.
    //
    //  Returns:
    //      The size.
    //
    //  Description:
    //      Returns the size. This is the number of elements that are allocated
    //      and already in use.
    //
    //==EDOC====================================================================
    unsigned int GetSize() const;

    //==SDOC====================================================================
    //==
    //==  IsEmpty
    //==
    //==========================================================================
    //
    //  Summary:
    //      Queries if the queue is empty (queue size = 0).
    //
    //  Returns:
    //      True if the size is 0.
    //
    //  Description:
    //      Returns true if the queue size is 0. An empty queue is a queue in
    //      which no elements are currently in use.
    //
    //==EDOC====================================================================
    bool IsEmpty() const;

    //==SDOC====================================================================
    //==
    //==  IsFull
    //==
    //==========================================================================
    //
    //  Summary:
    //      Queries if the queue is full (queue size = max capacity).
    //
    //  Returns:
    //      True if the size is equal to the capacity of the container.
    //
    //  Description:
    //      Returns true if the container used by the current queue object
    //      contains a number of elements equal to its capacity.  It means that
    //      any new insertion of elements into this queue causes a memory
    //      reallocation.
    //
    //==EDOC====================================================================
    bool IsFull() const;

    //==SDOC====================================================================
    //==
    //==  EraseAll
    //==
    //==========================================================================
    //
    //  Summary:
    //      Clears the queue content.
    //
    //  Description:
    //      Erases all elements from this queue object. The size is set to 0.
    //
    //==EDOC====================================================================
    void EraseAll();

    // Queue operations.
    //-------------------

    //==SDOC====================================================================
    //==
    //==  Dequeue
    //==
    //==========================================================================
    //
    //  Summary:
    //      Erases the element at the queue's first index.
    //
    //  Description:
    //      Erases the element at the queue's first index.
    //
    //==EDOC====================================================================
    void Dequeue();

    //==SDOC====================================================================
    //==
    //==  Enqueue
    //==
    //==========================================================================
    //
    //  Summary:
    //      Inserts the element at the queue's last index.
    //
    //  Parameters:
    //      rElement:
    //       The element to be inserted.
    //
    //  Returns:
    //      resS_OK
    //      resFE_OUT_OF_MEMORY
    //
    //  Description:
    //      Inserts the element at the queue's last index.
    //
    //==EDOC====================================================================
    mxt_result Enqueue(IN const _Type& rElement);

    //==SDOC====================================================================
    //==
    //==  GetFront
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the element at the queue's first index without erasing it.
    //
    //  Returns:
    //      The element at the queue's first index.
    //
    //  Description:
    //      Retrieves the element at the queue's first index without erasing it.
    //
    //==EDOC====================================================================
    const _Type& GetFront() const;

    //<COMBINE CQueue::GetFront@const>
    _Type& GetFront();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

// Hidden Members
//----------------
protected:
    _Container m_Container;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
template<class _Type, class _Container>
inline
CQueue<_Type, _Container>::CQueue(IN IAllocator* pAllocator)
: m_Container(pAllocator)
{
}

template<class _Type, class _Container>
inline
CQueue<_Type, _Container>::CQueue(IN const CQueue<_Type, _Container>& rQueue)
{
    *this = rQueue;
}

template<class _Type, class _Container>
CQueue<_Type, _Container>& CQueue<_Type, _Container>::
                           operator=(IN const CQueue<_Type, _Container>& rQueue)
{
    if (&rQueue != this)
    {
        m_Container = rQueue.m_Container;
    }
    return *this;
}

template<class _Type, class _Container>
inline
bool CQueue<_Type, _Container>::operator==(IN const CQueue<_Type, _Container>& rQueue) const
{
    return m_Container == rQueue.m_Container;
}

template<class _Type, class _Container>
inline
bool CQueue<_Type, _Container>::operator!=(IN const CQueue<_Type, _Container>& rQueue) const
{
    return m_Container != rQueue.m_Container;
}


template<class _Type, class _Container>
inline
bool CQueue<_Type, _Container>::operator<(IN const CQueue<_Type, _Container>& rQueue) const
{
    return m_Container < rQueue.m_Container;
}

template<class _Type, class _Container>
inline
bool CQueue<_Type, _Container>::operator>(IN const CQueue<_Type, _Container>& rQueue) const
{
    return m_Container > rQueue.m_Container;
}

template<class _Type, class _Container>
inline
bool CQueue<_Type, _Container>::operator<=(IN const CQueue<_Type, _Container>& rQueue) const
{
    return m_Container <= rQueue.m_Container;
}

template<class _Type, class _Container>
inline
bool CQueue<_Type, _Container>::operator>=(IN const CQueue<_Type, _Container>& rQueue) const
{
    return m_Container >= rQueue.m_Container;
}

template<class _Type, class _Container>
inline
CQueue<_Type, _Container>::~CQueue()
{
}

template<class _Type, class _Container>
inline
unsigned int CQueue<_Type, _Container>::GetCapacity() const
{
    return m_Container.GetCapacity();
}

template<class _Type, class _Container>
inline
unsigned int CQueue<_Type, _Container>::GetLockCapacity() const
{
    return m_Container.GetLockCapacity();
}

template<class _Type, class _Container>
inline
void CQueue<_Type, _Container>::LockCapacity()
{
    m_Container.LockCapacity();
}

template<class _Type, class _Container>
inline
mxt_result CQueue<_Type, _Container>::
                                 ReduceCapacity(IN unsigned int uDownToCapacity)
{
    return m_Container.ReduceCapacity(uDownToCapacity);
}

template<class _Type, class _Container>
inline
mxt_result CQueue<_Type, _Container>::
                                  ReserveCapacity(IN unsigned int uUpToCapacity)
{
    return m_Container.ReserveCapacity(uUpToCapacity);
}

template<class _Type, class _Container>
inline
void CQueue<_Type, _Container>::UnlockCapacity()
{
    m_Container.UnlockCapacity();
}

template<class _Type, class _Container>
inline
unsigned int CQueue<_Type, _Container>::GetSize() const
{
    return m_Container.GetSize();
}

template<class _Type, class _Container>
inline
bool CQueue<_Type, _Container>::IsEmpty() const
{
    return m_Container.IsEmpty();
}

template<class _Type, class _Container>
inline
bool CQueue<_Type, _Container>::IsFull() const
{
    return m_Container.IsFull();
}

template<class _Type, class _Container>
inline
void CQueue<_Type, _Container>::EraseAll()
{
    m_Container.EraseAll();
}

template<class _Type, class _Container>
inline
void CQueue<_Type, _Container>::Dequeue()
{
    m_Container.Dequeue();
}

template<class _Type, class _Container>
inline
mxt_result CQueue<_Type, _Container>::Enqueue(IN const _Type& rElement)
{
    return m_Container.Enqueue(rElement);
}

template<class _Type, class _Container>
inline
const _Type& CQueue<_Type, _Container>::GetFront() const
{
    return m_Container.GetFront();
}

template<class _Type, class _Container>
inline
_Type& CQueue<_Type, _Container>::GetFront()
{
    return m_Container.GetFront();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CQUEUE_H

