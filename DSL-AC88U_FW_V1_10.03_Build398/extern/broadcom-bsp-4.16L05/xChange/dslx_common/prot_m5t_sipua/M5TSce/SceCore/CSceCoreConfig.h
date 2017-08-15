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
#ifndef MXG_CSCECORECONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCECORECONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSCEREQUESTDISPATCHER_H
#include "SceCore/CSceRequestDispatcher.h"
#endif

#ifndef MXG_CSCEUSERCONFIG_H
#include "SceCoreComponents/CSceUserConfig.h"
#endif

#ifndef MXG_CSCEUSERLIST_H
#include "SceCore/CSceUserList.h"
#endif

#ifndef MXG_ISCECONFIG_H
#include "SceCore/ISceCoreConfig.h"
#endif

#ifndef MXG_ISCESHUTDOWNMGR_H
#include "SceCore/ISceShutdownMgr.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISIPCORECONFIG_H
#include "SipCore/ISipCoreConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class ISceUser;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceCoreConfig
//========================================
//
// Description:
//  Implementation of the ISceCoreConfig interface. Single configuration and
//  startup point for the SIP Client Engine.
//
// Location:
//   SceCore/CSceCoreConfig.h
//
//==============================================================================
class CSceCoreConfig : private ISceCoreConfig,
                       private ISceShutdownMgr,
                       private CEComDelegatingUnknown
{
//-- Friend Declarations.
    friend class CSceCoreInitializer;

//-- New types within class namespace.
public:
protected:
private:
    // This structure is used to hold the startup/Shutdown callback related information.
    struct SStartupShutdownCallbacks
    {
        mxt_PFNStartupCallback  m_pfnStartupCallback;
        mxt_PFNShutdownCallback m_pfnShutdownCallback;
        bool m_bCallShutdownCallback;

        SStartupShutdownCallbacks(IN mxt_PFNStartupCallback pfnStartupCallback = NULL,
                                  IN mxt_PFNShutdownCallback pfnShutdownCallback = NULL);
    };

    // This enum represents the current state of the SCE core mechanisms.
    enum ESceState
    {
        // This value indicates that the Startup method has been successfully called.
        eSCE_STATE_STARTED = 0x00000001,

        // This value indicates that the Shutdown method has been sucessfully called
        // and thus, that a shutdown is in progress.
        eSCE_STATE_SHUTDOWN_INITIATED = 0x00000002,
    };

//-- Published Interface.
public:
//M5T_INTERNAL_USE_END
#if 0
    //-- This is only for documentation purposes, this method should probably be
    //-- in the interface to remove this nasty hack.
    //
    // Summary:
    //  Gets the shutdown completion status.
    static bool CSceCoreConfig::IsShutdownCompleted();
#endif

//M5T_INTERNAL_USE_BEGIN
    static bool IsShutdownCompleted();

//-- Hidden Methods.
protected:
    //-- << ISceShutdownMgr >>
    virtual void EvOnShutdownCompleted(IN mxt_opaque opqShutdown);

    //-- << Internals >>

    //-- << Helpers >>

private:
    //-- << ISceCoreConfig >>
    virtual mxt_result Startup();
    virtual mxt_result ShutdownA(IN ISceShutdownMgr* pMgr,
                                 IN mxt_opaque opqShutdown);
    virtual mxt_result SetSceThread(IN IEComUnknown* pThread);
    virtual mxt_result SetSceThreads(IN IEComUnknown* pCoreThread,
                                     IN IEComUnknown* pTransportThread,
                                     IN IEComUnknown* pResolverThread);
    virtual mxt_result GetCoreThread(OUT IEComUnknown** ppThread) const;
    virtual mxt_result GetTransportThread(OUT IEComUnknown** ppThread) const;
    virtual mxt_result GetResolverThread(OUT IEComUnknown** ppThread) const;
// BRCM_CUSTOM - [add] Add seed initialization
    virtual void SetSeed(IN unsigned int uSeed);
// BRCM_CUSTOM - [end] Add seed initialization
    virtual mxt_result RegisterRequestHandler(IN ISceRequestHandler* pHandler);
    virtual mxt_result GetHandlerInterface(IN mxt_iid iidRequested,
                                           OUT void** ppInterface) const ;
    virtual mxt_result RegisterUserConfigObject(IN mxt_clsid classId);
    virtual mxt_result RegisterUser(IN ISceUser* pUser);
    virtual mxt_result UnregisterUser(IN ISceUser* pUser);
    virtual void RegisterSupportedMethod(IN ESipMethodType eMethod);
    virtual void RegisterSupportedExtension(IN ESipOptionTag eTag);
    virtual void RegisterSupportedPayload(IN ESipMediaType eMediaType);
    virtual void RegisterSupportedEvent(IN ESipEventType eEvent);
// BRCM_CUSTOM - [add] Remove from supported
    virtual void RemoveSupportedExtension(IN ESipOptionTag eTag);
// BRCM_CUSTOM - [end] Remove from supported
    virtual uint32_t GetSupportedMethods() const;
    virtual uint32_t GetSupportedExtensions() const;
    virtual uint32_t GetSupportedPayloads() const;
    virtual uint32_t GetSupportedEvents() const;
    virtual void GenerateInstanceId(IN const CString& rstrMacAddress,
                                    IN uint64_t uTime,
                                    OUT CString& rstrInstanceId);

    virtual void RegisterStartupShutdownCallbacks(IN mxt_PFNStartupCallback pfnStartupCallback,
                                                  IN mxt_PFNShutdownCallback pfnShutdownCallback);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceCoreConfig(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CSceCoreConfig();

    //-- << Initialization mechanism >>
    static mxt_result InitializeCSceCoreConfig();

    static void FinalizeCSceCoreConfig();

    //-- << CEComDelegatingUnknown Interface >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Helpers >>

    // Summary:
    //  Sets the specified bitmask in the ms_nSceState bitset.
    static void SetInBitSet(IN int nBitMask);

    // Summary:
    //  Resets the specified bitmask in the ms_nSceState bitset.
    static void ResetInBitSet(IN int nBitMask);

    // Summary:
    //  Tells if the specified bitmask is present in the ms_nSceState bitset.
    static bool IsInBitSet(IN int nBitMask);

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceCoreConfig(IN const CSceCoreConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceCoreConfig& operator=(IN const CSceCoreConfig& rSrc);


//-- Hidden Data Members.
protected:
private:
    ISceShutdownMgr* m_pShutdownMgr;

    static uint32_t ms_uMethods;
    static uint32_t ms_uExtensions;
    static uint32_t ms_uPayloads;
    static uint32_t ms_uEvents;
    static CSceUserList* ms_pCoreUserList;
    static CSceRequestDispatcher* ms_pCoreRequestDispatcher;
    static int ms_nSceState;
    static CVector<SStartupShutdownCallbacks*>* ms_pvecstStartupShutdownCallbacks;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline uint32_t CSceCoreConfig::GetSupportedMethods() const
{
    return ms_uMethods;
}

inline uint32_t CSceCoreConfig::GetSupportedExtensions() const
{
    return ms_uExtensions;
}

inline uint32_t CSceCoreConfig::GetSupportedPayloads() const
{
    return ms_uPayloads;
}

inline uint32_t CSceCoreConfig::GetSupportedEvents() const
{
    return ms_uEvents;
}

inline mxt_result CSceCoreConfig::RegisterRequestHandler(IN ISceRequestHandler* pHandler)
{
    return ms_pCoreRequestDispatcher->RegisterRequestHandler(pHandler);
}

inline mxt_result CSceCoreConfig::GetHandlerInterface(IN mxt_iid iidRequested,
                                                         OUT void** ppInterface) const
{
    // Forwards the call to CSceRequestDispatcher.
    return ms_pCoreRequestDispatcher->GetHandlerInterface(iidRequested, ppInterface);
}

inline mxt_result CSceCoreConfig::RegisterUser(IN ISceUser* pUser)
{
    return ms_pCoreUserList->RegisterUser(pUser);
}

inline mxt_result CSceCoreConfig::UnregisterUser(IN ISceUser* pUser)
{
    return ms_pCoreUserList->UnregisterUser(pUser);
}
//M5T_INTERNAL_USE_END

//<GROUP TID_PACKAGE_SCECORE_FUNCTIONS>
//==============================================================================
//==
//==  IsShutdownCompleted
//==
//==============================================================================
//
//  Returns:
//      - TRUE: The shutdown is completed.
//      - FALSE: The shutdown is not initiated or completed yet.
//
//  Description:
//      Gets the shutdown completion status.
//
//  #NOTE#
//      If ShutdownA has been called without a manager pointer, the application
//      MUST NOT finalize the Sip Client Engine until this method returns true.
//      Failure to do so could result in finalization problems making it
//      impossible for the process to terminate correctly.
//
//  See Also:
//      ShutdownA
//
//==============================================================================
inline bool CSceCoreConfig::IsShutdownCompleted()
{
    return IsInBitSet(eSCE_STATE_STARTED) == false;
}

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CSCECORECONFIG_H
