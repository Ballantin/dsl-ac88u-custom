//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation.
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
//==EDOC========================================================================
#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#define MXG_CSIPREQCTXCONNECTIONSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_ISIPREQCTXCONNECTIONSVC_H
#include "SipCore/ISipReqCtxConnectionSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==SDOC========================================================================
//== Class: CSipReqCtxConnectionSvc
//========================================
//
// Description:
//   This class implements every virtual method of the ISipReqCtxConnectionSvc
//   interface. This class should be the base class of every connection services
//   of the SIP stack. All methods implemented in this class exclusively
//   generate traces; this is to avoid SIP connection services to implement
//   unneeded methods. Methods that are really needed by services must be
//   overridden.
//
//   Note that this class should never be instantiated directly.
//
// Location:
//   SipCore/CSipReqCtxConnectionSvc.h
//
// See Also:
//   ISipRequestContext, ISipConnectionSvc
//
//==EDOC========================================================================
class CSipReqCtxConnectionSvc : public ISipReqCtxConnectionSvc
{
//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //---------------------

    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket&   rPacketSent);

    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket&   rPacket,
                                     IN mxt_result          resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    virtual mxt_result UpdatePacket(IN    ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket&         rPacket);

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipReqCtxConnectionSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipReqCtxConnectionSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipReqCtxConnectionSvc(IN const CSipReqCtxConnectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipReqCtxConnectionSvc& operator=(IN const CSipReqCtxConnectionSvc& rSrc);
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREQCTXCONNECTIONSVC_H

