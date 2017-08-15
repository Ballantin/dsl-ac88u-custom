//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation.
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
#ifndef MXG_CWATCHEDINSTANCE_H
#define MXG_CWATCHEDINSTANCE_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

//-- Data Members


//-- Parameters used by Value


//-- Interface Realized and/or Parent


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CWatchedInstance
//========================================
//
// Description:
//   This class is the basic class for all objects that may be watched using a
//   CDeprecatedInstanceTracker. The object that needs to be watched must
//   inherit from it. It must also initialize its protected static members,
//   ms_pvecpInstances and ms_pmutex in its initialize methods, as well as
//   finalize it.
//
// Location:
//   SipTransport/CWatchedInstance.h
//
//==============================================================================
template <class _Type>
class CWatchedInstance
{
    friend class CDeprecatedInstanceTracker;

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CWatchedInstance();

    // Summary:
    //  Destructor.
    virtual ~CWatchedInstance();

//-- Hidden Methods
protected:
    // Summary:
    //  Gets a CBlob containing information about the SipContexts still
    //  allocated.
    static void Dump(OUT CBlob& blobContextDump);

    // Summary:
    //  Gets the number of watched instances still allocated.
    static unsigned int GetNumInstancesLeft();

    // Summary:
    //  Gets a vector with the pointers to the still allocated instances.
    static void GetInstancesTable(OUT CVector<_Type*>& rvecpInstances);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CWatchedInstance(IN const CWatchedInstance& rSrc);

    // Summary:
    //  Assignment Operator.
    CWatchedInstance& operator=(IN const CWatchedInstance& rSrc);

    // Summary:
    //  Locks the mutex.
    static void LockMutex();

    // Summary:
    //  Unlocks the mutex.
    static void UnlockMutex();

//-- Hidden Data Members
protected:
    static CVector<_Type*>*          ms_pvecpInstances;
    static CMutex*                   ms_pmutex;
private:
};

//===========================================================================
//====  STATIC MEMBER INITIALIZATION  =======================================
//===========================================================================
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && \
     MXD_COMPILER_GNU_GCC != MXD_COMPILER_GNU_GCC_2_7 && \
     MXD_COMPILER_GNU_GCC != MXD_COMPILER_GNU_GCC_2_96)

    template <class _Type>
    CVector<_Type*>* CWatchedInstance< _Type>::ms_pvecpInstances = NULL;

    template <class _Type>
    CMutex* CWatchedInstance< _Type>::ms_pmutex = NULL;
#endif

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

template <class _Type>
CWatchedInstance< _Type>::CWatchedInstance()
{
    ms_pmutex->Lock();
    ms_pvecpInstances->InsertSorted(static_cast<_Type*>(this));
    ms_pmutex->Unlock();
}

template <class _Type>
CWatchedInstance<_Type>::~CWatchedInstance()
{
    // Find and remove instance.
    ms_pmutex->Lock();
    unsigned int uIndex = ms_pvecpInstances->FindSorted(static_cast<_Type*>(this));
    ms_pvecpInstances->Erase(uIndex);
    ms_pmutex->Unlock();
}

template <class _Type>
unsigned int CWatchedInstance< _Type>::GetNumInstancesLeft()
{
    unsigned int uInstances = 0;
    ms_pmutex->Lock();
    uInstances = ms_pvecpInstances->GetSize();
    ms_pmutex->Unlock();
    return uInstances;
}

template <class _Type>
void CWatchedInstance< _Type>::GetInstancesTable(OUT CVector<_Type*>& rvecpInstances)
{
    rvecpInstances = *ms_pvecpInstances;
}

template <class _Type>
void CWatchedInstance< _Type>::LockMutex()
{
    ms_pmutex->Lock();
}

template <class _Type>
void CWatchedInstance< _Type>::UnlockMutex()
{
    ms_pmutex->Unlock();
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CWATCHEDINSTANCE_H

