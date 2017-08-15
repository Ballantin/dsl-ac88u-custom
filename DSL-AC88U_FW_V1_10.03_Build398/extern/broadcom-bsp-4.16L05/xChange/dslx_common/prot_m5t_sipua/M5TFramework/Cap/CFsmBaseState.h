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
#ifndef MXG_CFSMBASESTATE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CFSMBASESTATE_H

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

//-- Data Members
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//== Class: CFsmBaseState
//==============================================================================
//
// Description:
//   Describes a given state and the logic executed upon entering the state
//   exiting the state and the state logic itself.
//
// Location:
//   Cap/CFsmBaseState.h
//
// See Also:
//   CTransition
//
//==============================================================================
class CFsmBaseState
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CFsmBaseState();

    // Summary:
    //  Destructeur
    virtual ~CFsmBaseState();

    // Summary:
    //  Retrieve the state parent.
    virtual CFsmBaseState* GetParent();

    //==========================================================================
    //==
    //==  EntryAction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Every state may have an entry action.
    //      By default this function do nothing it must be overriden if
    //      necessary
    //
    //==========================================================================
    virtual void EntryAction() {};

    //==========================================================================
    //==
    //==  Activity
    //==
    //==========================================================================
    //
    //  Summary:
    //      Every state may have an activity action.
    //      By default this function do nothing it must be overriden if
    //      necessary.
    //
    //==========================================================================
    virtual void Activity() {};

    //==========================================================================
    //==
    //==  ExitAction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Every state may have an exit action.
    //      By default this function do nothing it must be overriden if
    //      necessary.
    //
    //==========================================================================
    virtual void ExitAction() {};

    //==========================================================================
    //==
    //==  InitialAction
    //==
    //==========================================================================
    //
    //  Summary:
    //      Every state may have an initial action.
    //      By default this function do nothing it must be overriden if
    //      necessary.
    //
    //==========================================================================
    virtual void InitialAction() {};

    //==========================================================================
    //==
    //==  ChangeState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Must be implemented by the context specific state (CContextState)
    //
    //==========================================================================
    virtual void ChangeState(CFsmBaseState* pNextState) = 0;

    // Summary:
    //  Start the state activity or start the internal state machine
    virtual void DoActivity();

    // Summary:
    //  Start the state machine (Only called by the context)
    virtual void StartStateMachine();

    // Summary:
    //  Specify a value for the name attribute.
    void SetName(IN const char* pszName);

    // Summary:
    // Retreive the value of the name attribute.
    CString GetName();

// Hidden Data Members
protected:

    // Parent of the state
    CFsmBaseState* m_pParent;

    // Indicate the initial state in the case of a nested state, NULL otherwise
    CFsmBaseState* m_pInitialState;

    // State name.
    CString m_strName;
};

//==============================================================================
//==
//==  GetParent
//==
//==============================================================================
//
//  Returns:
//      Parent state.
//
//  Description:
//      Retrieve the parent state pointer.
//
//==============================================================================
inline CFsmBaseState* CFsmBaseState::GetParent()
{
    return m_pParent;
}

//==============================================================================
//==
//==  SetName
//==
//==============================================================================
//
//  Parameters:
//      pszName:
//       New name value.
//
//  Description:
//      Update state name value.
//
//==============================================================================
inline void CFsmBaseState::SetName(IN const char* pszName)
{
    m_strName = pszName;
}

//==============================================================================
//==
//==  GetName
//==
//==============================================================================
//
//  Returns:
//      State name value.
//
//  Description:
//      Retreive State name.
//
//==============================================================================
inline CString CFsmBaseState::GetName()
{
    return m_strName;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifdef MXD_CAP_ENABLE_SUPPORT

//M5T_INTERNAL_USE_END

#endif // #ifndef MXG_CFSMBASESTATE_H

