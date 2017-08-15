//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPREPLACESMGR_H
#define MXG_ISIPREPLACESMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT to be able \
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
class ISipUserAgentSvc;
class ISipReplacesSvc;
class ISipServerEventControl;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipReplacesMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The replaces manager is the interface through which the replaces service
//  reports events to the application. The replaces service informs the 
//  application through this interface that an INVITE request containing
//  a Replaces headers has been received, whether or not it contains an error
//  and whether or not it corresponds to an existing dialog.
//
// Location:
//   SipUserAgent/ISipReplacesMgr.h
//
// See Also:
//   ISipReplacesSvc
//
//==EDOC========================================================================
class ISipReplacesMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvReplaces
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a SIP INVITE request containing a Replaces header was
    //      received and that a corresponding dialog was found.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipReplacesSvc managing the incoming request.
    //
    //    pReplacedUaSvc:
    //      The User Agent Service of the dialog corresponding to the parameters
    //      in the Replaces header. It cannot be NULL.
    //
    //    bEarlyOnly:
    //      true if the early-only parameter was present in the Replaces header,
    //      false otherwise.
    //
    //    rRequest:
    //      The SIP request.
    //
    //    rApplicationData:
    //      Application data opaque to the service. This parameter is an INOUT
    //      parameter. It is used to correlate the events reported by multiple
    //      services for a unique received request. If the application already
    //      received an event for that request through another manager
    //      interface, rApplicationData equals the value stored in it by the
    //      application. Otherwise, if it is the first event reported for this
    //      received request, rApplicationData is set to 0. The value of
    //      rApplicationData when EvReplaces returns is accessible through
    //      the GetOpaque method of the ISipServerEventControl interface that
    //      accompanies the request when the owner service issues its event.
    //      This opaque data should be used to store the state indicating that a
    //      valid Replace was received and be able to act accordingly when the
    //      session manager receives the EvInvited event. Note that this event
    //      cannot be processed asynchronously since the opaque application
    //      data is passed by value to the ISipServerEventControl interface.
    //
    //  Description:
    //      Notifies the manager that a SIP INVITE request containing a Replaces
    //      header has been received and that a corresponding dialog has been
    //      found.
    //
    //==EDOC====================================================================
    virtual void EvReplaces(IN ISipReplacesSvc* pSvc,
                            IN ISipUserAgentSvc* pReplacedUaSvc,
                            IN bool bEarlyOnly,
                            IN const CSipPacket& rRequest,
                            INOUT mxt_opaque& rApplicationData) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInvalidReplaces
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a SIP INVITE request containing a Replaces header has
    //      been received, which was wrong or did not correspond to a single 
    //      existing dialog.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipReplacesSvc managing the incoming request.
    //
    //    opqApplicationData:
    //      The opaque data set by the application in a previous event for this
    //      same received request. When this is the first event for this
    //      request, this parameter is 0.
    //
    //    rRequest:
    //      The SIP request.
    //
    //    res:
    //      The error result:
    //      - resFE_FAIL when no dialog was found. A "481 Call/Transaction Does
    //      Not Exist" response has been automatically sent.
    //
    //      - resFE_INVALID_ARGUMENT when there is an error in the Replaces
    //      header or there is more than one Replaces header. A "400 Bad
    //      Request" response has been automatically sent.
    //
    //      - resFE_INVALID_STATE when the dialog matcher list has not been set
    //      beforehand or if the manager was not set. A "500 Server Internal
    //      Error" response has been automatically sent.
    //
    //  Description:
    //      Informs the manager that a SIP INVITE request containing a Replaces
    //      header has been received, which was wrong or did not correspond to a
    //      single existing dialog.
    //
    //==EDOC====================================================================
    virtual void EvInvalidReplaces(IN ISipReplacesSvc* pSvc,
                                   IN mxt_opaque opqApplicationData,
                                   IN const CSipPacket& rRequest,
                                   IN mxt_result res) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipReplacesMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipReplacesMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipReplacesMgr(IN const ISipReplacesMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipReplacesMgr& operator=(IN const ISipReplacesMgr& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREPLACESMGR_H

