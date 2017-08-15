//==============================================================================
//==============================================================================
//
//          Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCESERVEREVENTDATA_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCESERVEREVENTDATA_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members.

//-- Interface Realized and/or Parent.

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CNameAddr;
class IUaSspCall;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceServerEventData
//==============================================================================
//
// Summary:
//   Container for data related to server events.
//
// Description:
//   Holds data for all server events that might have happen while a request is
//   received.
//
// Location:
//   SceCoreComponents/CSceServerEventData.h
//
//==============================================================================
class CSceServerEventData
{
//-- Published Interface.
public:
    // Summary:
    //  Constructor.
    CSceServerEventData();

    // Summary:
    //  Destructor.
    virtual ~CSceServerEventData();

    //-- << ISipReplacesMgr::EvReplaces related methods >>
    void GetEvReplacesReplacedCall(OUT IUaSspCall*& rpReplacedCall);
    void SetEvReplacesReplacedCall(IN IUaSspCall* bReplacedCall);

    bool GetEvReplacesEarlyOnly();
    void SetEvReplacesEarlyOnly(IN bool bEarlyOnly);

    //-- << EvUncertifiedPrivacy related methods >>
    void SetEvUncertifiedPrivacyCalled(IN bool bEvUncertifiedPrivacyCalled);
    bool GetEvUncertifiedPrivacyCalled();

    //-- << EvUntrustedProxy related methods >>
    void SetEvUntrustedProxyCalled(IN bool bEvUntrustedProxyCalled);
    bool GetEvUntrustedProxyCalled();

    //-- << EvAssertedIdentity related methods >>
    void StoreEvAssertedIdentityParam(IN TO CNameAddr* pSipIdentity,
                                      IN TO CNameAddr* pTelIdentity);
    void RetrieveEvAssertedIdentityParam(OUT GO CNameAddr*& rpNaiSip,
                                         OUT GO CNameAddr*& rpNaiTel);

//-- Hidden Methods.
protected:
private:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Copy constructor.
    CSceServerEventData(IN const CSceServerEventData& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceServerEventData& operator=(IN const CSceServerEventData& rSrc);

    

//-- Hidden Data Members.
protected:
private:
    // ISipReplacesMgr::EvReplaces data members.
    IUaSspCall* m_pEvReplacesReplacedCall;
    bool m_bEvReplacesEarlyOnly;

    // ISipPrivacyMgr::EvUncertifiedPrivacy data member.
    bool m_bEvUncertifiedPrivacyCalled;

    // EvUntrustedProxy data member.
    bool m_bEvUntrustedProxyCalled;

    // EvAssertedIdentity data member.
    CNameAddr* m_pSipIdentity;
    CNameAddr* m_pTelIdentity;
};
//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  ISipReplacesMgr::EvReplaces RELATED METHODS  ===========================
//==============================================================================

//==============================================================================
//==
//==  GetEvReplacesEarlyOnly
//==
//==============================================================================
//
//  Returns:
//      true:
//       The early-only parameter was present in the Replaces header.
//
//      false:
//       EvReplaces was not called or the early-only parameter was not present
//       in the Replaces header.
//
//  Description:
//      Returns the data received in the ISipReplacesMgr::EvReplaces event.
//
//      Should not be used if GetEvReplacesReplacedCall returns NULL.
//
//==============================================================================
inline bool CSceServerEventData::GetEvReplacesEarlyOnly()
{
    return m_bEvReplacesEarlyOnly;
}

//==============================================================================
//==
//==  SetEvReplacesEarlyOnly
//==
//==============================================================================
//
//  Parameters:
//      bEarlyOnly:
//       The value taken from ISipReplacesMgr::EvReplaces.
//
//  Description:
//      Sets the data received in the ISipReplacesMgr::EvReplaces event.
//
//==============================================================================
inline void CSceServerEventData::SetEvReplacesEarlyOnly(IN bool bEarlyOnly)
{
    m_bEvReplacesEarlyOnly = bEarlyOnly;
}

//==============================================================================
//====  EVUNCERTIFIEDPRIVACY RELATED METHODS  ==================================
//==============================================================================
inline void CSceServerEventData::SetEvUncertifiedPrivacyCalled(IN bool bEvUncertifiedPrivacyCalled)
{
    m_bEvUncertifiedPrivacyCalled = bEvUncertifiedPrivacyCalled;
}

inline bool CSceServerEventData::GetEvUncertifiedPrivacyCalled()
{
    return m_bEvUncertifiedPrivacyCalled;
}

//==============================================================================
//====  EVUNTRUSTEDPROXY RELATED METHODS  ======================================
//==============================================================================
inline void CSceServerEventData::SetEvUntrustedProxyCalled(IN bool bEvUntrustedProxyCalled)
{
    m_bEvUntrustedProxyCalled = bEvUntrustedProxyCalled;
}

inline bool CSceServerEventData::GetEvUntrustedProxyCalled()
{
    return m_bEvUntrustedProxyCalled;
}


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCESERVEREVENTDATA_H
