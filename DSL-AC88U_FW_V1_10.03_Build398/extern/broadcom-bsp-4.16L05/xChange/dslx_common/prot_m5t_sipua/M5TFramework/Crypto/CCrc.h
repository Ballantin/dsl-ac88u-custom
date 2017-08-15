//==============================================================================
//==============================================================================
//
//     Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation. ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without written prior approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CCRC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCRC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
class CBlob;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//==============================================================================
//== Class: CCrc
//==============================================================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Used to calculate a cyclic redundancy check (CRC) on an array of bytes.
//
// Description:
//   This class is used to calculate a cyclic redundancy check (CRC) on an array
//   of bytes.
//
//   The CRC check is initialized by a call to Begin. Begin takes multiple
//   types of CRC checks that can be made on the array(s). One or many byte
//   arrays are then passed to Update and the CRC is calculated for every
//   byte array received. Once all arrays have been passed, the complement (XOR)
//   of the CRC is returned by calling End. The complement is returned since
//   this is the value that must be appended to the byte array for FCS
//   verification.
//
//   CRC Verification:
//      To check if an array is correct, it must have the complement (XOR) of
//      its CRC appended to the byte array. Verification is done by the
//      algorithm on the array with the complemented bytes appended to the end.
//      The new CRC should give a specific value: 0xf0b8 for CRC16 CCITT and
//      0xdebb20e3 for CRC32 IEEE 802.3. No value for CRC32C has been found
//      in the document related to the algorithm. If the value of the new CRC
//      is different from these values, then there has been an error in the
//      transmission of data. For this implementation, the complement (XOR) of
//      the returned CRC must be compared since the complement is already
//      returned by End.
//
// Location:
//   Crypto/CCrc.h
//
//==============================================================================
class CCrc
{
//-- New types within class namespace
public:

    //<GROUP CRYPTO_ENUMS>
    //
    // Summary:
    // CRC types.
    //
    // Description:
    // Defines the possible types if CRC available.
    enum ECrcType
    {
        // Description:
        // 16 bits CRC CCITT.
        eCRC16,
        // Description:
        // 32 bits CRC IEEE 802.3.
        eCRC32,
        // Description:
        // 32 bits CRC Castagnoli.
        eCRC32C
    };

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CCrc();

    // Summary:
    //  Destructor.
    virtual ~CCrc();

    // Summary:
    //  Initializes the CRC calculation for data blocks.
    mxt_result Begin(IN ECrcType eCrcType);

    // Summary:
    //  Inserts a data block from which to calculate the CRC.
    mxt_result Update(IN const uint8_t* puData, unsigned int uDataSize);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    mxt_result Update(IN CBlob* pblobData);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    // Summary
    //  Returns the value of the current 16 bits CRC.
    mxt_result End(OUT uint16_t* puCrc16);

    // Summary
    //  Returns the value of the current 32 bits CRC.
    mxt_result End(OUT uint32_t* puCrc32);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CCrc(IN const CCrc& rCCrc);

    // Summary:
    //  Assignment Operator.
    CCrc& operator=(IN const CCrc& rCCrc);


//-- Hidden Data Members
protected:
private:
    ECrcType m_eCrcType;

    // This value can be used for both the 32 and the
    // 16 bits implementations. In the 16 bits
    // implementation, this value is binary AND with
    // 0x0000FFFF.
    uint32_t m_uCrc;

    bool m_bCrcBegun;

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CCRC_H
