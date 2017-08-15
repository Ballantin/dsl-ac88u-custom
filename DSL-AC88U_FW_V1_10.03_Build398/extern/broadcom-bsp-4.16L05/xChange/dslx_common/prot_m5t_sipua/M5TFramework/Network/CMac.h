//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation
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
#ifndef MXG_CMAC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMAC_H
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
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

// Data Member
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

// Interface Realized & Parent

// Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)


// Forward Declarations Inside of the Namespace

//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================
//M5T_INTERNAL_USE_BEGIN
const char cDEFAULT_MAC_ADDR[] = "0090F8FFFFFF";
const uint8_t cMAC_LENGTH = 6;
const uint8_t cMAC_STR_LENGTH = cMAC_LENGTH * 2;
//M5T_INTERNAL_USE_END


//===========================================================================
//====  NEW TYPE DEFINITIONS  ===============================================
//===========================================================================

//==SDOC=====================================================================
//
//  class CMac
//
//===========================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//      This class stores and manages the Ethernet MAC address.
//
//  Description:
//      This class stores and manages the Ethernet MAC address.
//      NOTE: The MAC address is stored in the file named MAC.cfg
//
//  Published Interface:
//
//
//==EDOC=====================================================================
class CMac
{
//M5T_INTERNAL_USE_BEGIN
// Internal structures
//-------------------------

    // Holds the MAC address.
    struct stMac
    {
        uint8_t pEthMacAddr[cMAC_LENGTH];
    };
//M5T_INTERNAL_USE_END

// Published Interface
//-------------------------
public:
    // << Constructors / Destructors >>
    //--------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CMac();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CMac();

    // Copy constructor and assignment operator
    //------------------------------------------

    // Summary:
    //  Copy constructor.
    //--------------------
    CMac(const CMac& rhs);

    // Summary:
    //  Assignment operator.
    //-----------------------
    CMac& operator=(const CMac& rhs);

    // << accessors >>
    //--------------------------------------------

    // Summary:
    //  Sets the MAC address.
    //------------------------
    bool SetMac(IN const CString& rstrMac);

    // Summary:
    //  Sets the MAC address.
    //------------------------
    bool SetMac(IN const stMac& rstMac);

    // Summary:
    //  Sets the MAC address.
    //------------------------
    bool SetMac(IN const uint8_t* puMac);

    // Summary:
    //  Gets the MAC address.
    //------------------------
    bool GetMac(OUT CString& rstrMac) const;

    // Summary:
    //  Gets the MAC address.
    //------------------------
    bool GetMac(OUT stMac& rMac) const;

    // Summary:
    //  Gets the MAC address in string format.
    //-----------------------------------------
    const CString& GetMacStr() const;

    // Summary:
    //  Gets the MAC address in byte format.
    //---------------------------------------
    const uint8_t* GetMac() const;

    // Summary:
    //  Checks if the string holds a valid MAC address.
    //--------------------------------------------------
    bool IsValid(IN const CString& rstrMac) const;

    // Summary:
    //  Checks if the MAC address is valid.
    //--------------------------------------
    bool IsValid(IN const stMac& rMac) const;

    // Summary:
    //  Checks if the MAC address stored in this object is valid.
    //------------------------------------------------------------
    bool IsValid() const;

    // Summary:
    //  Sets the default value to this CMac object.
    //----------------------------------------------
    void SetWithDefaultMac();

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(const CMac& rMac) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(const CMac& rMac) const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//-------------------------
private:
    void ConvertMacStrToMacBytes(IN const CString& rstrMac, OUT stMac& rMac) const;
    void ConvertMacBytesToMacStr(IN const stMac& rMac, OUT CString& rstrMac) const;


// Hidden Data Members
//-------------------------
private:
    CString m_strMac;
//M5T_INTERNAL_USE_END
};


//===========================================================================
//====  INLINE FUNCTIONS  ===================================================
//===========================================================================

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    rMac:
//      Reference to the CMac to compare with.
//
//  Returns:
//      True if both CMac objects are equal, false otherwise.
//
//  Description:
//      Checks if both CMac objects are equal.
//
//==EDOC========================================================================
inline bool CMac::operator==(const CMac& rMac) const
{
    return (memcmp(GetMac(), rMac.GetMac(), cMAC_LENGTH) == 0);
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rMac:
//      Reference to the CMac to compare with.
//
//  Returns:
//      True if both CMac objects are different, false otherwise.
//
//  Description:
//      Checks if both CMac objects are different.
//
//==EDOC========================================================================
inline bool CMac::operator!=(const CMac& rMac) const
{
    return (memcmp(GetMac(), rMac.GetMac(), cMAC_LENGTH) != 0);
}

//==SDOC========================================================================
//==
//==  IsValid
//==
//==============================================================================
//
//  Returns:
//      True if the MAC address stored is valid, false otherwise.
//
//  Description:
//      Checks is the MAC address contained within this CMac object is valid.
//
//==EDOC========================================================================
inline bool CMac::IsValid() const
{
    static stMac mac;
    ConvertMacStrToMacBytes (m_strMac, mac);
    return IsValid (mac);
}
MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // #ifndef MXG_CMAC_H
