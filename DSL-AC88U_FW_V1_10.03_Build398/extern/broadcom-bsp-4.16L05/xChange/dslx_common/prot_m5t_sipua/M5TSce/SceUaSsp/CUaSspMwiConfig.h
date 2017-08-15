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
#ifndef MXG_CUASSPMWICONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPMWICONFIG_H

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

#ifndef MXG_IUASSPMWICONFIG_H
#include "SceUaSsp/IUaSspMwiConfig.h"
#endif

#ifndef MXG_ISCEUSERCONFIGTERMINATION_H
#include "SceCoreComponents/ISceUserConfigTermination.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspMwiConfig
//========================================
//
// Description:
//  This class implements the user configuration options for the MWI management.
//
// Location:
//   UaSsp/CUaSspMwiConfig.h
//
// See Also:
//   IUaSspMwiConfig
//
//==============================================================================
class CUaSspMwiConfig : protected CEComDelegatingUnknown,
                        protected IUaSspMwiConfig,
                        protected ISceUserConfigTermination
{
//-- Friend Declarations.
    friend class CSceUaSspInitializer;

//-- Published Interface.
public:


//-- Hidden Methods.
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaSspMwiConfig(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CUaSspMwiConfig();

    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspMwiConfig();
    static void FinalizeCUaSspMwiConfig();

    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << IUaSspMwiConfig >>
    virtual mxt_result SetMwiServerAddress(IN TO CSipUri* pAddress);
    virtual const CSipUri* GetMwiServerAddress() const;
    virtual mxt_result SetSubscriptionExpirationTime(IN uint32_t uNbSeconds);
    virtual uint32_t GetSubscriptionExpirationTime() const;
// BRCM_CUSTOM - [add] Add MWI subscription retry delay config
    virtual void SetSubscriptionRetryDelay(IN uint32_t uRetryDelayS);
    virtual uint32_t GetSubscriptionRetryDelay() const;
// BRCM_CUSTOM - [end] Add MWI subscription retry delay config
    virtual mxt_result SetComponent(IN IEComUnknown* pComponent);
    virtual void GetComponent(OUT IEComUnknown** ppComponent);

    //-- << ISceUserConfigTermination >>
    virtual void OnUserConfigTermination();


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaSspMwiConfig(IN const CUaSspMwiConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspMwiConfig& operator=(IN const CUaSspMwiConfig& rSrc);


//-- Hidden Data Members.
protected:
private:
    // Address of the MWI server to use.
    CSipUri* m_pAddress;

    // The requested duration time of a MWI subscription in seconds.
    uint32_t m_uSubscriptionExpirationTimeS;

// BRCM_CUSTOM - [add] Add MWI subscription retry delay config
    uint32_t m_uSubscriptionRetryDelayS;
// BRCM_CUSTOM - [end] Add MWI subscription retry delay config

    // The MWI component to use.
    IEComUnknown* m_pComponent;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CUASSPMWICONFIG_H

