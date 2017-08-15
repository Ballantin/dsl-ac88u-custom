//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPDIALOGMATCHERLIST_H
#define MXG_CSIPDIALOGMATCHERLIST_H

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
#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CToken;
class ISipDialogMatcher;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==SDOC========================================================================
//== Class: CSipDialogMatcherList
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//  The dialog matcher list keeps a list of active dialogs indexed by their
//  Call-ID.
//
//  The dialog matcher interface is responsible for adding itself to the dialog
//  matcher list when it is bound to a Call-ID.
//
//  Note that it is possible that two or more dialogs share the same Call-ID.
//  This list properly manages this case.
//
// Location:
//   SipCore/CSipDialogMatcherList.h
//
//==EDOC========================================================================
class CSipDialogMatcherList
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
    CSipDialogMatcherList();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipDialogMatcherList();


    //-- << Incoming packet management >>
    //--------------------------------------------

    // Summary:
    //  Notifies the list of the reception of a new packet.
    //------------------------------------------------------
    mxt_result OnPacketReceived(IN const CSipPacket& rPacket);


    //-- << External List Management >>
    //--------------------------------------------

    // Summary:
    //  Adds a SIP dialog to the list of managed dialogs.
    //---------------------------------------------------
    mxt_result RegisterSipDialogMatcher(IN ISipDialogMatcher& rDialogMatcher,
                                        IN const CToken& rCallId);

    // Summary:
    //  Removes a SIP dialog from the list of managed dialogs.
    //---------------------------------------------------------
    mxt_result UnregisterSipDialogMatcher(IN ISipDialogMatcher& rDialogMatcher,
                                          IN const CToken& rCallId);

    // Summary:
    //  Obtains dialog matcher pointers that match this Call-Id.
    //-----------------------------------------------------------
    void GetDialogMatchers(
                          IN const CToken& rCallId,
                          OUT CVector<ISipDialogMatcher*>& rvecpDialogMatchers);

//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipDialogMatcherList(IN const CSipDialogMatcherList& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipDialogMatcherList& operator=(IN const CSipDialogMatcherList& rSrc);


//-- Hidden Data Members
//------------------------
protected:

#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
    // GCC 2.7.2 does not support default parameters in templates,
    // in which case all the parameters must be specified.
    CMap<CString, CVector<ISipDialogMatcher*>*,
        CAATree<CMapPair<CString, CVector<ISipDialogMatcher*>*> > > m_mapDialogs;
#else
    CMap<CString, CVector<ISipDialogMatcher*>*> m_mapDialogs;
#endif

private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPDIALOGMATCHERLIST_H

