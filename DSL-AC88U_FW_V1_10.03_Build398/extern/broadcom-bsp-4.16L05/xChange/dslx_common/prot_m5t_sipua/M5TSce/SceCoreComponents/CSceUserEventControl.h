//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCEUSEREVENTCONTROL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEUSEREVENTCONTROL_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members.

//-- Interface Realized and/or Parent.
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEUSEREVENTCONTROL_H
#include "SceCoreComponents/ISceUserEventControl.h"
#endif

#ifndef MXG_ISCEUSEREVENTMGR_H
#include "SceCoreComponents/ISceUserEventMgr.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
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
//== Class: CSceUserEventControl
//==============================================================================
//
// Description:
//   Implementation of the ISceUserEventControl interface. This object is
//   never accessed directly. It is always used through the
//   ISceUserEventControl interface.
//
// Location:
//   SceCoreComponents/CSceUserEventControl.h
//
// See Also:
//   ISceUserEventControl
//
//==============================================================================
class CSceUserEventControl : private CEComDelegatingUnknown,
                             private ISceUserEventControl
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
    static mxt_result InitializeCSceUserEventControl();
    static void FinalizeCSceUserEventControl();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSceUserEventControl(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceUserEventControl();

    // Inherited from ISceUserEventControl
    virtual mxt_result AddUserEventMgr(IN ISceUserEventMgr* pEventMgr);
    virtual mxt_result RemoveUserEventMgr(IN ISceUserEventMgr* pEventMgr);
    virtual void PublishUserEvent(IN ISceUserEventMgr::EUserEvent eEvent, 
                                  IN IEComUnknown* pUserConfig,
                                  IN mxt_opaque opqEvent);

    // Inherited from CEComDelegatingUnknown.
    virtual mxt_result InitializeInstance();
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy constructor.
    CSceUserEventControl(IN const CSceUserEventControl& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceUserEventControl& operator=(IN const CSceUserEventControl& rSrc);

// Friends declarations.
    friend class CSceCoreComponentsInitializer;

//-- Hidden Data Members
protected:
private:

    // The manager container.
    CVector<ISceUserEventMgr*>* m_pvecpUserEventMgr;
};

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSCEUSEREVENTCONTROL_H


