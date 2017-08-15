//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==EDOC========================================================================
#ifndef MXG_CSIPSERVERLOCATIONSVC_H
#define MXG_CSIPSERVERLOCATIONSVC_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT to be able to use this \
class.
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_ISIPSERVERLOCATIONSVC_H
#include "SipCoreSvc/ISipServerLocationSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: CSipServerLocationSvc
//========================================
//
// Description:
//   This class implements the ISipConnectionSvc interface in order to provide
//   access to an CSipReqCtxServerLocationSvc instance in the form of a
//   ISipReqCtxConnectionSvc pointer, to the requesting CSipContext or
//   CSipRequestContext.
//
// Location:
//   SipCore/CSipServerLocationSvc.h
//
// See Also:
//   ISipServerLocationSvc, ISipConnectionSvc, CEComDelegatingUnknown,
//   ISipReqCtxConnectionSvc, CSipReqCtxServerLocationSvc
//
//==EDOC========================================================================
class CSipServerLocationSvc : private CEComDelegatingUnknown,
                              private ISipServerLocationSvc,
                              private ISipConnectionSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    friend class CSipReqCtxServerLocationSvc; // m_lastLocalAddrUsed
    //M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipServerLocationSvc();

    static void FinalizeCSipServerLocationSvc();

    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    // << Constructors / Destructors >>
    //--------------------------------------------
    CSipServerLocationSvc(IN IEComUnknown& rOuterIEComUnknown);
    virtual ~CSipServerLocationSvc();


    //-- << CEComDelegatingUnknown >>
    //--------------------------------------------

    // The following statement is essential, it provides the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);


    //-- << ISipConnectionSvc >>
    //--------------------------------------------

    // Summary:
    //  Obtains an instance of an ISipReqCtxConnectionSvc to attach to a
    //  request context.
    //-------------------------------------------------------------------
    virtual ISipReqCtxConnectionSvc*
            GetReqCtxConnectionSvcInstance(
                                IN const CSipPacket& rPacket,
                                IN ISipConnectionSvc::EDirection eDirection);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual void ClearConnectionSvc();


    //-- << ISipServerLocationSvc >>
    //--------------------------------------------

    // Summary:
    //  Sets the ServerLocationSvc mode.
    //-----------------------------------
    virtual void
        SetReqCtxServerLocationSvcMode(IN EServerLocationSvcMode eMode);

    // Summary:
    //  Sets the callback function for location list modifications.
    //--------------------------------------------------------------
    virtual mxt_result SetServerLocationListModifier(
                            IN mxt_PFNServerLocationListModifier pfnNew,
                            IN mxt_opaque opqToPass,
                            OUT mxt_PFNServerLocationListModifier& rpfnPrevious,
                            OUT mxt_opaque& ropqToPassToPrevious);

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    CSipServerLocationSvc(const CSipServerLocationSvc& rFrom);
    CSipServerLocationSvc& operator=(const CSipServerLocationSvc& rFrom);

// Hidden Data Members
//-------------------------
protected:
private:
    EServerLocationSvcMode m_eMode;

    mxt_PFNServerLocationListModifier m_pfnLocationListCallback;

    mxt_opaque m_opqCallbackParameter;

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
    mxt_opaque m_opqDnsUser;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif // #ifndef MXG_CSIPSERVERLOCATIONSVC_H


