//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CUASSPCALLCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPCALLCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IUASSPCALLCONFIG_H
#include "SceUaSsp/IUaSspCallConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspCallConfig
//==============================================================================
//<GROUP UaSsp>
//
// Description:
//   Implements the configuration specific to the IUaSspCall.
//
// Location:
//   UaSsp/CUaSspCallConfig.h
//
//==============================================================================
class CUaSspCallConfig : protected CEComDelegatingUnknown,
                         protected IUaSspCallConfig
{
//-- Friend Declarations
//------------------------
    friend class CSceUaSspInitializer;

//-- New types within class namespace
//-------------------------------------
public:
protected:
private:

//-- Published Interface
//------------------------
public:

    // Summary:
    //  Overrides the default minimal session expiration value. This method is
    //  an internal method and SHOULD NEVER be used. Its use is at the caller's
    //  risks. Should be 90 as per RFC4028, which it is by default.
    //--------------------------------------------------------------------------
    void InternalForceMinSessionExpiration(IN uint32_t uMinSessionExpirationS);

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CUaSspCallConfig(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CUaSspCallConfig();


    //-- << Initialization mechanism >>
    //--------------------------------------------
    static mxt_result InitializeCUaSspCallConfig();

    static void FinalizeCUaSspCallConfig();


    //-- << ECOM mechanism >>
    //------------------------
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << CEComDelegatingUnknown >>
    //--------------------------------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << IUaSspCallConfig >>
    //--------------------------------------------
    virtual void EnableSessionTimerSupport(IN bool bEnable);
    virtual bool IsSessionTimerSupportEnabled() const;
    virtual void SetSessionExpiration(IN uint32_t uExpirationSec);
    virtual uint32_t GetSessionExpiration() const;
// BRCM_CUSTOM [add] Min-SE configuration support
    virtual void SetMinSE(IN uint32_t uMinSE) ;
    virtual uint32_t GetMinSE() const;
// BRCM_CUSTOM [end] Min-SE configuration support
    virtual void SetSessionRefreshMethod(IN ESessionRefreshConfig eConfig);
    virtual ESessionRefreshConfig GetSessionRefreshMethod() const; 
    virtual void SetReliable1xxConfig(IN ERel1xxConfig eConfig);
    virtual ERel1xxConfig GetReliable1xxConfig();
    virtual void SetUpdateConfig(IN EUpdateConfig eConfig);
    virtual EUpdateConfig GetUpdateConfig();

// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
    virtual void SetConnectedIdConfig(IN EConnectedIdConfig eConfig);

    virtual EConnectedIdConfig GetConnectedIdConfig();
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916
// BRCM_CUSTOM - [begin] Two-step BYE process
    virtual void SetTwoStepByeConfig(IN bool bEnable);

    virtual bool GetTwoStepByeConfig();
// BRCM_CUSTOM - [end] Two-step BYE process

// BRCM_CUSTOM - [begin] Call termination reinvite timeout for hold
   virtual void SetNoTerm408ReInvMspConfig(IN bool bEnable);
   virtual bool GetNoTerm408ReInvMspConfig();
// BRCM_CUSTOM - [end] Call termination reinvite timeout for hold

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CUaSspCallConfig(IN const CUaSspCallConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CUaSspCallConfig& operator=(IN const CUaSspCallConfig& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    bool m_bSessionTimerEnabled;

    uint32_t m_uSessionExpirationSec;
// BRCM_CUSTOM [add] Min-SE configuration support
    uint32_t m_uMinSESec;
// BRCM_CUSTOM [end] Min-SE configuration support

    ESessionRefreshConfig m_eSessionRefreshConfig;

    ERel1xxConfig m_eRel1xxConfig;

    EUpdateConfig m_eUpdateConfig;

// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
    EConnectedIdConfig m_eConnectIdConfig;
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916
// BRCM_CUSTOM - [begin] Two-step BYE process
    bool m_bTwoStepByeEnable;
// BRCM_CUSTOM - [end] Two-step BYE process
// BRCM_CUSTOM - [begin] Call termination on reinvite timeout for hold
    bool m_bNoTerm408ReInvMsp;
// BRCM_CUSTOM - [end] Call termination on reinvite timeout for hold

    static uint32_t s_uMinSessionExpirationS;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPCALLCONFIG_H

