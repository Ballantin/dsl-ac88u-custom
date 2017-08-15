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
#ifndef MXG_ISCECORECONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCECORECONFIG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_SIPMETHODTYPES_H
#include "SceSipTypes/SipMethodTypes.h" // ESipMethodType
#endif

#ifndef MXG_SIPEVENTTYPES_H
#include "SceSipTypes/SipEventTypes.h" // ESipEventType
#endif

#ifndef MXG_SIPOPTIONTAGS_H
#include "SceSipTypes/SipOptionTags.h" // ESipOptionTag
#endif

#ifndef MXG_SIPMEDIATYPES_H
#include "SceSipTypes/SipMediaTypes.h" // ESipMediaType
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class CString;
class ISceRequestHandler;
class ISceShutdownMgr;
class ISceUser;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceCoreConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceCoreConfig
//========================================
//<GROUP TID_PACKAGE_SCECORE_CLASSES>
//
// Summary:
//  This interface defines the system-wide configuration options offered for the
//  SIP Client Engine (SCE).
//
// Description:
//  This interface defines the system-wide configuration options offered for the
//  SIP Client Engine (SCE).
//
//  To correctly configure the whole SIP Client Engine, the ISceCoreConfig must
//  be used along with the ISipCoreConfig from the M5T SIP stack.
//
//  #SIP Client Engine Startup#
//
//  The following sequence describes how an application can configure the SIP
//  Client Engine and the SIP stack for application startup:
//      - Create the SIP Client Engine configuration object using
//        CLSID_CSceCoreConfig and query ISceCoreConfig on it.
//      - Add request handlers if needed using RegisterRequestHandler. Default
//        request handler for the components compiled with the application are
//        already registered. This step is required only to register the
//        application's own components if necessary.
//      - Add user configuration object to be associated with each created
//        ISceUserConfig object using RegisterUserConfigObject. Again,
//        the SIP Client Engine automatically registers its own objects
//        according to the components compiled with the application. This step
//        is necessary only if the application wants to make available its own
//        configuration objects.
//      - Call ISceCoreConfig::Startup.
//      - Create the SIP Stack configuration object using
//        CLSID_CSipCoreConfig and query ISipCoreConfig on it.
//      - Configure one or more local addresses using
//        ISipCoreConfig::AddLocalAddress.
//      - Configure the listening sockets using ISipCoreConfig::ListenA.
//      - You may also wish to further configure the SIP stack after this
//        point using the available APIs of ISipCoreConfig. Most of the
//        available ISipCoreConfig APIs can be used at any time after
//        ISceCoreConfig::Startup has been called.
//
//  #SIP Client Engine Shutdown#
//      - Call ISceCoreConfig::ShutdownA.
//      - Stop the listening sockets using ISipCoreConfig::StopListeningA.
//      - If needed, remove any configured local addresses using
//        ISipCoreConfig::RemoveLocalAddress.
//
// #Restricted ISipCoreConfig APIs#
//  The following APIs from the ISipCoreConfig interface MUST NOT be used by the
//  application:
//      - ISipCoreConfig::Startup
//      - ISipCoreConfig::ShutdownA
//      - ISipCoreConfig::SetCoreThread
//      - ISipCoreConfig::SetKerberosThread
//      - ISipCoreConfig::SetTransportThread
//      - ISipCoreConfig::SetDnsResolverThread
//      - ISipCoreConfig::SetCoreUser
//      - ISipCoreConfig::SetDefaultDialogMatcherList
//
// Location:
//   SceCore/ISceCoreConfig.h
//
// See Also:
//   ISipCoreConfig
//
//==============================================================================
class ISceCoreConfig : public IEComUnknown
{
//-- Friend Declarations.

//-- Custom types.
public:
    //==========================================================================
    // Summary:
    //  Defines the signature for the startup and shutdown callbacks.
    //
    // Description:
    //  Some components might have to perform special operations at SIP Client
    //  Engine's startup or shutdown time. To do so, they can implement static
    //  methods that have the following signatures:
    //
    //  <CODE>
    //  static mxt_result StartupCallback();
    //  static void ShutdownCallback();
    //  </CODE>
    //
    //  And register those methods with the
    //  ISceCoreConfig::RegisterStartupShutdownCallbacks method.
    //
    // See also
    //  RegisterStartupShutdownCallbacks
    //==========================================================================
    typedef mxt_result (*mxt_PFNStartupCallback)();

    //<COMBINE mxt_PFNStartupCallback>
    typedef void (*mxt_PFNShutdownCallback)();

//-- Published Interface.
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceCoreConfig);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  Startup
    //==
    //==========================================================================
    //
    //  Summary:
    //      Starts the SIP Client Engine.
    //
    //  Returns:
    //      - resS_OK: The Engine is started.
    //      - resFE_INVALID_STATE: The Engine is already started.
    //      - resFE_FAIL: Failed to start the Engine, general initialization
    //        problem encountered.
    //
    //  Description:
    //      Initializes and starts the SIP Client Engine.
    //
    //      Registered startup callbacks are called after the standard
    //      synchronous startup process is over and only if it succeeded.
    //
    //  See Also:
    //      ShutdownA, RegisterStartupShutdownCallbacks
    //
    //==========================================================================
    virtual mxt_result Startup() = 0;

    //==========================================================================
    //==
    //==  ShutdownA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the SIP Client Engine.
    //
    //  Parameters:
    //    pMgr:
    //      Shutdown manager to which report the EvOnShutdownCompleted event.
    //      Can be NULL if the class initiating the shutdown does not wish
    //      to be notified of the event.
    //
    //    opqShutdown:
    //      Shutdown manager to which report the EvOnShutdownCompleted event.
    //      Can be NULL.
    //
    //  Returns:
    //      - resS_OK: The Engine is being terminated.
    //      - resFE_INVALID_STATE: The Engine was not started.
    //
    //  Description:
    //      Terminates the SIP Client Engine. Once shutdown is complete, an
    //      event is reported to the ISceShutdownMgr interface.
    //
    //      Registered shutdown callbacks are called before the standard
    //      asynchronous shutdown process starts.
    //
    //  #WARNING#
    //      If pMgr is NULL, the application MUST NOT finalize the SIP Client
    //      Engine until CSceCoreConfig::IsShutdownCompleted() returns true.
    //      Failure to do so could result in finalization problems making it
    //      impossible for the process to terminate correctly.
    //
    //  See Also:
    //      Startup, RegisterStartupShutdownCallbacks,
    //      CSceCoreCOnfig::IsShutdownCompleted
    //
    //==========================================================================
    virtual mxt_result ShutdownA(IN ISceShutdownMgr* pMgr,
                                 IN mxt_opaque opqShutdown) = 0;

    //==========================================================================
    //==
    //==  SetSceThread
    //==
    //==========================================================================
    // <FLAG Deprecated since v2.1.4>
    //
    //  Summary:
    //      Configures the main thread the SIP Client Engine must use.
    //
    //  Parameters:
    //    pThread:
    //      Pointer to a servicing thread object.
    //
    //  Returns:
    //      - resS_OK: Thread configured.
    //      - resFE_INVALID_STATE: Thread was already configured.
    //      - resFE_INVALID_ARGUMENT: The pThread parameter is invalid.
    //
    //  Description:
    //      Configures the servicing thread object the SIP Client Engine must
    //      use. Other threads may be internally created and managed by the SIP
    //      Client Engine.
    //
    //==========================================================================
    virtual mxt_result SetSceThread(IN IEComUnknown* pThread) = 0;

    //==========================================================================
    //==
    //==  SetSceThreads
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the core, transport, and the resolver threads used by the
    //      SIP Client Engine.
    //
    //  Parameters:
    //    pCoreThread:
    //      Pointer to a servicing thread object to set as the core thread.
    //      The thread object must be activated before calling SetSceThreads
    //      (see Activate method in IActivationSercice).
    //      If NULL, the thread will be created and activated automatically.
    //
    //    pTransportThread:
    //      Pointer to a servicing thread object to set as the thread used for
    //      transport. The thread object must be activated before calling
    //      SetSceThreads (see Activate method in IActivationSercice).
    //      If NULL, the thread will be created and activated automatically.
    //
    //    pResolverThread:
    //      Pointer to a servicing thread object that will be used for the DNS
    //      resolution mechanism. The thread object must be activated before
    //      calling SetSceThreads (see Activate method in IActivationSercice).
    //      If NULL, the thread will be created and activated automatically.
    //
    //  Returns:
    //      - resS_OK: Threads are configured.
    //      - resFE_UNEXPECTED: At least one thread was already configured.
    //      - resFE_INVALID_ARGUMENT: At least one pThread parameter is invalid.
    //
    //  Description:
    //      Configures the servicing thread objects the SIP Client Engine must
    //      use. Threads with NULL pointer in parameter are internally created
    //      and managed by the SIP Client Engine.
    //
    //  Warning:
    //      If this method fails then some threads created by this method might
    //      not be released. The SIP Client Engine could then be left in an
    //      unstable state and shutdown might not work.
    //
    //==========================================================================
    virtual mxt_result SetSceThreads(IN IEComUnknown* pCoreThread = NULL,
                                     IN IEComUnknown* pTransportThread = NULL,
                                     IN IEComUnknown* pResolverThread = NULL) = 0;

    //==========================================================================
    //==
    //==  GetCoreThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns a pointer to the core thread.
    //
    //  Parameters:
    //    ppThread:
    //      Address of a pointer to the IEComUnknown interface of the
    //      SCE core servicing thread.
    //
    //  Returns:
    //      - resS_OK: *ppThread contains a pointer to the core thread.
    //      - resFE_INVALID_ARGUMENT: pointer address in parameter is NULL.
    //
    //  Description:
    //      This method returns a pointer to the core thread. Make sure
    //      ReleaseIfRef is called upon the returned pointer, since internally
    //      AddIfRef has already been called.
    //
    //==========================================================================
    virtual mxt_result GetCoreThread(OUT IEComUnknown** ppThread) const = 0;

    //==========================================================================
    //==
    //==  GetTransportThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns a pointer to the transport thread.
    //
    //  Parameters:
    //    ppThread:
    //      Address of a pointer to the IEComUnknown interface of the SCE
    //      transport servicing thread.
    //
    //  Returns:
    //      - resS_OK: *ppThread contains a pointer to the transport thread.
    //      - resFE_INVALID_ARGUMENT: pointer address in parameter is NULL.
    //
    //  Description:
    //      This method returns a pointer to the transport thread. Make sure
    //      ReleaseIfRef is called upon the returned pointer, since internally
    //      AddIfRef has already been called.
    //
    //==========================================================================
    virtual mxt_result GetTransportThread(OUT IEComUnknown** ppThread) const = 0;

    //==========================================================================
    //==
    //==  GetResolverThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns a pointer to the resolver thread.
    //
    //  Parameters:
    //    ppThread:
    //      Address of a pointer to the IEComUnknown interface of the
    //      resolver servicing thread.
    //
    //  Returns:
    //      - resS_OK: *ppThread contains a pointer to the DNS resolving thread.
    //      - resFE_INVALID_ARGUMENT: pointer address in parameter is NULL.
    //
    //  Description:
    //      This method returns a pointer to the resolver thread. Make sure
    //      ReleaseIfRef is called upon the returned pointer, since internally
    //      AddIfRef has already been called.
    //
    //==========================================================================
    virtual mxt_result GetResolverThread(OUT IEComUnknown** ppThread) const = 0;

// BRCM_CUSTOM - [add] Add seed initialization
    //==========================================================================
    //==
    //==  SetSeed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configure the seed of the standard library's pseudo-random number
    //      generator.
    //
    //  Parameters:
    //    uSeed:
    //      Value used as seed by the generator.
    //
    //  Description:
    //      Configures the seed of the random number generator.
    //
    //==========================================================================
    virtual void SetSeed(IN unsigned int uSeed) = 0;
// BRCM_CUSTOM - [end] Add seed initialization

    //==========================================================================
    //==
    //==  RegisterRequestHandler
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a request handler.
    //
    //  Returns:
    //      - resS_OK: Request handler is configured.
    //      - resFE_INVALID_ARGUMENT: pHandler is invalid.
    //
    //  Parameters:
    //    pHandler:
    //      Pointer to a request handler ECOM object. A reference to this object
    //      is kept until ShutdownA is called.
    //
    //  Description:
    //      Registers a request handler. The request handlers are used to handle
    //      requests that did not match an existing dialog. An usual
    //      implementation of a request handler would be to have a look at the
    //      incoming type of request, create a component able to handle that
    //      request type, and then dispatch the request to the component for
    //      further processing.
    //
    //      A request handler is meant to be some form of component factory that
    //      creates a component based on the incoming request type.
    //
    //      The SIP Client Engine maintains a list of registered request
    //      handlers. The default request handlers are pre-registered at startup
    //      according to the components that were compiled with the SIP Client
    //      Engine. The application can register additional request handlers
    //      after Startup has been called. The SIP Client Engine always
    //      presents unmatched SIP requests to the request handlers in LIFO
    //      order. The last registered request handler gets to have a look at
    //      unmatched requests first. This allows an application to easily
    //      create its own handler that overrides part or the overall default
    //      behaviour of the SIP Client Engine handlers.
    //
    //  See Also:
    //      ISceRequestHandler
    //
    //==========================================================================
    virtual mxt_result RegisterRequestHandler(IN ISceRequestHandler* pHandler) = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  GetHandlerInterface
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the Client Engine handlers.
    //
    //  Parameters:
    //    iidRequested:
    //      The requested ECOM interface.
    //
    //    ppInterface:
    //      Double pointer to receive the pointer to the requested interface as
    //      an OUT parameter. Will return NULL if no such interface is supported
    //      by any of the handler.
    //
    //  Returns:
    //      - resS_OK: Requested interface found and returned in ppInterface.
    //      - resFE_FAIL: Requested interface not found. ppInterface also
    //                    returns NULL.
    //      - resFE_INVALID_ARGUMENT: iidRequested or ppInterface == NULL.
    //
    //  Description:
    //      This allows the user of the M5T SIP Client Engine to access a
    //      specific handler. The handlers in this case are parts of the Client
    //      Engine that are responsible for handling Out of Dialog SIP requests.
    //      Upon handling such a packet for which it is responsible, the handler
    //      usually creates a new component to manage the request.
    //
    //      The user of the M5T SIP Client Engine must access the handlers in
    //      order to configure them with a manager, thus allowing the user to
    //      know when a new component for handling a request has been created.
    //
    //      A reference is automaticaly added on success. The user is
    //      responsible to release it when this interface is no longer required.
    //
    //==========================================================================
    virtual mxt_result GetHandlerInterface(IN mxt_iid iidRequested,
                                           OUT void** ppInterface) const = 0;
//M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  GetHandlerInterface
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the Client Engine handlers.
    //
    //  Parameters:
    //    ppInterface:
    //      Double pointer to receive the pointer to the requested interface as
    //      an OUT parameter. Will return NULL if no such interface is supported
    //      by any of the handler.
    //
    //  Returns:
    //      - resS_OK: Requested interface found and returned in ppInterface.
    //      - resFE_FAIL: Requested interface not found. ppInterface also
    //                    returns NULL.
    //      - resFE_INVALID_ARGUMENT: ppInterface == NULL.
    //
    //  Description:
    //      This allows the user of the M5T SIP Client Engine to access a
    //      specific handler. The handlers in this case are parts of the Client
    //      Engine that are responsible for handling Out of Dialog SIP requests.
    //      Upon handling such a packet for wihch it is responsible, the handler
    //      usually creates a new component to manage the request.
    //
    //      The user of the M5T SIP Client Engine must access the handlers in
    //      order to configure them with a manager, thus allowing the user to
    //      know when a new component for handling a request has been created.
    //
    //      A reference is automaticaly added on success. The user is
    //      responsible to release it when this interface is no longer required.
    //
    //==========================================================================
    template<class _Type> mxt_result GetHandlerInterface(OUT _Type** ppInterface)
    {
        // Has to be inlined here because Visual C++ 6.0 does not like out of class
        // inlined specialized template method.
        if (ppInterface != NULL)
        {
            return GetHandlerInterface((*ppInterface)->GetIID(),
                                       reinterpret_cast<void**>(ppInterface));
        }
        return resFE_INVALID_ARGUMENT;
    }

    //==========================================================================
    //==
    //==  RegisterUserConfigObject
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a user configuration object.
    //
    //  Returns:
    //      - resS_OK: User configuration object is configured.
    //      - resFE_INVALID_ARGUMENT: classId is NULL.
    //      - resFE_DUPLICATE: classId already registered.
    //
    //  Parameters:
    //    classId:
    //      The class ID of the object to create. Cannot be NULL.
    //
    //  Description:
    //      Registers a configuration object that must be aggregated by any
    //      ISceUserConfig created. The new configuration object will be
    //      available through a standard QueryIf on any ISceUserConfig
    //      instance.
    //
    //      Some components rely on such user configuration interfaces
    //      being available from the ISceUserConfig in order to get their
    //      configuration. An application creating its own components could also
    //      want to maintain user configuration state in such an object that is
    //      automatically instantiated along with all created
    //      ISceUserConfig objects.
    //
    //      A list of default configuration objects will be registered according
    //      to the M5T components that are compiled with the SIP Client Engine.
    //
    //  See Also:
    //      ISceUserConfig
    //
    //==========================================================================
    virtual mxt_result RegisterUserConfigObject(IN mxt_clsid classId) = 0;

//M5T_INTERNAL_USE_BEGIN

    //==========================================================================
    //==
    //==  RegisterUser
    //==
    //==========================================================================
    //
    //  Summary:
    //    Registers a user in the user list.
    //
    //  Parameters:
    //    pUser:
    //      Pointer to the user to register. Must not be NULL.
    //
    //  Returns:
    //      - resS_OK: User is registered.
    //      - resFE_INVALID_ARGUMENT: pUser is invalid.
    //      - resFE_DUPLICATE: User was already registered.
    //
    //  Description:
    //      Registers a user to the list of users.
    //
    //  See Also:
    //      UnregisterUser
    //
    //==========================================================================
    virtual mxt_result RegisterUser(IN ISceUser* pUser) = 0;

    //==========================================================================
    //==
    //==  UnregisterUser
    //==
    //==========================================================================
    //
    //  Parameters:
    //    pUser:
    //      Pointer to a previously registered user. Must not be NULL.
    //
    //  Returns:
    //      - resS_OK: User is unregistered.
    //      - resFE_INVALID_ARGUMENT: pUser is invalid or was not previously
    //        registered.
    //
    //  Description:
    //      Unregisters a previously registered user.
    //
    //  See Also:
    //      RegisterUser
    //
    //==========================================================================
    virtual mxt_result UnregisterUser(IN ISceUser* pUser) = 0;
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  RegisterSupportedMethod
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a supported method.
    //
    //  Parameters:
    //    eMethod:
    //      Method to register.
    //
    //  Description:
    //      Registers a method as supported by the SIP Client Engine. This
    //      globally registers the method for all users of the SIP Client
    //      Engine.
    //
    //      A set of methods is pre-registered by the components compiled with
    //      the SIP Client Engine.
    //
    //      The registered methods are then used by the components in Allow
    //      headers.
    //
    //  See Also:
    //      RegisterSupportedExtension,
    //      RegisterSupportedPayloads,
    //      RegisterSupportedEvents,
    //      GetSupportedMethods
    //
    //==========================================================================
    virtual void RegisterSupportedMethod(IN ESipMethodType eMethod) = 0;

    //==========================================================================
    //==
    //==  RegisterSupportedExtension
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a supported extension.
    //
    //  Parameters:
    //    eTag:
    //      Option tag to register.
    //
    //  Description:
    //      Registers a SIP extension as supported by the SIP Client Engine.
    //      This globally registers the extension for all users of the SIP
    //      Client Engine.
    //
    //      A set of option tags is pre-registered by the components compiled
    //      with the SIP Client Engine.
    //
    //      The registered extensions are then used by the components in
    //      Supported headers and to find the list of extensions that are
    //      supported by two peers when a dialog is established.
    //
    //  See Also:
    //      RegisterSupportedMethod,
    //      RegisterSupportedPayloads,
    //      RegisterSupportedEvents,
    //      GetSupportedExtenisons
    //
    //==========================================================================
    virtual void RegisterSupportedExtension(IN ESipOptionTag eTag) = 0;

    //==========================================================================
    //==
    //==  RegisterSupportedPayload
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a supported payload.
    //
    //  Parameters:
    //    eMediaType:
    //      Media type to register.
    //
    //  Description:
    //      Registers a payload as supported by the SIP Client Engine. This
    //      globally registers the support of the payload for all users of the
    //      SIP Client Engine.
    //
    //      A set of payloads is pre-registered by the components compiled with
    //      the SIP Client Engine.
    //
    //      The registered payloads are then used by the components in Accept
    //      headers and to reject requests with an unrecognized payload type.
    //
    //  See Also:
    //      RegisterSupportedMethod,
    //      RegisterSupportedExtension,
    //      RegisterSupportedEvents,
    //      GetSupportedPayloads
    //
    //==========================================================================
    virtual void RegisterSupportedPayload(IN ESipMediaType eMediaType) = 0;

    //==========================================================================
    //==
    //==  RegisterSupportedEvent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a supported event.
    //
    //  Parameters:
    //    eEvent:
    //      Event type to register.
    //
    //  Description:
    //      Registers a SIP event as supported by the SIP Client Engine. This
    //      means that the SIP Client Engine can process incoming
    //      SUBSCRIBE requests for that specific event type. If the SIP Client
    //      Engine is just sending SUBSCRIBE requests for a specific event type,
    //      but never able to receive such SUBSCRIBE, then it does not need to
    //      register its support. This globally registers the support of the
    //      event for all users of the SIP Client Engine.
    //
    //      A set of events is pre-registered by the components compiled with
    //      the SIP Client Engine.
    //
    //      The registered events are then used by the components in Allow-Event
    //      headers.
    //
    //  See Also:
    //      RegisterSupportedMethod,
    //      RegisterSupportedExtension,
    //      RegisterSupportedPayloads,
    //      GetSupportedEvents
    //
    //==========================================================================
    virtual void RegisterSupportedEvent(IN ESipEventType eEvent) = 0;

    //==========================================================================
    //==
    //==  GetSupportedMethods
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the list of supported SIP methods.
    //
    //  Returns:
    //      A bitset encoding of all supported methods.
    //
    //  Description:
    //      Returns the list of supported SIP methods by the SIP Client Engine.
    //      The list is encoded as a bitset of ESipMethodType values.
    //
    //  See Also:
    //      RegisterSupportedMethod,
    //      ESipMethodType
    //
    //==========================================================================
    virtual uint32_t GetSupportedMethods() const = 0;

    //==========================================================================
    //==
    //==  GetSupportedExtensions
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the list of supported SIP extensions.
    //
    //  Returns:
    //      A bitset encoding of all supported extensions.
    //
    //  Description:
    //      Returns the list of supported SIP extensions by the SIP Client
    //      Engine. The list is encoded as a bitset of ESipOptionTag values.
    //
    //  See Also:
    //      RegisterSupportedExtension, ESipOptionTag
    //
    //==========================================================================
    virtual uint32_t GetSupportedExtensions() const = 0;

    //==========================================================================
    //==
    //==  GetSupportedPayloads
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the list of supported SIP payloads.
    //
    //  Returns:
    //      A bitset encoding of all supported payloads.
    //
    //  Description:
    //      Returns the list of supported SIP payloads by the SIP Client
    //      Engine. The list is encoded as a bitset of ESipMediaType values.
    //
    //  See Also:
    //      RegisterSupportedPayloads, ESipMediaType
    //
    //==========================================================================
    virtual uint32_t GetSupportedPayloads() const = 0;

    //==========================================================================
    //==
    //==  GetSupportedEvents
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the list of supported SIP events.
    //
    //  Returns:
    //      A bitset encoding of all supported events.
    //
    //  Description:
    //      Returns the list of supported SIP events by the SIP Client Engine.
    //      The list is encoded as a bitset of ESipEventType values.
    //
    //  See Also:
    //      RegisterSupportedEvents, ESipEventType
    //
    //==========================================================================
    virtual uint32_t GetSupportedEvents() const = 0;

    //==========================================================================
    //==
    //==  GenerateInstanceId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Generates a unique instance identifier.
    //
    //  Parameters:
    //    rstrMacAddress:
    //      MAC address of the network interface used.
    //
    //    uTime:
    //      Current time.
    //
    //    rstrInstanceId:
    //      Generated instance identifier.
    //
    //  Description:
    //      Generates a unique instance identifier (urn:uuid).
    //
    //==========================================================================
    virtual void GenerateInstanceId(IN const CString& rstrMacAddress,
                                    IN uint64_t uTime,
                                    OUT CString& rstrInstanceId) = 0;

    //==========================================================================
    //==
    //==  RegisterStartupShutdownCallbacks
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers callbacks to call in the SIP Client Engine's startup or
    //      shutdown process.
    //
    //  Parameters:
    //    pStartup:
    //      Pointer to a startup callback. May be NULL.
    //
    //    pShutdown:
    //      Pointer to a shutdown callback. May be NULL.
    //
    //  Description:
    //      This method allows a component to register callbacks to perform
    //      custom steps during the SIP Client Engine's startup or shutdown
    //      process. While it may seem illogical for a component to perform
    //      steps at startup and none at shutdown or the other way around, it is
    //      allowed to leave one of the parameters NULL. This is for
    //      extensibility.
    //
    //      Callback registration should be done at static initialization time
    //      but as long as it is done before calling ISceCoreConfig::Startup,
    //      the callbacks will be used.
    //
    //      The registered startup callbacks will be called in order of
    //      registration and only once all the SIP Client Engine's normal
    //      startup steps have been performed successfully.
    //
    //      The registered shutdown callbacks will be called in reverse order of
    //      registration, if the corresponding startup step has succeeded or if
    //      no startup callback was provided and before all the SIP Client
    //      Engine's standard shutdown steps.
    //
    //      There is no limit on the number of callbacks that can be registered
    //      by one or more components. This means that a single component may
    //      register more than one set of callbacks.
    //
    //      Once a callbacks set is registered, it cannot be unregistered and
    //      it is kept until the static finalization of the SIP Client Engine.
    //
    //  Warning:
    //      The SIP Client Engine's standard shutdown process is asynchronous
    //      as explained in ISceCoreConfig::ShutdownA, but the shutdown callbacks
    //      are NOT. They are called synchronously from the ShutdownA method.
    //      This means that upon a successful return from ShutdownA,
    //      the shutdown callbacks have already been executed. The application
    //      should no longer use the SIP Client Engine's components after
    //      returning from ShutdownA and only wait for the
    //      ISceShutdownMgr::EvOnShutdownCompleted event.
    //
    //  See Also:
    //   Startup, ShutdownA
    //
    //==========================================================================
    virtual void RegisterStartupShutdownCallbacks(IN mxt_PFNStartupCallback pStartup,
                                                  IN mxt_PFNShutdownCallback pShutdown) = 0;

// BRCM_CUSTOM - [add] Remove from supported header
    //==========================================================================
    //==
    //==  RemoveSupportedExtension
    //==
    //==========================================================================
    //
    //  Summary:
    //      Remove an item from the supported header.
    //
    //  Parameters:
    //    eTag:
    //      Item to be removed.
    //
    //  Description:
    //      Remove an item from the supported header.
    //
    //==========================================================================
    virtual void RemoveSupportedExtension(IN ESipOptionTag eTag) = 0;
// BRCM_CUSTOM - [end] Remove from supported header

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceCoreConfig() {}

    // Summary:
    //  Destructor.
    virtual ~ISceCoreConfig() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceCoreConfig(IN const ISceCoreConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceCoreConfig& operator=(IN const ISceCoreConfig& rSrc);


//-- Hidden Data Members
protected:
private:
//M5T_INTERNAL_USE_END

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCECORECONFIG_H
