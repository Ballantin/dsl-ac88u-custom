//==============================================================================
//==============================================================================
//
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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

//==============================================================================
//
// Portions of this software Copyright (c) 2008-2011 Broadcom
// 
//==============================================================================

#ifndef MXG_CSCEUSERCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEUSERCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members.
//-----------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CNAMEADDR_H
#include "SipParser/CNameAddr.h"
#endif


//-- Parameters used by Value.
//-----------------------------


//-- Interface Realized and/or Parent.
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEUSERCONFIG_H
#include "SceCoreComponents/ISceUserConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------
class CSipHeader;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceUserConfig
//========================================
//
// Summary:
//  This class implements the ISceUserConfig and is responsible to allocate
//  other registered user configuration components.
//
// Description:
//  This class implements the ISceUserConfig and is responsible to
//  allocate other registered user configuration components. This is an ECom
//  component which aggregates registered user configuration ECom components. As
//  a result, any user configuration component interfaces may be queried from
//  this component's interface.
//
// Location:
//  SceCoreComponents/CSceUserConfig.h
//
//==============================================================================
class CSceUserConfig : private CEComDelegatingUnknown,
                       private ISceUserConfig
{
//-- Friend Declarations.
//------------------------

    friend class CSceCoreComponentsInitializer;


//-- Published Interface.
//------------------------
public:

// BRCM_CUSTOM - [add] Add support for appending @host to Call-Id /
//                     Add support for disabling symmetric UDP /
//                     Control To-tag matching choices /
//                     Control behavior when no P-Associated-URI is received /
//                     Do not force validation of the P-Associated-URI content /
//                     Control registration failover behavior on all responses /
//                     Control reg-event interpretation of 'rejected' as /
//                     'deactivated' /
//                     Interpret subscription failure with 500 as need to retry. /
//                     Do not failover on 403 response to registration /
//                     Control silent termination of active calls. /
//                     Do not end subscription on refresh failures. /
//                     include default port number in contact header. /
//                     Include default port number in via header.
private:

    enum EBrcmExtraCfgs
    {
        eEXTRACFGS_NONE                             = 0x00000000,
        eEXTRACFGS_NO_HOST_IN_CALLID                = 0x00000001,
        eEXTRACFGS_NO_SYMMETRIC_UDP                 = 0x00000002,
        eEXTRACFGS_NO_TO_TAG_MATCH                  = 0x00000004,
        eEXTRACFGS_NO_PAU_UNREG                     = 0x00000008,
        eEXTRACFGS_PAU_NO_VALIDATE                  = 0x00000010,
        eEXTRACFGS_REG_FVR_ON_ALL_RSP               = 0x00000020,
        eEXTRACFGS_REJ_CTC_AS_DEACT_CTC             = 0x00000040,
        eEXTRACFGS_SUB_FAIL_500_RET                 = 0x00000080,
        eEXTRACFGS_REG_END_ON_403                   = 0x00000100,
        eEXTRACFGS_NO_SILENT_ACT_CALL_TERM          = 0x00000200,
        eEXTRACFGS_KEEP_SUB_ON_REFRESH_FAIL         = 0x00000400,
        eEXTRACFGS_DEFAULT_PORT_IN_CONTACT_HDR      = 0x00000800,
        eEXTRACFGS_DEFAULT_PORT_IN_VIA_HDR          = 0x00001000,
        eEXTRACFGS_PAI_IGNORE_INVALID_URI           = 0x00002000,
        eEXTRACFGS_REG_FVR_ON_STDERR                = 0x00004000,
        eEXTRACFGS_REG_FVR_ON_305                   = 0x00008000,
    };
// BRCM_CUSTOM - [end] Add support for appending @host to Call-Id /
//                     Add support for disabling symmetric UDP /
//                     Control To-tag matching choices /
//                     Control behavior when no P-Associated-URI is received /
//                     Do not force validation of the P-Associated-URI content /
//                     Control registration failover behavior on all responses /
//                     Control reg-event interpretation of 'rejected' as /
//                     'deactivated' /
//                     Interpret subscription failure with 500 as need to retry. /
//                     Do not failover on 403 response to registration /
//                     include default port number in contact header. /
//                     Include default port number in via header. /
//                     Ignore invalid uri in P-Asserted-Identity header

//-- Hidden Methods.
//-------------------
private:
    //-- << Startup mechanism >>
    //---------------------------
    static mxt_result InitializeCSceUserConfig();
    static void FinalizeCSceUserConfig();


    //-- << ECOM mechanism >>
    //------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSceUserConfig(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSceUserConfig();

    //-- << CEComDelegatingUnknown >>
    //----------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual unsigned int NonDelegatingReleaseIfRef();

    virtual mxt_result InitializeInstance();
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //   << ISceUserConfig >>
    //-------------------------------
    virtual void SetInstanceId(IN const CString& rstrInstanceId);
    virtual const CString& GetInstanceId() const;
    virtual void SetUserAddress(IN const CNameAddr& rNameAddr);
    virtual const CNameAddr& GetUserAddress() const;
    virtual mxt_result SetPreLoadedRoute(IN TO CSipHeader* pRouteHeaders);
    virtual const CSipHeader* GetPreLoadedRoute() const;
    virtual mxt_result SetServiceRoute(IN TO CSipHeader* pRouteHeaders);
    virtual const CSipHeader* GetServiceRoute() const;
    virtual void SetServiceRouteTreatment(IN EServiceRouteTreatment eTreatment);
    virtual EServiceRouteTreatment GetServiceRouteTreatment() const;
    virtual void SetLocalContactIdentity(IN const CString& pstrUserPart);
    virtual const CString& GetLocalContactIdentity() const;
    virtual mxt_result SetForcedContact(IN TOA CSipHeader* pContactHdr);
    virtual const CSipHeader* GetForcedContact() const;
    virtual void SetForceDefaultPortInContact(IN bool bForce);
    virtual bool GetForceDefaultPortInContact();
    virtual void SetTelUriBehavior(IN ETelUriBehavior eBehavior);
    virtual ETelUriBehavior GetTelUriBehavior() const;
    virtual void SetOpaque(IN mxt_opaque opq);
    virtual mxt_opaque GetOpaque();
    virtual void SetDscp(IN uint32_t uDscp);
    virtual uint32_t GetDscp();
    virtual unsigned int IncrementNumCircularReferences();
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    virtual mxt_opaque GetDnsUserOpaque() const;
    virtual void SetDnsUserOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
// BRCM_CUSTOM - [add] Add support for appending @host to Call-Id
    virtual void SetHostInCallId(IN bool bEnable);
    virtual bool IsHostInCallId() const;
// BRCM_CUSTOM - [end] Add support for appending @host to Call-Id
// BRCM_CUSTOM - [add] Add support for disabling symmetric UDP
    virtual void SetSymmetricUdp(IN bool bEnable);
    virtual bool IsSymmetricUdp() const;
// BRCM_CUSTOM - [end] Add support for disabling symmetric UDP
// BRCM_CUSTOM - [add] Control To-tag matching choices
    virtual void SetToTagMatch(IN bool bEnable);
    virtual bool IsToTagMatch() const;
// BRCM_CUSTOM - [end] Control To-tag matching choices
// BRCM_CUSTOM - [add] Control behavior when no P-Associated-URI is received
    virtual void SetNoPauUnreg(IN bool bEnable);
    virtual bool IsNoPauUnreg() const;
// BRCM_CUSTOM - [end] Control behavior when no P-Associated-URI is received
// BRCM_CUSTOM - [add] Do not force validation of the P-Associated-URI content
    virtual void SetPauNoValidate(IN bool bEnable);
    virtual bool IsPauNoValidate() const;
// BRCM_CUSTOM - [end] Do not force validation of the P-Associated-URI content
// BRCM_CUSTOM - [add] Ignore invalid uri in P-Asserted-Identity header
    virtual void SetPaiIgnoreInvalidUri(IN bool bEnable);
    virtual bool IsPaiIgnoreInvalidUri() const;
// BRCM_CUSTOM - [add] Ignore invalid uri in P-Asserted-Identity header
// BRCM_CUSTOM - [add] Control registration failover behavior on all responses
    virtual void SetRegFvrAllRsp(IN bool bEnable);
    virtual bool IsRegFvrAllRsp() const;
// BRCM_CUSTOM - [end] Control registration failover behavior on all responses
// BRCM_CUSTOM - [add] Control registration failover behavior on standard
//                     set of failover responses ( 408, 500, 504, 600)
   virtual void SetRegFvrOnStdErr(IN bool bEnable);
   virtual bool IsRegFvrOnStdErr() const;
// BRCM_CUSTOM - [end] Control registration failover behavior on standard
// BRCM_CUSTOM - [add] Control registration failover behavior on 305 error response
   virtual void SetRegFvrOn305(IN bool bEnable);
   virtual bool IsRegFvrOn305() const;
// BRCM_CUSTOM - [end] Control registration failover behavior on 305 error response
// BRCM_CUSTOM - [add] Control whether to apply registration failover on 403
//               Note that if Retry-After is included in the 403 response or 
//               pre-configured, then it will override the RegEndOn403 behavior
    virtual void SetRegEndOn403(IN bool bEnable);
    virtual bool IsRegEndOn403() const;
// BRCM_CUSTOM - [end] Control whether to apply registration failover on 403
// BRCM_CUSTOM - [add] Control reg-event interpretation of 'rejected' as
//                     'deactivated'
    virtual void SetRejCtcAsDeactCtc(IN bool bEnable);
    virtual bool IsRejCtcAsDeactCtc() const;
// BRCM_CUSTOM - [end] Control reg-event interpretation of 'rejected' as
//                     'deactivated'
// BRCM_CUSTOM - [add] Control subscription failure re-attempt behavior on
//                     500 response
    virtual void SetSubFail500(IN bool bEnable);
    virtual bool IsSubFail500() const;
// BRCM_CUSTOM - [end] Control subscription failure re-attempt behavior on
//                     500 response
// BRCM_CUSTOM - [add] Control whether to maintain subscription on refresh failures
    virtual void SetKeepSubOnRefreshFail(IN bool bEnable);
    virtual bool IsKeepSubOnRefreshFail() const;
// BRCM_CUSTOM - [end] Control whether to maintain subscription on refresh failures
// BRCM_CUSTOM - [add] Control whether to silently terminate active calls
    virtual void SetActCallNoSilentTerm(IN bool bEnable);
    virtual bool IsActCallNoSilentTerm() const;
// BRCM_CUSTOM - [end] Control whether to silently terminate active calls
// BRCM_CUSTOM - [add] Control whether to include the default (5060 or 5061) port in contact header
    virtual void IncludeDefaultPortInContactHeader(IN bool bEnable);
    virtual bool IncludeDefaultPortInContactHeader() const;
// BRCM_CUSTOM - [end] Control whether to include the default (5060 or 5061) port in contact header
// BRCM_CUSTOM - [add] Control whether to include the default (5060 or 5061) port in via header
    virtual void IncludeDefaultPortInViaHeader(IN bool bEnable);
    virtual bool IncludeDefaultPortInViaHeader() const;
// BRCM_CUSTOM - [end] Control whether to include the default (5060 or 5061) port in via header

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSceUserConfig(IN const CSceUserConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSceUserConfig& operator=(IN const CSceUserConfig& rSrc);


//-- Hidden Data Members.
//------------------------
private:

    mxt_opaque                  m_opq;
    CString                     m_strInstanceId;
    CNameAddr                   m_userAddr;
    CSipHeader*                 m_pPreLoadedRouteHeaders;
    CSipHeader*                 m_pServiceRouteHeaders;
    EServiceRouteTreatment      m_eServiceRouteTreatment;
    CString                     m_strLocalContactUserPart;
    CSipHeader*                 m_pForcedContact;
    bool                        m_bForceDefaultPortInContact;
    ETelUriBehavior             m_eTelUriBehavior;
    uint32_t                    m_uDscp;
    CVector<IEComUnknown*>*     m_pvecpUsrConfigObject;

    // This data member is used to count the number of circular reference
    // on the user config. It needs to be incremented when a reference can only
    // be broken by terminating the ISceUserConfig. It is used in
    // NonDelegatingReleaseIfRef.
    unsigned int                m_uNumCircularReferences;
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    mxt_opaque                  m_opqDnsUser;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
// BRCM_CUSTOM - [add] Add support for appending @host to Call-Id /
//                     Add support for disabling symmetric UDP /
//                     Control To-tag matching choices
    uint32_t                    m_uBrcmExtraCfgs;
// BRCM_CUSTOM - [end] Add support for appending @host to Call-Id /
//                     Add support for disabling symmetric UDP /
//                     Control To-tag matching choices

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
inline mxt_opaque CSceUserConfig::GetDnsUserOpaque() const
{
    return m_opqDnsUser;
}

inline void CSceUserConfig::SetDnsUserOpaque(IN mxt_opaque opq)
{
    m_opqDnsUser = opq;
}
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// BRCM_CUSTOM - [add] Add support for appending @host to Call-Id
inline void CSceUserConfig::SetHostInCallId(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_NO_HOST_IN_CALLID;
    }
    else
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_NO_HOST_IN_CALLID;
    }
}

