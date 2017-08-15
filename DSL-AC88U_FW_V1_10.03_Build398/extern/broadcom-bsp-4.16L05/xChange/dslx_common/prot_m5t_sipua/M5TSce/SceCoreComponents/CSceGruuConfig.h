//==============================================================================
//==============================================================================
//
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
//==============================================================================
#ifndef MXG_CSCEGRUUCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEGRUUCONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif


//   Parameters used by Value.
//-----------------------------


//-- Interface Realized and/or Parent.
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEGRUUCONFIG_H
#include "SceCoreComponents/ISceGruuConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------
class CSipUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceGruuConfig
//========================================
//
// Description:
//  This class is an ECom component which implements the ISceGruuConfig
//  interface. At initialization, it automatically registers to
//  CSceUserConfig using the method:
//  "CSceUserConfig::RegisterUserConfigObject(CLSID_CSceGruuConfig)".
//
// Location:
//   SceCoreComponents/CSceGruuConfig.h
//
//==============================================================================
class CSceGruuConfig : private CEComDelegatingUnknown,
                              private ISceGruuConfig
{
//-- Friend Declarations.
//------------------------
    friend class CSceCoreComponentsInitializer;


//-- Published Interface.
//------------------------
public:
    // Default maximum number of GRUUs in the temp list.
    //---------------------------------------------------
    static const unsigned int ms_uDEFAULT_NB_MAX_TEMP_GRUU;


//-- Hidden Methods.
//-------------------
private:
    //   << Startup mechanism >>
    //---------------------------
    static mxt_result InitializeCSceGruuConfig();
    static void FinalizeCSceGruuConfig();

    //-- << ECOM mechanism >>
    //------------------------
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSceGruuConfig(IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSceGruuConfig();


    //   << CEComUnknown >>
    //----------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);
    virtual mxt_result InitializeInstance();
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //   << ISceGruuConfig >>
    //-------------------------------
    virtual void SetPublicGruu(IN TO CSipUri* pGruu);
    virtual const CSipUri* GetPublicGruu() const;
    virtual mxt_result AddTempGruu(IN TO CSipUri* pGruu);
    virtual const CSipUri* GetTempGruu();
    virtual void ConfigureTempGruus(IN uint32_t uMaxTempGruus);
    virtual void InvalidateAllGruus();

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSceGruuConfig(IN const CSceGruuConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSceGruuConfig& operator=(IN const CSceGruuConfig& rSrc);


//-- Hidden Data Members.
//------------------------
private:
    // Public GRUU SIP URI.
    //----------------------
    CSipUri*            m_pPublicGruu;

    // Temporary GRUU list.
    //----------------------
    CList<CSipUri*>*    m_plstpTempGruu;

    // Last used GRUU that will be used when the temporary GRUU is empty.
    //--------------------------------------------------------------------
    CSipUri*            m_pLastUsedGruu;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline const CSipUri* CSceGruuConfig::GetPublicGruu() const
{
    return(m_pPublicGruu);
}


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSCEGRUUCONFIG_H
