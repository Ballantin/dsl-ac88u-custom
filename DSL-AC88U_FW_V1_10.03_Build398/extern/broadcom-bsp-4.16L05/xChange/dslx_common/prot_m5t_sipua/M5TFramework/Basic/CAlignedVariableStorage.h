//==SDOC========================================================================
//==============================================================================
//
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
#ifndef MXG_CALIGNEDVARIABLESTORAGE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CALIGNEDVARIABLESTORAGE_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

#if defined (__cplusplus)

//M5T_INTERNAL_USE_BEGIN
//==============================================================================
//==== MXD_DEFAULT_ALIGNMENT_TYPE used by CAlignedVariableStorage ==============
//==============================================================================
#if defined(MXD_COMPILER_GNU_GCC)
    #if (MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
        // We do not set a pre-defined alignment.
        #undef MXD_DEFAULT_ALIGNMENT_TYPE
    #else
        // We assume 32 bits alignment.
        //-----------------------------
        #define MXD_DEFAULT_ALIGNMENT_TYPE uint32_t
    #endif
#elif defined(MXD_COMPILER_MS_VC)
    #if (MXD_COMPILER_MS_VC > 1200)
        // We do not set a pre-defined alignment.
        #undef MXD_DEFAULT_ALIGNMENT_TYPE
    #else
        // We assume 32 bits alignment.
        //-----------------------------
        #define MXD_DEFAULT_ALIGNMENT_TYPE uint32_t
    #endif
#else
    #if defined(MXD_COMPILER_TI_CL6X)
        // We assume 32 bits alignment.
        //-----------------------------
        #define MXD_DEFAULT_ALIGNMENT_TYPE uint32_t
    #else
        // We assume 64 bits alignment.
        //-----------------------------
        #define MXD_DEFAULT_ALIGNMENT_TYPE uint64_t
    #endif
#endif

#if !defined(MXD_DEFAULT_ALIGNMENT_TYPE)
//==============================================================================
//==
//==  IfTypedef
//==
//==============================================================================
//
//  Summary:
//      This template implements a conditional type selection
//
//  Parameters:
//    bCondition:
//      A condition to be evaluated.
//
//    _TypeTrue:
//      The type to be selected when the condition is true.
//
//    _TypeFalse:
//      The type to be selected when the condition is false.
//
//  Returns:
//      _TypeTrue when the evaluated condition is true.
//      _TypeFalse when the evaluated condition is false.
//
//  Description:
//      This template is made of 3 templates:
//      - The general definition of the template.
//      - The specialization for a true condition.
//      - The specialization for a false condition.
//
//==EDOC========================================================================

// Summary:
//  This is the general definition of the template.
//------------------------------------------------
template <bool bCondition, typename _TypeTrue, typename _TypeFalse>
struct IfTypedef;

// Summary:
//  This is the specialization for the true condition.
//----------------------------------------------------
template <typename _TypeTrue, typename _TypeFalse>
struct IfTypedef<true, _TypeTrue, _TypeFalse>
{
  typedef _TypeTrue result;
};

// Summary:
//  This is the specialization for the false condition.
//-----------------------------------------------------
template <typename _TypeTrue, typename _TypeFalse>
struct IfTypedef<false, _TypeTrue, _TypeFalse>
{
  typedef _TypeFalse result;
};
#endif

//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//== Class: CAlignedVariableStorage
//==============================================================================
//<GROUP BASIC_TEMPLATES>
//
// Summary:
//   Implements a uint32_t (or a uint64_t for Solaris) aligned container.
//
// Description:
//   The CAlignedVariableStorage class provides a uint32_t (or a uint64_t for
//   Solaris) aligned container for objects that require such an alignment.
//
//   The most common scenario is when an object is allocated into a byte array
//   using a placement new. Depending on the platform, the byte array may not be
//   aligned properly. As a result, if the allocated object needs to access a
//   member that requires alignment, it will fail. The failure may cause a crash,
//   a processor exception, a bus error, etc.
//
//   The following is a simple example of an encountered problem.
//   <CODE>
//   class CExample
//   {
//      public:
//          CExample(){};
//          ~CExample(){};
//          void*       m_pVoid;
//   };
//
//   uint8_t gs_auArray[sizeof(CExample)];
//   int main()
//   {
//      CExample* pExample = new (gs_auArray) CExample;
//      // **************************************************************
//      // Unaligned access will occur if auArray is not aligned.
//      // **************************************************************
//      pExample->m_pVoid = NULL;
//
//      // Object destruction.
//      // -------------------
//      reinterpret_cast<CExample*>(gs_auArray)->~CExample();
//      ...
//   }
//   </CODE>
//
//   To ensure that the allocated objet is properly aligned, the
//   CAlignedVariableObject must be used as below:
//
//   <CODE>
//   CAlignedVariableStorage<CExample> gs_example;
//   int main()
//   {
//
//      // Object construction.
//      // --------------------
//      CExample* pExample = gs_example.Construct();
//
//      // **************************************************************
//      // Access will always be aligned.
//      // **************************************************************
//      pExample->m_pVoid = NULL;
//
//      // Object destruction.
//      // --------------------
//      pExample.Destruct();
//   }
//   </CODE>
//
// Additional note:
//   If MXD_DEFAULT_ALIGNMENT_TYPE is defined, the type it defines is then used
//   to align the structure. Otherwise, the default alignment of _Type is used,
//
// Warning:
//   Using this template requires that instantiated objects have a default
//   constructor, a destructor, and at least one non static data member.
//
// Location:
//   Basic/CAlignedVariableStorage.h
//
//==EDOC========================================================================
template<class _Type>
class CAlignedVariableStorage
{
//-- Published Interface
//------------------------
public:
    // Constructors / Destructors / Operators
    //-------------------------------------------------

    // Summary:
    //  Empty construtor.
    //--------------------
    CAlignedVariableStorage(){};

#ifndef MXD_OS_SYMBIAN
    // Summary:
    //  Empty destructor.
    //  The destructor is wanted to be non virtual for two reasons. First, there
    //  are no foreseen usecases where a class will inherit from this one.
    //  Finally, the object size will be kept to the minimum since there will
    //  be no virtual table.
    //--------------------------------------------------------------------------
    ~CAlignedVariableStorage(){};
#endif

    // Summary:
    //  Constructs the object and returns an instance pointer.
    //--------------------------------------------------------
    _Type* Construct();

    // Summary:
    //  Destructs the object.
    //------------------------
    void Destruct();

    // Summary:
    //  Returns a pointer to the object's instance.
    //----------------------------------------------
    _Type* Instance();

    // Summary:
    //  Returns a const pointer to the object's instance.
    //----------------------------------------------------
    const _Type* Instance() const;

    // Summary:
    //  Returns a reference to the object's value.
    //---------------------------------------------
    operator _Type&();

    // Summary:
    //  Assigns a value to the object.
    //---------------------------------
    CAlignedVariableStorage<_Type>& operator=(IN const _Type& rValue);

//M5T_INTERNAL_USE_BEGIN
private:
    // An union is used to make sure that the byte array is always aligned on
    // a desired boundary.
    //-----------------------------------------------------------------------
    union UAlignStorage
    {
        // Storage array.
        //----------------
        uint8_t m_auData[sizeof(_Type)];

        // Make sure that the storage array is aligned properly.
        //-------------------------------------------------------
        #if defined(MXD_DEFAULT_ALIGNMENT_TYPE)
            // We use the defined alignment.
            //------------------------------
            MXD_DEFAULT_ALIGNMENT_TYPE m_uUnused;
        #else
            // We determine the best alignment based on _Type.
            //------------------------------------------------
            typename IfTypedef<
                MX_ALIGNMENT_OF(_Type) == MX_ALIGNMENT_OF(uint8_t),
                uint8_t,
                uint8_t>::result m_uint8Unused;
            typename IfTypedef<
                MX_ALIGNMENT_OF(_Type) == MX_ALIGNMENT_OF(uint16_t),
                uint16_t,
                uint8_t>::result m_uint16Unused;
            typename IfTypedef<
                MX_ALIGNMENT_OF(_Type) == MX_ALIGNMENT_OF(uint32_t),
                uint32_t,
                uint8_t>::result m_uint32Unused;
            typename IfTypedef<
                MX_ALIGNMENT_OF(_Type) == MX_ALIGNMENT_OF(uint64_t),
                uint64_t,
                uint8_t>::result m_uint64Unused;
        #endif
    };

    // Object storage.
    //-----------------
    UAlignStorage m_stuStorage;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC====================================================================
//==
//==  Construct
//==
//==========================================================================
//
//  Returns:
//      A pointer to the object's instance.
//
//  Description:
//      Constructs the object using a placement new. The object's constructor
//      is invoked. It also returns a pointer to the created object's
//      instance.
//
// Warning:
//   Using this template requires that instantiated objects have a default
//   constructor, a destructor, and at least one non static data member.
//
//  See Also:
//      Destruct
//
//==EDOC====================================================================
template<class _Type>
inline _Type* CAlignedVariableStorage<_Type>::Construct()
{
    // Make sure all classes have default constructors!
    //--------------------------------------------------
    new (&m_stuStorage) _Type;
    return reinterpret_cast<_Type*>(&m_stuStorage);
}

//==SDOC====================================================================
//==
//==  Destruct
//==
//==========================================================================
//
//  Description:
//      Destructs the object. The object's destructor is invoked.
//
// Warning:
//   Using this template requires that instantiated objects have a default
//   constructor, a destructor, and at least one non static data member.
//
//  See Also:
//      Construct
//
//==EDOC====================================================================
template<class _Type>
inline void CAlignedVariableStorage<_Type>::Destruct()
{
    reinterpret_cast<_Type*>(&m_stuStorage)->~_Type();
}

//==SDOC====================================================================
//==
//==  Instance
//==
//==========================================================================
//
//  Returns:
//      Returns a pointer to the object's instance.
//
//  Description:
//      Returns the object's instance.
//
//==EDOC====================================================================
template<class _Type>
inline _Type* CAlignedVariableStorage<_Type>::Instance()
{
    return reinterpret_cast<_Type*>(&m_stuStorage);
}

//==SDOC====================================================================
//==
//==  Instance
//==
//==========================================================================
//
//  Returns:
//      Returns a const pointer to the object's instance.
//
//  Description:
//      Returns the object's instance.
//
//==EDOC====================================================================
template<class _Type>
inline const _Type* CAlignedVariableStorage<_Type>::Instance() const
{
    return reinterpret_cast<const _Type*>(&m_stuStorage);
}

//==SDOC====================================================================
//==
//==  _Type&
//==
//==========================================================================
//
//  Returns:
//      A reference to the object's value.
//
//  Description:
//      Returns a reference to the object's value.
//
//==EDOC====================================================================
template<class _Type>
inline CAlignedVariableStorage<_Type>::operator _Type&()
{
    return *(reinterpret_cast<_Type*>(&m_stuStorage));
}

//==SDOC====================================================================
//==
//==  operator=
//==
//==========================================================================
//
//  Parameters:
//    rValue:
//      Object's value.
//
//  Returns:
//      A reference to this instance.
//
//  Description:
//      Assigns a value to the object.
//
//==EDOC====================================================================
template<class _Type>
inline CAlignedVariableStorage<_Type>& CAlignedVariableStorage<_Type>::operator=(IN const _Type& rValue)
{
    *(reinterpret_cast<_Type*>(&m_stuStorage)) = rValue;
    return *this;
}

#endif // #if defined (__cplusplus)

MX_NAMESPACE_END(MXD_GNS)
#endif // #ifndef MXG_CALIGNEDVARIABLESTORAGE_H
