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
#ifndef MXG_CAUTOPTR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CAUTOPTR_H
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
//== Class: CAutoPtr
//==============================================================================
//<GROUP BASIC_TEMPLATES>
//
// Summary:
//   Class that automatically manages an object's lifetime.
//
// Description:
//   The CAutoPtr class automatically manages an object's lifetime. It 
//   automatically deletes from memory the object it has been assigned once the
//   CAutoPtr object is destroyed. This effectively means that an object allocated
//   in memory and put in a CAutoPtr never has to be deleted by using MX_DELETE,
//   thus reducing the risks of memory leaks.
//
//   The CAutoPtr class is especially useful when declared on the stack or
//   as a class member since it automatically deletes the managed object once
//   it is destroyed.
//
// Example:
//   In the code below, an object is created in the heap. That object is
//   automatically put in the CAutoPtr object that manages its lifetime.
//   The object is automatically released once the CAutoPtr object is deleted.
//   In this example, this occurs when the function returns.
//
//  <CODE>
//void function()
//{
//    CAutoPtr<CClass> spClass(MX_NEW(CClass));
//
//    spClass->ClassMethod();
//}
//  </CODE>
//
// Location:
//   Basic/CAutoPtr.h
//
//==============================================================================
template <class _Type>
class CAutoPtr
{
//-- Published Interface.
public:
    //-- Constructors / destructor.

    // Default constructor.
    explicit CAutoPtr(TOA _Type* pObject = NULL);
    // Copy constructor.
    CAutoPtr(IN TOA CAutoPtr<_Type>& rSrc);
    // Destructor.
    ~CAutoPtr();

    //-- Operators.
    // Assignment operator.
    CAutoPtr<_Type>& operator=(IN TOA _Type* pObject);
    // Assignment operator.
    CAutoPtr<_Type>& operator= (IN TOA CAutoPtr<_Type>& rRhs);
    // Comparison operator.
    bool operator==(IN const CAutoPtr<_Type>& rRhs) const;
    // Different than operator.
    bool operator!=(IN const CAutoPtr<_Type>& rRhs) const;
    // Greater than operator.
    bool operator>(IN const CAutoPtr<_Type>& rRhs) const;
    // Less than operator.
    bool operator<(IN const CAutoPtr<_Type>& rRhs) const;
    // Greater than or equal operator.
    bool operator>=(IN const CAutoPtr<_Type>& rRhs) const;
    // Less than or equal operator.
    bool operator<=(IN const CAutoPtr<_Type>& rRhs) const;
    // Dereference operator.
    _Type* operator->() const;
    // Reference operator.
    _Type& operator*() const;

