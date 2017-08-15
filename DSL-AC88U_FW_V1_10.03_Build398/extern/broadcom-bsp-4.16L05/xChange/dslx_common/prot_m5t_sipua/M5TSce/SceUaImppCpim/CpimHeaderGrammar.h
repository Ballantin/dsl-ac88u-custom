//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CPIMHEADERGRAMMAR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPIMHEADERGRAMMAR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_TXTHEADERGRAMMAR_H
#include "TextMessage/TxtHeaderGrammar.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== File: CpimHeaderGrammar
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Description:
//  This file holds the definition of the CPIM header types and their associated
//  grammar.
//
// Location:
//   SceUaImppCpim/CpimHeaderGrammar.h
//
//==============================================================================


// Summary:
//  The ECpimHeaderType enum is the list of headers that the Text Parser 
//  understands of CPIM.
//
// Description:
//  The ECpimHeaderType enum is the list of headers that the Text Parser 
//  understands of CPIM.
//
// Warning:
//  This enum is used to index a table of header descriptors.
//
enum ECpimHeaderType
{
    // Description:
    // RFC 3862 cc headers.
    eHDR_CPIM_CC,

    // Description:
    // RFC 3862 DateTime headers.
    eHDR_CPIM_DATE_TIME,

    // Description:
    // RFC 3862 From headers.
    eHDR_CPIM_FROM,

    // Description:
    // RFC 3862 NS headers.
    eHDR_CPIM_NS,

    // Description:
    // RFC 3862 Require headers.
    eHDR_CPIM_REQUIRE,

    // Description:
    // RFC 3862 Subject headers.
    eHDR_CPIM_SUBJECT,

    // Description:
    // RFC 3862 To headers.
    eHDR_CPIM_TO,

    // Description:
    // This is an application defined header (not related to any known RFC or
    // draft). It maps to the RFC3261 extension-header construct.
    eHDR_CPIM_EXTENSION,

//M5T_INTERNAL_BEGIN
    // Description:
    // Size of header array.
    eHDR_CPIM_LAST
//M5T_INTERNAL_END
};

//------------------------------------------------------------------------------
// Header descriptors
//------------------------------------------------------------------------------
//M5T_INTERNAL_USE_BEGIN
extern const STxtHeaderDescriptor g_astCpimHeaderDescriptor[eHDR_CPIM_LAST];
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CPIMHEADERGRAMMAR_H

