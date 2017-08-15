//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
//#define MXD_XML_ENABLE_SUPPORT
#ifndef MXG_CREGINFO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CREGINFO_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_REGINFO_SUPPORT
#endif

#if !defined(MXD_SIPSTACK_ENABLE_REGINFO_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_REGINFO_SUPPORT to be able to use this class.
#endif

#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CNAMEADDR_H
#include "SipParser/CNameAddr.h"
#endif

#ifndef MXG_CGENPARAMLIST_H
#include "SipParser/CGenParamList.h"
#endif

#ifndef MXG_IXMLPARSERMGR_H
#include "Xml/IXmlParserMgr.h"
#endif

#ifndef MXG_IXMLWRITEROUTPUTHANDLER_H
#include "Xml/IXmlWriterOutputHandler.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class IXmlParser;
class IXmlGenericWriter;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//@@REGINFOVAR
//==SDOC========================================================================
//== File: CRegInfo.h
//==========================
// <TITLE RegInfo Constants>
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//   This file contains some constants that are used by CRegInfo.
//
// Location:
//   SipParser/CRegInfo.h
//
// See Also:
//   CReginfo
//
//==EDOC========================================================================

// <TITLE RFC 3680 reginfo state values>
// <GROUP REGINFOVAR>
//
// Description:
//  According to RFC 3680, there are two valid values for the state of the
//  reginfo: full and partial
//
//------------------------------------------------------------------------------
const char* const pszREGINFO_STATE_PARTIAL = "partial";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_STATE_PARTIAL>
const char* const pszREGINFO_STATE_FULL = "full";

// <TITLE RFC 3680 registration state values>
// <GROUP REGINFOVAR>
//
// Description:
//  According to RFC 3680, the state of the registration must be one of the
//  following: init, active or terminated.
//
//------------------------------------------------------------------------------
const char* const pszREGINFO_REGISTRATION_STATE_INIT = "init";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_REGISTRATION_STATE_INIT>
const char* const pszREGINFO_REGISTRATION_STATE_ACTIVE = "active";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_REGISTRATION_STATE_INIT>
const char* const pszREGINFO_REGISTRATION_STATE_TERMINATED = "terminated";

// <TITLE RFC 3680 contact state values>
// <GROUP REGINFOVAR>
//
// Description:
//  According to RFC 3680, there are two valid values for the state of the
//  contact: active and terminated.
//-----------------------------------------------------------------------------
const char* const pszREGINFO_CONTACT_STATE_ACTIVE = "active";
// <GROUP REGINFOVAR>
//<COMBINE pszREGINFO_CONTACT_STATE_ACTIVE>
const char* const pszREGINFO_CONTACT_STATE_TERMINATED = "terminated";

// <TITLE Reginfo: RFC 3680 contact event values>
// <GROUP REGINFOVAR>
//
// Description:
//  According to RFC 3680, the following events are those that can make a contact
//  change state.
//
//------------------------------------------------------------------------------
const char* const pszREGINFO_CONTACT_EVENT_REGISTERED = "registered";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_CONTACT_EVENT_REGISTERED>
const char* const pszREGINFO_CONTACT_EVENT_CREATED = "created";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_CONTACT_EVENT_REGISTERED>
const char* const pszREGINFO_CONTACT_EVENT_REFRESHED = "refreshed";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_CONTACT_EVENT_REGISTERED>
const char* const pszREGINFO_CONTACT_EVENT_SHORTENED = "shortened";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_CONTACT_EVENT_REGISTERED>
const char* const pszREGINFO_CONTACT_EVENT_EXPIRED = "expired";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_CONTACT_EVENT_REGISTERED>
const char* const pszREGINFO_CONTACT_EVENT_DEACTIVATED = "deactivated";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_CONTACT_EVENT_REGISTERED>
const char* const pszREGINFO_CONTACT_EVENT_PROBATION = "probation";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_CONTACT_EVENT_REGISTERED>
const char* const pszREGINFO_CONTACT_EVENT_UNREGISTERED = "unregistered";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_CONTACT_EVENT_REGISTERED>
const char* const pszREGINFO_CONTACT_EVENT_REJECTED = "rejected";

// <TITLE Reginfo: RFC 3680 and draft-ietf-sipping-gruu-reg-event-08 reginfo element names>
// <GROUP REGINFOVAR>
//
// Description:
//  The following element names are those supported by RFC 3680 and
//  draft-ietf-sipping-gruu-reg-event-08.
//
//------------------------------------------------------------------------------
const char* const pszREGINFO_ELEMENT_REGINFO = "reginfo";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ELEMENT_REGINFO>
const char* const pszREGINFO_ELEMENT_REGISTRATION = "registration";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ELEMENT_REGINFO>
const char* const pszREGINFO_ELEMENT_CONTACT = "contact";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ELEMENT_REGINFO>
const char* const pszREGINFO_ELEMENT_URI = "uri";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ELEMENT_REGINFO>
const char* const pszREGINFO_ELEMENT_UNKNOWNPARAM = "unknown-param";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ELEMENT_REGINFO>
const char* const pszREGINFO_ELEMENT_PUBGRUU = "pub-gruu";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ELEMENT_REGINFO>
const char* const pszREGINFO_ELEMENT_TEMPGRUU = "temp-gruu";

// <TITLE Reginfo: RFC 3680 and draft-ietf-sipping-gruu-reg-event-08 reginfo attribute names>
// <GROUP REGINFOVAR>
//
// Description:
//  The following attribute names are those supported by RFC 3680,
//  draft-ietf-sipping-gruu-reg-event-08 and RFC 5628.
//
//------------------------------------------------------------------------------
const char* const pszREGINFO_ATTRIBUTE_VERSION = "version";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_STATE = "state";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_XMLNS = "xmlns";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_XMLNSGR = "xmlns:gr";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_AOR = "aor";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_ID = "id";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_EVENT = "event";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_DURATIONREGISTERED = "duration-registered";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_RELATIVEPRIORITY = "q";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_EXPIRES = "expires";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_RETRY_AFTER = "retry-after";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_CALLID = "callid";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_CSEQ = "cseq";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_DISPLAYNAME = "display-name";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_NAME = "name";
// <GROUP REGINFOVAR>
// <COMBINE pszREGINFO_ATTRIBUTE_VERSION>
const char* const pszREGINFO_ATTRIBUTE_URI = "uri";

// <TITLE Reginfo: Invalid values for contact's attributes>
// <GROUP REGINFOVAR>
//
// Description:
//  Invalid values for contact's attributes.
//
//------------------------------------------------------------------------------
const unsigned int uREGINFO_CONTACT_INVALID_DURATION_REGISTERED = ~0U;
// <GROUP REGINFOVAR>
// <COMBINE uREGINFO_CONTACT_INVALID_DURATION_REGISTERED>
const unsigned int uREGINFO_CONTACT_INVALID_EXPIRES = ~0U;
// <GROUP REGINFOVAR>
// <COMBINE uREGINFO_CONTACT_INVALID_DURATION_REGISTERED>
const unsigned int uREGINFO_CONTACT_INVALID_RETRY_AFTER = ~0U;
// <GROUP REGINFOVAR>
// <COMBINE uREGINFO_CONTACT_INVALID_DURATION_REGISTERED>
const unsigned int uREGINFO_CONTACT_INVALID_CSEQ = ~0U;

// <TITLE Reginfo: RFC 3680 and draft-ietf-sipping-gruu-reg-event-08 reginfo namespaces>
// <GROUP REGINFOVAR>
//
// Description:
//  The following namespaces are those supported by RFC 3680 and
//  draft-ietf-sipping-gruu-reg-event-08.
//
//------------------------------------------------------------------------------
const char* const pszNAMESPACE_GRUU = "urn:ietf:params:xml:ns:gruuinfo";
const char* const pszNAMESPACE_REGINFO = "urn:ietf:params:xml:ns:reginfo";
const char* const pszNAMESPACE_GRUU_PREFIX = "gr";

//==============================================================================
//== Class: CReginfo
//==============================================================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This class parses, stores, and generates a reginfo document as per RFC 3680
//   and extended by draft-ietf-sipping-gruu-reg-event-08.
//
//   When parsing, this class ignores namespaces. When generating, this class
//   defines "urn:ietf:params:xml:ns:reginfo" as the default namespace.
//
//   If there is at least one "pub-gruu" or "temp-gruu" element defined, the
//   generator defines "urn:ietf:params:xml:ns:gruuinfo" as a namespace with
//   value 'gr'.
//
// Location:
//   SipParser/CReginfo.h
//
// See Also:
//   CRegistrationElement, CContactElement
//
//==============================================================================
class CReginfo : private IXmlParserMgr
{
//-- New types within class namespace
public:

