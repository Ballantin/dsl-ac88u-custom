//==============================================================================
//==============================================================================
//
//         Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//         Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CFRAMEWORKINITIALIZER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CFRAMEWORKINITIALIZER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#if defined(MXD_OS_SYMBIAN)
    #include <es_sock.h>
#endif


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)

#if defined(MXD_FRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS)
    const size_t g_uFRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS =
        MXD_FRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS;
#else
    //<GROUP STARTUP_VARIABLES>
    //
    // Description:
    // Defaults to 64 if undefined.
    const size_t g_uFRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS = 64;
#endif // #if defined(MXD_FRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS)

#endif


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CFrameworkInitializer
//========================================
//<GROUP STARTUP_CLASSES>
//
// Summary:
//      This class is responsible of doing the initialization and finalization
//      of the Framework.
//
// Description:
//      CFrameworkInitializer::Initialize must be called before any of the
//      functionality from the Framework is used. CFrameworkInitializer::Finalize
//      must be called when the functionality from Framework is no longer
//      needed.
//
//      This class is not thread safe. If more than one call to Initialize or
//      Finalize are necessary, these calls must come from the same thread or be
//      protected against concurrent access. For example, if the M5T RTP and
//      M5T SIP are being used, since each needs to make a separate call to
//      CFrameworkInitializer Initialize and Finalize, these calls must be
//      thread safe.
//
//
//==EDOC========================================================================
class CFrameworkInitializer
{
//M5T_INTERNAL_USE_BEGIN
    // This enum is used to save the state of how far we got in the
    // initialization proces in order to do the partial finalize.
    enum EFinalizeFrom
    {
        eFROM_NO_INITIALIZATION,
#if defined(MXD_OS_SYMBIAN)
        eFROM_SYMBIAN_CFILE,
        eFROM_SYMBIAM_MITOS_FW,
#endif
        eFROM_MX_ASSERT,
#if defined(MXD_EVENT_NOTIFIER_ENABLE_SUPPORT)
        eFROM_MX_EVENT_NOTIFIER,
#endif
#if defined(MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT)
        eFROM_CMEMORY_ALLOCATOR,
#endif
        eFROM_RESULT,
#if  !defined(MXD_OS_NONE) && defined(MXD_NETWORK_ENABLE_SUPPORT)
        eFROM_CSOCKET,
#endif
#if !defined(MXD_OS_NONE)
        eFROM_CTHREAD,
#endif
#if defined(MXD_TRACE_ENABLE_SUPPORT)
        eFROM_MX_TRACE,
#endif
#if defined (MXD_CAP_ENABLE_SUPPORT)
        eFROM_CSTRING,
#endif
#if !defined(MXD_OS_NONE)
        eFROM_CTIMER,
        eFROM_MX_TRACE_KERNEL,
#endif
#if defined(MXD_TIME_ENABLE_SUPPORT)
        eFROM_CTIME,
        eFROM_MX_TRACE_TIME,
#endif
#if defined(MXD_ECOM_ENABLE_SUPPORT)
        eFROM_ECOM,
#endif
#if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)
    #if defined(MXD_XML_PARSER_EXPAT_ENABLE_SUPPORT)
        eFROM_CXML_PARSER_EXPAT,
    #endif
        eFROM_CXML_WRITER,
        eFROM_CXML_DOCUMENT,
        eFROM_CXML_ELEMENT,
#endif
#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)
        eFROM_CSERVICING_THREAD,
#if defined(MXD_ECOM_ENABLE_SUPPORT) && defined(MXD_NETWORK_ENABLE_SUPPORT)
        eFROM_CASYNC_SOCKET_FACTORY,
        eFROM_CASYNC_TCP_SERVER_SOCKET,
        eFROM_CASYNC_TCP_SOCKET,
        eFROM_CASYNC_UDP_SOCKET,
        eFROM_CTCP_SOCKET_OPTIONS,
#endif
#if defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)
        eFROM_CWATCHDOG_MONITOR,
#endif
#endif
#if defined (MXD_CAP_ENABLE_SUPPORT)
        eFROM_CMARSHALER,
        eFROM_CAA_TREE_BASE,
#endif
#if defined(MXD_NETWORK_ENABLE_SUPPORT)
        eFROM_CSOCKET_ADDR,
        eFROM_MX_TRACE_NETWORK,
#endif
#if defined(MXD_NETWORK_ENABLE_SUPPORT)
        eFROM_CASYNCRESOLVER,
#endif
#if defined(MXD_KERBEROS_ENABLE_SUPPORT)
        eFROM_CKERBEROS,
#endif
#if !defined(MXD_OS_NONE)
        eFROM_MX_STRING_FORMAT,
#endif
#if defined(MXD_FAULT_HANDLER_ENABLE_SUPPORT)
        eFROM_MX_FAULT,
#endif
#if !defined(MXD_CRYPTO_MD5_NONE) && \
    !defined(MXD_CRYPTO_SHA1_NONE) && \
    defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW)
        eFROM_CUUID_GENERATOR,
#endif
#if !defined(MXD_CRYPTO_ALL_NONE)
        eFROM_CCRYPTO,
#endif
#if defined(MXD_PKI_OPENSSL) || defined (MXD_PKI_MOCANA_SS)
        eFROM_CPKI,
#endif
#if !defined (MXD_TLS_NONE)
        eFROM_CTLS,
#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)
        eFROM_CASYNCTLSSOCKET,
        eFROM_CASYNCTLSSERVERSOCKET,
        eFROM_CASYNCTLSSERVERSOCKETFACTORYCREATIONMGR,
#endif
#endif
#if defined (MXD_RESULT_ENABLE_ERROR_MESSAGES) &&       \
    (defined (MXD_RESULT_ENABLE_ALL_ERROR_MESSAGES) || \
     defined (MXD_RESULT_ENABLE_MITOSFW_ERROR_MESSAGES))
        eFROM_RESULT_ID_MITOS_FW,
#endif
#if defined(MXD_CRYPTO_AES_MITOSFW) || \
    defined(MXD_CRYPTO_AES_OPENSSL) || \
    defined(MXD_CRYPTO_AES_MOCANA_SS)

        eFROM_CAES_MITOS_FW,
#endif
#if !defined(MXD_CRYPTO_SHA1_NONE) && defined(MXD_CRYPTO_SECURESEED_MITOSFW)
        eFROM_CSECURE_SEED_MITOS_FW,
#endif
#if !defined(MXD_CRYPTO_SHA1_NONE) && defined(MXD_CRYPTO_SECUREPRNG_MITOSFW)
        eFROM_CSECURE_PRNG_MITOS_FW,
#endif
#if !defined(MXD_CRYPTO_SHA1_NONE) && defined(MXD_CRYPTO_SECUREPRNG_OPENSSL)
        eFROM_CSECURE_PRNG_OPENSSL,
#endif
        eFROM_ALL
    };
//M5T_INTERNAL_USE_END

// Published Interface
//---------------------
public:

    //==SDOC====================================================================
    //==
    //== SFrameworkFinalizeInfo
    //==
    //==========================================================================
    //<GROUP STARTUP_STRUCTS>
    //
    // Summary:
    //  Structure used to report information at finalization time.
    //
    // Description:
    //  This structure is used to report some information when the framework's
    //  Finalization occurs.
    //
    //==EDOC====================================================================
    struct SFrameworkFinalizeInfo
#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)
        : public CMemoryAllocator::IMemoryBlockAccumulator
#endif
    {
        // Summary:
        //  Default Constructor.
        //-----------------------
        SFrameworkFinalizeInfo();

#if defined(MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT)
        // A structure holding memory allocation statistics.
        CMemoryAllocator::SMemoryStatistics m_stMemoryStatistics;
#endif

#if defined(MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT)

        // Summary:
        //  Virtual destructor to avoid the warning caused by having
        //  another virtual function.
        //-----------------------------------------------------------
        virtual ~SFrameworkFinalizeInfo()
        {
        }

        // Description:
        // Count of total allocated memory blocks.
        size_t m_uNumberOfAllocatedMemoryBlocks;

        //==SDOC================================================================
        //==
        //== SMemoryInfo
        //==
        //======================================================================
        //<GROUP STARTUP_STRUCTS>
        //
        // Summary:
        //  Holds a snapshot of memory at finalization time.
        //
        // Description:
        //  This structure is used to hold a snapshot of the information
        //  contained in a CMemoryAllocator::CMemoryBlock.
        //
        //==EDOC================================================================
        struct SMemoryInfo
        {
            // Description:
            // A string representing the Type of data contained in the memory
            // block.
            char const* m_pszType;
            // Description:
            // The filename of the file in which the memory allocation was done.
            char const* m_pszFilename;
            // Description:
            // The line number in the file at which the memory allocation was
            // done.
            uint16_t m_uLineNumber;
            // Description:
            // A boolean value representing whether the memory block is marked
            // as temporarily untracked.
            bool m_bIsTemporarilyUntracked;
            // Description:
            // The size of the memory block.
            size_t m_uSize;
            // Description:
            // A pointer to the memory block.
            void* m_pvoidAddress;
        };

        // Description:
        // Array of up to
        // MXD_FRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS of
        // currently allocated memory blocks. If there are less than
        // MXD_FRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS
        // allocated blocks, the m_uNumberOfAllocatedMemoryBlocks describes the
        // number of blocks in the array.
        SMemoryInfo m_astMemoryBlockInfo
            [g_uFRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS];
//M5T_INTERNAL_USE_BEGIN
    private:

        // Summary:
        //  Marks parent class as friend to allow access to the privately
        //  derived interface (i.e for implicit conversion).
        //----------------------------------------------------------------
        friend class CFrameworkInitializer;

        // Summary:
        //  Implementation of the
        //  CMemoryAllocator::IMemoryBlockAccumulator::Accumulate method.
        //--------------------------------------------------------------
        virtual void Accumulate(INOUT CMemoryAllocator::CMemoryBlock* pMemoryBlock);
//M5T_INTERNAL_USE_END
#endif
    };


    // Summary:
    //  Initializes the Framework.
    //-----------------------------
    static mxt_result Initialize();

    // Summary:
    //  Initializes the Framework.
    static mxt_result Initialize(IN const char* const* const ppszInitParameters,
                                 IN unsigned int uNumInitParameters);

    // Summary :
    //   Finalizes the Framework.
    //----------------------------
    static void Finalize();

    // Summary :
    //   Finalizes the Framework and returns information about the state of
    //   the framework at the point where the finalize method was invoked.
    //----------------------------------------------------------------------
    static unsigned int Finalize(OUT SFrameworkFinalizeInfo* pstFinalizeInfo);

    // Summary:
    //   Gets the initialization status of the Framework.
    //----------------------------------------------------
    static bool IsInitialized();

    // Summary:
    //   Gets the array of initialization parameters passed to the Initialize method.
    static const char* const* GetInitParameters();

    // Summary:
    //  Gets the number of initialization parameters passed to the Initialize method.
    static unsigned int GetInitParametersCount();

#if defined(MXD_OS_SYMBIAN)
    // Summary:
    //  Initializes Symbian 9.1 network functionality.
    //-------------------------------------------------
    static void InitializeSymbianNetwork(RConnection* pConnection,
                                         RSocketServ* pSocketServer);
#endif

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
private:
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif

    CFrameworkInitializer();
    ~CFrameworkInitializer();

    // Summary:
    //  This function is used to do a partial finalize in case the
    //  initialize process has failed somewhere.
    //-------------------------------------------------------------
    static void PartialFinalize(IN EFinalizeFrom eFinalizeFrom,
                                OUT SFrameworkFinalizeInfo* pstFinalizeInfo = NULL);

// Hidden Data Members
//---------------------
private:

    static unsigned int ms_uInitializationCount;
    static const char* const* ms_ppszInitParameters;
    static unsigned int m_uNumInitParameters;

//M5T_INTERNAL_USE_END
};

//==============================================================================
//==
//==  IsInitialized
//==
//==============================================================================
//
//  Returns:
//      True if the Framework is initialized, false otherwise.
//
//  Description:
//      This method gets the initialization status of the Framework.
//
//==============================================================================
inline
bool CFrameworkInitializer::IsInitialized()
{
    return ms_uInitializationCount != 0;
}

//==============================================================================
//==
//==  GetInitParameters
//==
//==============================================================================
//
//  Returns:
//      An array of initialization parameters
//
//  Description:
//      Gets the array of initialization parameters passed to the Initialize
//      method.
//
//  See Also:
//      Initialize, GetInitParametersCount
//
//==============================================================================
inline
const char* const* CFrameworkInitializer::GetInitParameters()
{
    return ms_ppszInitParameters;
}

//==============================================================================
//==
//==  GetInitParametersCount
//==
//==============================================================================
//
//  Returns:
//      The number of initialization parameters.
//
//  Description:
//      Gets the number of initialization parameters passed to the Initialize
//      method.
//
//  See Also:
//      Initialize, GetInitParameters
//
//==============================================================================
inline
unsigned int CFrameworkInitializer::GetInitParametersCount()
{
    return m_uNumInitParameters;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CFRAMEWORKINITIALIZER_H


