//==============================================================================
//==============================================================================
//
//      Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCEGLOBALCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEGLOBALCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members.
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Interface Realized and/or Parent.
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEGLOBALCONFIG_H
#include "SceCore/ISceGlobalConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceGlobalConfig
//==============================================================================
//
// Summary:
//  This class implements the ISceGlobalConfig and is responsible to allocate
//  other registered global configuration components.
//
// Description:
//  This class implements the ISceGlobalConfig and is responsible to
//  allocate other registered global configuration components. This is an ECom
//  component which aggregates registered global configuration ECom components.
//  As a result, the global configuration component interfaces may be queried
//  from a single place using SceGlobalConfig::SceGetGlobalConfig().
//
// Location:
//  SceCoreComponents/CSceGlobalConfig.h
//
// See Also:
//  SceGlobalConfig::SceGetGlobalConfig
//
//==============================================================================
class CSceGlobalConfig : private CEComDelegatingUnknown,
                         private ISceGlobalConfig
{
//-- Friend Declarations.
    friend class CSceCoreInitializer;

//-- Published Interface.
public:

//-- Hidden Methods.
private:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSceGlobalConfig();
    static void FinalizeCSceGlobalConfig();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceGlobalConfig(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceGlobalConfig();

    //-- << CEComDelegatingUnknown >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual mxt_result InitializeInstance();
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //   << ISceGlobalConfig >>

    virtual mxt_result RegisterGlobalConfigObject(IN mxt_clsid classId);
    virtual void SetLocalAddressType(IN ELocalAddressType eLocalAddressType);
    virtual ELocalAddressType GetLocalAddressType() const;

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceGlobalConfig(IN const CSceGlobalConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceGlobalConfig& operator=(IN const CSceGlobalConfig& rSrc);


//-- Hidden Data Members.
private:

    // The vector of global config objects.
    CVector<IEComUnknown*>* m_pvecpGlobalConfigObject;

    // The representation of the local address.
    ISceGlobalConfig::ELocalAddressType m_eLocalAddressRepresentation;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEGLOBALCONFIG_H

