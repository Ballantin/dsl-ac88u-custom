//==============================================================================
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
//==============================================================================
//==============================================================================
//
// Portions of this software Copyright (c) 2008-2012 Broadcom
// 
//==============================================================================

#ifndef MXG_CUASSPREGISTRATIONCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPREGISTRATIONCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IUASSPREGISTRATIONCONFIG_H
#include "SceUaSsp/IUaSspRegistrationConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CHostPort;
class CSipUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspRegistrationConfig
//========================================
//
// Description:
//  This class implements the user configuration options for registration
//  management.
//
// Location:
//   UaSsp/CUaSspRegistrationConfig.h
//
// See Also:
//   IUaSspRegistrationConfig
//
//==============================================================================
class CUaSspRegistrationConfig : protected CEComDelegatingUnknown,
                                 protected IUaSspRegistrationConfig
{
//-- Friend Declarations.
    friend class CSceUaSspInitializer;

//-- Published Interface.
public:


//-- Hidden Methods.
protected:

    //-- << IUaSspRegistrationConfig >>
    virtual mxt_result AddRegistrar(IN TO CHostPort* pRegistrar);
    virtual void SetRegistrarList(IN TO CVector<CHostPort*>* pvecpRegistrar);
    virtual CVector<CHostPort*>* GetRegistrarList();
    virtual void SetRegistrationRetryDelay(IN uint32_t uNbSeconds);
    virtual uint32_t GetRegistrationRetryDelay() const;
    virtual void SetRegistrationExpirationTime(IN uint32_t uNbSeconds);
    virtual uint32_t GetRegistrationExpirationTime() const;
    virtual void SetRegistrationExpiringThreshold(IN uint32_t uNbSeconds);
    virtual uint32_t GetRegistrationExpiringThreshold() const;
// BRCM_CUSTOM - [begin]
    virtual void SetGruuEnable(IN bool bGruuEnable);
    virtual bool GetGruuEnable() const;
// BRCM_CUSTOM - [end]
// BRCM_CUSTOM - [begin] Control contact header matching choices
    virtual void SetStrictContactMatch(IN bool bStrict);
    virtual bool GetStrictContactMatch() const;
// BRCM_CUSTOM - [end] Control contact header matching choices
    virtual void SetRegistrationStateMonitoringMode(IN IUaSspRegistrationConfig::ERegistrationMonitoringMode eMonitoringMode);
    virtual IUaSspRegistrationConfig::ERegistrationMonitoringMode GetRegistrationStateMonitoringMode() const;
    virtual void SetRegistrationStateSubscriptionDuration(IN uint32_t uNbSeconds);
    virtual uint32_t GetRegistrationStateSubscriptionDuration() const;
    virtual void SetSubscriptionBehaviorOnError(IN ESubscriptionBehaviorOnError eBehavior);
    virtual ESubscriptionBehaviorOnError GetSubscriptionBehaviorOnError() const;
    virtual void SetRegistrationStateSubscriptionRetryDelay(IN uint32_t uRetryDelayS);
    virtual uint32_t GetRegistrationStateSubscriptionRetryDelay() const;
// BRCM_CUSTOM - [begin] Control use of randomized re-registration time
    virtual void SetRandomReReg(IN bool bRandom);
    virtual bool GetRandomReReg() const;
// BRCM_CUSTOM - [end] Control use of randomized re-registration time
// BRCM_CUSTOM - [add] Controls the registration retry timer to use on 403
    virtual void SetRegistrationRetryTimerOn403(IN uint32_t uRetryTimer);
    virtual uint32_t GetRegistrationRetryTimerOn403() const;
// BRCM_CUSTOM - [end] Controls the registration retry timer to use on 403
// BRCM_CUSTOM - [add] Silent termination on registration timeout
    virtual void SetRegTimeoutSilentTerm(IN bool bRegTerm);
    virtual bool GetRegTimeoutSilentTerm() const;
// BRCM_CUSTOM - [end] Silent termination on registration timeout

    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspRegistrationConfig();
    static void FinalizeCUaSspRegistrationConfig();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaSspRegistrationConfig(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaSspRegistrationConfig();


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaSspRegistrationConfig(IN const CUaSspRegistrationConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspRegistrationConfig& operator=(IN const CUaSspRegistrationConfig& rSrc);


//-- Hidden Data Members.
protected:
private:
    CVector<CHostPort*>* m_pvecpRegistrars;
    uint32_t m_uRetryDelay;
    uint32_t m_uExpirationTime;
    uint32_t m_uExpiringThreshold;

// BRCM_CUSTOM - [begin] Control contact header matching choices
    bool m_bStrictContactMatch;
// BRCM_CUSTOM - [end] Control contact header matching choices

// BRCM_CUSTOM - [begin] Flag showing if GRUU support has been enabled
    bool m_bGruuEnabled;
// BRCM_CUSTOM - [end]

    IUaSspRegistrationConfig::ERegistrationMonitoringMode m_eMonitoringMode;
    uint32_t m_uSubscriptionDuration;
    // The subscriber retry delay in sec.
    uint32_t m_uRegistrationStateSubscriptionRetryDelayS;

    // Subscription behavior on REGISTER error.
    ESubscriptionBehaviorOnError m_eSubscriptionBehaviorOnError;

// BRCM_CUSTOM - [begin] Control use of randomized re-registration time
    bool m_bRandomReReg;
// BRCM_CUSTOM - [end] Control use of randomized re-registration time

// BRCM_CUSTOM - [add] Add support for configurable retry-after timer
//                     on 403 response
    uint32_t m_uRegRetryTimerOn403;
// BRCM_CUSTOM - [end] Add support for configurable retry-after timer
//                     on 403 response
// BRCM_CUSTOM - [add] Silent termination on registration timeout
    bool m_bRegTimeoutSilentTerm;
// BRCM_CUSTOM - [end] Silent termination on registration timeout

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
inline CVector<CHostPort*>* CUaSspRegistrationConfig::GetRegistrarList()
{
    return m_pvecpRegistrars;
}

inline void CUaSspRegistrationConfig::SetRegistrationRetryDelay(IN uint32_t uNbSeconds)
{
    m_uRetryDelay = uNbSeconds;
}

inline uint32_t CUaSspRegistrationConfig::GetRegistrationRetryDelay() const
{
    return m_uRetryDelay;
}

inline void CUaSspRegistrationConfig::SetRegistrationExpirationTime(IN uint32_t uNbSeconds)
{
    m_uExpirationTime = uNbSeconds;
}

inline uint32_t CUaSspRegistrationConfig::GetRegistrationExpirationTime() const
{
    return m_uExpirationTime;
}

inline void CUaSspRegistrationConfig::SetRegistrationExpiringThreshold(IN uint32_t uNbSeconds)
{
    m_uExpiringThreshold = uNbSeconds;
}

inline uint32_t CUaSspRegistrationConfig::GetRegistrationExpiringThreshold() const
{
    return m_uExpiringThreshold;
}
// BRCM_CUSTOM - [begin]
inline void CUaSspRegistrationConfig::SetGruuEnable(IN bool bGruuEnable)
{
    m_bGruuEnabled = bGruuEnable;
}

inline bool CUaSspRegistrationConfig::GetGruuEnable() const
{
    return m_bGruuEnabled;
}
// BRCM_CUSTOM - [end]
// BRCM_CUSTOM - [begin] Control contact header matching choices
inline void CUaSspRegistrationConfig::SetStrictContactMatch(IN bool bStrict)
{
    m_bStrictContactMatch = bStrict;
}

inline bool CUaSspRegistrationConfig::GetStrictContactMatch() const
{
    return m_bStrictContactMatch;
}
// BRCM_CUSTOM - [end] Control contact header matching choices

inline void CUaSspRegistrationConfig::SetRegistrationStateMonitoringMode(IN IUaSspRegistrationConfig::ERegistrationMonitoringMode eMonitoringMode)
{
    m_eMonitoringMode = eMonitoringMode;
}

inline CUaSspRegistrationConfig::ERegistrationMonitoringMode CUaSspRegistrationConfig::GetRegistrationStateMonitoringMode() const
{
    return m_eMonitoringMode;
}

inline void CUaSspRegistrationConfig::SetRegistrationStateSubscriptionDuration(IN uint32_t uNbSeconds)
{
    m_uSubscriptionDuration = uNbSeconds;
}

inline uint32_t CUaSspRegistrationConfig::GetRegistrationStateSubscriptionDuration() const
{
    return m_uSubscriptionDuration;
}

inline void CUaSspRegistrationConfig::SetRegistrationStateSubscriptionRetryDelay(IN uint32_t uRetryDelayS)
{
    m_uRegistrationStateSubscriptionRetryDelayS = uRetryDelayS;
}

inline uint32_t CUaSspRegistrationConfig::GetRegistrationStateSubscriptionRetryDelay() const
{
    return m_uRegistrationStateSubscriptionRetryDelayS;
}

// BRCM_CUSTOM - [begin] Control use of randomized re-registration time
inline void CUaSspRegistrationConfig::SetRandomReReg(IN bool bRandom)
{
    m_bRandomReReg = bRandom;
}

inline bool CUaSspRegistrationConfig::GetRandomReReg() const
{
    return m_bRandomReReg;
}
// BRCM_CUSTOM - [end] Control use of randomized re-registration time

// BRCM_CUSTOM - [add] Controls the registration retry timer to use on 403
inline void CUaSspRegistrationConfig::SetRegistrationRetryTimerOn403(IN uint32_t uRetryTimer)
{
   m_uRegRetryTimerOn403 = uRetryTimer;
}

inline uint32_t CUaSspRegistrationConfig::GetRegistrationRetryTimerOn403() const
{
   return m_uRegRetryTimerOn403;
}
// BRCM_CUSTOM - [end] Controls the registration retry timer to use on 403 

// BRCM_CUSTOM - [add] Silent termination on registration timeout
inline void CUaSspRegistrationConfig::SetRegTimeoutSilentTerm(IN bool bRegTerm)
{
   m_bRegTimeoutSilentTerm = bRegTerm;
}

inline bool CUaSspRegistrationConfig::GetRegTimeoutSilentTerm() const
{
   return m_bRegTimeoutSilentTerm;
}
// BRCM_CUSTOM - [end] Silent termination on registration timeout


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPREGISTRATIONCONFIG_H

