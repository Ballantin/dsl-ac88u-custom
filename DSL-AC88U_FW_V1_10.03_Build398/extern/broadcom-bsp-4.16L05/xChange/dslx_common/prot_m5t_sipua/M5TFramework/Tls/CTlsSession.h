//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CTLSSESSION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTLSSESSION_H
//M5T_INTERNAL_USE_END

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
#if defined(MXD_TLS_CTLSSESSION_INCLUDE)
    #include MXD_TLS_CTLSSESSION_INCLUDE
#else
    #error "No implementation defined for CTlsSession!!!"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CTlsSession
//========================================
//<GROUP TLS_CLASSES>
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
class CTlsSession : public MXD_TLS_CTLSSESSION_CLASSNAME
{
//M5T_INTERNAL_USE_BEGIN
    friend class CAsyncTlsSocket;
//M5T_INTERNAL_USE_END

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //----------------
    CTlsSession();

    // Summary:
    //  Copy constructor. Pointer version.
    //-------------------------------------
    CTlsSession(IN const CTlsSession* pTlsSession);

    // Summary:
    //  Copy constructor. Reference version.
    //---------------------------------------
    CTlsSession(IN const CTlsSession& rTlsSession);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CTlsSession();

    // Summary:
    //  Assignment operator.
    //-------------------------------------
    CTlsSession& operator=(IN const CTlsSession& rTlsSession);

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

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
private:

// Hidden Data Members
//---------------------
protected:

    mutable CMutex m_mutexConcurrencyProtection;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//== CTlsSession
//==
//==============================================================================
//
//  Description:
//      Constructor
//
//==============================================================================
inline
CTlsSession::CTlsSession()
:   MXD_TLS_CTLSSESSION_CLASSNAME()
{
}

//==============================================================================
//==
//== CTlsSession
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
CTlsSession::CTlsSession(IN const CTlsSession* pTlsSession)
:   MXD_TLS_CTLSSESSION_CLASSNAME(pTlsSession)
{
    if (pTlsSession)
    {
        *this = *pTlsSession;
    }
}

//==============================================================================
//==
//== CTlsSession
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
CTlsSession::CTlsSession(IN const CTlsSession& rTlsSession)
:   MXD_TLS_CTLSSESSION_CLASSNAME(rTlsSession)
{
    *this = rTlsSession;
}

//==============================================================================
//==
//== ~CTlsSession
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline
CTlsSession::~CTlsSession()
{
}


//==============================================================================
//==
//== Restore
//==
//==============================================================================
//
//  Parameters:
//    pBlob:
//      Pointer to a blob containing the session information.
//
//  Returns:
//      - resS_OK
//      - resFE_FAIL
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Restores the TLS session information from a blob.
//
//==============================================================================
inline mxt_result CTlsSession::Restore(IN CBlob* pBlob)
{
    return MXD_TLS_CTLSSESSION_CLASSNAME::Restore(pBlob);
}

//==============================================================================
//==
//== Store
//==
//==============================================================================
//
//  Parameters:
//    pBlob:
//      Pointer to a blob to contain the session information.
//
//  Returns:
//      - resS_OK
//      - resFE_FAIL
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Stores the TLS session information to a blob.
//
//==============================================================================
inline mxt_result CTlsSession::Store(OUT CBlob* pBlob) const
{
    return MXD_TLS_CTLSSESSION_CLASSNAME::Store(pBlob);
}

//==============================================================================
//==
//==  GetId
//==
//==============================================================================
//
//  Parameters:
//      pblobId:
//       The session ID.
//
//  Returns:
//      resS_OK:
//          pblobId holds a valid session ID.
//
//      resFE_INVALID_ARGUMENT:
//          pblobId is NULL.
//
//      resFE_INVALID_STATE:
//          No SSL session has been set.
//
//  Description:
//      Returns the session's ID.
//
//==============================================================================
inline mxt_result CTlsSession::GetId(OUT CBlob* pblobId) const
{
    return MXD_TLS_CTLSSESSION_CLASSNAME::GetId(OUT pblobId);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_TLS_NONE)

#endif // MXG_CTLSSESSION_H

