//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation. ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CPAIR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPAIR_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

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

#ifndef MXG_CALIGNEDVARIABLESTORAGE_H
#include "Basic/CAlignedVariableStorage.h"
#endif

// Data Member

// Interface Realized & Parent

// Forward Declarations Outside of the Namespace

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace

//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================

//===========================================================================
//====  NEW TYPE DEFINITIONS  ===============================================
//===========================================================================

#if defined(_Type)
#error "_Type" is already defined as a MACRO. Cannot be used again in template definition.
#endif
#if defined(_Type2)
#error "_Type2" is already defined as a MACRO. Cannot be used again in template definition.
#endif


//==SDOC========================================================================
//== Class: CPair
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//   Class providing a container to hold two distinct elements.
//
// Description:
//   The CPair class provides a container to hold two distinct elements.  It is
//   meant to be used by a higher level container or algorithm to bind these
//   elements,  An example would be to use the a CPair object to hold a key
//   and an element associated with that key.
//
//   Elements stored in a CPair object must at least implement the operator< and
//   operator!=.
//
// Warning:
//   This container is not thread safe.
//
// Location:
//   Cap/CPair.h
//
//==EDOC========================================================================
template<class _Type, class _Type2>
class CPair
{
// Published Interface
//---------------------
public:
    // << Constructors / Destructors / Operators >>
    //---------------------------------------------

    // Summary:
    //     Default constructor.
    //--------------------------
    CPair();

    // Summary:
    //     Constructor. Pair of items is passed.
    CPair(const _Type& rFirst, const _Type2& rSecond);

    // Summary:
    //     Copy constructor.
    //-----------------------
    CPair(const CPair<_Type, _Type2>& rPair);

    // Summary:
    //     Destructor.
    //-----------------
    virtual ~CPair();

    // Summary:
    //     Assignment operator.
    //--------------------------
    CPair<_Type, _Type2>& operator=(const CPair<_Type, _Type2>& rPair);

    // Summary:
    //     Less than operator.
    //-------------------------
    bool operator<(const CPair<_Type, _Type2>& rPair) const;

    // Summary:
    //     Less than or equal to operator.
    //-------------------------------------
    bool operator<=(const CPair<_Type, _Type2>& rPair) const;

    // Summary:
    //     Greater than operator.
    //----------------------------
    bool operator>(const CPair<_Type, _Type2>& rPair) const;

    // Summary:
    //     Greater than or equal to operator.
    //----------------------------------------
    bool operator>=(const CPair<_Type, _Type2>& rPair) const;

    // Summary:
    //     Different than operator.
    //------------------------------
    bool operator!=(const CPair<_Type, _Type2>& rPair) const;

    // Summary:
    //     Comparison operator.
    //--------------------------
    bool operator==(const CPair<_Type, _Type2>& rPair) const;


    // Manipulation of elements
    //-------------------------

    // Summary:
    //     This method returns a reference to the "first" member variable.
    //---------------------------------------------------------------------
    const _Type& GetFirst() const;

    // <COMBINE CPair::GetFirst@const>
    //------------------------------
    _Type& GetFirst();

    // Summary:
    //     This method returns a reference to the "second" member variable.
    //----------------------------------------------------------------------
    const _Type2& GetSecond() const;

    // <COMBINE CPair::GetSecond@const>
    //------------------------------
    _Type2& GetSecond();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
    // Private constructor used by the CMapPair class. See CMap.h for details.
    // =========================================================================
    // This constructor creates a pair with only the first element initialized
    // (no constructor is called for the second element of the pair).
    CPair(const _Type& rFirst);


// Hidden Members
//----------------
private:

    // CAlignedVariableStorage::Construct() and Destruct() will not be used
    // because some constructors require arguments and such constructors are
    // not provided by CAlignedVariableStorage. So we will do the placement new
    // manually. The destructor will also be called manually.
    CAlignedVariableStorage<_Type> m_first;
    CAlignedVariableStorage<_Type2> m_second;

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  CPair
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
CPair<_Type, _Type2>::CPair()
{
    new ((void*)m_first.Instance()) _Type();
    new ((void*)m_second.Instance()) _Type2();
}

//==SDOC========================================================================
//==
//==  CPair
//==
//==============================================================================
//
//  Parameters:
//    rFirst:
//      The element assigned to the "first" position of the pair.
//
//    rSecond:
//      The element assigned to the "second" position of the pair.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
CPair<_Type, _Type2>::CPair(const _Type& rFirst, const _Type2& rSecond)
{

    new ((void*)m_first.Instance()) _Type(rFirst);
    new ((void*)m_second.Instance()) _Type2(rSecond);
}

//==SDOC========================================================================
//==
//==  CPair
//==
//==============================================================================
//
//  Parameters:
//    rPair:
//      A reference to the source pair.
//
//  Description:
//      Copy Constructor.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
CPair<_Type, _Type2>::CPair(const CPair<_Type, _Type2>& rPair)
{
    new ((void*)m_first.Instance()) _Type(rPair.GetFirst());
    new ((void*)m_second.Instance()) _Type2(rPair.GetSecond());
}

template<class _Type, class _Type2>
inline
CPair<_Type, _Type2>::CPair(const _Type& rFirst)
{
    new ((void*)m_first.Instance()) _Type(rFirst);
}

//==SDOC====================================================================
//==
//==  ~CPair
//==
//==========================================================================
//
//  Description:
//      Destructor.
//
//==EDOC====================================================================
template<class _Type, class _Type2>
inline
CPair<_Type, _Type2>::~CPair()
{
    m_second.Instance()->~_Type2();
    m_first.Instance()->~_Type();
}

//==SDOC========================================================================
//==
//==  operator=
//==
//==============================================================================
//
//  Parameters:
//      rPair:
//          Reference to the right hand CPair.
//
//  Returns:
//      True if the first element of this is smaller than the other object's
//      first element or, if they are equal, if the second element of this is
//      smaller that the other object's second element. Otherwise, returns
//      false.
//
//  Description:
//      Validates if a CPair object is strictly smaller than another one.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
CPair<_Type, _Type2>& CPair<_Type, _Type2>::operator=(const CPair<_Type, _Type2>& rPair)
{
    GetFirst() = rPair.GetFirst();
    GetSecond() = rPair.GetSecond();
    return *this;
};

//==SDOC========================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//      rPair:
//          Reference to the right hand CPair.
//
//  Returns:
//      A reference to the assigned CPair.
//
//  Description:
//      Assigns the right handed Cpair to the left handed one.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
bool CPair<_Type, _Type2>::operator<(const CPair<_Type, _Type2>& rPair) const
{
    return (GetFirst() < rPair.GetFirst() || !(rPair.GetFirst() < GetFirst()) &&
            GetSecond() < rPair.GetSecond());
};

//==SDOC========================================================================
//==
//==  operator<=
//==
//==============================================================================
//
//  Parameters:
//      rPair:
//          Reference to the right hand CPair.
//
//  Returns:
//      True if the first element of this is smaller than the other object's
//      first element or, if they are equal, if the second element of this is
//      smaller than or equal to the other object's second element.  Otherwise,
//      returns false.
//
//  Description:
//      Validates if a CPair object is smaller than or equal to another one.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
bool CPair<_Type, _Type2>::operator<=(const CPair<_Type, _Type2>& rPair) const
{
    return (!(rPair < *this));
};

//==SDOC========================================================================
//==
//==  operator>
//==
//==============================================================================
//
//  Parameters:
//      rPair:
//          Reference to the right hand CPair.
//
//  Returns:
//      True if the first element of this is greater than the other object's
//      first element or, if they are equal, if the second element of this is
//      greater than the other object's second element. Otherwise, returns
//      false.
//
//  Description:
//      Validates if a CPair object is strictly greater than another one.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
bool CPair<_Type, _Type2>::operator>(const CPair<_Type, _Type2>& rPair) const
{
    return (rPair < *this);
};

//==SDOC========================================================================
//==
//==  operator>=
//==
//==============================================================================
//
//  Parameters:
//      rPair:
//          Reference to the right hand CPair.
//
//  Returns:
//      True if the first element of this is greater than the other object's
//      first element or, if they are equal, if the second element of this is
//      greater than or equal to the other object's second element.  Otherwise,
//      returns false.
//
//  Description:
//      Validates if a CPair object is greater than or equal to another one.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
bool CPair<_Type, _Type2>::operator>=(const CPair<_Type, _Type2>& rPair) const
{
    return (!(*this < rPair));
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rPair:
//          Reference to the right hand CPair.
//
//  Returns:
//      True if the first element of this is different than the other object's
//      first element or, if they are equal, if the second element of this is
//      different than to the other object's second element. Otherwise, returns
//      false.
//
//  Description:
//      Validates if a CPair object is different than another one.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
bool CPair<_Type, _Type2>::operator!=(const CPair<_Type, _Type2>& rPair) const
{
    return (GetFirst() != rPair.GetFirst() ||
            (!(GetFirst() != rPair.GetFirst()) && GetSecond() != rPair.GetSecond()));
};

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//      rPair:
//          Reference to the right hand CPair.
//
//  Returns:
//      True if the first element of this is equal to the other object's first
//      element and the second element of this is equal to the other object's
//      second element.  Otherwise, returns false.
//
//  Description:
//      Validates if a CPair object is equal to another one.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
bool CPair<_Type, _Type2>::operator==(const CPair<_Type, _Type2>& rPair) const
{
    return (!(*this != rPair));
};

//==SDOC========================================================================
//==
//==  GetFirst
//==
//==============================================================================
//
//  Returns:
//      The first element of the pair.
//
//  Description:
//      This method returns a reference to the "first" member variable.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
const _Type& CPair<_Type, _Type2>::GetFirst() const
{
	return *(reinterpret_cast<const _Type*>(m_first.Instance()));
};

template<class _Type, class _Type2>
inline
_Type& CPair<_Type, _Type2>::GetFirst()
{
    return *(reinterpret_cast<_Type*>(m_first.Instance()));
};

//==SDOC========================================================================
//==
//==  GetSecond
//==
//==============================================================================
//
//  Returns:
//      The second element of the pair.
//
//  Description:
//      This method returns a reference to the "second" member variable.
//
//==EDOC========================================================================
template<class _Type, class _Type2>
inline
const _Type2& CPair<_Type, _Type2>::GetSecond() const
{
    return *(reinterpret_cast<const _Type2*>(m_second.Instance()));
};

template<class _Type, class _Type2>
inline
_Type2& CPair<_Type, _Type2>::GetSecond()
{
    return *(reinterpret_cast<_Type2*>(m_second.Instance()));
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif
