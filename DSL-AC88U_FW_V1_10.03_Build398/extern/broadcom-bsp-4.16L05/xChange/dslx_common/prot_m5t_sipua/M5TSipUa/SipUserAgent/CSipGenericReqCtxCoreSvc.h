//==SDOC========================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPGENERICREQCTXCORESVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPGENERICREQCTXCORESVC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)

#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_IPRIVATESIPGENERICREQCTXSVC_H
#include "SipUserAgent/IPrivateSipGenericReqCtxSvc.h"
#endif

#ifndef MXG_ISIPDIALOGSERVEREVENTCTRL_H
#include "SipCore/ISipDialogServerEventCtrl.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipGenericSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipGenericReqCtxCoreSvc
//==============================================================================
// <FLAG New in 4.1.3>
//
// Description:
//   This class is associated with a request context. It is responsible to
//   manage generic requests, such as OPTIONS, INFO, MESSAGE, etc, on behalf of
//   the generic service.
//
//   Direct access to this class is prohibited, it should be accessed through
//   the ECOM mechanism via the class ID CLSID_CSipGenericReqCtxCoreSvc. This
//   class is meant for internal use by the stack.
//
// Location:
//   SipUserAgent/CSipGenericReqCtxCoreSvc.h
//
// See Also:
//   CSipGenericSvc, ISipReqCtxCoreSvc
//
//==============================================================================
class CSipGenericReqCtxCoreSvc : private CEComUnknown,
                                 private IPrivateSipGenericReqCtxSvc,
                                 private ISipDialogServerEventCtrl
{
//-- Friend Declarations
//-----------------------
    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- New types within class namespace
//------------------------------------
public:
protected:
private:

//-- Published Interface
//-----------------------
public:

//-- Hidden Methods
//------------------
protected:
private:
    //-- << Startup mechanism >>
    //--------------------------------------------
    static mxt_result InitializeCSipGenericReqCtxCoreSvc();

    static void FinalizeCSipGenericReqCtxCoreSvc();

    //-- << ECOM mechanism >>
    //------------------------
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComUnknown >>
    //--------------------------------------------
    MX_DECLARE_IECOMUNKNOWN

    virtual mxt_result InitializeInstance();
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipGenericReqCtxCoreSvc();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipGenericReqCtxCoreSvc();

    //-- << IPrivateSipReqCtxSvc, ISipReqCtxCoreSvc >>
    //--------------------------------------------------------
    virtual void SetParentSvc(IN CSipGenericSvc& rParentSvc);

    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket);

    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket);

    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket,
                                    IN unsigned int        uCode,
                                    IN const char*         pszReason,
                                    IN TO CHeaderList*     pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody);

    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket);

    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    virtual unsigned int AddRcsRef();

    virtual unsigned int ReleaseRcsRef();

    //-- << ISipDialogServerEventCtrl >>
    //-----------------------------------
    virtual mxt_result SendDialogEstablishingResponse(IN unsigned int uCode,
                                                      IN const char* pszReason,
                                                      IN TO CHeaderList* pExtraHeaders,
                                                      IN TO CSipMessageBody* pMessageBody);

    //-- << Helpers >>
    //-----------------
    // Summary:
    //  This method calls the ISipUserAgentSvc::Establish method when required.
    //--------------------------------------------------------------------------
    mxt_result EstablishDialog(IN CSipPacket& rResponse);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipGenericReqCtxCoreSvc(IN const CSipGenericReqCtxCoreSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipGenericReqCtxCoreSvc& operator=(IN const CSipGenericReqCtxCoreSvc& rSrc);


//-- Hidden Data Members
//-----------------------
protected:
private:

    // Enum kept to manage incoming events from the network. Those events are
    // the reception of requests or any provisional or final responses.
    //------------------------------------------------------------------------
    enum EGenericEvent
    {
        eINCOMING_REQUEST,
        eINCOMING_PROVISIONAL_RESPONSE,
        eINCOMING_SUCCESS_RESPONSE,
        eINCOMING_FAILURE_RESPONSE
    };

    // This member is to be able to Query interfaces correctly on the
    // CSipRequestContext. If the original IEComUnknown interface of the
    // CSipRequestContext is not kept, the aggregation makes the
    // m_pReqCtx->QueryIf() call map to the
    // CSipGenericReqCtxCoreSvc::NonDelegatingQueryIf() and it becomes
    // impossible to query interfaces on the CSipRequestContext object.
    //-------------------------------------------------------------------
    IEComUnknown* m_pReqCtxUnknown;

    // This holds a pointer to the parent generic service, it is used to query
    // the list of services that needs to see the packets and to access the
    // service's manager.
    //-------------------------------------------------------------------------
    CSipGenericSvc* m_pParentSvc;

    // State information to activate the dialog creation when required.
    //------------------------------------------------------------------
    bool m_bResponseCreatesDialog;
};
//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)

#endif //-- #ifndef MXG_CSIPGENERICREQCTXCORESVC_H

