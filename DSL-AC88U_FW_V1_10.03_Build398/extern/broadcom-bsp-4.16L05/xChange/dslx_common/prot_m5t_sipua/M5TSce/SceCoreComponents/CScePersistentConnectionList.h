//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCEPERSISTENTCONNECTIONLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEPERSISTENTCONNECTIONLIST_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
#ifndef MXG_CHOSTPORT_H
#include "SipParser/CHostPort.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CVLIST_H
#include "Cap/CVList.h"
#endif

#ifndef MXG_CUNCMP_H
#include "Cap/CUncmp.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEPERSISTENTCONNECTIONLIST_H
#include "SceCoreComponents/IScePersistentConnectionList.h"
#endif

#ifndef MXG_ISIPPERSISTENTCONNECTIONMGR_H
#include "SipCoreSvc/ISipPersistentConnectionMgr.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSipPersistentConnectionList;
class CSipUri;
class CSocketAddr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CScePersistentConnectionList
//==============================================================================
//
// Description:
//   Aggregate implementation of the IScePersistentConnectionList
//   interface.
//
// Location:
//   SceCoreComponents/CScePersistentConnectionList.h
//
// See Also:
//   IScePersistentConnectionList
//
//==============================================================================
class CScePersistentConnectionList : protected CEComDelegatingUnknown,
                                     protected IScePersistentConnectionList,
                                     protected ISipPersistentConnectionMgr
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CScePersistentConnectionList(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CScePersistentConnectionList();

    // << ECom Pattern >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    // Inherited from CEComDelegatingUnknown.
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Inherited from IScePersistentConnectionList.
    virtual mxt_result EnablePersistentConnection(IN ISceUserConfig* pUserConfig);

    virtual mxt_result DisablePersistentConnection(IN ISceUserConfig* pUserConfig);

    virtual mxt_result RegisterObserver(IN ISceUserConfig* pUserConfig,
                                        IN IScePersistentConnectionObserver* pObserver);

    virtual mxt_result UnregisterObserver(IN ISceUserConfig* pUserConfig,
                                          IN IScePersistentConnectionObserver* pObserver);

    // Inherited from ISipPersistentConnectionMgr.
    virtual void EvConnectionEstablished(IN mxt_opaque opqConnection,
                                         IN const CSocketAddr& rLocalAddr,
                                         IN const CSocketAddr& rPeerAddr);

    virtual void EvConnectionTerminated(IN mxt_opaque opqConnection);

    virtual void EvErrorOnConnection(IN mxt_opaque opqConnection,
                                     IN mxt_result resConnectionError);

    virtual void EvSipErrorOnConnection(IN mxt_opaque opqConnectionHandle, 
                                        IN const CSipPacket& rSentPacket,
                                        IN mxt_result resConnectionError);

private:
    //-- << Initialization mechanism >>

    static mxt_result InitializeCScePersistentConnectionList();

    static void FinalizeCScePersistentConnectionList();

    //-- << Helper methods >>

    // Summary:
    //  Binds user config and persistent connection.
    mxt_result ConnectHelper(IN ISceUserConfig& rUserConfig,
                             IN const CSipUri& rRouteUri);

    // Summary:
    //  Finds the connection bound to the user in parameter.
    unsigned int FindConnection(IN ISceUserConfig& rUserConfig,
                                OUT unsigned int& ruUserIdx);

    // Summary:
    //  Finds the connection corresponding to the opaque data in parameter.
    unsigned int FindConnection(IN mxt_opaque opqConnection);

    // Summary:
    //  Finds the connection connected to the host port in parameter.
    unsigned int FindConnection(IN const CHostPort& rPeerHostPort);

    // Summary:
    //  Finds the observers registered to the user config in parameter.
    unsigned int FindObservers(IN ISceUserConfig& rUserConfig);

    // Events that can be reported on the observer.
    enum EEvent
    {
        eESTABLISHED,
        eERROR,
        eTERMINATED
    };

    // Summary:
    //  Upddates the persistent connection local address and report an event on
    //  the connection for all users.
    void WarnObservers(IN mxt_opaque opqConnection,
                       IN EEvent eEvent,
                       IN const CSocketAddr* pNewLocalAddress);

    // Summary:
    //  Report an event on the connection for a user.
    void WarnObservers(IN ISceUserConfig& rUserConfig,
                       IN EEvent eEvent);

    // Summary:
    //  Set the persistent connection local address on the
    //  ISceUserSecurityConfig.
    void SetPersistentConnectionLocalAddress(IN ISceUserConfig& rUserConfig,
                                             IN const CSocketAddr* pSocketAddr);
    // Summary:
    //  Obtain the CSipPersistentConnectionList from the ISipCoreConfig.
    CSipPersistentConnectionList* GetSipPersistentConnectionList();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy constructor.
    CScePersistentConnectionList(IN const CScePersistentConnectionList& rSrc);

    // Summary:
    //  Assignment Operator.
    CScePersistentConnectionList& operator=(IN const CScePersistentConnectionList& rSrc);

// Friends declarations

//-- Hidden Data Members
protected:
private:

    // This structure represents an actual persistent connection along with the
    // user configs that are using it.
    struct SConnection
    {
        // The opaque identifying the connection on the
        // CSipPersistentConnectionList.
        mxt_opaque m_opqConnection;

        // The host and port to which the connection is connected.
        CHostPort m_peerHostPort;

        // Whether the connection is currently established or not. It is used
        // to report either EvConnectionEstablished or EvConnectionInError when
        // an observer is registered.
        bool m_bIsEstablished;

        // The user configurations using that connection.
        CList<ISceUserConfig*> m_lstpUserConfigs;

        // Summary:
        //  Comparison method to find the connection bound to a certain user
        //  config in a list.
        static int ContainsUser(IN const CUncmp<SConnection>& rstConnection,
                                IN const CUncmp<SConnection>& rstOtherConnection,
                                IN mxt_opaque opqUserIdx);

        // Summary:
        //  Verifies that the m_opqConnection data member of both connections
        //  are equal.
        static int IsEqualOpqConnection(IN const CUncmp<SConnection>& rstConnection,
                                        IN const CUncmp<SConnection>& rstOtherConnection,
                                        IN mxt_opaque opqUnused);

        // Summary:
        //  Verifies that the m_peerHostPort data member of both connections are
        //  equal.
        static int IsEqualPeerHostPort(IN const CUncmp<SConnection>& rstConnection,
                                       IN const CUncmp<SConnection>& rstOtherConnection,
                                       IN mxt_opaque opqUnused);
    };

    // The list of connections.
    static CVList< CUncmp<SConnection> >* ms_pvlststConnections;

    // This structure associates user configs with their observers.
    struct SObservers
    {
        // The observed user config.
        ISceUserConfig* m_pUserConfig;

        // The objects observing that user config.
        CList<IScePersistentConnectionObserver*> m_lstpObservers;

        // Summary:
        //  Verifies that the m_pUserConfig data member of both object are
        //  equal.
        static int IsEqualUserConfig(IN const CUncmp<SObservers>& rstObservers,
                                     IN const CUncmp<SObservers>& rstOtherObservers,
                                     IN mxt_opaque opqUnused);
    };

    static CVList< CUncmp<SObservers> >* ms_pvlststObservers;
};

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSCEPERSISTENTCONNECTIONLIST_H


