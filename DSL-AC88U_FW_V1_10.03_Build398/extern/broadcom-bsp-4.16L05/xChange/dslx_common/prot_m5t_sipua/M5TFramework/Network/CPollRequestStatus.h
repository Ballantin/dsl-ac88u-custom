//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CPOLLREQUESTSTATUS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPOLLREQUESTSTATUS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
#endif

//-- The CPollRequestStatus class under Symbian replaces
//-- the similar CPollSocket class.
//-------------------------------------------------------
#if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)


//-- Data Members
//-----------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_IPOLLEDREQUESTSTATUSMGR_H
#include "Network/IPolledRequestStatusMgr.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
const unsigned int uSOCKET_ALL_EVENTS = 7;
const unsigned int uSOCKET_IN_EXCEPTION = 1;
const unsigned int uSOCKET_READABLE = 2;
const unsigned int uSOCKET_WRITABLE = 4;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CPollRequestStatus
//========================================
//<GROUP NETWORK_CLASSES>
//<FLAG Symbian OS ONLY>
//
// Summary:
//   Class providing polled events detection on multiple TRequestStatus.
//
// Description:
//   This class is a CPollSocket look-alike for Symbian.
//
//   This class provides support for polled request status completion
//   detection on TRequestStatus and notification of these events to the
//   manager associated with the request status for which completion was
//   detected.
//
//   A request status must be registered before CPollRequestStatus polls it for
//   completion. This is done with a call to RegisterRequestStatus. The
//   request status, the manager and an optional opaque value must be provided.
//
//   Once registered, it is possible to enable the completion detection.
//   This is done with a call to EnableCompletionDetection. A request status
//   MUST be set to KRequestPending before calling EnableCompletionDetection.
//
//   It is also possible to disable the completion detection.
//   This is done with a call to DisableCompletionDetection.
//
//   A request status must be unregistered when events detection is no longer
//   required. This is done with a call to UnregisterRequestStatus.
//
//   The method Poll must be called to poll each registered request status. The
//   class will go through its request status list identifying the ones where
//   completion is detected. If completion is detected , the method Poll will
//   call IPolledRequestStatusMgr::EvPolledRequeststatusMgrEventDetected to
//   notify the manager about the completion detection. It is possible to
//   provide a timeout to the method Poll. If the timeout is different than 0,
//   then the Poll method will block until a completion is detected or the
//   timeout is reached.
//
//   This class has no thread nor concurrency protection. It is the user's
//   responsibility to manage that. It was decided to give this flexibility.
//   It prevents the performance costs of entering/exiting critical sections
//   systematically while there is a possibility that the user calls all
//   CPollRequestStatus methods from the same thread.
//
//   The main reason for the presence of this class is to request status
//   completion detection in the context of asynchronous sockets into the same
//   thread.
//
//   For the polling mechanism to work, each request status MUST be used in the
//   context of a single thread. It SHOULD only be used in conjunction with a
//   CServicingThread object.
//
// Location:
//   Network/CPollRequestStatus.h
//
// See Also:
//   IPolledRequestStatusMgr, CServicingThread
//
//==EDOC========================================================================
class CPollRequestStatus
{
//M5T_INTERNAL_USE_BEGIN
private:
    // New type definition - internal use only
    struct SPolledRequestStatus
    {
        TRequestStatus* m_pRequestStatus;
        IPolledRequestStatusMgr* m_pPolledRequestStatusMgr;
        mxt_opaque m_opq;
        bool m_bIsEnabled;
    };
//M5T_INTERNAL_USE_END

// Published Interface
//---------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CPollRequestStatus();

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CPollRequestStatus();

    // Summary:
    //  Returns the number of request status currently registered.
    //-------------------------------------------------------------
    unsigned int GetRegisteredRequestStatusCount();

    // Summary:
    //  Adds a request status / Polled request status manager association in
    //  the list of polled request status.
    //-----------------------------------------------------------------------
    mxt_result RegisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                     IN IPolledRequestStatusMgr* pPolledRequestStatusMgr,
                                     IN mxt_opaque opq = MX_INT32_TO_OPQ(0));

    // Summary:
    //  Enables the detection of completion for a request status.
    //------------------------------------------------------------
    mxt_result EnableCompletionDetection(IN TRequestStatus* pRequestStatus);

    // Summary:
    //  Gets wether or not detection is enabled.
    //-------------------------------------------
    mxt_result GetCompletionDetectionState(IN TRequestStatus* pRequestStatus,
                                           OUT bool& rbIsEnabled);

    // Summary:
    //  Disables the detection of completion for a request status.
    //-------------------------------------------------------------
    mxt_result DisableCompletionDetection(IN TRequestStatus* pRequestStatus);

    // Summary:
    //  Removes a request status from the list of polled request status.
    //-------------------------------------------------------------------
    mxt_result UnregisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                       OUT mxt_opaque* popq = NULL);

    // Summary:
    // This method is time critical and must be called periodically
    // to detect events on registered request status.
    //----------------------------------------------------------------
    mxt_result Poll(IN uint64_t uTimeoutMs = 0);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------
    CPollRequestStatus(IN const CPollRequestStatus& rSrc);
    CPollRequestStatus& operator=(IN const CPollRequestStatus& rSrc);
    CPollRequestStatus& operator==(IN const CPollRequestStatus& rSrc);

    //-- << Helpers >>
    //-----------------
    unsigned int Find(IN const TRequestStatus* pRequestStatus);

//-- Hidden Data Members
//------------------------
protected:

private:

    // The list of request status to poll.
    CVector<SPolledRequestStatus> m_vecstPolledRequestStatus;
    bool m_bRequestStatusUnregistered;
//M5T_INTERNAL_USE_END
};

//==SDOC========================================================================
//==
//==  GetRegisteredRequestStatusCount
//==
//==============================================================================
//
//  Returns:
//      The number of registered request status
//
//  Description:
//      Gets the number of registered request status in the list.
//
//==EDOC========================================================================
inline
unsigned int CPollRequestStatus::GetRegisteredRequestStatusCount()
{
    return m_vecstPolledRequestStatus.GetSize();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_CPOLLREQUESTSTATUS_H
