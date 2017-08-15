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
#ifndef MXG_CSIPFORKEDDIALOGGROUPER_H
#define MXG_CSIPFORKEDDIALOGGROUPER_H

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

#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif

#ifndef MXG_STATUSCODE_H
#include "SipParser/StatusCode.h" // ESipStatusClass
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_ISIPFORKEDDIALOGGROUPER_H
#include "SipCore/ISipForkedDialogGrouper.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CToken;
class ISipForkedDialogGrouperMgr;
class IPrivateSipContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipForkedDialogGrouper
//========================================
//
// Description:
//   This class regroups contexts representing dialogs created from a unique
//   request that has been forked.
//
// Location:
//   SipCore/CSipForkedDialogGrouper.h
//
// See Also:
//   CSomethingElse, CSomething
//
//==EDOC========================================================================
class CSipForkedDialogGrouper : public ISipForkedDialogGrouper,
                                private CEventDriven
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
    CSipForkedDialogGrouper(IN ISipForkedDialogGrouperMgr& rMgr,
                            IN IPrivateSipContext& rOriginatorContext,
// BRCM_CUSTOM - [add] Control To-tag matching choices
                            IN bool bMatchToTagPerRfc3261,
// BRCM_CUSTOM - [end] Control To-tag matching choices
                            IN const CToken* pCreatingDialogRequestMethod = NULL);

    //-- << Public methods >>
    //--------------------------------------------

    // Summary:
    //  Verifies if this object was already activated for the given CSipPacket.
    bool IsActiveFor(IN const CSipPacket& rInitialRequest);

    // Summary:
    //  Initializes the packet to match and activate the event driven.
    //-----------------------------------------------------------------
    mxt_result Activate(IN const CSipPacket& rInitialRequest);

    // Summary:
    //  Prepares this object to be released.
    void Abort();

    // Summary:
    //  Dispatches the packet to the appropriate context.
    //----------------------------------------------------
    mxt_result OnPacketReceived(IN const CSipPacket& rPacket);

    // Summary:
    //  Verifies if the packet can be handled by that group.
    bool MatchPacket(IN const CSipPacket& rPacket);

    // Summary:
    //  Verifies if the response can be handled by that group.
    //---------------------------------------------------------
    bool MatchResponse(IN const CSipPacket& rResponse);

    // Summary:
    //  Informs the dialog grouper that the context has been cleared.
    void ContextCleared(IN IPrivateSipContext* pClearedContext);

    // Summary:
    //  Returns a pointer to the originator context for that group.
    //--------------------------------------------------------------
    void GetOriginatorContext(OUT IPrivateSipContext*& rpOriginator);

    // Summary:
    //  Returns the manager of this instance.
    ISipForkedDialogGrouperMgr* GetManager() const;

    // Summary:
    //  Returns the opaque value given with the new ISipContext.
    mxt_opaque GetNewTransactionOpaque() const;

    // Summary:
    //  Returns the method as set in the constructor.
    const CToken* GetCreatingDialogRequestMethod() const;

    // Summary:
    //  Returns the method searched for new dialogs.
    const CToken& GetMethod() const;


    //-- << ISipForkedDialogGrouper Interface >>
    //--------------------------------------------

    virtual mxt_result ContextCreated(IN ISipContext& rNewContext,
                                      IN const CSipPacket& rPacket,
                                      IN mxt_opaque opqTransaction);

    virtual unsigned int AddRef();

    virtual unsigned int ReleaseRef();


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Destructor. Private because this object is reference counted and is a
    //  CEventDriven.
    //------------------------------------------------------------------------
    virtual ~CSipForkedDialogGrouper();

