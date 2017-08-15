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
#ifndef MXG_CPRESURI_H
#define MXG_CPRESURI_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)

//-- Data Members.
#ifndef MXG_CHOSTPORT_H
#include "SipParser/CHostPort.h"
#endif

//-- Interface Realized and/or Parent.
#ifndef MXG_CMAILBOXURI_H
#include "SipParser/CMailboxUri.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CBlob;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CPresUri
//======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The CPresUri class is used to store, parse, and serialize PRES Mailbox URIs.
//   There are only two mailbox URI supported right now, IUri::eIM and
//   IUri::ePRES.
//
//   A PRES Mailbox URI is made up of an addr-spec, and an optional header list.
//
//  <CODE>
//    RFC 3859 PRES ABNF:
//    (The only difference with the normal ABNF is that the to part is mandatory)
//       PRES-URI       = "pres:" to [ headers ]
//       to             =  mailbox
//
//       headers        =  "?" header *( "&" header )
//       header         =  hname "=" hvalue
//       hname          =  1*( hnv-unreserved / unreserved / escaped )
//       hvalue         =  *( hnv-unreserved / unreserved / escaped )
//       hnv-unreserved =  "[" / "]" / "/" / "?" / ":" / "+" / "$"
//  </CODE>
//
// Location:
//   SipParser/CPresUri.h
//
// See Also:
//   CNameAddr, CMailboxUri
//
//==============================================================================
class CPresUri : public CMailboxUri
{
//-- New types definition.
public:

//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CPresUri();

    // Summary:
    //  Copy constructor.
    CPresUri(IN const CPresUri& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CPresUri();


    //-- << IUri >>

    // Summary:
    //  Generates a copy of this URI.
    virtual GO IUri* GenerateCopy() const;

    // Summary:
    //  Returns the URI's scheme.
    virtual const char* GetScheme() const;

    // Summary:
    //  Returns the URI type.
    virtual EUriType GetUriType() const;

    // Summary:
    //  Parses a byte string into useable data.
    virtual mxt_result Parse(IN ESpecialCharactersAllowed eAllowSpecials,
                             INOUT const char*& rpcPos);

    // Summary:
    //  Outputs the data member in a format that is ready to send on the
    //  network.
    virtual void Serialize(INOUT CBlob& rBlob) const;


//-- Hidden Methods.
protected:
    //-- << Helpers >>

private:

//-- Hidden Data Members.
protected:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetScheme
//==
//==============================================================================
//
//  Returns:
//      URI Scheme.
//
//  Description:
//      Returns the scheme.
//
//==============================================================================
inline const char* CPresUri::GetScheme() const
{
    return szURISCHEME_PRES;
}

//==============================================================================
//==
//==  GetUriType
//==
//==============================================================================
//
//  Returns:
//      URI type.
//
//  Description:
//      Returns the URI type. This method is required because of the
//      IsEquivalent method.
//
//==============================================================================
inline IUri::EUriType CPresUri::GetUriType() const
{
    return IUri::ePRES;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CPRESURI_H
