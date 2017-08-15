//==============================================================================
//==============================================================================
//
//              Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCEPUBLISHER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEPUBLISHER_H

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

#ifndef MXG_SIPEVENTTYPES_H
#include "SceSipTypes/SipEventTypes.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CHeaderList;
class CNameAddr;
class CSceCallerPreferences;
class CSipMessageBody;
class IScePublisherMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IScePublisher);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IScePublisher
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//  This interface can be used to publish data about any event type.
//
// Description:
//  The publisher component can be used to publish data for any type of
//  events. The user can use the Publish() API to publish data about a single
//  event type, configurable using the SetEventType method. The publisher
//  component will use EvPublicationStatus to report the state of the
//  publication.
//
//  The publisher component will automatically refresh and republish upon
//  failure unless it returns eFAILED (unrecoverable error),
//  eUSER_TERMINATED (all dialogs from this user are being terminated) or
//  eTERMINATED (user initiated termination) in
//  IScePublisherMgr::EvPublicationStatus, in which case the component should
//  be released.
//
// Location:
//   SceCoreComponents/IScePublisher.h
//
// See Also:
//   IScePublisherMgr, IScePublisherConfig
//
//==============================================================================
class IScePublisher : public IEComUnknown
{
//-- Friend Declarations


//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IScePublisher);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which this object reports events.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      resS_OK if the Manager is set correctly, an error otherwise.
    //
    //  Description:
    //      Configures the manager to which this object reports events.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IScePublisherMgr* pMgr) = 0;


    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures this object with a user configuration object.
    //
    //  Parameters:
    //      pConfig:
    //       Pointer to the ECOM configuration object for this user. It must not
    //       be NULL.
    //
    //  Returns:
    //      resS_OK if the configuration is correctly set, an error otherwise.
    //
    //  Description:
    //      Configures this object with a user configuration object.
    //
    //==========================================================================
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig) = 0;

    //==========================================================================
    //==
    //==  GetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the component's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the component.
    //
    //==========================================================================
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig) = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque parameter to the publisher.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the publisher. It can be used by the
    //      application to store any information.
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the opaque parameter to the publisher.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the publisher. It can be used by the
    //      application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //==
    //==  SetEventType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the event type this object will be issuing publication
    //      data for.
    //
    //  Parameters:
    //      eEventType:
    //       The event type of the publication.
    //
    //  Returns:
    //      resS_OK if the event type is correctly set, an error otherwise.
    //
    //  Description:
    //      Configures the event type this object will be issuing publication
    //      data for. This MUST be set prior to starting to publish data.
    //
    //==========================================================================
    virtual mxt_result SetEventType(ESipEventType eEventType) = 0;

    //==========================================================================
    //==
    //==  GetEventType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured event type.
    //
    //  Returns:
    //      The event type.
    //
    //  Description:
    //      Gets the configured event type.
    //
    //==========================================================================
    virtual ESipEventType GetEventType() = 0;

    //==========================================================================
    //==
    //==  SetPeerAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the peer address this object will be issuing publication
    //      data to.
    //
    //  Parameters:
    //      rPeerAddr:
    //       The peer address to send the publication to.
    //
    //  Returns:
    //      resS_OK if the peer address is correctly set, an error otherwise.
    //
    //  Description:
    //      Configures the peer address this object will be issuing publication
    //      data to. This MUST be set prior to starting to publish data.
    //
    //==========================================================================
    virtual mxt_result SetPeerAddr(IN const CNameAddr& rPeerAddr) = 0;

    //==========================================================================
    //==
    //==  GetPeerAddr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the configured peer address.
    //
    //  Returns:
    //      The peer address.
    //
    //  Description:
    //      Gets the configured peer address.
    //
    //==========================================================================
    virtual mxt_result GetPeerAddr(OUT CNameAddr& rPeerAddr) = 0;

    //==========================================================================
    //==
    //==  SetPublicationRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the publication retry delay this object will be using
    //      when a publication needs to be retried.
    //
    //  Parameters:
    //      uNbSeconds:
    //       The number of seconds the publisher will wait before retrying an
    //       unsuccessful publication.
    //
    //  Description:
    //      Configures the publication retry delay this object will be using
    //      when a publication needs to be retried.
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
    //      The retry delay.
    //
    //  Description:
    //      Gets the configured publication retry delay.
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
    //      Configures the publication expiration time.
    //
    //  Parameters:
    //      uNbSeconds:
    //       The number of seconds a successful publication will remain active.
    //
    //  Description:
    //      Configures the publication expiration time.
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
    //      The expiration time.
    //
    //  Description:
    //      Gets the configured publication expiration time.
    //
    //==========================================================================
    virtual uint32_t GetPublicationExpirationTime() const = 0;

    //==========================================================================
    //==
    //==  Publish
    //==
    //==========================================================================
    //
    //  Summary:
    //      Publishes or refreshes data on the server.
    //
    //  Parameters:
    //    opqTransaction:
    //      An opaque data meant for the manager to identify the transaction in
    //      progress. Currently unused.
    //
    //    pExtraHeaders:
    //      The extra headers to send along with the PUBLISH request.
    //      Ownership is always taken.
    //
    //    pMessageBody:
    //      The message body to send along with the PUBLISH request.
    //      Ownership is always taken.
    //
    //    pCallerPreferences:
    //      The caller preferences to send along with the PUBLISH request.
    //      Ownership is always taken.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Publishes or sends a refresh of the data to the server.
    //
    //      This method must be used for the initial publication and whenever
    //      the application wants to update its data on the server.
    //
    //  See Also:
    //      Terminate
    //
    //==========================================================================
    virtual mxt_result Publish(IN mxt_opaque opqTransaction,
                               IN TOA CHeaderList* pExtraHeaders,
                               IN TOA CSipMessageBody* pMessageBody,
                               IN TOA CSceCallerPreferences* pCallerPreferences) = 0;

    //==========================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the publication.
    //
    //  Returns:
    //      resS_OK if the termination is successfully started, an error
    //      otherwise.
    //
    //  Description:
    //      Terminates the publication gracefully. The application must wait for
    //      the IScePublisherMgr::EvPublicationStatus(eTerminated) event before
    //      releasing this component.
    //
    //==========================================================================
    virtual mxt_result Terminate() = 0;

    //==========================================================================
    //==
    //==  Abort
    //==
    //==========================================================================
    //
    //  Summary:
    //      Aborts the publication.
    //
    //  Returns:
    //      resS_OK: Aborting. Please note that
    //               IScePublisherMgr::EvPublicationStatus(eTerminated) might be
    //               called before this method returns.
    //
    //      resFE_INVALID_STATE: This object is already terminated.
    //
    //  Description:
    //      Abruptly terminates this object. The server is not notified that the
    //      published data is not valid anymore. The application MUST wait for
    //      the IScePublisherMgr::EvPublicationStatus(eTerminated) event before
    //      releasing this component.
    //
    //==========================================================================
    virtual mxt_result Abort() = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IScePublisher(){}

    // Summary:
    //  Destructor.
    virtual ~IScePublisher(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IScePublisher(IN const IScePublisher& rSrc);

    // Summary:
    //  Assignment Operator.
    IScePublisher& operator=(IN const IScePublisher& rSrc);


//-- Hidden Data Members
protected:
private:

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEPUBLISHER_H

