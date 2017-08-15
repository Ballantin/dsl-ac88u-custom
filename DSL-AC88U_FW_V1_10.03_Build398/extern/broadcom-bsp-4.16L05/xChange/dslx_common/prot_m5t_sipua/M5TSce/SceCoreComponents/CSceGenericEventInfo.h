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
#ifndef MXG_CSCEGENERICEVENTINFO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEGENERICEVENTINFO_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members.

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Interface Realized and/or Parent.
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEGENERICEVENTINFO_H
#include "SceCoreComponents/ISceGenericEventInfo.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CSceCallerPreferences;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceGenericEventInfo
//==============================================================================
//
// Description:
//   Implementation of the ISceGenericEventInfo interface. This object is
//   never accessed directly. It is always used through the
//   ISceGenericEventInfo interface.
//
// Location:
//   SceCoreComponents/CSceGenericEventInfo.h
//
// See Also:
//   ISceGenericEventInfo
//
//==============================================================================
class CSceGenericEventInfo : private CEComDelegatingUnknown,
                             private ISceGenericEventInfo
{
//-- Published Interface.
public:
    // << ECom Pattern >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

//-- Hidden Methods.
protected:
private:
    //   << Startup mechanism >>
    static mxt_result InitializeCSceGenericEventInfo();
    static void FinalizeCSceGenericEventInfo();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSceGenericEventInfo(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceGenericEventInfo();

    //   << ISceGenericEventInfo interface >>
    virtual void AddComponent(IN mxt_clsid classId);
    virtual void SetNetworkAssertedIdentity(IN TO CNameAddr* pNaiSip,
                                            IN TO CNameAddr* pNaiTel);
    virtual void GetNetworkAssertedIdentity(OUT const CNameAddr*& rpNaiSip,
                                            OUT const CNameAddr*& rpNaiTel);
    virtual void SetCallerPreferences(IN TO CSceCallerPreferences* pCallerPrefs);
    virtual void GetCallerPreferences(OUT const CSceCallerPreferences*& rpCallerPrefs);

    // Inherited from CEComDelegatingUnknown.
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy constructor.
    CSceGenericEventInfo(IN const CSceGenericEventInfo& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceGenericEventInfo& operator=(IN const CSceGenericEventInfo& rSrc);

// Friends declarations.
    friend class CSceCoreComponentsInitializer;

//-- Hidden Data Members.
protected:
private:
    // The asserted identity of the user sending the request in SIP URI format.
    CNameAddr* m_pNaiSip;
    // The asserted identity of the user sending the request in TEL URI format.
    CNameAddr* m_pNaiTel;

    // The caller preferences of the user sending the request.
    CSceCallerPreferences* m_pCallerPrefs;

    // Vector containing the different interface added.
    CVector<IEComUnknown*>* m_pvecpCustomObject;
};

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCEGENERICEVENTINFO_H
