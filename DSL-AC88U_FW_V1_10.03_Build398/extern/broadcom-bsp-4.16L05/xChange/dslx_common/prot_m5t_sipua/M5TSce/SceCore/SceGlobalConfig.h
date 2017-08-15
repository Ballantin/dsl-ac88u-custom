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
#ifndef MXG_SCEGLOBALCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCEGLOBALCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members.
#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif
//M5T_INTERNAL_USE_END

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//====  STATIC PUBLIC METHODS  =================================================
//==============================================================================

//  Summary:
//      Gets the requested interface from the Sip Client Engine Global
//      Configuration.
mxt_result SceGetGlobalConfig(IN mxt_iid iidRequested,
                              OUT void** ppInterface);

//M5T_INTERNAL_USE_BEGIN
//  Summary:
//      Creates the SIP Client Engine Global Configuration.
mxt_result InitializeSceGlobalConfig();

//  Summary:
//      Releases the SIP Client Engine Global Configuration.
void FinalizeSceGlobalConfig();
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SceGetGlobalConfig
//==
//==============================================================================
//<GROUP TID_PACKAGE_SCECORE_FUNCTIONS>
//
//  Summary:
//     Gets the requested interface from the Sip Client Engine Global
//     Configuration.
//
//  Parameters:
//      ppInterface:
//          The pointer of the interface pointer variable to store the
//          requested interface.
//
//  Returns:
//      - resS_OK: The interface has been found and returned.
//      - Other: An error occured or the interface does not exist.
//
//  Description:
//      This is a templated version of the original SceGetGlobalConfig method.
//      It is designed to ease code readability.
//
//==============================================================================
template<class _Type> mxt_result SceGetGlobalConfig(OUT _Type** ppInterface)
{
    // Has to be inlined here because Visual C++ 6.0 does not like out of class
    // inlined specialized template methods.
    if (ppInterface != NULL)
    {
        return SceGetGlobalConfig((*ppInterface)->GetIID(),
                                   reinterpret_cast<void**>(ppInterface));
    }
    return resFE_INVALID_ARGUMENT;
}



MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SCEGLOBALCONFIG_H

