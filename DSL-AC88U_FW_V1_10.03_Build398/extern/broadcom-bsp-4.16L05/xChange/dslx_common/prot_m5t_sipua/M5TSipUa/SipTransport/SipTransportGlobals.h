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
//   any form whatsoever without written prior approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_SIPTRANSPORTGLOBALS_H
#define MXG_SIPTRANSPORTGLOBALS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SIP Stack Global Configuration
//-----------------------------------
#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_TLS
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
class CBlob;
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

//  The servicing thread used by all SipTransport classes.
//---------------------------------------------------------
extern IEComUnknown* g_pTransportThread;

// The shared principal buffer for receiving and sending all data.
//-----------------------------------------------------------------
extern CBlob* g_pPrincipalBuffer;

static const char* const gs_apszSOCKET_TYPE_SIP = "SIP";

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPTRANSPORTGLOBALS_H

