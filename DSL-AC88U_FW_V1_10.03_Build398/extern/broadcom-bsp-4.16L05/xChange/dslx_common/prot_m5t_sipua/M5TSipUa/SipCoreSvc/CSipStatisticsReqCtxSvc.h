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
#ifndef MXG_CSIPSTATISTICSREQCTXSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSTATISTICSREQCTXSVC_H
//M5T_INTERNAL_USE_END

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

#ifndef MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT to be able to use this class.
#endif

//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class ISipTransactionStatistics;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipStatisticsReqCtxSvc
//========================================
//
// Description:
//   If the Statistics Service is in stateful mode, for each outgoing ACK or
//   incoming INVITE, a CSipStatisticsReqCtxSvc is created to gather statistics
//   on the associated request context.
//
//   This request context service handles some special cases. Retransmitted 1xx
//   by the Provisional Reliable response service is handled and counted as
//   retransmission. This service also handles retransmitted 2xx to an INVITE
//   and retransmitted ACK request.
//
//
// Location:
//   SipCoreSvc/CSipStatisticsReqCtxSvc.h
//
//==EDOC========================================================================
class CSipStatisticsReqCtxSvc : public CSipReqCtxConnectionSvc
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipStatisticsReqCtxSvc(IN ISipTransactionStatistics* pTransStats);

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSipStatisticsReqCtxSvc();


    //-- << Stereotype >>
    //--------------------------------------------


    //-- << ISipReqCtxConnectionSvc >>
    //----------------------------------

    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);


//-- Hidden Methods
//-------------------
private:

    bool IsReliabilityRequired(IN const CSipPacket& rPacket);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipStatisticsReqCtxSvc(IN const CSipStatisticsReqCtxSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipStatisticsReqCtxSvc& operator=(IN const CSipStatisticsReqCtxSvc& rSrc);

    // Summary:
    //  Sets last packet sent.
    //-------------------------
    void SetLastPacketSent(IN const CSipPacket* pPacket);

//-- Hidden Data Members
//------------------------
private:
    // Statistics container.
    //-----------------------
    ISipTransactionStatistics* m_pTransStats;

    // Last packet sent.
    //-------------------
    const CSipPacket* m_pLastPacketSent;

    // RSeq contained in the last 1xx sent.
    //--------------------------------------
    uint64_t m_uRSeq;
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSTATISTICSREQCTXSVC_H

