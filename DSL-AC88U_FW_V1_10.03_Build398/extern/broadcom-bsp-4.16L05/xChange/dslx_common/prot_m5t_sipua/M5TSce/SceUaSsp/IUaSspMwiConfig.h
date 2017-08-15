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
#ifndef MXG_IUASSPMWICONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPMWICONFIG_H
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
class CSipUri;



//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IUaSspMwiConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspMwiConfig
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This interface defines the user configuration options for message waiting
//   indication.
//
// Description:
//  This interface defines the user configuration options for message waiting
//  indication.
//
// Location:
//   UaSsp/IUaSspMwiConfig.h
//
// See Also:
//   IUaSspMwi, IUaSspMwiMgr
//
//==============================================================================
class IUaSspMwiConfig : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:

//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaSspMwiConfig);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetMwiServerAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the address of the server providing the MWI service.
    //
    //  Parameters:
    //    pAddress:
    //      The address of the server. OWNERSHIP is TAKEN. Cannot be NULL.
    //
    //  Returns:
    //      - resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Configures the address of the server providing the MWI service. This
    //      address is usually the address-of-record (AOR) of the user. This
    //      address is used only when subscribing to receive MWI notifications
    //      and is thus not used by the unsolicited MWI component.
    //
    //      By default, this address is NULL.
    //
    //  See Also:
    //      GetMwiServerAddress
    //
    //==========================================================================
    virtual mxt_result SetMwiServerAddress(IN TO CSipUri* pAddress) = 0;

    //==========================================================================
    //==
    //==  GetMwiServerAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured MWI server address.
    //
    //  Returns:
    //      The address of the MWI server.
    //
    //  Description:
    //      Gets the configured MWI server address.
    //
    //  See Also:
    //      SetMwiServerAddress
    //
    //==========================================================================
    virtual const CSipUri* GetMwiServerAddress() const = 0;

    //==========================================================================
    //==
    //==  SetSubscriptionExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the requested expiration time of a MWI subscription.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //          uNbSeconds is 0.
    //      - resS_OK:
    //          the requested MWI subscription expiration time has been
    //          modified.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to set as expiration time of MWI subscriptions
    //      (ie. added into the Expires header in MWI SUBSCRIBE requests). This
    //      parameter MUST be different than 0.
    //
    //  Description:
    //      Configures the expiration time to ask for in SUBSCRIBE requests.
    //
    //      It is possible for a server to lower the proposed expiration time.
    //
    //      A server might also reject the expiration time if it is too short,
    //      in which case the subscription will be retried with the shortest
    //      possible value supported by the server.
    //
    //      This defaults to 3600 seconds (one hour).
    //
    //      Note that the change does not affect already established
    //      subscriptions.
    //
    //  See Also:
    //      GetSubscriptionExpirationTime
    //
    //==========================================================================
    virtual mxt_result SetSubscriptionExpirationTime(IN uint32_t uNbSeconds) = 0;

    //==========================================================================
    //==
    //==  GetSubscriptionExpirationTime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configured expiration time for a MWI subscription.
    //
    //  Returns:
    //      The number of seconds used for expiration time of MWI subscriptions,
    //      in seconds.
    //
    //  Description:
    //      Returns the configured expiration time for a MWI subscription, in
    //      seconds.
    //
    //  See Also:
    //      SetSubscriptionExpirationTime
    //
    //==========================================================================
    virtual uint32_t GetSubscriptionExpirationTime() const = 0;

// BRCM_CUSTOM - [add] Add MWI subscription retry delay config
    //==========================================================================
    //==
    //==  SetSubscriptionRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the time to wait after a failed MWI subscription attempt
    //      and the next one.
    //
    //  Parameters:
    //    uRetryDelayS:
    //      The number of seconds to wait.
    //
    //  Description:
    //      Configures the number of seconds to wait after a failed subscription
    //      attempt and the next one.
    //
    //      This defaults to 60 seconds.
    //
    //  See Also:
    //      GetSubscriptionRetryDelay
    //
    //==========================================================================
    virtual void SetSubscriptionRetryDelay(IN uint32_t uRetryDelayS) = 0;

    //==========================================================================
    //==
    //==  GetSubscriptionRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configured MWI subscription retry delay.
    //
    //  Returns:
    //      The MWI subscription retry delay, in seconds.
    //
    //  Description:
    //      Returns the configured MWI subscription retry delay.
    //
    //  See Also:
    //      SetSubscriptionRetryDelay
    //
    //==========================================================================
    virtual uint32_t GetSubscriptionRetryDelay() const = 0;
// BRCM_CUSTOM - [add] Add MWI subscription retry delay config

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  SetComponent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the component that will be handling unsolicited
    //      notifications.
    //
    //  Parameters:
    //    pComponent:
    //      Pointer to the MWI component.
    //
    //  Returns:
    //      Always returns resS_OK as of now.
    //
    //  Description:
    //      Configures the component that will be handling unsolicited
    //      notifications.
    //
    //      This method is internal to M5T and the UA SSP and not meant for use
    //      by an external application.
    //
    //  See Also:
    //      GetComponent
    //
    //==========================================================================
    virtual mxt_result SetComponent(IN IEComUnknown* pComponent) = 0;

    //==========================================================================
    //==
    //==  GetComponent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the component that will handle unsolicited notifies.
    //
    //  Parameters:
    //    ppComponent:
    //      Double pointer to the component. If it returns non-NULL, a reference
    //      will have been counted on the object.
    //
    //  Description:
    //      Gets the component that will handle unsolicited notifies.
    //
    //      This method is internal to M5T and the UA SSP and not meant for use
    //      by an external application.
    //
    //  See Also:
    //      SetComponent
    //
    //==========================================================================
    virtual void GetComponent(OUT IEComUnknown** ppComponent) = 0;
//M5T_INTERNAL_USE_END


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaSspMwiConfig() {}

    // Summary:
    //  Destructor.
    virtual ~IUaSspMwiConfig() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaSspMwiConfig(IN const IUaSspMwiConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaSspMwiConfig& operator=(IN const IUaSspMwiConfig& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPMWICONFIG_H

