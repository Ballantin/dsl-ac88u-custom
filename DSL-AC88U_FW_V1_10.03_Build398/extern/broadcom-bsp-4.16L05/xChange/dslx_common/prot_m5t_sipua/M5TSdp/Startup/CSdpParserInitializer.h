//==============================================================================
//==============================================================================
//
//   Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSDPPARSERINITIALIZER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPPARSERINITIALIZER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members


//-- Parameters used by Value


//-- Interface Realized and/or Parent


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpParserInitializer
//========================================
//<GROUP TID_PACKAGE_STARTUP_CLASSES>
//
// Summary:
//   This class is responsible of doing the initialization of SDP SAFE.
//
// Description:
//   Initializes the SDP Parser and its dependencies.
//
// Location:
//   Startup/CSdpParserInitializer.h
//
//==============================================================================
class CSdpParserInitializer
{
//M5T_INTERNAL_USE_BEGIN
    // This enum is used to save the state of how far we got in the
    // initialization proces in order to do the partial finalize.
    enum EFinalizeFrom
    {
        eFROM_NO_INITIALIZATION,
        eFROM_PKG_FRAMEWORK,
#if defined (MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE)
        eFROM_PKG_MIKEY,
#endif
        eFROM_ALL_INITIALIZED
    };
//M5T_INTERNAL_USE_END



//-- Published Interface
public:
    //-- << Stereotype >>

    // Summary:
    //  Initializes the SDP Parser and its dependencies.
    static mxt_result Initialize();

    // Summary:
    //  Finalizes the SDP Parser and its dependencies.
    static void Finalize();


//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif

    // Summary: Default constructor.
    CSdpParserInitializer();

    // Summary: Destructor.
    virtual ~CSdpParserInitializer();

    // Summary: Copy Constructor.
    CSdpParserInitializer(IN const CSdpParserInitializer& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpParserInitializer& operator=(IN const CSdpParserInitializer& rSrc);

    static void PartialFinalize(IN EFinalizeFrom eFinalizeFrom);


//-- Hidden Data Members
protected:
private:

    // Count the number of time Initialize and Finalize have been called to
    // make sure that both are done only once.
    static unsigned int ms_uInitializationCount;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSDPPARSERINITIALIZER_H

