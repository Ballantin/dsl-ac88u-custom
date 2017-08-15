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
#ifndef MXG_CSIPTRANSACTIONCOMPLETIONSVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPTRANSACTIONCOMPLETIONSVC_H
//M5T_INTERNAL_USE_END

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

#if !defined(MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_COMPLETION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_COMPLETION_SVC_SUPPORT to be able \
to use this class.
#endif

#ifndef MXG_ISIPTRANSACTIONCOMPLETIONSVC_H
#include "SipUserAgent/ISipTransactionCompletionSvc.h"
#endif

#ifndef MXG_ISIPCORESVC_H
#include "SipCore/ISipCoreSvc.h"
#endif

#ifndef MXG_ISIPCONNECTIONSVC_H
#include "SipCore/ISipConnectionSvc.h"
#endif

#ifndef MXG_CSIPREQCTXCORESVC_H
#include "SipCore/CSipReqCtxCoreSvc.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipReqCtxConnectionSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
const unsigned int uNOTIFY_MANAGER = 1;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipTransactionCompletionSvc
//==============================================================================
//
// Description:
//   This is the implementation of the ISipTransactionCompletionSvc interface.
//   It holds states about ongoing transactions and notifies its manager when
//   all transactions of one request type are completed.
//
// Location:
//   SipUserAgent/CSipTransactionCompletionSvc.h
//
// See Also:
//   ISipTransactionCompletionSvc
//
//==============================================================================
class CSipTransactionCompletionSvc : private ISipTransactionCompletionSvc,
                                     private ISipCoreSvc,
                                     private ISipConnectionSvc,
                                     private CSipReqCtxCoreSvc,
                                     private CEComDelegatingUnknown,
                                     public CEventDriven
{
//-- Friend Declarations
    friend class CSipStackInitializer;

//-- New types within class namespace
public:

    // This structure is used to maintain information about the various
    // transaction completion.
    //------------------------------------------------------------------
    struct STransCompletionInfo
    {
        // The count of currently on-going requests for the given SIP
        // method.
        //------------------------------------------------------------
        unsigned int m_uRequestCount;

        // True if parallel request are allowed.
        //---------------------------------------
        bool m_bAllowParallelRequest;

        // Summary:
        //  Initializes the default value of the struct.
        //----------------------------------------------
        STransCompletionInfo()
        :   m_uRequestCount(0),
            m_bAllowParallelRequest(true)
        {
        }
    };

//-- Published Interface

    // Summary:
    //  Returns a reference to the transaction completion info map.
    //--------------------------------------------------------------
    CMap<CString,
        STransCompletionInfo *,
         CAATree<CMapPair<CString,
            STransCompletionInfo *> > >& GetTransactionCompletionInfoMap();

    // Summary:
    //  Posts a message if the manager is not NULL.
    //----------------------------------------------
    void PostMessage(IN const CString& strMethod);

    //-- << CEComUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                    OUT void** ppInterface);

//-- Hidden Methods
protected:
private:

    //-- << Constructors / Destructors / Operators >>
    // Summary:
    //  Default Constructor.
    CSipTransactionCompletionSvc();

    // Summary:
    //  Default Constructor.
    CSipTransactionCompletionSvc(IN IEComUnknown& rOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSipTransactionCompletionSvc();

    //-- << ISipTransactionCompletionSvc >>
    virtual void EnableParallelTransactions(IN const CString& rstrMethod,
                                            IN bool bEnable);

    virtual void SetManager(IN ISipTransactionCompletionMgr* pMgr);

    virtual bool CanEstablishNewTransaction(IN const CString& strMethod);

    //-- << ISipReqCtxCoreSvc >>
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList);

    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket);

    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket);

    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent);

    virtual unsigned int AddRcsRef();

    virtual unsigned int ReleaseRcsRef();

    //-- << ISipCoreSvc >>
    virtual ISipReqCtxCoreSvc* GetReqCtxCoreSvcInstance(IN const CSipPacket& rPacket,
                                                        IN ISipCoreSvc::EDirection eDirection);

    virtual EOwnerConfidenceLevel GetOwnerConfidenceLevel(IN const CSipPacket& rPacket);

    virtual void GetOwnerInstance(IN const CSipPacket& rPacket,
                                  OUT ISipReqCtxCoreSvc*& rpSipReqCtxCoreSvc);

    virtual mxt_result ClearCoreSvc();

    //-- << ISipConnectionSvc >>
    virtual ISipReqCtxConnectionSvc* GetReqCtxConnectionSvcInstance(IN const CSipPacket& rPacket,
                                                                    IN ISipConnectionSvc::EDirection eDirection);

    virtual void ClearConnectionSvc();


    //-- << CEventDriven >>
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);
    //-- << Startup mechanism >>
    static mxt_result InitializeCSipTransactionCompletionSvc();

    static void FinalizeCSipTransactionCompletionSvc();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Helpers >>

    // Summary:
    //  Inserts a transaction into the map.
    //--------------------------------------
    void InsertTransaction(IN const CString& rstrMethod);

    //-- << Deactivated Constructors / Destructors / Operators >>
    // Summary:
    //  Copy Constructor.
    CSipTransactionCompletionSvc(IN const CSipTransactionCompletionSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipTransactionCompletionSvc& operator=(IN const CSipTransactionCompletionSvc& rSrc);


//-- Hidden Data Members
protected:
private:

    // The event manager used to report events to the application.
    //-------------------------------------------------------------
    ISipTransactionCompletionMgr* m_pTransCompletionMgr;

    // This map will hold the application's transaction completion information
    // on a SIP method basis.
    //
    // The key MUST be a CString. If 'const char*' is used instead, the search
    // would be based on the string address and not on the string content. It
    // can not be of type ESipMethod because the generic service can be used
    // with methods that are not in that enumeration type.
    //
    //-- Declaration of container type is required for the support of
    //-- VxWorks54_Ncui2_ppc_gcc272[_d].
    //-------------------------------------------------------------------------
    CMap<CString,
         STransCompletionInfo *,
         CAATree<CMapPair<CString,
                          STransCompletionInfo *> > > m_mapTransCompletionMgrs;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetTransactionCompletionInfoMap
//==
//==============================================================================
//
//  Returns:
//      A reference to a map that contains the information about the ongoing
//      transactions.
//
//  Description:
//      Returns a reference to the map that contains the information about the
//      ongoing transactions.
//
//==============================================================================
inline CMap<CString,
            CSipTransactionCompletionSvc::STransCompletionInfo *,
                CAATree<CMapPair<CString,
                    CSipTransactionCompletionSvc::STransCompletionInfo *> > > &
       CSipTransactionCompletionSvc::GetTransactionCompletionInfoMap()
{
    return m_mapTransCompletionMgrs;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPTRANSACTIONCOMPLETIONSVC_H

