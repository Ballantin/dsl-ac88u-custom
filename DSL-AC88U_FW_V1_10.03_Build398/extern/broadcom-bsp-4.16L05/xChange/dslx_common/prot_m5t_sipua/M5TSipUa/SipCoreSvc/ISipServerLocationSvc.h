//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPSERVERLOCATIONSVC_H
#define MXG_ISIPSERVERLOCATIONSVC_H

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


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_NETWORKTOOLS_H
#include "Network/NetworkTools.h" // SNaptrRecord
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHostPort;
class ISipReqCtxConnectionSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID( ISipServerLocationSvc );
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  mxt_PFNServerLocationListModifier
//==
//==============================================================================
// <GROUP SIPCORESVC_TYPES>
//
//  Summary:
//      Callback to a list modifier.
//
//  Description:
//      This data type defines a function prototype for a list modifier,
//      which can modify the list, removing or adding records, or simply
//      modifying the order of the records created by the resolution service.
//      The only restriction is that the modification be made without changing
//      the execution context.
//
//  Parameters:
//    rFqdn: The address being resolved.
//
//    rlstNaptrRecord: The list of NAPTR records.
//
//    opq: An opaque.
//
//  Location:
//    SipCoreSvc/CSipReqCtxServerLocationSvc.h
//
//==EDOC========================================================================
typedef void (*mxt_PFNServerLocationListModifier)(
                                    IN const CHostPort& rFqdn,
                                    INOUT CList<SNaptrRecord>& rlstNaptrRecord,
                                    IN mxt_opaque opq);

//==SDOC========================================================================
//== Class: ISipServerLocationSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This service offers the functionality described in RFC 3263: Locating SIP
//   Servers. This service is also used for transport selection, reconnection
//   and failover to alternative transports. Hence, this service MUST always be
//   attached to a context, unless the application knows it will use an
//   established persistent connection.
//
//   It performs NAPTR, SRV, and A queries to resolve where SIP packets must be
//   sent. It supports failover for requests when configured statefully.
//
//   DNS access is performed asynchronously by using a single thread for all
//   queries. The core thread is not blocked while performing server location
//   services, unless the stack is configured to have DNS access and the SIP
//   core module share the same thread.
//
//   When used on a ISipContext with stateless services, this service must be
//   configured as stateless through SetReqCtxServerLocationSvcMode. Its default
//   behaviour is to be stateful.
//
//#The ISipServerLocationSvc is an ECOM object#
//
//  The ISipServerLocationSvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipServerLocationSvc</B>
//
//  <B>Interface Id: IID_ISipServerLocationSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCore/ISipServerLocationSvc.h
//
// See Also:
//
//==EDOC========================================================================
class ISipServerLocationSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipServerLocationSvc);
    //M5T_INTERNAL_USE_END

    // Description:
    //  Indicates how the server location service behaves when the requests fail
    //  to be sent.
    enum EServerLocationSvcMode
    {
        // Stateful mode: Server location service will fail-over when the stack
        // is not able to send requests. This is the default mode.
        //----------------------------------------------------------------------
        eSTATEFUL,

        // Stateles mode: Disable the fail-over mechanism.
        //-------------------------------------------------
        eSTATELESS
    };

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetReqCtxServerLocationSvcMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the server location service mode.
    //
    //  Parameters:
    //    eMode, one of eSTATEFUL or eSTATELESS.
    //
    //  Description:
    //      Sets the server location service mode to stateful or stateless. When
    //      stateful, this service fails over when not able to send requests,
    //      while it does not do so when configured in stateless.
    //
    // Warning:
    //      Not properly setting the server location service mode to eSTATELESS,
    //      when required, may result in misbehaviours. For example:
    //      - Invalid branch value when forwarding requests/responses.
    //
    //==EDOC====================================================================
    virtual void
        SetReqCtxServerLocationSvcMode(IN EServerLocationSvcMode eMode) = 0;


    //==SDOC====================================================================
    //==
    //==  SetServerLocationListModifier
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets a callback function for location list modifications.
    //
    //  Parameters:
    //    pfnNew:
    //      The new callback function for the location list modifications. It
    //      must not be NULL.
    //
    //    opqToPass:
    //      The opaque value passed to the callback function.
    //
    //    rpfnPrevious:
    //      The callback function that was already set in the service. This
    //      callback function should be called by the new callback method. It is
    //      not changed if this method fails.
    //
    //      NULL if there was no callback function already set.
    //
    //    ropqToPassToPrevious:
    //      The opaque value that must be passed to rpfnPrevious when it is
    //      called. It is not changed if this method fails.
    //
    //      This parameter must not be used if rpfnPrevious is NULL.
    //
    //  Returns:
    //    resFE_INVALID_ARGUMENT:
    //      pfnNew is NULL.
    //
    //    resS_OK:
    //      The callback method has been changed.
    //
    //  Description:
    //      Sets a new callback method for location list modifications. The
    //      callback function method is called with opqCallbackParameter after
    //      the asynchronous resolution has succeeded to allow another object
    //      to modify the record list.
    //
    //      The callback method returned by this method should be called with
    //      its opaque value in the new callback function.
    //
    //      The change in callback function only affects the new transactions.
    //      It is not used for current requests.
    //
    //==EDOC====================================================================
    virtual mxt_result SetServerLocationListModifier(
                            IN mxt_PFNServerLocationListModifier pfnNew,
                            IN mxt_opaque opqCallbackParameter,
                            OUT mxt_PFNServerLocationListModifier& rpfnPrevious,
                            OUT mxt_opaque& ropqPreviousParameter) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipServerLocationSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipServerLocationSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipServerLocationSvc(IN const ISipServerLocationSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipServerLocationSvc& operator=(IN const ISipServerLocationSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSERVERLOCATIONSVC_H
