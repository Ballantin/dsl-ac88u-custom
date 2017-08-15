//==============================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CPKI_H
#define MXG_CPKI_H

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
#include "Config/FrameworkCfg.h" // MXD_PKI_NONE
#endif

#if !defined(MXD_PKI_NONE)

//-- Data Members
//-----------------
#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

#ifdef MXD_PKI_OPENSSL
#include "openssl/ssl.h"
#else
typedef unsigned int X509;
#endif

//-- Data Members
//-----------------
#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif // #ifndef MXG_CMUTEX_H

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CPki
//========================================
//<GROUP PKI_CLASSES>
//
// Description:
//   CPki is a class used only by the PKI folder classes. This is why all
//   methods are private. No user shall ever have to directly use this class.
//
// Location:
//   Pki/CPki.h
//
//==============================================================================
class CPki
{

// Published Interface
//---------------------
public:

    // Summary:
    //  Returns the unique instance of CPki
    static CPki* Instance();

    // Summary:
    //  Must be called by classes in the Pki folder to
    //  initialize the Pki stack.
    void Initialize();

    // Summary:
    //  Must be called by classes in the Pki folder to
    //  uninitialize the Pki stack.
    void Uninitialize();

    // Summary:
    //  Enters the concurrency protection domain.
    //  Please, take note that this method reserves the system wide crypto
    //  critical section. Exit from it quickly, using the method Exit.
    void Enter() const;

    // Summary:
    //  Exits the concurrency protection domain.
    void Exit() const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // << Constructors / Destructors >>
    //----------------------------------
    CPki();
    virtual ~CPki();

private:

    static mxt_result InitializeCPki();
    static void FinalizeCPki();

// Friends declarations
//----------------------
    friend mxt_result InitializeCPki();
    friend void FinalizeCPki();

// Hidden Data Members
//---------------------
private:

    // Summary:
    //  Contains the count of how many Initializes have been done for which
    //  no call to Uninitialize has been done.
    unsigned int m_uInitializationCount;

    // Summary:
    //  Protection against multiple concurrent accesses to internal variables.
    CMutex m_mutexConcurrencyProtection;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
void CPki::Enter() const
{
    CCrypto::Instance()->Enter();
}

inline
void CPki::Exit() const
{
    CCrypto::Instance()->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CPKI_H
//M5T_INTERNAL_USE_END

