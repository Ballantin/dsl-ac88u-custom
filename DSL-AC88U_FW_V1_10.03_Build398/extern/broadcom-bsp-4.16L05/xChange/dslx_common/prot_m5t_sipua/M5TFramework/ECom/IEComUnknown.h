//==============================================================================
//==============================================================================
//
//      Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_IECOMUNKNOWN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IECOMUNKNOWN_H
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
#include "Config/FrameworkCfg.h" // MXD_ECOM_ENABLE_SUPPORT
#endif

#if defined(MXD_ECOM_ENABLE_SUPPORT)

#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif

#ifndef MXG_CSHAREDPTR_H
#include "Basic/CSharedPtr.h"
#endif

// Data Member

// Interface Realized & Parent

// Forward Declarations Outside of the Namespace

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace

//M5T_INTERNAL_USE_BEGIN
// Patch for GCC 2.7.2
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
    #define MXD_FRAMEWORK_INTERNAL_GCC272_COMPILER_FIX_FORWARD_DECLARATIONS
        // Get all Ecom class forward declarations from external header file.
        // When compiling/linking with GCC 2.7.2, this file must be created by
        // integrator and put somewhere in the application path.
        //---------------------------------------------------------------------
        #include "Gcc272CompilerFix.h"
    #undef MXD_FRAMEWORK_INTERNAL_GCC272_COMPILER_FIX_FORWARD_DECLARATIONS
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IEComUnknown);

// Need to override QueryIf to avoid GCC 2.7.2 linker issues.
//------------------------------------------------------------
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
#define MX_QUERY_IF_IMPLEMENTATION(szIfaceName)                                      \
    mxt_result QueryIf(OUT szIfaceName** ppInterface)                                \
    {                                                                                \
        MX_DECLARE_ECOM_IID(szIfaceName);                                            \
        if (ppInterface != NULL)                                                     \
        {                                                                            \
            return QueryIf(IID_##szIfaceName,                                        \
                           reinterpret_cast<void**>(ppInterface));                   \
        }                                                                            \
        return resFE_INVALID_ARGUMENT;                                               \
    }
