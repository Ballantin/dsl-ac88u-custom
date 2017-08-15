//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_CTLSSESSIONOPENSSL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTLSSESSIONOPENSSL_H

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

#if defined(MXD_TLS_OPENSSL)

//-- Data Members
//-----------------
#ifndef MXG_CTLS_H
#include "Tls/CTls.h"
#endif // #ifndef MXG_CTLS_H
#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif // #ifndef MXG_CMUTEXN_H

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_TLS_CTLSSESSION_CLASSNAME
    #define MXD_TLS_CTLSSESSION_CLASSNAME CTlsSessionOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CTlsSession
//========================================
//
// Summary:
//   Class that manages the TLS session identifier.
//
// Description:
//   CTlsSession is the class that manages the TLS session identifier. It serves
//   as a container for the session identifier. The session identifier is used
//   to re-establish a connection without going through the whole complete
//   handshaking.
//
// Location:
//   Tls/CTlsSession.h
//
// See Also:
//
//==EDOC========================================================================
class CTlsSessionOpenSsl
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //----------------
    CTlsSessionOpenSsl();

    // Summary:
    //  Copy constructor. Pointer version.
    //-------------------------------------
    CTlsSessionOpenSsl(IN const CTlsSessionOpenSsl* pTlsSession);

    // Summary:
    //  Copy constructor. Reference version.
    //---------------------------------------
    CTlsSessionOpenSsl(IN const CTlsSessionOpenSsl& rTlsSession);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CTlsSessionOpenSsl();

    // Summary:
    //  Assignment operator.
    //-------------------------------------
    CTlsSessionOpenSsl& operator=(IN const CTlsSessionOpenSsl& rTlsSession);

    // Summary:
    //  Restores a TLS session from binary data.
    //-------------------------------------------
    mxt_result Restore(IN CBlob* pBlob);

    // Summary:
    //  Stores a TLS session in binary data.
    //---------------------------------------
    mxt_result Store(OUT CBlob* pBlob) const;

    // Summary:
    //   Returns the session's ID.
    //-----------------------------
    mxt_result GetId(OUT CBlob* pblobId) const;

    // Summary:
    //   Sets the SSL_SESSION.
    //-------------------------
    void SetOpenSslSession(IN SSL_SESSION* pSslSession);

// Hidden Methods
//----------------
protected:
private:


// Hidden Data Members
//---------------------
protected:

    mutable CMutex m_mutexConcurrencyProtection;

    SSL_SESSION* m_pSslSession;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//== CTlsSessionOpenSsl
//==
//==============================================================================
//
//  Description:
//      Constructor
//
//==============================================================================
inline
CTlsSessionOpenSsl::CTlsSessionOpenSsl()
: m_pSslSession(NULL)
{
}

//==============================================================================
//==
//== CTlsSessionOpenSsl
//==
//==============================================================================
//
//  Parameters:
//      pTlsSession:
//          Pointer to the session to copy.
//
//  Description:
//      Copy constructor
//
//==============================================================================
inline
CTlsSessionOpenSsl::CTlsSessionOpenSsl(IN const CTlsSessionOpenSsl* pTlsSession)
:   m_pSslSession(NULL)
{
    if (pTlsSession)
    {
        *this = *pTlsSession;
    }
}

//==============================================================================
//==
//== CTlsSessionOpenSsl
//==
//==============================================================================
//
//  Parameters:
//      rTlsSession:
//          Reference to the session to copy.
//
//  Description:
//      Copy constructor
//
//==============================================================================
inline
CTlsSessionOpenSsl::CTlsSessionOpenSsl(IN const CTlsSessionOpenSsl& rTlsSession)
:   m_pSslSession(NULL)
{
    *this = rTlsSession;
}

//==============================================================================
//==
//== ~CTlsSessionOpenSsl
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline
CTlsSessionOpenSsl::~CTlsSessionOpenSsl()
{
    if (m_pSslSession)
    {
        SSL_SESSION_free(m_pSslSession);
    }
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_TLS_OPENSSL)

//M5T_INTERNAL_USE_END

#endif // MXG_CTLSSESSIONOPENSSL_H

