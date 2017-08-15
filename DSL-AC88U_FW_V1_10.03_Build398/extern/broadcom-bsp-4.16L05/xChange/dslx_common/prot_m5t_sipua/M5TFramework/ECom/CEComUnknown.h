//==SDOC========================================================================
//==============================================================================
//
//        Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CECOMUNKNOWN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CECOMUNKNOWN_H
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

// Data Member
#ifndef MXG_CATOMIC_H
#include "Kernel/CAtomic.h"
#endif

// Forward Declarations Outside of the Namespace

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
//==SDOC========================================================================
//==
//==  class IEComNonDelegatingUnknown
//==
//==============================================================================
//<GROUP ECOM_CLASSES>
//
//  Summary:
//      Used to make aggregation transparent to users.
//
//  Description:
//      This interface is identical to IEComUnknown with the exception that the
//      prefix "NonDelegating" has been added to the interface name and its
//      methods. Its purpose is to help make aggregation transparent to users.
//
//      Inheritance from this interface is strictly prohibited. The class
//      CEComUnknown is the only one allowed.
//
//  See also:
//      CEComUnknown, IEComUnknown
//
//==EDOC========================================================================
class IEComNonDelegatingUnknown
{
// Friends Declaration
//-------------------------


// Published Interface
//-------------------------
public:

    //==========================================================================
    //==
    //==  NonDelegatingQueryIf
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
    //      * resS_OK: The query has been successful. ppInterface now contains a
    //                 pointer to the requested interface.
    //
    //      * resFE_MITOSFW_ECOM_NOINTERFACE: No interface has been found
    //                                        corresponding to the iidRequested.
    //                                        In this condition, NULL is
    //                                        assigned to the content of
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
    //                      OUT reinterpret_cast<void**>(&pSomeInterface));
    //
    //      // There is a templated version of QueryIf that can be used. This
    //      // version does not require the reinterpret_cast, determines the
    //      // queried interface's iid on its own and, is preferred. See
    //      // IEComUnknown::QueryIf documentation for more details.
    //      pSomeOtherInterface->
    //              QueryIf(OUT &pSomeInterface);
    //  </CODE>
    //
    //  See also:
    //      mxt_iid,
    //      <LINK IEComUnknown, Reference counting rules>
    //
    //==========================================================================
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested, OUT void** ppInterface) = 0;

    //==========================================================================
    //==
    //==  NonDelegatingAddIfRef
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
    //      NonDelegatingReleaseIfRef,
    //      <LINK IEComUnknown, Reference counting rules>
    //
    //==========================================================================
    virtual unsigned int NonDelegatingAddIfRef() = 0;

    //==========================================================================
    //==
    //==  NonDelegatingReleaseIfRef
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
    //      NonDelegatingAddIfRef,
    //      <LINK IEComUnknown, Reference counting rules>.
    //
    //==========================================================================
    virtual unsigned int NonDelegatingReleaseIfRef() = 0;

