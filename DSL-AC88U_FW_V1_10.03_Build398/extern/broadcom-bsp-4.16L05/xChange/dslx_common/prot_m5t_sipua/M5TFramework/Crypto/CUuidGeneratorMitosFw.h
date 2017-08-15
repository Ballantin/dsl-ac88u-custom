//==============================================================================
//===========================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CUUIDGENERATORMITOSFWMITOSFW_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUUIDGENERATORMITOSFWMITOSFW_H

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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

#if defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW)

#ifndef MXG_CHASH_H
#include "Crypto/CHash.h"
#endif // #ifndef MXG_CHASH_H

// Data Member
#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

// Interface Realized & Parent

//==============================================================================
//====  MEDIATRIX NAME SPACE  ==================================================
//==============================================================================
MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
class CBlob;
class CString;

//==============================================================================
//====  DEFINES  ===============================================================
//==============================================================================
const uint32_t uMAX_MACHINE_ID_LENGTH    = 50;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_UUIDGENERATOR_CLASSNAME
    #define MXD_CRYPTO_UUIDGENERATOR_CLASSNAME CUuidGeneratorMitosFw
#endif

//==============================================================================
//==
//==  class CUuidGeneratorMitosFw
//==
//==============================================================================
//
//  Summary:
//    Class used to generate a unique ID
//
//  Description:
//    Class used to generate a unique ID. The ID is generated from a buffer of
//    data specified, an internal counter and the machine ID string. The ID is
//    generated using the specified hash algorithm.
//
//==============================================================================
class CUuidGeneratorMitosFw
{

// Published Interface
//-------------------------
public:

    // Summary:
    //  Sets the machine ID.
    //-----------------------
    static void SetMachineId(IN const CString& rstrId);

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //----------------
    CUuidGeneratorMitosFw();

    // Summary:
    //  Destructor.
    //----------------
    ~CUuidGeneratorMitosFw();

    // Summary:
    //  Generate a unique identifier.
    //--------------------------------
    void Generate(IN  CHash::EAlgorithm eAlgorithm,
                  IN  const CString&    rstrData,
                  IN  uint32_t          uMaxLength,
                  OUT CString&          rstrUuid) const;

// Hidden Methods
//----------------
private:
    //Deactivated
    CUuidGeneratorMitosFw(IN const CUuidGeneratorMitosFw& from);
    const CUuidGeneratorMitosFw& operator=(IN const CUuidGeneratorMitosFw& from);

    void Hash(IN  CHash::EAlgorithm eAlgorithm,
              IN  const CString&    rstrData,
              OUT CBlob&            rHash) const;
    void ConvertToHexAscii(IN  const CBlob& rhash,
                           OUT CString&     rstrHash) const;
    void Cut(IN  const CString& rstrData,
             IN  uint32_t       uNewLength,
             OUT CString&       rstrResult) const;

    static mxt_result InitializeCUuidGeneratorMitosFw();
    static void FinalizeCUuidGeneratorMitosFw();

// Friends declarations
//----------------------
    friend mxt_result InitializeCUuidGeneratorMitosFw();
    friend void FinalizeCUuidGeneratorMitosFw();

// Hidden Data Members
//--------------------
protected:
private:
    static char     ms_szMachineId[uMAX_MACHINE_ID_LENGTH];
    static uint32_t ms_uCounter;
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW)

//M5T_INTERNAL_USE_END
#endif // #ifndef MXG_CUUIDGENERATORMITOSFWMITOSFW_H


