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
#ifndef MXG_CSIPSTATISTICSCONTAINER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPSTATISTICSCONTAINER_H
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
// MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT
//------------------------------------------------
#include "Config/SipStackCfg.h"
#endif


#ifndef MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT
#error You must define MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT to be able to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_METHOD_H
#include "SipParser/Method.h"
#endif

//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_ISIPSTATISTICSINFO_H
#include "SipCoreSvc/ISipStatisticsInfo.h"
#endif

#ifndef MXG_ISIPTRANSACTIONSTATISTICS_H
#include "SipTransaction/ISipTransactionStatistics.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
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
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(CSipStatisticsContainer);
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//== Class: CSipStatisticsContainer
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//      This class contains the entire statistics defined in the class
//      ISipStatisticsInfo. When passed to the statistics or server location
//      service, data is added to this container by using the implemented
//      interface of ISipTransactionStatistics. Statistics can be retrieved at
//      any time by calling one of the get methods. The same container can be
//      used for all contexts or a new one can be used for each context.
//
//      Note that a DNS request done by the CSipUAAssertedIdentitySvc,
//      CSipPersistentConnexionList, or CSipPrivacySvc are not counted in the
//      statistics.
//
//      Packets locally generated in the transaction are not counted in the
//      statistics. These packets are not received by a socket. They are only
//      generated to properly terminate the transaction.
//
//#The CSipStatisticsContainer is an ECOM object#
//
//  The CSipStatisticsContainer is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_ISipTransactionStatistics</B>
//
//  <B>Interface Id: IID_ISipTransactionStatistics</B>
//
//  A user can call QueryIf on a CLSID_CSipStatisticsContainer object created
//  with the CreateEComInstance method to get either the
//  ISipTransactionStatistics or the ISipStatisticsInfo interface of the object.
//
// Location:
//   SipCoreSvc/CSipStatisticsContainer.h
//
// See Also:
//   ISipStatisticsInfo, ISipTransactionStatistics
//
//==EDOC========================================================================
class CSipStatisticsContainer : public ISipStatisticsInfo,
                                public ISipTransactionStatistics,
                                private CEComDelegatingUnknown
{
//-- Friend Declarations
//------------------------
    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- New types within class namespace
//-----------------------------------

//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(CSipStatisticsContainer);
    //M5T_INTERNAL_USE_END

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSipStatisticsContainer();

    //-- << EComDeletatingUnknown Interface >>
    //-----------------------------------------
    MX_DECLARE_DELEGATING_IECOMUNKNOWN;

    //-- << ISipStatisticsInfo Interface >>
    //--------------------------------------

    //  Summary:
    //      Resets all statistics.
    //-----------------------------
    virtual void Reset();

    //  Summary:
    //      Gets the total number of requests received.
    //--------------------------------------------------
    virtual unsigned int GetTotalNumRequestsRx() const;

    //  Summary:
    //      Gets the total number of requests sent.
    //----------------------------------------------
    virtual unsigned int GetTotalNumRequestsTx() const;

    //  Summary:
    //      Gets the total number of responses received.
    //---------------------------------------------------
    virtual unsigned int GetTotalNumResponsesRx() const;

    //  Summary:
    //      Gets the total number of responses sent.
    //-----------------------------------------------
    virtual unsigned int GetTotalNumResponsesTx() const;

    //  Summary:
    //      Gets the total number of transactions.
    //---------------------------------------------
    virtual unsigned int GetTotalNumTransactions() const;

    //  Summary:
    //      Gets the total number of active transactions.
    //----------------------------------------------------
    virtual unsigned int GetNumActiveTransactions() const;

    //  Summary:
    //      Gets the number of times the specified method has been received.
    //-----------------------------------------------------------------------
    virtual unsigned int GetNumRequestsRx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times the specified method has been sent.
    //-------------------------------------------------------------------
    virtual unsigned int GetNumRequestsTx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times the specified final response has been
    //      received.
    //---------------------------------------------------------------------
    virtual unsigned int GetNumFinalResponseRx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times the specified non-final response has been
    //      sent.
    //-------------------------------------------------------------------------
    virtual unsigned int GetNumNonFinalResponseTx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times the specified final response has been
    //      sent.
    //---------------------------------------------------------------------
    virtual unsigned int GetNumFinalResponseTx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times the specified method has been
    //      retransmitted.
    //-------------------------------------------------------------
    virtual unsigned int GetNumRequestRetransmissionsTx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times a final response to the specified method
    //      has been retransmitted.
    //------------------------------------------------------------------------
    virtual unsigned int GetNumFinalResponseRetransmissionsTx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times a non-final response to the specified
    //      method has been retransmitted.
    //---------------------------------------------------------------------
    virtual unsigned int GetNumNonFinalResponseRetransmissionsTx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times the specified method has been received and
    //      processed as a retransmission.
    //--------------------------------------------------------------------------
    virtual unsigned int GetNumRequestRetransmissionsRx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times a final response to the specified method
    //      has been received and processed as a retransmission.
    //------------------------------------------------------------------------
    virtual unsigned int GetNumFinalResponseRetransmissionsRx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times a non-final response to the specified
    //      method has been received.
    //---------------------------------------------------------------------
    virtual unsigned int GetNumNonFinalResponseRx(IN const ESipMethod eMethod) const;

    //  Summary:
    //      Gets the number of times a DNS query has failed.
    //-------------------------------------------------------
    virtual unsigned int GetNumDnsQueriesFailed() const;

    //  Summary:
    //      Gets the number of times a DNS query has succeeded.
    //----------------------------------------------------------
    virtual unsigned int GetNumDnsQueriesSucceeded() const;


    //-- << ISipTransactionStatistics Interface >>
    //---------------------------------------------

    //  Summary:
    //      Notifies that a packet has been successfully sent.
    //---------------------------------------------------------
    void NotifySentPacket(IN const CSipPacket& rPacket,
                          IN bool bRetransmission);

    //  Summary:
    //      Notifies that a packet has been received.
    //------------------------------------------------
    void NotifyReceivedPacket(IN const CSipPacket& rPacket,
                              IN bool bRetransmission) ;

    //  Summary:
    //      Notifies that a new transaction has been created.
    //--------------------------------------------------------
    void NotifyTransactionStart();

    //  Summary:
    //      Notifies that a transaction has been terminated.
    //-------------------------------------------------------
    void NotifyTransactionEnd();

    //  Summary:
    //      Notifies that a DNS query has been done.
    //-----------------------------------------------
    void NotifyDnsQueryResult(IN bool bRes);

//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipStatisticsContainer();

    static void FinalizeCSipStatisticsContainer();

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipStatisticsContainer(IN IEComUnknown& rOuterIEComUnknown);

    //-- << EComDeletatingUnknown Interface >>
    //-----------------------------------------
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    static mxt_result CreateInstance(
                                IN IEComUnknown* pOuterIEComUnknown,
                                OUT CEComUnknown** ppCEComUnknown);

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipStatisticsContainer(IN const CSipStatisticsContainer& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipStatisticsContainer& operator=(IN const CSipStatisticsContainer& rSrc);


//-- Hidden Data Members
//------------------------
private:
    //  The total number of requests received.
    //-----------------------------------------
    unsigned int m_uTotalNumRequestsRx;

    //  The total number of requests sent.
    //-------------------------------------
    unsigned int m_uTotalNumRequestsTx;

    //  The total number of responses received.
    //------------------------------------------
    unsigned int m_uTotalNumResponsesRx;

    //  The total number of responses sent.
    //--------------------------------------
    unsigned int m_uTotalNumResponsesTx;

    //  The total number of transactions.
    //------------------------------------
    unsigned int m_uTotalNumTransactions;

    //  The total number of active transactions.
    //-------------------------------------------
    unsigned int m_uNumActiveTransactions;

    //  The number of times the specified method was received.
    //---------------------------------------------------------
    unsigned int m_auNumRequestsRx[eSIP_METHOD_UNKNOWN];

    //  The number of times the specified method was sent.
    //-----------------------------------------------------
    unsigned int m_auNumRequestsTx[eSIP_METHOD_UNKNOWN];

    //  The number of times a non-final response to the specified method
    //  was received
    //--------------------------------------------------------------------
    unsigned int m_auNumNonFinalResponseRx[eSIP_METHOD_UNKNOWN];

    //  The number of times the specified final response was received.
    //-----------------------------------------------------------------
    unsigned int m_auNumFinalResponseRx[eSIP_METHOD_UNKNOWN];

    //  The number of times the specified non-final response was sent.
    //-----------------------------------------------------------------
    unsigned int m_auNumNonFinalResponseTx[eSIP_METHOD_UNKNOWN];

    //  The number of times the specified final response was sent.
    //-------------------------------------------------------------
    unsigned int m_auNumFinalResponseTx[eSIP_METHOD_UNKNOWN];

    //  The number of times the specified method was retransmitted.
    //--------------------------------------------------------------
    unsigned int m_auNumRequestRetransmissionsTx[eSIP_METHOD_UNKNOWN];

    //  The number of times a final response to the specified method
    //  was retransmitted.
    //----------------------------------------------------------------
    unsigned int m_auNumFinalResponseRetransmissionsTx[eSIP_METHOD_UNKNOWN];

    //  The number of times a non-final response to the specified method
    //  was retransmitted.
    //--------------------------------------------------------------------
    unsigned int m_auNumNonFinalResponseRetransmissionsTx[eSIP_METHOD_UNKNOWN];

    //  The number of times the specified method was received and
    //  processed as a retransmission.
    //-------------------------------------------------------------
    unsigned int m_auNumRequestRetransmissionsRx[eSIP_METHOD_UNKNOWN];

    //  The number of times a final response to the specified method
    //  was received and processed as a retransmission.
    //---------------------------------------------------------------------
    unsigned int m_auNumFinalResponseRetransmissionsRx[eSIP_METHOD_UNKNOWN];

    //  The number of times a DNS query has failed.
    //----------------------------------------------
    unsigned int m_uNumDnsQueriesFailed;

    //  The number of times a DNS query has succeded.
    //------------------------------------------------
    unsigned int m_uNumDnsQueriesSucceded;

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
inline unsigned int CSipStatisticsContainer::GetTotalNumRequestsRx() const
{
    return m_uTotalNumRequestsRx;
}

inline unsigned int CSipStatisticsContainer::GetTotalNumRequestsTx() const
{
    return m_uTotalNumRequestsTx;
}

inline unsigned int CSipStatisticsContainer::GetTotalNumResponsesRx() const
{
    return m_uTotalNumResponsesRx;
}

inline unsigned int CSipStatisticsContainer::GetTotalNumResponsesTx() const
{
    return m_uTotalNumResponsesTx;
}

inline unsigned int CSipStatisticsContainer::GetTotalNumTransactions() const
{
    return m_uTotalNumTransactions;
}

inline unsigned int CSipStatisticsContainer::GetNumActiveTransactions() const
{
    return m_uNumActiveTransactions;
}

inline unsigned int CSipStatisticsContainer::GetNumRequestsRx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumRequestsRx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumRequestsTx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumRequestsTx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumFinalResponseRx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumFinalResponseRx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumNonFinalResponseTx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumNonFinalResponseTx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumFinalResponseTx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumFinalResponseTx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumRequestRetransmissionsTx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumRequestRetransmissionsTx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumFinalResponseRetransmissionsTx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumFinalResponseRetransmissionsTx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumNonFinalResponseRetransmissionsTx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumNonFinalResponseRetransmissionsTx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumRequestRetransmissionsRx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumRequestRetransmissionsRx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumFinalResponseRetransmissionsRx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumFinalResponseRetransmissionsRx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumNonFinalResponseRx(
                                              IN const ESipMethod eMethod) const
{
    return m_auNumNonFinalResponseRx[eMethod];
}

inline unsigned int CSipStatisticsContainer::GetNumDnsQueriesFailed() const
{
    return m_uNumDnsQueriesFailed;
}

inline unsigned int CSipStatisticsContainer::GetNumDnsQueriesSucceeded() const
{
    return m_uNumDnsQueriesSucceded;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSTATISTICSCONTAINER_H


