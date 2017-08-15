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
#ifndef MXG_CSCEFILTERDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEFILTERDOCUMENT_H

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

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IFILTERDOCUMENT_H
#include "SceXml/ISceFilterDocument.h"
#endif

#ifndef MXG_ISCEXMLDOCUMENT_H
#include "SceXml/ISceXmlDocument.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CBlob;
class CMimeObject;
class CSipMessageBody;
class CSubAllocator;
class IUri;
class IXmlDocument;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// These are the names of all simple-filter XML elements and attributes.
extern const char* const g_pszXMLFILTER_FILTER_SET;
extern const char* const g_pszXMLFILTER_NS_BINDINGS;
extern const char* const g_pszXMLFILTER_NS_BINDING;
extern const char* const g_pszXMLFILTER_PREFIX;
extern const char* const g_pszXMLFILTER_URN;
extern const char* const g_pszXMLFILTER_FILTER;
extern const char* const g_pszXMLFILTER_WHAT;
extern const char* const g_pszXMLFILTER_INCLUDE;
extern const char* const g_pszXMLFILTER_EXCLUDE;
extern const char* const g_pszXMLFILTER_TYPE;
extern const char* const g_pszXMLFILTER_XPATH;
extern const char* const g_pszXMLFILTER_NAMESPACE;
extern const char* const g_pszXMLFILTER_TRIGGER;
extern const char* const g_pszXMLFILTER_CHANGED;
extern const char* const g_pszXMLFILTER_FROM;
extern const char* const g_pszXMLFILTER_TO;
extern const char* const g_pszXMLFILTER_BY;
extern const char* const g_pszXMLFILTER_ADDED;
extern const char* const g_pszXMLFILTER_REMOVED;
extern const char* const g_pszXMLFILTER_ID;
extern const char* const g_pszXMLFILTER_URI;
extern const char* const g_pszXMLFILTER_DOMAIN;
extern const char* const g_pszXMLFILTER_REMOVE;
extern const char* const g_pszXMLFILTER_ENABLED;
extern const char* const g_pszXMLFILTER_PACKAGE;
extern const char* const g_pszXMLFILTER_NAMESPACE_FILTER;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceFilterDocument
//========================================
//
// Summary:
//  This class represents a Filter document.
//
// Description:
//  This class is the implementation of a Filter document as specified in RFC
//  4661. For more information about the Filter document see ISceFilterDocument.
//
// Location:
//   SceXml/CSceFilterDocument.h
//
// See Also:
//   ISceFilterDocument, CSceFilterElement
//
//==============================================================================
class CSceFilterDocument : protected CEComDelegatingUnknown,
                           private ISceFilterDocument,
                           private ISceXmlDocument
{
//-- Friend Declarations.
    // MXI_DESIGN_NOTES (2008-12-16): CSceFilterDocument should eventually be
    //                                initialized in CoreComponents once it
    //                                no longer requires UA-IMPP specific files.
    friend class CSceUaImppInitializer;

//-- Published Interface.
public:
    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual mxt_result InitializeInstance();

    //-- << Public methods >>
    // Summary:
    //  Sets the package to which the filter criteria is applied.
    virtual mxt_result SetPackage(IN const char* pszPackage);

    // Summary:
    //  Gets the package to which the filter criteria is applied.
    virtual const char* GetPackage();

    // Summary:
    //  Gets a pointer to all <ns-binding> elements held by this filter
    //  document.
    virtual mxt_result GetNsBindings(OUT CVector<CXmlElement*>& rvecpNsBindings);

    // Summary:
    //  Gets the namespace and its associated prefix of a <ns-binding> element.
    virtual mxt_result GetNsBinding(IN CXmlElement* pNsBinding,
                                    OUT const char*& rpszPrefix,
                                    OUT const char*& rpszUrn) const;
    
    // Summary:
    //  Adds a <ns-binding> element to the filter document.
    virtual CXmlElement* AddNsBinding(IN const char* pszPrefix,
                                      IN const char* pszUrn);

    // Summary:
    //  Gets a pointer to all <filter> elements held by this filter
    //  document.
    virtual mxt_result GetFilters(OUT CVector<CXmlElement*>& rvecpFilters);
    
    // Summary:
    //  Adds a <filter> element to the filter document.
    virtual CXmlElement* AddFilter(IN const char* pszUri = NULL,
                                   IN const char* pszDomain = NULL,
                                   IN bool bRemove = false,
                                   IN bool bEnable = true);

    // Summary:
    //  Adds a <filter> element to the filter document.
    virtual CXmlElement* AddFilter(IN const IUri* pUri,
                                   IN const char* pszDomain = NULL,
                                   IN bool bRemove = false,
                                   IN bool bEnable = true);

    // Summary:
    //  Checks if the document and associated allocator need to be cleaned-up
    //  and cleans it them up if necessary.
    virtual mxt_result VerifyDocumentCleanup();

    // Summary:
    //  Finds and parses the filter XML document found in the MIME object.
    virtual mxt_result Parse(IN const CMimeObject& rMimeObject);

    // Summary:
    //  Serializes the filter XML document into a MIME object.
    virtual void Serialize(OUT GO CMimeObject*& rpMimeObject);

    // Summary:
    //  Finds and parses the filter XML document found in the CSipMessageBody object.
    virtual mxt_result Parse(IN const CSipMessageBody& rMessageBody);

    // Summary:
    //  Serializes the Filter XML document into a CSipMessageBody object.
    virtual void Serialize(OUT GO CSipMessageBody*& rpMessageBody);

    // Summary:
    //  Parses the filter XML document from a pointer to the actual XML data.    
    virtual mxt_result Parse(INOUT const char*& rpcPos, IN unsigned int uLength = 0);    

    // Summary:
    //  Serializes the filter XML document in a blob.
    virtual void Serialize(INOUT CBlob& rBlob);

    // Summary:
    //  Verifies if the filter document can optimize the memory it uses.
    virtual bool IsCleanupRequired(OUT unsigned int& ruNumBytesSaved);
    
    // Summary:
    //  Optimizes the memory used by the filter document.
    virtual mxt_result Cleanup();
    

    //-- << Helper Methods >>
    //--------------------------------------------

    // Summary:
    //  Gets all elements named pszElementName found as child of pParentElement.
    static mxt_result GetElementFrom(IN CXmlElement* pParentElement,
                                     IN const char* pszElementNamespace,
                                     IN const char* pszElementName,
                                     OUT CVector<CXmlElement*>& rvpElement);


//-- Hidden Methods.
protected:
    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CSceFilterDocument.
    static mxt_result InitializeCSceFilterDocument();

    // Summary:
    //  Finalizes the CSceFilterDocument
    static void FinalizeCSceFilterDocument();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceFilterDocument(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CSceFilterDocument();
private:


//-- Hidden Data Members.
protected:
private:

    // ECom unknown for the iXML document used to hold the PIDF information.
    IEComUnknown* m_pXmlDocumentUnknown;

    // XML document used to hold the filter information.
    IXmlDocument* m_pXmlDocument;

    // Allocator used for the XML document.
    CSubAllocator* m_pAllocator;

    // The allocation block size.
    unsigned int m_uBlockSize;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEFILTERDOCUMENT_H

//M5T_INTERNAL_USE_END
