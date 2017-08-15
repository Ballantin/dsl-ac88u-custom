//==============================================================================
//==============================================================================
//
//   Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPREQCTXCONNECTIONRETRYSVC_H
#define MXG_CSIPREQCTXCONNECTIONRETRYSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#ifndef MXD_SIPSTACK_ENABLE_SIP_CONNECTION_RETRY_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_CONNECTION_RETRY_SVC_SUPPORT to be able to use this \
class.
#endif

//-- Data Members

//-- Parameters used by Value

//-- Interface Realized and/or Parent
#ifndef MXG_ISIPREQCTXCONNECTIONSVC_H
#include "SipCore/ISipReqCtxConnectionSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//== Class: CSipReqCtxConnectionRetrySvc
//========================================
//
// Description:
//   This class implements the ISipReqCtxConnectionSvc interface which is used
//   by the ISipRequestContext to interact with the connection services.
//   It is obtained by the request context through the ISipConnectionSvc
//   interface.
//
//   When it received an OnSendFailure request due to a TCP_RST for the first 
//   time, the service tries to retry the destination by using the
//   information already put in the packet. If the same event occurs again in the 
//   same transaction, the service doesn't handle the packet and lets the rest of
//   the stack handle it until the end of the request context.
//
// Location:
//   SipCore/CSipReqCtxConnectionRetrySvc.h
//
// See Also:
//   ISipReqCtxConnectionSvc, IServerLocatorMgr, CServerLocator
//
//==============================================================================
class CSipReqCtxConnectionRetrySvc : public ISipReqCtxConnectionSvc
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor. 
    CSipReqCtxConnectionRetrySvc();

//-- Hidden Methods
protected:
   
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Destructor.
    virtual ~CSipReqCtxConnectionRetrySvc();


private:
    //-- << ISipReqCtxConnectionSvc >>

    // Summary:
    //  Method called after an updated packet was successfully sent.
    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent);

    // Summary:
    //  Method called after an updated packet was not successfully sent.
    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    // Summary:
    //  Updates rPacket with the resolved target.
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Destroys this instance of ISipReqCtxConnectionSvc.
    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipReqCtxConnectionRetrySvc(IN const CSipReqCtxConnectionRetrySvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipReqCtxConnectionRetrySvc& operator=(
                                IN const CSipReqCtxConnectionRetrySvc& rSrc);


//-- Hidden Data Members
protected:
  
private:
    bool m_bFailureHandled;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREQCTXCONNECTIONRETRYSVC_H

