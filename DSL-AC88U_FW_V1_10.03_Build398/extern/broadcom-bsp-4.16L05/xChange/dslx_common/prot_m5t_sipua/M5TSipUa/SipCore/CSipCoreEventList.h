//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPCOREEVENTLIST_H
#define MXG_CSIPCOREEVENTLIST_H

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


//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipReqCtxCoreSvc;
class ISipRequestContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipCoreEventList
//========================================
//
// Description:
//  The core event list is used to manage the events reported by the various
//  ISipCoreSvc when a packet is received.
//
//  Once events are collected, the owner of the event list can command the
//  generation of the next event.
//
// Location:
//   SipCore/CSipCoreEventList.h
//
//
//==EDOC========================================================================
class CSipCoreEventList
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
    CSipCoreEventList();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipCoreEventList();


    //-- << Initialization / Termination >>
    //--------------------------------------------

    // Summary:
    //  Initializes the event list for processing events related to a new
    //  incoming packet.
    //--------------------------------------------------------------------
    void Initialize(IN ISipRequestContext& rContext,
                    IN const CSipPacket& rPacket);

    // Summary:
    //  Flushes remaining events from the event list.
    //------------------------------------------------
    void Finalize();


    //-- << Event Processing >>
    //--------------------------------------------

    // Summary:
    //  Adds an event to the event list.
    //-----------------------------------
    mxt_result AddEvent( IN mxt_opaque opqEvent,
                         IN ISipReqCtxCoreSvc* pSvc,
                         IN bool bPrepend = false);


    // Summary:
    //  Commands the event list to report the next event it has on its list.
    //-----------------------------------------------------------------------
    mxt_result CallNextEvent();


    // Summary:
    //  Looks whether or not the event list is currently processing events.
    //----------------------------------------------------------------------
    bool IsProcessingEvents() const;



//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipCoreEventList(IN const CSipCoreEventList& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipCoreEventList& operator=(IN const CSipCoreEventList& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    struct SSipCoreEvent
    {
        ISipReqCtxCoreSvc* m_pSvc;
        mxt_opaque m_opqEvent;
    };

    // The list of events.
    //---------------------
    CVector<SSipCoreEvent> m_vecstEvents;

    // The request context which calls the events.
    //---------------------------------------------
    ISipRequestContext* m_pContext;

    // The packet associated to the request context.
    //-----------------------------------------------
    const CSipPacket* m_pPacket;

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  IsProcessingEvents
//==
//==============================================================================
//
//  Returns:
//      true: The event list is currently processing events.
//
//  Description:
//      This method allows the user of the event list to know whether or not it
//      is currently processing events.
//
//  See Also:
//      Initialize, Finalize
//
//==EDOC========================================================================
inline bool CSipCoreEventList::IsProcessingEvents() const
{
    return (m_pPacket != NULL);
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCOREEVENTLIST_H

