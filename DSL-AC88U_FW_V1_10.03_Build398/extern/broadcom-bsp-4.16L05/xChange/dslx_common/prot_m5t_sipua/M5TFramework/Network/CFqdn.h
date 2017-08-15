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
//   any form whatsoever without written prior approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CFQDN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CFQDN_H
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
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

//-- Data Members
//-----------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CMARSHALER_H
#include "Cap/CMarshaler.h"
#endif

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

//==SDOC========================================================================
//== Class: CFqdn
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//   This class is a simple container for FQDN/port number association.
//
// Description:
//   This class is a simple container for FQDN/port number association. The
//   FQDN member is maintained as a CString while the port number is an integer.
//   This class is here for reutilization purposes. It offers a very simple
//   interface with a default/copy/initializer constructor. As well, if offers
//   methods that simply provide access to data members using set and get.
//   It also provides some basic operators like =, != and ==.
//
// Location:
//   Network/CFqdn.h
//
//==EDOC========================================================================
class CFqdn
{
//M5T_INTERNAL_USE_BEGIN
//-- Friend Declarations
//------------------------
    friend CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CFqdn& rData);
    friend CMarshaler& operator>> (IN CMarshaler& rMarshaler, IN CFqdn& rData);
//M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //---------------
    CFqdn();

    // Summary:
    //  Constructor.
    //---------------
    CFqdn(IN const CString& rstrFqdn, IN const uint16_t uPort = 0);

    // Summary:
    //  Copy constructor.
    //---------------
    CFqdn(IN const CFqdn& rSrc);

    // Summary:
    //  Destructor.
    //---------------
    ~CFqdn();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CFqdn& operator=(IN const CFqdn& rSrc);

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CFqdn& rSrc) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CFqdn& rSrc) const;

    // Summary:
    //  Gets the FQDN name.
    //----------------------
    CString GetFqdn() const;

    // Summary:
    //  Gets the FQDN port.
    //----------------------
    uint16_t GetPort() const;

    // Summary:
    //  Resets the FQDN.
    //-------------------
    void Reset();

    // Summary:
    //  Sets the FQDN name.
    //----------------------
    void SetFqdn(IN const CString& rstrFqdn);

    // Summary:
    //  Sets the FQDN port.
    //----------------------
    void SetPort(IN const uint16_t uPort);

    // Data members.

    // CString containing the FQDN name.
    CString m_strFqdn;
    // The FQDN port number.
    uint16_t m_uPort;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//== CAsyncUdpSocket
//==
//==============================================================================
//
//  Description:
//      Constructor
//
//==============================================================================
inline
CFqdn::CFqdn()
: m_strFqdn(),
  m_uPort(0)
{
}

//==============================================================================
//==
//== CFqdn
//==
//==============================================================================
//
//  Parameters:
//      rstrFqdn:
//          Reference to a CStrng containing the FQDN name.
//
//      uPort:
//          Port number of the FQDN.
//
//  Description:
//      Constructor. Builds the fully qualified domain name using the name and
//      port number provided.
//
//==============================================================================
inline
CFqdn::CFqdn(IN const CString& rstrFqdn, IN const uint16_t uPort)
: m_strFqdn(rstrFqdn),
  m_uPort(uPort)
{
}

//==============================================================================
//==
//== CFqdn
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          Reference to a CFqdn to copy.
//
//  Description:
//      Copy constructor
//
//==============================================================================
inline
CFqdn::CFqdn(IN const CFqdn& rSrc)
: m_strFqdn(rSrc.m_strFqdn),
  m_uPort(rSrc.m_uPort)
{
}

//==============================================================================
//==
//== ~CFqdn
//==
//==============================================================================
//
//  Description:
//      Destructor
//
//==============================================================================
inline
CFqdn::~CFqdn()
{
}

//==============================================================================
//==
//== operator=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          A reference to the CFqdn to assign.
//
//  Returns:
//      A reference to the assigned CFqdn.
//
//  Description:
//      Assigns the right hand CFqdn to the left hand one.
//
//==============================================================================
inline
CFqdn& CFqdn::operator=(IN const CFqdn& rSrc)
{
    if (&rSrc != this)
    {
        m_strFqdn = rSrc.m_strFqdn;
        m_uPort = rSrc.m_uPort;
    }
    return *this;
}

//==============================================================================
//==
//== operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          A reference to the CFqdn to compare with.
//
//  Returns:
//      True if both CFqdns are different, false otherwise.
//
//  Description:
//      Compares the two CFqdns to see if they are different.
//
//==============================================================================
inline
bool CFqdn::operator!=(IN const CFqdn& rSrc) const
{
    return !(*this == rSrc);
}

//==============================================================================
//==
//== operator==
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          A reference to the CFqdn to compare with.
//
//  Returns:
//      True if both CFqdns are equal, false otherwise.
//
//  Description:
//      Compares the two CFqdns to see if they are equal.
//
//==============================================================================
inline
bool CFqdn::operator==(IN const CFqdn& rSrc) const
{
    return (m_strFqdn == rSrc.m_strFqdn && m_uPort == rSrc.m_uPort);
}

//==============================================================================
//==
//== GetFqdn
//==
//==============================================================================
//
//  Returns:
//      The fully qualified domain name in string format.
//
//  Description:
//      Returns the fully qualified domain name in string format.
//
//==============================================================================
inline
CString CFqdn::GetFqdn() const
{
    return m_strFqdn;
}

//==============================================================================
//==
//== GetPort
//==
//==============================================================================
//
//  Returns:
//      The fully qualified domain name's port number.
//
//  Description:
//      Returns the fully qualified domain name's port number.
//
//==============================================================================
inline
uint16_t CFqdn::GetPort() const
{
    return m_uPort;
}

//==============================================================================
//==
//== GetFqdn
//==
//==============================================================================
//
//  Description:
//      Resets the data contained in the CFqdn.
//
//==============================================================================
inline
void CFqdn::Reset()
{
    m_strFqdn.EraseAll();
    m_uPort = 0 ;
}

//==============================================================================
//==
//== SetFqdn
//==
//==============================================================================
//
//  Parameters:
//      rstrFqdn:
//          A reference to a CString holding the fully qualified domain name to
//          set.
//
//  Description:
//      Sets the name of the fully qualified domain name to use.
//
//==============================================================================
inline
void CFqdn::SetFqdn(IN const CString& rstrFqdn)
{
    m_strFqdn = rstrFqdn;
}

//==============================================================================
//==
//== SetPort
//==
//==============================================================================
//
//  Parameters:
//      uPort:
//          The port number to set to this CFqdn
//
//  Description:
//      Sets the port number of the fully qualified domain name to use.
//
//==============================================================================
inline
void CFqdn::SetPort(IN const uint16_t uPort)
{
    m_uPort = uPort;
}

//M5T_INTERNAL_USE_BEGIN
inline
CMarshaler& operator<<(IN CMarshaler& rMarshaler, IN const CFqdn& rData)
{
    rMarshaler << rData.m_strFqdn;
    rMarshaler << rData.m_uPort;
    return rMarshaler;
}

inline
CMarshaler& operator>>(IN CMarshaler& rMarshaler, IN CFqdn& rData)
{
    rMarshaler >> rData.m_strFqdn;
    rMarshaler >> rData.m_uPort;
    return rMarshaler;
}
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_CFQDN_H
