//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_AUTHENTICATION_H
#define MXG_AUTHENTICATION_H

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
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;


//==SDOC========================================================================
//== File: Authentication.h
//===========================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This file contains SIP authentication routines. The only supported
//   authentication scheme is Digest. The only supported algorithm is MD5
//   (including MD5-sess). Refer to RFC 2617 - HTTP Authentication- Basic and
//   Digest Access Authentication.txt, as well as to
//   draft-smith-sip-auth-examples-00.txt for more information on SIP Digest
//   authentication.
//
//   This file contains numerous methods required to calculate a MD5 checksum.
//   Most of these methods are of no use to external users. The method
//   MxCalculateMd5RequestDigest() is the only appropriate interface that
//   external users may find useful.
//
// Location:
//   SipParser/Authentication.h
//
//==EDOC========================================================================

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
const char* const szDIGEST   = "Digest";

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//====  PUBLIC METHOD PROTOTYPES  ==============================================
//==============================================================================

// Summary:
//  This method calculates a response by using the required authentication
//  parameters and algorithms as per RFC 2617.
//-------------------------------------------------------------------------
mxt_result
MxCalculateMd5RequestDigest( IN const CString& rstrMethod,
                             IN const CString& rstrUnqUsername,
                             IN const CString& rstrUnqRealm,
                             IN const CString& rstrPassword,
                             IN const CString& rstrDigestUriValue,
                             IN const CString& rstrUnqQopValue,
                             IN const CString& rstrAlgorithm,
                             OUT CString& rstrQuotedDigest,
                             IN const CString& rstrUnqNonceValue = "",
                             IN const CString& rstrNcValue = "",
                             IN const CString& rstrUnqCNonceValue = "",
                             IN const CBlob* pEntityBody = NULL );

//==============================================================================
//====  HELPER METHOD PROTOTYPES  ==============================================
//==============================================================================

// Summary:
//  This method calculates an MD5 checksum on the provided data.
//---------------------------------------------------------------
mxt_result MxCalculateMd5Checksum( IN const uint8_t* puInputData,
                                   IN unsigned int uDataLen,
                                   INOUT CString& rstrOutput );

// Summary:
//  This method creates the A1 string as per RFC 2617. It is to be used when
//  the "algorithm" parameter's value is "MD5" or unspecified.
//---------------------------------------------------------------------------
void MxCreateMd5A1( IN const CString& rstrUnqUsername,
                    IN const CString& rstrUnqRealm,
                    IN const CString& rstrPassword,
                    OUT CString& rstrMd5A1 );

// Summary:
//  This method calculates the A1 checksum as per RFC 2617. It is to be used
//  when the "algorithm" parameter's value is "MD5-sess".
//---------------------------------------------------------------------------
mxt_result MxCalculateMd5SessA1( IN const CString& rstrUnqUsername,
                                 IN const CString& rstrUnqRealm,
                                 IN const CString& rstrPassword,
                                 IN const CString& rstrDigestUriValue,
                                 IN const CString& rstrUnqNonceValue,
                                 IN const CString& rstrUnqCNonceValue,
                                 OUT CString& rstrMd5SessA1 );

// Summary:
//  This method creates the A2 string as per RFC 2617. It is to be used when
//  the "qop" parameter's value is "auth" or unspecified.
//---------------------------------------------------------------------------
void MxCreateAuthA2( IN const CString& rstrMethod,
                     IN const CString& rstrDigestUriValue,
                     OUT CString& rstrAuthA2 );

// Summary:
//  This method calculates the A2 checksum as per RFC 2617. It is to be used
//  when the "qop" parameter's value is "auth-int".
//---------------------------------------------------------------------------
mxt_result MxCalculateAuthIntA2( IN const CString& rstrMethod,
                                 IN const CString& rstrDigestUriValue,
                                 IN const CBlob* pEntityBody,
                                 OUT CString& rstrAuthIntA2 );

//  Summary:
//    Retrieves the MD5 hash of A1 when the MD5 algorithm is used.
//-----------------------------------------------------------------
mxt_result MxGetMd5HashA1(IN  const CString& rstrUsername,
                          IN  const CString& rstrRealm,
                          IN  const CString& rstrPassword,
                          OUT CString&       rstrHashA1);

// Summary:
//  Removes quotation marks from a string.
//-----------------------------------------
void RemoveQuotationMarks(INOUT CString& rstrString);

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_AUTHENTICATION_H

