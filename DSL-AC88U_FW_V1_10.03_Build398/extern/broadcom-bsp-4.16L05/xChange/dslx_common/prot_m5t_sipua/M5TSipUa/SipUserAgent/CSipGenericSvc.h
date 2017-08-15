//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPGENERICSVC_H
#define MXG_CSIPGENERICSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT to be able \
to use this class.
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSIPCLIENTSVC_H
#include "SipUserAgent/CSipClientSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_ISIPGENERICSVC_H
#include "SipUserAgent/ISipGenericSvc.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class ISipGenericMgr;
class IPrivateSipGenericReqCtxSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipGenericSvc
//========================================
//
// Description:
//   This class is the implementation of the ISipGenericSvc interface. It allows
//   the application to manage simultaneously several generic requests such as
//   OPTIONS, INFO, MESSAGE, etc. Direct access to this class is prohibited, it
//   should be accessed through the ECOM mechanism via the ISipContext.
//
// Location:
//   SipUserAgent/CSipGenericSvc.h
//
// See Also:
//   CSipClientSvc, ISipCoreSvc, ISipGenericSvc, ISipContext
//
//==EDOC========================================================================
class CSipGenericSvc : private CSipClientSvc,
                       private ISipCoreSvc,
                       private ISipGenericSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
    // GCC 2.7.2 does not handle well the overridden QueryIf when implemented by
    // multiple interfaces, in which case the following class needs to specify
    // on which private interface the QueryIf must be called.
    friend class CSipGenericReqCtxCoreSvc;
#endif
    //M5T_INTERNAL_USE_END

//-- New types within class namespace.
//-------------------------------------
public:

    // Summary:
    //  This method is used by request context services created by the generic
    //  service to access its manager.
    //-------------------------------------------------------------------------
    ISipGenericMgr* GetManager();

    //-- Public so the child request context services can access AddIfRef(),
    //-- ReleaseIfRef() and QueryIf().
    //-----------------------------------------------------------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //---------------------------

    static mxt_result InitializeCSipGenericSvc();

    static void FinalizeCSipGenericSvc();


    //-- << ECOM mechanism >>
    //------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipGenericSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipGenericSvc();


    //-- << CEComUnknown >>
    //----------------------
    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << CSipClientSvc >>
    //-----------------------

    virtual void GetRequestContext(IN mxt_opaque opqTransaction,
                                   IN const CToken& rMethod,
                                   OUT ISipRequestContext*& rpRequestContext);

    virtual void RequestContextInError(IN ISipRequestContext& rRequestContext);

    //-- << ISipCoreSvc >>
    //---------------------

    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN EDirection eDirection);

    virtual ISipCoreSvc::EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    virtual mxt_result ClearCoreSvc();

    //-- << ISipGenericSvc >>
    //------------------------

    virtual mxt_result SetManager(IN ISipGenericMgr* pMgr);

    virtual mxt_result SendRequest(
                               IN    const char* szMethod,
                               IN    mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT   ISipClientTransaction*& rpTransaction);


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipGenericSvc(IN const CSipGenericSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipGenericSvc& operator=(IN const CSipGenericSvc& rSrc);


//-- Hidden Data Members.
//------------------------
protected:

    // The event manager used to report events to the application.
    //-------------------------------------------------------------
    ISipGenericMgr* m_pMgr;

private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
//==SDOC========================================================================
//==
//==  GetManager
//==
//==============================================================================
//
//  Returns:
//    A pointer to the generic service's manager interface. It returns NULL
//    after ClearCoreSvc() has been called.
//
//  Description:
//      This method is used by request context services created by the generic
//      service to access its manager.
//
//==EDOC========================================================================
inline ISipGenericMgr* CSipGenericSvc::GetManager()
{
    return m_pMgr;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPGENERICSVC_H
