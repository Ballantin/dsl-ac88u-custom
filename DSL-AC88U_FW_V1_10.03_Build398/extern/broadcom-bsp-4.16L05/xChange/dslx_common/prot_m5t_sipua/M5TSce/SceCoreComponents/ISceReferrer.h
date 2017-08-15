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
#ifndef MXG_ISCEREFERRER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEREFERRER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCECORECOMPONENTSCFG_H
#include "Config/SceCoreComponentsCfg.h"
#endif


#if !defined(MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT)
#error You must define MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT to be able to \
use this class.
#endif


#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CNameAddr;
class CSceCallerPreferences;
class CSipPacket;
class ISceReferrerMgr;
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceReferrer);
//M5T_INTERNAL_USE_END


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceReferrer
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Description:
//  The referrer component allows the user to send and receive REFER requests
//  that do not match an existing dialog. The REFER request creates its own
//  dialog and is thus separated from other dialogs.
//
//  An instance of this component can only be used for either sending or
//  receiving a REFER. It cannot perform both at the same time.
//
//#Sending a REFER#
//  This is done using any of the Refer methods. After a REFER is sent, the user
//  must expect ISceReferrerMgr::EvReferStatus to report whether or not the
//  request was accepted. If the request was accepted, the component may report
//  zero or more progress statuses through ISceReferrerMgr::EvReferProgress.
//  These statuses represent the progression of the request that was sent by the
//  peer as a result of the reception of the REFER request.
//
//  ISceReferrerMgr::EvTerminated can be reported at any time after
//  ISceReferrerMgr::EvReferStatus is reported. EvTerminated is always the last
//  event reported and the application must thus release its reference on this
//  component's instance as it no longer can be used.
//
// #Receiving a REFER#
//  The first event reported can either be ISceReferrerMgr::EvRefer or
//  ISceReferrerMgr::EvTerminated.
//
//  If ISceReferrerMgr::EvTerminated is reported first, the user of the
//  component has nothing else to do than to release the component. This can
//  happen when the request was received and later found to be invalid.
//
//  If ISceReferrerMgr::EvRefer is reported first, the user must then choose to
//  proceed or not with this REFER request and call AcceptRefer or RejectRefer.
//
//  If RejectRefer is called, the component will report
//  ISceReferrerMgr::EvTerminated and the user can release its reference to the
//  component.
//
//  If AcceptRefer is called, then the user must proceed with the REFER request
//  and send the proper request to the peer identified through the rReferTo
//  parameter of EvRefer. The statuses of that request must then be relayed
//  through NotifyReferProgress. When the user knows that it has received a
//  final status and the request will progress no more, it must set the bFinal
//  parameter of NotifyReferProgress to true, otherwise it should be false.
//  ISceReferrerMgr::EvTerminated will be reported immediately after
//  NotifyReferProgress is called with bFinal set to true.
//
//  Note that ISceReferrerMgr::EvTerminated can be reported at any time after
//  AcceptRefer is called, even before NotifyReferProgress is called with bFinal
//  set to true. This can happen under different conditions, like if the
//  implicit subscription created with the REFER request is expired or if the
//  peer sends back an error to a NOTIFY request.
//
// Location:
//   SceCoreComponents/ISceReferrer.h
//
// See Also:
//   ISceReferrerMgr, ISceReferHandler
//
//==============================================================================
class ISceReferrer : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceReferrer);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this
    //      referrer.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //         pMgr is NULL.
    //      - resS_OK:
    //         The manager is set.
    //
    //  Description:
    //      Configures the manager to which events are reported for this
    //      referrer.
    //
    //      IMPORTANT: This method MUST be called before a refer process is
    //                 initiated.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN ISceReferrerMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the referrer with a configuration object.
    //
    //  Parameters:
    //      pConfig:
    //       Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //         pConfig is NULL or the ISceUserConfig interface cannot be queried
    //         from pConfig.
    //      - resFE_INVALID_STATE:
    //         A configuration object has already been set.
    //      - resS_OK:
    //         The configuration object has been set.
    //
    //  Description:
    //      Configures the referrer with a configuration object.
    //
    //      IMPORTANT: This method MUST be called before a refer process is
    //                 initiated.
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
    //      Gets the referrer's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the referrer.
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
    //      Sets an opaque paramerer to the referrer.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the referrer. It can be used by the
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
    //      Gets the opaque parameter to the referrer.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the referrer. It can be used by
    //      the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //  The possible behaviour of the component when receiving an invalid REFER.
    //
    // Description:
    //  The possible behaviour of the component when receiving a REFER request
    //  containing either no Target-Dialog header or an invalid Target-Dialog
    //  header.
    //
    //==========================================================================
    enum EInvalidReferBehaviour
    {
        // Description:
        //  The REFER will be answered with a 202.
        eDEFAULT_BEHAVIOUR,
        // Description:
        //  The REFER will be rejected with a 481.
        eREJECT_WITH_481
    };

    //==========================================================================
    //==
    //==  SetInvalidTargetDialogReferBehaviour
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the behaviour of the component when an invalid REFER request
    //      is received.
    //
    //  Parameters:
    //      eBehaviour:
    //       The behaviour to use.
    //
    //  Description:
    //      Sets the behaviour of the component when a REFER request with an
    //      invalid or no Target-Dialog header is received.
    //
    //==========================================================================
    virtual void SetInvalidTargetDialogReferBehaviour(IN EInvalidReferBehaviour eBehaviour) = 0;


    //==========================================================================
    //==
    //==  Refer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a REFER request to a peer with which a dialog is already
    //      established.
    //
    //  Parameters:
    //    pTargetDialog:
    //      Pointer to the component that manages the dialog with the peer to
    //      which the REFER must be sent. Cannot be NULL.
    //
    //    pReplacedDialog:
    //      Pointer to the component that manages the dialog with a third-party
    //      peer that is to be replaced by the dialog issuing from handling the
    //      REFER request by the peer. Cannot be NULL.
    //
    //    bEarlyOnly:
    //      Boolean to specify if the replaced call can be replaced only if it
    //      is in early dialog state.
    //
    //    pCallerPreferences:
    //      The caller preferences to add to the outgoing request. Ownership is
    //      is taken. May be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_STATE:
    //          - A REFER is already handled by the component.
    //          - No manager is set.
    //      - resFE_INVALID_ARGUMENT:
    //          - pReplacedDialog is NULL.
    //          - Failed to get the ISipUserAgentSvc from pReplacedDialog.
    //          - pReplacedDialog is not in a dialog.
    //          - The current target of pReplacedDialog is not sip or sips.
    //          - pTargetDialog is NULL.
    //          - Failed to get the ISipUserAgentSvc from pTargetDialog.
    //          - pTargetDialog is not in a dialog.
    //      - resFE_FAIL:
    //          - Failed to create or configure the ISipContext.
    //      - resS_OK:
    //          - The REFER is about to be sent.
    //
    //  Description:
    //      Sends a REFER request to a peer with which a dialog is already
    //      established. This version of Refer can be used to have the peer
    //      handling the REFER request replace a dialog established between the
    //      referrer and a 3rd party peer.
    //
    //      pTargetDialog is used to build the Target-Dialog SIP header, which
    //      identifies which dialog is to be affected by the REFER request.
    //
    //      pReplacedDialog is used to build the Refer-To header along with the
    //      Replaces header included in it.
    //
    //==========================================================================
    virtual mxt_result Refer(IN IEComUnknown* pTargetDialog,
                             IN IEComUnknown* pReplacedDialog,
                             IN bool bEarlyOnly,
                             IN TOA CSceCallerPreferences* pCallerPreferences) = 0;

    //==========================================================================
    //==
    //==  Refer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a REFER request to a peer with which a dialog is already
    //      established.
    //
    //  Parameters:
    //    pTargetDialog:
    //      Pointer to the component that manages the dialog with the peer to
    //      which the REFER must be sent. Cannot be NULL.
    //
    //    rDestinationAddr:
    //      The address of the 3rd party peer where the peer receiving the REFER
    //      (ie. referee) should send the request.
    //
    //    pCallerPreferences:
    //      The caller preferences to add to the outgoing request. Ownership is
    //      is taken. May be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_STATE:
    //          - A REFER is already handled by the component.
    //          - No manager is set.
    //      - resFE_INVALID_ARGUMENT:
    //          - pTargetDialog is NULL.
    //          - Failed to get the ISipUserAgentSvc from pTargetDialog.
    //          - pTargetDialog is not in a dialog.
    //      - resFE_FAIL:
    //          - Failed to create or configure the ISipContext.
    //      - resS_OK:
    //          - The REFER is about to be sent.
    //
    //  Description:
    //      Sends a REFER request to a peer with which a dialog is already
    //      established. This version of Refer is used to send a REFER without
    //      having a Replaces header in the Refer-To header.
    //
    //      pTargetDialog is used to build the Target-Dialog SIP header, which
    //      identifies which dialog is to be affected by the REFER request.
    //
    //==========================================================================
    virtual mxt_result Refer(IN IEComUnknown* pTargetDialog,
                             IN const CNameAddr& rDestinationAddr,
                             IN TOA CSceCallerPreferences* pCallerPreferences) = 0;

    //==========================================================================
    //==
    //==  AcceptRefer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Accept to process an incoming REFER request.
    //
    //  Returns:
    //      - resFE_INVALID_STATE:
    //          - component is not acting as a referee.
    //          - component is already terminating.
    //      - resFE_FAIL:
    //          - failed to send the NOTIFY.
    //      - resS_OK:
    //          - NOTIFY is about to be sent.
    //
    //  Description:
    //      Informs the referrer component that it can proceed with an incoming
    //      REFER request.
    //
    //  See Also:
    //      ISceReferrerMgr::EvRefer
    //
    //==========================================================================
    virtual mxt_result AcceptRefer() = 0;

    //==========================================================================
    //==
    //==  RejectRefer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Rejects an incoming REFER request.
    //
    //  Returns:
    //      - resFE_INVALID_STATE:
    //          - component is not acting as a referee.
    //          - component is already terminating.
    //      - resS_OK:
    //          - final NOTIFY is about to be sent.
    //
    //  Description:
    //      Informs the referrer component that it must NOT proceed with an
    //      incoming REFER request. The request will be declined.
    //
    //  See Also:
    //      ISceReferrerMgr::EvRefer
    //
    //==========================================================================
    virtual mxt_result RejectRefer() = 0;

    //==========================================================================
    //==
    //==  NotifyReferProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the progress of the REFER operation to the peer that sent
    //      the REFER request.
    //
    //  Parameters:
    //    uStatusCode:
    //      The status of the operation, as a SIP status code.
    //
    //    bFinal:
    //      When it is true, send a final NOTIFY and start a shutdown process.
    //
    //  Returns:
    //      - resFE_INVALID_STATE:
    //          - component is not acting as a referee.
    //          - component is already terminating.
    //      - resFE_FAIL:
    //          - failed to send the NOTIFY.
    //      - resS_OK:
    //          - NOTIFY is about to be sent.
    //
    //  Description:
    //      Reports the progress of the REFER operation.
    //
    //      This method can be called multiple time for any status codes while
    //      bFinal is false.
    //
    //      It can only be called once with bFinal set to true and cannot be
    //      called afterwards.
    //
    //==========================================================================
    virtual mxt_result NotifyReferProgress(IN uint16_t uStatusCode,
                                           IN bool bFinal) = 0;

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    //==========================================================================
    //==
    //==  TerminateImmediately
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminate this referrer component immediately and silently.
    //
    //==========================================================================
    virtual void TerminateImmediately() = 0;
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

//M5T_INTERNAL_USE_BEGIN
    //-- << Internal methods >>

    //==========================================================================
    //==
    //==  HandleIncomingRefer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that informs the ISceReferrer to proceed with an
    //      incoming REFER request.
    //
    //  Parameters:
    //      rRequest:
    //       The request to handle.
    //
    //  Returns:
    //      - resFE_FAIL:
    //         rRequest is not handled.
    //
    //      - resS_OK:
    //         rRequest is being handled.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      This method must be called after the referrer has been configured
    //      with its user configuration object (see SetConfiguration) and
    //      with its manager (see SetManager).
    //
    //      It informs the referrer that it can proceed with the incoming REFER
    //      request.
    //
    //      This method does not have to be called by the application. It is
    //      called automatically by the UA SSP after reporting the
    //      ISceNewReferrerMgr::EvNewReferrerObject.
    //
    //  See Also:
    //      SetConfiguration, SetManager
    //
    //==========================================================================
    virtual mxt_result HandleIncomingRefer(IN const CSipPacket& rRequest) = 0;

    //==========================================================================
    //==
    //==  InternalTerminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that initiates component shutdown.
    //
    //  Returns:
    //      - resFE_INVALID_STATE:
    //         The object is already terminating.
    //      - resS_OK:
    //         The object is terminating.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      Initiates shutdown on the component.
    //
    //==========================================================================
    virtual mxt_result InternalTerminate() = 0;
//M5T_INTERNAL_USE_END

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceReferrer() {}

    // Summary:
    //  Destructor.
    virtual ~ISceReferrer() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceReferrer(IN const ISceReferrer& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceReferrer& operator=(IN const ISceReferrer& rSrc);


//-- Hidden Data Members.
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEREFERRER_H

