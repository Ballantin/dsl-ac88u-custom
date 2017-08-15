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
#ifndef MXG_CSIPCONTEXT_H
#define MXG_CSIPCONTEXT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING
#endif

//-- Data Members
//-----------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IPRIVATESIPCONTEXT_H
#include "SipCore/IPrivateSipContext.h"
#endif

#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING
    #ifndef MXG_CWATCHEDINSTANCE_H
    #include "SipTransport/CWatchedInstance.h"
    #endif
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipReqCtxCoreSvc;
class ISipReqCtxConnectionSvc;
class ISipContextObserver;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipContext
//========================================
//
// Description:
//  This class implements the ISipContext interface and aggregates services. It
//  offers some more APIs than the ones defined in the ISipContext interface,
//  which are used privately within the SipCore.
//
// Location:
//   SipCore/CSipContext.h
//
// See Also:
//   ISipContext
//
//==EDOC========================================================================
class CSipContext : private IPrivateSipContext,
                    private CEComUnknown
#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING
                    , public CWatchedInstance<CSipContext>
#endif
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
#ifdef MXD_SIPSTACK_ENABLE_SIPCONTEXT_INSTANCE_TRACKING
    friend class CInstanceTracker;      // To cast back to ISipContext
#endif
#ifdef MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING
    friend class CDeprecatedInstanceTracker; // To cast back to ISipContext
#endif
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    mxt_opaque GetDnsUserOpaqueHelper();
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    mxt_opaque GetGuidOpaqueHelper();
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
    mxt_opaque GetTosOpaqueHelper();
// BRCM_CUSTOM - [end] Add TOS config support
//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipContext();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipContext();

#if defined MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING
    //-- << CWatchedInstance Mechanism >>

    // Summary:
    //  Gets a CBlob containing information about the ISipContext still
    //  allocated.
    static void Dump(OUT CBlob& rblobContextDump);
#endif

private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipContext();

    static void FinalizeCSipContext();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComUnknown Interface >>.
    //---------------------------------
    MX_DECLARE_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISipContext Interface >>.
    //--------------------------------

    // Summary:
    //  Attaches a service to the context.
    //-----------------------------------------------------------------
    virtual mxt_result AttachService(IN mxt_clsid classId);

    // Summary:
    //  Tries to handle an incoming packet.
    //--------------------------------------
    virtual mxt_result OnPacketReceived(IN const CSipPacket& rPacket);

    // Summary:
    //  Prepares the context to be deleted.
    //--------------------------------------
    virtual mxt_result Clear();

    // Summary:
    //  Sets opaque data.
    //--------------------
    virtual void SetOpaque(IN mxt_opaque opaque);

    // Summary:
    //  Gets opaque data.
    //--------------------
    virtual mxt_opaque GetOpaque();

    // Summary:
    //  Sets the entity ID.
    virtual void SetEntityId(IN unsigned int uEntityId);

    // Summary:
    //  Gets the entity ID.
    virtual unsigned int GetEntityId();

    // Summary:
    //  Adds a observer to this context.
    virtual mxt_result AddObserver(ISipContextObserver* pObserver);

    // Summary:
    //  Removes the given observer.
    virtual mxt_result RemoveObserver(ISipContextObserver* pObserver);
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    virtual mxt_opaque GetDnsUserOpaque();
    virtual void SetDnsUserOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    virtual mxt_opaque GetGuidOpaque();
    virtual void SetGuidOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
    virtual mxt_opaque GetTosOpaque();
    virtual void SetTosOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add TOS config support

    //-- << IPrivateSipContext Interface >>
    //--------------------------------------------

    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    virtual mxt_result SetForkedDialogGrouper(IN CSipForkedDialogGrouper* pGrouper);

    virtual void GetForkedDialogGrouper(OUT CSipForkedDialogGrouper*& rpGrouper);

    // Summary:
    //  Retrieves a vector of interested services for a particular SIP
    //  transaction.
    virtual void GetReqCtxCoreSvc(IN const CSipPacket& rPacket,
                                  IN ISipCoreSvc::EDirection eDirection,
                                  INOUT CVector<ISipReqCtxCoreSvc*>& rvecpSvc);

    // Summary:
    //  Retrieves a vector of interested connection services for a new SIP
    //  transaction.
    virtual void GetReqCtxConnectionSvc(IN const CSipPacket& rPacket,
                                        IN ISipConnectionSvc::EDirection eDirection,
                                        INOUT CVector<ISipReqCtxConnectionSvc*>& rvecpSvc);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipContext(IN const CSipContext& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipContext& operator=(IN const CSipContext& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    CSipForkedDialogGrouper* m_pForkedDialogGrouper;

    CVector<IEComUnknown*> m_vecpServices;

    mxt_opaque m_opaque;

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    mxt_opaque m_opqDnsUser;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
// BRCM_CUSTOM - [add] Add CCTK GUID opaque
    mxt_opaque m_opqGuid;
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
    mxt_opaque m_opqTos;
// BRCM_CUSTOM - [end] Add TOS config support

    unsigned int m_uEntityId;

    // The vector of observers. Each object may have many observers.
    CVector<ISipContextObserver*> m_vecpContextObservers;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline void CSipContext::SetEntityId(IN unsigned int uEntityId)
{
    m_uEntityId = uEntityId;
}

inline unsigned int CSipContext::GetEntityId()
{
    return m_uEntityId;
}

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
inline mxt_opaque CSipContext::GetDnsUserOpaqueHelper()
{
    return m_opqDnsUser;
}

inline mxt_opaque CSipContext::GetDnsUserOpaque()
{
    return CSipContext::GetDnsUserOpaqueHelper();
}

inline void CSipContext::SetDnsUserOpaque(IN mxt_opaque opq)
{
    m_opqDnsUser = opq;
}
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// BRCM_CUSTOM - [add] Add CCTK GUID opaque
inline mxt_opaque CSipContext::GetGuidOpaqueHelper()
{
    return m_opqGuid;
}

inline mxt_opaque CSipContext::GetGuidOpaque()
{
    return CSipContext::GetGuidOpaqueHelper();
}

inline void CSipContext::SetGuidOpaque(IN mxt_opaque opq)
{
    m_opqGuid = opq;
}
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
inline mxt_opaque CSipContext::GetTosOpaqueHelper()
{
    return m_opqTos;
}

inline mxt_opaque CSipContext::GetTosOpaque()
{
    return CSipContext::GetTosOpaqueHelper();
}

inline void CSipContext::SetTosOpaque(IN mxt_opaque opq)
{
    m_opqTos = opq;
}
// BRCM_CUSTOM - [end] Add TOS config support

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCONTEXT_H

