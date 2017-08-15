//==============================================================================
//==============================================================================
//
//          Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCEPRESENCEAUTHORIZATIONDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEPRESENCEAUTHORIZATIONDOCUMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
//-------------------------
class CBlob;
class CSipMessageBody;
class CTime;
class CXmlElement;
class IUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
MX_DECLARE_ECOM_IID(IScePresenceAuthorizationDocument);

//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: IScePresenceAuthorizationDocument
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Description:
//  The IScePresenceAuthorizationDocument interface defines the API needed to
//  create and manipulate a presence authorization document, as defined in RFCs
//  4745 and 5025. 
//
//  The component implementing this interface keeps the authorization
//  information encoded in XML. It implements the \<ruleset\> element of RFC
//  4745 and supports the "application/auth-policy+xml" MIME format. The
//  CScePresenceAuthorizationRule helper class can be used to manipulate rules
//  and the \<rule\> element included by this document.
//
//  The IScePresenceAuthorizationDocument is used as a wrapper over an XML
//  document to make it easy for applications to manipulate authorization rules
//  within this XML tree. Users of this interface can always access the XML tree
//  directly by querying the IXmlDocument interface, while most of the methods
//  of the IScePresenceAuthorizationDocument and CScePresenceAuthorizationRule
//  gives direct access to the XML elements within this tree.
//
//#Presence Authorization Overview#
//
//  The presence authorization document is composed of zero or more rules. An
//  application can create a rule by calling AddRule. It then can configure the
//  rule using a CScePresenceAuthorizationRule instance.
//
//  The presence authorization document is generally used by two different
//  entities:
//    - It is used by a client to define which watchers are allowed to
//      subscribe to the presence state of the user. The authorization document
//      also allows a client to specify different transformations to the user's
//      presence state in order to hide or show specific elements from watchers.
//    - Once a user has created its presence authorization document, a
//      presence server will use this document to decide how to handle incoming
//      subscriptions for the user's presence state. When the server accepts a
//      subscription to the user's presence state, the server will use the
//      transformations in the document to figure out which part of the
//      presence document is provided to the watcher.
//
//  When a watcher tries to access the presence information of a user, the
//  server will locate the rule set of this user and then try to see if the
//  request matches one or more rules by comparing some information from the
//  watcher's request with the conditions of each rule.
//
//  If there are no rules that matches, the server rejects the request. If more
//  than one rules matches, the server merges all matching rules action and
//  transformations. Depending on the resulting merged action value, the server
//  may reject the request to access to the presence information, it may wait
//  for user's approval or grant access to the user's presence state.
//
//  Once the server grants access to the presence state of the user, it applies
//  transformations to the presence information in order to only let authorized
//  information be transmitted to the watcher. This transformation is based on
//  the merged transformation values from all matching rules.
//
//#Simple Authorization Document Example#
//
//  A simple authorization document could be created to have the following
//  behavior:
//    - Any non-authenticated watcher is denied access to the presence
//      document.
//    - Any authenticated watcher that is on the list of accepted watcher
//      (white list) has his subscription automatically accepted and is granted
//      full access to the presence state of the user.
//    - Any authenticated watcher that is on the list of rejected watchers
//      (black list) has his subscription automatically rejected.
//    - Any authenticated watcher who is not on the white list or on the black
//      list has his subscription placed in the pending state to wait for user
//      confirmation on whether or not to accept the subscription and grant
//      access to the presence data.
//
//  To create an authorization document with the above permissions, the
//  following would have to be done.
//
//    - Create a new rule to handle the white list by calling AddRule and then
//      creating a CScePresenceAuthorizationRule from the returned CXmlElement.
//      We call it pRuleWhiteList.
//    - For each allowed watchers, call
//      <C>pRuleWhiteList-\>AddIdentityCondition(eMATCH_URI, pszUri);</C>,
//      where pszUri is the URI of the allowed watcher.
//    - Allow the watchers on the white list to subscribe to presence state by
//      calling <C>pRuleWhiteList-\>SetSubscriptionAction(eACTION_ALLOW);</C>.
//    - Allow watchers on the white list access to all device, person
//      and service information by calling
//      <C>pRuleWhiteList-\>AddProvideDevice(eDEVICE_ALL);
//      pRuleWhiteList-\>AddProvidePerson(ePERSON_ALL);
//      pRuleWhiteList-\>AddProvideService(eSERVICE_ALL);</C>.
//    - Allow the watchers on the white list access to all attributes of all
//      person, device and service presence elements by using
//      <C>pRuleWhiteList-\>SetAllowAllAttributes(true);</C>.
//    - Create a new rule to handle the black list and the default processing
//      by calling AddRule and then creating a CScePresenceAuthorizationRule
//      from the returned CXmlElement. We call it pRuleBlackList.
//    - Allow all watchers to match this rule, except those that are part of
//      the black list. This is done by calling <C>pCondition =
//      pRuleBlackList-\>AddIdentityCondition(eMATCH_ALL, NULL);</C> once and
//      then calling <C>pRuleBlackList-\>AddIdentityException(pCondition,
//      eMATCH_URI, pUri);</C> for each blacklisted URIs.
//    - Set the subscription action of this rule to wait for user confirmation
//      before accepting the subscription by calling
//      <C>pRuleBlackList-\>SetSubscriptionAction(eACTION_CONFIRM);</C>.
//
//  As a summary, a watcher that is neither on the black list or on the white
//  list will have his identity match on pRuleBlackList (it matches any identity
//  except those explicitely black listed). The server will use the action
//  specified by this rule, which is to place the subscription in the pending
//  state.
//
//  A watcher that is on the white list will match on both pRuleWhiteList and
//  pRuleBlackList. However, when the server merges the action (takes greatest
//  action value) and the provided items (uses the union of both rules), then
//  the subscription is accepted and the watcher is allowed to have full access
//  to the presence information.
//      
//  A watcher that is on the black list will not match on pRuleWhiteList and is
//  also specifically excluded from matching pRuleBlackList. Since it does not
//  match any list, the server uses the default action for the subscription,
//  which is to reject it.
//
// Location:
//   SceXml/IScePresenceAuthorizationDocument.h
//
// See Also:
//   CScePresenceAuthorizationRule
//
//==============================================================================
class IScePresenceAuthorizationDocument : public IEComUnknown
{
//-- Friend Declarations.
//------------------------

//-- New types within class namespace.
//-------------------------------------
public:
protected:
private:

//-- Published Interface.
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IScePresenceAuthorizationDocument);
    //M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  AddRule
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a rule to the document.
    //
    //  Returns:
    //      A pointer to the newly added rule element.
    //
    //  Description:
    //      Adds a rule element to the presence authorization document.
    //
    //==========================================================================
    virtual CXmlElement* AddRule() = 0;

    //==========================================================================
    //==
    //==  GetRules
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the rules held by the document.
    //
    //  Parameters:
    //    rvpRule:
    //      Vector in which to put every \<rule\> element held in this document.
    //
    //  Returns:
    //      resS_OK upon success an error otherwise.
    //
    //  Description:
    //      Gets a pointer to all rule elements held by this presence
    //      authorization document. A \<rule\> element can be manipulated using
    //      a CScePresenceAuthorizationRule.
    //
    //  See Also:
    //      CScePresenceAuthorizationRule
    //
    //==========================================================================
    virtual mxt_result GetRules(OUT CVector<CXmlElement*>& rvpRule) = 0;

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IScePresenceAuthorizationDocument() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IScePresenceAuthorizationDocument() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IScePresenceAuthorizationDocument(IN const IScePresenceAuthorizationDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IScePresenceAuthorizationDocument& operator=(IN const IScePresenceAuthorizationDocument& rSrc);


//-- Hidden Data Members.
//------------------------
protected:
private:

//M5T_INTERNAL_USE_END

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEPRESENCEAUTHORIZATIONDOCUMENT_H
