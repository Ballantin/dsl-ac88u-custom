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
#ifndef MXG_IUAIMPPPRESENCEPUBLISHER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUAIMPPPRESENCEPUBLISHER_H

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
class CSipMessageBody;
class IScePidfDocument;
class ISceUserConfig;
class IUaImppPresencePublisherMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IUaImppPresencePublisher);

//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaImppPresencePublisher
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//  This interface defines the API of the presence publisher component.
//
// Description:
//  This interface defines the API of the presence publisher component. This
//  component manages the publication of the presence state of a user to a
//  presence server. This publication corresponds to the user's soft-state, or
//  the frequently changing presence state of the user. The soft-state is
//  maintained on the presence server using periodic SIP PUBLISH requests. The
//  soft-state will be removed from the presence server whenever the component
//  stops refreshing it.
//
//  This interface must be used in the following manner:
//    - After setting the manager and the configuration object, the
//      application must properly configure the presence state of the user by
//      using the PIDF document returned by GetPidfDocument.
//
//    - Once the document is properly set with the user's state and
//      information, the application must call PublishPresenceState to publish
//      the initial presence state on the server.
//
//    - Whenever the presence state of the user is changed, the application
//      must reflect this change in the PIDF document returned by
//      GetPidfDocument and then it must again call PublishPresenceState to
//      inform the server of the state change.
//
//    - When the application no longer wants to publish the user's presence
//      state, it must call Terminate and wait for the
//      IUaImppPresencePublisherMgr::EvPublicationStatus(eTerminated) event.
//      After this event is received, the application can safely release the
//      component.
//
//  The publisher component will automatically perform publication refreshes
//  whenever it is necessary. It will also retry to send publication requests
//  whenever a preceeding request has failed. However, if the component finds
//  that it will never be able to successfully publish the presence state, it
//  will report IUaImppPresencePublisherMgr::EvPublicationStatus(EFailed).
//
//  An application may use the UA-IMPP PIDF Document Control Component to manage
//  the hard-state of the user that must persist on the persence server even if
//  it is not refreshed.
//
// Location:
//   SceUaImpp/IUaImppPresencePublisher.h
//
// See Also:
//   IUaImppPresencePublisherMgr, IUaImppPresencePublisherConfig
//
//==============================================================================
class IUaImppPresencePublisher : public IEComUnknown
{
//-- Friend Declarations


//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaImppPresencePublisher);
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
    //      resS_OK: Manager set correctly, an error otherwise.
    //
    //  Description:
    //      Configures the manager to which this object reports events.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IUaImppPresencePublisherMgr* pMgr) = 0;


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
    //      resS_OK: Configuration correctly set, an error otherwise.
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
    //      Sets an opaque paramerer to the presence publisher.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the presence publisher. It can be
    //      used by the application to store any information.
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
    //      Gets the opaque parameter to the presence publisher.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the presence publisher. It can be
    //      used by the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;


    //==SDOC====================================================================
    //==
    //==  GetPidfDocument
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gives access to the PIDF document that will be used to track the
    //      user's presence and state.
    //
    //  Parameters:
    //    ppDocument:
    //      Double pointer to a PIDF document. Will be set to point to an
    //      IScePidfDocument upon success. A reference will be counted for the
    //      returned ECOM object if set to a non-NULL pointer.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Gives access to the PIDF document that will be used to track the
    //      user's presence and state.
    //
    //      The application must use this PIDF document to modify the presence
    //      state of the user.
    //
    //==EDOC====================================================================
    virtual mxt_result GetPidfDocument(INOUT IScePidfDocument** ppDocument) = 0;


    //==========================================================================
    //==
    //==  PublishPresenceState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Publishes or refreshes the presence state of the user.
    //
    //  Parameters:
    //    bForceFullState:
    //      This parameter allows to force a "full-state" publication when
    //      the publication mode is eMODE_PARTIAL.
    //      When the publication mode is eMODE_FULL, the value of is parameter
    //      is ignored.
    //
    //  Returns:
    //      resS_OK upon success, an error otherwise.
    //
    //  Description:
    //      Publishes or sends a refresh of the presence state of the user
    //      using the current PIDF document.
    //
    //      This method must be used for the initial publication and whenever
    //      the application wants to update its presence state on the server.
    //
    //  See Also:
    //      Terminate
    //
    //==========================================================================
    virtual mxt_result PublishPresenceState(bool bForceFullState = false) = 0;


    //==========================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the publication of the presence state.
    //
    //  Returns:
    //      resS_OK: Terminate successful, an error otherwise.
    //
    //  Description:
    //      Terminates the publication of the presence state. The application
    //      must wait for the
    //      IUaImppPresencePublisherMgr::EvPublicationStatus(eTerminated) event
    //      before releasing this component.
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
    //      Aborts the publication of the presence state.
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
    //      presence state is not valid anymore.
    //
    //      The application MUST wait for the
    //      IUaImppPresencePublisherMgr::EvPublicationStatus(eTerminated) event
    //      before releasing this component.
    //
    //==========================================================================
    virtual mxt_result Abort() = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaImppPresencePublisher(){}

    // Summary:
    //  Destructor.
    virtual ~IUaImppPresencePublisher(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaImppPresencePublisher(IN const IUaImppPresencePublisher& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaImppPresencePublisher& operator=(IN const IUaImppPresencePublisher& rSrc);


//-- Hidden Data Members
protected:
private:

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUAIMPPPRESENCEPUBLISHER_H

