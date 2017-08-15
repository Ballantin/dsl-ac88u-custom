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
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CECOMDELEGATINGUNKNOWN_H
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

// Data Member
#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

// Interface Realized & Parent
#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
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

//==SDOC========================================================================
//==
//==  class CEComDelegatingUnknown
//==
//==============================================================================
//<GROUP ECOM_CLASSES>
//
//  Summary:
//      Base class to be used when an ECOM class can be aggregated.
//
//  Description:
//      ECOM classes can be described from two points of view: the ECOM
//      implementer's view and the ECOM user's view. Inheriting
//      CEComDelegatingUnknown is related to the implementer's view.
//
//      This class MUST be used as the base class of every ECOM class when it
//      is known that they can be aggregated. In addition to the behaviour
//      provided by the CEComUnknown base class, this class holds another
//      important information about the ECOM object's state: a pointer to the
//      ECOM object's real IEComUnknown interface. It makes aggregation optional
//      and transparent.
//
//      <B>Any ECOM that inherits this class MUST:</B>
//      - Implement the three IEComUnknown methods. It MUST always be made using
//        the macro MX_DECLARE_DELEGATING_IECOMUNKNOWN.
//      - Override at least NonDelegatingQueryIf to add the possibility to query
//        for its supported interface(s). NonDelegatingAddIfRef and
//        NonDelegatingReleaseIfRef can also be overridden if their default
//        behaviour needs to be augmented.
//
//  See Also:
//      CEComUnknown, IEComUnknown, IEComNonDelegatingUnknown
//
//==EDOC========================================================================
class CEComDelegatingUnknown :  public CEComUnknown
{
// Friends Declaration
//-------------------------

// Published Interface
//-------------------------
public:
    // << Constructors / Destructors >>
    //--------------------------------------------

    //==========================================================================
    //==
    //==  CEComDelegatingUnknown
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor
    //
    //  Parameters:
    //      pOuterIEComUnknown:
    //          A pointer to the ECOM object aggregator. NULL if the ECOM
    //          object is not being aggregated.
    //
    //  Description:
    //      Constructor.
    //
    //==========================================================================
    CEComDelegatingUnknown(IN IEComUnknown* pOuterIEComUnknown = NULL):
        CEComUnknown(),
        m_pCurrentIEComUnknown(NULL)
    {
        if (NULL == pOuterIEComUnknown)
        {
            // The ECOM object is not being aggregated, use the non delegating
            // unknown. This process require a double cast. The inner cast is
            // type-safe because the "this" is casted into a type it inherit.
            // Contrarely the outer cast is not type-safe but in that case, it has
            // no impact because the IEComNonDelegatingUnknown has the exact same
            // list of methods and arguments as IEComUnknown. In fact, that is
            // exactly what we want to do. That way, a client never know about
            // aggregation and always queries interfaces the same way.

            // We use pTmp to avoid a type-punning warning under GCC 4.1.2.
            // It is likely to be the same with all GCC 4.x
            // There is no impact on code size.
            IEComNonDelegatingUnknown* pTmp = static_cast<IEComNonDelegatingUnknown*>(this);
            m_pCurrentIEComUnknown = reinterpret_cast<IEComUnknown*>(pTmp);
        }
        else
        {
            // The ECOM object is being aggregated, use outer unknown.
            m_pCurrentIEComUnknown = pOuterIEComUnknown;
        }
    }

    //==========================================================================
    //==
    //==  ~CEComDelegatingUnknown
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor
    //
    //  Description:
    //      Destructor.
    //
    //==========================================================================
    virtual ~CEComDelegatingUnknown()
    {
    }

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//-------------------------
protected:
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
    //      This method is available to the ECOM object to query itself for its
    //      current IEComUnknown owner. The method has the protected attribute
    //      and therefore is accessible only to the object itself. This method
    //      exists to address aggregation transparently without breaking QueryIf
    //      rules, e.g., "You always get the same IEComUnknown".
    //
    //  See also:
    //      MX_DECLARE_IECOMUNKNOWN, MX_DECLARE_DELEGATING_IECOMUNKNOWN,
    //      CEComUnknown, CEComDelegatingUnknown
    //
    //==========================================================================
    IEComUnknown* GetOwnerIEComUnknown() const
    {
        return m_pCurrentIEComUnknown;
    }

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    CEComDelegatingUnknown(const CEComDelegatingUnknown& from);
    CEComDelegatingUnknown& operator=(const CEComDelegatingUnknown& from);

// Hidden Data Members
//-------------------------
protected:
private:
    IEComUnknown* m_pCurrentIEComUnknown;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // #ifndef MXG_CECOMDELEGATINGUNKNOWN_H
