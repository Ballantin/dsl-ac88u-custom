//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_ISIPREQCTXCONNECTIONSVC_H
#define MXG_ISIPREQCTXCONNECTIONSVC_H

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


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipRequestContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipReqCtxConnectionSvc
//========================================
//
// Description:
//   This interface is used by the ISipRequestContext to interact with the
//   connection services. It is obtained by the request context through the
//   ISipConnectionSvc interface. When a packet is to be sent to the transaction
//   or the transport layer, it is first presented to all connection services so
//   they can update the destination for the packet and possibly the topmost Via
//   header. Upon receiving the result of the sent action, the
//   ISipRequestContext informs all connection services so they can update their
//   state and, upon failure, suggests other target where it is possible to
//   attempt sending the request.
//
// Location:
//   SipCore/ISipReqCtxConnectionSvc.h
//
// See Also:
//   ISipRequestContext, ISipConnectionSvc
//
//==EDOC========================================================================
class ISipReqCtxConnectionSvc
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>

    // Description:
    //   This enum indicates the current state of the retrying state of a
    //   transaction. The CSipConnectionRetrySvc service introduced the notion
    //   of retransmission to the same target, without any changes in a packet
    //   nor in the internal states of the stack (see documentation of
    //   CSipConnectionRetrySvc for more details). This enum is used to explicit
    //   in which state the failure handling currently is.
    enum ECurrentRetryState
    {
        // Description:
        //   The retrying process doesn't occur yet. No service changed this
        //   default value yet.
        eNOT_RETRYING,

        // Description:
        //   This state is set when the stack needs to retry to send a packet
        //   without any changes in its target or other internal state. E.g.
        //   this is set by the CSipConnectionRetrySvc to retransmit the exact
        //  same packet to a server after reception of a RST TCP error.
        eRETRYING_SAME_TARGET,

        // Description:
        //   This state is set when the stack needs to retry to send a packet
        //   to a different target. e.g. CSipServerLocationSvc will update the
        //   packet before its retransmission.
        eRETRYING_DIFFERENT_TARGET
    };

    //==SDOC====================================================================
    //==
    //==  OnSendSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the connection service that the packet has been successfully
    //      sent.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacketSent:
    //      The packet that has been successfully sent.
    //
    //  Description:
    //      Informs the request context connection service that the last packet
    //      has been successfully sent.
    //
    //      For each call to UpdatePacket, this method or OnSendFailure must be
    //      called prior to the next call to UpdatePacket.
    //
    //  See Also:
    //      UpdatePacket, OnSendFailure
    //
    //==EDOC====================================================================
    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent) = 0;

    //==SDOC====================================================================
    //==
    //==  OnSendFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the connection service that the packet has not been
    //      successfully sent.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacket:
    //      The packet that failed to be sent.
    //
    //    resCause:
    //      Error code indicating the cause of the failure.
    //
    //    reRetryState:
    //      The state of the retry process. See documentation of
    //      ECurrentRetryState for detailed information of each possible
    //      values. A service must not take action if the value is different
    //      than eNOT_RETRYING.
    //
    //  Returns:
    //      A result indicating whether or not other targets are available.
    //
    //  Description:
    //      Informs the request context connection service that the last packet
    //      has not been successfully sent. It can be because the connection
    //      does not exist, an ICMP error was received, or a timeout occurred.
    //
    //      For each call to UpdatePacket, this method or OnSendSuccess must be
    //      called prior to the next call to UpdatePacket.
    //
    //  See Also:
    //      UpdatePacket, OnSendSuccess, CSipPacket, ECurrentRetryState
    //
    //==EDOC====================================================================
    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState) = 0;

    //==SDOC====================================================================
    //==
    //==  UpdatePacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Modifies a packet about to be sent.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacket:
    //      The packet to be sent.
    //
    //  Returns:
    //      resS_OK:
    //          The packet has been successfully updated.
    //
    //      resSW_SIPCORE_WAIT_ASYNC_RESPONSE:
    //          Asynchronous processing is needed to query the application for
    //          packet modification.
    //
    //      resFE_SIPCORE_PACKET_BLOCKED:
    //          The packet transmission is blocked.
    //
    //  Description:
    //      Updates the internal state and the packet in parameter before it is
    //      sent on the network. It can make asynchronous process (e.g., DNS
    //      queries). It can also forbid sending the packet by returning an
    //      error code. This can be used if a particular target is known to
    //      generate timeouts, for instance.
    //
    //==EDOC====================================================================
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  ReleaseReqCtxConnectionSvcInstance
    //==
    //==========================================================================
    //
    //  Summary:
    //      Releases instance or ISipReqCtxConnectionSvc.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //  Description:
    //      Releases this instance of ISipReqCtxConnectionSvc. Since the
    //      instance allocation method is hidden by the
    //      ISipConnectionSvc::GetReqCtxConnectionSvcInstance method, the
    //      instance deallocation method must also be hidden. This allows to use
    //      reference-counted or dynamically allocated objects in the
    //      implementation of this interface. Reference-counted objects
    //      decrement the count here, while dynamically allocated objects call
    //      delete on the "this" pointer.
    //
    //  See Also:
    //      ISipConnectionSvc
    //
    //==EDOC====================================================================
    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipReqCtxConnectionSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipReqCtxConnectionSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipReqCtxConnectionSvc(IN const ISipReqCtxConnectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipReqCtxConnectionSvc& operator=(IN const ISipReqCtxConnectionSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREQCTXCONNECTIONSVC_H

