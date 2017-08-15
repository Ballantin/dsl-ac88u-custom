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
#ifndef MXG_CSIPVIAMANAGEMENTSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPVIAMANAGEMENTSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_VIA_MANAGEMENT_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_VIA_MANAGEMENT_SVC_SUPPORT to be able to use \
this class.
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPVIAMANAGEMENTSVC_H
#include "SipCoreSvc/ISipViaManagementSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_ISIPREQUESTCONTEXT_H
#include "SipCore/ISipRequestContext.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_CSIPPACKET_H
#include "SipTransport/CSipPacket.h"
#endif

#ifndef MXG_ISIPSERVERLOCATIONSVC_H
#include "SipCoreSvc/ISipServerLocationSvc.h"
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
//== Class: CSipViaManagementSvc
//==============================================================================
//
// Description:
//   This class implements the ISipViaManagementSvc and ISipConnectionSvc
//   interfaces, which is used by the ISipRequestContext to interact with the
//   connection services. It is obtained by the request context.
//
//   It is responsible for updating the top-most Via header in a request.
//
// Location:
//   SipCoreSvc/CSipViaManagementSvc.h
//
//==============================================================================
class CSipViaManagementSvc : private CEComDelegatingUnknown,
                             private ISipViaManagementSvc,
                             private ISipConnectionSvc,
                             private CSipReqCtxConnectionSvc
{
//-- Friend Declarations
    friend class CSipStackInitializer;

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    // Summary:
    //  Adds the rport parameter to the Via header.
    static void AddViaRportParam(IN bool bAddRport);

    //-- << ISipViaManagementSvc >>

    virtual void SetViaManagementSvcMode(EViaManagementMode eMode);

    //-- << CSipReqCtxConnectionSvc >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown, OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested, OUT void** ppInterface);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext, INOUT CSipPacket& rPacket);

    virtual ISipReqCtxConnectionSvc* GetReqCtxConnectionSvcInstance(IN const CSipPacket& rPacket,
                                                                    IN EDirection eDirection);

    virtual void ClearConnectionSvc();

    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

//-- Hidden Methods
protected:
private:

    //-- << Startup mechanism >>
    static mxt_result InitializeCSipViaManagementSvc();
    static void FinalizeCSipViaManagementSvc();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipViaManagementSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipViaManagementSvc();


    // Summary:
    //  Copy Constructor.
    CSipViaManagementSvc(IN const CSipViaManagementSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipViaManagementSvc& operator=(IN const CSipViaManagementSvc& rSrc);


//-- Hidden Data Members
protected:
    // Flag stating if a rport parameter needs to be inserted.
    static bool ms_bAddRport;

    // Flag stating if the branch parameter needs to be updated (proxy).
    bool m_bIsStateless;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  AddViaRportParam
//==
//==============================================================================
//
//  Parameters:
//    bAddRport:
//      True to add an rport parameter to the Via header in all requests,
//      false otherwise.
//
//  Description:
//      Adds the rport parameter to the Via header of all outgoing requests.
//      RFC 3581 permits the addition of the rport parameter to force
//      UAs to reply on the same UDP address/port on which the request has been
//      received. The local address/port used to receive the packet should
//      also be used for NAT traversal purposes.
//
//==============================================================================
inline
void CSipViaManagementSvc::AddViaRportParam(IN bool bAddRport)
{
    ms_bAddRport = bAddRport;
}

inline
void CSipViaManagementSvc::SetViaManagementSvcMode(EViaManagementMode eMode)
{
    m_bIsStateless = (eMode == eSTATELESS);
}

inline
ISipReqCtxConnectionSvc* CSipViaManagementSvc::GetReqCtxConnectionSvcInstance(IN const CSipPacket& rPacket,
                                                                              IN EDirection eDirection)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(rPacket);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(eDirection);
    AddIfRef();
    return this;
}

inline
void CSipViaManagementSvc::ClearConnectionSvc()
{

}

inline
void CSipViaManagementSvc::ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(rRequestContext);
    ReleaseIfRef();
}
MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPVIAMANAGEMENTSVC_H

//M5T_INTERNAL_USE_END
