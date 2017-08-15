//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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

//@@MX_PACK_MEMBER
//<GROUP BASIC_MACROS>
//<TITLE MX_PACK MACROS>
//==SDOC========================================================================
//
// Summary:
//  Macro for packing structures and class members.
//
// Description:
//  Macro for packing class members.
//      MX_PACK_MEMBER
//  Macro for packing structure.
//      MX_PACK_STRUCT
//
//  The packing and alignment attributes are usually set to the compiler default
//  values. Therefore, when it is required that a structure or class is packed
//  or aligned using values different from the default values, it must be
//  specified explicitly using compiler specific keywords. It is assumed that
//  when changed for a specific definition, these attributes are set back to the
//  compiler defaults.
//
//  This mechanism is intended to always pack on the smallest possible alignment
//  (1 byte for variable, 1 bit for bit-field). The mechanism follows the
//  assumption described in the previous paragraph by reverting the attributes
//  to their default value.
//
//  <CODE>
//  Examples of good utilization:
//
//  1-   #include "Basic/MxPack.h"
//       struct SSomeStruct
//       {
//           char cMember        MX_PACK_MEMBER;
//           int16_t nMember     MX_PACK_MEMBER;
//           uint32_t uMember    MX_PACK_MEMBER;
//       } MX_PACK_STRUCT;
//       #include "Basic/MxPack.h"
//
//  2-   #include "Basic/MxPack.h"
//       struct SSomeStruct
//       {
//           char cMember        MX_PACK_MEMBER;
//           int16_t nMember     MX_PACK_MEMBER;
//           uint32_t uMember    MX_PACK_MEMBER;
//       } MX_PACK_STRUCT;
//
//       struct SSomeOtherStruct
//       {
//           char cMember        MX_PACK_MEMBER;
//           int16_t nMember     MX_PACK_MEMBER;
//           uint32_t uMember    MX_PACK_MEMBER;
//       } MX_PACK_STRUCT;
//       #include "Basic/MxPack.h"
//
//  3-   #include "SomeOtherStruct.h"
//       #include "Basic/MxPack.h"
//       struct SSomeStruct
//       {
//           char cMember        MX_PACK_MEMBER;
//           int16_t nMember     MX_PACK_MEMBER;
//           uint32_t uMember    MX_PACK_MEMBER;
//       } MX_PACK_STRUCT;
//       #include "Basic/MxPack.h"
//  </CODE>
//
//  Caveats:
//      - Bit-fields are not supported when MXD_COMPILER_DIAB is defined.
//
//      - Never include a file between the two MxPack includes to avoid
//        unwanted side effects. The side effects are related to compiler
//        specific primitives. For example, in the following code, only the
//        SSomeOtherStruct structure is packed when MXD_COMPILER_DIAB or
//        MXD_COMPILER_MS_VC is defined. On the other hand, there are no side
//        effects when MXD_COMPILER_GNU_GCC is defined.
//
//        <CODE>
//        Example possibly introducing side effects:
//        SomeStruct.h:
//            #include "Basic/MxPack.h"
//                struct SSomeStruct
//                {
//                    char cMember        MX_PACK_MEMBER;
//                    int16_t nMember     MX_PACK_MEMBER;
//                    uint32_t uMember    MX_PACK_MEMBER;
//                };
//            #include "Basic/MxPack.h"
//        SomeOtherStruc.h:
//            #include "Basic/MxPack.h"
//            #include "SomeStruct.h"
//                struct SSomeOtherStruct
//                {
//                    char cMember        MX_PACK_MEMBER;
//                    int16_t nMember     MX_PACK_MEMBER;
//                    uint32_t uMember    MX_PACK_MEMBER;
//                };
//            #include "Basic/MxPack.h"
//        </CODE>
//
//------------------------------------------------------------------------------

//@@MX_PACK_STRUCT
//<GROUP BASIC_MACROS>
//<COMBINE MX_PACK_MEMBER>

//M5T_INTERNAL_USE_BEGIN
#if  defined(MX_PACK_STRUCT) || defined(MX_PACK_MEMBER)
    #if !defined(MX_PACK_STRUCT)
        #define MX_PACK_STRUCT
    #endif
    #if !defined(MX_PACK_MEMBER)
        #define MX_PACK_MEMBER
    #endif
#else
    #if defined(MXD_COMPILER_MS_VC) || defined(MXD_COMPILER_WINSCW)
        #define MX_PACK_STRUCT
        #define MX_PACK_MEMBER
    #elif defined (MXD_COMPILER_GNU_GCC)
        #define MX_PACK_STRUCT __attribute__ ((packed))
        #if MXD_COMPILER_GNU_GCC >= MXD_COMPILER_GNU_GCC_4_1
            // With GCC 4.1.2, when MX_PACK_MEMBER is defined with
            // "__attribute__ ((packed))" and this macro is used on a stucture member
            // that is already aligned on a byte, the following warning is generated:
            // "warning: 'packed' attribute ignored for field of type 'uint8_t'".
            // Also, packing a structure should not require to pack each member
            // individually. Because it has been designed like this for a
            // long time and we do not want to break any previous code, we decided to
            // remove the member packing attribute only starting with GCC 4.1.
            #define MX_PACK_MEMBER 
        #else
            #define MX_PACK_MEMBER __attribute__ ((packed))
        #endif
    #elif defined (MXD_COMPILER_DIAB)
        #define MX_PACK_STRUCT
        #define MX_PACK_MEMBER
    #elif defined(MXD_COMPILER_TI_CL6X)
        #define MX_PACK_MEMBER
        #define MX_PACK_STRUCT
    #else
        #define MX_PACK_MEMBER
        #define MX_PACK_STRUCT
    #endif
#endif

#ifndef PACK_STARTED
    #if defined(MXD_COMPILER_MS_VC) || defined(MXD_COMPILER_WINSCW)
        #pragma pack(push, mxt_pack_id, 1)
    #elif defined(MXD_COMPILER_GNU_GCC)
        // Nothing
    #elif defined(MXD_COMPILER_TI_CL6X)
        // Nothing
    #elif defined (MXD_COMPILER_DIAB)
        #pragma pack(1)
    #endif

    #define PACK_STARTED
#else
    #if defined(MXD_COMPILER_MS_VC) || defined(MXD_COMPILER_WINSCW)
        #pragma pack(pop, mxt_pack_id)
    #elif defined(MXD_COMPILER_GNU_GCC)
        // Nothing
    #elif defined(MXD_COMPILER_TI_CL6X)
        // Nothing
    #elif defined (MXD_COMPILER_DIAB)
        #pragma pack(0)          // Set to default packing.
    #endif

    #undef PACK_STARTED
#endif
//M5T_INTERNAL_USE_END
