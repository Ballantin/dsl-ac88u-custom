//==============================================================================
//==============================================================================
//
//      Copyright(c) 2008 Media5 Corporation. ("Media5")
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
#ifndef MXG_CSIPGENERICCLIENTAUTHENTICATIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPGENERICCLIENTAUTHENTICATIONSVC_H

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


//-- Data Members


//-- Parameters used by Value


//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPGENERICCLIENTAUTHENTICATIONSVC_H
#include "SceStackServices/ISipGenericClientAuthenticationSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISipGenericClientAuthenticationMgr;
class CSipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
const mxt_opaque opqCHALLENGE_RECEIVED_EVENT = reinterpret_cast<mxt_opaque>(MX_INT32_TO_OPQ(1));

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipGenericClientAuthenticationSvc
//========================================
//
// Description:
//   This class is used to report the reception of 401 or 407 response that
//   has not been handled by the ISipDigestClientAuthSvc service.
//
// Location:
//   SceStackServices/CSipGenericClientAuthenticationSvc.h
//
// See Also:
//   ISipGenericClientAuthenticationSvc, ISipGenericClientAuthenticationMgr
//
//==============================================================================
class CSipGenericClientAuthenticationSvc : private CEComDelegatingUnknown,
                                           private ISipGenericClientAuthenticationSvc,
                                           private CSipReqCtxCoreSvc,
                                           private ISipCoreSvc
{
//-- Friend Declarations

    friend class CSceStackServicesInitializer;


//-- Hidden Methods
private:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipGenericClientAuthenticationSvc();

    static void FinalizeCSipGenericClientAuthenticationSvc();


    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipGenericClientAuthenticationSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipGenericClientAuthenticationSvc();

    //-- << CEComUnknown >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISipPacketObserverSvc >>

    virtual mxt_result SetManager(IN ISipGenericClientAuthenticationMgr* pMgr);

    //-- << ISipReqCtxCoreSvc >>
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    virtual unsigned int AddRcsRef();

    virtual unsigned int ReleaseRcsRef();


    //-- << ISipCoreSvc >>

    virtual void SetParentSipContext(IN CSipContext& rSipContext);

    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN EDirection eDirection);

    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    virtual mxt_result ClearCoreSvc();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipGenericClientAuthenticationSvc(IN const CSipGenericClientAuthenticationSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipGenericClientAuthenticationSvc& operator=(IN const CSipGenericClientAuthenticationSvc& rSrc);


    //-- << Helpers >>


//-- Hidden Data Members
protected:
private:

    ISipGenericClientAuthenticationMgr* m_pMgr;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//====  HELPER AND PRIVATE METHODS  ============================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSIPGENERICCLIENTAUTHENTICATIONSVC_H

