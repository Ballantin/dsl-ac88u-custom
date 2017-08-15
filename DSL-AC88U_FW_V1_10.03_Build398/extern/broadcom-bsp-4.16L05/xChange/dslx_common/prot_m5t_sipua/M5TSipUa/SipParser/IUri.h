//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_IURI_H
#define MXG_IURI_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT
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
class CBlob;
class CString;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//@@IURIVAR
//==SDOC========================================================================
//== File: IUri.h
//==========================
// <TITLE IUri Constants>
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//   This file contains some constants that are used by IUri.
//
// Location:
//   SipParser/IUri.h
//
// See Also:
//   IUri
//
//==EDOC========================================================================

// <GROUP IURIVAR>
//
// Description:
//     RFC3261 "sip" URI scheme name. See <LINK IUri::EUriType::eSIP>.
//---------------------------------------------------------------------
const char* const szURISCHEME_SIP  = "sip";

// <GROUP IURIVAR>
//
// Description:
//     RFC3261 "sips" URI scheme name. See <LINK IUri::EUriType::eSIPS>.
//-----------------------------------------------------------------------
const char* const szURISCHEME_SIPS = "sips";

// <GROUP IURIVAR>
//
// Description:
//     RFC3966 "tel" URI scheme name. See <LINK IUri::EUriType::eTEL>.
//---------------------------------------------------------------------
const char* const szURISCHEME_TEL = "tel";

#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)
// <GROUP IURIVAR>
//
// Description:
//     RFC 3860 "im" URI scheme name. See <LINK IUri::EUriType::eIM>.
//---------------------------------------------------------------------
const char* const szURISCHEME_IM = "im";

// <GROUP IURIVAR>
//
// Description:
//     RFC 3859 "pres" URI scheme name. See <LINK IUri::EUriType::ePRES>.
//---------------------------------------------------------------------
const char* const szURISCHEME_PRES = "pres";
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IUri
//========================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The IUri is the base class for all URI types that are supported by the SIP
//   stack. It contains basic operations that all URI-abstracting classes should
//   implement.
//
//   The IUri interface is used in the SIP stack everywhere a URI can be used.
//   For example, the request-line would be abstracted with a IUri.
//
//  <CODE>
//    Uses of IUri in the SIP stack:
//      RFC3261 ABNF:
//          Request-URI =  SIP-URI / SIPS-URI / TEL-URI / absoluteURI /
//                         IM-URI / PRES-URI
//
//          Alert-Info  =  "Alert-Info" HCOLON alert-param *(COMMA alert-param)
//          alert-param =  LAQUOT absoluteURI RAQUOT *( SEMI generic-param )
//
//          Call-Info   =  "Call-Info" HCOLON info *(COMMA info)
//          info        =  LAQUOT absoluteURI RAQUOT *( SEMI info-param)
//
//          name-addr   =  [ display-name ] LAQUOT addr-spec RAQUOT
//          addr-spec   =  SIP-URI / SIPS-URI / TEL-URI / absoluteURI
//
//          Error-Info  =  "Error-Info" HCOLON error-uri *(COMMA error-uri)
//          error-uri   =  LAQUOT absoluteURI RAQUOT *( SEMI generic-param )
//
//          Proxy-Authenticate  =  "Proxy-Authenticate" HCOLON challenge
//          challenge           =  ("Digest" LWS digest-cln *(COMMA digest-cln))
//                                   / other-challenge
//          other-challenge     =  auth-scheme LWS auth-param
//                                   *(COMMA auth-param)
//          digest-cln          =  realm / domain / nonce
//                                    / opaque / stale / algorithm
//                                    / qop-options / auth-param
//          realm               =  "realm" EQUAL realm-value
//          realm-value         =  quoted-string
//          domain              =  "domain" EQUAL LDQUOT URI
//                                   *( 1*SP URI ) RDQUOT
//          URI                 =  absoluteURI / abs-path
//  </CODE>
//
// Location:
//   SipParser/IUri.h
//
// See Also:
//   CUriFactory
//
//==EDOC========================================================================
class IUri
{
//-- New types definition
//------------------------
public:

    // All URI types must be listed in this enum. It is required because of the
    // IsEquivalent method.
    //--------------------------------------------------------------------------
    enum EUriType
    {
        // Any RFC2396/RFC3261-compliant URI that the stack does not support
        // explicitely, for example http or mail.
        //-------------------------------------------------------------------
        eABSOLUTE,

        // RFC3261 "sip" URI.
        //--------------------
        eSIP,

        // RFC3261 "sips" URI.
        //---------------------
        eSIPS,

        // RFC3966 "tel" URI.
        //--------------------
        eTEL,

        // RFC2822 mailbox URI.
        eMAILBOX,

#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)

