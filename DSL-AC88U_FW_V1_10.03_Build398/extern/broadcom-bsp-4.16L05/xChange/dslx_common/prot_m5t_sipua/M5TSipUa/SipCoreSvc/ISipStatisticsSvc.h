//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPSTATISTICSSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSTATISTICSSVC_H
//M5T_INTERNAL_USE_END

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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT to be able to use \
this class.
#endif

//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class ISipTransactionStatistics;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID( ISipStatisticsSvc );
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipStatisticsSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This service is responsible to gather statistical data of packets that
//   are sent and received on the associated context.
//
//   Stateless vs stateful
//   In stateful mode, for an incoming INVITE or an outgoing ACK, a new
//   statistics request context service is created and associated with the
//   request context. It can detect retransmitted 2xx to INVITE, retransmitted 
//   ACK request, and retransmitted reliable 1xx. Set to stateful for a 
//   transaction stateful proxy or user agent.
// 
//   Less memory is used in stateless mode. Set to stateless for a
//   stateless proxy context.
//
//   If a CSipStatisticsContainer is set on the service with 
//   SetTransactionStatistics, statistics are accumulated in the associated
//   container.
//
//   This service must be attached to the context after the CSipSessionSvc and 
//   CSipServerLocationSvc to be able to gather statistics properly.
//
//#The ISipStatisticsSvc is an ECOM object#
//
//  The ISipStatisticsSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_ISipStatisticsSvc</B>
//
//  <B>Interface Id: IID_ISipStatisticsSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipStatisticsSvc.h
//
//==EDOC========================================================================
class ISipStatisticsSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- New types within class namespace
//-----------------------------------
public:
protected:
private:

//-- Published Interface
//------------------------
public:

    // Description:
    //  Used to configure the service for stateful or stateless operation.
    //
    // See also:
    //  SetServiceMode
    enum EServiceMode
    {
        // Description:
        //  Sets the mode to eSTATEFUL for transaction stateful proxy 
        //  and user agents.
        eSTATEFUL,
        // Description:
        //  Set to eSTATELESS for a stateless proxy.
        eSTATELESS
    };

    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipStatisticsSvc);
    //M5T_INTERNAL_USE_END

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetServiceMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the service for stateful or stateless operation.
    //
    //  Parameters:
    //    eMode:
    //      Either of eSTATEFUL or eSTATELESS.
    //
    //  Description:
    //      Configures how the service behaves under certain conditions. The
    //      service uses less resources when behaving statelessly, but some
    //      retransmissions will not be detected. Set the mode to eSTATEFUL for
    //      transaction stateful proxy and user agents. Set to eSTATELESS for a
    //      stateless proxy.
    //
    //==EDOC====================================================================
    virtual void SetServiceMode(IN const EServiceMode eMode) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTransactionStatistics
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the transaction statistics to be used.
    //
    //  Parameters:
    //    pTransStats:
    //      Pointer to the transaction statistics.
    //
    //  Description:
    //      Configures the transaction statistics to use with this context. If
    //      the statistic service is configured after the server location
    //      service is attached to a context, then the statistics service
    //      automatically sets the proper transaction statistics in the server
    //      location service.
    //
    //==EDOC====================================================================
    virtual void SetTransactionStatistics(
                                IN ISipTransactionStatistics* pTransStats) = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  GetTransactionStatistics
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the transaction statistics to be used.
    //
    //  Parameters:
    //    rpTransStats:
    //      A reference to a pointer that will be filled with the
    //      ISipTransactionStatistics set in this object.
    //
    //      NULL is set if SetTransactionStatistics was not called on this
    //      service.
    //
    //  Description:
    //      Gives the ISipTransactionStatistics to use with this context.
    //
    //==========================================================================
    virtual void GetTransactionStatistics(OUT ISipTransactionStatistics*& rpTransStats) = 0;
//M5T_INTERNAL_USE_END



//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipStatisticsSvc() {};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipStatisticsSvc() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipStatisticsSvc(IN const ISipStatisticsSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipStatisticsSvc& operator=(IN const ISipStatisticsSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================



MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISipStatisticsSvc_H