    // Description:
    //  Contains the information about a contact.
    struct SContact
    {
        // Constructor.
        SContact() :
            m_uDurationRegisteredS(uREGINFO_CONTACT_INVALID_DURATION_REGISTERED),
            m_uExpiresS(uREGINFO_CONTACT_INVALID_EXPIRES),
            m_uRetryAfterS(uREGINFO_CONTACT_INVALID_RETRY_AFTER),
            m_uCSeq(uREGINFO_CONTACT_INVALID_CSEQ),
            m_pTempGruu(NULL),
            m_pPubGruu(NULL)
        {
        }

        // Destructor.
        ~SContact()
        {
            MX_DELETE(m_pTempGruu);
            m_pTempGruu = NULL;

            MX_DELETE(m_pPubGruu);
            m_pPubGruu = NULL;
        }

        // Validates the content of the Contact.
        bool IsValid() const
        {
            bool bIsValid = true;
            // Validates attributes that are always mandatory.
            if (m_strState == "" || m_strEvent == "" || m_strId == "")
            {
                bIsValid = false;
            }
            // Validates attributes that are sometimes mandatory.
            else if ((m_strEvent == pszREGINFO_CONTACT_EVENT_SHORTENED &&
                 m_uExpiresS == uREGINFO_CONTACT_INVALID_EXPIRES) ||
                (m_strEvent == pszREGINFO_CONTACT_EVENT_PROBATION &&
                 m_uRetryAfterS == uREGINFO_CONTACT_INVALID_RETRY_AFTER))
            {
                bIsValid = false;
            }
            // Validates elements that are always mandatory.
            else if (m_nameAddr.GetUri() == NULL)
            {
                bIsValid = false;
            }
            return bIsValid;
        }

        // Values can be "active" or "terminated" but this is not enforced by
        // this object. This parameter is mandatory.
        CString m_strState;
        // The event that caused the contact state machine to go into its
        // current state. The valid values are "created", "deactivated",
        // "expired", "probation", "refreshed", "registered", "rejected",
        // "shortened", or "unregistered", but this is not enforced by this
        // object. This parameter is mandatory.
        CString m_strEvent;
        // The amount of time that the contact has been bound to the
        // address-of-record, in seconds. This parameter is optional. To omit
        // it, set it to uREGINFO_CONTACT_INVALID_DURATION_REGISTERED, which is
        // also the default value.
        unsigned int m_uDurationRegisteredS;
        // The number of seconds remaining until the binding is due to expire.
        // This parameter is mandatory when the event is "shortened",
        // otherwise, it is optional when the state is "active" and finally, it
        // is unused otherwise. Note however that this object does not enforce
        // these rules. To omit it, set it to uREGINFO_CONTACT_INVALID_EXPIRES,
        // which is also the default value.
        unsigned int m_uExpiresS;
        // The amount of seconds after which the owner of the contact is
        // expected to retry its registration. This parameter is mandatory when
        // the event is "probation". Othewise, it is not used. Note however
        // that this object does not enforce these rules. To omit it, set it to
        // uREGINFO_CONTACT_INVALID_RETRY_AFTER, which is also the default
        // value.
        unsigned int m_uRetryAfterS;
        // The id for that contact. It should be the same across notification if
        // the URI is the same. This parameter is mandatory.
        CString m_strId;
        // The q parameter of the contact. This parameter is optional and is
        // not used when empty. This parameter should be a float but this rule
        // is not enforced by this object.
        CString m_strQ;
        // The current Call-ID carried in the REGISTER that was last used to
        // update this contact. This parameter is optional and is not used when
        // empty.
        CString m_strCallId;
        // The last CSeq value present in a REGISTER request that updated this
        // contact value. This parameter is optional. To omit it, set it to
        // uREGINFO_CONTACT_INVALID_CSEQ, which is also the default value.
        unsigned int m_uCSeq;
        // The name addr associated with that contact. The URI part of the name
        // addr is the "uri" element and the display-name part is the
        // "display-name" element. If the display-name is empty, the
        // "display-name" element is omitted.
        CNameAddr m_nameAddr;
        // The Instance ID associated with this contact. This parameter is
        // optional and is not used when empty. It corresponds to an
        // "unknown-param" element with a 'name' attribute of "+sip.instance".
        CString m_strInstanceId;
        // Temporary GRUU associated with the combination of the AOR and the
        // Instance ID. This parameter is optional and MAY be included only
        // when the instance-id is included. It corresponds to a "temp-gruu"
        // element of the contact. The structure has the ownership of this
        // object. When the pointer is NULL, the element is not part of the
        // contact element.
        IUri* m_pTempGruu;
        // The public GRUU associated with the combination of the AOR and the
        // Instance ID. This parameter is optional and SHOULD be included only
        // when the instance-id is included. It corresponds to a "pub-gruu"
        // element of the contact. The structure has the ownership of this
        // object. When the pointer is NULL, the element is not part of the
        // contact element.
        IUri* m_pPubGruu;
        // The unknown parameters associated with the contact. There can be 0
        // or more unknown parameters. These parameters each correspond to one
        // "unknown-param" element with the parameter name as the "name"
        // attribute and the parameter value as the element value. Note that
        // the CGenParamList is used solely as a container and that it cannot
        // be used to parse and generate the XML elements. Also note that the
        // elements already explicitely mentionned in this structure (like 'q'
        // or "+sip.instance") should not be duplicated in this list.
        CGenParamList m_unknownParams;
    };

