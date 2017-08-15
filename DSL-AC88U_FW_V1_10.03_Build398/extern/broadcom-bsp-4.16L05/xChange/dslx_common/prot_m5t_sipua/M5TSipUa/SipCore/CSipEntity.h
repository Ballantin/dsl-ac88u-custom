//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPENTITY_H
#define MXG_CSIPENTITY_H

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


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_ISIPTRANSPORTOBSERVER_H
#include "SipTransport/ISipTransportObserver.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CMarshaler;
class ISipCoreUser;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipEntity
//========================================
//
// Summary:
//  SipCore entry point when receiving SIP packets.
//
// Description:
//  This class implements the ISipTransportObserver interface. Through this
//  interface, this class is notified of incoming packets. If the packets have
//  not been handled by the SipTransaction package (or a previous observer), 
//  then it synchronizes the incoming packet with the core thread and, once
//  synchronized, dispatches the packet to the stack user through the
//  ISipCoreUser interface.
//
// Location:
//   SipCore/CSipEntity.h
//
// See Also:
//   ISipTransportObserver, CSipDialogMatcher
//
//==EDOC========================================================================
class CSipEntity : public ISipTransportObserver,
                   public CEventDriven
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //-----------------------
    CSipEntity();

    // Summary:
    //  Sets the interface through which SipCore specific events are reported.
    //-------------------------------------------------------------------------
    static void SetCoreUser(IN ISipCoreUser* pCoreUser);

    // Summary:
    //  Gets the interface through which SipCore specific events are reported.
    //-------------------------------------------------------------------------
    static ISipCoreUser* GetCoreUser();

    // Summary:
    //  Prevents the CSipEntity from processing packets.
    //---------------------------------------------------
    void Shutdown();


    //-- << ISipTransportObserver Interface >>
    //--------------------------------------------

    virtual mxt_result EvPacketReceived(IN const CSipPacket& rPacket,
                                        IN bool bHandled);

    virtual void EvTransportError(IN mxt_result res,
                                  IN const CSocketAddr& rSource,
                                  IN const CSocketAddr& rDestination,
                                  IN ESipTransport eTransport);

    virtual void EvConnectionClosed(IN const CSocketAddr& rSource,
                                    IN const CSocketAddr& rDestination,
                                    IN ESipTransport eTransport,
                                    IN EClosureType  eClosureType);

    virtual void EvConnectionEstablished(IN const CSocketAddr& rSource,
                                         IN const CSocketAddr& rDestination,
                                         IN ESipTransport eTransport);

    virtual void EvObserverRemoved();


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

   // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipEntity();


    //-- << Messaging methods >>
    //--------------------------------------------

    void InternalEvPacketReceived(IN const CSipPacket& rPacket);


    //-- << CEventDriven interface >>
    //--------------------------------------------

    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);


private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipEntity();


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipEntity(IN const CSipEntity& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipEntity& operator=(IN const CSipEntity& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    static ISipCoreUser* ms_pCoreUser;

    // Tells if this object is shutting down.
    //----------------------------------------
    bool m_bIsShuttingDown;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline void CSipEntity::SetCoreUser(IN ISipCoreUser* pCoreUser)
{
    ms_pCoreUser = pCoreUser;
}

inline ISipCoreUser* CSipEntity::GetCoreUser()
{
    return ms_pCoreUser;
}

inline void CSipEntity::Shutdown()
{
    // Must be called once.
    //----------------------
    MX_ASSERT( !m_bIsShuttingDown );

    m_bIsShuttingDown = true;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPENTITY_H

