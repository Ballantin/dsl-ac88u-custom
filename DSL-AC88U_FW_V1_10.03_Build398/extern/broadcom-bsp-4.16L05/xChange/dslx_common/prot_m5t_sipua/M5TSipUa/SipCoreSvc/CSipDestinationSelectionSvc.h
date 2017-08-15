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
#ifndef MXG_CSIPDESTINATIONSELECTIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPDESTINATIONSELECTIONSVC_H


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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_DESTINATION_SELECTION_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_DESTINATION_SELECTION_SVC_SUPPORT to be able to use \
this class.
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPDESTINATIONSELECTIONSVC_H
#include "SipCoreSvc/ISipDestinationSelectionSvc.h"
#endif

#ifndef MXG_CSIPREQCTXDESTINATIONSELECTIONSVC_H
#include "SipCoreSvc/CSipReqCtxDestinationSelectionSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_CSIPPACKET_H
#include "SipTransport/CSipPacket.h"
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
//== Class: CSipDestinationSelectionSvc
//==============================================================================
//
// Description:
//   This class implements the ISipConnectionSvc interface, which is used by the
//   ISipRequestContext to interact with the connection services. It is obtained
//   by the request context.
//
//   It is responsible for updating the top-most Via header in a request.
//
// Location:
//   SipCoreSvc/CSipDestinationSelectionSvc.h
//
//==============================================================================
class CSipDestinationSelectionSvc : private CEComDelegatingUnknown,
                                    private ISipDestinationSelectionSvc,
                                    private ISipConnectionSvc
{
//-- Friend Declarations
    friend class CSipStackInitializer;

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:

    //-- << ISipDestinationSelectionSvc >>

    virtual void ForceDestination(IN const CSipUri& rForcedDestinationUri);

    virtual void SetDestinationId(IN unsigned int uDestinationId);

    //-- << ISipConnectionSvc >>

    virtual ISipReqCtxConnectionSvc* GetReqCtxConnectionSvcInstance(IN const CSipPacket& rPacket,
                                                                    IN EDirection eDirection);

    virtual void ClearConnectionSvc();

    //-- << CEComDelegatingUnknown >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown, OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested, OUT void** ppInterface);

//-- Hidden Methods
protected:
private:
    //-- << Startup mechanism >>
    static mxt_result InitializeCSipDestinationSelectionSvc();
    static void FinalizeCSipDestinationSelectionSvc();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipDestinationSelectionSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipDestinationSelectionSvc();

    // Summary:
    //  Copy Constructor.
    CSipDestinationSelectionSvc(IN const CSipDestinationSelectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipDestinationSelectionSvc& operator=(IN const CSipDestinationSelectionSvc& rSrc);

//-- Hidden Data Members
protected:
private:
    // The forced destination URI.
    const CSipUri* m_pForcedDestinationUri;

    // The destination ID.
    unsigned int m_uDestinationId;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline ISipReqCtxConnectionSvc* CSipDestinationSelectionSvc::GetReqCtxConnectionSvcInstance(IN const CSipPacket& rPacket,
                                                                                            IN EDirection eDirection)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(rPacket);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(eDirection);

    // Create a new svc instance, since this service is unique per transaction.
    return MX_NEW(CSipReqCtxDestinationSelectionSvc)(m_pForcedDestinationUri, m_uDestinationId);
}

inline void CSipDestinationSelectionSvc::ClearConnectionSvc()
{

}

inline void CSipDestinationSelectionSvc::SetDestinationId(IN unsigned int uDestinationId)
{
    m_uDestinationId = uDestinationId;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPDESTINATIONSELECTIONSVC_H

//M5T_INTERNAL_USE_END
