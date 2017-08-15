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
#ifndef MXG_CSIPHISTORYINFOSVC_H
#define MXG_CSIPHISTORYINFOSVC_H

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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_HISTORY_INFO_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_HISTORY_INFO_SVC_SUPPORT to be able to use this class.
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


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipHeader;
class CNameAddr;


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipHistoryInfoSvc
//==============================================================================
//<GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   This service implements RFC4244 as a UA. It only copies the History-Info
//   headers from a 3xx response to the next non-ACK request sent.
//
//   Note that the current behaviour of this service is that it does not add the
//   'histinfo' option tag in any packet.
//
//#The CSipHistoryInfoSvc is an ECOM object#
//
//  The CSipHistoryInfoSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipHistoryInfoSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/CSipHistoryInfoSvc.h
//
//==============================================================================
class CSipHistoryInfoSvc : private CEComDelegatingUnknown,
                           private ISipCoreSvc,
                           private CSipReqCtxCoreSvc
{
//M5T_INTERNAL_USE_BEGIN

//-- Friend Declarations
friend class CSipStackInitializer;


public:

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    //----------------------
    CSipHistoryInfoSvc(IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    //-------------
    virtual ~CSipHistoryInfoSvc();

private:

    //-- << Startup mechanism >>
    static mxt_result InitializeCSipHistoryInfoSvc();
    static void FinalizeCSipHistoryInfoSvc();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown, OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested, OUT void**  ppInterface);


    //-- << ISipCoreSvc Interface >>

    // Summary:
    //  A pointer to the ISipReqCtxCoreSvc to attach to the request context.
    //  It can be NULL.
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket, IN EDirection eDirection);

    // Summary:
    //  Establishes the confidence level for which the service is a good
    //  candidate to own a request context.
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    // Summary:
    //  Returns the ISipReqCtxCoreSvc instance to use as the owner for a request
    //  context.
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket, OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    virtual mxt_result ClearCoreSvc();


    //-- << ISipReqCtxCoreSvc Interface >>

    // Summary:
    //  Informs the request context core service of a received packet.
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Modifies a packet about to be sent.
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext, INOUT CSipPacket& rPacket);
    // Summary:
    //  Adds a reference on this request context core service.
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Removes a reference on this request context core service.
    virtual unsigned int ReleaseRcsRef();


    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipHistoryInfoSvc(IN const CSipHistoryInfoSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipHistoryInfoSvc& operator=(IN const CSipHistoryInfoSvc& rSrc);

//-- Hidden Data Members
protected:
private:
    
    CSipHeader* m_pHistoryInfoHeader;
    CNameAddr*  m_pRedirectToAddr;

//M5T_INTERNAL_USE_END
};


MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSIPHISTORYINFOSVC_H

