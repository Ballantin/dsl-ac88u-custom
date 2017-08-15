//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPCONNECTIONBLACKLISTSVC_H
#define MXG_CSIPCONNECTIONBLACKLISTSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT to be able to use \
this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_NETWORKTOOLS_H
#include "Network/NetworkTools.h" // SNaptrRecord
#endif

// mxt_PFNServerLocationListModifier.
//------------------------------------
#ifndef MXG_ISIPSERVERLOCATIONSVC_H
#include "SipCoreSvc/ISipServerLocationSvc.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCOREOUTPUTCONTROLLINGSVC_H
#include "SipCoreSvc/ISipConnectionBlacklistSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipConnectionBlacklist;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipConnectionBlacklistSvc
//========================================
//
// Description:
//   This service manages the CSipConnectionBlacklist object. It adds a callback
//   method to the ISipServerLocationSvc. The callback method is in the
//   CSipConnectionBlacklist class and filters the destinations (which is an IP
//   address, a port, and a transport) that may be used.
//
//   It also adds a destination to the blacklist when sending a packet to it
//   failed. Complementary, it removes a destination from the blacklist when
//   sending a packet to it succeeded.
//
// Location:
//   SipCoreSvc/CSipConnectionBlacklistSvc.h
//
// See Also:
//   CSipConnectionBlacklist
//
//==EDOC========================================================================
class CSipConnectionBlacklistSvc : private CEComDelegatingUnknown,
                                   private ISipConnectionBlacklistSvc,
                                   private ISipConnectionSvc,
                                   private CSipReqCtxConnectionSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //--------------------------------------------

    // Summary:
    //  Filters the location list. The blacklisted ones are removed.
    //---------------------------------------------------------------
    static void LocationListFilter( IN const CHostPort& rFqdn,
                                    INOUT CList<SNaptrRecord>& rlstNaptrRecord,
                                    IN mxt_opaque opq );


//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipConnectionBlacklistSvc();

    static void FinalizeCSipConnectionBlacklistSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipConnectionBlacklistSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipConnectionBlacklistSvc();


    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result InitializeInstance();

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);


    //-- << ISipConnectionSvc >>
    //--------------------------------------------

    // Summary:
    //  Obtains an instance of an ISipReqCtxConnectionSvc to attach to a
    //  request context.
    //-------------------------------------------------------------------
    virtual ISipReqCtxConnectionSvc*
                GetReqCtxConnectionSvcInstance(
                                IN const CSipPacket& rPacket,
                                IN ISipConnectionSvc::EDirection eDirection);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual void ClearConnectionSvc();


    //-- << ISipReqCtxConnectionSvc >>
    //--------------------------------------------

    // Summary:
    //  Notifies the connection service that the packet has been successfully
    //  sent.
    //------------------------------------------------------------------------
    virtual void OnSendSuccess(IN ISipRequestContext& rRequestContext,
                               IN const CSipPacket& rPacketSent);

    // Summary:
    //  Notifies the connection service that the packet has not been
    //  successfully sent.
    //---------------------------------------------------------------
    virtual mxt_result OnSendFailure(IN ISipRequestContext& rRequestContext,
                                     IN const CSipPacket& rPacket,
                                     IN mxt_result resCause,
                                     INOUT ECurrentRetryState& reRetryState);

    // Summary:
    //  If m_bActiveMode remains true, this method does not change the packet in any way.
    //  When m_bActiveMode is false, it blocks the packets whose target addresses are
    //  in the black list.
    virtual mxt_result UpdatePacket(IN    ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket&         rPacket);

    // Summary:
    //  Releases instance or ISipReqCtxConnectionSvc.
    //------------------------------------------------
    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

    // Summary:
    //  Gets the blacklist instance in use.
    //--------------------------------------
    static CSipConnectionBlacklist* GetBlacklist();


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipConnectionBlacklistSvc(IN const CSipConnectionBlacklistSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipConnectionBlacklistSvc& operator=(
                                    IN const CSipConnectionBlacklistSvc& rSrc);


//-- Hidden Data Members
//------------------------
private:

    // This is the callback function that must be called in LocationListFilter
    // is called.
    //-------------------------------------------------------------------------
    mxt_PFNServerLocationListModifier m_pfnLocationListCallback;

    // This is the opaque value that must be passed to
    // m_pfnLocationListCallback.
    //-------------------------------------------------
    mxt_opaque m_opqCallbackParameter;

    // This flag changes the behavior of the BlackListSvc.
    // It is automatically set when this service starts.
    // When this service is used with ServerLocationSvc, this flag is set to "true".
    // This is the normal operating mode.
    // When this service is used with TargetReuseSvc, this flag is set to "false".
    // In this mode, the LocationListFilter callback is not registered, and
    // this service will intercept, in UpdatePacket(), the packets whose targets
    // are in the black list.
    bool m_bActiveMode;

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCONNECTIONBLACKLISTSVC_H

