//==============================================================================
//==============================================================================
//
//              Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCEGLOBALCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEGLOBALCONFIG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceGlobalConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceGlobalConfig
//==============================================================================
//<GROUP TID_PACKAGE_SCECORE_CLASSES>
//
// Summary:
//   This interface defines the global configuration options of the system.
//
// Description:
//  This interface defines the global configuration options of the system.
//
//  This interface serves as entry point to add other global interfaces to the
//  current global configuration.
//
//  A global configuration is one that affects the system, regardless of the
//  users that uses it. User-specific configuration is already handled by the
//  ISceUserConfig interface and its children.
//
// Location:
//   SceCore/ISceGlobalConfig.h
//
// See Also:
//  SceGlobalConfig::SceGetGlobalConfig
//
//==============================================================================
class ISceGlobalConfig : public IEComUnknown
{
//-- Friend Declarations.

//-- Published Interface.
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceGlobalConfig);
//M5T_INTERNAL_USE_END

    //<GROUP TID_PACKAGE_SCECORE_STRUCTSENUMS>
    // Summary:
    //  Enumeration describing the representation of the local address.
    //
    // Description:
    //  Enumeration describing the representation of the local address.
    enum ELocalAddressType
    {
        // The local address is represented as an FQDN when possible.
        ePREFER_FQDN,

        // The local address is represented as an IP address.
        eFORCE_IP_ADDRESS
    };

    //==========================================================================
    //==
    //==  RegisterGlobalConfigObject
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a global configuration object.
    //
    //  Parameters:
    //    classId:
    //      The class ID of the object to create. Cannot be NULL.
    //
    //  Returns:
    //      - resS_OK: Global configuration object is configured.
    //      - resFE_INVALID_ARGUMENT: classId is NULL.
    //      - resFE_DUPLICATE: classId already registered.
    //
    //  Description:
    //      Registers a configuration object that must be aggregated by the
    //      global configuration object. The new configuration object will be
    //      available through SceGetGlobalConfig().
    //
    //      Some components rely on such global configuration interfaces
    //      being available in order to get their configuration. An application
    //      creating its own components could also want to maintain a global
    //      configuration state in such an object that is automatically
    //      aggregated to the global configuration.
    //
    //  See Also:
    //      SceGlobalConfig::SceGetGlobalConfig
    //
    //==========================================================================
    virtual mxt_result RegisterGlobalConfigObject(IN mxt_clsid classId) = 0;

    //==========================================================================
    //==
    //==  SetLocalAddressType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the representation of the local address.
    //
    //  Parameters:
    //    eLocalAddressType:
    //      The representation of the local address to set.
    //
    //  Description:
    //      Configures how the local address is presented to remote
    //      applications.
    //
    //      When setting to eFORCE_IP_ADDRESS, the address is shown as an IP
    //      address.
    //
    //      When set to ePREFER_FQDN, the first FQDN set in
    //      ISipCoreConfig::AddLocalAddress for the local address sending the
    //      packet will be used as the local address if it is possible. This
    //      applies to the SIP Contact header and the SDP 'o' line value.
    //
    //      The FQDN will not be used for the Contact header of a user when the
    //      user has a forced Contact (ISceUserConfig::SetForcedContact) or a
    //      configured GRUU (ISceGruuConfig::SetPublicGruu or
    //      ISceGruuConfig::AddTempGruu).
    //
    //      The FQDN will not be used for the 'o' line if the address has been
    //      explicitely set (IMspConfig::SetSdpOriginAddress).
    //
    //      Default value is to use the local IP address as our local address
    //      (ie. eFORCE_IP_ADDRESS).
    //
    //  See Also:
    //      SIP-UA's ISipCoreConfig::AddLocalAddress,
    //      SIP-UA's ISipCoreConfig::SetViaAddressType
    //
    //==========================================================================
    virtual void SetLocalAddressType(IN ELocalAddressType eLocalAddressType) = 0;

    //==========================================================================
    //==
    //==  GetLocalAddressType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the representation of the local address.
    //
    //  Returns:
    //      The configured representation of the local address.
    //
    //  Description:
    //      Returns the representation of the local address.
    //
    //  See Also:
    //      SIP-UA's ISipCoreConfig::AddLocalAddress,
    //      SIP-UA's ISipCoreConfig::SetViaAddressType
    //
    //==========================================================================
    virtual ELocalAddressType GetLocalAddressType() const = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods.
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceGlobalConfig() {}

    // Summary:
    //  Destructor.
    virtual ~ISceGlobalConfig() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceGlobalConfig(IN const ISceGlobalConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceGlobalConfig& operator=(IN const ISceGlobalConfig& rSrc);

//-- Hidden Data Members.
protected:
private:
//M5T_INTERNAL_USE_END

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEGLOBALCONFIG_H

