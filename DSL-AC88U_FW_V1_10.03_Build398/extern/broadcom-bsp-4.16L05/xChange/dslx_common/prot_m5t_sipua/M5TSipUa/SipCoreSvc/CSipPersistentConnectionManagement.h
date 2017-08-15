//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_CSIPPERSISTENTCONNECTIONMANAGEMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPPERSISTENTCONNECTIONMANAGEMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT) || \
    !defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT \
and MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT \
to be able to use this class.
#endif

//-- Data Members

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPSWITCHBACKPERSISTENTCONNECTION_H
#include "SipCoreSvc/ISipSwitchbackPersistentConnection.h"
#endif

#ifndef MXG_ISIPPERSISTENTCONNECTIONMGR_H
#include "SipCoreSvc/ISipPersistentConnectionMgr.h"
#endif

#ifndef MXG_CSERVERLOCATOR_H
#include "SipCoreSvc/CServerLocator.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CServerLocator;
class CSipPersistentConnectionList;
class CSipUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipPersistentConnectionManagement
//==============================================================================
//
// Description:
//  Non aggregate implementation of the CSipPersistentConnectionManagement
//  interface. This class is responsible to manage the persistent connections
//  and perform DNS requests to realize the switchback feature.
//
//  DNS requests are performed when:
//  - Establish(FQDN) is made.
//  - A failure occurs on the top priority established connection.
//  - The error timeout elapses.
//
//  It uses CSipPersistentConnectionList only to establish and re-establish
//  persistent connections based on IP addresses.
//
// Location:
//   SipCoreSvc/CSipPersistentConnectionManagement.h
//
// See Also:
//   ISipSwitchbackConnectionList
//
//==============================================================================
class CSipPersistentConnectionManagement : private CEComUnknown,
                                           private CEventDriven,
                                           private ISipSwitchbackPersistentConnection,
                                           private ISipPersistentConnectionMgr,
                                           private IServerLocatorMgr
{
//-- Published Interface
public:
    // << ECom Pattern >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Initialization mechanism >>
    static mxt_result InitializeCSipPersistentConnectionManagement();

    static void FinalizeCSipPersistentConnectionManagement();

//-- Hidden Methods
protected:
private:
    // Inherited from CEComUnknown.
    MX_DECLARE_IECOMUNKNOWN

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Inherited from CEventDriven.
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    // Inherited from ISipSwitchbackPersistentConnection.
    virtual mxt_result SetErrorTimeoutMs(IN unsigned int uTimeoutMs);

    virtual mxt_result Establish(IN ESipTransport eTransport,
                                 IN mxt_opaque opqConnection,
                                 IN uint16_t uLocalPort,
                                 IN const CString& rstrPeerHost,
                                 IN uint16_t uPeerPort,
                                 IN ISipSwitchbackPersistentConnectionMgr* pMgr);

    virtual void Terminate();

    virtual mxt_result Terminate(IN mxt_opaque opqConnection);

    virtual mxt_result ConnectivityVerified(IN mxt_opaque opqConnection);

    virtual mxt_result GenerateError(IN mxt_opaque opqConnection);

    virtual bool IsVerified(IN mxt_opaque opqConnection) const;

    virtual mxt_result GetPriority(IN mxt_opaque opqConnection,
                                   OUT unsigned int& ruPriority) const;

    virtual mxt_result GetConnection(IN mxt_opaque opqConnection,
                                     OUT ESipTransport& reTransport,
                                     OUT const CSocketAddr*& rpLocalAddr,
                                     OUT const CSocketAddr*& rpPeerAddr) const;

    virtual void GetConnectionList(OUT CVector<mxt_opaque>& rvecopqConnection) const;

    virtual mxt_result UpdatePacket(INOUT CSipPacket& rPacket,
                                    INOUT mxt_opaque& ropqConnection) const;

    // Inherited from ISipPersistentConnectionMgr.
    virtual void EvConnectionEstablished(IN mxt_opaque opqPersistentConnectionList,
                                         IN const CSocketAddr& rLocalAddr,
                                         IN const CSocketAddr& rPeerAddr);

    virtual void EvConnectionTerminated(IN mxt_opaque opqPersistentConnectionList);

    virtual void EvErrorOnConnection(IN mxt_opaque opqPersistentConnectionList,
                                     IN mxt_result resError);

    virtual void EvSipErrorOnConnection(IN mxt_opaque opqConnectionHandle, 
                                        IN const CSipPacket& rSentPacket,
                                        IN mxt_result resConnectionError);

#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
    virtual void EvStartKeepAliveResult(IN mxt_opaque opqPersistentConnectionList,
                                        IN mxt_result res);

    virtual void EvSendResult(IN mxt_opaque opqPersistentConnectionList,
                              IN mxt_opaque opq,
                              IN mxt_result res);
#endif // -- #ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT

    // Inherited from IServerLocatorMgr.
    virtual void OnAllTargetsResolved(IN TOA CList<SNaptrRecord>* plststNaptrRecords,
                                      IN mxt_opaque opq);

    virtual void OnTargetResolved(IN TOA CList<SNaptrRecord>* plststNaptrRecords,
                                  IN mxt_opaque opq);

    virtual void OnTargetResolved(IN TOA CList<SSrvRecord>* plststSrvRecords,
                                  IN mxt_opaque opq);

    virtual void OnTargetResolved(IN TOA CList<CSocketAddr>* plstSocketAddr,
                                  IN mxt_opaque opq);

    // Helper methods

    // Summary:
    //  Manages the DNS results.
    void InternalOnAllTargetsResolvedNaptr(IN CMarshaler* pParams);

    // The state of a persistent connection.
    enum EPersistentConnectionState
    {
        // Ready to be connected using Establish or ReEstablish.
        eREADY_TO_CONNECT,
        // Trying to connect (waiting feedback from Establish or ReEstablish).
        eCONNECTING,
        // Connected (EvEstablished was called).
        eESTABLISHED,
        // Connected and verified (EvEstablished and ConnectivityVerified were
        // called).
        eESTABLISHED_VERIFIED,
        // Cannot try to reconnect until that the error timeout expires.
        eWAITING_IN_ERROR,
        // In the process of terminating.
        eTERMINATING
    };

    // Represents a persistent connection.
    struct SPersistentConnection
    {
        // The state of the persistent connection.
        EPersistentConnectionState m_eState;
        // The identifier of the persistent connection object within
        // CSipPersistentConnectionList. It is set when Establish is called on
        // CSipPersistentConnectionList.
        mxt_opaque m_opqPersistentConnectionList;
        // The peer address found in the SRV record as returned by the DNS
        // query. In case the SRV record had a port set to zero and Establish()
        // was called on CSipPersistentConnectionList, the port of this peer
        // address is the default port according to the transport.
        CSocketAddr m_peerAddr;

        SPersistentConnection(IN const CSocketAddr& rPeerAddr)
        :   m_eState(eREADY_TO_CONNECT),
            m_opqPersistentConnectionList(0),
            m_peerAddr(rPeerAddr)
        {}
    };

    // Summary:
    //  Finds the persistent connection according to the opaque of
    //  CSipPersistentConnectionList.
    CSipPersistentConnectionManagement::SPersistentConnection*
        FindConnection(IN mxt_opaque opqPersistentConnectionList) const;

    // Summary:
    //  Finds a persistent connection zero-based index according to the peer
    //  address.
    unsigned int FindConnection(IN const CSocketAddr& rPeerAddress) const;

    // Summary:
    //  Connects to the highest priority available target.
    void ConnectToHighestPriorityAvailableTarget();

    // Summary:
    //  Sets the state associated to a failure.
    void FailureOccured(IN CSipPersistentConnectionManagement::SPersistentConnection* pstPersistentConnection,
                        IN mxt_result res);

    // Summary:
    //  Tells if the switchback instance can terminate.
    bool CanTerminate() const;

    // Summary:
    //  The termination process has completed. It resets the state to idle so
    //  the application can call again Establish.
    void TerminateCompleted();

    // Summary:
    //  Does a query to the DNS.
    void QueryDns();

    // Summary:
    //  Tells if the specified persistent connection is the highest priority
    //  currently established.
    bool IsHighestPriorityConnectionEstablished(
        IN CSipPersistentConnectionManagement::SPersistentConnection* pstPesistentConnection) const;

    // Summary:
    //  Sets the specified bitmask in the bitset.
    void SetInBitSet(IN int nBitMask, INOUT int& rnBitSet) const;

    // Summary:
    //  Resets the specified bitmask in the bitset.
    void ResetInBitSet(IN int nBitMask, INOUT int& rnBitSet) const;

    // Summary:
    //  Tells if the specified bitmask is present in the bitset.
    bool IsInBitSet(IN int nBitMask, IN int nBitSet) const;

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipPersistentConnectionManagement();

    // Summary:
    //  Destructor.
    virtual ~CSipPersistentConnectionManagement();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipPersistentConnectionManagement(IN const CSipPersistentConnectionManagement& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipPersistentConnectionManagement& operator=(IN const CSipPersistentConnectionManagement& rSrc);

// Friends declarations
    friend struct SPersistentConnection;

//-- Hidden Data Members
protected:
private:
    // Messages.
    enum EMessage
    {
        eONALLTARGETSRESOLVEDNAPTR
    };

    // The server locator used to resolve FQDN/hostname.
    CServerLocator* m_pServerLocator;

    // The persistent connection list. It is used to establish and re-establish
    // persistent connections.
    CSipPersistentConnectionList* m_pPersistentConnectionList;

    // The manager.
    ISipSwitchbackPersistentConnectionMgr* m_pMgr;

    // The configurable error timeout used when starting a timer because of a
    // failure that occurred while connecting or when a connection was connected.
    unsigned int m_uErrorTimeoutMs;

    // This is the global state of the switchback mechanism.
    enum EGlobalState
    {
        // Nothing has occurred. Establish has not been made yet.
        eIDLE = 1,
        // Establish method was called.
        eESTABLISH_DONE = 2,
        // Failure occurred on one or many persistent connections and the error
        // timeout timer is running.
        eERROR_OCCURRED = 4,
        // DNS request is underway.
        eRESOLVING = 8,
        // Terminate method was called. When in this state, the state
        // returns to eIDLE when all persistent connections are terminated.
        eTERMINATE_DONE = 16
    };

    // The global state of the switchback mechanism.
    int m_nBitSetGlobalState;

    // The transport specified by Establish.
    ESipTransport m_eTransport;

    // The local address specified by Establish.
    mxt_opaque m_opqLocal;
    uint16_t m_uLocalPort;

    // The URI to resolve.
    CSipUri* m_pSipUriToResolve;

    // The persistent connections. The order of the vector defines the priority
    // of the persistent connections as returned by the DNS. Item at index zero
    // has the highest priority.
    CVector<SPersistentConnection*> m_vecpstPersistentConnection;
};

//==============================================================================
//==
//==  CanTerminate
//==
//==============================================================================
//
//  Summary:
//      Tells if the switchback instance can terminate.
//
//  Returns:
//      - true: Can terminate.
//      - false: Cannot terminate.
//
//  Description:
//      Tells if there are persistent connections left and if a DNS request is
//      currently is underway. If this method returns false, it means that it
//      has to wait that every persistent connection is terminated and the DNS
//      is resolved before completing the termination.
//
//==============================================================================
inline bool CSipPersistentConnectionManagement::CanTerminate() const
{
    return m_vecpstPersistentConnection.GetSize() == 0 &&
           IsInBitSet(static_cast<int>(eTERMINATE_DONE), m_nBitSetGlobalState) == true &&
           IsInBitSet(static_cast<int>(eRESOLVING), m_nBitSetGlobalState) == false;
}

//==============================================================================
//==
//==  SetInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to set in rnBitSet.
//
//      rnBitSet:
//       The value in which we set nBitMask.
//
//  Description:
//      Sets the specified bitmask in the bitset.
//
//==============================================================================
inline void CSipPersistentConnectionManagement::SetInBitSet(IN int nBitMask,
                                                            INOUT int& rnBitSet) const
{
    rnBitSet = rnBitSet | nBitMask;
}

//==============================================================================
//==
//==  ResetInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to reset in rnBitSet.
//
//      rnBitSet:
//       The value in which we set nBitMask.
//
//  Description:
//      Resets the specified bitmask in the bitset.
//
//==============================================================================
inline void CSipPersistentConnectionManagement::ResetInBitSet(IN int nBitMask,
                                                              INOUT int& rnBitSet) const
{
    rnBitSet = rnBitSet & (~nBitMask);
}

//==============================================================================
//==
//==  IsInBitSet
//==
//==============================================================================
//
//  Parameters:
//      nBitMask:
//       The enum value to search nBitSet.
//
//      nBitSet:
//       The value in which we look if nBitMask is present.
//
//  Returns:
//      true:
//       nBitMask is present in nBitSet.
//
//      false:
//       nBitMask is not present in nBitSet.
//
//  Description:
//      Verifies if a bitmask value is present in a specified bitset.
//
//==============================================================================
inline bool CSipPersistentConnectionManagement::IsInBitSet(IN int nBitMask,
                                                           IN int nBitSet) const
{
    return (nBitSet & nBitMask) == nBitMask;
}

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSIPPERSISTENTCONNECTIONMANAGEMENT_H

