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
#ifndef MXG_MIMEHEADERGRAMMAR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MIMEHEADERGRAMMAR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CTXTHEADER_H
#include "TextMessage/CTxtHeader.h"
#endif

#ifndef MXG_TXTHEADERGRAMMAR_H
#include "TextMessage/TxtHeaderGrammar.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
//==============================================================================
//== File: MimeHeaderGrammar
//========================================
//
// Description:
//  This file holds the definition of the MIME header types and their associated
//  grammar.
//
// Location:
//   MimeMessage/MimeHeaderGrammar.h
//
//==============================================================================
extern const STxtHeaderDescriptor g_astMimeHeaderDescriptor[];
//M5T_INTERNAL_USE_END

//<GROUP MIME_MESSAGE_ENUMS>
//
// Summary:
//  The EMimeHeaderType enum is the list of headers that the Text Parser
//  understands.
//
// Description:
//  The EMimeHeaderType enum is the list of MIME headers that the Text Based
//  Protocol Parser understands. The eHDR_MIME_EXTENSION value is a special case
//  and represents the generic form of header as per RFC 3261, and is used to
//  extend the parser to work with headers that it does not know about.
//
//-- Warning to developpers:
//--  This enum is used to index the g_astMimeHeaderDescriptor table of header
//--  descriptors. That table MUST stay in sync with this enum.
enum EMimeHeaderType
{
    // Description:
    // RFC 2045 (Multipurpose Internet Mail Extensions (MIME) Part One) headers.
    eHDR_MIME_CONTENT_DESCRIPTION,

    // Description:
    // RFC 2183 (The Content-Disposition Header Field).
    eHDR_MIME_CONTENT_DISPOSITION,

    // Description:
    // RFC 2616 (HTTP) headers.
    eHDR_MIME_CONTENT_ENCODING,

    // Description:
    // RFC 2045 (Multipurpose Internet Mail Extensions (MIME) Part One) headers.
    eHDR_MIME_CONTENT_ID,

    // Description:
    // RFC 3282 (Content Language Headers) headers.
    eHDR_MIME_CONTENT_LANGUAGE,

    // Description:
    // RFC 2616 (HTTP) headers.
    eHDR_MIME_CONTENT_LENGTH,

    // Description:
    // RFC 2045 (Multipurpose Internet Mail Extensions (MIME) Part One) headers.
    eHDR_MIME_CONTENT_TRANSFER_ENCODING,

    // Description:
    // RFC 2045 (Multipurpose Internet Mail Extensions (MIME) Part One) headers.
    eHDR_MIME_CONTENT_TYPE,

    // Description:
    // This is an application defined header (not related to any known RFC or
    // draft). It is used when we don't recognize a header. This enum must be
    // kept last. It maps to the RFC3261 extension-header construct.
    eHDR_MIME_EXTENSION,

    //M5T_INTERNAL_USE_BEGIN
    // Description:
    // This is the last item, as thus it must be kept last. It is mainly use to
    // get the size of g_astMimeHeaderDescriptor.
    eHDR_MIME_COUNT
    //M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_MIMEHEADERGRAMMAR_H

