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
#ifndef MXG_CUASSPCALLEVENTINFO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUASSPCALLEVENTINFO_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IUASSPCALLEVENTINFO_H
#include "SceUaSsp/IUaSspCallEventInfo.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaSspCallEventInfo
//==============================================================================
//
// Description:
//   Implementation of the IUaSspCallEventInfo interface.
//
// Location:
//   SceUaSsp/CUaSspCallEventInfo.h
//
// See Also:
//   IUaSspCallEventInfo
//
//==============================================================================
class CUaSspCallEventInfo : protected CEComDelegatingUnknown,
                            protected IUaSspCallEventInfo
{
//-- Friend Declarations
    friend class CSceUaSspInitializer;

//-- Published Interface
public:

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CUaSspCallEventInfo(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CUaSspCallEventInfo();

private:
    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaSspCallEventInfo();

    static void FinalizeCUaSspCallEventInfo();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << IUaSspCallEventInfo >>
    virtual void SetReplacedCall(IN IUaSspCall* pReplacedCall);

    virtual void GetReplacedCall(OUT IUaSspCall*& rpReplacedCall);

    virtual bool IsOptionsRequest() const;
    virtual void SetIsOptionsRequest(IN bool bIsOptionsRequest);

    virtual const CNameAddr* GetAlertInfo() const;
    virtual void SetAlertInfo(IN const CNameAddr& rNameAddr);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy constructor.
    CUaSspCallEventInfo(IN const CUaSspCallEventInfo& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaSspCallEventInfo& operator=(IN const CUaSspCallEventInfo& rSrc);

// Friends declarations

//-- Hidden Data Members
protected:
private:

    // Data related to SetReplacedCall / GetReplacedCall.
    IUaSspCall* m_pReplacedCall;

    // Indicates if it is for an option request.
    bool m_bIsOptionsRequest;

    // The Alert-Info header.
    CNameAddr* m_pAlertInfoNameAddr;
};

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CUASSPCALLEVENTINFO_H

