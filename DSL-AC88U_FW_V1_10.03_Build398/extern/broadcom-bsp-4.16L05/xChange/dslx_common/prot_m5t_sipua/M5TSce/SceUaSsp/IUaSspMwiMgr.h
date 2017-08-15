//==============================================================================
//==============================================================================
//
//      Copyright(c) 2007 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_IUASSPMWIMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPMWIMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_ISCESUBSCRIBERMGR_H
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CMessageSummary;
class CNameAddr;
class IUaSspMwi;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspMwiMgr
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This is the interface through which the MWI components report their events.
//
// Description:
//  This is the interface through which the MWI components report their events
//  about the message-inbox state of a user.
//
// Location:
//   UaSsp/IUaSspMwiMgr.h
//
// See Also:
//   IUaSspMwi, IUaSspMwiConfig
//
//==============================================================================
class IUaSspMwiMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:

    //==========================================================================
    //==
    //==  EvMwiStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the subscription status.
    //
    //  Parameters:
    //      pMwi:
    //       The MWI component reporting this event.
    //
    //      eStatus:
    //       The status of the subscription.
    //
    //  Description:
    //      Reports the subscription status of the user. This event can be
    //      reported even if the subscription status does not change. For
    //      instance, the eSUBSCRIBED status can be reported once the first
    //      subscription succeeds and each time the subscription is successfully
    //      refreshed.
    //
    //      When eStatus is eUNSUBSCRIBED, the MWI component will try to
    //      re-subscribe the user at the specified interval.
    //
    //      When eStatus is either eTERMINATED or eSUBSCRIPTION_FAILED, the
    //      MWI component does not attempt periodic subscription
    //      retries.
    //
    //      It is possible for a subscription to move directly from the state
    //      eSUBSCRIBED to eUNSUBSCRIBED or eSUBSCRIPTION_FAILED without
    //      reporting any other events.
    //
    //      For the unsolicited MWI component, this event will only get reported
    //      with eTERMINATED when IUaSspMwi::Terminate is called.
    //
    //==========================================================================
    virtual void EvSubscriptionStatus(IN IUaSspMwi* pMwi,
                                      IN ISceSubscriberMgr::ESubscriptionStatus eStatus) = 0;


    //==========================================================================
    //==
    //==  EvMailboxStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the status of the mailbox.
    //
    //  Parameters:
    //    rSummary:
    //      Message summary object detailing the content of the mailbox.
    //
    //    pMwi:
    //      Pointer to the MWI component reporting this event.
    //
    //  Description:
    //      Reports that the status of the mailbox was changed and provides an
    //      updated view of the mailbox through the rSummary parameter.
    //
    //==========================================================================
    virtual void EvMailboxStatus(IN IUaSspMwi* pMwi,
                                 IN const CMessageSummary& rSummary) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IUaSspMwiMgr() {}

    // Summary:
    //  Destructor.
    virtual ~IUaSspMwiMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IUaSspMwiMgr(IN const IUaSspMwiMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IUaSspMwiMgr& operator=(IN const IUaSspMwiMgr& rSrc);

//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPMWIMGR_H

