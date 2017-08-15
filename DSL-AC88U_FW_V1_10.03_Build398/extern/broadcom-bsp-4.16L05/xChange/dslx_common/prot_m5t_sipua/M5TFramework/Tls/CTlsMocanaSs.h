//==============================================================================
//==============================================================================
//
//      Copyright(c) 2007 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CTLSMOCANASS_H
#define MXG_CTLSMOCANASS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_TLS_MOCANA_SS)

#ifndef MXD_PKI_MOCANA_SS
#error "MXD_PKI_MOCANA_SS MUST be defined in order to define MXD_TLS_MOCANA_SS"
#endif


MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_TLS_CTLS_CLASSNAME
    #define MXD_TLS_CTLS_CLASSNAME CTlsMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTlsMocanaSs
//========================================
//
// Description:
//   CTls is a class used only by CAsyncTlsSocket and CAsyncTlsServerSocket.
//   This is why all methods are private. No user shall ever have to directly
//   use this class.
//
// Location:
//   Tls/CTlsMocanaSs.h
//
// See Also:
//   CAsyncTlsSocket, CAsyncTlsServerSocket
//
//==============================================================================
class CTlsMocanaSs
{
// Published Interface.
public:

// Hidden Methods.
protected:
    // << Constructors / Destructors >>.

    // Summary:
    //   Constructor.
    CTlsMocanaSs();

    // Summary:
    //   Destructor.
    virtual ~CTlsMocanaSs();

// Hidden Data Members.
private:

};

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_TLS_MOCANA_SS)

#endif // MXG_CMOCANATLS_H
//M5T_INTERNAL_USE_END

