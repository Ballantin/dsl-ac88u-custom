//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_IUAIMPPPRESENCEPUBLISHERCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPPRESENCEPUBLISHERCONFIG_H


//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IUaImppPresencePublisherConfig);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppPresencePublisherConfig
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//    This interface defines configuration options for publishing presence
//    information of a user.
//
// Description:
//  This interface defines configuration options for publishing presence
//  information of a user.
//
// Location:
//   SceUaImpp/IUaImppPresencePublisherConfig.h
//
//==============================================================================
class IUaImppPresencePublisherConfig : public IEComUnknown
{
//-- Friend Declarations


//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppPresencePublisherConfig);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetPublicationRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the time to wait before retrying a failed publication
    //      attempt.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to wait.
    //
    //  Description:
    //      Configures the number of seconds to wait before retrying a failed
    //      publication attempt.
    //
    //      The publication component will wait for uNbSeconds after a
    //      publication failure or timeout before it tries to publish the
    //      presence information again.
    //
    //      This defaults to 60 seconds.
    //
    //  See Also:
    //      GetPublicationRetryDelay
    //
    //==========================================================================
    virtual void SetPublicationRetryDelay(IN uint32_t uNbSeconds) = 0;

    //==========================================================================
    //==
    //==  GetPublicationRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured publication retry delay.
    //
    //  Returns:
    //      The publication retry delay.
    //
    //  Description:
    //      Gets the configured publication retry delay.
    //
    //  See Also:
    //      SetPublicationRetryDelay
    //
    //==========================================================================
    virtual uint32_t GetPublicationRetryDelay() const = 0;

    //==========================================================================
    //==
    //==  SetPublicationExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the requested expiration time in publication requests.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to set as expiration time of publications.
    //
    //  Description:
    //      Configures the expiration time to set in publication requests.
    //
    //      It is possible for a server to reject the proposed publication
    //      expiration time if it is too short, in which case the presence
    //      publication component will use the shortest possible value supported
    //      by the server.
    //
    //      It is possible for a server to accept the publication while
    //      reducing the expiration time, in which case the publication
    //      component will refresh the publication based on the server provided
    //      expiration time.
    //
    //      When uNbSeconds is set to zero, no expiration will set in the
    //      request and the server answering it will be responsible to choose
    //      the publication expiration time.
    //
    //      This defaults to 3600 seconds (one hour).
    //
    //  See Also:
    //      GetPublicationExpirationTime
    //
    //==========================================================================
    virtual void SetPublicationExpirationTime(IN uint32_t uNbSeconds) = 0;

    //==========================================================================
    //==
    //==  GetPublicationExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured publication expiration time.
    //
    //  Returns:
    //      The publication expiration time.
    //
    //  Description:
    //      Gets the configured publication expiration time.
    //
    //  See Also:
    //      SetPublicationExpirationTime
    //
    //==========================================================================
    virtual uint32_t GetPublicationExpirationTime() const = 0;

    //  Summary:
    //  This enumeration defines the possible ways that the PIDF information
    //  may be published.
    //
    // Description:
    //  This enumeration defines the possible ways that the PIDF information
    //  may be published.
    enum EPublicationMode
    {
        // Description:
        // The publisher component will try to publish the PIDF information
        // using the partial pidf format "application/pidf-diff+xml". If the
        // server rejects this format, the component will fallback to
        // publishing full PIDF documents using the format
        // "application/pidf+xml".
        eMODE_PARTIAL,

        // Description:
        // The publisher component will always publish the presence state of
        // the user using full PIDF document and the "application/pidf+xml"
        // format.
        eMODE_FULL
    };


    //==========================================================================
    //==
    //==  SetPublicationMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures how the component must publish the PIDF information of
    //      the user.
    //
    //  Parameters:
    //    eMode:
    //      The publication mode. See EPublicationMode.
    //
    //  Description:
    //      Configures how the component must publish the PIDF information of
    //      the user.
    //
    //      The publication mode defaults to eMODE_PARTIAL.
    //
    //  See Also:
    //      GetPublicationMode, EPublicationMode
    //
    //==========================================================================
    virtual void SetPublicationMode(IN EPublicationMode eMode) = 0;


    //==========================================================================
    //==
    //==  GetPublicationMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configuration of the PIDF information publication mode.
    //
    //  Description:
    //      This method returns the configured PIDF information publication
    //      mode. The possibles modes are eMODE_PARTIAL, the default mode, or
    //      eMODE_PARTIAL.
    //
    //  See Also:
    //      SetPublicationMode, EPublicationMode
    //
    //==========================================================================
    virtual EPublicationMode GetPublicationMode() const = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaImppPresencePublisherConfig(){}

    // Summary:
    //  Destructor.
    virtual ~IUaImppPresencePublisherConfig(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaImppPresencePublisherConfig(IN const IUaImppPresencePublisherConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaImppPresencePublisherConfig& operator=(IN const IUaImppPresencePublisherConfig& rSrc);


//-- Hidden Data Members
protected:
private:

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUAIMPPPRESENCEPUBLISHERCONFIG_H

