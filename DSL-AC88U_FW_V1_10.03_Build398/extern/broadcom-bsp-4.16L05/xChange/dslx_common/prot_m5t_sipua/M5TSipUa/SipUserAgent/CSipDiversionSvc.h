//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPDIVERSIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPDIVERSIONSVC_H

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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_DIVERSION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_DIVERSION_SVC_SUPPORT to be able to use this class.
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_ISIPDIVERSIONSVC_H
#include "SipUserAgent/ISipDiversionSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CNameAddr;
class CSipHeader;
class CString;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipDiversionSvc
//==============================================================================
//
// Description:
//   The diversion service is responsible for copying the Diversion headers
//   found in an incoming 3xx to the following INVITE request. The Diversion
//   headers will be copied to INVITEs until a 2xx has been received. It can
//   also be used to add a Diversion header to outgoing 3xx.
//
// Location:
//   SipUserAgent/CSipDiversionSvc.h
//
//==============================================================================
class CSipDiversionSvc : private CEComDelegatingUnknown,
                         private ISipCoreSvc,
                         private ISipDiversionSvc,
                         private CSipReqCtxCoreSvc
{
//-- Friend Declarations
    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- Published Interface
public:

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipDiversionSvc(IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CSipDiversionSvc();

private:

    //-- << Startup mechanism >>
    static mxt_result InitializeCSipDiversionSvc();
    static void FinalizeCSipDiversionSvc();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISipCoreSvc Interface >>

    // Summary:
    //  A pointer to the ISipReqCtxCoreSvc to attach to the request context.
    //  It can be NULL.
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN EDirection eDirection);

    // Summary:
    //  Establishes the confidence level for which the service is a good
    //  candidate to own a request context.
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    // Summary:
    //  Returns the ISipReqCtxCoreSvc instance to use as the owner for a request
    //  context.
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    virtual mxt_result ClearCoreSvc();


    //-- << ISipDiversionSvc Interface >>
    // Summary:
    //  Sets the reason associated to the diversion.
    virtual void SetReason(IN const char* pszReason);


    //-- << ISipReqCtxCoreSvc Interface >>

    // Summary:
    //  Informs the request context core service of a received packet.
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Modifies a packet about to be sent.
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);
    // Summary:
    //  Adds a reference on this request context core service.
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Removes a reference on this request context core service.
    virtual unsigned int ReleaseRcsRef();


    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipDiversionSvc(IN const CSipDiversionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipDiversionSvc& operator=(IN const CSipDiversionSvc& rSrc);


//-- Hidden Data Members
protected:
private:

    // This member is used to hold the Diversion headers found in a 3xx.
    CSipHeader* m_pDiversionHeader;

    // The reason set by the application.
    CString* m_pstrReason;

    // The request URI of the last received INVITE.
    CNameAddr* m_pInviteUri;
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPDIVERSIONSVC_H

//M5T_INTERNAL_USE_END
