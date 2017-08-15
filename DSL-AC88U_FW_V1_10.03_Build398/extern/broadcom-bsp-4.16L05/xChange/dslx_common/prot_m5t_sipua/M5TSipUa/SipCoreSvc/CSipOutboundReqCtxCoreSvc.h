//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPOUTBOUNDREQCTXCORESVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPOUTBOUNDREQCTXCORESVC_H

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

#ifndef MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT to be able to use \
this class.
#endif

//-- Data Members

//-- Interface Realized and/or Parent


#ifndef MXG_ISIPREQCTXCORESVC_H
#include "SipCore/ISipReqCtxCoreSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSipOutboundConnectionSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipOutboundReqCtxCoreSvc
//==============================================================================
//
// Description:
// Description:
//   This class implements the ISipReqCtxCoreSvc interface which is used
//   by the ISipRequestContext to interact with the core services.
//   It is obtained by the request context through the ISipCoreSvc
//   interface of the CSipOutboundConnectionSvc.
//
//   The event EvOutboundRequiredByPeer is reported by this service when a
//   REGISTER 2xx response with Require: outbound is received. This class is
//   only interested in 2xx response to REGISTER requests.
//
// Location:
//   SipCoreSvc/CSipOutboundReqCtxCoreSvc.h
//
//==============================================================================
class CSipOutboundReqCtxCoreSvc : public ISipReqCtxCoreSvc
{
//-- Friend Declarations

//-- Published Interface
public:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipOutboundReqCtxCoreSvc(IN CSipOutboundConnectionSvc* pSipOutboundConnectionSvc,
                              IN bool bAddRef = true);

    // Summary:
    //  Destructor.
    virtual ~CSipOutboundReqCtxCoreSvc();

// Hidden Methods
private:
    //-- << ISipReqCtxCoreSvc methods >>

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket& rPacket,
                                    IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TO CHeaderList* pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket);

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    virtual unsigned int AddRcsRef();

    virtual unsigned int ReleaseRcsRef();

//-- Hidden Methods
protected:
private:


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipOutboundReqCtxCoreSvc(IN const CSipOutboundReqCtxCoreSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipOutboundReqCtxCoreSvc& operator=(IN const CSipOutboundReqCtxCoreSvc& rSrc);


// Friends declarations

//-- Hidden Data Members
protected:
private:

    // Enum kept to manage events.
    enum EOutboundEvent
    {
        eOUTBOUND_REQUIRED_BY_PEER
    };

    CSipOutboundConnectionSvc* m_pSipOutboundConnectionSvc;

    unsigned int m_uRefCount;
};

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSIPOUTBOUNDREQCTXCORESVC_H

