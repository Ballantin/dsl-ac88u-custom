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
#ifndef MXG_CSCEXMLHELPER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEXMLHELPER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CString;
class CTime;
class CXmlElement;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

// MXI_DESIGN_NOTES: This class should also be usable when not using UA-IMPP.
//==============================================================================
//== Class: CSceXmlHelper
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Summary:
//  This class is a helper class for XML.
//
// Description:
//  This class implements static XML helper function to be used in the
//  SipClientEngine.
//
// Location:
//   SceXml/CSceXmlHelper.h
//
//==============================================================================
class CSceXmlHelper
{
//-- Friend Declarations.


//-- Published Interface.
public:

    // Summary:
    //  Sets the "from" attributes in pElement.
    static mxt_result SetFrom(IN CXmlElement* pElement,
                              IN const CTime& rFrom);

    // Summary:
    //  Sets the "until" attributes in pElement.
    static mxt_result SetUntil(IN CXmlElement* pElement,
                               IN const CTime& rUntil);

    // Summary:
    //  Gets the "from" attributes from pElement.
    static mxt_result GetFrom(IN CXmlElement* pElement,
                              OUT CTime& rFrom);

    // Summary:
    //  Gets the "until" attributes from pElement.
    static mxt_result GetUntil(IN CXmlElement* pElement,
                               OUT CTime& rUntil);

    // Summary:
    //  Removes all elements contained in the vector from the XML document.
    static mxt_result RemoveAllElements(INOUT CVector<CXmlElement*>& rvpElement);

    // Summary:
    //  Gets the value of the "xml:lang" attribute.
    static const char* GetLanguage(IN CXmlElement* pElement);

    // Summary:
    //  Gets all elements named pszElementName found as child of pParentElement.
    static mxt_result GetElementFrom(IN CXmlElement* pParentElement,
                                     IN const char* pszElementNamespace,
                                     IN const char* pszElementName,
                                     OUT CVector<CXmlElement*>& rvpElement);

    // Summary:
    //  Adds a new element in pXmlParentElement.
    static CXmlElement* AddElement(INOUT CXmlElement* pXmlParentElement,
                                   IN const char* pszElementNamespace,
                                   IN const char* pszElementNamespacePrefix,
                                   IN const char* pszElementName,
                                   IN const char* pszElementValue = "",
                                   IN const CVector<CXmlElement::SIdentificationInfo>* pvecstElementOrder = NULL,
                                   IN bool bAddId = false);

    // Summary:
    //  Tries to find the specified element, if the element does not exists it
    //  is created.
    static CXmlElement* GetElement(IN CXmlElement* pXmlParentElement,
                                   IN const char* pszElementNamespace,
                                   IN const char* pszElementNamespacePrefix,
                                   IN const char* pszElementName,
                                   IN const char* pszElementValue = "",
                                   IN const CVector<CXmlElement::SIdentificationInfo>* pvecstElementOrder = NULL);

    // Summary:
    //  Transforms a time in string into a CTime.
    static void SetTime(IN const char* pszTimeValue, OUT CTime& rTime);

    // Summary:
    //  Transforms a time CTime into a string.
    static void GetXmlTime(IN const CTime& rTime, OUT CString& strTime);

    // Summary:
    //  Adds an Id attribute to an element.
    static void AddIdAttribute(INOUT CXmlElement* pElement, IN const char* pszData);

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>
    // Summary:
    //  Default Constructor.
    CSceXmlHelper();

    // Summary:
    //  Destructor.
    virtual ~CSceXmlHelper();

private:

//-- Hidden Data Members.
protected:
private:

//M5T_INTERNAL_USE_END

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEXMLHELPER_H

