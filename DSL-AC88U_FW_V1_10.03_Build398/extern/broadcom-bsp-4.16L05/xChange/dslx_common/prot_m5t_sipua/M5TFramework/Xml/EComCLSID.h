//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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

// M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

// M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_XML_ENABLE_SUPPORT)
#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  STATIC MEMBERS INITIALIZATION  =========================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
// Generic name so only one parser and one writer can be enabled at the same time.
MX_DECLARE_ECOM_CLSID(CXmlParser);
MX_DECLARE_ECOM_CLSID(CXmlWriter);
MX_DECLARE_ECOM_CLSID(CXmlDocument);
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT)
