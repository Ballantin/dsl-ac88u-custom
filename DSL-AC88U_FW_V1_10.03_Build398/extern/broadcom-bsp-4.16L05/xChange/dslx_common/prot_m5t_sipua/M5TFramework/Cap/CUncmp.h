//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CUNCMP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUNCMP_H
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
//== Class: CUncmp
//========================================
//<GROUP CAP_TEMPLATES>
//
// Summary:
//   Class allowing the use of comparison operators within container classes.
//
// Description:
//   This class is a template helper for the container classes CList, CVList
//   CVector, CAATree, and CMap. The purpose of CUncmp is to allow the use of
//   comparison methods without having to implement the != and < operators when
//   using incomparable types such as structures or complex objects.
//
//   CUncmp is required when the user calls one the following container class'
//   methods:
//
//      For CList, CVList, and CVector:
//          - InsertSorted
//          - Sort
//          - Find
//          - FindSorted
//          - GetMinElementIndex
//          - GetMaxElementIndex
//
//      For CAATree and CMap:
//          - Insert
//          - FindPtr
//          - EraseElement
//          - Erase
//
// Example:
//   A user may want to use a CList template of structures and call
//   CList::InsertSorted by passing its custom compare function as a parameter.
//
//   The following code shows how the user gets a compilation error when the
//   CUncmp helper is not used:
//
//  <CODE>
//
//      #include "Config/MxConfig.h"
//      #include "Cap/CList.h"
//
//      typedef struct
//      {
//          int i;
//      } STest;
//
//      int MyCompareFunction(IN const STest& rOneElement,
//                            IN const STest& rOtherElement,
//                            IN mxt_opaque opq)
//      {
//          if (rOneElement.i < rOtherElement.i)
//          {
//              return -1;
//          }
//          else if (rOneElement.i != rOtherElement.i)
//          {
//              return 1;
//          }
//          else
//          {
//              return 0;
//          }
//      }
//
//      int main()
//      {
//          CList<STest>    lstSTest;
//          STest           sTest;
//
//          // ********************************************************
//          // The compiler fails because the < and != operators are not
//          // defined by the STest structure. Operators are required
//          // by the default internal CList::Compare method.
//          // ********************************************************
//          lstSTest.InsertSorted(sTest, MyCompareFunction);
//
//          return 0;
//      }
//
//
//  </CODE>
//
//   The following code shows how the user may use the CUncmp helper to avoid
//   compiler errors:
//
//  <CODE>
//
//      #include "Config/MxConfig.h"
//      #include "Cap/CList.h"
//      #include "Cap/CUncmp.h"
//
//      typedef struct
//      {
//          int i;
//      } STest;
//
//      int MyCompareFunction(IN const CUncmp<STest>& rOneElement,
//                            IN const CUncmp<STest>& rOtherElement,
//                            IN mxt_opaque opq)
//      {
//          if (rOneElement.i < rOtherElement.i)
//          {
//              return -1;
//          }
//          else if (rOneElement.i != rOtherElement.i)
//          {
//              return 1;
//          }
//          else
//          {
//              return 0;
//          }
//      }
//
//      int main()
//      {
//          CList< CUncmp<STest> >  lstSTest;
//          STest                   stTest;
//
//          lstSTest.InsertSorted(CUncmp<STest>(stTest), MyCompareFunction);
//
//          return 0;
//      }
//
//
//  </CODE>
//
//   This helper template class inherits from the parameterized class and
//   implements the != and < operators.
//
//   When using a CAATree or CMap, the comparison function must be provided with
//   the SetComparisonFunction method before adding elements to the container.
//
// Warning:
//   The Sort method of the container classes MUST NOT be used until the
//   comparison function is provided, otherwise it generates an ASSERT.
//
// Location:
//   Cap/CUncmp.h
//
// See Also:
//   CList, CVList, CVector, CAATree, CMap
//
//==EDOC========================================================================
template<class _Type>
class CUncmp : public _Type
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //     Different than operator.
    //-------------------------------
    bool operator!=(IN const CUncmp<_Type>& rUncmp) const;

    // Summary:
    //     Less than operator.
    //-------------------------------
    bool operator<(IN const CUncmp<_Type>& rUncmp) const;

    // Summary:
    //      Default Constructor.
    //---------------------------
    CUncmp();

    // Summary:
    //      Copy Constructor.
    //------------------------
    CUncmp(IN const _Type& rType);

    // Summary:
    //     Destructor.
    //------------------
    ~CUncmp();

//-- Hidden Methods
//-------------------
protected:
private:

//-- Hidden Data Members
//------------------------
protected:
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rUncmp:
//      Reference to the CUncmp to compare.
//
//  Returns:
//      Returns true if the address of the left hand CUncmp is different than
//      the address of the right hand CUncmp, otherwise returns false.
//
//  Description:
//      Verifies if the left hand CUncmp address is different than the right
//      hand CUncmp address.
//
//      This operator MUST NOT be called by the containers. This is avoided when
//      the user provides its custom compare function to the container. Any call
//      to this operator generates an ASSERT.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CUncmp<_Type>::operator!=(IN const CUncmp<_Type>& rUncmp) const
{
    // CUncmp does not support compare operator
    MX_ASSERT(false);
    return this != &rUncmp;
}

//==SDOC========================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Parameters:
//    rUncmp:
//      Reference to the CUncmp to compare.
//
//  Returns:
//      Returns true if the address of the left hand CUncmp is less than the
//      address of the right hand CUncmp, else returns false.
//
//  Description:
//      Verifies if the left hand CUncmp address is less than the right hand
//      CUncmp address.
//
//      This operator MUST NOT be called by the containers. This is avoided
//      when the user provides its custom compare function to the container.
//      Any call to this operator generates an ASSERT.
//
//==EDOC========================================================================
template<class _Type>
inline
bool CUncmp<_Type>::operator<(IN const CUncmp<_Type>& rUncmp) const
{
    // CUncmp does not support compare operator
    MX_ASSERT(false);
    return this < &rUncmp;
}

//==SDOC====================================================================
//==
//==  CUncmp
//==
//==========================================================================
//
//  Description:
//      Default Constructor.
//
//==EDOC====================================================================
template<class _Type>
inline
CUncmp<_Type>::CUncmp()
{
}

//==SDOC====================================================================
//==
//==  CUncmp
//==
//==========================================================================
//
//  Parameters:
//    rType:
//      A reference to _Type (parameterized class or structure).
//
//  Description:
//      Copy Constructor.
//
//==EDOC====================================================================
template<class _Type>
inline
CUncmp<_Type>::CUncmp(IN const _Type& rType)
{
    _Type* pThis = this;
    *pThis = rType;
}

//==SDOC====================================================================
//==
//==  ~CUncmp
//==
//==========================================================================
//
//  Description:
//      Destructor.
//
//==EDOC====================================================================
template<class _Type>
inline
CUncmp<_Type>::~CUncmp()
{
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CUNCMP_H