    // Description:
    //  Contains the information about a registration.
    struct SRegistration
    {
        // Constructor.
        SRegistration() :
            m_pAor(NULL)
        {
        }

        // Destructor.
        ~SRegistration()
        {
            MX_DELETE(m_pAor);
            m_pAor = NULL;

            while(!m_vecpstContacts.IsEmpty())
            {
                SContact* pstContact = m_vecpstContacts.GetAt(0);
                m_vecpstContacts.Erase(0);

                MX_DELETE(pstContact);
                pstContact = NULL;
             }
        }

        // Validates the content of the Registration.
        bool IsValid() const
        {
            bool bIsValid = true;
            // Validates mandatory elements and attributes.
            if (m_pAor == NULL   ||
                m_strId == ""    ||
                m_strState == "" ||
                m_vecpstContacts.GetSize() == 0)
            {
                bIsValid = false;
            }
            else
            {
                unsigned int i = 0;
                unsigned int uSize = m_vecpstContacts.GetSize();
                // Validates contacts.
                for (; i < uSize; i++)
                {
                    bIsValid = m_vecpstContacts.GetAt(i)->IsValid();
                    if (bIsValid == false)
                    {
                        i = uSize;
                    }
                }
            }
            return bIsValid;
        }

        // The URI, which is the address-of-record this registration refers to.
        // This is the "aor" attribute of the "registration" element. This
        // parameter is mandatory. The structure has the ownership of this
        // object.
        IUri* m_pAor;
        // The id attribute identifies this registration. This parameter is
        // mandatory.
        CString m_strId;
        // Indicates the state of the registration. The valid values are
        // "init", "active", and "terminated" but this is not enforced by this
        // object. This parameter is mandatory.
        CString m_strState;
        // The registered contacts. There can be any number of contacts
        // (including 0) and they each correspond to one "contact" element.
        CVector<SContact*> m_vecpstContacts;
    };

protected:
private:

    // Position meaning for the bits in the m_bsState bitset.
    enum EState
    {
        // This is set when the document is not a valid registration document.
        ePARSER_IN_ERROR = 0x0001,
        // This is set when a reginfo start element is found.
        eREGINFO_OPENED = 0x0002,
        // This is set when a registration start element is found.
        eREGISTRATION_OPENED = 0x0004,
        // This is set when a contact start element is found.
        eCONTACT_OPENED = 0x0008,
        // This is set when a uri start element is found.
        eURI_OPENED = 0x0010,
        // This is set when a unknown-param start element is found.
        eUNKNOWNPARAM_OPENED = 0x0020,
        // This is set when an unknown-param element with a name of
        // +sip.instance is found.
        eSIPINSTANCE_OPENED = 0x0040,
        // This is set when a pub-gruu start element is found.
        ePUBGRUU_OPENED = 0x0080,
        // This is set when a temp-gruu start element is found.
        eTEMPGRUU_OPENED = 0x0100
    };

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CReginfo();

    // Summary:
    //  Destructor.
    virtual ~CReginfo();

  	// Summary:
    //  Resets internal settings.
	void Reset();

    // Summary:
    //  Parses a reginfo XML document.
    mxt_result Parse(IN unsigned int uDataSize,
                     INOUT const uint8_t* puBuffer);

    // Summary:
    //  Parses a reginfo XML document.
    mxt_result Parse(IN const CBlob& rDocument);

    // Summary:
    //  Gets the version attribute.
    unsigned int GetVersion() const;

    // Summary:
    //  Returns the state attribute for this reginfo document.
    const char* GetState() const;

    // Summary:
    //  Gets the number of registration.
    unsigned int GetRegistrationCount() const;

    // Summary:
    //  Gets the specified registration element.
    const SRegistration* GetRegistration(IN unsigned int uIdx) const;

    // Summary:
    //  Gets the specified registration element.
    SRegistration* GetRegistration(IN unsigned int uIdx);

    // Summary:
    //  Sets the version attribute.
    void SetVersion(IN unsigned int uVersion);

    // Summary:
    //  Sets the state attribute for this reginfo document.
    void SetState(IN const char* pszState);

    // Summary:
    //  Appends a registration element.
    void AppendRegistration(IN TO SRegistration* pstRegistration);

    // Summary:
    //  Outputs the reginfo to the blob.
    mxt_result Serialize(INOUT CBlob& rBlob);


//-- Hidden Methods
protected:
private:
    //-- << IXmlParserMgr interface >>

    // Summary:
    //  Reports that the start of an element was detected.
    void EvStartElement(IN const char* pszNamespaceUri,
                        IN const char* pszElementName,
                        IN const char** ppAttributes,
                        IN const char** ppNewNamespaces = NULL);

    // Summary:
    //  Reports that the end of an element was detected.
    void EvEndElement(IN const char* pszNamespaceUri,
                      IN const char* pszElementName);

    // Summary:
    //  Reports that character data was detected.
    void EvCharacterData(IN const char* pcText,
                         IN const unsigned int uTextSize);

    //-- << Parse helper methods >>
    // Summary:
    //  Parses a reginfo element.
    void ParseReginfoElement(IN const char** ppAttributes);

    // Summary:
    //  Parses a registration element.
    void ParseRegistrationElement(IN const char** ppAttributes);

    // Summary:
    //  Parses a contact element.
    void ParseContactElement(IN const char** ppAttributes);

    // Summary:
    //  Parses an unknown-param element.
    void ParseUnknownParamElement(IN const char** ppAttributes);

    // Summary:
    //  Parses a pub-gruu element.
    void ParsePubGruuElement(IN const char** ppAttributes);

    // Summary:
    //  Parses a temp-gruu element.
    void ParseTempGruuElement(IN const char** ppAttributes);


    //-- << Bitset methods >>
    // Summary:
    //  Sets the state in the bitset.
    void SetState(IN EState eState);

    // Summary:
    //  Checks if the state is set.
    bool IsStateSet(IN EState eState) const;

    // Summary:
    //  Resets a state.
    void ResetState(IN EState eState);


    //-- << Helper methods >>
    // Summary:
    //  Gets the currently parsed registration.
    SRegistration* GetCurrentRegistration() const;

    // Summary:
    //  Gets the currently parsed contact.
    SContact* GetCurrentContact() const;

    // Summary:
    //  Deletes every registration owned by this element.
    void DeleteRegistrations();


    //-- << Serialization helper methods >>
    // Summary:
    //  Serializes the registrations.
    mxt_result SerializeRegistrations(IN IXmlGenericWriter* pXmlWriter) const;

    // Summary:
    //  Serializes the contacts.
    mxt_result SerializeContacts(IN const SRegistration* pstRegistration,
                                 IN IXmlGenericWriter* pXmlWriter) const;

    // Summary:
    //  Returns the XML string corresponding to the URI.
    void GetIUriXmlString(IN const IUri* pUri,
                          OUT CString& rstrUri) const;


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CReginfo(IN const CReginfo& rSrc);

    // Summary:
    //  Assignment Operator.
    CReginfo& operator=(IN const CReginfo& rSrc);


//-- Hidden Data Members
protected:
private:

    // Version of the reginfo document.
    unsigned int m_uVersion;

    // State of the reginfo document.
    CString m_strState;

    // List of Registrations of the reginfo document.
    CVector<SRegistration*> m_vecpstRegistrations;

    // Bit set holding the state of te parsing.
    uint16_t m_bsState;

    // The XmlParser used to parse the documents.
    IXmlParser* m_pXmlParser;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  Parse
//==
//==============================================================================
//
//  Parameters:
//      rDocument:
//          The document held within a blob object.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT: The data is not a valid XML document.
//
//      - resFE_FAIL: The data is not a valid reginfo document.
//
//      - resS_OK: Data successfully parsed.
//
//  Description:
//      Parses the reginfo XML document. The parsing follows RFC 3680 and
//      draft-ietf-sipping-gruu-reg-event-08. The comments, the unknown
//      elements, and the unknown attributes are ignored. Note that the
//      "unknown-param" element is NOT an unknow element.
//
//==EDOC========================================================================
inline mxt_result CReginfo::Parse(IN const CBlob& rDocument)
{
    return Parse(rDocument.GetSize(),
                 rDocument.GetFirstIndexPtr());
}

//==SDOC========================================================================
//==
//==  GetVersion
//==
//==============================================================================
//
//  Returns:
//      The version attribute.
//
//  Description:
//      Gets the version of the reginfo document. This version number starts
//      at 0 and is incremented by 1 each time a new reginfo document is
//      created inside a subscription. It enables the subscriber to properly
//      order the received reginfo document and detect when there is a gap
//      between two partial documents so the subscriber can request a new
//      full document.
//
//      This attribute is mandatory and is always output in generated
//      reginfo documents.
//
//      This attribute defaults to 0.
//
//==EDOC========================================================================
inline unsigned int CReginfo::GetVersion() const
{
    return m_uVersion;
}

//==SDOC========================================================================
//==
//==  GetState
//==
//==============================================================================
//
//  Returns:
//      The state attribute. It can be NULL when not set.
//
//  Description:
//      Returns the state of this reginfo document. The state indicates
//      whether the document contains the full registration state or only
//      information on those registrations that have changed since the previous
//      document (partial).
//
//      This attribute is mandatory. It is always output in generated
//      reginfo documents.
//
//==EDOC========================================================================
inline const char* CReginfo::GetState() const
{
    return m_strState.CStr();
}

//==SDOC========================================================================
//==
//==  SetState
//==
//==============================================================================
//
//  Parameters:
//    szState:
//      The state attribute.
//
//  Description:
//      Sets the state of this reginfo document. The state indicates
//      whether the document contains the full registration state or only
//      information on those registrations that have changed since the previous
//      document (partial).
//
//      The allowed values as per RFC 3680 are "partial" and "full" but this
//      method does not enforce it.
//
//      This attribute is mandatory. It is always output in generated
//      reginfo documents.
//
//==EDOC========================================================================
inline void CReginfo::SetState(IN const char* pszState)
{
    m_strState = pszState;
}

//==SDOC========================================================================
//==
//==  GetRegistrationCount
//==
//==============================================================================
//
//  Returns:
//      The number of registrations.
//
//  Description:
//      Gets the number of registrations in this reginfo XML document.
//
//==EDOC========================================================================
inline unsigned int CReginfo::GetRegistrationCount() const
{
    return m_vecpstRegistrations.GetSize();
}

//==SDOC========================================================================
//==
//==  SetVersion
//==
//==============================================================================
//
//  Parameters:
//    uVersion:
//      The version attribute.
//
//  Description:
//      Sets the version of the reginfo document. This version number starts
//      at 0 and is incremented by 1 each time a new reginfo document is
//      created inside a subscription. It enables the subscriber to properly
//      order the received reginfo document and detect when there is a gap
//      between two partial documents so the subscriber can request a new
//      full document.
//
//      This attribute is mandatory and is always output in generated
//      reginfo documents.
//
//      This attribute defaults to 0.
//
//==EDOC========================================================================
inline void CReginfo::SetVersion(IN unsigned int uVersion)
{
    m_uVersion = uVersion;
}

//==SDOC========================================================================
//==
//==  SetState
//==
//==============================================================================
//
//  Parameters:
//    eState:
//      The state to set in the bitset.
//
//  Description:
//      Sets the state in the bitset (m_bsState). There is no check to see
//      whether or not the state is already set.
//
//==EDOC========================================================================
inline void CReginfo::SetState(IN EState eState)
{
    m_bsState = (m_bsState | static_cast<uint16_t>(eState));
}

//==SDOC========================================================================
//==
//==  IsStateSet
//==
//==============================================================================
//
//  Parameters:
//    eState:
//      The state to check.
//
//  Returns:
//      true if the state is set, false otherwise.
//
//  Description:
//      Checks whether or not the state is set in the bitset (m_bsState).
//
//==EDOC========================================================================
inline bool CReginfo::IsStateSet(IN EState eState) const
{
    return (m_bsState & static_cast<uint16_t>(eState)) != 0;
}

//==SDOC========================================================================
//==
//==  ResetState
//==
//==============================================================================
//
//  Parameters:
//    eState:
//      The state to reset.
//
//  Description:
//      Resets the state in the bitset (m_bsState). There is no check to see
//      whether or not the state is set.
//
//==EDOC========================================================================
inline void CReginfo::ResetState(IN EState eState)
{
    m_bsState = (m_bsState & (~ static_cast<uint16_t>(eState)));
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CREGINFO_H
