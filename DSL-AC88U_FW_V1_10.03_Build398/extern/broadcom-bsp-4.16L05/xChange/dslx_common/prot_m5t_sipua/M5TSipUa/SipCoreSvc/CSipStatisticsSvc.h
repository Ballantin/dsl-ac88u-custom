//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPSTATISTICSSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSTATISTICSSVC_H
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

#ifndef MXG_ISIPSTATISTICSSVC_H
#include "SipCoreSvc/ISipStatisticsSvc.h"
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
#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
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
//== Class: CSipStatisticsSvc
//========================================
//
// Description:
//   This service is responsible to gather statistical data for packets that
//   are sent and received on the associated context.
//
//   Direct access to this class is prohibited, it should be accessed through
//   the ECOM mechanism via the CSipContext.
//
// Location:
//   SipCoreSvc/CSipStatisticsSvc.h
//
// See Also:
//   CStatisticsContainer, CSipStatisticsReqCtxSvc
//
//==EDOC========================================================================
class CSipStatisticsSvc : public ISipStatisticsSvc,
                          public CSipReqCtxCoreSvc,
                          public CSipReqCtxConnectionSvc,
                          public ISipCoreSvc,
                          public ISipConnectionSvc,
                          private CEComDelegatingUnknown

{
//-- Friend Declarations
//------------------------
    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipStatisticsSvc();

    static void FinalizeCSipStatisticsSvc();

    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipStatisticsSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSipStatisticsSvc();


    //-- << Stereotype >>
    //--------------------------------------------

    //-- << ISipStatisticsSvc >> --
    //------------------------------
    
    virtual void SetServiceMode(IN const EServiceMode eMode);

    virtual void SetTransactionStatistics(
                                IN ISipTransactionStatistics* pTransStats);

    virtual void GetTransactionStatistics(OUT ISipTransactionStatistics*& rpTransStats);


    //-- << ISipReqCtxCoreSvc >> --
    //------------------------------

    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    virtual unsigned int AddRcsRef();

    virtual unsigned int ReleaseRcsRef();

    //-- << ISipCoreSvc >> --
    //------------------------
    virtual
    ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                        IN const CSipPacket& rPacket,
                                        IN ISipCoreSvc::EDirection eDirection);

    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                              IN const CSipPacket& rPacket);

    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    virtual mxt_result ClearCoreSvc();

    //-- << ISipReqCtxConnectionSvc >>
    //----------------------------------

    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent);

    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);


    //-- << ISipConnectionSvc >> --
    //------------------------------
    virtual ISipReqCtxConnectionSvc*
                GetReqCtxConnectionSvcInstance(
                                IN const CSipPacket& rPacket,
                                IN ISipConnectionSvc::EDirection eDirection);

    virtual void ClearConnectionSvc();

    //-- << CEComDelegatingUnknown >>
    //--------------------------------------------

    // The following statement is essential, it provides the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //  Override CEComDelegatingUnknown
    //----------------------------------
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

//-- Hidden Methods
//-------------------
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipStatisticsSvc(IN const CSipStatisticsSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipStatisticsSvc& operator=(IN const CSipStatisticsSvc& rSrc);


//-- Hidden Data Members
//------------------------
private:
    // Statistics container.
    //-----------------------
    ISipTransactionStatistics* m_pTransStats;

    // Tells the service mode.
    //-------------------------
    EServiceMode m_eServiceMode;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline void CSipStatisticsSvc::SetServiceMode(IN const EServiceMode eMode)
{
    m_eServiceMode = eMode;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSTATISTICSSVC_H

