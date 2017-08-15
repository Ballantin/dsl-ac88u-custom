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
#ifndef MXG_ISCEREFERRERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEREFERRERMGR_H
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


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class ISceReferrer;
class CNameAddr;
class IEComUnknown;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISceReferrerMgr
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Description:
//  This is the interface through which the referrer component reports events.
//
// Location:
//   SceCoreComponents/ISceReferrerMgr.h
//
// See Also:
//   ISceReferrer
//
//==EDOC========================================================================
class ISceReferrerMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:

    //==========================================================================
    //==
    //==  EvRefer
    //==
    //==========================================================================
    //
    //  Summary:
    //      A REFER request identifying a Target-Dialog was received.
    //
    //  Parameters:
    //    pReferrerComponent:
    //      Pointer to the component reporting this event.
    //
    //    pReferSource:
    //      AOR of the peer that sent the REFER request.
    //
    //    pTargetDialog:
    //      Pointer to the component that represents the target dialog. Can be
    //      NULL if the Target-Dialog header did not match any existing dialog.
    //
    //    rReferTo:
    //      The 3rd-party address where to send the request.
    //
    //    pAdditionalParameters:
    //       Additional parameters in relation to the unhandled request. Can be
    //       NULL if no additional parameters are present.
    //
    //       This returned parameter is an ECOM object that can be queried for
    //       all the interfaces needed to be watched or processed by the
    //       application from the Base Component or from specific component
    //       interfaces.
    //
    //       If it is NULL, it means that no additional parameters could be
    //       returned to the application, either because there was a processing
    //       problem or that no server event yielded information worth
    //       returning.
    //
    //  Description:
    //      Reports that a REFER request was received. This request contained a
    //      Target-Dialog header that identified either an existing dialog
    //      (pTargetDialog is non-NULL) or an unknown dialog (pTargetDialog is
    //      NULL).
    //
    //      After this event is reported, the user of this component must call
    //      either ISceReferrer::AcceptRefer or ISceReferred::RejectRefer.
    //
    //      If ISceReferrer::AcceptRefer is called, then the user must take
    //      action on the REFER (like placing a call to rReferTo) and use
    //      ISceReferrer::NotifyReferProgress to report the progression of the
    //      request until EvTerminated is reported.
    //
    //      If ISceReferrer::RejectRefer is called, the user should wait for
    //      EvTerminated to be reported to release this component.
    //
    //==========================================================================
    virtual void EvRefer(IN ISceReferrer* pReferrerComponent,
                         IN const CNameAddr& pReferSource,
                         IN IEComUnknown* pTargetDialog,
                         IN const CNameAddr& rReferTo,
                         IN IEComUnknown* pAdditionalParameters) = 0;

    //==========================================================================
    //==
    //==  EvReferProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the progression of a request after a REFER request was
    //      accepted by a peer.
    //
    //  Parameters:
    //    pReferrerComponent:
    //      Pointer to the component reporting this event.
    //
    //    uStatusCode:
    //      The SIP status code corresponding to the current progression of the
    //      request.
    //
    //  Description:
    //      This event reports the progression of a request after a REFER
    //      request is sent with ISceReferrer::Refer and accepted by the peer
    //      (EvReferStatus(eSTATUS_ACCEPTED) has been reported).
    //
    //      This event can be reported multiple time with different SIP status
    //      codes. Note that it can be reported more than once for a final
    //      status code (for example if the request is challenged or
    //      redirected).
    //
    //==========================================================================
    virtual void EvReferProgress(IN ISceReferrer* pReferrerComponent,
                                 IN uint16_t uStatusCode) = 0;

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //  The possible statuses of the REFER.
    //
    // Description:
    //  The possible statuses of the REFER request that was sent to the peer.
    //
    //==========================================================================
    enum EReferStatus
    {
        // Description:
        //  The request was accepted with a 2xx class response.
        eSTATUS_ACCEPTED,

        // Description:
        //  The request was rejected because Target-Dialog is not supported.
        eSTATUS_NO_TARGET_DIALOG,

        // Description:
        // The request was rejected for any other reason.
        eSTATUS_FAILED
    };


    //==========================================================================
    //==
    //==  EvReferStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports whether a REFER request sent was accepted or not by the
    //      peer.
    //
    //  Parameters:
    //    pReferrerComponent:
    //      Pointer to the component reporting this event.
    //
    //    eStatus:
    //      The status that identifies whether or not the REFER request was
    //      accepted.
    //
    //  Description:
    //      Reports if a sent REFER request was accepted or rejected by the
    //      peer.
    //
    //      When eStatus is eSTATUS_NO_TARGET_DIALOG or eSTATUS_FAILED, it means
    //      that the request was rejected. EvTerminated will be reported
    //      immediately after this event.
    //
    //      When eStatus is eSTATUS_ACCEPTED, it means that the user should
    //      expect to receive zero or more EvReferProgress until EvTerminated is
    //      reported.
    //
    //==========================================================================
    virtual void EvReferStatus(IN ISceReferrer* pReferrerComponent,
                               IN EReferStatus eStatus) = 0;


    //==========================================================================
    //==
    //==  EvTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the referrer component is terminated and must be
    //      released by the application.
    //
    //  Parameters:
    //    pReferrerComponent:
    //      Pointer to the component reporting this event.
    //
    //  Description:
    //      Reports that the referrer component is terminated and must be
    //      released by the application.
    //
    //      This is always the last event reported by this component.
    //
    //==========================================================================
    virtual void EvTerminated(IN ISceReferrer* pReferrerComponent) = 0;


//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceReferrerMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISceReferrerMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceReferrerMgr(IN const ISceReferrerMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceReferrerMgr& operator=(IN const ISceReferrerMgr& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEREFERRERMGR_H

