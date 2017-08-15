//==SDOC========================================================================
//===========================================================================
//
//        Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CUUIDGENERATOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUUIDGENERATOR_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

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

#if !defined(MXD_CRYPTO_UUIDGENERATOR_NONE)

#if defined(MXD_CRYPTO_UUIDGENERATOR_INCLUDE)
    #include MXD_CRYPTO_UUIDGENERATOR_INCLUDE
#else
    #error "No implementation defined for CUuidGenerator!!!"
#endif

#ifndef MXG_CHASH_H
#include "Crypto/CHash.h"
#endif // #ifndef MXG_CHASH_H

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
class CBlob;
class CString;

//===========================================================================
//==
//==  class CUuidGenerator
//==
//===========================================================================
//<GROUP CRYPTO_CLASSES>
//
//  Summary:
//    Class used to generate a unique ID
//
//  Description:
//    Class used to generate a unique ID. The ID is generated from a buffer of
//    data specified, an internal counter and the machine ID string. The ID is
//    generated using the specified hash algorithm.
//
//===========================================================================
class CUuidGenerator : public MXD_CRYPTO_UUIDGENERATOR_CLASSNAME
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
    CUuidGenerator();

    // Summary:
    //  Destructor.
    //----------------
    ~CUuidGenerator();

    // Summary:
    //  Generate a unique identifier.
    //--------------------------------
    void Generate(IN  CHash::EAlgorithm eAlgorithm,
                  IN  const CString&    rstrData,
                  IN  uint32_t          uMaxLength,
                  OUT CString&          rstrUuid) const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
private:
    //Deactivated
    CUuidGenerator(IN const CUuidGenerator& from);
    const CUuidGenerator& operator=(IN const CUuidGenerator& from);
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
CUuidGenerator::CUuidGenerator()
 :   MXD_CRYPTO_UUIDGENERATOR_CLASSNAME()
{
}

inline
CUuidGenerator::~CUuidGenerator()
{
}

inline
void CUuidGenerator::SetMachineId(IN const CString& rstrId)
{
    MXD_CRYPTO_UUIDGENERATOR_CLASSNAME::SetMachineId(rstrId);
}

inline
void CUuidGenerator::Generate(IN  CHash::EAlgorithm eAlgorithm,
                              IN  const CString&    rstrData,
                              IN  uint32_t          uMaxLength,
                              OUT CString&          rstrUuid) const
{
    MXD_CRYPTO_UUIDGENERATOR_CLASSNAME::Generate(eAlgorithm, rstrData, uMaxLength, rstrUuid);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_UUIDGENERATOR_NONE)

#endif // #ifndef MXG_CUUIDGENERATOR_H
