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
#ifndef MXG_ISCEUSERIDENTITYCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUSERIDENTITYCONFIG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CNameAddr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceUserIdentityConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUserIdentityConfig
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines configuration options regarding the local and peer
//   user identity in the system.
//
// Description:
//  This interface defines configuration options regarding the local and peer
//  user identity in the system.
//
//  This interface offers privacy configuration, which allows a local user to
//  hide as much as possible his local identity to other peers. Moreover, the
//  interface also offers settings for network asserted identity, allowing a
//  user to use a preferred identity and using the network asserted identity for
//  incoming requests.
//
//  This interface is available through user configuration objects and also
//  through components. By default, a component uses the user identity
//  configuration found in the user configuration object, however it is possible
//  to override these settings per component when using this interface from a
//  component.
//
//#Trusted Servers Management#
//  The Network Asserted Identity service and the Privacy service rely on a
//  trusted server to know whether they can trust the P-Asserted-Identity header
//  or trust the network with the user's privacy requirements.
//
//  The M5T SIP Client Engine automatically manages trusted servers and it
//  assumes that the trusted server is the first host in the user's pre-loaded
//  route.
//
//  When the trusted server is configured as an FQDN, DNS resolution is
//  perfomed to find out the set of trusted addresses associated with this
//  server. Note that DNS resolution is performed once for each component
//  created. It is thus important to enable DNS caching in order to minimize the
//  time required for performing this resolution. If your platform does not
//  support DNS caching, see MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT and
//  MXD_RESOLVER_CACHE_ENABLE_SUPPORT in the M5T Framework API Reference to know
//  how to enable the M5T DNS resolver and the DNS caching feature.
//
// Location:
//   SceCoreComponents/ISceUserIdentityConfig.h
//
//==============================================================================
class ISceUserIdentityConfig : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceUserIdentityConfig);
//M5T_INTERNAL_USE_END

    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Description:
    //  This enumeration defines the possible policies to use with network
    //  asserted identity. Network asserted identity is based on RFC 3325.
    enum ENetworkAssertedIdMode
    {
        // Description:
        // Disables the network asserted identity service for this user.
        // Requests and responses from any host will be accepted. The
        // P-Asserted-Identity header is ignored in incoming packets.
        //---------------------------------------------------------------
        eNAIMODE_DISABLE,

        // Description:
        // Enables the network asserted identity service for this user. Requests
        // from any host will be accepted, however the M5T SIP Client Engine
        // will not interpret the network asserted identity received from
        // untrusted hosts. Network asserted identity from the trusted host will
        // be provided to the user through the ISceGenericServerEventInfo
        // interface.
        //-----------------------------------------------------------------
        eNAIMODE_ENABLE,

        // Description:
        // Enables and enforces the network asserted identity service for this
        // user. Incoming packets from untrusted sources are automatically
        // rejected by components.
        //-----------------------------------------------------------------
        eNAIMODE_ENFORCE
    };

    //==========================================================================
    //==
    //==  SetNetworkAssertedIdentityMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures how to apply network asserted identity for the user.
    //
    //  Parameters:
    //    eMode:
    //      The network asserted identity mode to apply.
    //
    //  Description:
    //      Configures how to apply network asserted identity for the user. See
    //      ENetworkAssertedIdMode to see how each mode affects the user and its
    //      associated components.
    //
    //      The default mode used if this method is not called is
    //      eNAIMODE_DISABLE.
    //
    //      Note that a component will always use the mode set in the
    //      configuration so setting a different mode to a component will not
    //      change it's behavior.
    //
    //  See Also:
    //      ENetworkAssertedIdMode, GetNetworkAssertedIdentityMode
    //
    //==========================================================================
    virtual void SetNetworkAssertedIdentityMode(IN ENetworkAssertedIdMode eMode) = 0;


    //==========================================================================
    //==
    //==  SetPreferredIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the preferred identities used for this user/component.
    //
    //  Parameters:
    //    pSipIdentity:
    //      The preferred SIP identity. This CNameAddr must hold a "sip" or
    //      "sips" URI. Can be NULL. Ownership is always TAKEN.
    //
    //    pTelIdentity:
    //      The preferred TEL identity. This CNameAddr must hold a "tel" URI.
    //      Can be NULL. Ownership is always TAKEN.
    //
    //  Returns:
    //      resS_OK upon success or a generic error code on failure.
    //
    //  Description:
    //      Configures the preferred identities for this user/component. The
    //      preferred identities are included in SIP packets following RFC 3325,
    //      using the P-Preferred-Identity header.
    //
    //      This method configures the default preferred identity for a user,
    //      when set on a user configuration object, or for a component when set
    //      on a component object. Setting a preferred identity on a component
    //      overrides the preferred identity configured at the user
    //      configuration level.
    //
    //      Note that the preferred identity will not be used when
    //      ISceUserIdentityConfig::SetNetworkAssertedIdentityMode is set
    //      to eNAIMODE_DISABLE.
    //
    //  See Also:
    //      ISceUserIdentityConfig::SetNetworkAssertedIdentityMode,
    //      GetPreferredIdentity
    //
    //==========================================================================
    virtual mxt_result SetPreferredIdentity(IN TOA CNameAddr* pSipIdentity,
                                            IN TOA CNameAddr* pTelIdentity) = 0;


    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   The possible privacy configuration that can be applied on a user
    //   configuration object or on a component.
    //
    // Description:
    //  This enumeration defines the possible privacy configuration that can be
    //  applied on a user configuration object or on a component.
    //
    //==========================================================================
    enum EPrivacyMode
    {
        // Description:
        // Disables the privacy service overrride for this component. The
        // privacy settings will be taken from the user configuration instead,
        // if there is any.
        ePRIV_USE_USER_CONFIG,

        // Description:
        // Disables the privacy service for this user/component. No Privacy SIP
        // headers will be inserted in outgoing SIP packets and no anonymizing
        // transformation will be applied.
        ePRIV_DISABLE,

        // Description:
        // Enables local privacy. Sensitive SIP headers like the From header
        // will be anonymized. The Contact header will use a temporary GRUU if
        // one is available, otherwise the public GRUU, and, as a last resort, a
        // valid Contact will be used.
        //
        // When this setting is used, the SCE will not include any Privacy
        // header but will anonymize the aforementionned headers before sending
        // on the network. The Referred-By header will not be added.
        ePRIV_LOCAL,

        // Description:
        // Enables local privacy and requires network assisted privacy to
        // anonymize SIP headers that cannot be annonymized by the M5T SIP
        // Client Engine (like Via and possibly the Contact if no temporary
        // GRUUs are available). This level also includes anonymizing the
        // potential network asserted identity of the user if a request is to
        // be forwarded outside of the domain associated with the trusted
        // server.
        //
        // When this setting is used, the SCE will include the following Privacy
        // header. "Privacy: Header; id; critical". A Proxy-Require header is
        // also added to ensure that the proxy supports the requested privacy
        // setting. The Referred-By header will not be added.
        ePRIV_NETWORK_LEVEL_1,

        // Description:
        // Enables local privacy and requires network assisted privacy as
        // described in ePRIV_NETWORK_LEVEL_1. Furthermore, requires the
        // network privacy service to anonymize the SDP sessions sent by the
        // M5T SIP Client Engine on behalf of the user.
        //
        // When this setting is used, the SCE will include the following Privacy
        // header. "Privacy: Header; id; session; critical". A Proxy-Require
        // header is also added to ensure that the proxy supports the requested
        // privacy setting. The Referred-By header will not be added.

        ePRIV_NETWORK_LEVEL_2,

// BRCM_CUSTOM - [add] Add OIR/TIR support per 3GPP TS 22.173
        // Description:
        // Enables local privacy and require network assisted privacy to
        // anonymize SIP headers that cannot be annonymized by the M5T SIP
        // Client Engine. This level includes anonymizing the potential
        // network asserted identity of the user if a request is to be
        // forwarded outside of the domain associated with the trusted
        // server.
        ePRIV_ID_RESTRICTION_LEVEL_3,
// BRCM_CUSTOM - [end] Add OIR/TIR support per 3GPP TS 22.173

// BRCM_CUSTOM - [add] Add custom privacy level for legacy interoperability
        ePRIV_ID_NON_CRIT_LEVEL_4,
// BRCM_CUSTOM - [end] Add custom privacy level for legacy interoperability
    };

    //==========================================================================
    //==
    //==  SetPrivacyMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the default privacy mode.
    //
    //  Parameters:
    //    eMode:
    //      The privacy mode. See EPrivacyMode.
    //
    //  Description:
    //      Configures the privacy mode for this user or component. The privacy
    //      mode configured at the user configuration level can be seen as the
    //      default privacy level that will be used by all components. However,
    //      the component can also be set with a privacy level, which overrides
    //      the level found in the user configuration object associated with the
    //      component.
    //
    //      The default privacy mode used if this method is not called is
    //      ePRIV_DISABLE.
    //
    //  See Also:
    //      EPrivacyMode
    //
    //==========================================================================
    virtual void SetPrivacyMode(IN EPrivacyMode eMode) = 0;


    //==========================================================================
    //==
    //==  GetNetworkAssertedIdentityMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current network asserted identity usage for the user.
    //
    //  Returns:
    //      The current network asserted identity usage for the user.
    //
    //  Description:
    //      Returns the current network asserted identity usage for the user.
    //
    //      The default mode used if the SetNetworkAssertedIdentityMode method
    //      has not been called is eNAIMODE_DISABLE.
    //
    //  See Also:
    //      ENetworkAssertedIdMode, SetNetworkAssertedIdentityMode
    //
    //==========================================================================
    virtual ENetworkAssertedIdMode GetNetworkAssertedIdentityMode() const = 0;

    //==========================================================================
    //==
    //==  GetPreferredIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the preferred identities used for this user/component.
    //
    //  Parameters:
    //    rpSipIdentity:
    //      Pointer to a CNameAddr that will be set to point to the
    //      configured SIP identity. Can be set to NULL if no preferred SIP
    //      identity is configured.
    //
    //    rpTelIdentity:
    //      Pointer to a CNameAddr that will be set to point to the
    //      configured TEL identity. Can be set to NULL if no preferred TEL
    //      identity is configured.
    //
    //  Description:
    //      Returns the preferred identities for this user/component. The
    //      preferred identities are included in SIP packets following RFC 3325,
    //      using the P-Preferred-Identity header.
    //
    //      This method returns the current preferred identity for a user,
    //      when called on a user configuration object, or for a component when
    //      called on a component object.
    //
    //  See Also:
    //      SetNetworkAssertedIdentityMode, SetPreferredIdentity
    //
    //==========================================================================
    virtual void GetPreferredIdentity(OUT const CNameAddr*& rpSipIdentity,
                                      OUT const CNameAddr*& rpTelIdentity) const = 0;

    //==========================================================================
    //==
    //==  GetPrivacyMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current privacy mode.
    //
    //  Returns:
    //      The current privacy mode.
    //
    //  Description:
    //      Returns the current privacy mode for this user or component. The
    //      privacy mode configured at the user configuration level can be seen
    //      as the default privacy level that will be used by all components.
    //      However, the component can also be set with a privacy level, which
    //      overrides the level found in the user configuration object
    //      associated with the component.
    //
    //      The default privacy mode used if the SetPrivacyMode method has not
    //      been called is ePRIV_DISABLE.
    //
    //  See Also:
    //      EPrivacyMode, SetPrivacyMode
    //
    //==========================================================================
    virtual EPrivacyMode GetPrivacyMode() const = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISceUserIdentityConfig() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISceUserIdentityConfig() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISceUserIdentityConfig(IN const ISceUserIdentityConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISceUserIdentityConfig& operator=(IN const ISceUserIdentityConfig& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEUSERIDENTITYCONFIG_H

