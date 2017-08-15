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
#ifndef MXG_ISCEPERSISTENTCONNECTIONLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEPERSISTENTCONNECTIONLIST_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IScePersistentConnectionObserver;
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IScePersistentConnectionList);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IScePersistentConnectionList
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   The persistent connection list manages all persistent connections for the
//   users of the M5T SIP Client Engine.
//
// Description:
//  The persistent connection list manages all persistent connections for the
//  users of the M5T SIP Client Engine. It should not be accessed directly by
//  applications. Instead, the application should call
//  EnablePersistentTlsConnection on the ISceUserSecurityConfig.
//
// Location:
//   SceCoreComponents/IScePersistentConnectionList.h
//
//==============================================================================
class IScePersistentConnectionList : public IEComUnknown
{
//-- Published Interface
public:
    MX_DECLARE_ECOM_GETIID(IScePersistentConnectionList);

    //===========================================================================
    //==
    //==  EnablePersistentConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables a persistent connection for the user in parameter.
    //
    //  Parameters:
    //    pUserConfig:
    //      The user configuration for which to establish a persistent
    //      connection.
    //
    //  Returns:
    //      - resS_OK: The connection is being established.
    //
    //      - resFE_INVALID_ARGUMENT: The pUserConfig is NULL, does not support
    //      the ISceUserSecurityConfig interface or does not have a valid
    //      preloaded route configured. It can also be that pUserConfig is
    //      already associated to a persistent connection.
    //
    //      - resFE_FAIL: There was a problem establishing the connection.
    //
    //  Description:
    //      This method MUST ONLY be called by the ISceUserConfig or one of its
    //      aggregated configuration. Otherwise, an invalid circular reference
    //      will get added on the ISceUserConfig.
    //
    //      This method is called by the ISceUserSecurityConfig when
    //      EnablePersistentTlsConnection is called on it.
    //
    //      It associates a persistent TLS connection to a user config. The
    //      remote side of the connection is set to the topmost preloaded Route
    //      header configured in the user config. No association will be created
    //      if the user config has no preloaded route.
    //
    //      If the connection already exists (because it is used by another
    //      user), it is simply associated with the new user. If the connection
    //      does not exist, it is created.
    //
    //      When the persistent connection is no longer needed by the user in
    //      parameter, it must call DisablePersistentConnection to remove the
    //      association between the persistent connection and itself.
    //
    //      Every observer associated with the user config in parameter will be
    //      notified when the connection is connected or when an error occurs on
    //      the connection. Note that if the connection is already connected (as
    //      it was already used by another user), the EvConnectionEstablished()
    //      event will be issued immediately (before EnablePersistentConnection
    //      exits).
    //
    //  See Also:
    //      ISceUserSecurityConfig::EnablePersistentTlsConnection,
    //      DisablePersistentConnection.
    //
    //===========================================================================
    virtual mxt_result EnablePersistentConnection(IN ISceUserConfig* pUserConfig) = 0;

    //===========================================================================
    //==
    //==  DisablePersistentConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Disables the persistent connection for the user in parameter.
    //
    //  Parameters:
    //    pUserConfig:
    //      The user configuration for which to disable the persistent
    //      connection.
    //
    //  Returns:
    //      - resS_OK: The connection is disabled.
    //
    //      - resFE_INVALID_ARGUMENT: pUserConfig is not associated with any
    //      persistent connection.
    //
    //  Description:
    //      This method is called by ISceUserSecurityConfig when
    //      DisablePersistentTlsConnection is called on it.
    //
    //      It removes the association between the user config and its
    //      persistent connection. If the connection is no longer used by any
    //      user, it is terminated.
    //
    //      Every observer associated to this user are notified that the
    //      persistent connection is no longer used by this user
    //      (EvConnectionTerminated). They are notified immediately (before
    //      DisablePersistentConnection exits).
    //
    //  See Also:
    //      IScePersistentConnectionObserver,
    //      ISceUserSecurityConfig::DisablePersistentTlsConnection.
    //
    //===========================================================================
    virtual mxt_result DisablePersistentConnection(IN ISceUserConfig* pUserConfig) = 0;

    //===========================================================================
    //==
    //==  RegisterObserver
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a persistent connection state observer.
    //
    //  Parameters:
    //    pUserConfig:
    //      The user config for which to observe persistent connection state.
    //
    //    pObserver:
    //      Pointer to the observer interface where to report state changes.
    //
    //  Returns:
    //      - resS_OK: The observer was properly associated to the user config.
    //
    //      - resFE_INVALID_ARGUMENT: Either pUserConfig or pObserver are NULL
    //      or they are already associated.
    //
    //  Description:
    //      Associates a persistent connection state observer to a user config.
    //
    //      An observer instance can observe multiple user configs. User configs
    //      can also be observed by multiple observers.
    //
    //      The observer must be prepared to receive an event before this method
    //      returns as one event is called to inform the new observer of the
    //      current state of the persistent connection for the user.
    //
    //  See Also:
    //      IScePersistentConnectionObserver, UnregisterObserver
    //
    //===========================================================================
    virtual mxt_result RegisterObserver(IN ISceUserConfig* pUserConfig,
                                        IN IScePersistentConnectionObserver* pObserver) = 0;

    //===========================================================================
    //==
    //==  UnregisterObserver
    //==
    //==========================================================================
    //
    //  Summary:
    //      Unregisters a persistent connection state observer.
    //
    //  Parameters:
    //    pUserConfig:
    //      The user config for which to stop to observe persistent connection
    //      state.
    //
    //    pObserver:
    //      Pointer to the observer interface that is no longer interested in
    //      persistent connection state for that user.
    //
    //  Returns:
    //      - resS_OK: The observer was properly removed for that user config.
    //
    //      - resFE_INVALID_ARGUMENT: There is no association between this
    //      observer and this user config. Although the net result is the same
    //      as if the association was actually removed by this method, an error
    //      is returned as the observer should be aware of the user config it
    //      observes and hence should not unregister user configs it does not
    //      observe.
    //
    //  Description:
    //      Dissociates a persistent connection state observer from a user
    //      config.
    //
    //      It has no impact on associations that the observer may have with
    //      other user configs. It has no impact on associations that the user
    //      config may have with other observers.
    //
    //      After this method is called, the observer will no longer receive any
    //      event about state change in the persistent connection for this user
    //      config.
    //
    //  See Also:
    //      RegisterObserver
    //
    //===========================================================================
    virtual mxt_result UnregisterObserver(IN ISceUserConfig* pUserConfig,
                                          IN IScePersistentConnectionObserver* pObserver) = 0;


protected:
    //-- << Constructors / Destructors / Operators >>
    IScePersistentConnectionList() {}
    virtual ~IScePersistentConnectionList() {}
};


MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISCEPERSISTENTCONNECTIONLIST_H

