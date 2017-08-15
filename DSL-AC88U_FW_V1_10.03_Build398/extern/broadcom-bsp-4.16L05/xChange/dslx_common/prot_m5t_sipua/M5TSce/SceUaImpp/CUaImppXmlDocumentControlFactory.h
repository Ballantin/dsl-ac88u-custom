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
#ifndef MXG_CUAIMPPXMLDOCUMENTCONTROLFACTORY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPXMLDOCUMENTCONTROLFACTORY_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCEUAIMPPCFG_H
#include "Config/SceUaImppCfg.h"
#endif

#if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.

//==============================================================================
//== Class: CUaImppXmlDocumentControlFactory
//==============================================================================
//
// Description:
//   This class creates and sets the different Xml Document Control instances.
//
// Location:
//   SceUaImpp/CUaImppXmlDocumentControlFactory.h
//
//==============================================================================
class CUaImppXmlDocumentControlFactory
{
//-- Friend Declarations.
    friend class CSceUaImppInitializer;

//-- Published Interface.
public:

//-- Hidden Methods.
protected:
private:

    //-- << Initialization mechanism >>

    static mxt_result InitializeCUaImppXmlDocumentControlFactory();
    static void FinalizeCUaImppXmlDocumentControlFactory();

    //-- << Ecom mechanism >>

    static mxt_result CreateGenericInstance(IN IEComUnknown* pOuterIEComUnknown,
                                            OUT CEComUnknown** ppCEComUnknown);
    static mxt_result CreatePidfInstance(IN IEComUnknown* pOuterIEComUnknown,
                                         OUT CEComUnknown** ppCEComUnknown);
    static mxt_result CreatePresenceAuthorizationInstance(IN IEComUnknown* pOuterIEComUnknown,
                                                          OUT CEComUnknown** ppCEComUnknown);
    static mxt_result CreateResourceListsInstance(IN IEComUnknown* pOuterIEComUnknown,
                                                  OUT CEComUnknown** ppCEComUnknown);
    static mxt_result CreateRlsServicesInstance(IN IEComUnknown* pOuterIEComUnknown,
                                                OUT CEComUnknown** ppCEComUnknown);

//-- Hidden Data Members.
protected:
private:

};


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CUAIMPPXMLDOCUMENTCONTROLFACTORY_H

