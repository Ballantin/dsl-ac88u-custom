//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPSYMMETRICUDPSVC_H
#define MXG_CSIPSYMMETRICUDPSVC_H

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


#ifndef MXD_SIPSTACK_ENABLE_SIP_SYMMETRIC_UDP_SVC_SUPPORT
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_SYMMETRIC_UDP_SVC_SUPPORT to be able to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CSIPREQCTXCONNECTIONSVC_H
#include "SipCore/CSipReqCtxConnectionSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_ISIPSYMMETRICUDPSVC_H
#include "SipCoreSvc/ISipSymmetricUdpSvc.h"
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

//==SDOC========================================================================
//== Class: CSipSymmetricUdpSvc
//========================================
//
// Description:
//   This class implements the ISipSymmetricUdpSvc interface in order to provide
//   access to an CSipSymmetricUdpSvc instance in the form of a
//   ISipReqCtxConnectionSvc pointer, to the requesting CSipContext or
//   CSipRequestContext.
//
//   When CSipSymmetricUdpSvc receives an UpdatePacket request for a UDP
//   request, it changes the  port number of the local address to the
//   one it's listening on for the currently selected interface.
//
//   The ISipSymmetricUdpSvc service may be added to a SIP context when an
//   application needs to send UDP requests from a specific port on the
//   selected interface.
//
// Location:
//   SipCoreSvc/CSipSymmetricUdpSvc.h
//
// See Also:
//   ISipSymmetricUdpSvc, CEComDelegatingUnknown, ISipReqCtxConnectionSvc
//
//==EDOC========================================================================
class CSipSymmetricUdpSvc : private CEComDelegatingUnknown,
                            private ISipConnectionSvc,
                            private CSipReqCtxConnectionSvc,
                            private ISipSymmetricUdpSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


// Hidden Methods
//-------------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipSymmetricUdpSvc();

    static void FinalizeCSipSymmetricUdpSvc();

    //-- << ECOM mechanism >>
    //--------------------------------------------

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    CSipSymmetricUdpSvc(IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSipSymmetricUdpSvc();

    //-- << CEComDelegatingUnknown Interface >>.
    //-------------------------------------------

    // The following statement is essential, it provides the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    //--------------------------------------------------------------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    //  Override CEComDelegatingUnknown
    //----------------------------------
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << ISipConnectionSvc >>.
    //----------------------------

    // Summary:
    //  Queries the ISipReqCtxConnectionSvc instance.
    //------------------------------------------------
    virtual ISipReqCtxConnectionSvc*
            GetReqCtxConnectionSvcInstance(
                                   IN const CSipPacket& rPacket,
                                   IN ISipConnectionSvc::EDirection eDirection);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual void ClearConnectionSvc();

    //-- << ISipReqCtxConnectionSvc >>.
    //----------------------------------

    // Summary:
    //  Destroys this instance of ISipReqCtxConnectionSvc.
    //-----------------------------------------------------
    virtual void ReleaseReqCtxConnectionSvcInstance(IN ISipRequestContext& rRequestContext);

    // Summary:
    //  Updates rPacket with the resolved target.
    //--------------------------------------------
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipSymmetricUdpSvc(IN const CSipSymmetricUdpSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipSymmetricUdpSvc& operator=(IN const CSipSymmetricUdpSvc& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSYMMETRICUDPSVC_H

