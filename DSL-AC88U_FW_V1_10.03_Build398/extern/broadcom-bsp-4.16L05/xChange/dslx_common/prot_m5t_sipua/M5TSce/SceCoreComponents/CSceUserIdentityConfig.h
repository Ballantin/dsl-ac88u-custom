//==============================================================================
//==============================================================================
//
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
#ifndef MXG_CSCEUSERIDENTITYCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEUSERIDENTITYCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEUSERIDENTITYCONFIG_H
#include "SceCoreComponents/ISceUserIdentityConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CNameAddr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceUserIdentityConfig
//==============================================================================
//
// Description:
//   Implementation of the ISceUserIdentityConfig interface. This object is
//   never accessed directly. It is always used through the
//   ISceUserIdentityConfig interface.
//
// Location:
//   SceCoreComponents/CSceUserIdentityConfig.h
//
// See Also:
//   ISceUserIdentityConfig
//
//==============================================================================
class CSceUserIdentityConfig : private CEComDelegatingUnknown,
                               private ISceUserIdentityConfig
{
//-- Published Interface
public:
    // << ECom Pattern >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

//-- Hidden Methods
//-------------------
protected:
private:
    //   << Startup mechanism >>
    static mxt_result InitializeCSceUserIdentityConfig();
    static void FinalizeCSceUserIdentityConfig();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSceUserIdentityConfig(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceUserIdentityConfig();

    // Inherited from ISceUserIdentityConfig
    virtual void SetNetworkAssertedIdentityMode(IN ENetworkAssertedIdMode eMode);
    virtual mxt_result SetPreferredIdentity(IN TO CNameAddr* pSipIdentity,
                                            IN TO CNameAddr* pTelIdentity);
    virtual void SetPrivacyMode(IN EPrivacyMode eMode);
    virtual ENetworkAssertedIdMode GetNetworkAssertedIdentityMode() const;
    virtual void GetPreferredIdentity(OUT const CNameAddr*& rpSipIdentity,
                                      OUT const CNameAddr*& rpTelIdentity) const;
    virtual EPrivacyMode GetPrivacyMode() const;

    // Inherited from CEComDelegatingUnknown.
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy constructor.
    CSceUserIdentityConfig(IN const CSceUserIdentityConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceUserIdentityConfig& operator=(IN const CSceUserIdentityConfig& rSrc);

// Friends declarations
    friend class CSceCoreComponentsInitializer;

//-- Hidden Data Members
protected:
private:

    // The default preferred Identities.
    CNameAddr* m_pDefaultSipId;
    CNameAddr* m_pDefaultTelId;

    // The default privacy Mode.
    EPrivacyMode m_eDefaultPrivacyMode;

    // The default network asserted identity mode.
    ENetworkAssertedIdMode m_eDefaultNetworkAssertedIdMode;

};

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSCEUSERIDENTITYCONFIG_H


