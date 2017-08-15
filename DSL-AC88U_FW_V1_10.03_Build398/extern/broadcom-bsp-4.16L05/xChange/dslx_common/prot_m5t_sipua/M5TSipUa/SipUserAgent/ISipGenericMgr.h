//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2001-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPGENERICMGR_H
#define MXG_ISIPGENERICMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipServerEventControl;
class ISipClientEventControl;
class ISipGenericSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipGenericMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The generic manager is the interface through which the generic service
//  reports events to the application. The generic service informs the 
//  application through this interface of incoming generic requests and
//  incoming responses to generic requests sent to a remote party.
//
// Location:
//   SipUserAgent/ISipGenericMgr.h
//
// See Also:
//   ISipGenericSvc
//
//==EDOC========================================================================
class ISipGenericMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //    Reports that a SIP request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipGenericSvc managing the incoming request.
    //
    //    pServerEventCtrl:
    //      Interface to the server event control for this server transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Informs the manager that a generic request has been received. The
    //      application MUST use the <C>SendResponse</C> method of the
    //      ISipServerEventControl in parameter to send a response to that
    //      request.
    //
    //      Managers should not send 1xx responses to non-INVITE requests, but
    //      should instead answer as soon as possible with a final response.
    //
    //  See Also:
    //      ISipServerEventControl::SendResponse
    //
    //==EDOC====================================================================
    virtual void EvRequest(IN ISipGenericSvc*         pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket&       rRequest) = 0;


    //==SDOC====================================================================
    //==
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //    Reports that a provisional response to a previously sent request has
    //    been received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipGenericSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      The interface to the client event control for this transaction.
    //
    //    rResponse:
    //      The SIP response.
    //
    //  Description:
    //      Informs the manager that a response to one of the generic request
    //      it has sent has been received. This event can be called for zero
    //      to many provisional responses (1xx).
    //
    //  See Also:
    //      EvSuccess, EvFailure, ISipGenericSvc::SendRequest
    //
    //==EDOC====================================================================
    virtual void EvProgress(IN ISipGenericSvc*         pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket&       rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //    Reports that a final response (2xx) to a previously sent request has
    //    been received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipGenericSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      The interface to the client event control for this transaction.
    //
    //    rResponse:
    //      The SIP response.
    //
    //  Description:
    //      Informs the manager that a response to one of the generic request
    //      it has sent has been received. This event can be called for one 
    //      final response (2xx).
    //
    //  See Also:
    //      EvProgress, EvFailure, ISipGenericSvc::SendRequest
    //
    //==EDOC====================================================================
    virtual void EvSuccess(IN ISipGenericSvc*         pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket&       rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //    Reports that a final response (>= 300) to a previously sent request
    //    has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipGenericSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      The interface to the client event control for this transaction.
    //
    //    rResponse:
    //      The SIP response.
    //
    //  Description:
    //      Informs the manager that a response to one of the generic request
    //      it has sent has been received. This event can be called for one 
    //      final response (>= 300).
    //
    //  See Also:
    //      EvProgress, EvSuccess, ISipGenericSvc::SendRequest
    //
    //==EDOC====================================================================
    virtual void EvFailure(IN ISipGenericSvc*         pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket&       rResponse) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipGenericMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipGenericMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipGenericMgr(IN const ISipGenericMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipGenericMgr& operator=(IN const ISipGenericMgr& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPGENERICMGR_H

