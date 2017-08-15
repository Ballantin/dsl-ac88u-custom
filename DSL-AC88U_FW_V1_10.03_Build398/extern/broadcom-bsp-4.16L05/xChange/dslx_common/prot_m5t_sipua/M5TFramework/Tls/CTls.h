//==============================================================================
//==============================================================================
//
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2010 Media5 Corporation ("Media5")
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

//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CTLS_H
#define MXG_CTLS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if !defined(MXD_TLS_NONE)

#if !defined(MXD_PKI_OPENSSL) && !defined(MXD_PKI_MOCANA_SS)
#error MXD_PKI_OPENSSL or MXD_PKI_MOCANA_SS MUST be defined in order to define MXD_TLS_OPENSSL or MXD_TLS_MOCANA
#endif

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

//-- Data Members
//-----------------
#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif // #ifndef MXG_CMUTEX_H

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#if defined(MXD_TLS_CTLS_INCLUDE)
    #include MXD_TLS_CTLS_INCLUDE
#else
    #error "No implementation defined for CTls!!!"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
enum ETlsIdentity
{
    eTLS_IDENTITY_CLIENT,
    eTLS_IDENTITY_SERVER,
    eTLS_IDENTITY_UNKNOWN
};
//M5T_INTERNAL_USE_END

//<GROUP TLS_ENUMS>
//
// Summary:
//     SSL/TLS protocol versions.
//
// Description:
//     SSL/TLS protocol versions supported by M5T Framwork.
enum ETlsVersion
{
    // Description:
    // SSL v3.0
    eTLS_VERSION_SSL_3_0,
    // Description:
    // TLS v1.0
    eTLS_VERSION_TLS_1_0
};

//==SDOC========================================================================
//== Class: CTls
//========================================
//<GROUP TLS_CLASSES>
//
// Summary:
//   This class performs initialization of the TLS engine.
//
// Description:
//   CTls is a class used only by CAsyncTlsSocket and CAsyncTlsServerSocket.
//   This is why all methods are private. No user shall ever have to directly
//   use this class.
//
// Location:
//   Tls/CTls.h
//
// See Also:
//   CAsyncTlsSocket, CAsyncTlsServerSocket
//
//==EDOC========================================================================
class CTls : public MXD_TLS_CTLS_CLASSNAME
{
// Published Interface
//---------------------
public:

    // Summary:
    //  Returns the unique instance of CTls.
    static CTls* Instance();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // << Constructors / Destructors >>
    //----------------------------------
    CTls();
    virtual ~CTls();

private:

    static mxt_result InitializeCTls();
    static void FinalizeCTls();

// Friends declarations
//----------------------
    friend mxt_result InitializeCTls();
    friend void FinalizeCTls();

// Hidden Data Members
//---------------------
protected:
private:

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_TLS_NONE)

#endif // MXG_CTLS_H
//M5T_INTERNAL_USE_END

