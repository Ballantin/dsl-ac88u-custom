//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPREDIRECTIONSVC_H
#define MXG_CSIPREDIRECTIONSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT to be \
able to use this class.
#endif


//-- Data Members
//-----------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPREDIRECTIONSVC_H
#include "SipUserAgent/ISipRedirectionSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class IUri;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
const mxt_opaque opqCSIPREDIRECTIONSVC_EVENT = MX_INT32_TO_OPQ(0x1111);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipRedirectionSvc
//========================================
//
// Description:
//   This class is used to manage contacts to which a user may have been
//   redirected. When it receives a 3xx response, it remembers the other
//   contacts to try. It warns its manager every time it receives a failure
//   response other than 6xx and if there is still other contacts to try to
//   contact the other side. The redirection service does not give twice the
//   same contact to try.
//
//   The remembered contacts are only cleared upon reception of a 6xx response
//   (and when the destructor is called).
//
// Updated Behaviour:
//
//  The behaviour of the redirection service has changed from MxSF v3.x.
//
//  - In v4.0, the contacts are removed from the list when a request is sent
//    using this contact as Request-URI.
//
//  - In v4.0, when a contact is received twice in a 3xx response, the maximum
//    qvalue between the current and the received (or 1.0 if there is no qvalue
//    associated with the received contact) is taken as the new qvalue.
//
//  - The responses received from requests made inside of a dialog are not
//    monitored by the service.
//
//  - Even if a 2xx response is received, the contacts that were not tried are
//  no longer cleared so this service sends an EvRedirected as soon as it
//  receives a negative response to a request made outside of a dialog, if there
//  are still contacts to try.
//
// Location:
//   SipUserAgent/CSipRedirectionSvc.h
//
// See Also:
//   ISipRedirectionSvc, ISipRedirectionMgr
//
//==EDOC========================================================================
class CSipRedirectionSvc : private CEComDelegatingUnknown,
                           private ISipRedirectionSvc,
                           private CSipReqCtxCoreSvc,
                           private ISipCoreSvc
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------
private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipRedirectionSvc();

    static void FinalizeCSipRedirectionSvc();


    //-- << ECOM mechanism >>
    //-------------------------

    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);


    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipRedirectionSvc( IN IEComUnknown& rOuterIEComUnknown );

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipRedirectionSvc();


    //-- << Helpers >>
    //-------------------------------------------------

    // Summary:
    //  Tells if there is at least one contact that has never been tried.
    //--------------------------------------------------------------------
    bool AreThereContactsToTry();

    // Summary:
    //  Searches the URI in the redirected contacts and marks it as tried.
    //--------------------------------------------------------------------
    bool RequestUriTried( IUri& rTriedUri );

    // Summary:
    //  Adds the new contacts from the packet in the redirected contacts.
    //--------------------------------------------------------------------
    bool UpdateContacts( IN const CSipPacket& rPacket );

    // Summary:
    //  Clears all the contacts remembered by the service.
    //-----------------------------------------------------
    void ClearContacts();

    // Summary:
    //  Searches for the Contact in the header list.
    //-----------------------------------------------
    int FindContact( IN CSipHeader& rContactToSearch,
                     IN CSipHeader* pFromHeaderList );

    // Summary:
    //  Searches for the URI in the header list.
    //-------------------------------------------
    int FindUri( IN IUri& rUriToSearch, IN CSipHeader* pFromHeaderList );

    // Summary:
    //  Removes the contact from the untried list.
    //---------------------------------------------
    GO CSipHeader* RemoveContactFromUntriedList( IN int nIndexToRemove );


    //-- << CEComUnknown >>
    //--------------------------------------------

    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);


    //-- << ISipRedirectionSvc >>
    //--------------------------------------------

    // Summary:
    //  Configures the manager of this service manager.
    //--------------------------------------------------
    virtual mxt_result SetManager( IN ISipRedirectionMgr* pMgr );

    // Summary:
    //  Makes the parent context use the Contact header with the highest 'q
    //  value' for the request-URI of future requests.
    //----------------------------------------------------------------------
    virtual mxt_result UseNextContact();

    // Summary:
    //  Makes the parent context use the URL at position uContactIndex in the
    //  order of preference for the request-URI of future requests.
    //------------------------------------------------------------------------
    virtual mxt_result UseContact( IN unsigned int uContactIndex );

    // Summary:
    //  Retrieves the list of possible contacts where the parent context's
    //  target could be reached.
    //---------------------------------------------------------------------
    virtual const CSipHeader* GetContacts();

    // Summary:
    //  Removes the specified contact from the list.
    //-----------------------------------------------
    virtual mxt_result RemoveContact( IN unsigned int uContactIndex );


    //-- << ISipReqCtxCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Informs the request context core service of a received packet.
    //-----------------------------------------------------------------
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    // Summary:
    //  Modify a packet about to be sent.
    //------------------------------------
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    // Summary:
    //  Let the service call an event on the application.
    //----------------------------------------------------
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    // Summary:
    //  Let the service clear the state associated with an event in which the
    //  application has no interest.
    //------------------------------------------------------------------
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    // Summary:
    //  Add a reference on this request context core service.
    //--------------------------------------------------------
    virtual unsigned int AddRcsRef();

    // Summary:
    //  Remove a reference on this request context core service.
    //-----------------------------------------------------------
    virtual unsigned int ReleaseRcsRef();


    //-- << ISipCoreSvc >>
    //--------------------------------------------

    // Summary:
    //  Obtain an instance of an ISipReqCtxCoreSvc to attach to a request
    //  context.
    //--------------------------------------------------------------------
    virtual
    ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                IN EDirection eDirection);

    // Summary:
    //  Gives to which degree the service is a good candidate to own a request
    //  context.
    //-------------------------------------------------------------------------
    virtual
    EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    // Summary:
    //  Return the ISipReqCtxCoreSvc instance to use as the owner for a request
    //  context.
    //--------------------------------------------------------------------------
    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    // Summary:
    //  Warns the service that the context is about to be destroyed.
    //---------------------------------------------------------------
    virtual mxt_result ClearCoreSvc();


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipRedirectionSvc(IN const CSipRedirectionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipRedirectionSvc& operator=(IN const CSipRedirectionSvc& rSrc);


    //-- << Helpers >>
    //-------------------------------------------------

    // Summary:
    //  Add the header to the end of the chain.
    //------------------------------------------
    void AppendToChain( IN TO CSipHeader* pContactToAppend,
                       INOUT CSipHeader*& rpFirstContact );

    // Summary:
    //  Unlinks the header at given index.
    //-------------------------------------
    GO CSipHeader* UnlinkContact( IN int nHeaderIndex,
                                  INOUT CSipHeader*& rpFirstContact );

    // Summary:
    //  Adds the new contacts to the redirected contacts.
    //----------------------------------------------------
    bool UpdateContacts( IN TO CSipHeader* pContactToAdd );

    // Summary:
    //  Adds or updates the contact in the untried list.
    //---------------------------------------------------
    void UpdateUntriedContact( IN TO CSipHeader* pContactToAdd,
                               IN uint16_t uQValue,
                               INOUT bool& rbAddedContact );


//-- Hidden Data Members
//------------------------
protected:
private:

    ISipRedirectionMgr* m_pMgr;

    CSipHeader* m_pContactsToTry;
    CVector<uint16_t> m_vecuQValues;

    CSipHeader* m_pTriedContacts;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//====  HELPER AND PRIVATE METHODS  ============================================
//==============================================================================

//==SDOC========================================================================
//==
//==  AreThereContactsToTry
//==
//==============================================================================
//
//  Returns:
//      true if there are still contacts to try in the vector.
//
//  Description:
//      This method tells if there are still known contacts to try.
//
//==EDOC========================================================================
inline bool CSipRedirectionSvc::AreThereContactsToTry()
{
    return ( m_pContactsToTry != NULL );
}

//==SDOC========================================================================
//==
//==  RemoveContactFromUntriedList
//==
//==============================================================================
//
//  Parameters:
//    nIndexToRemove:
//      The index to remove from the untried list. This is the index to use in
//      CSipHeader::GetNextHeader to find the contact header in
//      m_pContactsToTry.
//
//      -1 if the contact header to remove is m_pContactsToTry ( no need to call
//      CSipHeader::GetNextHeader to access it ). In this case, m_pContactsToTry
//      takes the value of the next pointed header in the chain (or NULL if
//      none).
//
//  Returns:
//      The removed header if nIndexToRemove was a valid index.
//
//      NULL if nIndexToRemove was invalid.
//
//  Description:
//      Removes the contact from the untried list and returns the removed
//      contact.
//
//==EDOC========================================================================
inline GO CSipHeader* CSipRedirectionSvc::RemoveContactFromUntriedList(
                                                        IN int nIndexToRemove )
{
    // Remove the qvalue associated to the contact.
    //----------------------------------------------
    m_vecuQValues.Erase( nIndexToRemove + 1 );

    // Unlink the contact and remember it to put it into the tried list.
    //-------------------------------------------------------------------
    return GO UnlinkContact( IN nIndexToRemove, INOUT m_pContactsToTry );
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPREDIRECTIONSVC_H