inline bool CSceUserConfig::IsHostInCallId() const
{
    return ((m_uBrcmExtraCfgs & eEXTRACFGS_NO_HOST_IN_CALLID) == 0);
}
// BRCM_CUSTOM - [end] Add support for appending @host to Call-Id

// BRCM_CUSTOM - [add] Add support for disabling symmetric UDP
inline void CSceUserConfig::SetSymmetricUdp(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_NO_SYMMETRIC_UDP;
    }
    else
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_NO_SYMMETRIC_UDP;
    }
}

inline bool CSceUserConfig::IsSymmetricUdp() const
{
    return ((m_uBrcmExtraCfgs & eEXTRACFGS_NO_SYMMETRIC_UDP) == 0);
}
// BRCM_CUSTOM - [end] Add support for disabling symmetric UDP

// BRCM_CUSTOM - [add] Control To-tag matching choices
inline void CSceUserConfig::SetToTagMatch(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_NO_TO_TAG_MATCH;
    }
    else
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_NO_TO_TAG_MATCH;
    }
}

inline bool CSceUserConfig::IsToTagMatch() const
{
    return ((m_uBrcmExtraCfgs & eEXTRACFGS_NO_TO_TAG_MATCH) == 0);
}
// BRCM_CUSTOM - [end] Control To-tag matching choices

