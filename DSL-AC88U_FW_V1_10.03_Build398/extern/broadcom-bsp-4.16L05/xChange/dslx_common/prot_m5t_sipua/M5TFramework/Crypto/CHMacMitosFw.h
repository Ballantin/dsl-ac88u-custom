//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CHMACMITOSFW_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CHMACMITOSFW_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CHash;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CHMacMitosFw
//========================================
//
// Description:
//   Class that manages the HMAC algorithm. This implementation is a version of
//   the code found in RFC4634 (ftp://ftp.rfc-editor.org/in-notes/rfc4634.txt)
//   used by written permission from the authors.
//
// Location:
//   Crypto/CHMacMitosFw.h
//
//==============================================================================
class CHMacMitosFw
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    //-- << Stereotype >>

//-- Hidden Methods
protected:
    // Summary:
    //  Default Constructor.
    CHMacMitosFw();

    // Summary:
    //  Destructor.
    virtual ~CHMacMitosFw();

    // Summary:
    //  Prepares the computing of a new HMAC message digest.
    mxt_result HMacReset(IN const uint8_t* puKey,
                         IN unsigned int uKeySize,
                         IN CHash* pInnerHash,
                         IN CHash* pOuterHash,
                         IN unsigned int uHashBlockSize,
                         IN uint8_t* puInnerPadding,
                         IN uint8_t* puOuterPadding);

    // Summary:
    //  Returns the computed HMAC message digest.
    mxt_result HMacResult(IN CHash* pInnerHash,
                          IN CHash* pOuterHash,
                          IN uint8_t* puInnerHashDigest,
                          OUT uint8_t* puDigest);

//-- Hidden Data Members
protected:
private:
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
inline
CHMacMitosFw::CHMacMitosFw()
{
}

inline
CHMacMitosFw::~CHMacMitosFw()
{
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CHMACMITOSFW_H
//M5T_INTERNAL_USE_END

