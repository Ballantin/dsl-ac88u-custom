//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPTRANSPORTTOOLS_H
#define MXG_CSIPTRANSPORTTOOLS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CString;
class CMarshaler;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

// Contains the transport types supported by SIP.
// The values are bitmaskable except for eINVALID.
enum ESipTransport
{
    eINVALID = 0,
    eUDP = 1,
    eTCP = 2,
    eTLS = 4
};

//==============================================================================
//====  MARSHALING METHODS  ====================================================
//==============================================================================
CMarshaler& operator<<(IN CMarshaler& rMarshaler, IN ESipTransport eTransport);
CMarshaler& operator>>(IN CMarshaler& rMarshaler, IN ESipTransport& reTransport);

//==SDOC========================================================================
//== Class: CSipTransportTools
//========================================
//
// Description:
//   This class is a bundle of convenient tools for the SipTransport package.
//
// Location:
//   SipTransport/CSipTransportTools.h
//
//==EDOC========================================================================
class CSipTransportTools
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipTransportTools();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipTransportTools();

    // Summary:
    //  Converts the transport id from string to enum.
    //-------------------------------------------------
    static ESipTransport ConvertTransportId(IN const CString& rstrTransport);

    // Summary:
    //  Converts the transport id from enum to string.
    //-------------------------------------------------
    static const char* ConvertTransportId(IN const ESipTransport eTransport);

    // Summary:
    //  Determines whether or not the protocol passed as a parameter is 
    //  connection oriented.
    //----------------------------------------------------------------------
    static bool IsConnectionOriented(IN const ESipTransport eTransport);

    // Summary:
    //  Determines whether or not the protocol passed as a parameter is stream
    //  oriented.
    //-------------------------------------------------------------------------
    static bool IsStreaming(IN const ESipTransport eTransport);

    // Summary:
    //  Hashes the specified string.
    static unsigned int Hash(IN const char* pszKey,
                             IN unsigned int uSize);

    // Summary:
    //  Hashes the specified null terminated string.
    static unsigned int Hash(IN const char* pszKey);

//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipTransportTools(IN const CSipTransportTools& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipTransportTools& operator=(IN const CSipTransportTools& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

#if ((!defined(MXD_COMPILER_GNU_GCC)) || \
    (defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC >= MXD_COMPILER_GNU_GCC_3_0)))
//==SDOC========================================================================
//==
//==  IsConnectionOriented
//==
//==============================================================================
//
//  Parameters:
//    eTransport:
//      The transport ID.
//
//  Returns:
//      True if the transport is connection oriented, false otherwise.
//
//  Description:
//      Determines whether or not the transport passed as a parameter is 
//      connection oriented.
//
//==EDOC========================================================================
inline
bool CSipTransportTools::IsConnectionOriented(IN const ESipTransport eTransport)
{
    return (eTransport == eTCP || eTransport == eTLS);
}

//==SDOC========================================================================
//==
//==  IsStreaming
//==
//==============================================================================
//
//  Parameters:
//    eTransport:
//      The transport ID.
//
//  Returns:
//      True if the transport is stream oriented, false otherwise.
//
//  Description:
//      Determines whether or not the transport passed as a parameter is stream
//      oriented.
//
//==EDOC========================================================================
inline
bool CSipTransportTools::IsStreaming(IN const ESipTransport eTransport)
{
    return (eTransport == eTCP || eTransport == eTLS);
}
#endif


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPTRANSPORTTOOLS_H

