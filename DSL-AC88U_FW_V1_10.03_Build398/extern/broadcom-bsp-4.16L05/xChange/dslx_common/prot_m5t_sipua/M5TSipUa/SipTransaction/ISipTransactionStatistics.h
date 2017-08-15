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
#ifndef MXG_ISIPTRANSACTIONSTATISTICS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPTRANSACTIONSTATISTICS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipTransactionStatistics);
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//== Class: ISipTransactionStatistics
//========================================
//
// Description:
//   This interface is used to signal to a container some statistical data. It 
//   is notified of sent and received packets, of the creation and termination
//   of transaction, and of DNS queries.
//
//#The ISipTransactionStatistics is an ECOM object#
//
//  The ISipTransactionStatistics is an ECOM object that is accessed through
//  the following ECOM identifier:
//
//  <B>Interface Id: IID_ISipTransactionStatistics</B>
//
//  A user can call QueryIf on a CLSID_CSipStatisticsContainer object created 
//  with the CreateEComInstance method to get the ISipTransactionStatistics
//  interface of the object. 
//
// Location:
//   SipTransaction/ISipTransactionStatistics.h
//
//==EDOC========================================================================
class ISipTransactionStatistics : public IEComUnknown
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
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipTransactionStatistics);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  NotifySentPacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification that a packet has been successfully sent.
    //
    //  Parameters:
    //    rPacket:
    //      The sent packet.
    //
    //    bRetransmission:
    //      True if the packet was a retransmission.
    //
    //  Description:
    //      Notifies the implementation that a packet has been sent.
    //
    //==EDOC====================================================================
    virtual void NotifySentPacket(IN const CSipPacket& rPacket, 
                                  IN bool bRetransmission) = 0;
    
    
    //==SDOC====================================================================
    //==
    //==  NotifyReceivedPacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification that a packet has been received.
    //
    //  Parameters:
    //    rPacket:
    //      The received packet.
    //
    //    bRetransmission:
    //      True if the packet was a retransmission.
    //
    //  Description:
    //      Notifies the implementation that a packet has been received.
    //
    //==EDOC====================================================================
    virtual void NotifyReceivedPacket(IN const CSipPacket& rPacket,
                                      IN bool bRetransmission) = 0;


    //==SDOC====================================================================
    //==
    //==  NotifyTransactionStart
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification that a new transaction has been created.
    //
    //  Description:
    //      Notifies the implementation that a new transaction has been created.
    //
    //==EDOC====================================================================
    virtual void NotifyTransactionStart() = 0;


    //==SDOC====================================================================
    //==
    //==  NotifyTransactionEnd
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification that a transaction has been terminated.
    //
    //  Description:
    //      Notifies the implementation that a transaction has been terminated.
    //
    //==EDOC====================================================================
    virtual void NotifyTransactionEnd() = 0;


    //==SDOC====================================================================
    //==
    //==  NotifyDnsQueryResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notification that a DNS query has been done.
    //
    //  Parameters:
    //    bRes:
    //      Tells whether or not the DNS query was successful.
    //
    //  Description:
    //      Notifies the implementation that a DNS query has been done.
    //
    //
    //==EDOC====================================================================
    virtual void NotifyDnsQueryResult(IN bool bRes) = 0;
    
//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipTransactionStatistics() {};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipTransactionStatistics() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipTransactionStatistics(IN const ISipTransactionStatistics& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipTransactionStatistics& operator=(IN const ISipTransactionStatistics& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTRANSACTIONSTATISTICS_H

