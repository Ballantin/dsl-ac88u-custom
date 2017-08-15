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
#ifndef MXG_CSTACK_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSTACK_H
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
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
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

//@@CStack
//==SDOC========================================================================
//== Class: CStack
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//   Interface for basic containers so they are used as a standard stack.
//
// Description:
//
//   This template class implements an interface to a basic container (CList,
//   CVList, or CVector) to be used in a standard "stack" (LIFO) manner. It is
//   provided for code readability reasons. Therefore, when the required
//   container needs to be used as a stack, the CStack interface should be
//   preferred to the direct access to the basic container interface.
//
//   Memory management is left to the container.
//
//   The initialization of a CStack based on a CList looks as follows:
//      <CODE>
//          CStack<int, CList<int> > stack;
//      </CODE>
//
//   The initialization of a CStack with the default container looks as follows:
//      <CODE>
//          CStack<int> stack;
//      </CODE>
//
//   The signature of a method receiving a stack and an array of elements to be
//   inserted into this stack would look as follows (see the CStack test case
//   for details)
//      <CODE>
//          template <class _Stack, class _Type>
//          void TestAll(IN _Stack stack, IN _Type* pDataArray)
//      </CODE>
//
//   The CStack class currently supports the CVector, CList, and CVList
//   containers.  CVector has been chosen as the default container, since it is
//   the container of choice for fast insertion/suppression at the container's
//   end and this is precisely where the stack implementation performs its push
//   and pop operations on a CVector.
//
// Warning:
//  This class is not thread safe and has the same features and limitations as
//  the container used to implement it. Refer to the basic container
//  documentation to know which one to use.
//
//  Make sure to reserve enough memory before you begin insertions of elements
//  into a stack object.  This is done with a call to the ReserveCapacity
//  method. Otherwise, if you build up a stack of N elements with consecutive
//  calls like the following:
//      <CODE>
//          stack.Push(element);
//      </CODE>
//  you will cause N memory reallocation!  Thus, a stack built with calls to
//  Push without any previous memory allocation requires a lot more computation
//  than building the same stack after a call to ReserveCapacity.
//
// Location:
//   Cap/CStack.h
//
// See also:
//   CVList, CList, CVector, CQueue
//
//==EDOC========================================================================
// M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
// GCC 2.7.2 does not support default parameters in templates,
// in which case all the parameters must be specified.
template<class _Type, class _Container>
#else
// M5T_INTERNAL_USE_END
template<class _Type, class _Container = CVector<_Type> >
// M5T_INTERNAL_USE_BEGIN
#endif
// M5T_INTERNAL_USE_END
class CStack
{
// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CStack
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
    CStack(IN IAllocator* pAllocator = NULL);

    //==SDOC====================================================================
    //==
    //==  CStack
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
    virtual ~CStack();

    //==SDOC====================================================================
    //==
    //==  CStack
    //==
    //==========================================================================
    //
    //  Summary:
    //      Copy Constructor.
    //
    //  Parameters:
    //    rStack:
    //      A reference to the source Stack.
    //
    //  Description:
    //      Copy Constructor.
    //
    //==EDOC====================================================================
    CStack(IN const CStack<_Type, _Container>& rStack);

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
    //    rStack:
    //      Reference to the CStack object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      A reference to this CStack instance.
    //
    //  Description:
    //      Assignment operator.
    //
    //==EDOC====================================================================
    CStack<_Type, _Container>& operator=
                                   (IN const CStack<_Type, _Container>& rStack);

    //==SDOC====================================================================
    //==
    //==  operator==
    //==
    //==========================================================================
    //
    //  Summary:
    //      Equal to operator.
    //
    //  Parameters:
    //    rStack:
    //      Reference to the CStack object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand stack is equal to the right hand
    //      stack, otherwise returns false.
    //
    //  Description:
    //      Verifies that the left hand stack is equal to the right hand
    //      stack. This is done by comparing containers element by element and
    //      returns once an element is different from the other. This behaviour
    //      is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator== (IN const CStack<_Type, _Container>& rStack) const;

    //==SDOC====================================================================
    //==
    //==  operator!=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Different than operator.
    //
    //  Parameters:
    //    rStack:
    //      Reference to the CStack object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand stack is different from the right hand
    //      stack, otherwise returns false.
    //
    //  Description:
    //      Verifies that the left hand stack is different than the right hand
    //      stack. This is done by comparing containers element by element and
    //      returns once an element is different from the other. This behaviour
    //      is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator!= (IN const CStack<_Type, _Container>& rStack) const;

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
    //    rStack:
    //      Reference to the CStack object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand stack is less than the right hand
    //      stack, otherwise returns false.
    //
    //  Description:
    //      Verifies that the left hand stack is less than the right hand
    //      stack. This is done by comparing containers element by element and
    //      returns once an element is different from the other. This behaviour
    //      is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator< (IN const CStack<_Type, _Container>& rStack) const;

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
    //    rStack:
    //      Reference to the CStack object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand stack is greater than the right hand
    //      stack, otherwise returns false.
    //
    //  Description:
    //      Verifies that the left hand stack is greater than the right hand
    //      Stack. This is done by comparing containers element by element and
    //      returns once an element is different from the other. This behaviour
    //      is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator> (IN const CStack<_Type, _Container>& rStack) const;

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
    //    rStack:
    //      Reference to the CStack object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand stack is less than or equal to the
    //      right hand stack, otherwise returns false.
    //
    //  Description:
    //      Verifies that the left hand stack is less than or equal to the
    //      right hand stack. This is done by comparing containers element by
    //      element and returns once an element is different from the other.
    //      This behaviour is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator<= (IN const CStack<_Type, _Container>& rStack) const;

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
    //    rStack:
    //      Reference to the CStack object at the right hand side of the
    //      assignation.
    //
    //  Returns:
    //      Returns true if the left hand stack is greater than or equal to
    //      the right hand stack, otherwise returns false.
    //
    //  Description:
    //      Verifies that the left hand stack is greater than or equal to the
    //      right hand stack. This is done by comparing containers element by
    //      element and returns once an element is different from the other.
    //      This behaviour is identical to the one used in strcmp.
    //
    //==EDOC====================================================================
    bool operator>= (IN const CStack<_Type, _Container>& rStack) const;

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
    //      Returns the capacity, which is the number of elements already
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
    //      Reduces the capacity to uDownToCapacity if the value is less than
    //      the current capacity. If uDownToCapacity is lower than the current
    //      stack size, the capacity is reduced down to the stack size (no
    //      element is lost).
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
    //      Returns the size, which is the number of elements that are allocated
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
    //      Queries if the stack is empty (stack size = 0).
    //
    //  Returns:
    //      True if the size is 0.
    //
    //  Description:
    //      Returns true if the stack size is 0. An empty stack is a stack in
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
    //      Queries if the stack is full (stack size = max capacity).
    //
    //  Returns:
    //      True if the size is equal to the capacity of the container.
    //
    //  Description:
    //      Returns true if the container used by the current stack object
    //      contains a number of elements equal to its capacity.  It means that
    //      any new insertion of elements into this stack causes a memory
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
    //      Clears the stack content.
    //
    //  Description:
    //      Erases all elements from this stack object.  The size is set to 0.
    //
    //==EDOC====================================================================
    void EraseAll();

    // Stack operations.
    //-------------------

    //==SDOC====================================================================
    //==
    //==  Pop
    //==
    //==========================================================================
    //
    //  Summary:
    //      Erases the element at the top.
    //
    //  Description:
    //      Erases the element at the top.
    //
    //==EDOC====================================================================
    void Pop();

    //==SDOC====================================================================
    //==
    //==  Push
    //==
    //==========================================================================
    //
    //  Summary:
    //      Inserts an element at the top.
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
    //      Inserts an element at the top.
    //
    //==EDOC====================================================================
    mxt_result Push(IN const _Type& rElement);

    //==SDOC====================================================================
    //==
    //==  GetTop
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the element at the top without erasing it.
    //
    //  Returns:
    //      The element at the top.
    //
    //  Description:
    //      Retrieves the element at the top without erasing it.
    //
    //==EDOC====================================================================
    const _Type& GetTop() const;

    //==SDOC====================================================================
    //==
    //==  GetTop
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the element at the top without erasing it.
    //
    //  Returns:
    //      The element at the top.
    //
    //  Description:
    //      Retrieves the element at the top without erasing it.
    //
    //==EDOC====================================================================
    _Type& GetTop();

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
CStack<_Type, _Container>::CStack(IN IAllocator* pAllocator)
: m_Container(pAllocator)
{
}

template<class _Type, class _Container>
inline
CStack<_Type, _Container>::CStack(IN const CStack<_Type, _Container>& rStack)
{
    *this = rStack;
}

template<class _Type, class _Container>
CStack<_Type, _Container>& CStack<_Type, _Container>::
                           operator=(IN const CStack<_Type, _Container>& rStack)
{
    if (&rStack != this)
    {
        m_Container = rStack.m_Container;
    }
    return *this;
}

template<class _Type, class _Container>
inline
bool CStack<_Type, _Container>::operator==(IN const CStack<_Type, _Container>& rStack) const
{
    return m_Container == rStack.m_Container;
}

template<class _Type, class _Container>
inline
bool CStack<_Type, _Container>::operator!=(IN const CStack<_Type, _Container>& rStack) const
{
    return m_Container != rStack.m_Container;
}

template<class _Type, class _Container>
inline
bool CStack<_Type, _Container>::operator<(IN const CStack<_Type, _Container>& rStack) const
{
    return m_Container < rStack.m_Container;
}

template<class _Type, class _Container>
inline
bool CStack<_Type, _Container>::operator>(IN const CStack<_Type, _Container>& rStack) const
{
    return m_Container > rStack.m_Container;
}

template<class _Type, class _Container>
inline
bool CStack<_Type, _Container>::operator<=(IN const CStack<_Type, _Container>& rStack) const
{
    return m_Container <= rStack.m_Container;
}

template<class _Type, class _Container>
inline
bool CStack<_Type, _Container>::operator>=(IN const CStack<_Type, _Container>& rStack) const
{
    return m_Container >= rStack.m_Container;
}

template<class _Type, class _Container>
inline
CStack<_Type, _Container>::~CStack()
{
}

template<class _Type, class _Container>
inline
unsigned int CStack<_Type, _Container>::GetCapacity() const
{
    return m_Container.GetCapacity();
}

template<class _Type, class _Container>
inline
unsigned int CStack<_Type, _Container>::GetLockCapacity() const
{
    return m_Container.GetLockCapacity();
}

template<class _Type, class _Container>
inline
void CStack<_Type, _Container>::LockCapacity()
{
    m_Container.LockCapacity();
}

template<class _Type, class _Container>
inline
mxt_result CStack<_Type, _Container>::
                                 ReduceCapacity(IN unsigned int uDownToCapacity)
{
    return m_Container.ReduceCapacity(uDownToCapacity);
}

template<class _Type, class _Container>
inline
mxt_result CStack<_Type, _Container>::
                                  ReserveCapacity(IN unsigned int uUpToCapacity)
{
    return m_Container.ReserveCapacity(uUpToCapacity);
}

template<class _Type, class _Container>
inline
void CStack<_Type, _Container>::UnlockCapacity()
{
    m_Container.UnlockCapacity();
}

template<class _Type, class _Container>
inline
unsigned int CStack<_Type, _Container>::GetSize() const
{
    return m_Container.GetSize();
}

template<class _Type, class _Container>
inline
bool CStack<_Type, _Container>::IsEmpty() const
{
    return m_Container.IsEmpty();
}

template<class _Type, class _Container>
inline
bool CStack<_Type, _Container>::IsFull() const
{
    return m_Container.IsFull();
}

template<class _Type, class _Container>
inline
void CStack<_Type, _Container>::EraseAll()
{
    m_Container.EraseAll();
}

template<class _Type, class _Container>
inline
void CStack<_Type, _Container>::Pop()
{
    m_Container.Pop();
}

template<class _Type, class _Container>
inline
mxt_result CStack<_Type, _Container>::Push(IN const _Type& rElement)
{
    return m_Container.Push(rElement);
}

template<class _Type, class _Container>
inline
const _Type& CStack<_Type, _Container>::GetTop() const
{
    return m_Container.GetTop();
}

template<class _Type, class _Container>
inline
_Type& CStack<_Type, _Container>::GetTop()
{
    return m_Container.GetTop();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif // MXG_CSTACK_H

