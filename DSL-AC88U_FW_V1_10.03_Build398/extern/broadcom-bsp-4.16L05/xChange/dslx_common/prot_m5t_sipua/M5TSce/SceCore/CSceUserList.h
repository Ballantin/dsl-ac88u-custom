//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCEUSERLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEUSERLIST_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSIPDIALOGMATCHERLIST_H
#include "SipCore/CSipDialogMatcherList.h"
#endif

#ifndef MXG_ISIPCOREUSER_H
#include "SipCore/ISipCoreUser.h"
#endif

#ifndef MXG_ISIPGENERICMGR_H
#include "SipUserAgent/ISipGenericMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CSceRequestDispatcher;
class ISceUser;
class ISceShutdownMgr;
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceUserList
//========================================
//
// Description:
//  The SCE User List has some important responsibilities:
//
//    - It is the entry point from the stack. It is an ISipCoreUser and
//      receives all SIP packets that do not matches an existing transaction.
//  
//    - Being the entry point, it uses the dialog matcher list to try to
//      associate incoming SIP packets to an existing dialog. When a packet
//      matches an existing dialog, nothing more is done other than providing
//      the packet to the dialog.
//  
//    - Packets that do not match an existing dialog are presented to a list
//      of ISceUser. Each core user can examine the request to find out
//      if it is destined to the user it represents. When a user is found, the
//      associated ISceUser returns an ISceUserConfig object to be
//      associated with the incoming packet.
//
//    - Whether or not a user is found for this packet, the packet is then
//      passed over to the CSceRequestDisptacher.
//      
//      
//  This class is meant to be an internal class to the SIP Client Engine. This
//  means its interface should not be used by external applications.
//      
// Location:
//   SceCore/CSceUserList.h
//
// See Also:
//   CSceRequestDisptacher
//
//==============================================================================
class CSceUserList : private ISipCoreUser,
                     private ISipGenericMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceUserList();

    // Summary:
    //  Destructor.
    virtual ~CSceUserList();

    // Summary:
    //  Unregisters a user in the user list.
    mxt_result RegisterUser(IN ISceUser* pUser);

    // Summary:
    //  Unregisters a user from the user list.
    mxt_result UnregisterUser(IN ISceUser* pUser);

    // Summary:
    //  Configures the single request dispatcher to use.
    void SetRequestDispatcher(IN CSceRequestDispatcher* pDispatcher);

    // Summary:
    //  Start the Sip Stack.
    mxt_result Startup();

    // Summary:
    //  Shutdown the Sip Stack.
    mxt_result ShutdownA(IN ISceShutdownMgr* pMgr, 
                         IN mxt_opaque opqShutdown);
 
//-- Hidden Methods.
protected:
private:
    //-- << ISipGenericMgr Implementation >>
    virtual void EvRequest(IN ISipGenericSvc*         pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket&       rRequest);

    virtual void EvProgress(IN ISipGenericSvc*         pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket&       rResponse);

    virtual void EvSuccess(IN ISipGenericSvc*         pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket&       rResponse);

    virtual void EvFailure(IN ISipGenericSvc*         pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket&       rResponse);

    //-- << ISipCoreUser >>
    virtual void EvOnPacketReceived(IN const CSipPacket& rPacket);
    virtual void EvShutdownCompleted();
    virtual void EvCommandResult(IN mxt_result res, IN mxt_opaque opq) 
        { 
            MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(res);
            MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(opq);
        };


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceUserList(IN const CSceUserList& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceUserList& operator=(IN const CSceUserList& rSrc);

//-- Hidden Data Members.
protected:
private:
    CVector<ISceUser*>       m_vecCoreUsers;
    CSipDialogMatcherList    m_lstDialogMatcher;
    CSceRequestDispatcher*   m_pDispatcher;
    bool                     m_bSipStackActive;
    ISceShutdownMgr*         m_pShutdownMgr;
    mxt_opaque               m_opqShutdown;
    ISipContext*             m_pContext;
    uint16_t                 m_uResponse;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SetRequestDispatcher
//==
//==============================================================================
//
//  Parameters:
//    pDispatcher:
//      Pointer to the request dispatcher.
//
//  Description:
//      Configures the request dispatcher.
//
//==============================================================================
inline void CSceUserList::SetRequestDispatcher(IN CSceRequestDispatcher* pDispatcher)
{
    m_pDispatcher = pDispatcher;
}


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CSCEUSERLIST_H
