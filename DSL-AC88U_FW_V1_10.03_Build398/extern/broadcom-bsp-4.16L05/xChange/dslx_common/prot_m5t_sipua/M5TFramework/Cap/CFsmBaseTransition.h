//==============================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2010 Media5 Corporation ("Media5")
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
#ifndef MXG_CFSMBASETRANSITION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CFSMBASETRANSITION_H

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- CommandDigitsInterface Global Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)

MX_NAMESPACE_START(MXD_GNS)

class CFsmBaseState;

//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================
const int MAX_IMBRICATION = 20; // Maximum level of imbrication

//==============================================================================
//== Class: CFsmBaseTransition
//========================================
//
// Description:
//   Perform transition between a source state and a destination state.
//
// Location:
//   Cap/CFsmBaseTransition.h
//
// See Also:
//   CFsmBaseState
//
//==============================================================================
class CFsmBaseTransition
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CFsmBaseTransition(IN CFsmBaseState* pSource, IN CFsmBaseState* pDestination);

    // Summary:
    //  Destructor.
    virtual ~CFsmBaseTransition();

    // Summary:
    //  Execute the exit of the source state
    void ExitState();

    // Summary:
    //  Execute the entry to the destination state
    void EnterState();

// Hidden Data Members
private:

    // Memorize if the ExitState function has been called
    bool m_bHasExited;

    // Memorize if the EnterState function has been called
    bool m_bHasEntered;

    // Position in m_apBufferSource
    int m_nCountSource;

    // Position in m_apBufferDest
    int m_nCountDest;

    // Buffer of all the state to exit
    CFsmBaseState* m_apBufferSource[MAX_IMBRICATION];

    // Buffer of all the state to enter
    CFsmBaseState* m_apBufferDest[MAX_IMBRICATION];

    // Destination State
    CFsmBaseState* m_pStateDest;
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifdef MXD_CAP_ENABLE_SUPPORT

//M5T_INTERNAL_USE_END

#endif // #ifndef MXG_CFSMBASETRANSITION_H