private:

    //-- << CEventDriven Interface >>.
    //---------------------------------

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);


    //-- << Helper methods >>

    // Summary:
    //  Comparison function for the CMap. Tags are compared case insensitive.
    static int CaseInsCmp(IN const CString& rFirst,
                          IN const CString& rSecond,
                          IN mxt_opaque opq);

    bool MatchPacketHelper(IN const CSipPacket& rPacket);

    // Summary:
    //  Passes the failure response to the early dialogs and terminates this
    //  object.
    void TerminateGrouper(IN const CSipPacket* pFailureResponse);

    // Summary:
    //  Empties m_mapDialogs and resets the context's forked dialog grouper.
    void EmptyDialogMap();


    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipForkedDialogGrouper(IN const CSipForkedDialogGrouper& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipForkedDialogGrouper& operator=(IN const CSipForkedDialogGrouper& rSrc);


//-- Hidden Data Members
//------------------------
protected:
    // The ISipContext from which the request came out. This pointer is only
    // NULL when this object is terminated.
    IPrivateSipContext* m_pOriginatorContext;

private:

    ISipForkedDialogGrouperMgr& m_rMgr;

    // Used only when matching responses because it is used to create the
    // simulated failures responses for early branches that do not receive
    // success responses. NULL when matching requests.
    //---------------------------------------------------------------------
    const CSipPacket* m_pInitialRequest;

    CToken m_callId;

    CToken m_localTag;

    CToken m_method;

    // Used only when matching responses. Empty when a received request will
    // trigger the forking.
    CToken m_tokenSequenceNumber;

    // Stores the highest class handled by this grouper so far. This is useful
    // to know whether a final success or a final failure has been received.
    // This has meaning only when matching responses.
    //-------------------------------------------------------------------------
    ESipStatusClass m_eHighestClassHandled;

    // Keyed with the remote tag for the dialog. Note that the comparison
    // function is overriden with the CaseInsCmp method since tag comparison is
    // case insensitive.
    //--------------------------------------------------------------------------
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
    // GCC 2.7.2 does not support default parameters in templates,
    // in which case all the parameters must be specified.
    CMap<CString, IPrivateSipContext*,
        CAATree<CMapPair<CString, IPrivateSipContext*> > > m_mapDialogs;
#else
    CMap<CString, IPrivateSipContext*> m_mapDialogs;
#endif


    CVector<IPrivateSipContext*> m_vecpEarlyDialogs;

    // This is the opaque value that was given in ContextCreated to associate
    // with a created CSipRequestContext. Its validity is really short (around
    // the time IPrivateSipContext::HandlePacket is called in ContextCreated).
    mxt_opaque m_opqNewTransaction;

// BRCM_CUSTOM - [add] Control To-tag matching choices
    bool m_bMatchToTagPerRfc3261;
// BRCM_CUSTOM - [end] Control To-tag matching choices
    unsigned int m_uRefCount;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  PUBLIC METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetManager
//==
//==============================================================================
//
//  Returns:
//      The manager of this instance.
//
//  Description:
//      Returns the manager of this instance.
//
//==============================================================================
inline ISipForkedDialogGrouperMgr* CSipForkedDialogGrouper::GetManager() const
{
    return &m_rMgr;
}

//==============================================================================
//==
//==  GetNewTransactionOpaque
//==
//==============================================================================
//
//  Returns:
//      The opaque value given in ISipForkedDialogGrouper::ContextCreated.
//
//  Description:
//      Returns the opaque value given with the new ISipContext.
//
//      IMPORTANT: this method will return NULL if this method is not called
//      within the IPrivateSipContext::HandlePacket method called in
//      ContextCreated.
//
//==============================================================================
inline mxt_opaque CSipForkedDialogGrouper::GetNewTransactionOpaque() const
{
    return m_opqNewTransaction;
}

//==============================================================================
//==
//==  GetCreatingDialogRequestMethod
//==
//==============================================================================
//
//  Returns:
//      NULL if Activate was not called yet or if NULL was passed in the
//      constructor.
//
//      A pointer to the method of the request creating a dialog (eg. "NOTIFY"
//      for a sent SUBSCRIBE).
//
//  Description:
//      Returns the method as set in the constructor.
//
//==============================================================================
inline const CToken* CSipForkedDialogGrouper::GetCreatingDialogRequestMethod() const
{
    // Return NULL if we know that we are matching a request.
    return m_tokenSequenceNumber.IsEmpty() ? &m_method :
                                             NULL;
}

//==============================================================================
//==
//==  GetMethod
//==
//==============================================================================
//
//  Returns:
//      The method searched for new dialogs.
//
//  Description:
//      Returns the method searched for new dialogs.
//
//      May return an empty token when this method is called before Activate is
//      successfully called (in case of response creating new forked dialogs).
//
//==============================================================================
inline const CToken& CSipForkedDialogGrouper::GetMethod() const
{
    return m_method;
}

//==============================================================================
//====  PRIVATE METHODS  =======================================================
//==============================================================================

//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

inline int CSipForkedDialogGrouper::CaseInsCmp(IN const CString& rFirst,
                                               IN const CString& rSecond,
                                               IN mxt_opaque opq)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(opq);
    return rFirst.CaseInsCmp(rSecond);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPFORKEDDIALOGGROUPER_H

