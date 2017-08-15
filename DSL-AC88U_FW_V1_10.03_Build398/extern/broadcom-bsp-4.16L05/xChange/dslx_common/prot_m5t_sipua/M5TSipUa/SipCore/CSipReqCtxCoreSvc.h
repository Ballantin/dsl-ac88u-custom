//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPREQCTXCORESVC_H
#define MXG_CSIPREQCTXCORESVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_ISIPREQCTXCORESVC_H
#include "SipCore/ISipReqCtxCoreSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipReqCtxCoreSvc
//========================================
//
// Description:
//   This class implements every virtual method of the ISipReqCtxCoreSvc
//   interface. This class should be the base class of every core services of
//   the SIP stack. All methods implemented in this class, except AddRcsRef
//   and ReleaseRcsRef, assert to never be called and trace an error message;
//   this is to avoid SIP core services to implement unneeded methods. Methods
//   that are really needed by services must be overridden.
//
//   Note that this class should never be instantiated directly.
//
// Location:
//   SipCore/CSipReqCtxCoreSvc.h
//
// See Also:
//   ISipRequestContext, ISipCoreSvc
//
//==EDOC========================================================================
class CSipReqCtxCoreSvc : public ISipReqCtxCoreSvc
{

//-- Protected Interface
//------------------------
protected:
    //-- << Stereotype >>
    //---------------------

    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    virtual void OnPacketReceived(IN    ISipRequestContext& rRequestContext,
                                  IN    const CSipPacket&   rPacket,
                                  INOUT CSipCoreEventList&  rEventList);

    virtual mxt_result UpdatePacket(IN    ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket&         rPacket);

    virtual mxt_result OnPacketUpdated(IN    ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket&         rPacket);

    virtual mxt_result SendResponse(IN    ISipRequestContext& rRequestContext,
                                    IN    const CSipPacket&   rPacket,
                                    IN    unsigned int        uCode,
                                    IN    const char*         szReason,
                                    IN TO CHeaderList*        pExtraHeaders,
                                    IN TO CSipMessageBody*    pMessageBody);

    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket);

    virtual void OnPacketReadyToSend(IN    ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque          opqEvent,
                           IN const CSipPacket&   rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque          opqEvent);

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipReqCtxCoreSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipReqCtxCoreSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipReqCtxCoreSvc(IN const CSipReqCtxCoreSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipReqCtxCoreSvc& operator=(IN const CSipReqCtxCoreSvc& rSrc);

};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREQCTXCORESVC_H

