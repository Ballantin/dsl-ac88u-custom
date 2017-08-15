//==============================================================================
//==============================================================================
//
//      Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCEUACAPABILITIESCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEUACAPABILITIESCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members.
#ifndef MXG_CSCESIPFEATURESET_H
#include "SceCoreComponents/CSceSipFeatureSet.h"
#endif

//-- Parameters used by Value.

//-- Interface Realized and/or Parent.
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEUACAPABILITIESCONFIG_H
#include "SceCoreComponents/ISceUaCapabilitiesConfig.h"
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
//== Class: CSceUaCapabilitiesConfig
//========================================
//
// Description:
//  This class implements the ISceUaCapabilitiesConfig interface and is 
//  responsible to manage the UA capabilities for the user configuration. This
//  is an ECom component which is registered and aggregated  in a user 
//  configuration ECom components. As a result, any user configuration component
//  interfaces may be queried from this component's interface.
//
//  If this object is aggregated within a component, the aggregating component
//  will not get notified of capabilities change. In order for a component to 
//  be notified of any change in the capabilities, it must directly implement 
//  the ISceUaCapabilities interface.
//
// Location:
//  SceCoreComponents/CSceUaCapabilitiesConfig.h
//
//==============================================================================
class CSceUaCapabilitiesConfig : private CEComDelegatingUnknown,
                                 private ISceUaCapabilitiesConfig
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;


//-- Published Interface.
public:


//-- Hidden Methods.
private:
    //-- << Startup mechanism >>
    static mxt_result InitializeCSceUaCapabilitiesConfig();
    static void FinalizeCSceUaCapabilitiesConfig();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceUaCapabilitiesConfig(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceUaCapabilitiesConfig();

    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //   << ISceUaCapabilitiesConfig >>
    virtual mxt_result SetUaCapabilities(IN TO CSceSipFeatureSet* pCapabilities);
    virtual const CSceSipFeatureSet* GetUaCapabilities() const;

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceUaCapabilitiesConfig(IN const CSceUaCapabilitiesConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceUaCapabilitiesConfig& operator=(IN const CSceUaCapabilitiesConfig& rSrc);


//-- Hidden Data Members.
//------------------------
private:

    CSceSipFeatureSet* m_pUaCapabilities;
};

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEUACAPABILITIESCONFIG_H

