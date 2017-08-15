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
#ifndef MXG_IUASSPCALLCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPCALLCONFIG_H
//M5T_INTERNAL_USE_END

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
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IUaSspCallConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspCallConfig
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This interface defines the user configuration options for call management.
//
// Description:
//  This interface defines the user configuration options for call management.
//
// Location:
//   UaSsp/IUaSspCallConfig.h
//
// See Also:
//   IUaSspCall, IUaSspCallMgr
//
//==============================================================================
class IUaSspCallConfig : public IEComUnknown
{
//-- Friend Declarations


//-- Published Interface
public:

//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaSspCallConfig);
//M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  EnableSessionTimerSupport
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables or disables the support of the SIP session-timer mechanism.
    //
    //  Parameters:
    //    bEnable:
    //      Tells if the support of the SIP session-timer mechanism is enabled
    //      or not.
    //
    //      By default, the support of the SIP session-timer mechanism is
    //      enabled.
    //
    //  Description:
    //      Enables or disables the support of the SIP session-timer mechanism
    //      for this user.
    //
    //  See Also:
    //      IsSessionTimerSupportEnabled, SetSessionExpiration,
    //      SetSessionRefreshMethod
    //
    //==========================================================================
    virtual void EnableSessionTimerSupport(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  IsSessionTimerSupportEnabled
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells if the support of the SIP session-timer mechanism is enabled
    //      or not.
    //
    //  Returns:
    //      True if the support of the SIP session-timer mechanism is enabled,
    //      false otherwise.
    //
    //  Description:
    //      Tells if the support of the SIP session-timer mechanism is enabled
    //      or not.
    //
    //  See Also:
    //      EnableSessionTimerSupport
    //
    //==========================================================================
    virtual bool IsSessionTimerSupportEnabled() const = 0;

    //==========================================================================
    //==
    //==  SetSessionExpiration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the default session expiration time requested when
    //      negotiating the SIP session-timer mechanism.
    //
    //  Parameters:
    //    uExpirationSec:
    //      The number of seconds that the session should be considered as valid
    //      without any refreshes. If no session refresh request has been
    //      successfully exchanged within this interval, the session will be
    //      terminated.
    //
    //      The minimal value that can be configured is 90 seconds, as per RFC
    //      4028. If uExpirationSec is less than 90 seconds, then no session
    //      expiration will be defined and session-timer will possibly be
    //      disabled, depending on if the session's peer requires session-timer
    //      support.
    //
    //      The default value used if SetSessionExpiration is not called is 1800
    //      seconds (30 minutes).
    //
    //  Description:
    //      Configures the default session expiration time requested when
    //      negotiating the SIP session-timer mechanism. The actual session
    //      expiration is negotiated session per session with the peer with
    //      which the session is being established, and may differ from the
    //      value configured with this method.
    //
    //      The endpoint that needs to send the session refresh request is also
    //      negotiated with the peer. When the user represented by this
    //      configuration object must refresh the session, it does so at half
    //      of the negotiated session expiration, as per RFC 4028.
    //      
    //      When the endpoint that needs to send the session refresh request is
    //      the session's peer, the implementation will expect a timely session
    //      refresh request from the peer. The session will be considered as
    //      terminated if no session refresh request is received before X
    //      seconds of the negotiated session expiration time, where X is the
    //      smaller between 32 seconds or one third of the negotiated session
    //      expiration time.
    //
    //  See Also:
    //      EnableSessionTimerSupport, GetSessionExpiration,
    //      SetSessionRefreshMethod
    //
    //==========================================================================
    virtual void SetSessionExpiration(IN uint32_t uExpirationSec) = 0;

    //==========================================================================
    //==
    //==  GetSessionExpiration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current value of the default session expiration time
    //      requested when negotiating the SIP session-timer mechanism.
    //
    //  Returns:
    //      An unsigned int value representing the number of seconds for the
    //      expiration of a SIP session.
    //
    //  Description:
    //      Returns the current value of the default session expiration time
    //      requested when negotiating the SIP session-timer mechanism.
    //
    //  See Also:
    //      EnableSessionTimerSupport, SetSessionExpiration
    //
    //==========================================================================
    virtual uint32_t GetSessionExpiration() const = 0;
    
// BRCM_CUSTOM [add] Min-SE configuration support
    //==========================================================================
    //==
    //==  SetMinSE
    //==
    //==========================================================================
    //
    // Summary:
    //      Configures the minimum session expiration time requested when
    //      negotiating the SIP session-timer mechanism.
    //
    //  Parameters:
    //    uMinSE:
    //      The number of seconds that session expiration can be set to at a
    //      minimum.
    //
    //      The minimal value that can be configured is 90 seconds, as per RFC
    //      4028. If uExpirationSec is less than 90 seconds, then the Min-SE
    //      will be set to equal the configured session expiration time.
    //
    //      The default value used if SetSessionExpiration is the same as the
    //      session expires value.
    //      
    //
    //  Description:
    //      Configures the minimum session expiration time requested when
    //      negotiating the SIP session-timer mechanism. 

    //
    //  See Also:
    //      EnableSessionTimerSupport, GetSessionExpiration, 
    //      SetSessionExpiration, SetSessionRefreshMethod
    //
    //==========================================================================
    virtual void SetMinSE(IN uint32_t uMinSE) = 0;
    
    //==========================================================================
    //==
    //==  GetMinSE
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current value of the minimum session expiration time
    //      requested when negotiating the SIP session-timer mechanism.
    //
    //  Returns:
    //      An unsigned int value representing the number of seconds for the
    //      minimum acceptable session interval
    //
    //  Description:
    //      Returns the current value of the minimum session expiration time
    //      requested when negotiating the SIP session-timer mechanism.
    //
    //  See Also:
    //      EnableSessionTimerSupport, SetMinSE
    //
    //==========================================================================
    virtual uint32_t GetMinSE() const = 0;
// BRCM_CUSTOM [end] Min-SE configuration support
    
    // Summary:
    //  Possible configurations to refresh a session using the SIP session-timer
    //  mechanism.
    //
    // Description:
    //  This enumeration defines the possible configurations to refresh a
    //  session using the SIP session-timer mechanism.
    enum ESessionRefreshConfig
    {
        // Description:
        //  Refreshes sessions with an UPDATE request if it can be used.
        //  Otherwise refreshes sessions with INVITE requests.
        eREFRESH_AUTOMATIC,

        // Description:
        //  Refreshes sessions with an INVITE request only.
        eREFRESH_INVITE
    };

    //==========================================================================
    //==
    //==  SetSessionRefreshMethod
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures what session-timer refresh requests are sent.
    //
    //  Parameters:
    //    eConfig:
    //      The session-refresh configuration.
    //
    //  Description:
    //      Configures what request can be sent by the call component to refresh
    //      a session, as per the session-timer mechanism.
    //
    //      The call component always supports the reception of INVITE and UPDATE
    //      requests as session refresh requests.
    //
    //      By default, the configuration is eREFRESH_AUTOMATIC.
    //
    //  See Also:
    //      EnableSessionTimerSupport, GetSessionRefreshMethod,
    //      SetSessionExpiration
    //
    //==========================================================================
    virtual void SetSessionRefreshMethod(IN ESessionRefreshConfig eConfig) = 0;

    //==========================================================================
    //==
    //==  GetSessionRefreshMethod
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns which SIP request is currently used to perform session
    //      refreshes.
    //
    //  Returns:
    //      The session-refresh configuration.
    //
    //  Description:
    //      Returns which SIP request is currently used to perform session
    //      refreshes.
    //
    //  See Also:
    //      EnableSessionTimerSupport, SetSessionRefreshMethod
    //
    //==========================================================================
    virtual ESessionRefreshConfig GetSessionRefreshMethod() const = 0; 


    // Summary:
    //  The available configuration options for reliable provisional responses.
    //
    // Description:
    // This enumeration defines the available configuration options for
    // managing reliable provisional responses, as defined in RFC 3262.
    enum ERel1xxConfig
    {
        // Description:
        //  Disables the reliable provisional response services. No reliable
        //  provisional responses can be sent or received.
        eREL1xx_DISABLE,

        // Description:
        //  Configures calls to use a reliable provisional responses when
        //  required. The first time a provisional response is sent with SDP
        //  triggers the use of reliable provisional responses. All provisional
        //  before the first one with SDP are unreliable, and all provisional
        //  after the first one with SDP are reliable. Note
        //  that if the caller forces the use of reliable provisional response
        //  through a Require header, all provisional responses will be
        //  reliable. If the caller does not support reliable responses,
        //  responses are sent normally.
        eREL1xx_SUPPORT,

        // Description:
        //  Requires the use of reliable provisional responses locally and by
        //  the peer with which the user is communicating. This forces all such
        //  responses sent to be reliable, and forces the peer to support and
        //  always send reliable provisional responses.
        eREL1xx_REQUIRE
    };


    //==========================================================================
    //==
    //==  SetReliable1xxConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures how to manage reliable provisional responses for this
    //      user.
    //
    //  Parameters:
    //    eConfig:
    //      Enumeration defining how to manage reliable provisional responses.
    //
    //  Description:
    //      Configures how to manage reliable provisional responses for this
    //      user.
    //
    //      The default configuration, if this is not configured otherwise, is
    //      eREL1xx_SUPPORT.
    //
    //  See Also:
    //      ERel1xxConfig
    //
    //==========================================================================
    virtual void SetReliable1xxConfig(IN ERel1xxConfig eConfig) = 0;


    //==========================================================================
    //==
    //==  GetReliable1xxConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configuration for reliable provisional responses
    //      management for this user.
    //
    //  Returns:
    //      Enumeration defining how reliable provisional responses are managed.
    //
    //  Description:
    //      Returns the configuration for reliable provisional responses
    //      management for this user.
    //
    //  See Also:
    //      ERel1xxConfig, SetReliable1xxConfig
    //
    //==========================================================================
    virtual ERel1xxConfig GetReliable1xxConfig() = 0;

// BRCM_CUSTOM - [begin] Two-step BYE process
    //==========================================================================
    //==
    //==  SetTwoStepByeConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures how CUaSspCall would behave when a falure response for
    //      BYE is received.
    //
    //  Parameters:
    //    bEnable:
    //      True to enable the two step termination process.
    //
    //  Description:
    //      Configures how CUaSspCall would behave when a falure response for
    //      BYE is received.
    //
    //      The default configuration if this is not configured otherwise is
    //      set to false.
    //
    //  See Also:
    //      GetTwoStepByeConfig
    //
    //==========================================================================
    virtual void SetTwoStepByeConfig(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  GetTwoStepByeConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configuration whether two-step termination process is
    //      used.
    //
    //  Returns:
    //      Boolean defining whether two-step termination process is enabled.
    //
    //  Description:
    //      Returns the configuration whether two-step termination process is
    //      used for this user.
    //
    //  See Also:
    //      SetTwoStepByeConfig
    //
    //==========================================================================
    virtual bool GetTwoStepByeConfig() = 0;
// BRCM_CUSTOM - [end] Two-step BYE process

// BRCM_CUSTOM - [begin] Call termination reinvite timeout for hold
    //==========================================================================
    //==
    //==  SetNoTerm408ReInvMspConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures how CUaSspCall would behave when a response timeout (408)
    //      failure is received for a REINVITE to place a call on hold.
    //
    //  Parameters:
    //    bEnable:
    //      True to not terminate the call and just cancel the offer.
    //
    //  Description:
    //      Configures how CUaSspCall would behave when a response timeout (408)
    //      failure is received for a REINVITE to place a call on hold.
    //
    //      The default configuration if this is not configured otherwise is
    //      set to false.
    //
    //  See Also:
    //      GetNoTerm408ReInvMspConfig
    //
    //==========================================================================
   virtual void SetNoTerm408ReInvMspConfig(IN bool bEnable) = 0;
   
    //==========================================================================
    //==
    //==  GetNoTerm408ReInvMspConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configuration whether call is not terminated on reinvite 
    //      timeout(408 response) for call hold.
    //
    //  Returns:
    //      Boolean defining whether call is not be terminated.
    //
    //  Description:
    //      Returns the configuration whether call is not terminated on reinvite 
    //      timeout(408 response) for call hold.
    //
    //  See Also:
    //      SetNoTerm408ReInvMspConfig
    //
    //==========================================================================
   virtual bool GetNoTerm408ReInvMspConfig() = 0;
// BRCM_CUSTOM - [begin] Call termination reinvite timeout for hold

// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
    // Description:
    // This enumeration defines the available configuration options for
    // managing connented ID, as defined in RFC 4916.
    //------------------------------------------------------------------
    enum EConnectedIdConfig
    {
        // Description:
        //  Disables the connected Id services. No change in
        //  From or To headers can be sent or received.
        //-------------------------------------------------------------------
        eCONNECTED_ID_DISABLE,

        // Description:
        //  Configures calls to use connected ID. The "from-change" tag will be used
        //  in the Supported header. If used in an out going call and the remote party
        //  supports the feature, an UPDATE or a reINVITE will be received if the 
        //  remote party's NameAddr is not represtented in the To header of the out 
        //  going INVITE. This UPDATE or reINVITE will contain an updated From header
        //  with correct user name and URI.  If used in an incoming call an UPDATE
        //  will be sent if the To header of the INVITE message does not represent
        //  the local party's address.
        //----------------------------------------------------------------------
        eCONNECTED_ID_SUPPORT
    };

    //==========================================================================
    //==
    //==  SetConnectedIdConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the connected ID settings for this user.
    //
    //  Parameters:
    //    eConfig:
    //      Enumeration defining the connected ID settings.
    //
    //  Description:
    //      This function sets the connected ID configuration. Connected ID can
    //      be disabled or supported.
    //
    //      The default configuration if this is not configured otherwise is
    //      eCONNECTED_ID_DISABLED.
    //
    //  See Also:
    //      EConnectedIdConfig, GetConnectedIdConfig
    //
    //==========================================================================
    virtual void SetConnectedIdConfig(IN EConnectedIdConfig eConfig) = 0;


    //==========================================================================
    //==
    //==  GetConnectedIdConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configuration of connected ID for the user.
    //
    //  Returns:
    //      Enumeration defining how connected ID is managed.
    //
    //  Description:
    //      Returns the configuration for connected ID management for 
    //      this user.
    //
    //  See Also:
    //      EConnectedIdConfig, SetConnectedIdConfig
    //
    //==========================================================================
    virtual EConnectedIdConfig  GetConnectedIdConfig() = 0;
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916

    // Summary:
    //  The available ways to support the UPDATE method.
    //
    // Description:
    //  This enumeration defines the available configuration options for
    //  managing UPDATE requests.
    enum EUpdateConfig
    {
        // Description:
        //  Disables UPDATE support. No UPDATE will be sent and incoming UPDATE
        //  requests will get rejected with a 405 (Method Not Allowed).
        eUPDATE_NO_SUPPORT,

        // Description:
        //  Enables support for UPDATE requests. UPDATE requests will be
        //  processed. If the peer allows it, UPDATE requests will be sent to
        //  refresh a dialog and in some offer-answer negotiations requiring its
        //  use.
        eUPDATE_SUPPORT
    };


    //==========================================================================
    //==
    //==  SetUpdateConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures how to support UPDATE requests for this user.
    //
    //  Parameters:
    //    eConfig:
    //      The way to support UPDATE requests.
    //
    //  Description:
    //      Configures how UPDATE requests are supported for this user.
    //
    //      The default configuration is eUPDATE_SUPPORT.
    //
    //  See Also:
    //      GetUpdateConfig, EUpdateConfig
    //
    //==========================================================================
    virtual void SetUpdateConfig(IN EUpdateConfig eConfig) = 0;


    //==========================================================================
    //==
    //==  GetUpdateConfig
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gives how UPDATE requests are supported for this user.
    //
    //  Returns:
    //      The way UPDATE requests are supported.
    //
    //  Description:
    //      Returns how UPDATE requests are supported for this user.
    //
    //  See Also:
    //      SetUpdateConfig, EUpdateConfig
    //
    //==========================================================================
    virtual EUpdateConfig GetUpdateConfig() = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaSspCallConfig(){}

    // Summary:
    //  Destructor.
    virtual ~IUaSspCallConfig(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaSspCallConfig(IN const IUaSspCallConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaSspCallConfig& operator=(IN const IUaSspCallConfig& rSrc);


//-- Hidden Data Members
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPCALLCONFIG_H
