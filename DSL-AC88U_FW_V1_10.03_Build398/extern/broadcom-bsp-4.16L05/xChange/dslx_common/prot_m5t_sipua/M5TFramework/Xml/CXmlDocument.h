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
#ifndef MXG_CXMLDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CXMLDOCUMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

//-- Data Members.

//-- Interface Realized and/or Parent.
#ifndef MXG_CECOMUNKNOWN_H
#include "ECom/CEComUnknown.h"
#endif

#ifndef MXG_IXMLDOCUMENT_H
#include "Xml/IXmlDocument.h"
#endif

#ifndef MXG_IXMLPARSERMGR_H
#include "Xml/IXmlParserMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CString;
class CXmlElement;
class IXmlDocumentMgr;
class IXmlPatchMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CXmlDocument
//==============================================================================
//<GROUP XML_CLASSES>
//
// Description:
//   This class implements the XML document interface.
//
// Location:
//   Xml/CXmlDocument.h
//
// See Also:
//   IXmlDocument
//
//==============================================================================
class CXmlDocument : protected CEComUnknown,
                     private IXmlDocument,
                     private IXmlParserMgr
{
public:
//-- Hidden Methods.
protected:
    //-- << Static Initializer >>

    static mxt_result InitializeCXmlDocument();
    static void FinalizeCXmlDocument();

    //-- Friendship declaration.
    friend mxt_result InitializeCXmlDocument();
    friend void FinalizeCXmlDocument();

private:
    //-- << ECom Pattern >>

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComUnknown Pattern >>
    MX_DECLARE_IECOMUNKNOWN
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << IXmlDocument interface >>

    virtual mxt_result SetAllocator(IN IAllocator* pAllocator);
    virtual void SetPatchManager(IN IXmlPatchMgr* pMgr);
    virtual IXmlPatchMgr* GetPatchManager();
    virtual void SetDocumentManager(IN IXmlDocumentMgr* pMgr);
    virtual IXmlDocumentMgr* GetDocumentManager();
    virtual void SetOpaque(mxt_opaque opqData);
    virtual mxt_opaque GetOpaque();
    virtual CXmlElement* CreateRootElement(IN const char* pszNamespace,
                                           IN const char* pszNamespacePrefix,
                                           IN const char* pszElementName);
    virtual CXmlElement* GetRootElement();
    virtual const CXmlElement* GetRootElement() const;
    virtual void DeleteRootElement();
    virtual mxt_result Parse(IN const uint8_t* puXmlBuffer,
                             IN const unsigned int uBufferSize);
    virtual mxt_result Serialize(INOUT CBlob& rBlob);
    virtual mxt_result SetDictionary(const CVector<const char*>* pvecpszDictionary);
    virtual const CVector<const char*>* GetDictionary();
    virtual void SetCaseSensitive(IN bool bCaseSensitive);
    virtual bool IsCaseSensitive();

    // Internal methods.
    virtual uint8_t* GO AllocateMemory(IN unsigned int uAllocationSize);
    virtual void ReleaseMemory(IN TOA uint8_t* pMemoryBlock);
    virtual void AllocateString(OUT GO char*& rpszDestination,
                                IN const char* pszSource);
    virtual void ReleaseString(IN TOA char* pszString);

    //-- << IXmlParserMgr interface >>

    virtual void EvStartElement(IN const char* pszNamespaceUri,
                                IN const char* pszElementName,
                                IN const char** ppAttributes,
                                IN const char** ppNewNamespaces);
    virtual void EvEndElement(IN const char* pszNamespaceUri,
                              IN const char* pszElementName);
    virtual void EvCharacterData(IN const char* pcText,
                                 IN const unsigned int uTextSize);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CXmlDocument();

    // Summary:
    //  Destructor.
    virtual ~CXmlDocument();

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CXmlDocument(IN const CXmlDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    CXmlDocument& operator=(IN const CXmlDocument& rSrc);

//-- Hidden Data Members.
protected:
private:
    // The document allocator if used.
    IAllocator* m_pAllocator;
    // The root element of the document.
    CXmlElement* m_pRootElement;

    // The dictionary to use for element names, attribute names and namespace
    // URIs.
    const CVector<const char*>* m_pvecpszDictionary;

    // The working buffer when parsing.
    CString* m_pstrWorkingBuffer;
    // The current element to take action upon when parsing.
    CXmlElement* m_pWorkingElement;

    // The opaque identifier of this document.
    mxt_opaque m_opq;

    // The document patch's manager.
    IXmlPatchMgr* m_pPatchMgr;

    // The document manager.
    IXmlDocumentMgr* m_pDocumentMgr;
    
    bool m_bIsCaseSensitive;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif //-- #if defined(MXD_XML_ENABLE_SUPPORT) && defined(MXD_ECOM_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CXMLDOCUMENT_H
