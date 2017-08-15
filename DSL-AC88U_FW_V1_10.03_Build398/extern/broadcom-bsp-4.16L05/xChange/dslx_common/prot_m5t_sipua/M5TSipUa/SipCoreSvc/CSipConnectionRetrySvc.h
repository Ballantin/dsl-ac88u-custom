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
#ifndef MXG_CSIPCONNECTIONRETRYSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPCONNECTIONRETRYSVC_H
//M5T_INTERNAL_USE_END

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

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

class CSipUri;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//== Class: CSipConnectionRetrySvc
//==============================================================================
//
// Description:
//   This service is used to have a packet resent if a TCP_RST is received after
//   a request is sent. 
//
//   When a request fails to be sent due to a TCP_RST for the first 
//   time, the service tries to retry the destination before doing failover. 
//   If the same event occurs again in the same transaction, the service lets 
//   normal failover be processed.
//
//   This service MUST be attached before the CSipServerLocationSvc.
//
//#The CSipConnectionRetrySvc is an ECOM object#
//
//  The CSipConnectionRetrySvc is an ECOM object that is created through the
//  following ECOM identifier:
//
//  <B>Class Id: CLSID_CSipOutboundConnectionSvc</B>
//
//  Since this service has no user interface, it cannot be queried from the
//  ISipContext to which this service is attached.
//
// Location:
//   SipCore/CSipConnectionRetrySvc.h
//
// See Also:
//   CEComDelegatingUnknown, ISipReqCtxConnectionSvc, 
//   CSipReqCtxConnectionRetrySvc
//
//==============================================================================
class CSipConnectionRetrySvc : private CEComDelegatingUnknown,
                               private ISipConnectionSvc
{
//M5T_INTERNAL_USE_BEGIN

//-- Friend Declarations

    friend class CSipStackInitializer;

//-- Hidden Methods
private:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipConnectionRetrySvc();

    static void FinalizeCSipConnectionRetrySvc();

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    // << Constructors / Destructors >>
    CSipConnectionRetrySvc(IN IEComUnknown& rOuterIEComUnknown);
    virtual ~CSipConnectionRetrySvc();


    //-- << CEComDelegatingUnknown >>

    // The following statement is essential, it provide the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);


    //-- << ISipConnectionSvc >>

    // Summary:
    //  Obtain an instance of an ISipReqCtxConnectionSvc to attach to a request
    //  context.
    virtual ISipReqCtxConnectionSvc*
            GetReqCtxConnectionSvcInstance(
                                IN const CSipPacket& rPacket,
                                IN ISipConnectionSvc::EDirection eDirectiont);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    virtual void ClearConnectionSvc();


private:

    // Deactivated Constructors / Destructors / Operators
    CSipConnectionRetrySvc(const CSipConnectionRetrySvc& rFrom);
    CSipConnectionRetrySvc& operator=(const CSipConnectionRetrySvc& rFrom);

// Hidden Data Members
protected:
private:
    
//M5T_INTERNAL_USE_END
    
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif // #ifndef MXG_CSIPCONNECTIONRETRYSVC_H


