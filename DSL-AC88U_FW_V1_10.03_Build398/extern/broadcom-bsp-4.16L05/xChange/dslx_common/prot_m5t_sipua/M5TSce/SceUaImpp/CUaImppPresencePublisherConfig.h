//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CUAIMPPPRESENCEPUBLISHERCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPPRESENCEPUBLISHERCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IUAIMPPPRESENCEPUBLISHERCONFIG_H
#include "SceUaImpp/IUaImppPresencePublisherConfig.h"
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
//== Class: CUaImppPresencePublisherConfig
//==============================================================================
//
// Description:
//   This class implements the IUaImppPresencePublisherConfig interface. It
//   holds the configurations related to presence publishing.
//
// Location:
//   SceUaImpp/CUaImppPresencePublisherConfig.h
//
//==============================================================================
class CUaImppPresencePublisherConfig : protected CEComDelegatingUnknown,
                                       protected IUaImppPresencePublisherConfig
{
//-- Friend Declarations
    friend class CSceUaImppInitializer;

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:

//-- Hidden Methods
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaImppPresencePublisherConfig(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CUaImppPresencePublisherConfig();


    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaImppPresencePublisherConfig();

    static void FinalizeCUaImppPresencePublisherConfig();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << IUaImppPresencePublisherConfig >>
    virtual void SetPublicationRetryDelay(IN uint32_t uNbSeconds);

    virtual uint32_t GetPublicationRetryDelay() const;

    virtual void SetPublicationExpirationTime(IN uint32_t uNbSeconds);

    virtual uint32_t GetPublicationExpirationTime() const;

    virtual void SetPublicationMode(IN EPublicationMode eMode);

    virtual EPublicationMode GetPublicationMode() const;

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppPresencePublisherConfig(IN const CUaImppPresencePublisherConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppPresencePublisherConfig& operator=(IN const CUaImppPresencePublisherConfig& rSrc);


//-- Hidden Data Members
protected:
private:

    // This is the value set by SetPublicationRetryDelay.
    // Default is 60 seconds (1 minute).
    uint32_t m_uPublicationRetryDelayS;

    // This is the value set by SetPublicationExpirationTime.
    // Default is 3600 seconds (1 hour).
    uint32_t m_uPublicationExpirationTimeS;

    // This is the publishing mode (partial or full) of the PIDF document.
    // Default is eMODE_PARTIAL.
    EPublicationMode m_ePublicationMode;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CUAIMPPPRESENCEPUBLISHERCONFIG_H

//M5T_INTERNAL_USE_END