// Hidden Methods
//-------------------------
protected:

    // << Constructors / Destructors >>
    //--------------------------------------------
    IEComNonDelegatingUnknown(){}
    virtual ~IEComNonDelegatingUnknown(){}

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    IEComNonDelegatingUnknown(const IEComNonDelegatingUnknown& from);
    const IEComNonDelegatingUnknown& operator=(const IEComNonDelegatingUnknown& from);
};
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//==
//==  class CEComUnknown
//==
//==============================================================================
//<GROUP ECOM_CLASSES>
//
// Summary:
//      Used as the base class of every ECOM object when it
//      is known that they will never be aggregated.
//
//  Description:
//      ECOM objects can be described from two points of view. The ECOM
//      implementer's view and the ECOM user's view. Inheriting CEComUnknown
//      is related to the implementer's view.
//
//      This class MUST be used as the base class of every ECOM object when it
//      is known that they will never be aggregated. The class holds one
//      important information about the object's state: the object's reference
//      count. The class also provides the default behaviour for the three
//      methods present in the IEComNonDelegatingUnknown interface.
//
//  #Distinction between IEComUnknown and IEComNonDelegatingUnknown:#
//      Every ECOM object inherits from both interfaces. IEComUnknown is the
//      base interface of all the object's inherited interfaces and
//      IEComNonDelegatingUnknown is the base interface of CEComUnknown also
//      inherited by the object's.
//
//      IEComUnknown is the interface that is ALWAYS used by ECOM users. It is
//      always implemented the same way. On the other hand,
//      IEComNonDelegatingUnknown methods are NEVER used or invoked.
//      IEComNonDelegatingUnknown is only a placeholder for ECOM implementers
//      to override the specific IEComUnknown behaviour of their object.
//
//      Looking carefully at both interfaces reveals that they are almost
//      identical. In fact, besides the names that differ, both interfaces have
//      the same number of methods in the same order, each with the same number
//      of arguments also in the same order. It has been voluntarily made this
//      way to address aggregation more easily for ECOM implementers and make
//      it transparent to ECOM users. When no aggregation is used, the ECOM
//      abstract creation mechanism has the IEComUnknown interface point on the
//      IEComNonDelegatingUnknown of the CEComUnknown class from which an ECOM
//      inherits. Alternatively, when aggregation is used, the creation
//      mechanism has the IEComUnknown interface point to the
//      IEComNonDelegatingUnknown of the CEComUnknown inherited by the outer
//      component. This mechanism is in place to address one of the QueryIf
//      rules: "You always get the same IEComUnknown".
//
//      <B>Any ECOM that inherits this class MUST:</B>
//      - Implement the three IEComUnknown methods. It MUST always be made using
//        the macro MX_DECLARE_IECOMUNKNOWN.
//      - Override at least NonDelegatingQueryIf to add the possibility to query
//        for its supported interface(s). NonDelegatingAddIfRef and
//        NonDelegatingReleaseIfRef can also be overridden if their default
//        behaviour needs to be augmented.
//
//  See Also:
//      CEComDelegatingUnknown, IEComUnknown, IEComNonDelegatingUnknown
//
//==EDOC========================================================================
class CEComUnknown :  public IEComNonDelegatingUnknown
{
// Friends Declaration
//-------------------------


// Published Interface
//-------------------------
public:
    // << Constructors / Destructors >>
    //--------------------------------------------

    // Summary:
    //  Constuctor.
    //--------------
    CEComUnknown(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CEComUnknown();

    // << Stereotype >>
    //------------------

    // Summary:
    //  Initializes the instance.
    //-----------------------------
    virtual mxt_result InitializeInstance();

//M5T_INTERNAL_USE_BEGIN
    //--------------------------------------------
    //-- IEComNonDelegatingUnknown interface
    //--------------------------------------------
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);
    virtual unsigned int NonDelegatingAddIfRef();
    virtual unsigned int NonDelegatingReleaseIfRef();

// Hidden Methods
//-------------------------
private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    CEComUnknown(const CEComUnknown& from);
    CEComUnknown& operator=(const CEComUnknown& from);

// Hidden Data Members
//-------------------------
protected:
    // Should be called internally only.
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);
    unsigned int GetReferenceCount() const;

    //==========================================================================
    //==
    //==  GetOwnerIEComUnknown
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current IEComUnknown owner.
    //
    //  Returns:
    //      IEComUnknown pointer of the owner.
    //
    //  Description:
    //      This method is available to the object to query itself for its
    //      current IEComUnknown owner. The method has the protected attribute
    //      and therefore is accessible only to the object itself. This method
    //      is present to offer the same behaviour as the one in
    //      CEComDelegatingUnknown except that it always returns the current
    //      object IEComUnknown (this) because the object cannot be aggregated.
    //
    //  See also:
    //      MX_DECLARE_IECOMUNKNOWN, MX_DECLARE_DELEGATING_IECOMUNKNOWN,
    //      CEComUnknown, CEComDelegatingUnknown
    //
    //==========================================================================
    IEComUnknown* GetOwnerIEComUnknown() const
    {
        // We use pTmp to avoid a type-punning warning under GCC 4.1.2.
        // It is likely to be the same with all GCC 4.x
        // There is no impact on code size.
        CEComUnknown* pTmp = const_cast<CEComUnknown*>(this);
        return reinterpret_cast<IEComUnknown*>(static_cast<IEComNonDelegatingUnknown*>(pTmp));
    }

private:
    CAtomicValue<uint32_t> m_uReferenceCount;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // #ifndef MXG_CECOMUNKNOWN_H