// BRCM_CUSTOM - [add] Control behavior when no P-Associated-URI is received
inline void CSceUserConfig::SetNoPauUnreg(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_NO_PAU_UNREG;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_NO_PAU_UNREG;
    }
}

inline bool CSceUserConfig::IsNoPauUnreg() const
{
    return ((m_uBrcmExtraCfgs & eEXTRACFGS_NO_PAU_UNREG) != 0);
}
// BRCM_CUSTOM - [end] Control behavior when no P-Associated-URI is received

// BRCM_CUSTOM - [add] Do not force validation of the P-Associated-URI content
inline void CSceUserConfig::SetPauNoValidate(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_PAU_NO_VALIDATE;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_PAU_NO_VALIDATE;
    }
}

inline bool CSceUserConfig::IsPauNoValidate() const
{
    return ((m_uBrcmExtraCfgs & eEXTRACFGS_PAU_NO_VALIDATE) != 0);
}
// BRCM_CUSTOM - [end] Do not force validation of the P-Associated-URI content

// BRCM_CUSTOM - [add] Ignore invalid uri in P-Asserted-Identity header
inline void CSceUserConfig::SetPaiIgnoreInvalidUri( IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_PAI_IGNORE_INVALID_URI;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_PAI_IGNORE_INVALID_URI;
    }
}

inline bool CSceUserConfig::IsPaiIgnoreInvalidUri() const
{
    return ((m_uBrcmExtraCfgs & eEXTRACFGS_PAI_IGNORE_INVALID_URI) != 0);
}
// BRCM_CUSTOM - [end] Ignore invalid uri in P-Asserted-Identity header

// BRCM_CUSTOM - [add] Control registration failover behavior on all responses
inline void CSceUserConfig::SetRegFvrAllRsp(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_REG_FVR_ON_ALL_RSP;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_REG_FVR_ON_ALL_RSP;
    }
}

inline bool CSceUserConfig::IsRegFvrAllRsp() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_REG_FVR_ON_ALL_RSP) != 0);
}
// BRCM_CUSTOM - [end] Control registration failover behavior on all responses

// BRCM_CUSTOM - [add] Control registration failover behavior on standard
//                     set of failover responses ( 408, 500, 504, 600)
inline void CSceUserConfig::SetRegFvrOnStdErr(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_REG_FVR_ON_STDERR;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_REG_FVR_ON_STDERR;
    }
}

inline bool CSceUserConfig::IsRegFvrOnStdErr() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_REG_FVR_ON_STDERR) != 0);
}
// BRCM_CUSTOM - [end] Control registration failover behavior on standard
//                     set of failover responses ( 408, 500, 504, 600)

// BRCM_CUSTOM - [add] Control registration failover behavior on 305 error response
inline void CSceUserConfig::SetRegFvrOn305(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_REG_FVR_ON_305;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_REG_FVR_ON_305;
    }
}

inline bool CSceUserConfig::IsRegFvrOn305() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_REG_FVR_ON_305) != 0);
}
// BRCM_CUSTOM - [end] Control registration failover behavior on 305 error response

// BRCM_CUSTOM - [add] Control whether to apply registration failover on 403
inline void CSceUserConfig::SetRegEndOn403(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_REG_END_ON_403;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_REG_END_ON_403;
    }
}

inline bool CSceUserConfig::IsRegEndOn403() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_REG_END_ON_403) != 0);
}
// BRCM_CUSTOM - [end] Control whether to apply registration failover on 403

// BRCM_CUSTOM - [add] Control reg-event interpretation of 'rejected' as
//                     'deactivated'
inline void CSceUserConfig::SetRejCtcAsDeactCtc(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_REJ_CTC_AS_DEACT_CTC;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_REJ_CTC_AS_DEACT_CTC;
    }
}

inline bool CSceUserConfig::IsRejCtcAsDeactCtc() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_REJ_CTC_AS_DEACT_CTC) != 0);
}
// BRCM_CUSTOM - [end] Control reg-event interpretation of 'rejected' as
//                     'deactivated'

// BRCM_CUSTOM - [add] Control subscription failure re-attempt behavior on
//                     500 response
inline void CSceUserConfig::SetSubFail500(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_SUB_FAIL_500_RET;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_SUB_FAIL_500_RET;
    }
}

inline bool CSceUserConfig::IsSubFail500() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_SUB_FAIL_500_RET) != 0);
}
// BRCM_CUSTOM - [end] Control subscription failure re-attempt behavior on
//                     500 response

// BRCM_CUSTOM - [add] Control whether to maintain subscription on refresh failure
inline void CSceUserConfig::SetKeepSubOnRefreshFail(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_KEEP_SUB_ON_REFRESH_FAIL;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_KEEP_SUB_ON_REFRESH_FAIL;
    }
}

inline bool CSceUserConfig::IsKeepSubOnRefreshFail() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_KEEP_SUB_ON_REFRESH_FAIL) != 0);
}
// BRCM_CUSTOM - [end] Control whether to maintain subscription on refresh failure

// BRCM_CUSTOM - [add] Control whether to silently terminate active calls
inline void CSceUserConfig::SetActCallNoSilentTerm(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_NO_SILENT_ACT_CALL_TERM;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_NO_SILENT_ACT_CALL_TERM;
    }
}

inline bool CSceUserConfig::IsActCallNoSilentTerm() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_NO_SILENT_ACT_CALL_TERM) != 0);
}
// BRCM_CUSTOM - [end] Control whether to silently terminate active calls

// BRCM_CUSTOM - [add] Control whether to include the default port number in contact header
inline void CSceUserConfig::IncludeDefaultPortInContactHeader(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_DEFAULT_PORT_IN_CONTACT_HDR;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_DEFAULT_PORT_IN_CONTACT_HDR;
    }
}

inline bool CSceUserConfig::IncludeDefaultPortInContactHeader() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_DEFAULT_PORT_IN_CONTACT_HDR) != 0);
}
// BRCM_CUSTOM - [end] Control whether to include the default port number in contact header

// BRCM_CUSTOM - [add] Control whether to include the default (5060 or 5061) port in via header
inline void CSceUserConfig::IncludeDefaultPortInViaHeader(IN bool bEnable)
{
    if (bEnable)
    {
        m_uBrcmExtraCfgs |= eEXTRACFGS_DEFAULT_PORT_IN_VIA_HDR;
    }
    else
    {
        m_uBrcmExtraCfgs &= ~eEXTRACFGS_DEFAULT_PORT_IN_VIA_HDR;
    }
}

inline bool CSceUserConfig::IncludeDefaultPortInViaHeader() const
{
   return ((m_uBrcmExtraCfgs & eEXTRACFGS_DEFAULT_PORT_IN_VIA_HDR) != 0);
}
// BRCM_CUSTOM - [end] Control whether to include the default (5060 or 5061) port in via header

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEUSERCONFIG_H

