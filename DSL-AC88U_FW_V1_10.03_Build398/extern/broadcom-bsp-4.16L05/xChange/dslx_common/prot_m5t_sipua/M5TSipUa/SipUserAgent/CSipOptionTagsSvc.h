//==============================================================================
//==============================================================================
//
//      Copyright(c) 2009 Media5 Corporation. ("Media5")
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
//==============================================================================
#ifndef MXG_CSIPOPTIONTAGSSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPOPTIONTAGSSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT \
    to be able to use this class.
#endif


//-- Data Members.

//-- Parameters used by Value.

//-- Interface Realized and/or Parent.
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPOPTIONTAGSSVC_H
#include "SipUserAgent/ISipOptionTagsSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CSipHeader;
class ISipCoreConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipOptionTagsSvc
//========================================
//
// Description:
//   This class is the implementation of the ISipOptionTagsMgr
//   interface. Direct access to this class is prohibited, it should be accessed
//   through the ECOM mechanism via the ISipContext.
//
// Location:
//   SipUserAgentSvc/CSipOptionTagsSvc.h
//
// See Also:
//   ISipOptionTagsSvc
//
//==============================================================================
class CSipOptionTagsSvc : private CEComDelegatingUnknown,
                          private ISipCoreSvc,
                          private CSipReqCtxCoreSvc,
                          private ISipOptionTagsSvc
{
//-- Friend Declarations.

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    // To  have access to the global setters.
    friend class CSipCoreConfig;
    //M5T_INTERNAL_USE_END

//-- Hidden Methods.
protected:
private:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipOptionTagsSvc();
    static void FinalizeCSipOptionTagsSvc();

    //-- << Global extensions API >>
    //-- Only accessed by CSipCoreConfig.
    static mxt_result AddGlobalExtension(IN const CString& rstrExtension);
    static mxt_result RemoveGlobalExtension(IN const CString& rstrExtension);
    static void GetGlobalExtensions(OUT const CList<CString>*& rplststrExtension);

    //-- << ECOM mechanism >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor. Used through factory method local to this class.
    CSipOptionTagsSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipOptionTagsSvc();

    //-- << ISipOptionTagsSvc methods >>
    virtual void SetManager(IN ISipOptionTagsMgr* pMgr);
    virtual void SetCustomSupportedExtensions(IN TOA CList<CString>* plststrCustomExtensions);
    virtual mxt_result AddSupportedExtension(IN const CString& rstrExtension);
    virtual mxt_result RemoveSupportedExtension(IN const CString& rstrExtension);
    virtual void SetSupportedHeaderAddition(IN ISipOptionTagsSvc::EBehavior eOutgoingBehavior);
    virtual void SetRequireHeaderVerification(IN ISipOptionTagsSvc::EBehavior eIncomingBehavior);

    //-- << ISipCoreSvc methods >>

    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN EDirection eDirection);
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);
    virtual mxt_result ClearCoreSvc();

    //-- << ISipReqCtxCoreSvc methods >>

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

    //-- << ECOM mechanism >>

    MX_DECLARE_DELEGATING_IECOMUNKNOWN
    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

//-- Hidden Methods.
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipOptionTagsSvc(IN const CSipOptionTagsSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipOptionTagsSvc& operator=(IN const CSipOptionTagsSvc& rSrc);

    //-- << Helpers >>
    void ProcessEventHelper(IN ISipRequestContext& rRequestContext,
                            IN unsigned int uEvent,
                            IN const CSipPacket* pPacket);

//-- Hidden Data Members.
protected:
private:
    // Manager.
    ISipOptionTagsMgr* m_pMgr;

    // The custom extensions to use.
    CList<CString>* m_plststrCustomExtensions;

    // The Unsupported header to add to the response if needed.
    CSipHeader* m_pUnsupported;

    // The incoming behavior.
    ISipOptionTagsSvc::EBehavior m_eIncomingBehavior;

    // The outgoing behavior.
    ISipOptionTagsSvc::EBehavior m_eOutgoingBehavior;

    // The global list of supported extensions.
    static CList<CString>* ms_plststrSupportedExtensions;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSIPGLARESVC_H

