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
#ifndef MXG_SCEUAIMPPGLOBALS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCEUAIMPPGLOBALS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Vector that will be used to hold pointer to every char* defined in PidfData
// and UaImppXmlDictionnary.
extern CVector<const char*>* g_vpszSceXmlDictionnary;

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SCEUAIMPPGLOBALS_H

//M5T_INTERNAL_USE_END