        // RFC3860 "im" mailbox type URI.
        //--------------------------------
        eIM,

        // RFC3859 "pres" mailbox type URI.
        //----------------------------------
        ePRES,
#endif
    };

    // Enum to use in parameter to the IUri::Parse method. Specifies if the URI
    // can accept the characters '?', ',' or ';'. In most cases, this
    // corresponds to the URI being within angle quotes or not.
    //--------------------------------------------------------------------------
    enum ESpecialCharactersAllowed
    {
        // The URI CANNOT contain commas, semi-colons and question marks.
        //----------------------------------------------------------------
        eDISALLOW_SPECIAL_CHARS,

        // The URI can contain commas, semi-colons and question marks.
        //-------------------------------------------------------------
        eALLOW_SPECIAL_CHARS,
    };


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUri(){};

    //-- << Interface >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  GenerateCopy
    //==
    //==========================================================================
    //
    //  Summary:
    //      Generates a copy of this URI.
    //
    //  Returns:
    //      The new IUri child class copy. Ownership of this instance is
    //      given to the caller.
    //
    //  Description:
    //      Allocates a new instance of the current URI type and copies its
    //      content into it. The use of this method is that users of the IUri
    //      class are able to create copies of their URIs by using a pointer
    //      to the base class (IUri) rather than having to typecast it.
    //
    //==EDOC====================================================================
    virtual GO IUri* GenerateCopy() const = 0;


    //==SDOC====================================================================
    //==
    //==  GetScheme
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the URI's scheme.
    //
    //  Returns:
    //      URI's scheme.
    //
    //  Description:
    //      Returns the URI's scheme.
    //
    //  See Also:
    //      SetScheme
    //
    //==EDOC====================================================================
    virtual const char* GetScheme() const = 0;

    //==SDOC====================================================================
    //==
    //==  GetUriType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the URI type.
    //
    //  Returns:
    //      URI type.
    //
    //  Description:
    //      Returns the URI type. This method is required because of the
    //      IsEquivalent method.
    //
    //==EDOC====================================================================
    virtual EUriType GetUriType() const = 0;

    //==SDOC====================================================================
    //==
    //==  IsEquivalent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Compares the given URI with this instance by using applicable RFC
    //      rules.
    //
    //  Parameters:
    //    rSrc:
    //      The URI against which to verify the equivalence.
    //
    //  Returns:
    //      Returns true if rSrc is an equivalent URI following comparison
    //      methods underlined in RFC 3261 or the appropriate RFC for the
    //      implementing URI type.
    //
    //  Description:
    //      Compares the given URI with this instance by using applicable RFC
    //      rules.
    //
    //==EDOC====================================================================
    virtual bool IsEquivalent(IN const IUri& rSrc) const = 0;

    //==SDOC====================================================================
    //==
    //==  Parse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Parses a byte string into usable data.
    //
    //  Parameters:
    //    eAllowSpecials:
    //      A value of IUri::eALLOW_SPECIAL_CHARS indicates that the URI can
    //      consider the comma ',', question mark '?', or semi-colon ';'
    //      characters as part of this URI. This is as per RFC 3261 conformance
    //      item {811} (saying that URIs that contain one of these characters
    //      MUST be within angle quotes). This restriction extends to all
    //      (name-addr | addr-spec) constructs (including SIP extensions).
    //
    //    rpcPos:
    //      Position from which to start. rpcPos initially points after the 
    //      scheme and colon. rpcPos is adjusted as parsing progresses. If 
    //      Parse() fails, rpcPos points to the position that caused the 
    //      failure. URIs MUST NOT skip trailing LWS.
    //
    //  Returns:
    //      See the child implementation for details.
    //
    //  Description:
    //      Parses a URI.
    //
    //==EDOC====================================================================
    virtual mxt_result Parse( IN ESpecialCharactersAllowed eAllowSpecials,
                              INOUT const char*& rpcPos ) = 0;


    //==SDOC====================================================================
    //==
    //==  Reset
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reinitializes the instance.
    //
    //  Description:
    //      Reinitializes the instance.
    //
    //==EDOC====================================================================
    virtual void Reset() = 0;


    //==SDOC====================================================================
    //==
    //==  Serialize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Outputs the data members in a format that is ready to be sent on the
    //      network.
    //
    //  Parameters:
    //    rBlob:
    //      CBlob into which the URI is input.
    //
    //  Description:
    //      Outputs the contents of the URI. This method outputs the scheme and
    //      the following COLON, then the body of the URI. Angle brackets are
    //      *NOT* added by URIs.
    //
    //==EDOC====================================================================
    virtual void Serialize( INOUT CBlob& rBlob ) const = 0;


//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Default Constructor.
    //-----------------------
    IUri(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUri(IN const IUri& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUri& operator=(IN const IUri& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IURI_H

