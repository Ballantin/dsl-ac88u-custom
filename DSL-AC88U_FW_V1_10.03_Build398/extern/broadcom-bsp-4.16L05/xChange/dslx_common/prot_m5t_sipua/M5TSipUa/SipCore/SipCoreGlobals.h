//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_SIPCOREGLOBALS_H
#define MXG_SIPCOREGLOBALS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IEComUnknown;

//==============================================================================
//====  EXTERNAL PACKAGE DATA  =================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//====  METHOD PROTOTYPES  =====================================================
//==============================================================================

//==============================================================================
//====  INTERNAL PACKAGE DATA  =================================================
//==============================================================================

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

// Description:
//  The servicing thread used by most SipCore classes.
//-----------------------------------------------------
extern IEComUnknown* g_pCoreThread;

// Description:
//  The servicing thread used by server location services (DNS Resolver thread).
//------------------------------------------------------------------------------
extern IEComUnknown* g_pResolverThread;

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPCOREGLOBALS_H

