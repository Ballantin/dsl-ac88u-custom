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
#ifndef MXG_ISIPREDIRECTIONMGR_H
#define MXG_ISIPREDIRECTIONMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT to be \
able to use this class.
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
class ISipClientEventControl;
class ISipRedirectionSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipRedirectionMgr
//===========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The Redirection Event Manager is the interface through which the Redirection
//  Service talks to the application. The stack informs the application through
//  this interface that the user it is trying to reach can be found at a 
//  different location.
//
// Location:
//   SipUserAgent/ISipRedirectionMgr.h
//
// See Also:
//  ISipRedirectionSvc
//==============================================================================
class ISipRedirectionMgr
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:
    // << Destructors >>
    //-------------------
    virtual ~ISipRedirectionMgr() {}

    //==========================================================================
    //==
    //==  EvRedirected
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that the party it is trying to reach can be
    //      reached at another URL.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipRegistrationSvc that is managing this response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality. The 
    //      request can be reissued (with ReIssueRequest) through this interface.
    //
    //    rPacket:
    //     The packet that triggered a redirection.
    //
    //  Description:
    //      Notifies the application that the party it is trying to reach is
    //      unreachable at the last URL tried but other possible URLs remain.
    //      The first time such an event occurs in a context is upon receiving
    //      a 3xx response to an outgoing request. It then occurs every time
    //      a new target has failed and that there are more targets to try.
    //
    //      When this event occurs, the application should use the redirection
    //      service UseNextContact or the UseContact method and re-issue the
    //      request with pClientEventCtrl.
    //
    //      This event never occurs with a 6xx response packet since such a
    //      response code means that the last tried target has authoritative
    //      information that the request will fail on every possible target. In
    //      this case, all the untried URLs remembered by the redirection
    //      service are cleared. This means that as soon as a 6xx response
    //      is received, there will not be any event generated (not even when the
    //      6xx response is received), until another 3xx response is received.
    //
    //  See Also:
    //      ISipRedirectionSvc
    //==========================================================================
// BRCM_CUSTOM - [mod] - Add proxy failover support.
    virtual void EvRedirected( IN ISipRedirectionSvc* pSvc,
                               IN ISipClientEventControl* pClientEventCtrl,
                               IN const CSipPacket& rPacket
#if MXD_BRCM_PROXY_FAILOVER_SUPPORT
                               , IN mxt_opaque handle
#endif /* MXD_BRCM_PROXY_FAILOVER_SUPPORT */
                               ) = 0;
// BRCM_CUSTOM - [end] - Add proxy failover support.

// Hidden Methods
//-------------------------
protected:
    // << Constructors >>
    //--------------------
    ISipRedirectionMgr() {}

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    ISipRedirectionMgr( const ISipRedirectionMgr& rFrom );
    ISipRedirectionMgr& operator=( const ISipRedirectionMgr& rFrom );

// Hidden Data Members
//-------------------------
protected:
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPREDIRECTIONMGR_H

