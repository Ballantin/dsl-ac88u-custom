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
#ifndef MXG_ISCEPERSISTENTCONNECTIONOBSERVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEPERSISTENTCONNECTIONOBSERVER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IScePersistentConnectionObserver
//==============================================================================
//<GROUP TID_PACKAGE_SCE_CORE_COMPONENTS_CLASSES>
//
// Summary:
//   Interface on which the persistent connection state is reported for a
//   specific user config.
//
// Description:
//   This interface receives the state for the persistent connection of a
//   specific user config.
//
//   This interface should not be used by the application. It is used by objects
//   of the client engine to know when the persistent connection is established,
//   in error or terminated for a single user config.
//
// Location:
//   SceCoreComponents/IScePersistentConnectionObserver.h
//
// See Also:
//   IScePersistentConnectionList
//
//==============================================================================
class IScePersistentConnectionObserver
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:

    //===========================================================================
    //==
    //==  EvConnectionEstablished
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the observer that the persistent connection is established.
    //
    //  Parameters:
    //    pUserConfig:
    //      The user config for which the persistent connection is established.
    //
    //  Description:
    //      This event is called when the persistent connection is established
    //      and ready to be used. The observer can consider that the connection
    //      is valid until EvErrorOnConnection or EvConnectionTerminated is
    //      called.
    //
    //===========================================================================
    virtual void EvConnectionEstablished(IN ISceUserConfig* pUserConfig) = 0;

    //===========================================================================
    //==
    //==  EvErrorOnConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the observer that the persistent connection is in error.
    //
    //  Parameters:
    //    pUserConfig:
    //      The user config for which the persistent connection is in error.
    //
    //  Description:
    //      This event is called when an error is detected on a persistent
    //      connection. The observer must consider that the connection is no
    //      longer usable. EvConnectionEstablished will be called when the
    //      connection is no longer in error unless it is disabled in which case
    //      EvConnectionTerminated will be called instead.
    //
    //===========================================================================
    virtual void EvErrorOnConnection(IN ISceUserConfig* pUserConfig) = 0;

    //===========================================================================
    //==
    //==  EvConnectionTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the observer that the persistent connection is no longer
    //      used by the user config.
    //
    //  Parameters:
    //    pUserConfig:
    //      The user config for which the persistent connection is disabled.
    //
    //  Description:
    //      This event is called when the persistent connection is disabled for
    //      the user config. The observer should consider that the persistent
    //      connection is not usable. If the user config re-enable the
    //      connection, EvConnectionEstablished will be called unless the
    //      connection is in error in which case EvErrorOnConnection will be
    //      called instead.
    //
    //===========================================================================
    virtual void EvConnectionTerminated(IN ISceUserConfig* pUserConfig) = 0;


//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IScePersistentConnectionObserver() {}

    // Summary:
    //  Destructor.
    virtual ~IScePersistentConnectionObserver() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IScePersistentConnectionObserver(IN const IScePersistentConnectionObserver& rSrc);

    // Summary:
    //  Assignment Operator.
    IScePersistentConnectionObserver& operator=(IN const IScePersistentConnectionObserver& rSrc);


//-- Hidden Data Members
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_ISCEPERSISTENTCONNECTIONOBSERVER_H


