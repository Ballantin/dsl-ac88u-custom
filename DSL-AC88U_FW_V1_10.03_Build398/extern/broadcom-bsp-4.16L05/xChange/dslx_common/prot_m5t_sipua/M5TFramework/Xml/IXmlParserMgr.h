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
#ifndef MXG_IXMLPARSERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IXMLPARSERMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IXmlParserMgr
//========================================
//<GROUP XML_CLASSES>
//
// Description:
//  This interface defines the event that can be reported by the XML parser 
//  implementing the IXmlParser interface.
//
// Location:
//   Xml/IXmlParserMgr.h
//
// See Also:
//   IXmlParserMgr
//
//==============================================================================
class IXmlParserMgr
{
//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvStartElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the start of an element was detected.
    //
    //  Parameters:
    //    pszNamespaceUri:
    //      Pointer to a NULL terminated string containing the full namespace
    //      URI associated with the element. It can be NULL if there is no
    //      namespace associated with the element.
    //
    //    pszElementName:
    //      Pointer to a NULL terminated string containing the element's name,
    //      free of any namespace or namespace prefixes. It will always be a
    //      valid, non-NULL pointer.
    //
    //    ppAttributes:
    //      Attributes triplets, with the first element of an attribute being
    //      the attribute namespace, the second element being the attribute
    //      name (free of any namespace or namespace prefix), and the third
    //      element being the attribute value. 3 NULL pointers will signal
    //      the end of the array.
    //
    //    ppNewNamespaces:
    //      New namespaces that are declared along with this XML element.
    //      If there are no namespaces, this pointer is NULL.
    //      ppNewNamespaces points to a table of strings where the first
    //      position corresponds to the prefix of the first namespace, the
    //      second position to the URI of the first namespace, and so on.
    //      An entry with both pointers equal to NULL signals the end of the
    //      table.
    //
    //  Description:
    //      Reports that the start of an XML element was detected.
    //
    //      ppAttributes is an array of C-Style strings. For each attribute
    //      associated with the element, one C-Style string points to the URI
    //      namespace of the attribute, another to its name, and a last one to
    //      its value, in that order. Thus the number of items in this array
    //      must be zero or a multiple of 3. If there are no attributes,
    //      ppAttributes will be NULL. If there is one or more attributes, each
    //      attribute is reported in triplets, and the last triplet will point
    //      to NULL strings.
    //
    //      ppNewNamespaces  is an array of C-Style strings. It is arranged
    //      in pairs, where the first element of the pair is the prefix and
    //      the second one is the namespace URI. Thus the number of items in
    //      this array must be zero or a multiple of 2. If there are no
    //      namespaces, ppNewNamespaces will be NULL. If there is one or more
    //      namespaces, each namespace is reported as pairs, and the last pair
    //      will point to NULL strings.
    //
    //  Warning:
    //      The returned strings are valid only for the duration of the event.
    //      The parser manager must make a copy of any strings it wants to keep.
    //
    //  Example:
    //      Assume the following XML element:
    //
    //      <CODE>
    //
    //      \<element xmlns="urn:test1"
    //                xmlns:ns2="urn:test2"
    //                xmlns:ns3="urn:test3"
    //                att1="1" ns2:att2="2" ns3:att3="3"\>
    //      \</element\>
    //
    //      </CODE>
    //
    //      The szNamespaceUri and szElementName parameters would be the
    //      following:
    //
    //      <CODE>
    //      szNamespaceUri = Pointer to "urn:test1" // default namespace
    //      szElementName = Pointer to "element"
    //      </CODE>
    //
    //      The pAttributes parameter of EvStartElement would be:
    //
    //      <CODE>
    //      pAttributes[0] = NULL pointer // default namespace does not apply to attributes!
    //      pAttributes[1] = Pointer to "att1"
    //      pAttributes[2] = Pointer to "1"
    //      pAttributes[3] = Pointer to "urn:test2"
    //      pAttributes[4] = Pointer to "att2"
    //      pAttributes[5] = Pointer to "2"
    //      pAttributes[6] = Pointer to "urn:test3"
    //      pAttributes[7] = Pointer to "att3"
    //      pAttributes[8] = Pointer to "3"
    //      pAttributes[9] = NULL pointer
    //      pAttributes[10] = NULL pointer
    //      pAttributes[11] = NULL pointer
    //      </CODE>
    //
    //
    //      The ppNewNamespaces parameter of EvStartElement would be:
    //
    //      <CODE>
    //      pAttributes[0] = NULL pointer               // default namespace does not have a prefix.
    //      pAttributes[1] = Pointer to "urn:test1"     // default namespace.
    //      pAttributes[2] = Pointer to "ns2"
    //      pAttributes[3] = Pointer to "urn:test2"
    //      pAttributes[4] = Pointer to "ns3"
    //      pAttributes[5] = Pointer to "urn:test3"
    //      pAttributes[6] = NULL pointer
    //      pAttributes[7] = NULL pointer
    //      </CODE>
    //
    //==========================================================================
    virtual void EvStartElement(IN const char* pszNamespaceUri,
                                IN const char* pszElementName,
                                IN const char** ppAttributes,
                                IN const char** ppNewNamespaces = NULL) = 0;

    //==========================================================================
    //==
    //==  EvEndElement
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the end of an element was detected.
    //
    //  Parameters:
    //    pszNamespaceUri:
    //      Pointer to a NULL terminated string containing the full namespace
    //      URI associated with the element. It can be NULL if there is no
    //      namespace associated with the element.
    //
    //    pszElementName:
    //      Pointer to a NULL terminated string containing the element's name,
    //      free of any namespace or namespace prefixes. It will always be a
    //      valid, non-NULL pointer.
    //
    //  Description:
    //      Reports that the end of an XML element was detected.
    //
    //  See Also:
    //      EvStartElement
    //
    //==========================================================================
    virtual void EvEndElement(IN const char* pszNamespaceUri,
                              IN const char* pszElementName) = 0;

    //==========================================================================
    //==
    //==  EvCharacterData
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that character data was detected.
    //
    //  Parameters:
    //    pcText:
    //      Pointer to the data that was detected. It is not NULL terminated.
    //
    //    uTextSize:
    //      The number of bytes detected.
    //
    //  Description:
    //      Reports that character data was detected. This event is reported for
    //      the actual data held by the XML elements.
    //
    //      This event can be reported multiple times for a single XML element,
    //      even if there is no markup within the element.
    //
    //  Example:
    //      Assume the following element:
    //
    //      <CODE>
    //
    //      \<element\>
    //      1234567890
    //      \</element\>
    //
    //      </CODE>
    //
    //      The EvCharacterData can be called from one ("1234567890") to 10
    //      times ("1","2","3","4","5","6","7","8","9" and "0"), splitting the
    //      characters across all calls.
    //
    //==========================================================================
    virtual void EvCharacterData(IN const char* pcText,
                                 IN const unsigned int uTextSize) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IXmlParserMgr() {};

    // Summary:
    //  Destructor.
    virtual ~IXmlParserMgr() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IXmlParserMgr(IN const IXmlParserMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IXmlParserMgr& operator=(IN const IXmlParserMgr& rSrc);

//-- Hidden Data Members
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IXMLPARSERMGR_H

