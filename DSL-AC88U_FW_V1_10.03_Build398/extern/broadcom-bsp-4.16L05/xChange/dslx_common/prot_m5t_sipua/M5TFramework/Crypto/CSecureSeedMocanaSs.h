//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSECURESEEDMOCANASS_H
#define MXG_CSECURESEEDMOCANASS_H

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

#if !defined(MXD_CRYPTO_SHA1_NONE) && defined(MXD_CRYPTO_SECURESEED_MOCANA_SS)

//-- Data Members
//-----------------
#ifndef MXG_CSHA1_H
    #include "Crypto/CSha1.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_SECURESEED_CLASSNAME
    #define MXD_CRYPTO_SECURESEED_CLASSNAME CSecureSeedMocanaSs
#endif

//==SDOC========================================================================
//== Class: CSecureSeed
//========================================
//
// Description:
//   Class used to generate random bytes to seed a pseudo-random generator. Once
//   generated, these bytes are hashed by using Sha1 to give a 20 byte seed.
//
//==EDOC========================================================================
class CSecureSeedMocanaSs
{
//-- Published Interface
//------------------------
public:

    // Summary
    //  Generates a seed.
    //--------------------
    static mxt_result GenerateSeed(OUT uint8_t* puSeedValue);

// Hidden Methods
//-----------------
private:

    static mxt_result InitializeCSecureSeedMocanaSs();
    static void FinalizeCSecureSeedMocanaSs();

// Friends declarations
//----------------------
    friend mxt_result InitializeCSecureSeedMocanaSs();
    friend void FinalizeCSecureSeedMocanaSs();

protected:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Constructor.
    //-----------------------
    CSecureSeedMocanaSs();

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSecureSeedMocanaSs();

//-- Hidden Data Members
//------------------------
protected:
private:

    static unsigned int ms_uCounter;
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SECURESEED_MOCANA_SS)

#endif //-- #ifndef MXG_CSECURESEEDMOCANASS_H
//M5T_INTERNAL_USE_END

