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
#ifndef MXG_CSIPREQCTXDESTINATIONSELECTIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPREQCTXDESTINATIONSELECTIONSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_ISIPREQUESTCONTEXT_H
#include "SipCore/ISipRequestContext.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipReqCtxDestinationSelectionSvc
//==============================================================================
//
// Description:
//   This class implements the ISipReqCtxConnectionSvc interface, which is used
//   by the ISipRequestContext to interact with the connection services. It is
//   obtained by the request context through the ISipConnectionSvc interface.
//
//   It is responsible to appropriately set the next-hop URI.
//
// Location:
//   SipCoreSvc/CSipReqCtxDestinationSelectionSvc.h
//
//==============================================================================
class CSipReqCtxDestinationSelectionSvc : public CSipReqCtxConnectionSvc
{
//-- Friend Declarations
    friend class CSipDestinationSelectionSvc;

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:

    // Summary:
    //  Sets the peer address correctly inside a CSipPacket.
    static void SetPacketPeerAddress(INOUT CSipPacket& rPacket);


    //-- << CSipReqCtxConnectionSvc >>

    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

//-- Hidden Methods
protected:

    // Summary:
    //  Destructor.
    virtual ~CSipReqCtxDestinationSelectionSvc();

private:

    // Summary:
    //  Private method that builds a SipUri based on the information found in
    //  rPacket.
    void BuildUri(INOUT CSipPacket& rPacket, INOUT CSipUri* pSipUri);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipReqCtxDestinationSelectionSvc(IN const CSipUri* pForcedDestinationUri,
                                      IN unsigned int uDestinationId);

    // Summary:
    //  Copy Constructor.
    CSipReqCtxDestinationSelectionSvc(IN const CSipReqCtxDestinationSelectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipReqCtxDestinationSelectionSvc& operator=(IN const CSipReqCtxDestinationSelectionSvc& rSrc);


//-- Hidden Data Members
protected:
private:
    bool m_bInFailOver;

    // The forced destination URI.
    const CSipUri* m_pForcedDestinationUri;
    
    // The destination ID.
    unsigned int m_uDestinationId;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline
void CSipReqCtxDestinationSelectionSvc::ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(rRequestContext);
    MX_DELETE(this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREQCTXDESTINATIONSELECTIONSVC_H

//M5T_INTERNAL_USE_END
