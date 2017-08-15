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
#ifndef MXG_ISIPSTATISTICSINFO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSTATISTICSINFO_H
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
#ifndef MXG_METHOD_H
#include "SipParser/Method.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipStatisticsInfo);
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//== Class: ISipStatisticsInfo
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This interface is used to get statistics about sent and received packets,
//   ongoing transactions, and DNS queries.
//
//#The ISipStatisticsInfo is an ECOM object#
//
//  The ISipStatisticsInfo is an ECOM object that is accessed through
//  the following ECOM identifier:
//
//  <B>Interface Id: IID_ISipStatisticsInfo</B>
//
//  A user can call QueryIf on a CLSID_CSipStatisticsContainer object created 
//  with the CreateEComInstance method to get the ISipStatisticsInfo
//  interface of the object. 
//
// Location:
//   SipCoreSvc/ISipStatisticsInfo.h
//
//==EDOC========================================================================
class ISipStatisticsInfo : public IEComUnknown
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
    MX_DECLARE_ECOM_GETIID(ISipStatisticsInfo);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  Reset
    //==
    //==========================================================================
    //
    //  Summary:
    //      Resets all statistics.
    //
    //  Description:
    //      Resets all statistics to zero.
    //
    //==EDOC====================================================================
    virtual void Reset() = 0;

    //==SDOC====================================================================
    //==
    //==  GetTotalNumRequestsRx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the total number of requests received.
    //
    //  Returns:
    //      The total number of requests received.
    //      
    //
    //  Description:
    //      Gets the total number of requests received, including
    //      retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetTotalNumRequestsRx() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetTotalNumRequestsTx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the total number of requests sent.
    //
    //  Returns:
    //      The total number of requests sent.
    //
    //  Description:
    //      Gets the total number of requests sent, including retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetTotalNumRequestsTx() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetTotalNumResponsesRx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the total number of responses received.
    //
    //  Returns:
    //      The total number of responses received.
    //
    //  Description:
    //      Gets the total number of responses received, including
    //      retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetTotalNumResponsesRx() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetTotalNumResponsesTx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the total number of responses sent.
    //
    //  Returns:
    //      The total number of responses sent.
    //
    //  Description:
    //      Gets the total number of responses sent, including retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetTotalNumResponsesTx() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetTotalNumTransactions
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the total number of transactions.
    //
    //  Returns:
    //      The total number of transactions.
    //
    //  Description:
    //      Gets the total number of transactions. This includes terminated and
    //      active transactions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetTotalNumTransactions() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumActiveTransactions
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the total number of active transactions.
    //
    //  Returns:
    //      The total number of active transactions.
    //
    //  Description:
    //      Gets the total number of active transactions. An ACK request to a
    //      2xx INVITE response is considered as a different transaction than
    //      the INVITE, while an ACK to a non 2xx is considered as the same
    //      transaction as the INVITE transaction.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumActiveTransactions() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumRequestsRx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times the specified SIP method has been received.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times the specified SIP method has been received.
    //
    //  Description:
    //      Gets the number of times the specified SIP method has been received. 
    //      This excludes retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumRequestsRx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumRequestsTx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times the specified SIP method has been sent.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times the specified SIP method has been sent.
    //
    //  Description:
    //      Gets the number of times the specified SIP method has been sent. 
    //      This excludes retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumRequestsTx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumFinalResponseRx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times the specified final response has been
    //      received.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times a final response to the specified SIP method has
    //      been received.
    //
    //  Description:
    //      Gets the number of times the specified final response has been  
    //      received. This excludes retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumFinalResponseRx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumNonFinalResponseTx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times the specified non-final response has been
    //      sent.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times a non-final response to the specified SIP method
    //      has been sent.
    //
    //  Description:
    //      Gets the number of times the specified non-final response has been
    //      sent. This excludes retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumNonFinalResponseTx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumFinalResponseTx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times the specified final response has been sent.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times a final response to the specified SIP method has
    //      been sent.
    //
    //  Description:
    //      Gets the number of times the specified final response has been sent. 
    //      This excludes retransmissions.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumFinalResponseTx(IN const ESipMethod eMethod) const = 0;
    
    //==SDOC====================================================================
    //==
    //==  GetNumRequestRetransmissionsTx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times the specified SIP method has been
    //      retransmitted.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times the specified SIP method has been retransmitted.
    //
    //  Description:
    //      Gets the number of times the specified SIP method has been
    //      retransmitted.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumRequestRetransmissionsTx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumFinalResponseRetransmissionsTx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times a final response to the specified SIP
    //      method has been retransmitted.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times a final response to the specified SIP method has
    //      been retransmitted.
    //
    //  Description:
    //      Gets the number of times a final response to the specified SIP 
    //      method has been retransmitted.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumFinalResponseRetransmissionsTx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumNonFinalResponseRetransmissionsTx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times a non-final response to the specified SIP
    //      method has been retransmitted.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times a non-final response to the specified SIP method
    //      has been retransmitted.
    //
    //  Description:
    //      Gets the number of times a non-final response to the specified SIP
    //      method has been retransmitted. This occurs if a non-final response
    //      is sent to a request and a retransmission of the request is 
    //      received. The stack sends back the non-final response again, thus it
    //      is counted as a retransmission.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumNonFinalResponseRetransmissionsTx(IN const ESipMethod eMethod) const = 0;
    
    //==SDOC====================================================================
    //==
    //==  GetNumRequestRetransmissionsRx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times the specified SIP method has been received 
    //      and processed as a retransmission.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times the specified SIP method has been received and
    //      processed as a retransmission.
    //
    //  Description:
    //      Gets the number of times the specified SIP method has been received
    //      and processed as a retransmission.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumRequestRetransmissionsRx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumFinalResponseRetransmissionsRx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times a final response to the specified SIP
    //      method has been received and processed as a retransmission.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times a final response to the specified SIP method
    //      has been received and processed as a retransmission.
    //
    //  Description:
    //      Gets the number of times a final response to the specified SIP
    //      method has been received and processed as a retransmission.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumFinalResponseRetransmissionsRx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumNonFinalResponseRx
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times a non-final response to the specified SIP
    //      method has been received.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method.
    //
    //  Returns:
    //      The number of times a non-final response to the specified SIP method
    //      has been received.
    //
    //  Description:
    //      Gets the number of times a non-final response to the specified SIP
    //      method has been received.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumNonFinalResponseRx(IN const ESipMethod eMethod) const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumDnsQueriesFailed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times a DNS query has failed.
    //
    //  Returns:
    //      The number of times a DNS query has failed.
    //
    //  Description:
    //      Gets the number of times a DNS query has failed. DNS queries done
    //      by the CSipPersistentConnectionList, CSipPrivacySvc, and
    //      CSipUaAssertedIdentitySvc are not included.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumDnsQueriesFailed() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetNumDnsQueriesSucceeded
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the number of times a DNS query has succeeded.
    //
    //  Returns:
    //      The number of times a DNS query has succeeded.
    //
    //  Description:
    //      Gets the number of times a DNS query has succeeded. DNS queries done
    //      by the CSipPersistentConnectionList, CSipPrivacySvc, and
    //      CSipUaAssertedIdentitySvc are not included.
    //
    //==EDOC====================================================================
    virtual unsigned int GetNumDnsQueriesSucceeded() const = 0;


//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipStatisticsInfo() {};

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipStatisticsInfo() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipStatisticsInfo(IN const ISipStatisticsInfo& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipStatisticsInfo& operator=(IN const ISipStatisticsInfo& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSTATISTICSINFO_H

