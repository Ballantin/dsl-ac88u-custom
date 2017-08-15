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
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPCONNECTIONBLACKLIST_H
#define MXG_CSIPCONNECTIONBLACKLIST_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT  to be able to use \
this class.
#endif


//-- Data Members.
//-----------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Parameters used by Value.
//-----------------------------
#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" //-- ESipTransport
#endif

#ifndef MXG_ISIPCONNECTIONBLACKLISTMGR_H
#include "SipCoreSvc/ISipConnectionBlacklistMgr.h"
#endif

//-- Interface Realized and/or Parent.
//-------------------------------------
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------
class ISipConnectionBlacklistMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipConnectionBlacklist
//========================================
//
// Description:
//   This class implements a destination blacklist. A destination (IP address,
//   port number, and transport) should be added to the blacklist when there is
//   knowledge that it should not be used. It should be removed when one knows 
//   that the destination can now be used again.
//
//   The destinations in the blacklist are not tried by the M5T SIP stack to
//   communicate with the peer unless all the destinations are in the blacklist.
//   In this case, the preferred destination is used.
//
//   Destinations are left in the blacklist for a configurable amount of time
//   after which they are automatically removed from the list.
//
//   The blacklist is also automatically managed by the
//   ISipConnectionBlacklistSvc if it is added to the connection services of a
//   context. The service adds a destination to the list when sending something
//   to it failed and removes a destination from the list when sending something
//   to it succeeded. The service also sets LocationListFilter to be used when 
//   the destinations are available. This method filters the destinations and 
//   removes the ones that are in its list.
//
//   IMPORTANT: All accesses to this class MUST be done from the g_pCoreThread
//   thread.
//
// Location:
//   SipCoreSvc/CSipConnectionBlacklist.h
//
// See Also:
//   ISipConnectionBlacklistSvc
//
//==EDOC========================================================================
class CSipConnectionBlacklist : public CEventDriven
{
//-- Friend Declarations.
//------------------------

//-- Published Interface.
//------------------------
public:
    //-- << Stereotype >>
    //--------------------

    // Summary:
    //  Adds the destination to the blacklist.
    //-----------------------------------------
    mxt_result AddToBlacklist(IN const CSocketAddr& rSocketAddr,
                              IN ESipTransport eTransport);

    // Summary:
    //  Removes the destination from the blacklist.
    //----------------------------------------------
    mxt_result RemoveFromBlacklist(IN const CSocketAddr& rSocketAddr,
                                   IN ESipTransport eTransport);

    // Summary:
    //  Tests if the destination is in the blacklist.
    //------------------------------------------------
    bool IsBlacklisted(IN const CSocketAddr& rSocketAddr,
                       IN ESipTransport eTransport) const;

    // Summary:
    //  Sets the amount of time a destination is kept on the blacklist.
    //------------------------------------------------------------------
    void SetBlacklistDuration(IN uint64_t uDurationInMs);

    // Summary:
    //  Sets the connection blacklist manager to which events will be reported.
    //--------------------------------------------------------------------------
    void SetManager( IN ISipConnectionBlacklistMgr* pMgr );

    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipConnectionBlacklist();

//-- Hidden Methods.
//-------------------
protected:
    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipConnectionBlacklist();

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipConnectionBlacklist(IN const CSipConnectionBlacklist& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipConnectionBlacklist& operator=(IN const CSipConnectionBlacklist& rSrc);


    //-- << ITimerServiceMgr >>
    //--------------------------

    // Summary:
    //  Notifies the manager that a new timer has elapsed or has been stopped.
    //-------------------------------------------------------------------------
    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);


    //-- << Helpers >>
    //-----------------

    // Summary:
    //  Gets the destination in the blacklist.
    //-----------------------------------------
    unsigned int GetBlacklisted(IN const CSocketAddr& rSocketAddr,
                                IN ESipTransport eTransport) const;

    // Summary:
    //  Get the destination in the blacklist.
    //----------------------------------------
    unsigned int GetBlacklisted(IN mxt_opaque opq) const;

//-- Hidden Data Members.
//------------------------
protected:
private:
    struct SDestination
    {
        CSocketAddr* m_pSocketAddr;
        ESipTransport m_eTransport;
        unsigned int m_uTimerId;

        SDestination(const CSocketAddr& rSourceAddr, ESipTransport eSourceTransport, unsigned int uTimerId)
        :   m_pSocketAddr(NULL),
            m_eTransport(eSourceTransport),
            m_uTimerId(uTimerId)
        {
            m_pSocketAddr = MX_NEW(CSocketAddr)(rSourceAddr);
        }

        ~SDestination()
        {
            MX_DELETE(m_pSocketAddr);
        }
    private:
        // Summary:
        //  Default Constructor.
        //-----------------------
        SDestination();
    };

    // This vector contains the blacklisted destinations.
    //----------------------------------------------------
    CVector<SDestination*> m_vecpstBlacklistedDestination;

    // The duration for which the destinations will be kept on the blacklist.
    // Will be used to start the timers.
    //------------------------------------------------------------------------
    uint64_t m_uBlacklistDuration;

    ISipConnectionBlacklistMgr* m_pConnectionBlacklistMgr;

    static const unsigned int ms_uINVALID_INDEX;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCONNECTIONBLACKLIST_H

