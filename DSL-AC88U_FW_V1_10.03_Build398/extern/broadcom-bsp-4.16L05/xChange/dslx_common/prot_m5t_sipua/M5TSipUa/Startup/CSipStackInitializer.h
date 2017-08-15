//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPSTACKINITIALIZER_H
#define MXG_CSIPSTACKINITIALIZER_H

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

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
// Method pointers declaration for Finalize and Initialize.
typedef void (*PFNFinalizeMethod)();
typedef mxt_result (*PFNInitializeMethod)();
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//== Class: CSipStackInitializer
//========================================
// <GROUP STARTUP_CLASSES>
//
// Description:
//   Initializes the SIP stack and its dependencies.
//
// Location:
//   Startup/CSipStackInitializer.h
//
//==EDOC========================================================================
class CSipStackInitializer
{
//-- New types within class namespace.
private:
    // Structure to hold information on a module's global variable
    // initialization and finalization.
    struct SModule
    {
        // Method to call to initialize the module.
        PFNInitializeMethod m_pfnInitialize;

        // Method to call to finalize the module.
        PFNFinalizeMethod m_pfnFinalize;

        // Module initializer's name.
        const char* m_pszName;
    };



//-- Published Interface
//------------------------
public:

    //-- << Stereotype >>
    //--------------------------------------------

    // Summary:
    //  Initializes the SIP stack and its dependencies.
    static mxt_result Initialize();

    // Summary:
    //  Finalizes the SIP stack and its dependencies.
    static void Finalize();

    // Summary:
    //   Registers the module's tracing nodes.
    static void RegisterTracingNodes();

    // Summary:
    //   Unregisters the module's tracing nodes.
    static void UnregisterTracingNodes();

//-- Hidden Methods
//-------------------
protected:
private:
//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif
//M5T_INTERNAL_USE_END

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipStackInitializer();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipStackInitializer();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSipStackInitializer(IN const CSipStackInitializer& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSipStackInitializer& operator=(IN const CSipStackInitializer& rSrc);


//-- Hidden Data Members
//------------------------
protected:
    // Keeps the tracing initialization state.
    static bool ms_bTracingInitialized;

    // Counts the number of times Initialize and Finalize have been called to
    // make sure their processing is made only once.
    static unsigned int ms_uInitCount;

    // Counts the number of modules successfully initialized. This number will
    // then be used by the Finalize method as the starting index from where it
    // will call, in reverse, the finalizer's registered in ms_astMODULES.
    static unsigned int ms_uModulesInitCount;

    // This array stores all modules to be initialized.
    static const SModule ms_astMODULES[];
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPSTACKINITIALIZER_H

