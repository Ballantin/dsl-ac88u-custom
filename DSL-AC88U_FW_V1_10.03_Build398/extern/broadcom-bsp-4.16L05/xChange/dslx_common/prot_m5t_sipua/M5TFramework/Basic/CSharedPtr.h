//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
//==============================================================================
#ifndef MXG_CSHAREDPTR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSHAREDPTR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
    #include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
    #include "Config/FrameworkCfg.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSharedPtr
//==============================================================================
//<GROUP BASIC_TEMPLATES>
//
// Summary:
//   Class that automatically manages an object's lifetime.
//
// Description:
//   The CSharedPtr class automatically manages an object's lifetime. It
//   takes care of managing the object's reference count. The reference to
//   the managed object is automatically released when the object is
//   destroyed. This allows code that uses an object to not worry about
//   the object's lifetime and reference count.
//
//   For the CSharedPtr class to work, the object stored MUST implement the
//   AddIfRef() and ReleaseIfRef() methods. When created, the object's
//   reference count MUST be 1. Calling AddIfRef adds a reference on the
//   object, and ReleaseIfRef() removes one. When the reference count reaches
//   0, the object MUST delete itself. Concurrency protection, if needed, is
//   left to the implementation of these two methods.
//
//   The CSharedPtr class is especially useful when declared on the stack or
//   as a class member since it automatically releases the object once it is
//   destroyed.
//
// Note:
//   The AddIfRef() and ReleaseIfRef() methods MUST NOT be called on the object
//   managed by a CSharedPtr. Doing so would cause the reference count on the
//   object to become invalid, resulting in either early deletion or memory
//   leaks.
//
// Example:
//   In the code below, an ECOM interface is acquired via the CreateEComInstance
//   method. That reference to the interface is automatically put in the
//   CSharedPtr object that manages its lifetime. The interface is
//   automatically released once the CSharedPtr object is deleted. In this
//   example, this occurs when the function returns.
//
//  <CODE>
//      void Function()
//      {
//          CSharedPtr<IInterface> spInterface;
//
//          mxt_result res = CreateEComInstance(CLSID_CInterface, NULL, OUT spInterface);
//
//          if (MX_RIS_S(res))
//          {
//              spInterface->InterfaceMethod();
//          }
//      }
//  </CODE>
//
// Location:
//   Basic/CSharedPtr.h
//
//==============================================================================
template <class _Type>
class CSharedPtr
{
//-- Published Interface.
public:
    //-- Constructors / destructor.

    // Summary:
    //   Default constructor.
    explicit CSharedPtr(IN _Type* pObject = NULL);

    // Summary:
    //   Copy constructor.
    CSharedPtr(IN const CSharedPtr<_Type>& rSrc);

    // Summary:
    //   Destructor.
    ~CSharedPtr();

    //-- Operators.

    // Summary:
    //   Assignment operator.
    CSharedPtr<_Type>& operator=(IN _Type* pObject);

    // Summary:
    //   Assignment operator.
    CSharedPtr<_Type>& operator=(IN const CSharedPtr<_Type>& rRhs);

    // Summary:
    //   Comparison operator.
    bool operator==(IN const CSharedPtr<_Type>& rRhs) const;

    // Summary:
    //   Comparison operator.
    bool operator==(IN const _Type* pObject) const;

    // Summary:
    //   Different than operator.
    bool operator!=(IN const CSharedPtr<_Type>& rRhs) const;

    // Summary:
    //   Different than operator.
    bool operator!=(IN const _Type* pObject) const;

    // Summary:
    //   Greater than operator.
    bool operator>(IN const CSharedPtr<_Type>& rRhs) const;

    // Summary:
    //   Greater than operator.
    bool operator>(IN const _Type* pObject) const;

    // Summary:
    //   Less than operator.
    bool operator<(IN const CSharedPtr<_Type>& rRhs) const;

    // Summary:
    //   Less than operator.
    bool operator<(IN const _Type* pObject) const;

    // Summary:
    //   Greater than or equal to operator.
    bool operator>=(IN const CSharedPtr<_Type>& rRhs) const;

    // Summary:
    //   Greater than or equal to operator.
    bool operator>=(IN const _Type* pObject) const;

    // Summary:
    //   Less than or equal to operator.
    bool operator<=(IN const CSharedPtr<_Type>& rRhs) const;

    // Summary:
    //   Less than or equal to operator.
    bool operator<=(IN const _Type* pObject) const;

    // Summary:
    //   Dereference operator.
    _Type* operator->() const;

    // Summary:
    //   Reference operator.
    _Type& operator*() const;

    // Summary:
    //   Cast operator to _Type**.
    operator _Type**();

    //-- Public methods.

    // Summary:
    //   Gets the pointer to the managed object.
    _Type* Get() const;

    // Summary:
    //   Resets the managed object.
    void Reset(IN _Type* pObject = NULL);

//-- Hidden Data Members.
private:
    // Pointer to the object that is managed.
    _Type* m_pObject;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  CSharedPtr
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      Pointer to an object to be managed by this CSharedPtr. NULL by
//      default.
//
//  Description:
//    This is the default constructor of the CSharedPtr object. If pObject is
//    not NULL, this object takes its ownership.
//
//  See Also:
//    ~CSharedPtr
//
//  Example:
//    This example shows different ways to create a CSharedPtr object using its
//    default constructor.
//
//  <CODE>
//      CSharedPtr<IInterface> autoInterface;
//
//      IInterface* pInterface = ...
//      CSharedPtr<IInterface> autoInterface(IN pInterface);
//      pInterface = NULL;
//  </CODE>
//
//==============================================================================
template <class _Type>
inline CSharedPtr<_Type>::CSharedPtr(IN _Type* pObject)
:   m_pObject(pObject)
{
    if (m_pObject != NULL)
    {
        m_pObject->AddIfRef();
    }
}

//==============================================================================
//==
//==  CSharedPtr
//==
//==============================================================================
//
//  Parameters:
//    rSrc:
//     A reference to the CSharedPtr object to copy.
//
//  Description:
//    This is the copy constructor of the CSharedPtr. It creates a new
//    CSharedPtr object that manages a new reference of the object.
//
//  See Also:
//    ~CSharedPtr
//
//==============================================================================
template <class _Type>
inline CSharedPtr<_Type>::CSharedPtr(IN const CSharedPtr<_Type>& rSrc)
:   m_pObject(NULL)
{
    // Use the assignment operator.
    *this = rSrc;
}

//==============================================================================
//==
//==  ~CSharedPtr
//==
//==============================================================================
//
//  Description:
//    This is the destructor of the CSharedPtr object. It releases the
//    reference on the object it manages.
//
//  See Also:
//    CSharedPtr
//
//==============================================================================
template <class _Type>
inline CSharedPtr<_Type>::~CSharedPtr()
{
    if (m_pObject != NULL)
    {
        m_pObject->ReleaseIfRef();
        m_pObject = NULL;
    }
}

//==============================================================================
//==
//==  operator=
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      Pointer to an object to be managed by this CSharedPtr.
//
//  Returns:
//    A reference to the assigned CSharedPtr object.
//
//  Description:
//    Assigns the object on the right hand side of the operator to
//    the CSharedPtr on the left hand side of the operator.
//
//==============================================================================
template <class _Type>
inline CSharedPtr<_Type>& CSharedPtr<_Type>::operator=(IN _Type* pObject)
{
    Reset(pObject);

    return *this;
}

//==============================================================================
//==
//==  operator=
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CSharedPtr object that is on the right hand side of the operator.
//
//  Returns:
//    A reference to the assigned CSharedPtr object.
//
//  Description:
//    Assigns the object managed by the CSharedPtr on the right hand
//    side of the operator to the CSharedPtr on the left hand side of the
//    operator. The reference held by the CSharedPtr on the right hand side of
//    the operator is released.
//
//  See Also:
//    operator==, operator!=
//
//==============================================================================
template <class _Type>
inline CSharedPtr<_Type>& CSharedPtr<_Type>::operator=(IN const CSharedPtr<_Type>& rRhs)
{
    if (&rRhs != this)
    {
        Reset(rRhs.m_pObject);
    }

    return *this;
}

//==============================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CSharedPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: Both CSharedPtr objects manage the same object instance.
//    - false: The CSharedPtr objects manage different object instances.
//
//  Description:
//    Compares the CSharedPtr on the right hand side of the operator to the
//    CSharedPtr on the left hand side of the operator to verify that they
//    manage the same object instance.
//
//  See Also:
//    operator=, operator!=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator==(IN const CSharedPtr<_Type>& rRhs) const
{
    return m_pObject == rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      The pointer that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr and the pointer manage the same object instance.
//    - false: The CSharedPtr and the pointer manage different object instances.
//
//  Description:
//    Compares the pointer on the right hand side of the operator to the
//    CSharedPtr on the left hand side of the operator to verify that they
//    manage the same object instance.
//
//  See Also:
//    operator=, operator!=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator==(IN const _Type* pObject) const
{
    return m_pObject == pObject;
}

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CSharedPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr objects manage different object instances.
//    - false: Both CSharedPtr objects manage the same object instance.
//
//  Description:
//    Compares the CSharedPtr on the right hand side of the operator to the
//    CSharedPtr on the left hand side of the operator to verify that they
//    manage different object instances.
//
//  See Also:
//    operator=, operator==
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator!=(IN const CSharedPtr<_Type>& rRhs) const
{
    // Use the comparison operator.
    return !(*this == rRhs);
}

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      The pointer that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr and the pointer manage different object instances.
//    - false: The CSharedPtr and the pointer manage the same object instance.
//
//  Description:
//    Compares the pointer on the right hand side of the operator to the
//    CSharedPtr on the left hand side of the operator to verify that they
//    manage different object instances.
//
//  See Also:
//    operator=, operator==
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator!=(IN const _Type* pObject) const
{
    // Use the comparison operator.
    return !(*this == pObject);
}

//==============================================================================
//==
//==  operator>
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CSharedPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr object on the left hand side of the operator is
//            greater than the one on the right hand side of the operator.
//    - false: The CSharedPtr object on the left hand side of the operator is
//             smaller than the one on the right hand side of the operator.
//
//  Description:
//    Verifies that the CSharedPtr on the left hand side of the operator is
//    greater than the CSharedPtr on the right hand side of the operator. The
//    comparison is done using the value of the pointers to the managed objects.
//
//  See Also:
//    operator<, operator>=, operator<=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator>(IN const CSharedPtr<_Type>& rRhs) const
{
    return m_pObject > rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator>
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      The pointer that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr object on the left hand side of the operator is
//            greater than the pointer on the right hand side of the operator.
//    - false: The CSharedPtr object on the left hand side of the operator is
//             smaller than the pointer on the right hand side of the operator.
//
//  Description:
//    Verifies that the CSharedPtr on the left hand side of the operator is
//    greater than the pointer on the right hand side of the operator. The
//    comparison is done using the value of the pointer managed by the
//    CSharedPtr object and pObject.
//
//  See Also:
//    operator<, operator>=, operator<=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator>(IN const _Type* pObject) const
{
    return m_pObject > pObject;
}

//==============================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CSharedPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr object on the left hand side of the operator is
//            smaller than the one on the right hand side of the operator.
//    - false: The CSharedPtr object on the left hand side of the operator is
//             greater than the one on the right hand side of the operator.
//
//  Description:
//    Verifies that the CSharedPtr on the left hand side of the operator is
//    smaller than the CSharedPtr on the right hand side of the operator. The
//    comparison is done using the value of the pointers to the managed objects.
//
//  See Also:
//    operator>, operator>=, operator<=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator<(IN const CSharedPtr<_Type>& rRhs) const
{
    return m_pObject < rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      The pointer that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr object on the left hand side of the operator is
//            smaller than the pointer on the right hand side of the operator.
//    - false: The CSharedPtr object on the left hand side of the operator is
//             greater than the pointer on the right hand side of the operator.
//
//  Description:
//    Verifies that the CSharedPtr on the left hand side of the operator is
//    smaller than the pointer on the right hand side of the operator. The
//    comparison is done using the value of the pointer managed by the
//    CSharedPtr object and pObject.
//
//  See Also:
//    operator>, operator>=, operator<=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator<(IN const _Type* pObject) const
{
    return m_pObject < pObject;
}

//==============================================================================
//==
//==  operator>=
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CSharedPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr object on the left hand side of the operator is
//            greater than or equal to the one on the right hand side of the
//            operator.
//    - false: The CSharedPtr object on the left hand side of the operator is
//             smaller than the one on the right hand side of the operator.
//
//  Description:
//    Verifies that the CSharedPtr on the left hand side of the operator is
//    greater than or equal to the CSharedPtr on the right hand side of the
//    operator. The comparison is done using the value of the pointers to
//    the managed objects.
//
//  See Also:
//    operator>, operator<, operator<=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator>=(IN const CSharedPtr<_Type>& rRhs) const
{
    return m_pObject >= rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator>=
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      The pointer that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr object on the left hand side of the operator is
//            greater than or equal to the pointer on the right hand side of the
//            operator.
//    - false: The CSharedPtr object on the left hand side of the operator is
//             smaller than the pointer on the right hand side of the operator.
//
//  Description:
//    Verifies that the CSharedPtr on the left hand side of the operator is
//    greater than or equal to the pointer on the right hand side of the
//    operator. The comparison is done using the value of the pointer managed
//    by the CSharedPtr object and pObject.
//
//  See Also:
//    operator>, operator<, operator<=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator>=(IN const _Type* pObject) const
{
    return m_pObject >= pObject;
}

//==============================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CSharedPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr object on the left hand side of the operator is
//            smaller than or equal to the one on the right hand side of the
//            operator.
//    - false: The CSharedPtr object on the left hand side of the operator is
//             greater than the one on the right hand side of the operator.
//
//  Description:
//    Verifies that the CSharedPtr on the left hand side of the operator is
//    smaller or equal than the CSharedPtr on the right hand side of the
//    operator. The comparison is done using the value of the pointers to the
//    managed objects.
//
//  See Also:
//    operator>, operator<, operator>=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator<=(IN const CSharedPtr<_Type>& rRhs) const
{
    return m_pObject <= rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      The pointer that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CSharedPtr object on the left hand side of the operator is
//            smaller than or equal to the pointer on the right hand side of the
//            operator.
//    - false: The CSharedPtr object on the left hand side of the operator is
//             greater than the pointer on the right hand side of the operator.
//
//  Description:
//    Verifies that the CSharedPtr on the left hand side of the operator is
//    smaller than or equal to the pointer on the right hand side of the
//    operator. The comparison is done using the value of the pointer managed
//    by the CSharedPtr object and pObject.
//
//  See Also:
//    operator>, operator<, operator>=
//
//==============================================================================
template <class _Type>
inline bool CSharedPtr<_Type>::operator<=(IN const _Type* pObject) const
{
    return m_pObject <= pObject;
}

//==============================================================================
//==
//==  operator->
//==
//==============================================================================
//
//  Returns:
//    - A pointer to the object managed by this CSharedPtr.
//
//  Description:
//    Returns a pointer to the object managed by this object. This
//    allows the CSharedPtr object to be used as if it was the object.
//
//  Note:
//    The object managed must be non-NULL, otherwise this operator 
//    generates an assertion.
//
//  See Also:
//    operator*
//
//==============================================================================
template <class _Type>
inline _Type* CSharedPtr<_Type>::operator->() const
{
    MX_ASSERT(m_pObject != NULL);
    return m_pObject;
}

//==============================================================================
//==
//==  operator*
//==
//==============================================================================
//
//  Returns:
//    - A reference to the object managed by this CSharedPtr.
//
//  Description:
//    Returns a reference to the object managed by this object. This
//    allows the CSharedPtr object to be used as if it was the object.
//
//  Note:
//    The object managed must be non-NULL, otherwise this operator 
//    generates an assertion.
//
//  See Also:
//    operator->
//
//==============================================================================
template <class _Type>
inline _Type& CSharedPtr<_Type>::operator*() const
{
    MX_ASSERT(m_pObject != NULL);
    return *m_pObject;
}

//==============================================================================
//==
//==  operator _Type**
//==
//==============================================================================
//
//  Returns:
//    - The address to the pointer to the object managed by this CSharedPtr.
//
//  Description:
//    Returns the address to the pointer to the object managed by this object.
//    This allows the CSharedPtr object to be used when a function takes the
//    address of a pointer to the object as an out parameter.
//
//  <CODE>
//      void Get(OUT IInterface** ppInterface)
//      {
//          *ppInterface = ...
//      }
//
//      void Function()
//      {
//          CSharedPtr<IInterface> spInterface;
//
//          mxt_result res = Get(OUT spInterface);
//
//          if (MX_RIS_S(res))
//          {
//              spInterface->InterfaceMethod();
//          }
//      }
//  </CODE>
//
//==============================================================================
template <class _Type>
inline CSharedPtr<_Type>::operator _Type**()
{
    if (m_pObject != NULL)
    {
        m_pObject->ReleaseIfRef();
        m_pObject = NULL;
    }

    return &m_pObject;
}

//==============================================================================
//==
//==  Get
//==
//==============================================================================
//
//  Returns:
//    - A pointer to the object managed by this CSharedPtr.
//
//  Description:
//    Returns a pointer to the object managed by this object. No
//    reference is added to the returned pointer.
//
//==============================================================================
template <class _Type>
inline _Type* CSharedPtr<_Type>::Get() const
{
    return m_pObject;
}

//==============================================================================
//==
//==  Reset
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      A pointer to an object to replace the one managed by this CSharedPtr.
//      NULL by default.
//
//  Description:
//    Resets the object managed by this CSharedPtr to the one specified in
//    pObject. If an object is already managed by this CSharedPtr, it is
//    released before assigning the new one. If pObject is NULL, this
//    CSharedPtr no longer manages any object.
//
//==============================================================================
template <class _Type>
inline void CSharedPtr<_Type>::Reset(IN _Type* pObject)
{
    if (m_pObject != NULL)
    {
        m_pObject->ReleaseIfRef();
    }

    m_pObject = pObject;

    if (m_pObject != NULL)
    {
        m_pObject->AddIfRef();
    }
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSHAREDPTR_H
