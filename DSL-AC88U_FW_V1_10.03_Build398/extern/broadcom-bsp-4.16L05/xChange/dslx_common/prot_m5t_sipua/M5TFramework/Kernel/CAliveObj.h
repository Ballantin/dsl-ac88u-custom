//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================

#ifndef MXG_CALIVEOBJ_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CALIVEOBJ_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES  ===========================================================
//===========================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#if !defined(MXD_OS_NONE)

// Data Member
//------------
#ifndef MXG_CSEMAPHORE_H
#include "Kernel/CSemaphore.h"
#endif
#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

// Parent
//-------
#ifndef MXG_CTHREAD_H
#include "Kernel/CThread.h"
#endif

//===========================================================================
//====  MEDIATRIX NAME SPACE  ===============================================
//===========================================================================

MX_NAMESPACE_START(MXD_GNS)

//===========================================================================
//====  DEFINES  ============================================================
//===========================================================================

// Version definition used by children of CAliveObj
//-------------------------------------------------
//M5T_INTERNAL_USE_BEGIN
#define ALIVEOBJ_VERSION_1
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//==
//==  CAliveObj
//==
//==============================================================================
//<GROUP KERNEL_CLASSES>
//
//  Summary:
//    Manages the life of an object.
//
//  Description:
//    The responsibility of the Alive Object is to manage the life of an object.
//    Each Alive Object is associated to one thread. Part of its
//    responsibilities are to start the thread and to manage the internal life
//    state of the thread. The Alive Object is an abstract base class,
//    therefore it is designed to be specialized by other classes. At its
//    creation the Alive Object is in an unborn state (the associated thread has
//    never been started). The Alive Object becomes alive when its member
//    function Activate is called, at which point the associated thread is
//    started. Following the termination of the thread, the Alive Object
//    becomes dead. It is then possible to call Activate again.
//
//    On every known Operating System, when a thread is started it needs a
//    pointer to a function. This function is the equivalent of the main for
//    this thread. In the context of the Alive Object this main function is
//    called the "Behavior" of the Alive Object. The Behavior member function
//    is a pure virtual function, therefore it must be defined in a class
//    derived from Alive Object.
//
//    The normal way to end a thread is to exit from its main function
//    (Behavior member function), therefore the Terminate member function must
//    somehow indicate to the Behavior member function to end.
//
//    The Alive Object termination is not managed by CAliveObj. This is mainly
//    because there are many ways to accomplish this. Upon thread termination,
//    the method BehaviorTerminating is called. This allows the
//    inheriting class to signal its termination with the mechanism of its
//    choice.
//
//==EDOC========================================================================
class CAliveObj : public CThread
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CAliveObj();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CAliveObj();

    // Summary:
    //  Gives life to the object (Start the associated thread).
    //----------------------------------------------------------
    mxt_result Activate(IN const char* pszName = NULL,
                        IN uint32_t uStackSize = 0,
                        IN CThread::EPriority ePriority = CThread::eNORMAL);

    // Summary:
    //  Indicates the state of the object (thread-safe).
    //---------------------------------------------------
    bool IsAlive();

    // Summary:
    //  Indicates the state of the object (thread-safe).
    //---------------------------------------------------
    bool IsDead();

    // Summary:
    //  Indicates the state of the object (thread-safe).
    //---------------------------------------------------
    bool IsUnborn();

//M5T_INTERNAL_USE_BEGIN
// Protected Methods.
//--------------------
protected:

    // The inheriting class MUST overload this method. It will be called when
    // the thread is started.
    virtual void Behavior() = 0;

    // The inheriting class could overload this method to signal the
    // termination of the thread using whatever method it supports.
    virtual void BehaviorTerminating() {}

// Private Methods
//---------------
private:

    // Main function of the associated thread.
    static mxt_opaque StartMechanism(IN mxt_opaque);

    // Disable default copy constructor and of the assignement operator.
    CAliveObj(CAliveObj& from);
    CAliveObj& operator=(CAliveObj& from);

// Private Data Members
//--------------------
private:

    enum EState
    {
        eINVALID,
        eUNBORN,
        eACTIVATING, // It is a specialized case of eALIVE...
        eALIVE,
        eDEAD
    };

    EState m_eState;

    // Used to check if the the Activate method was called.
    // m_bActivating will become false again when m_bAlive becomes true.
    bool m_bActivating;

    // Used to check if the object is "Alive".
    bool m_bAlive;

    // Used to check that the object has never been Activated yet.
    bool m_bUnborn;

    // Semaphore used to wait for the thread creation.
    CBinarySemaphore m_semAlive;

    // Semaphore used to wait for Activate() to complete.
    CBinarySemaphore m_semActivateCompleted;

    // Critical Section for the thread-safe protection.
    CMutex m_mutexActivationProtection;
    CMutex m_mutexStateProtection;
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //#if !defined(MXD_OS_NONE)

#endif // #ifndef MXG_CALIVEOBJ_H

