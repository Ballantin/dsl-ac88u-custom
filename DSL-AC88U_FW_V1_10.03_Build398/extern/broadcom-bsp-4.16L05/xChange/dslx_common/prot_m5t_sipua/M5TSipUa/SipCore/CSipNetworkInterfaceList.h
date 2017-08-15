//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPNETWORKINTERFACELIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPNETWORKINTERFACELIST_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_ISIPCORECONFIG_H
#include "SipCore/ISipCoreConfig.h"
#endif

#ifndef MXG_ISOCKET_H
#include "Network/ISocket.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_ISIPTRANSPORTUSER_H
#include "SipTransport/ISipTransportUser.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHosrPort;
class ISipCoreUser;
class ISipTransportMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipNetworkInterfaceList
//========================================
//
// Description:
//   This class is used by the SIP stack to manage the network interfaces (local
//   addresses) that are available on the system, along with their associated
//   FQDNs. The network interfaces are stored in a CVector that holds the
//   following SNetworkInterface data:
//
//      - IP address of the network interface (local address).
//      - FQDN list.
//      - List of accessible networks. It stores which remote addresses are
//        reachable through that network interface. This list is commonly named
//        a "routing table".
//      - Listening information. It holds every port number to which the network
//        interface is listening.
//
//   CSipNetworkInterfaceList provides the following functionalities:
//
//      1-Local address management:
//          All local addresses or network interfaces that are available by the
//          application should be added by the AddLocalAddress method. When
//          that local address becomes unavailable, it can be removed by using
//          RemoveLocalAddress. Those methods can be accessed by the application
//          through the ISipCoreConfig interface.
//
//      2-Port listening management:
//          Port listening is managed internally; this is done through ListenA
//          and StopListeningA. Those methods can also be called by the
//          application through the ISipCoreConfig interface.
//
//      3-Routing table:
//          Network interfaces can be configured with a list of accessible
//          networks. That list is used by the GetLocalAddress when the SIP
//          stack wants to know which network interface to use to send a
//          packet to a specific destination address.
//
// Location:
//   SipCore/CSipNetworkInterfaceList.h
//
// See Also:
//   ISipCoreConfig, ISipCoreConfig::SAccessibleNetwork
//
//==EDOC========================================================================
class CSipNetworkInterfaceList : public ISipTransportUser,
                                 public CEventDriven
{
//-- Forward Declarations
//-------------------------

//-- Friend Declarations
//------------------------
//M5T_INTERNAL_USE_BEGIN
friend class CSipPersistentConnectionList;
//M5T_INTERNAL_USE_END

//-- New types within class namespace
//-----------------------------------
//M5T_INTERNAL_USE_BEGIN
public:
    struct SListeningInfo
    {
        mxt_opaque    m_opqAddress;
        uint16_t      m_uPort;
        ESipTransport m_eTransport;
        unsigned int  m_uEntityId;
        // Tells if we are currently processing an action for this listening
        // info.
        bool          m_bActionProcessing;

        // Summary:
        //  Constructor.
        //---------------
        SListeningInfo(IN mxt_opaque    opqAddress,
                       IN uint16_t      uPort,
                       IN ESipTransport eTransport)
        :   m_opqAddress(opqAddress),
            m_uPort(uPort),
            m_eTransport(eTransport),
            m_uEntityId(0),
            m_bActionProcessing(false)
        {
        };
    };

    struct SNetworkInterface
    {
        // IP address of the network interface.
        //--------------------------------------
        CSocketAddr                                        m_socketAddr;

        // FQDN list.
        //------------
        const CVector<CString>*                            m_pvecStrFqdn;

        // Accessible networks.
        //----------------------
        const CVector<ISipCoreConfig::SAccessibleNetwork>* m_pvecDestinations;

        // Listening infos.
        //------------------
        CVector<SListeningInfo*>                           m_vecpstListeningInfo;

        // This flag is used to delay the deletion of this structure on
        // RemoveLocalAddress when the m_vecpstListeningInfo is not empty.
        //------------------------------------------------------------------
        bool m_bRemoveRequested;

        // This is the address that will be reported instead of the real interface
        // address. When NULL (default), the real interface address is reported.
        CHostPort* m_pVisibleLocalAddress;

        // Summary:
        //  Constructor.
        //---------------
        SNetworkInterface(
            IN    const CSocketAddr                                  rSocketAddr,
            IN TO const CVector<CString>*                            pvecStrFqdn,
            IN TO const CVector<ISipCoreConfig::SAccessibleNetwork>* pvecDestinations)
            : m_socketAddr(rSocketAddr),
              m_pvecStrFqdn(TO pvecStrFqdn),
              m_pvecDestinations(TO pvecDestinations),
              m_bRemoveRequested(false),
              m_pVisibleLocalAddress(NULL)
        {
            // We only need to store the address part of the CSocketAddr, the
            // port number is not needed.
            //----------------------------------------------------------------
            m_socketAddr.SetPort(0);
        };

        // Summary:
        //  Destructor.
        //--------------
        ~SNetworkInterface()
        {
            // Makes sure there is no remaining listening socket.
            //----------------------------------------------------
            MX_ASSERT(m_vecpstListeningInfo.GetSize() == 0);

            MX_DELETE(m_pvecStrFqdn);
            m_pvecStrFqdn = NULL;
            MX_DELETE(m_pvecDestinations);
            m_pvecDestinations = NULL;

            MX_DELETE(m_pVisibleLocalAddress);
            m_pVisibleLocalAddress = NULL;
        };
    };
//M5T_INTERNAL_USE_END
protected:
private:

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipNetworkInterfaceList(IN ISipTransportMgr* pTransportMgr);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipNetworkInterfaceList();

    //-- << Stereotype >>
    //--------------------------------------------

    // Summary:
    //  Adds a local address.
    //------------------------
    mxt_result AddLocalAddress(
        IN    const CSocketAddr&                                 rAddr,
        IN TO const CVector<CString>*                            pvecStrFqdn,
        IN TO const CVector<ISipCoreConfig::SAccessibleNetwork>* pvecDestinations,
        OUT   mxt_opaque&                                        ropqAddress);

    // Summary:
    //  Removes a local address.
    //---------------------------
    mxt_result RemoveLocalAddress(IN const mxt_opaque opqAddress);

    // Summary:
    //  Returns the local address that should be used for a destination address.
    //---------------------------------------------------------------------------
    mxt_result GetLocalAddress(IN const CSocketAddr& rDestinationAddr,
                               IN ESipTransport eTransport,
                               IN unsigned int uEntityId,
                               OUT CSocketAddr& rLocalAddress,
                               INOUT CHostPort* pLocalHost = NULL,
                               OUT bool* pbForced = NULL) const;

    // Summary:
    //  Returns the local interface that should be used for a destination address.
    //---------------------------------------------------------------------------
    void GetLocalInterface(IN const CSocketAddr& rDestinationAddr,
                           OUT CString& rstrInterface) const;

    // Summary:
    //  Returns the number of local addresses configured.
    //----------------------------------------------------
    unsigned int GetLocalAddressCount();

    // Summary:
    //  Finds, in the routing table, the FQDN vector corresponding to the local
    //  address.
    //--------------------------------------------------------------------------
    const CVector<CString>* GetFqdnVector(IN const CSocketAddr& rLocalAddress) const;

    // Summary:
    //  Listens for incoming SIP packets on an IP address.
    //-----------------------------------------------------
    mxt_result ListenA(IN  mxt_opaque    opqAddress,
                       IN  uint16_t      uPort,
                       IN  ESipTransport eTransport,
                       IN  ISipCoreUser* pCoreUser,
                       IN  mxt_opaque    opqCallBackParam,
                       OUT mxt_opaque&   ropqListen);

    // Summary:
    //  Associates an entity ID to a listening port.
    //-----------------------------------------------------
    mxt_result SetEntityId(IN mxt_opaque opqListen,
                           IN unsigned int uEntityId);

    // Summary:
    //  Stops listening for incoming SIP packets on an IP address.
    //-------------------------------------------------------------
    mxt_result StopListeningA(IN mxt_opaque    opqListen,
                              IN ISipCoreUser* pCoreUser,
                              IN mxt_opaque    opqCallBackParam);

    // Summary:
    //  Gets the network interfaces address list.
    //--------------------------------------------
    void GetNetworkInterfaceList(OUT CVector<ISipCoreConfig::SNetworkIf>& rvecstNetworkIf) const;

//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Gets the internal network interfaces address list.
    //-----------------------------------------------------
    void GetInternalNetworkInterfaceList(OUT const CVector<SNetworkInterface*>*& rpvecstNetworkIf) const;
//M5T_INTERNAL_USE_END

    // Summary:
    //  Clears and stops listening on local addresses.
    //-------------------------------------------------
    void ShutdownA(IN ISipTransportUser* pNetworkInterfaceListMgr,
                   IN mxt_opaque         opq);

    // Summary:
    //  Find the first listening port on rLocalAddress with transport
    //  eTransport.
    //----------------------------------------------------------------
    uint16_t FindFirstListeningPort(IN const CSocketAddr& rLocalAddress,
                                    IN ESipTransport      eTransport);

    // Summary:
    //  Sets the closing behavior of client sockets.
    void SetSocketClosureType(IN ISocket::ECloseBehavior eCloseBehavior);

    // Summary:
    //  Returns the current socket closure behaviour for the client sockets.
    ISocket::ECloseBehavior GetSocketClosureType() const;

    //  Summary:
    //      Allows the application to override the hostport part of the Via and
    //      Record-Route headers in sent packets.
    mxt_result ForceVisibleLocalAddress(IN mxt_opaque opqAddress,
                                        IN TOA CHostPort* pVisibleAddress);

//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipNetworkInterfaceList(IN const CSipNetworkInterfaceList& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipNetworkInterfaceList& operator=(IN const CSipNetworkInterfaceList& rSrc);

    // Summary:
    //  Notifies the transport user if shutdown is completed.
    //-------------------------------------------------------
    void VerifyAndNotifyShutdown();

    //-- << ISipTransportUser >>.
    //----------------------------
    virtual void EvCommandResult(IN mxt_result res, IN mxt_opaque opq);

    //-- << CEventDriven >>.
    //-----------------------

    // Summary:
    //  Notifies the manager that a new message must be processed.
    //-------------------------------------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool         bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler*  pParameter);

    //-- << Helper method >>.
    //-------------------------

    // Summary:
    //  Finds the network interface from the m_vecpstNetworkInterfaces.
    //------------------------------------------------------------------
    SNetworkInterface* FindNetworkInterface(IN mxt_opaque opqAddress) const;

    // Summary:
    //  Finds the network address from the m_vecpstNetworkInterfaces.
    //------------------------------------------------------------------
    mxt_result FindLocalAddress(IN const CSocketAddr& rDestinationAddr,
                                OUT SNetworkInterface** ppstNI,
                                IN ESipTransport eTransport = eINVALID,
                                IN unsigned int uEntityId = 0,
                                OUT SListeningInfo** ppstLI = NULL) const;


    // Summary:
    //  Finds the network interface from the m_vecpstNetworkInterfaces.
    //------------------------------------------------------------------
    SListeningInfo* FindListeningInfo(IN const SNetworkInterface* pstNI,
                                      IN ESipTransport eTransport,
                                      IN unsigned int uEntityId) const;

    // Summary:
    //  Closes the connected client sockets that are bound to this local address.
    void CloseSockets(IN const CSocketAddr& rLocalInterfaceAddress);

    //-- << Internal method >>.
    //---------------------------

    void InternalEvCommandResultA(IN CMarshaler* pParams);

//-- Hidden Data Members
//------------------------
protected:
private:

    // Pending event type.
    //---------------------
    enum ETransportAction
    {
        eLISTENA,
        eSTOP_LISTENA,
    };

    // Pending transport user event data (ListenA or StopListeningA).
    //----------------------------------------------------------------
    struct STransportUserPendingEvent
    {
        SListeningInfo*  m_pstListeningInfo;
        ISipCoreUser*    m_pCoreUser;
        mxt_opaque       m_opqCallBackParam;
        ETransportAction m_eTransportAction;

        // Summary:
        //  Constructor.
        //---------------
        STransportUserPendingEvent(
            IN SListeningInfo*  pstListeningInfo,
            IN ISipCoreUser*    pCoreUser,
            IN mxt_opaque       opqCallBackParam,
            IN ETransportAction eTransportAction)
                : m_pstListeningInfo(pstListeningInfo),
                  m_pCoreUser(pCoreUser),
                  m_opqCallBackParam(opqCallBackParam),
                  m_eTransportAction(eTransportAction)
        {
        };
    };

    // Asynchronous events IDs.
    //--------------------------
    enum EMsgNumber
    {
        eEV_COMMAND_RESULT_A,
    };

    // Network interfaces.
    //---------------------
    CVector<SNetworkInterface*> m_vecpstNetworkInterfaces;

    // Transport user pending events.
    //--------------------------------
    CVector<STransportUserPendingEvent*> m_vecpstPendingEvent;

    // Transport manager.
    //--------------------
    ISipTransportMgr* m_pTransportMgr;

    // Closing behavior.
    ISocket::ECloseBehavior m_eCloseBehavior;

    // Transport user.
    //----------------
    ISipTransportUser* m_pTransportUser;

    bool m_bShutdownInProgress;
    mxt_opaque m_opqShutdown;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC====================================================================
//==
//==  GetLocalAddressCount
//==
//==========================================================================
//
//  Returns:
//    Number of local addresses.
//
//  Description:
//      Returns the total number of local addresses present in the network
//      interface.
//
//  See Also:
//      AddLocalAddress
//
//==EDOC====================================================================
inline unsigned int CSipNetworkInterfaceList::GetLocalAddressCount()
{
    return m_vecpstNetworkInterfaces.GetSize();
}

//==============================================================================
//==
//==  SetSocketClosureType
//==
//==============================================================================
//
//  Parameters:
//      eCloseBehavior:
//       The closing behavior to use.
//
//  Description:
//      This sets the closing behavior to use to close connected client sockets
//      while removing a network interface. This closing behavior is also used
//      if ShutdownA is called and some network interfaces are still present.
//
//==============================================================================
inline void CSipNetworkInterfaceList::SetSocketClosureType(IN ISocket::ECloseBehavior eCloseBehavior)
{
    m_eCloseBehavior = eCloseBehavior;
}

//==============================================================================
//==
//==  GetSocketClosureType
//==
//==============================================================================
//
//  Return:
//       The current closing behavior.
//
//  Description:
//      This fetches the current closing behavior that will be used when closing
//      client sockets.
//
//==============================================================================
inline ISocket::ECloseBehavior CSipNetworkInterfaceList::GetSocketClosureType() const
{
    return m_eCloseBehavior;
}

//M5T_INTERNAL_USE_BEGIN
//==============================================================================
//==
//==  GetInternalNetworkInterfaceList
//==
//==============================================================================
//
//  Parameters:
//    rpvecstNetworkIf:
//      Internal local network interfaces addresses list.
//
//  Description:
//      Returns the internal list of local network interface addresses. This
//      list contains all the information about the added network interface
//      addresses.
//
//  #Warning#
//      This method should not be used by any application unless it knows what
//      it is doing. This is a direct access to the internal structure and any
//      changes made to it could alter the stack's stability.
//
//  See Also:
//      AddLocalAddress, ListenA
//
//==============================================================================
inline void CSipNetworkInterfaceList::GetInternalNetworkInterfaceList(
                OUT const CVector<SNetworkInterface*>*& rpvecstNetworkIf) const
{
    rpvecstNetworkIf = &m_vecpstNetworkInterfaces;
}
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPNETWORKINTERFACELIST_H

