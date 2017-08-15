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
#ifndef MXG_CSCEUSERSECURITYCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEUSERSECURITYCONFIG_H

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

#ifndef MXG_ISCEUSERSECURITYCONFIG_H
#include "SceCoreComponents/ISceUserSecurityConfig.h"
#endif

#ifndef MXG_ISCEUSERCONFIGTERMINATION_H
#include "SceCoreComponents/ISceUserConfigTermination.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceUserSecurityConfig
//==============================================================================
//
// Description:
//   Implementation of the ISceUserSecurityConfig interface. This object is
//   never accessed directly. It is always used through the
//   ISceUserSecurityConfig interface.
//
// Location:
//   SceCoreComponents/CSceUserSecurityConfig.h
//
// See Also:
//   ISceUserSecurityConfig
//
//==============================================================================
class CSceUserSecurityConfig : private CEComDelegatingUnknown,
                               private ISceUserSecurityConfig,
                               private ISceUserConfigTermination
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
    static mxt_result InitializeCSceUserSecurityConfig();
    static void FinalizeCSceUserSecurityConfig();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSceUserSecurityConfig(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceUserSecurityConfig();

    // Inherited from CEComDelegatingUnknown.
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Inherited from ISceUserSecurityConfig.
    virtual void EnablePersistentTlsConnection(IN bool bEnable);

    virtual bool IsPersistentTlsConnectionEnabled() const;

    virtual void SetPersistentConnectionLocalAddress(IN const CSocketAddr* pLocalAddr);

    virtual const CSocketAddr* GetPersistentConnectionLocalAddress() const;

    virtual void SetSignalingSecurityPolicy(IN ESigSecurityPolicy ePolicy);

    virtual ESigSecurityPolicy GetSignalingSecurityPolicy() const;

    //-- << ISceUserConfigTermination >>
    virtual void OnUserConfigTermination();


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy constructor.
    CSceUserSecurityConfig(IN const CSceUserSecurityConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceUserSecurityConfig& operator=(IN const CSceUserSecurityConfig& rSrc);

// Friends declarations
    friend class CSceCoreComponentsInitializer;

//-- Hidden Data Members
protected:
private:

    // The local address used for the persistent connection. Will be used by
    // BuildContact to create the Contact when persistent connection are used.
    // NULL when persistent connections are not in use or are in error. This
    // data member is set by the persistent connection list when the connection
    // is established.
    CSocketAddr* m_pPersistentConnectionLocalAddress;

    // This is the default signaling security policy.
    ESigSecurityPolicy m_eSigSecurityPolicy;

    // Tells if a persistent connection is ongoing or not.
    bool m_bIsPersistentConnectionOngoing;
};

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSCEUSERSECURITYCONFIG_H


