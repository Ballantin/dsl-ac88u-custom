//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_TXTMESSAGETOKENS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_TXTMESSAGETOKENS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CTXTOKEN_H
#include "TextMessage/CTxtToken.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//<GROUP TEXT_MESSAGE_VARIABLES>
// Summary:
// A token to easily parse header names.
extern const CTxtToken::STokenCharacteristics g_stHEADER_NAME_TOKEN;

//<GROUP TEXT_MESSAGE_VARIABLES>
// Summary:
// A token to use with the eTHDT_STRING and eTHDT_FREESTRING header data type.
extern const CTxtToken::STokenCharacteristics g_stSTRING_TOKEN;

//<GROUP TEXT_MESSAGE_VARIABLES>
// Summary:
// A token to use in eTHDT_TOKEN based headers.
extern const CTxtToken::STokenCharacteristics g_stHEADER_VALUE_TOKEN;

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_TXTHEADERGRAMMAR_H

