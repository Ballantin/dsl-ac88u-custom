//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_SERVICING_THREAD_ENABLE_SUPPORT
#endif

#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)
#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  STATIC MEMBERS INITIALIZATION  =========================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_CLSID(CServicingThread);
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)


#endif // MXD_SERVICING_THREAD_ENABLE_SUPPORT