    //-- Public methods.
    // Gets the pointer to the object.
    _Type* Get() const;
    // Releases the object.
    _Type* GO Release();
    // Resets the object.
    void Reset(IN TOA _Type* pObject = NULL);

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
//==  CAutoPtr
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      Pointer to an object to be managed by this CAutoPtr. NULL by
//      default.
//
//  Description:
//    This is the default constructor of the CAutoPtr object. If pObject is
//    not NULL, this object takes its ownership.
//
//  See Also:
//    ~CAutoPtr
//
//  Example:
//    This example shows different ways to create a CAutoPtr object using its
//    default constructor.
//
//  <CODE>
//CAutoPtr<CClass> spClass;
//
//CClass* pClass;
//CAutoPtr<CClass> spClass(TOA pClass);
//pClass = NULL;
//  </CODE>
//
//==============================================================================
template <class _Type>
inline CAutoPtr<_Type>::CAutoPtr(TOA _Type* pObject)
:   m_pObject(pObject)
{
}

//==============================================================================
//==
//==  CAutoPtr
//==
//==============================================================================
//
//  Parameters:
//    rSrc:
//     A reference to the CAutoPtr object to copy.
//
//  Description:
//    This is the copy constructor of the CAutoPtr. It creates a new
//    CAutoPtr object that manages a new reference of the object.
//
//  See Also:
//    ~CAutoPtr
//
//==============================================================================
template <class _Type>
inline CAutoPtr<_Type>::CAutoPtr(IN TOA CAutoPtr<_Type>& rSrc)
:   m_pObject(NULL)
{
    // Use the assignment operator.
    *this = rSrc;
}

//==============================================================================
//==
//==  ~CAutoPtr
//==
//==============================================================================
//
//  Description:
//    This is the destructor of the CAutoPtr object. It releases the
//    reference on the object it manages.
//
//  See Also:
//    CAutoPtr
//
//==============================================================================
template <class _Type>
inline CAutoPtr<_Type>::~CAutoPtr()
{
    if (m_pObject != NULL)
    {
        MX_DELETE(m_pObject);
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
//      Pointer to an object to be managed by this CAutoPtr.
//
//  Returns:
//    A reference to the assigned CAutoPtr object.
//
//  Description:
//    Assigns the object on the right hand side of the operator to
//    the CAutoPtr on the left hand side of the operator. The object managed by
//    the CAutoPtr on the left hand side of the operator is deleted and replaced
//    by the object on the right hand side of the operator.
//
//==============================================================================
template <class _Type>
inline CAutoPtr<_Type>& CAutoPtr<_Type>::operator=(IN TOA _Type* pObject)
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
//      The CAutoPtr object that is on the right hand side of the operator.
//
//  Returns:
//    A reference to the assigned CAutoPtr object.
//
//  Description:
//    Assigns the object managed by the CAutoPtr on the right hand
//    side of the operator to the CAutoPtr on the left hand side of the
//    operator. The object managed by the CAutoPtr on the left hand side of the
//    operator is deleted and replaced by the object managed by the CAutoPtr on
//    the right hand side of the operator. The CAutoPtr on the left hand side
//    of the operator stops managing the object, meaning that it no longer has
//    a pointer to the object.
//
//  See Also:
//    operator==, operator!=
//
//==============================================================================
template <class _Type>
inline CAutoPtr<_Type>& CAutoPtr<_Type>::operator=(IN TOA CAutoPtr<_Type>& rRhs)
{
    if (&rRhs != this)
    {
        Reset(rRhs.m_pObject);
        rRhs.m_pObject = NULL;
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
//      The CAutoPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: Both CAutoPtr objects manage the same object instance.
//    - false: The CAutoPtr objects manage different object instances.
//
//  Description:
//    Compares the CAutoPtr on the right hand side of the operator to the
//    CAutoPtr on the left hand side of the operator to verify that they
//    manage the same object instance.
//
//  See Also:
//    operator=, operator!=
//
//==============================================================================
template <class _Type>
inline bool CAutoPtr<_Type>::operator==(IN const CAutoPtr<_Type>& rRhs) const
{
    return m_pObject == rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CAutoPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CAutoPtr objects manage different object instances.
//    - false: Both CAutoPtr objects manage the same object instance.
//
//  Description:
//    Compares the CAutoPtr on the right hand side of the operator to the
//    CAutoPtr on the left hand side of the operator to verify that they
//    manage different object instances.
//
//  See Also:
//    operator=, operator==
//
//==============================================================================
template <class _Type>
inline bool CAutoPtr<_Type>::operator!=(IN const CAutoPtr<_Type>& rRhs) const
{
    // Use the comparison operator.
    return !(*this == rRhs);
}

//==============================================================================
//==
//==  operator>
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CAutoPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CAutoPtr object on the left hand side of the operator is
//            greater than the one on the right hand side of the operator.
//    - false: The CAutoPtr object on the left hand side of the operator is
//             smaller than the one on the right hand side of the operator.
//
//  Description:
//    Verifies that the CAutoPtr on the left hand side of the operator is
//    greater than the CAutoPtr on the right hand side of the operator. The
//    comparison is done using the value of the pointers to the managed objects.
//
//  See Also:
//    operator<, operator>=, operator<=
//
//==============================================================================
template <class _Type>
inline bool CAutoPtr<_Type>::operator>(IN const CAutoPtr<_Type>& rRhs) const
{
    return m_pObject > rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CAutoPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CAutoPtr object on the left hand side of the operator is
//            smaller than the one on the right hand side of the operator.
//    - false: The CAutoPtr object on the left hand side of the operator is
//             greater than the one on the right hand side of the operator.
//
//  Description:
//    Verifies that the CAutoPtr on the left hand side of the operator is
//    smaller than the CAutoPtr on the right hand side of the operator. The
//    comparison is done using the value of the pointers to the managed objects.
//
//  See Also:
//    operator>, operator>=, operator<=
//
//==============================================================================
template <class _Type>
inline bool CAutoPtr<_Type>::operator<(IN const CAutoPtr<_Type>& rRhs) const
{
    return m_pObject < rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator>=
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CAutoPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CAutoPtr object on the left hand side of the operator is
//            greater thant or equal to the one on the right hand side of the
//            operator.
//    - false: The CAutoPtr object on the left hand side of the operator is
//             smaller than the one on the right hand side of the operator.
//
//  Description:
//    Verifies that the CAutoPtr on the left hand side of the operator is
//    greater than or equal to the CAutoPtr on the right hand side of the
//    operator. The comparison is done using the value of the pointers to
//    the managed objects.
//
//  See Also:
//    operator>, operator<, operator<=
//
//==============================================================================
template <class _Type>
inline bool CAutoPtr<_Type>::operator>=(IN const CAutoPtr<_Type>& rRhs) const
{
    return m_pObject >= rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//    rRhs:
//      The CAutoPtr object that is on the right hand side of the operator.
//
//  Returns:
//    - true: The CAutoPtr object on the left hand side of the operator is
//            smaller than or equal to the one on the right hand side of the
//            operator.
//    - false: The CAutoPtr object on the left hand side of the operator is
//             greater than the one on the right hand side of the operator.
//
//  Description:
//    Verifies that the CAutoPtr on the left hand side of the operator is
//    smaller than or equal to the CAutoPtr on the right hand side of the
//    operator. The comparison is done using the value of the pointers to the
//    managed objects.
//
//  See Also:
//    operator>, operator<, operator>=
//
//==============================================================================
template <class _Type>
inline bool CAutoPtr<_Type>::operator<=(IN const CAutoPtr<_Type>& rRhs) const
{
    return m_pObject <= rRhs.m_pObject;
}

//==============================================================================
//==
//==  operator->
//==
//==============================================================================
//
//  Returns:
//    - A pointer to the object managed by this CAutoPtr.
//
//  Description:
//    Returns a pointer to the object managed by this object. This
//    allows the CAutoPtr object to be used as if it was the object.
//
//  Note:
//    The managed object must not be NULL, otherwise this operator 
//    generates an assertion.
//
//  See Also:
//    operator*
//
//==============================================================================
template <class _Type>
inline _Type* CAutoPtr<_Type>::operator->() const
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
//    - A reference to the object managed by this CAutoPtr.
//
//  Description:
//    Returns a reference to the object managed by this object. This
//    allows the CAutoPtr object to be used as if it was the object.
//
//  Note:
//    The managed object must not be NULL, otherwise this operator 
//    generates an assertion.
//
//  See Also:
//    operator->
//
//==============================================================================
template <class _Type>
inline _Type& CAutoPtr<_Type>::operator*() const
{
    MX_ASSERT(m_pObject != NULL);
    return *m_pObject;
}

//==============================================================================
//==
//==  Get
//==
//==============================================================================
//
//  Returns:
//    - A pointer to the object managed by this CAutoPtr.
//
//  Description:
//    Returns a pointer to the object managed by this object.
//
//  See Also:
//    Release
//
//==============================================================================
template <class _Type>
inline _Type* CAutoPtr<_Type>::Get() const
{
    return m_pObject;
}

//==============================================================================
//==
//==  Release
//==
//==============================================================================
//
//  Returns:
//    - The reference to the object managed by this CAutoPtr.
//
//  Description:
//    Releases the object managed by this object. A pointer to the
//    object is returned, and this CAutoPtr stops managing the object, meaning
//    that it no longer has a pointer to the object.
//    Ownership of the object is given to the caller of the method.
//
//  See Also:
//    Get
//
//==============================================================================
template <class _Type>
inline _Type* GO CAutoPtr<_Type>::Release()
{
    _Type* pTmp = m_pObject;
    m_pObject = NULL;
    return pTmp;
}

//==============================================================================
//==
//==  Reset
//==
//==============================================================================
//
//  Parameters:
//    pObject:
//      A pointer to an object to replace the one managed by this CAutoPtr.
//      NULL by default.
//
//  Description:
//    Resets the object managed by this CAutoPtr to the one specified in
//    pObject. If an object is already managed by this CAutoPtr, it is
//    released before assigning the new one. If pObject is NULL, this
//    CAutoPtr no longer manages any object.
//
//==============================================================================
template <class _Type>
inline void CAutoPtr<_Type>::Reset(IN TOA _Type* pObject)
{
    if (m_pObject != NULL)
    {
        MX_DELETE(m_pObject);
    }

    m_pObject = pObject;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CAUTOPTR_H