#endif
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: IEComUnknown
//=================================
//<GROUP ECOM_CLASSES>
//
//  Summary:
//      Base class to be inherited by all ECOM interfaces.
//
//  Description:
//      ECOM classes can be described from two points of view: the ECOM
//      implementer's view and the ECOM user's view. IEComUnknown is the
//      interface that is ALWAYS used by ECOM users.
//
//      This interface is at the base of the entire ECOM mechanism. It provides
//      the capacity to dynamically discover the interfaces supported by an
//      ECOM class with QueryIf and to manage the ECOM object's lifetime using
//      reference counting with AddIfRef and ReleaseIfRef.
//
//      Inheritance from this interface is limited to ECOM interfaces only.
//      Moreover, ECOM interfaces MUST absolutely inherit from this interface.
//      This restriction comes from the QueryIf rules described below.
//
//      Overriding IEComUnknown's methods is required by all ECOMs and MUST be
//      done using MX_DECLARE_IECOMUNKNOWN or MX_DECLARE_DELEGATING_IECOMUNKNOWN
//      macros.
//
//      #QueryIf rules:#
//      - You always get the same IEComUnknown.
//      - You can get an interface if you got it before.
//      - You can get the interface you have.
//      - You can always get back where you started.
//      - You can get there from anywhere if you can get there from somewhere.
//
//      #Reference counting rules:#
//      The reference count of an ECOM object determines if it must remain in
//      memory. When the ECOM object is no longer needed, its reference count
//      should become zero, causing it to delete itself from memory. Reference
//      counting is achieved with AddIfRef and ReleaseIfRef to inform the ECOM
//      object about the number of references kept on it.
//      - Always call AddIfRef in a method that returns an interface. QueryIf
//        and CreateEComInstance are examples.
//      - Call AddIfRef each time a copy of an ECOM interface pointer is kept.
//      - Call ReleaseIfRef on an ECOM interface pointer before making it
//        invalid.
//      - Keep the count right to avoid memory leaks. Each successful call to
//        CreateEComInstance, QueryIf, and AddIfRef must be matched with an
//        associated call to ReleaseIfRef.
//
//      #ECOM object ownership rules:#
//      - If you created the ECOM object, you're owning it.
//      - If you need an ECOM object, it doesn't necessarily mean you have to
//        own it.
//      - If you own an ECOM object, this ECOM object should not own you.
//      - If an ECOM object's lifetime is longer than yours, you don't need to
//        own it.
//      - A container owns its content like parents own their children, but the
//        opposite is not true.
//
//  Warning:
//      - In any ECOM class hierarchy, only one (1) occurrence of CEComUnknown
//        or CEComDelegatingUnknown is permitted.
//
//      - In any ECOM class hierarchy, only one (1) implementation of each
//        IEComUnknown interface method is permitted.
//
//      - It is an error for one ECOM to inherit from another ECOM. In such a
//        case, aggregation and containment can be used instead.
//
//  Location:
//      ECom/IEComUnknown.h
//
//  See Also:
//      CreateEComInstance, CEComUnknown, CEComDelegatingUnknown
//
//==============================================================================
class IEComUnknown
{
// Published Interface
//-------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IEComUnknown);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  QueryIf
    //==
    //==========================================================================
    //
    //  Summary:
    //      Queries an object for a supported interface.
    //
    //  Parameters:
    //      iidRequested:
    //          The interface identifier (IID) of the interface to retrieve.
    //
    //      ppInterface:
    //          The pointer of the interface pointer variable to store the
    //          requested interface.
    //
    //  Returns:
    //      * resS_OK: The query succeeded. ppInterface now contains a pointer
    //                 to the requested interface.
    //
    //      * resFE_MITOSFW_ECOM_NOINTERFACE: No interface could be found
    //                                        corresponding to the iidRequested.
    //                                        In this condition, NULL is
    //                                        assigned the content of
    //                                        ppInterface.
    //
    //  Description:
    //      This method is used to query an object for a supported interface.
    //      If the object supports the interface specified by iidRequested,
    //      then the pointer to this interface is returned in ppInterface.
    //
    //      When successful, a call to QueryIf automatically increments the
    //      object's reference count.
    //
    //  <CODE>
    //      // Example of QueryIf.
    //      // Note the IID_ISomeOtherInterface parameter and the
    //      // reinterpret_cast that are required.
    //
    //      ISomeOtherInterface* pSomeOtherInterface = ...
    //      ISomeInterface* pSomeInterface = NULL;
    //      mxt_result res =
    //          pSomeOtherInterface->
    //              QueryIf(IID_ISomeOtherInterface,
    //                      reinterpret_cast<void**>(&pSomeInterface));
    //  </CODE>
    //
    //  See also:
    //      mxt_iid,
    //      <LINK IEComUnknown, Reference counting rules>
    //
    //==========================================================================
    virtual mxt_result QueryIf(IN  mxt_iid iidRequested, OUT void** ppInterface) = 0;

//M5T_INTERNAL_USE_BEGIN
// On GCC 2.7.2, we have to manually instantiate the templated QueryIf method
// for each ECom derivative. Gcc272CompilerFix.h will contain all QueryIf
// instanciations, except for the IID_IEComUnknown version which is defined
// below. IID_IEComUnknown is a special case because the variable itself must
// be declared in global scope in this file (see
// MX_DECLARE_ECOM_IID(IEComUnknown) above).
//----------------------------------------------------------------------------
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)

    // Get all QueryIf methods from external header file.
    //----------------------------------------------------
    #define MXD_FRAMEWORK_INTERNAL_GCC272_COMPILER_FIX_QUERYIF_IMPLEMENTATIONS
        #include "Gcc272CompilerFix.h"
    #undef MXD_FRAMEWORK_INTERNAL_GCC272_COMPILER_FIX_QUERYIF_IMPLEMENTATIONS

    mxt_result QueryIf(OUT IEComUnknown** ppInterface)
    {
        if (ppInterface != NULL)
        {
            return QueryIf(IID_IEComUnknown,
                           reinterpret_cast<void**>(ppInterface));
        }
        return resFE_INVALID_ARGUMENT;
    }

#else
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  QueryIf
    //==
    //==========================================================================
    //
    //  Summary:
    //      Queries an object for a supported interface.
    //
    //  Parameters:
    //      ppInterface:
    //          The pointer of the interface pointer variable to store the
    //          requested interface.
    //
    //  Returns:
    //      * resS_OK: The query succeeded. ppInterface now contains a pointer
    //                 to the requested interface.
    //
    //      * resFE_MITOSFW_ECOM_NOINTERFACE: No interface could be found
    //                                        corresponding to the iidRequested.
    //                                        In this condition, NULL is
    //                                        assigned the content of
    //                                        ppInterface.
    //
    //      * resFE_INVALID_ARGUMENT: ppInterface parameter is NULL.
    //
    //  Description:
    //      This is a templated version of the original QueryIf method. It is
    //      designed to ease code readability.
    //
    //      For this method to compile, the **ppInterface referenced class must
    //      contain MX_DECLARE_ECOM_GETIID.
    //
    //      This method is used to query an object for a supported interface.
    //
    //      When successful, a call to QueryIf automatically increments the
    //      object's reference count.
    //
    //  <CODE>
    //      // Example of QueryIf, using the templated method.
    //
    //      ISomeOtherInterface* pSomeOtherInterface = ...
    //      ISomeInterface* pSomeInterface = NULL;
    //      mxt_result res = pSomeOtherInterface->QueryIf(&pSomeInterface);
    //  </CODE>
    //
    //  See also:
    //      MX_DECLARE_ECOM_GETIID,
    //      <LINK IEComUnknown, Reference counting rules>
    //
    //==========================================================================
    template<class _Type> mxt_result QueryIf(OUT _Type** ppInterface)
    {
        // Has to be inlined here because Visual C++ 6.0 does not like out of class
        // inlined specialized template method.
        if (ppInterface != NULL)
        {
            return QueryIf((*ppInterface)->GetIID(), reinterpret_cast<void**>(ppInterface));
        }
        return resFE_INVALID_ARGUMENT;
    }

    //==========================================================================
    //==
    //==  QueryIf
    //==
    //==========================================================================
    //
    //  Summary:
    //      Queries an object for a supported interface.
    //
    //  Parameters:
    //      rInterface:
    //          Reference to the CSharedPtr object that will store the
    //          requested interface.
    //
    //  Returns:
    //      * resS_OK: The query succeeded. rInterface now contains an pointer
    //                 to the requested interface.
    //
    //      * resFE_MITOSFW_ECOM_NOINTERFACE: No interface could be found
    //                                        corresponding to the iidRequested.
    //                                        In this condition, NULL is
    //                                        assigned the content of
    //                                        ppInterface.
    //
    //  Description:
    //      This is a templated version of the original QueryIf method. It is
    //      designed to ease code readability.
    //
    //      For this method to compile, the rInterface referenced class must
    //      contain MX_DECLARE_ECOM_GETIID.
    //
    //      This method is used to query an object for a supported interface.
    //
    //      When successful, a call to QueryIf automatically increments the
    //      object's reference count.
    //
    //  <CODE>
    //      // Example of QueryIf, using the templated method.
    //
    //      CSharedPtr<ISomeOtherInterface*> spSomeOtherInterface = ...
    //      CSharedPtr<ISomeInterface> spSomeInterface;
    //      mxt_result res = spSomeOtherInterface->QueryIf(spSomeInterface);
    //  </CODE>
    //
    //  See also:
    //      MX_DECLARE_ECOM_GETIID,
    //      <LINK IEComUnknown, Reference counting rules>
    //
    //==========================================================================
    template<class _Type> mxt_result QueryIf(OUT CSharedPtr<_Type>& rInterface)
    {
        _Type** ppInterface = static_cast<_Type**>(rInterface);
        return QueryIf((*ppInterface)->GetIID(), reinterpret_cast<void**>(ppInterface));
    }
//M5T_INTERNAL_USE_BEGIN
#endif // #if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  AddIfRef
    //==
    //==========================================================================
    //
    //  Summary:
    //      Increments the reference count on the ECOM implementing this
    //      interface.
    //
    //  Returns:
    //      The increased reference count ( meaningless see warning below! ).
    //
    //  Description:
    //      This method is used to increment the reference count on the ECOM
    //      implementing this interface. It is the reference count that controls
    //      the lifetime of every ECOM. See <LINK IEComUnknown, Reference counting rules>.
    //
    //  Warning:
    //      The return value is meaningless and is provided for debugging
    //      purposes only.
    //
    //  See also:
    //      ReleaseIfRef, <LINK IEComUnknown, Reference counting rules>
    //
    //==========================================================================
    virtual unsigned int AddIfRef() = 0;

    //==========================================================================
    //==
    //==  ReleaseIfRef
    //==
    //==========================================================================
    //
    //  Summary:
    //      Decrements the reference count on the ECOM implementing this
    //      interface.
    //
    //  Returns:
    //      The decreased reference count ( meaningless see warning below! ).
    //
    //  Description:
    //      This method is used to decrement the reference count on the ECOM
    //      implementing this interface. It is the reference count that controls
    //      the lifetime of every ECOM. When the reference count reaches zero,
    //      the ECOM deletes itself.
    //
    //  Warning:
    //      The return value is meaningless and is provided for debugging
    //      purposes only.
    //
    //  See Also:
    //      AddIfRef, <LINK IEComUnknown, Reference counting rules>.
    //
    //==========================================================================
    virtual unsigned int ReleaseIfRef() = 0;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//-------------------------
protected:

    // << Constructors / Destructors >>
    //--------------------------------------------
    IEComUnknown() {}
    virtual ~IEComUnknown() {}

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    IEComUnknown(const IEComUnknown& from);
    IEComUnknown& operator=(const IEComUnknown& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_IECOMUNKNOWN_H

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT)

