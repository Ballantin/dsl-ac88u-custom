//==============================================================================
//==============================================================================
//
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CTLSSESSIONMOCANASS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTLSSESSIONMOCANASS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_TLS_MOCANA_SS)

//-- Data Members

#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_TLS_CTLSSESSION_CLASSNAME
    #define MXD_TLS_CTLSSESSION_CLASSNAME CTlsSessionMocanaSs
#endif

//==============================================================================
//== Class: CTlsSession
//========================================
//
// Summary:
//   Class that manages the TLS session identifier.
//
// Description:
//   CTlsSession is the class that manages the TLS session identifier. It serves
//   as a container for the session ID and the master secret. The session ID is
//   used to re-establish a connection without going through the whole complete
//   handshaking.
//
// Location:
//   Tls/CTlsSession.h
//
//==============================================================================
class CTlsSessionMocanaSs
{
// Published Interface
public:
    // << Constructors / Destructors >>

    // Summary:
    //  Constructor.
    CTlsSessionMocanaSs();

    // Summary:
    //  Copy constructor. Pointer version.
    CTlsSessionMocanaSs(IN const CTlsSessionMocanaSs* pTlsSession);

    // Summary:
    //  Copy constructor. Reference version.
    CTlsSessionMocanaSs(IN const CTlsSessionMocanaSs& rTlsSession);

    // Summary:
    //  Destructor.
    virtual ~CTlsSessionMocanaSs();

    // Summary:
    //  Assignment operator.
    CTlsSessionMocanaSs& operator=(IN const CTlsSessionMocanaSs& rTlsSession);

    // Summary:
    //   Equality operator.
    bool operator==(IN const CBlob& rSessionId) const;

    // Summary:
    //   Inequality operator.
    bool operator!=(IN const CBlob& rSessionId) const;

    // Summary:
    //  Restores a TLS session ID from binary data.
    mxt_result Restore(IN CBlob* pblobId);

    // Summary:
    //  Restores a TLS session master se from binary data.
    mxt_result RestoreMasterSecret(IN CBlob* pblobMasterSecret);

    // Summary:
    //  Stores a TLS session ID in binary data.
    mxt_result Store(OUT CBlob* pblobId) const;

    // Summary:
    //  Stores a TLS session master secret in binary data.
    mxt_result StoreMasterSecret(OUT CBlob* pblobMasterSecret) const;

    // Summary:
    //   Returns the session's ID.
    mxt_result GetId(OUT CBlob* pblobId) const;

    // Summary:
    //   Returns a pointer to the session master secret.
    uint8_t* GetMasterSecret();

// Hidden Data Members
protected:

    // The session ID.
    CBlob m_blobSessionId;

    // The master secret.
    CBlob m_blobMasterSecret;
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_TLS_MOCANA_SS)

//M5T_INTERNAL_USE_END

#endif // MXG_CTLSSESSIONMOCANASS_H

