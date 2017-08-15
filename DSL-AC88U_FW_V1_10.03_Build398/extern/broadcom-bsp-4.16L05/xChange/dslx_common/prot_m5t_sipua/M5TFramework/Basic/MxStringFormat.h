//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_MXSTRINGFORMAT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXSTRINGFORMAT_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
    #include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
    #include "Config/FrameworkCfg.h"
#endif

// This file has to be protected so it is not included when not needed/supported.
// The protection makes sure that all the services that need it are disabled.
#if defined(MXD_CAP_ENABLE_SUPPORT) || \
    defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW) || \
    !defined(MXD_OS_NONE) || \
    (defined(MXD_TRACE_ENABLE_SUPPORT) && \
     !defined(MXD_TRACE_FORMAT_HANDLER_OVERRIDE))

//==============================================================================
//====  FUNCTION PROTOTYPES  ===================================================
//==============================================================================

#ifdef __cplusplus
extern "C" {
#endif
// Summary:
//  Performs a comparison between two character strings.
int MxStringCaseCompareLength(IN const char *pszString1,
                              IN const char *pszString2,
                              IN int nLength);
#ifdef __cplusplus
}
#endif

// Summary:
//  Performs a comparison between two character strings.
int MxStringCaseCompare(IN const char *pszString1, IN const char *pszString2);

// Summary:
//  Writes into a buffer a formatted character string.
int MxVsnprintf(OUT char *pszBuffer, IN int nSize,
                IN const char *pszFormat, INOUT va_list args);

// Summary:
//  Writes into a buffer a formatted character string.
int MxSnprintf(OUT char *pszBuffer, IN int nSize,
               IN const char *pszFormat, ...);

// Summary:
//  Copies the system error message into the given buffer.
int MxStrErrorReentrant(OUT char *pszBuffer, IN int nSize, IN int nErrorNumber);

// Summary:
//  Converts an unsigned integer into a character string, in the given 'base'
//  representation.
char* MxUintToString(IN uint64_t uValue,
                     IN unsigned int uCapacity,
                     OUT char* pszBuffer,
                     OUT unsigned int* puSize,
                     IN EMxBase eBase = eBASE_DECIMAL);

// Summary:
//  Converts a signed integer into a character string, in the given 'base'
//  representation.
char* MxIntToString(IN int64_t nValue,
                    IN unsigned int uCapacity,
                    OUT char* pszBuffer,
                    OUT unsigned int* puSize,
                    IN EMxBase eBase = eBASE_DECIMAL);

// Summary:
//  Converts a character buffer into an unsigned integer, in the given 'base'
//  representation.
uint64_t MxStringToUint(IN const char* pszBuffer,
                        IN unsigned int uSize,
                        IN EMxBase eBase = eBASE_DECIMAL,
                        OUT mxt_result* pRes = NULL,
                        OUT unsigned int* puSizeParsed = NULL);

// Summary:
//  Converts a  null-terminated character string into an unsigned integer, in the
//  given 'base' representation.
uint64_t MxStringToUint(IN const char* pszBuffer,
                        IN EMxBase eBase = eBASE_DECIMAL,
                        OUT mxt_result* pRes = NULL,
                        OUT unsigned int* puSizeParsed = NULL);

// Summary:
//  Converts a character buffer into a signed integer, in the given 'base'
//  representation.
int64_t MxStringToInt(IN const char* pszBuffer,
                      IN unsigned int uSize,
                      IN EMxBase eBase = eBASE_DECIMAL,
                      OUT mxt_result* pRes = NULL,
                      OUT unsigned int* puSizeParsed = NULL);

// Summary:
//  Converts a  null-terminated character string into a signed integer, in the
//  given 'base' representation.
int64_t MxStringToInt(IN const char* pszBuffer,
                      IN EMxBase eBase = eBASE_DECIMAL,
                      OUT mxt_result* pRes = NULL,
                      OUT unsigned int* puSizeParsed = NULL);

// Summary:
//  Converts a hexadecimal value to its ASCII value in the ASCII chart.
unsigned int HexToAscii(IN const uint8_t* puHex,
                        IN unsigned int uHexSize,
                        OUT uint8_t* puAscii,
                        IN unsigned int uAsciiBufSize,
                        OUT mxt_result* pres = NULL);

// Summary:
//  Converts an ASCII value to its hexadecimal value in the ASCII chart.
unsigned int AsciiToHex(IN bool bConvertToUpper,
                        IN const uint8_t* puAscii,
                        IN unsigned int uAsciiSize,
                        OUT uint8_t* puHex,
                        IN unsigned int uHexBufSize);

// Summary:
//  Converts a string to a number of n bytes.
mxt_result MxStringToByteArrayNumber(IN const char* pszBuffer,
                               IN int nBase,
                               OUT uint8_t* puArray,
                               IN unsigned int uArraySize);

// Summary:
//  Converts a number of n bytes to a string.
int MxByteArrayNumberToString(IN uint8_t* puArray,
                        IN unsigned int uArraySize,
                        OUT char *pszBuffer,
                        IN int nSize,
                        IN int nBase);

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

#endif // #if defined(MXD_CAP_ENABLE_SUPPORT) ||
       //     defined(MXD_CRYPTO_UUIDGENERATOR_MITOSFW) ||
       //     !defined(MXD_OS_NONE) ||
       //     (defined(MXD_TRACE_ENABLE_SUPPORT) &&
       //      !defined(MXD_TRACE_FORMAT_HANDLER_OVERRIDE))

#endif // #ifndef MXG_MXSTRINGFORMAT_H


