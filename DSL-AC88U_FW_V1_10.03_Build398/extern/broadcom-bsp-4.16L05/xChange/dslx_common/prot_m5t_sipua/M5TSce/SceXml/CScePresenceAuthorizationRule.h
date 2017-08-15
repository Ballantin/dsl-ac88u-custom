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
#ifndef MXG_CSCEPRESENCEAUTHORIZATIONRULE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEPRESENCEAUTHORIZATIONRULE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSCEXMLHELPER_H
#include "SceXml/CSceXmlHelper.h"
#endif

#ifndef MXG_UAIMPPXMLDICTIONARY_H
#include "SceXml/UaImppXmlDictionary.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CTime;
class CXmlElement;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CScePresenceAuthorizationRule
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Description:
//  The CScePresenceAuthorizationRule class implements the XML management of the
//  definition of a single rule item, as defined in RFC 4745. This class also
//  implements the RFC 5025 extension which allows a user to control which
//  watchers have access to their presence information, and which part of this
//  presence information is made public or kept private.
//
//  The CScePresenceAuthorizationRule is a simple wrapper that can be created
//  from a CXmlElement returned by the IScePresenceAuthorizationDocument. It
//  makes it easy to manipulate presence authorization rules. Once configured,
//  these rules can then be sent to a presence server which will use them to
//  authorize access to the presence information.
//
//  As defined in RFC 4745, a rule is composed of conditions, actions and
//  transformations.
//
//#Conditions#
//  Conditions are used to match an incoming request with a rule.
//  There are three types of conditions defined.
//    - Identity: This is a condition based on the identity of the watcher
//      The identity condition can specify multiple user identity or even
//      domains, along with exceptions.
//    - Shpere: This condition is based on the \<sphere\> element of user's
//      presence, which identifies the current state and role that the user
//      currently plays. Good examples are "work" and "home". Note that this is
//      based on the sphere of the presence document that is being accessed.
//      More than one sphere condition can be set.
//    - Validity: This condition is used to assign validity periods to rules.
//      This can only be fixed, absolute times and the condition cannot represent
//      periodicity. More than one validity period can be assigned to a validity
//      condition.
//
//  In order for a request to match a rule, all conditions must evaluate to
//  TRUE.
//    - The Identity condition evaluates to TRUE if the requestor matches at
//      least one of the allowed identities.
//    - The Sphere condition evaluates to TRUE whenever the current sphere
//      state of the person in the presence document matches one of the sphere
//      state set in the Sphere condition.
//    - The Validity condition evaluates to TRUE whenever the current time
//      falls within the bounds of an allowed validity period.
//
//  If a condition is not set, then its evaluation defaults to TRUE.
//  More precisely, for the Identity condition, if it is not set, it matches any
//  requestor identity, authorized or not. To only allow authorized users, an
//  Identity condition must be set with the eMATCH_ALL parameter.
//
//#Actions#
//  The action takes place only when the corresponding conditions were all
//  evaluated to TRUE.
//
//  There is currently only one action type defined, which applies to the SIP
//  subscription request used to access the presence state of the user. The
//  action allows to block the subscription, delay it for user approval,
//  polite-block the subscription or accept it.
//
//  When merging actions from multiple matching rules, the highest value from
//  ESubscriptionAction is kept. As an example, if a request matches two rules,
//  one which action is eACTION_CONFIRM and the other is eACTION_ALLOW, the
//  resulting action will be eACTION_ALLOW.
//
//#Transformations#
//  Transformations are applied only when the the resulting action from the
//  merged rules accepts the subscription to the persence state of the user.
//  Otherwise no presence information is transmitted to the peer by the server.
//
//  The transformations can only be used to allow information from the real
//  presence document to be placed in the presence document that is sent to the
//  watcher. Thus, if none of the SetProvide...() or AddProvide...() method is
//  called to allow presence in formation to be published, an empty presence
//  document will be sent to requests matching this rule.
//
//  As with actions, all the transformations of matching rules are merged before
//  they are applied to a presence document. The merging of individual
//  transformation items is done as follows:
//    - For items that are configured as sets (AddProvideDevice, AddProvidePerson,
//      AddProvideService and AddProvideUnknownAttribute), the UNION of all
//      permissions is used.
//    - For items that are configured with an enumeration (SetProvideUserInput),
//      the maximum value found across all transformations is used.
//    - For items that are configured by simple boolean values, an OR
//      operation is performed across all transformations for this item.
//
// Location:
//   SceXml/CScePresenceAuthorizationRule.h
//
// See Also:
//   IScePresenceAuthorizationDocument
//
//==============================================================================
class CScePresenceAuthorizationRule
{
//M5T_INTERNAL_USE_BEGIN

//-- Friend Declarations.
    friend class CSceUaImppInitializer;

//M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CScePresenceAuthorizationRule(IN CXmlElement& rElementRule);

    // Summary:
    //  Destructor.
    //--------------
    ~CScePresenceAuthorizationRule();

    //-- << CONDITIONS >>
    //--------------------------------------------

    // Description:
    //  This enumeration defines the type of conditions that can be applied to
    //  the identity of the watcher. It is used for building conditions and
    //  exceptions to these conditions.
    enum EIdentityCondition
    {
        // Description:
        //  The identity to configure is an URI.
        //  - If the watcher's URI matches the configured URI found among an
        //    identity condition then the identity condition is evaluated to
        //    true.
        //  - If the watcher's URI matches the configured URI found among an
        //    exception, then the identity condition is evaluated to false.
        //
        //  This corresponds to the \<one\> element for conditions, and
        //  to the \<except id="userUri"\> element for exceptions.
        eMATCH_URI,

        // Description:
        //  The identity is a domain name.
        //  - If the watcher's URI matches this domain name and its URI is not
        //    found among exceptions, then the identity condition is evaluated
        //    to true.
        //  - If the watcher's URI matches the configured domain found among
        //    an exception, then the identity condition evaluated to false.
        //
        //  This corresponds to the \<many domain="domainName"\> element for
        //  conditions, and to the \<except domain="domainName"\> for
        //  exceptions.
        eMATCH_DOMAIN,

        // Description:
        //  The identity matches all authenticated watcher, except if the
        //  watcher domain is found among the domains to exclude, or if the
        //  watcher's URI is found among the URIs to exclude.
        //
        //  This corresponds to the \<many\> element without a domain attribute.
        eMATCH_ALL
    };


    // Summary:
    //  Gets all identity conditions found in this rule.
    mxt_result GetIdentityConditions(OUT CVector<CXmlElement*>& rvpIdentity);

    // Summary:
    //  Adds an identity condition to this rule.
    CXmlElement* AddIdentityCondition(IN EIdentityCondition eIdentityType,
                                      IN const char* pszValue);

    // Summary:
    //  Gets information about an identity condition.
    mxt_result GetIdentityCondition(IN CXmlElement* pIdentityCondition,
                                    OUT EIdentityCondition& reIdentityType,
                                    OUT const char*& rpszValue);
    // Summary:
    //  Adds an exception to an identity condition.
    CXmlElement* AddIdentityException(IN CXmlElement* pIdentityCondition,
                                      IN EIdentityCondition eExceptionType,
                                      IN const char* pszExceptValue);

    // Summary:
    //  Gets all exceptions found with an identity condition.
    mxt_result GetIdentityExceptions(IN CXmlElement* pIdentityCondition,
                                     OUT CVector<CXmlElement*>& rvpExceptions);

    // Summary:
    //  Gets information about an exception to an identity condition.
    mxt_result GetIdentityException(IN CXmlElement* pExceptionElement,
                                    OUT EIdentityCondition& reExceptionType,
                                    OUT const char*& rpszExceptionValue);


    // Summary:
    //  Sets a sphere condition with the specified value.
    CXmlElement* SetSphereCondition(IN const char* pszShereValues);

    // Summary:
    //  Gets the sphere condition.
    CXmlElement* GetSphereCondition(OUT const char** ppszSphereValues);

    // Summary:
    //  Adds an entry to the validity condition.
    CXmlElement* AddValidityCondition(IN const CTime& rFrom,
                                      IN const CTime& rUntil);

    // Summary:
    //  Gets all validity conditions found in this rule.
    mxt_result GetValidityConditions(OUT CVector<CXmlElement*>& rvpValidity);

    // Summary:
    //  Gets information about a specific validity condition.
    mxt_result GetValidityCondition(IN CXmlElement* pValidityElement,
                                    OUT CTime& rFrom,
                                    OUT CTime& rUntil);

    // Summary:
    //  Updates information about a specific validity condition.
    mxt_result UpdateValidityCondition(IN CXmlElement* pValidityElement,
                                       IN CTime& rFrom,
                                       IN CTime& rUntil);

    //-- << ACTIONS >>
    //--------------------------------------------

    // Description:
    //  This enumeration defines the possible subscription actions to associate
    //  with this rule.
    enum ESubscriptionAction
    {
        // Description:
        //  Block the subscription and place it in the terminated state.
        eACTION_BLOCK,

        // Description:
        //  Place the subscription in the pending state and wait for user
        //  intervention to manually accept or decline the watcher from
        //  subscribing to the presence state.
        eACTION_CONFIRM,

        // Description:
        //  Accept the subscription but provide a "polite" presence document
        //  with no useful information in it.
        eACTION_POLITEBLOCK,

        // Description:
        //  Accept the subscription and publish presence information according
        //  to the transformations of all matching.
        eACTION_ALLOW
    };

    // Summary:
    //  Sets the action to take when this rule matches the incoming request.
    CXmlElement* SetSubscriptionAction(IN ESubscriptionAction eAction);

    // Summary:
    //  Gets the action associated with this rule.
    CXmlElement* GetSubscriptionAction(OUT ESubscriptionAction* peAction);

    //-- << TRANSFORMATIONS >>
    //--------------------------------------------

    // Description:
    //  This enumeration defines ways to allow the publication of a PIDF device
    //  information.
    enum EDeviceTransform
    {
        // Description:
        //  Provide all devices that have the specified \<class\> information to
        //  be part of the resulting presence document. See
        //  CScePidfDevice::GetClass.
        eDEVICE_CLASS,

        // Description:
        //  Provide the device that has the specified \<deviceID\> to be part of
        //  the resulting presence document. See CScePidfDevice::GetDeviceId.
        eDEVICE_ID,

        // Description:
        //  Provide the device that has the specific "id" attribute to be part of
        //  the resulting presence document. See CScePidfDevice::GetId().
        eDEVICE_OCCURENCEID,

        // Description:
        //  Provide all devices to be part of the resulting presence document.
        eDEVICE_ALL
    };

    // Summary:
    //  Specifies which device element to provide when the rule matches an
    //  incoming request.
    CXmlElement* AddProvideDevice(IN EDeviceTransform eDeviceTransform,
                                  IN const char* pszValue);

    // Summary:
    //  Get all information about which devices are provided when this rule
    //  matches.
    mxt_result GetProvideDevices(OUT CVector<CXmlElement*>& rvpProvideDevice);


    // Summary:
    //  Gets information about a provide-device element.
    mxt_result GetProvideDevice(IN CXmlElement* pProvideDeviceElement,
                                OUT EDeviceTransform& reDeviceTransform,
                                OUT const char*& rpszValue);



    // Description:
    //  This enumeration defines ways to allow the publication of a PIDF person
    //  information.
    enum EPersonTransform
    {
        // Description:
        //  Provide all person elements that have the specified \<class\>
        //  information to be part of the resulting presence document. See
        //  CScePidfPerson::GetClass.
        ePERSON_CLASS,

        // Description:
        //  Provide the person element that has the specific "id" attribute to be
        //  part of the resulting presence document. See CScePidfPerson::GetId().
        ePERSON_OCCURENCEID,

        // Description:
        //  Include all person elements in the resulting presence document.
        ePERSON_ALL
    };

    // Summary:
    //  Specifies which person element to provide when the rule matches an
    //  incoming request.
    CXmlElement* AddProvidePerson(IN EPersonTransform ePersonTransform,
                                  IN const char* pszValue);

    // Summary:
    //  Get all information about which persons are provided when this rule
    //  matches.
    mxt_result GetProvidePersons(OUT CVector<CXmlElement*>& rvpProvidePerson);

    // Summary:
    //  Gets information about a provide-person element.
    mxt_result GetProvidePerson(IN CXmlElement* pProvidePersonElement,
                                OUT EPersonTransform& rePersonTransform,
                                OUT const char*& rpszValue);


    // Description:
    //  This enumeration defines ways to allow the publication of a PIDF service
    //  information.
    enum EServiceTransform
    {
        // Description:
        //  Provide all service elements that have the specified \<class\>
        //  information to be part of the resulting presence document. See
        //  CScePidfService::GetClass.
        eSERVICE_CLASS,

        // Description:
        //  Provide the service that has the specific "id" attribute to be part
        //  of the resulting presence document. See CScePidfService::GetId().
        eSERVICE_OCCURENCEID,

        // Description:
        //  Provide the services that have the specified URI to be part of the
        //  resulting presence document. See CScePidfService::GetContact.
        eSERVICE_URI,

        // Description:
        //  Provide the services that have the specified URI scheme to be part
        //  of the resulting presence document. See CScePidfService::GetContact.
        eSERVICE_URISCHEME,

        // Description:
        //  Include all service elements in the resulting presence document.
        eSERVICE_ALL
    };

    // Summary:
    //  Specifies which service element to provide when the rule matches an
    //  incoming request.
    CXmlElement* AddProvideService(IN EServiceTransform eServiceTransform,
                                   IN const char* pszValue);

    // Summary:
    //  Get all information about which services are provided when this rule
    //  matches.
    mxt_result GetProvideServices(OUT CVector<CXmlElement*>& rvpService);

    // Summary:
    //  Gets information about a provide-service element.
    mxt_result GetProvideService(IN CXmlElement* pProvideServiceElement,
                                 OUT EServiceTransform& reServiceTransform,
                                 OUT const char*& rpszValue);



    // Summary:
    //  Provide (or not) the activities when the rule matches.
    CXmlElement* SetProvideActivities(IN bool bProvide);

    // Summary:
    //  Gets whether or not the activity elements are provided.
    CXmlElement* GetProvideActivities(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the class when the rule matches.
    CXmlElement* SetProvideClass(IN bool bProvide);
    // Summary:
    //  Gets whether or not the class elements are provided.
    CXmlElement* GetProvideClass(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the deviceId when the rule matches.
    CXmlElement* SetProvideDeviceId(IN bool bProvide);
    // Summary:
    //  Gets whether or not the device ID elements are provided.
    CXmlElement* GetProvideDeviceId(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the mood when the rule matches.
    CXmlElement* SetProvideMood(IN bool bProvide);
    // Summary:
    //  Gets whether or not the mood elements are provided.
    CXmlElement* GetProvideMood(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the place-is when the rule matches.
    CXmlElement* SetProvidePlaceIs(IN bool bProvide);

    // Summary:
    //  Gets whether or not the place-is elements are provided.
    CXmlElement* GetProvidePlaceIs(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the place-type when the rule matches.
    CXmlElement* SetProvidePlaceType(IN bool bProvide);
    // Summary:
    //  Gets whether or not the place-type elements are provided.
    CXmlElement* GetProvidePlaceType(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the privacy when the rule matches.
    CXmlElement* SetProvidePrivacy(IN bool bProvide);
    // Summary:
    //  Gets whether or not the privacy elements are provided.
    CXmlElement* GetProvidePrivacy(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the relationship when the rule matches.
    CXmlElement* SetProvideRelationship(IN bool bProvide);
    // Summary:
    //  Gets whether or not the relationship elements are provided.
    CXmlElement* GetProvideRelationship(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the sphere when the rule matches.
    CXmlElement* SetProvideSphere(IN bool bProvide);
    // Summary:
    //  Gets whether or not the sphere elements are provided.
    CXmlElement* GetProvideSphere(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the status-icon when the rule matches.
    CXmlElement* SetProvideStatusIcon(IN bool bProvide);
    // Summary:
    //  Gets whether or not the status-icon elements are provided.
    CXmlElement* GetProvideStatusIcon(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) the time-offset when the rule matches.
    CXmlElement* SetProvideTimeOffset(IN bool bProvide);
    // Summary:
    //  Gets whether or not the time-offset elements are provided.
    CXmlElement* GetProvideTimeOffset(OUT bool* pbProvide);

    // Description:
    //  This enumeration defines how the user-input element is provided.
    enum EUserInputTransform
    {
        // Description:
        //  Provide no user-input information.
        eUSERINPUT_NONE,
        // Description:
        //  Provide bare user-input information. No "idle-threshold" and
        //  "last-input" information is provided.
        eUSERINPUT_BARE,
        // Description:
        //  Provide user-input information with the "idle-threshold"
        //  information.
        eUSERINPUT_THRESHOLDS,
        // Description:
        //  Provide full user-input information.
        eUSERINPUT_FULL
    };

    // Summary:
    //  Provide (or not) the user input information when the rule matches.
    CXmlElement* SetProvideUserInput(IN EUserInputTransform eUserInputTransform);
    // Summary:
    //  Gets whether or not the user-input elements are provided.
    CXmlElement* GetProvideUserInput(OUT EUserInputTransform* peState);

    // Summary:
    //  Provide (or not) the notes when the rule matches.
    CXmlElement* SetProvideNote(IN bool bProvide);
    // Summary:
    //  Gets whether or not the note elements are provided.
    CXmlElement* GetProvideNote(OUT bool* pbProvide);

    // Summary:
    //  Provide (or not) a specific unknown element when the rule matches.
    CXmlElement* AddProvideUnknownAttribute(IN const char* pszAttrNamespace,
                                            IN const char* pszAttrName,
                                            IN bool bProvide);


    // Summary:
    //  Gets information about which unknown elements are provided.
    mxt_result GetProvideUnknownAttributes(OUT CVector<CXmlElement*>& rvpElems);

    // Summary:
    //  Gets more information about unknown elements that are provided (or not).
    mxt_result GetProvideUnknownAttribute(IN CXmlElement* pElement,
                                          OUT const char*& rpszAttrNamespace,
                                          OUT const char*& rpszAttrName,
                                          OUT bool& bProvide);

    // Summary:
    //  Provide (or not) all presence elements when the rule matches.
    CXmlElement* SetProvideAllAttribute(IN bool bProvide);

    // Summary:
    //  Gets whether or not all elements are provided.
    CXmlElement* GetProvideAllAttribute(OUT bool* pbProvide);


//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CScePresenceAuthorizationRule.
    static mxt_result InitializeCScePresenceAuthorizationRule();

    // Summary:
    //  Finalizes the CScePresenceAuthorizationRule.
    static void FinalizeCScePresenceAuthorizationRule();

    // Summary:
    //  Provide (or not) to the specified element when the rule matches.
    CXmlElement* SetProvideBoolean(IN const char* pszProvideName, IN bool bProvide);

    // Summary:
    //  Gets whether or not the specified elements are provided.
    CXmlElement* GetProvideBoolean(IN const char* pszProvideName, OUT bool* pbProvide);

    // Summary:
    //  Converts a ESubscriptionAction into a string.
    const char* ConvertSubscriptionAction(ESubscriptionAction eAction);

    // Summary:
    //  Converts a string into a ESubscriptionAction.
    ESubscriptionAction ConvertSubscriptionAction(const char* pszSubscriptionAction);

    // Summary:
    //  Converts a EDeviceTransform into a string.
    const char* ConvertDeviceTransform(EDeviceTransform eTransform);

    // Summary:
    //  Converts a string into a EDeviceTransform.
    EDeviceTransform ConvertDeviceTransform(const char* pszDeviceTransform);

    // Summary:
    //  Converts a EPersonTransform into a string.
    const char* ConvertPersonTransform(EPersonTransform eTransform);

    // Summary:
    //  Converts a string into a EPersonTransform.
    EPersonTransform ConvertPersonTransform(const char* pszPersonTransform);

    // Summary:
    //  Converts a EServiceTransform into a string.
    const char* ConvertServiceTransform(EServiceTransform eTransform);

    // Summary:
    //  Converts a string into a EServiceTransform.
    EServiceTransform ConvertServiceTransform(const char* pszServiceTransform);

    // Summary:
    //  Converts a EUserInputTransform into a string.
    const char* ConvertUserInputTransform(EUserInputTransform eTransform);

    // Summary:
    //  Converts a string into a EUserInputTransform.
    EUserInputTransform ConvertUserInputTransform(const char* pszUserInputTransform);

    // Summary:
    //  Gets or created an element with the specified name and namespace under
    //  the \<conditions\> element.
    CXmlElement* GetCreateCondition(IN const char* pszNamespace,
                                    IN const char* pszNsPrefix,
                                    IN const char* pszName);

    // Summary:
    //  Finds an element with the specified name and namespace under the
    //  \<conditions\> element.
    CXmlElement* FindCondition(IN const char* pszNamespace,
                               IN const char* pszName);

    // Summary:
    //  Gets or created an element with the specified name and namespace under
    //  the \<transformations\> element.
    CXmlElement* GetCreateTransform(IN const char* pszNamespace,
                                    IN const char* pszNsPrefix,
                                    IN const char* pszName);

    // Summary:
    //  Finds an element with the specified name and namespace under the
    //  \<transformations\> element.
    CXmlElement* FindTransform(IN const char* pszNamespace,
                               IN const char* pszName);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CScePresenceAuthorizationRule(IN const CScePresenceAuthorizationRule& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CScePresenceAuthorizationRule& operator=(IN const CScePresenceAuthorizationRule& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    // The internal rule XML element.
    CXmlElement* m_pRuleXmlElement;

    // Vector representing the sequence for the <device> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecRuleSequence;

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetIdentityExceptions
//==
//==============================================================================
//
//    pIdentityCondition:
//      The \<many\> element from which to get the exceptions.
//
//  Parameters:
//    pIdentityCondition:
//      Vector provided by the application, which is filled with all
//      exception element that pIdentityCondition contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all exceptions found with an identity condition.
//
//      More information about each child element can be retreived using
//      GetIdentityException.
//
//  See Also:
//      AddIdentityException, GetIdentityException
//
//==============================================================================
inline mxt_result CScePresenceAuthorizationRule::GetIdentityExceptions(IN CXmlElement* pIdentityCondition,
                                                             OUT CVector<CXmlElement*>& rvpExceptions)
{
    return  CSceXmlHelper::GetElementFrom(pIdentityCondition,
                                          g_pszXML_NAMESPACE_COMMON_POLICY,
                                          g_pszXML_EXCEPT,
                                          OUT rvpExceptions);
}

//==============================================================================
//==
//==  SetProvideActivities
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the activities should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the activities when the rule
//      matches. As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the <activities> element defined in
//     [9]. The name of the element providing this permission is
//     \<provide-activities\>, and it is a Boolean type. If its value is TRUE,
//     then the \<activities\> element in the person data element is reported to
//     the watcher. If FALSE, this presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvideActivities
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideActivities(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_ACTIVITIES, bProvide);
}

//==============================================================================
//==
//==  GetProvideActivities
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the activities should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the activities elements are provided.
//
//  See Also:
//      SetProvideActivities
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideActivities(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_ACTIVITIES, pbProvide);
}

//==============================================================================
//==
//==  SetProvideClass
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the class should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the class when the rule mathces.
//      As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<class\> element defined in [9].
//     The name of the element providing this permission is \<provide-class\>, and
//     it is a Boolean type. If its value is TRUE, then any \<class\> element in a
//     person, service, or device data element is reported to the watcher. If
//     FALSE, this presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvideClass
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideClass(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_CLASS, bProvide);
}

//==============================================================================
//==
//==  GetProvideClass
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the class should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the class elements should be provided.
//
//  See Also:
//      SetProvideClass
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideClass(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_CLASS, pbProvide);
}

//==============================================================================
//==
//==  SetProvideDeviceId
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the deviceId should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the deviceId when the rule
//      matches. As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<deviceID\> element in a \<tuple\>
//     element, as defined in [9]. The name of the element providing this
//     permission is \<provide-deviceID\>, and it is a Boolean type. If its value
//     is TRUE, then the \<deviceID\> element in the service data element is
//     reported to the watcher. If FALSE, this presence attribute is removed if
//     present. Note that the \<deviceID\> in a device data element is always
//     included, and not controlled by this permission.
// </CODE>
//
//  See Also:
//      GetProvideDeviceId
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideDeviceId(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_DEVICEID, bProvide);
}

//==============================================================================
//==
//==  GetProvideDeviceId
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the deviceId should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the deviceId elements should be provided.
//
//  See Also:
//      SetProvideDeviceId
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideDeviceId(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_DEVICEID, pbProvide);
}

//==============================================================================
//==
//==  SetProvideMood
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the mood should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the mood when the rule matches. As
//      defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<mood\> element defined in [9].
//     The name of the element providing this permission is \<provide-mood\>, and
//     it is a Boolean type.  If its value is TRUE, then the \<mood\> element in
//     the person data element is reported to the watcher.  If FALSE, this
//     presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvideMood
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideMood(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_MOOD, bProvide);
}

//==============================================================================
//==
//==  GetProvideMood
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the mood should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the mood elements should be provided.
//
//  See Also:
//      SetProvideMood
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideMood(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_MOOD, pbProvide);
}

//==============================================================================
//==
//==  SetProvidePlaceIs
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the place-is should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the place-is when the rule matches.
//      As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<place-is\> element defined in [9].
//     The name of the element providing this permission is \<provide-place-is\>,
//     and it is a Boolean type. If its value is TRUE, then the \<place-is\>
//     element in the person data element is reported to thewatcher. If FALSE,
//     this presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvidePlaceIs
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvidePlaceIs(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_PLACE_IS, bProvide);
}

//==============================================================================
//==
//==  GetProvidePlaceIs
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the place-is should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the place-is elements should be provided.
//
//  See Also:
//      SetProvidePlaceIs
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvidePlaceIs(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_PLACE_IS, pbProvide);
}

//==============================================================================
//==
//==  SetProvidePlaceType
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the place-type should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the place-type when the rule
//      matches. As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<place-type\> element defined in
//     [9]. The name of the element providing this permission is
//     \<provide-place-type\>, and it is a Boolean type. If its value is TRUE,
//     then the \<place-type\> element in the person data element is reported to
//     the watcher. If FALSE, this presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvidePlaceType
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvidePlaceType(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_PLACE_TYPE, bProvide);
}

//==============================================================================
//==
//==  GetProvidePlaceType
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the place-type should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the place-type elements should be provided.
//
//  See Also:
//      SetProvidePlaceType
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvidePlaceType(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_PLACE_TYPE, pbProvide);
}

//==============================================================================
//==
//==  SetProvidePrivacy
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the privacy should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the privacy when the rule matches.
//      As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<privacy\> element defined in [9].
//     The name of the element providing this permission is \<provide-privacy\>,
//     and it is a Boolean type. If its value is TRUE, then the \<privacy\>
//     element in the person or service data element is reported to the watcher.
//     If FALSE, this presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvidePrivacy
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvidePrivacy(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_PRIVACY, bProvide);
}

//==============================================================================
//==
//==  GetProvidePrivacy
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the privacy should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the privacy elements should be provided.
//
//  See Also:
//      SetProvidePrivacy
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvidePrivacy(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_PRIVACY, pbProvide);
}

//==============================================================================
//==
//==  SetProvideRelationship
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the relationship should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the relationship when the rule
//      matches. As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<relationship\> element defined
//     in [9]. The name of the element providing this permission is
//     \<provide-relationship\>, and it is a Boolean type. If its value is TRUE,
//     then the \<relationship\> element in the service data element is reported
//     to the watcher. If FALSE, this presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvideRelationship
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideRelationship(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_RELATIONSHIP, bProvide);
}

//==============================================================================
//==
//==  GetProvideRelationship
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the relationship should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the relationship elements should be provided.
//
//  See Also:
//      SetProvideRelationship
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideRelationship(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_RELATIONSHIP, pbProvide);
}

//==============================================================================
//==
//==  SetProvideSphere
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the sphere should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the sphere when the rule
//      matches. As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<sphere\> element defined in [9].
//     The name of the element providing this permission is \<provide- sphere\>,
//     and it is a Boolean type. If its value is TRUE, then the \<sphere\> element
//     in the person data element is reported to the watcher. If FALSE, this
//     presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvideSphere
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideSphere(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_SPHERE, bProvide);
}

//==============================================================================
//==
//==  GetProvideSphere
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the sphere should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the sphere elements should be provided.
//
//  See Also:
//      SetProvideSphere
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideSphere(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_SPHERE, pbProvide);
}

//==============================================================================
//==
//==  SetProvideStatusIcon
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the status-icon should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the status-icon when the rule
//      matches. As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<status-icon\> element defined in
//     [9]. The name of the element providing this permission is
//     \<provide-status-icon\>, and it is a Boolean type. If its value is TRUE,
//     then any \<status-icon\> element in the person or service data element is
//     reported to the watcher. If FALSE, this presence attribute is removed if
//     present.
// </CODE>
//
//  See Also:
//      GetProvideStatusIcon
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideStatusIcon(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_STATUS_ICON, bProvide);
}

//==============================================================================
//==
//==  GetProvideStatusIcon
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the status-icon should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the status-icon elements should be provided.
//
//  See Also:
//      SetProvideStatusIcon
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideStatusIcon(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_STATUS_ICON, pbProvide);
}

//==============================================================================
//==
//==  SetProvideTimeOffset
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the time-offset should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the time-offset when the rule
//      matches. As defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<time-offset\> element defined in
//     [9]. The name of the element providing this permission is
//     \<provide-time-offset\>, and it is a Boolean type. If its value is TRUE,
//     then the \<time-offset\> element in the person data element is reported to
//     the watcher. If FALSE, this presence attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvideTimeOffset
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideTimeOffset(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_TIME_OFFSET, bProvide);
}

//==============================================================================
//==
//==  GetProvideTimeOffset
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the time-offset should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the time-offset elements should be provided.
//
//  See Also:
//      SetProvideTimeOffset
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideTimeOffset(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_TIME_OFFSET, pbProvide);
}

//==============================================================================
//==
//==  SetProvideNote
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not the note should be provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide the note when the rule matches. As
//      defined in RFC 5025:
//
// <CODE>
//     This permission controls access to the \<note\> element defined in [3] for
//     \<tuple\> and [10] for \<person\> and \<device\>. The name of the element
//     providing this permission is \<provide-note\>, and it is a Boolean type.
//     If its value is TRUE, then any \<note\> elements in the person, service, or
//     device data elements are reported to the watcher. If FALSE, this presence
//     attribute is removed if present.
// </CODE>
//
//  See Also:
//      GetProvideNote
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideNote(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_NOTE, bProvide);
}

//==============================================================================
//==
//==  GetProvideNote
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not the note should be
//      provided.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not the note elements should be provided.
//
//  See Also:
//      SetProvideNote
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideNote(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_NOTE, pbProvide);
}

//==============================================================================
//==
//==  SetProvideAllAttribute
//==
//==============================================================================
//
//  Parameters:
//    bProvide:
//      Whether or not to grant access to all presence attributes for every
//      person, device, and tuple elements that are present in the
//      document.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Specifies whether or not to provide access to all presence elements for
//      every person, device, and tuple elements that are present in the
//      document. As defined in RFC 5025:
//
// <CODE>
//     This permission grants access to all presence attributes in all of the
//     person, device, and tuple elements that are present in the document (the
//     ones present in the document are determined by the \<provide-persons\>,
//     \<provide-devices\>, and \<provide-services\> permissions).
// </CODE>
//
//  See Also:
//      GetProvideAllAttribute
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::SetProvideAllAttribute(IN bool bProvide)
{
    return SetProvideBoolean(g_pszXML_PROVIDE_ALL_ATTRIBUTE, bProvide);
}

//==============================================================================
//==
//==  GetProvideAllAttribute
//==
//==============================================================================
//
//  Parameters:
//    pbProvide:
//      Boolean which is filled with whether or not to access to all presence
//      attributes for every person, device, and tuple elements present in the
//      document is granted.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Gets whether or not to provide access to all presence elements for every
//      person, device, and tuple elements present in the document is granted.
//
//  See Also:
//      SetProvideAllAttribute
//
//==============================================================================
inline CXmlElement* CScePresenceAuthorizationRule::GetProvideAllAttribute(OUT bool* pbProvide)
{
    return GetProvideBoolean(g_pszXML_PROVIDE_ALL_ATTRIBUTE, pbProvide);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEPRESENCEAUTHORIZATIONRULE_H

