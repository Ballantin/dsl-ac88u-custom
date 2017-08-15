//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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

//==============================================================================
//
// Portions of this software Copyright (c) 2009-2011 Broadcom
// 
//==============================================================================

#ifndef MXG_CSIPUSERAGENTSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPUSERAGENTSVC_H
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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT to be \
able to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CGENPARAMLIST_H
#include "SipParser/CGenParamList.h"
#endif

#ifndef MXG_CNAMEADDR_H
#include "SipParser/CNameAddr.h"
#endif

#ifndef MXG_CSIPHEADER_H
#include "SipParser/CSipHeader.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" // ESipTransport
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_IPRIVATESIPUSERAGENTSVC_H
#include "SipUserAgent/IPrivateSipUserAgentSvc.h"
#endif

#ifndef MXG_ISIPDIALOGMATCHER_H
#include "SipCore/ISipDialogMatcher.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CToken;
class CSipDialogMatcherList;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipUserAgentSvc
//==============================================================================
//
// Description:
//   This class implements the two ECOM interfaces ISipCoreSvc and
//   ISipUserAgentSvc. It also implements the ISipDialogMatcher to register
//   itself to the CSipDialogMatcherList.
//
// Location:
//   SipUserAgent/CSipUserAgentSvc.h
//
// See Also:
//   ISipCoreSvc, ISipUserAgentSvc, ISipDialogMatcher
//
//==EDOC========================================================================
class CSipUserAgentSvc : private CEComDelegatingUnknown,
                         private ISipCoreSvc,
                         private IPrivateSipUserAgentSvc, // ISipUserAgentSvc
                         private ISipDialogMatcher
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:

    // Summary:
    //  Sets the default Call-ID length.
    static void SetDefaultCallIdLength(IN unsigned int uCallIdLength);

    // Summary:
    //  Gets the default Call-ID length.
    static unsigned int GetDefaultCallIdLength();

//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipUserAgentSvc();

    static void FinalizeCSipUserAgentSvc();


    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipUserAgentSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipUserAgentSvc();


    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);


    //-- << ISipCoreSvc Interface >>
    //--------------------------------

    // Summary:
    //  A pointer to the ISipReqCtxCoreSvc to attach to the request context.
    //  It can be NULL.
    //-----------------------------------------------------------------------
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(
                                                   IN const CSipPacket& rPacket,
                                                   IN EDirection eDirection);

    // Summary:
    //  Establishes the confidence level for which the service is a good
    //  candidate to own a request context.
    //-------------------------------------------------------------------
    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(
                                                  IN const CSipPacket& rPacket);

    // Summary:
    //  Returns the ISipReqCtxCoreSvc instance to use as the owner for a request
    //  context.
    //--------------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();


    //-- << ISipUserAgentSvc Interface >>
    //--------------------------------------------

    // Summary:
    //  Sets basic information for the request in parameter.
    //-------------------------------------------------------
    virtual mxt_result CreateBasicRequest(INOUT CSipPacket& rPacket);

    // Summary:
    //  Adds a Contact header to put in the requests and responses sent.
    //------------------------------------------------------------------
    virtual mxt_result AddLocalContact(IN TOS CSipHeader* pHdrContact);

    // Summary:
    //  Sets the Route header to put in out of dialog requests sent.
    //--------------------------------------------------------------
    virtual mxt_result SetPreloadedRoute(IN TO CSipHeader* pPreloadedRoute);

    virtual mxt_result SetEstablishedRoute(IN TO CSipHeader* pEstablishedRoute);

    // Summary:
    //  Sets the current target where to send the requests.
    //------------------------------------------------------
    virtual mxt_result SetCurrentTarget(
            IN const IUri* pUri,
            IN ETargetHeaderOption eTargetHeaderOption = eCLEAR_TARGET_HEADERS);

    // Summary:
    //  Sets the Call-ID for this context.
    //-------------------------------------
    virtual mxt_result SetCallId(IN const CSipHeader& rCallID);

    // Summary:
    //  Sets the address to put in the To headers of outgoing requests.
    //-----------------------------------------------------------------
    virtual void SetRemoteAddr(IN const CNameAddr& rNameAddr);

    // Summary:
    //  Sets the tag parameter to put in the To header of requests sent.
    //------------------------------------------------------------------
    virtual void SetRemoteTag(IN const CString& rstrTag);

    // Summary:
    //  Sets the parameters to put in the To header of requests sent including
    //  tag.
    //-------------------------------------------------------------------------
    virtual mxt_result SetRemoteDescriptorParameters(
                                              IN TO CGenParamList* pParameters);

    // Summary:
    //  Sets the address to put in the From headers of outgoing requests.
    //-------------------------------------------------------------------
    virtual void SetLocalAddr(IN const CNameAddr& rNameAddr);

    // Summary:
    //  Sets the tag parameter to put in the From header of requests sent.
    //--------------------------------------------------------------------
    virtual void SetLocalTag(IN const CString& rstrTag);

    // Summary:
    //  Sets the parameters to put in the From header of requests sent
    //  including tag.
    //-----------------------------------------------------------------
    virtual mxt_result SetLocalDescriptorParameters(
                                              IN TO CGenParamList* pParameters);

    // Summary:
    //  Sets the sequence number to put in requests to send.
    //-------------------------------------------------------
    virtual void SetLocalSequenceNumber(IN uint32_t uSequenceNumber);

    // Summary:
    //  Sets the sequence number expected in the next request received.
    //------------------------------------------------------------------
    virtual void SetRemoteSequenceNumber(IN uint32_t uSequenceNumber);

    // Summary:
    //  Sets the sequence number expected in the next request received.
    //------------------------------------------------------------------
    virtual void SetRemoteSequenceNumber64bits(IN uint64_t uSequenceNumber);

    // Summary:
    //  Gets the Contact that is used to put in the requests and
    //  responses sent.
    //----------------------------------------------------------
    virtual const CSipHeader& GetLocalContact() const;

    // Summary:
    //  Accepts or not out of order requests.
    //------------------------------------------------------------------
    virtual void AcceptOutOfOrderRemoteRequests(IN bool bAccept);

    // Summary:
    //  Gets the configured Contact vector.
    //-------------------------------------
    virtual CVector<CSipHeader*>& GetLocalContactVector();

    // Summary:
    //  Gets the Route header to put in out of dialog requests sent.
    //--------------------------------------------------------------
    virtual const CSipHeader* GetPreloadedRoute() const;

    virtual const CSipHeader* GetEstablishedRoute() const;

    // Summary:
    //  Gets the current target where to send the requests.
    //------------------------------------------------------
    virtual const IUri* GetCurrentTarget() const;

    // Summary:
    //  Gets the Call-ID for this context.
    //-------------------------------------
    virtual const CSipHeader& GetCallId() const;

    // Summary:
    //  Gets the address to put in the To headers of outgoing requests.
    //-----------------------------------------------------------------
    virtual const CNameAddr& GetRemoteAddr() const;

    // Summary:
    //  Gets the tag parameter to put in the To header of requests sent.
    //------------------------------------------------------------------
    virtual const CString& GetRemoteTag() const;

    // Summary:
    //  Gets the parameters to put in the To header of requests sent including
    //  tag.
    //-------------------------------------------------------------------------
    virtual const CGenParamList* GetRemoteDescriptorParameters() const;

    // Summary:
    //  Gets the address to put in the From headers of outgoing requests.
    //-------------------------------------------------------------------
    virtual const CNameAddr& GetLocalAddr() const;

    // Summary:
    //  Gets the tag parameter to put in the From header of requests sent.
    //--------------------------------------------------------------------
    virtual const CString& GetLocalTag() const;

    // Summary:
    //  Gets the parameters to put in the From header of requests sent
    //  including tag.
    //-----------------------------------------------------------------
    virtual const CGenParamList* GetLocalDescriptorParameters() const;

    // Summary:
    //  Gets the sequence number to put in requests to send.
    //-------------------------------------------------------
    virtual uint32_t GetLocalSequenceNumber() const;

    // Summary:
    //  Gets the sequence number expected in the next request received.
    //------------------------------------------------------------------
    virtual uint32_t GetRemoteSequenceNumber() const;

    // Summary:
    //  Gets the sequence number expected in the next request received.
    //------------------------------------------------------------------
    virtual uint64_t GetRemoteSequenceNumber64bits() const;

    // Summary:
    //  Establish a dialog with a packet sent or received.
    //-----------------------------------------------------
    virtual mxt_result Establish(IN const CSipPacket& rPacket,
                                 IN EPacketDirection eDirection);

    // Summary:
    //  Establishes a dialog for a request that will be sent.
    //-------------------------------------------------------
    virtual mxt_result EstablishForSentRequest();

    // Summary:
    //  Completes the dialog information with the received packet.
    //------------------------------------------------------------
    virtual
        mxt_result CompleteDialogData( IN const CSipPacket& rReceivedPacket );

    // Summary:
    //  Updates the Route set of a dialog.
    //------------------------------------
    virtual mxt_result UpdateRoute(IN const CSipPacket& rPacket);

    // Summary:
    //  Terminates the usage of a dialog.
    //----------------------------------
    virtual mxt_result TerminateUsage();

    // Summary:
    //  Obtains the dialog state.
    //---------------------------
    virtual EState GetState() const;

    //  Summary:
    //      Sets the instance-id of this user-agent.
    virtual void SetInstanceId(IN const CString& rstrInstanceId);

    //  Summary:
    //      Gets the instance-id.
    virtual const CString& GetInstanceId() const;

// BRCM_CUSTOM - [add] Control To-tag matching choices
    // Summary:
    //  Configure To-tag matching option.
    //------------------------------------
    virtual void SetToTagMatchPerRfc3261(IN bool bEnable);
    //Summary:
    //   Returns the To-tag matching option.
    virtual bool IsToTagMatchPerRfc3261() const;
// BRCM_CUSTOM - [end] Control To-tag matching choices

    //-- << IPrivateSipUserAgentSvc Interface >>
    //--------------------------------------------

    // Summary:
    //  Updates the Contact header of the SIP packet.
    //------------------------------------------------
    mxt_result UpdateCurrentContact(INOUT CSipPacket& rPacket);

    // Summary:
    //  Ensures this service is correctly configured.
    //------------------------------------------------
    virtual mxt_result Validate();

    // Summary:
    //  Resets the boolean which assumes that validation succeeded.
    //-------------------------------------------------------------
    virtual void ResetValidationSucceeded();

    //-- << ISipDialogMatcher Interface >>
    //--------------------------------------------

    // Summary:
    //  Handles the packet with the parent context if it belongs to this dialog.
    //--------------------------------------------------------------------------
    virtual mxt_result OnPacketReceived(IN const CSipPacket& rPacket);

    // Summary:
    //  Sets the dialog matcher list.
    //--------------------------------
    virtual void SetDialogMatcherList(
                                IN CSipDialogMatcherList& rDialogMatcherList);

    // Summary:
    //  Obtains the dialog matcher list.
    //-----------------------------------
    virtual CSipDialogMatcherList* GetDialogMatcherList();


    //-- << Helper methods >>
    //--------------------------------------------

    static void SetTag(INOUT CGenParamList*& rpParams,
                       IN const CString& rstrTag);

    mxt_result SetDescriptorParameters(
                                    INOUT CGenParamList*& rpParamListDataMember,
                                    IN TO CGenParamList* pParameters,
                                    IN const CString& rstrListName) const;

    static const CToken& GetTag(IN const CGenParamList* pParams);

    bool MatchDialog(IN const CSipPacket& rPacket,
                     IN EPacketDirection eDirection);

    void SetRoute(IN const CSipPacket& rPacket,
                  IN EPacketDirection eDirection);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipUserAgentSvc(IN const CSipUserAgentSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipUserAgentSvc& operator=(IN const CSipUserAgentSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
    // The dialog matcher on which all services will register themself.
    //------------------------------------------------------------------
    CSipDialogMatcherList* m_pDialogMatcherList;

    // The URI to put in Request-URI.
    //--------------------------------
    IUri* m_pCurrentTarget;

    // Local Contact headers.
    //------------------------
    CVector<CSipHeader*> m_vecpHdrLocalContact;

    // Current dialog info.
    //----------------------
    CSipHeader* m_pRouteHdr;
    CSipHeader* m_pPreloadedRouteHdr;
    CSipHeader m_callIdHdr;

    CNameAddr m_localNameAddr;
    CNameAddr m_remoteNameAddr;
    CGenParamList* m_pLocalParams;
    CGenParamList* m_pRemoteParams;

    uint32_t m_uLocalSequenceNumber;
    uint64_t m_uRemoteSequenceNumber;

    unsigned int m_uNbDialogUsage;

    // This member tells if it is possible for a sent request to create a
    // dialog with a request received before a response. When this parameter is
    // true, the state returned by GetState will be
    // eWAITING_FOR_DIALOG_COMPLETION.
    //--------------------------------------------------------------------------
    bool m_bDialogToBeCompleted;

    // This member is needed to prevent adding dialogs to the dialog matcher
    // list after the Clear method has been called.
    //-----------------------------------------------------------------------
    bool m_bClearWasCalled;

    // This member keeps the validation success state for efficiency.
    //-----------------------------------------------------------------------
    bool m_bValidationSucceeded;

    // This member tells if it is possible to accept out of order requests.
    // This is false by default.
    bool m_bAcceptOutOfOrder;

    // The instance-id of this user-agent.
    CString m_strInstanceId;

    // The default Call-ID length.
    static unsigned int ms_uCallIdLength;
// BRCM_CUSTOM - [add] Control To-tag matching choices
    // This member tells if RFC3261 To-tag matching needs to be enabled.
    //-------------------------------------------------------------------
    bool m_bMatchToTagPerRfc3261;
// BRCM_CUSTOM - [end] Control To-tag matching choices

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
inline unsigned int CSipUserAgentSvc::GetDefaultCallIdLength()
{
    return ms_uCallIdLength;
}

inline const CSipHeader& CSipUserAgentSvc::GetLocalContact() const
{
    MX_ASSERT(m_vecpHdrLocalContact.GetSize() > 0);

    return *m_vecpHdrLocalContact[0];
}
inline void CSipUserAgentSvc::AcceptOutOfOrderRemoteRequests(IN bool bAccept)
{
    m_bAcceptOutOfOrder = bAccept;
}
inline CVector<CSipHeader*>& CSipUserAgentSvc::GetLocalContactVector()
{
    return m_vecpHdrLocalContact;
}

inline const CSipHeader* CSipUserAgentSvc::GetPreloadedRoute() const
{
    return m_pPreloadedRouteHdr;
}

inline const CSipHeader* CSipUserAgentSvc::GetEstablishedRoute() const
{
    return m_pRouteHdr;
}

inline const IUri* CSipUserAgentSvc::GetCurrentTarget() const
{
    return m_pCurrentTarget;
}

inline const CSipHeader& CSipUserAgentSvc::GetCallId() const
{
    return m_callIdHdr;
}

inline const CNameAddr& CSipUserAgentSvc::GetRemoteAddr() const
{
    return m_remoteNameAddr;
}

inline const CString& CSipUserAgentSvc::GetRemoteTag() const
{
    return GetTag(m_pRemoteParams).GetString();
}

inline const CGenParamList* CSipUserAgentSvc::GetRemoteDescriptorParameters() const
{
    return m_pRemoteParams;
}

inline const CNameAddr& CSipUserAgentSvc::GetLocalAddr() const
{
    return m_localNameAddr;
}

inline const CString& CSipUserAgentSvc::GetLocalTag() const
{
    return GetTag(m_pLocalParams).GetString();
}

inline const CGenParamList* CSipUserAgentSvc::GetLocalDescriptorParameters() const
{
    return m_pLocalParams;
}

inline uint32_t CSipUserAgentSvc::GetLocalSequenceNumber() const
{
    return m_uLocalSequenceNumber;
}

inline uint32_t CSipUserAgentSvc::GetRemoteSequenceNumber() const
{
    return MX_LOW32(m_uRemoteSequenceNumber);
}
inline uint64_t CSipUserAgentSvc::GetRemoteSequenceNumber64bits() const
{
    return m_uRemoteSequenceNumber;
}

// BRCM_CUSTOM - [add] Control To-tag matching choices
inline void CSipUserAgentSvc::SetToTagMatchPerRfc3261(IN bool bEnable)
{
    m_bMatchToTagPerRfc3261 = bEnable;
}
inline bool CSipUserAgentSvc::IsToTagMatchPerRfc3261() const
{
    return m_bMatchToTagPerRfc3261;
}
// BRCM_CUSTOM - [end] Control To-tag matching choices

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPUSERAGENTSVC_H

