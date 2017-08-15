//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CUAIMPPPAGECONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPPAGECONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IUAIMPPPAGECONFIG_H
#include "SceUaImpp/IUaImppPageConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//== Class: CUaImppPageConfig
//========================================
//
// Description:
//  This class implements the user configuration options for Page Receiver.
//
// Location:
//   UaImpp/CUaImppPageConfig.h
//
// See Also:
//   IUaImppPageConfig
//
//==============================================================================
class CUaImppPageConfig : protected CEComDelegatingUnknown,
                          protected IUaImppPageConfig
{
//-- Friend Declarations.
    friend class CSceUaImppInitializer;

//-- Published Interface.
public:


//-- Hidden Methods.
protected:

    //-- << IUaImppPageConfig >>
    virtual void SetComponent(IN IEComUnknown* pComponent);
    virtual void GetComponent(OUT IEComUnknown** ppComponent);

    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaImppPageConfig();
    static void FinalizeCUaImppPageConfig();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaImppPageConfig(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaImppPageConfig();


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppPageConfig(IN const CUaImppPageConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppPageConfig& operator=(IN const CUaImppPageConfig& rSrc);


//-- Hidden Data Members.
protected:
private:
    // The Page Receiver component to use.
    IEComUnknown* m_pComponent;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CUAIMPPPAGECONFIG_H

//M5T_INTERNAL_USE_END
