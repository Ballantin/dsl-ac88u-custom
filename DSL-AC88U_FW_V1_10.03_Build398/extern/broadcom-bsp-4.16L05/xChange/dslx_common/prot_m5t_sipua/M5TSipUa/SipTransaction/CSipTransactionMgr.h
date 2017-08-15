//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPTRANSACTIONMGR_H
#define MXG_CSIPTRANSACTIONMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H 
#include "Config/SipStackCfg.h" //MXD_SEQUENTIAL_TRANSACTION_TABLE
#endif

//-- Data Members
//-----------------
#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSIPTRANSACTION_H
#include "SipTransaction/CSipTransaction.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_ISIPTRANSACTIONMGR_H
#include "SipTransaction/ISipTransactionMgr.h"
#endif

#ifndef MXG_ISIPTRANSPORTOBSERVER_H
#include "SipTransport/ISipTransportObserver.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CSipHeader;
class ISipTransactionStatistics;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: CSipTransactionMgr
//========================================
//
// Description:
//   The transaction manager is responsible for managing the SIP transaction
//   level, pretty much as defined in RFC 3261, section 17. The transaction
//   manager offers services to the Transaction User, which generally is a
//   User-Agent or a stateful proxy.
//
//   The transaction manager receives a SIP packet from the Transaction User and
//   sends it to a specific destination, while managing timeouts and
//   retransmissions. This class is not responsible for performing server
//   location as defined in RFC 3263. The packet must specify the transport to
//   use and a fully-resolved peer address where the packet is to be sent.
//
//   The transaction manager offers the proper APIs to create client and
//   server transactions for INVITE and non-INVITE requests.
//
//   Its responsibilities for client transactions it manages include:
//
//      - Notifies the Transaction User when the transaction times out.
//      - Notifies the Transaction User when a transport error is reported.
//      - Retransmits requests upon retransmission timeout.
//      - Automatically sends ACKs for negative final responses to INVITE, as
//        this is a hop by hop request.
//      - Notifies the Transaction User when receiving responses to the request.
//      - Buffers final responses retransmissions.
//
//   Its responsibilities for server transactions it manages include:
//
//      - Buffers request retransmissions.
//      - Buffers ACKs to negative final responses, as they are generated hop
//        by hop and should not contain anything useful to the Transaction User.
//      - Notifies the Transaction User when a transport error is reported.
//
//   This class is not responsible for managing CANCEL requests. CANCEL requests
//   are viewed as a normal transaction by this manager. The Transaction User
//   is responsible to create a transaction for each CANCEL request sent or
//   received. The manager only reports the reception of a CANCEL through the
//   event EvRequestCancelled.
//
// Location:
//   SipTransaction/CSipTransactionMgr.h
//
// See Also:
//   ISipTransactionMgr, ISipTransportObserver
//
//==EDOC========================================================================
class CSipTransactionMgr : public ISipTransactionMgr,
                           public ISipTransportObserver,
                           public CEventDriven
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipTransactionMgr();


    //-- << ISipTransactionMgr >>
    //--------------------------------------------


    //-- << Initialization and Termination>>
    //--------------------------------------------

    // Summary:
    //  Sets the transport manager.
    //------------------------------
    virtual mxt_result Initialize(IN ISipTransportMgr& rTransportMgr);

    // Summary:
    //  Removes all the active transactions asynchronously.
    //------------------------------------------------------
    virtual void ShutdownA(IN ISipTransportUser& rUser,
                           IN mxt_opaque opq);


    //-- << Transaction Management >>
    //--------------------------------------------

    // Summary:
    //  Removes the specified transaction from the active transactions list.
    //-----------------------------------------------------------------------
    virtual
    mxt_result ForceTransactionTimeout(IN mxt_opaque opqTransaction);

    // Summary:
    //  Removes the specified transaction from the active transactions list.
    //-----------------------------------------------------------------------
    virtual mxt_result AbandonLastResponse(IN mxt_opaque opqTransaction);

    // Summary:
    //  Creates a server or client transaction.
    //------------------------------------------
    virtual mxt_result CreateTransaction(IN ETransactionRole eRole,
                                         IN const CSipPacket& rPacket,
                                         IN ISipTransactionUser* pUser,
                                         IN  ISipTransactionStatistics* pTransStats,
// BRCM_CUSTOM - [add] Add CCTK GUID opaque
                                         IN mxt_opaque opqGuid,
// BRCM_CUSTOM - [end] Add CCTK GUID opaque
// BRCM_CUSTOM - [add] Add TOS config support
                                         IN mxt_opaque opqTos,
// BRCM_CUSTOM - [end] Add TOS config support
                                         OUT mxt_opaque&                ropqTransaction,
                                         IN  const ISipTransactionMgr::STransactionsTimers* pstTransactionTimers);


    // Summary:
    //  Sets the transaction statistics container to use.
    //----------------------------------------------------
    virtual void SetTransactionStatistics(
                                IN mxt_opaque opqTransaction,
                                IN ISipTransactionStatistics* pTransStats);

    virtual mxt_result StopRetransmission(IN mxt_opaque opqTransaction);

    // Summary:
    //  Sends a response on the specified transaction.
    //-------------------------------------------------
    virtual mxt_result SendResponse(IN const CSipPacket&    rPacket,
                                    IN mxt_opaque           opqTransaction);

    // Summary:
    //  Removes the transaction from the list. Only used by the transaction
    //  child classes.
    //----------------------------------------------------------------------
    void RemoveTransaction(IN CSipTransaction& rTransaction);


    //-- << ISipTransportObserver >>
    //--------------------------------------------

    virtual mxt_result EvPacketReceived(IN const CSipPacket& rPacket,
                                        IN bool bHandled);

    virtual void EvTransportError(IN mxt_result    res,
                                  IN const CSocketAddr&  rSource,
                                  IN const CSocketAddr&  rDestination,
                                  IN ESipTransport eTransport);

    virtual void EvConnectionClosed(IN const CSocketAddr&   rSource,
                                    IN const CSocketAddr&   rDestination,
                                    IN ESipTransport        eTransport,
                                    IN EClosureType         eClosureType);

    virtual void EvConnectionEstablished(IN const CSocketAddr& rSource,
                                         IN const CSocketAddr& rDestination,
                                         IN ESipTransport      eTransport);

    virtual void EvObserverRemoved();


//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Destructor. Cannot use : must call Release instead.
    //------------------------------------------------------
    virtual ~CSipTransactionMgr();


private:

    //-- << CEventDriven >>.
    //-----------------------

    // Summary:
    //  Notifies the manager that a new message must be processed.
    //-------------------------------------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool         bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler*  pParameter);


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipTransactionMgr(IN const CSipTransactionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipTransactionMgr& operator=(IN const CSipTransactionMgr& rSrc);


    //-- << Internal messaging methods >>.
    //-------------------------------------

    // Summary:
    //  Shutdown the object.
    //-----------------------
    void InternalShutdownA(IN CMarshaler* pParams);

    // Summary:
    //  Warns the transport user that shutdown has been completed.
    //-------------------------------------------------------------
    void InternalShutdownCompletedA(IN CMarshaler* pParams);

    // Summary:
    //  Handles transport errors.
    //-------------------------------------------------------------
    void InternalEvTransportError(IN CMarshaler* pParams);

    //-- << Helpers >>.
    //------------------

    // Summary:
    //  Asynchronously warn the transport user that shutdown has been completed.
    //--------------------------------------------------------------------------
    void ShutdownCompletedA(IN ISipTransportUser* pUser,
                            IN mxt_opaque opq);

    // Summary:
    //  Returns true if the transaction is found in the list.
    //--------------------------------------------------------
    bool FindTransaction(IN CSipTransaction& rTransaction,
                         IN bool bRemoveIfFound);

    // Summary:
    //  Removes the specified transaction handle.
    //-------------------------------------------
    mxt_result RemoveTransactionHelper(IN mxt_opaque opqTransaction, IN bool bServerTransaction = false);


//-- Hidden Data Members
//------------------------
protected:

    // The list of transactions. This contains all the transactions currently
    // active in the system. There are 2 transaction types: Client and server.
    // The server type needs to be separated in two different categories: One
    // to treat requests built according to RFC 3261 and the other to treat
    // requests according to RFC 2543. This data structure is a map of vectors
    // as a transaction is not identified uniquely by its key.
    //-------------------------------------------------------------------------
#ifdef MXD_SEQUENTIAL_TRANSACTION_TABLE
    CVector<CSipTransaction*> m_vecpTransactionTable;
#else
    #if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 does not support default parameters in templates,
        // in which case all the parameters must be specified.
        CMap<unsigned int, CVector<CSipTransaction*>*,
            CAATree<CMapPair<unsigned int, CVector<CSipTransaction*>*> > > m_mapTransactionTable;
    #else
        CMap<unsigned int, CVector<CSipTransaction*>*> m_mapTransactionTable;
    #endif
#endif // #ifdef MXD_SEQUENTIAL_TRANSACTION_TABLE

    // The vector of transaction that could possibly receive merged requests
    // corresponding to them.
    //-----------------------------------------------------------------------
    CVector<CSipTransaction*> m_vecpMergeDetectionTransactions;

    // Mutex needed to synchronize the transactions table because it can be
    // accessed by the SipTransport and the SipCore threads.
    //----------------------------------------------------------------------
    CMutex m_mutexTransactionTable;

private:
    // Asynchronous events IDs.
    //--------------------------
    enum EMsgNumber
    {
        eSHUTDOWNA,
        eSHUTDOWNCOMPLETEDA,
        eTRANSPORT_ERROR
    };

    bool m_bIsShuttingDown;

};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPTRANSACTIONMGR_H

