//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_CSCERESOURCELISTSDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCERESOURCELISTSDOCUMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IRESOURCELISTDOCUMENT_H
#include "SceXml/ISceResourceListsDocument.h"
#endif

#ifndef MXG_ISCEXMLDOCUMENT_H
#include "SceXml/ISceXmlDocument.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;
class CMimeObject;
class CXmlElement;
class IXmlDocument;
class CSubAllocator;
class CSipMessageBody;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSceResourceListsDocument
//==============================================================================
//
// Summary:
//  This class implements a Resource List document.
//
// Description:
//  The Resource Lists Document implements a document that manages zero or more
//  CSceResourceList. Each resource may contain zero or more resources which
//  are identified as URIs. The resource list document can be used to represent
//  a buddy list or a contact list.
//
//
// Location:
//   SceXml/CSceResourceListsDocument.h
//
// See Also:
//   CSceResourceList
//
//==EDOC========================================================================
class CSceResourceListsDocument : protected CEComDelegatingUnknown,
                                  private ISceResourceListsDocument,
                                  private ISceXmlDocument
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << IResourceListDocument Interface >>

    // Summary:
    //  Adds a resource list to the document.
    virtual CXmlElement* AddResourceList(const char* pszListName = NULL);

    // Summary:
    //  Gets all resource list elements held by this document.
    virtual mxt_result GetResourceLists(OUT CVector<CXmlElement*>& rvpLists);

    //-- << Initialization mechanism >>

    // Summary:
    //  Initializes the CSceResourceListsDocument.
    static mxt_result InitializeCSceResourceListsDocument();

    // Summary:
    //  Finalizes the CSceResourceListsDocument
    static void FinalizeCSceResourceListsDocument();

    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);
    virtual mxt_result InitializeInstance();

    //-- << ISceXmlDocument Interface >>

    //  Summary:
    //      Finds and parses the XML document found in the MIME object.
    virtual mxt_result Parse(IN const CMimeObject& rMimeObject);

    //  Summary:
    //      Serializes the XML document into a MIME object.
    virtual void Serialize(OUT GO CMimeObject*& rpMimeObject);

    //  Summary:
    //      Finds and parses the authorization document found in the
    //      CSipMessageBody object.
    virtual mxt_result Parse(IN const CSipMessageBody& rMessageBody);

    //  Summary:
    //      Serializes the XML document into a CSipMessageBody object.
    virtual void Serialize(OUT GO CSipMessageBody*& rpMessageBody);

    //  Summary:
    //      Parses the XML document from a pointer to the actual XML data.
    virtual mxt_result Parse(INOUT const char*& rpcPos, IN unsigned int uLength = 0);

    //  Summary:
    //      Serializes the XML in a blob.
    virtual void Serialize(INOUT CBlob& rBlob);

    //  Summary:
    //      Checks if the document and associated allocator need to be
    //      cleaned-up and cleans them up if necessary.
    virtual mxt_result VerifyDocumentCleanup();

    // Summary:
    //      Verifies if the XML document can optimize the memory it uses.
    virtual bool IsCleanupRequired(OUT unsigned int& ruNumBytesSaved);

    // Summary:
    //      Optimizes the memory used by the XML document.
    virtual mxt_result Cleanup();


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSceResourceListsDocument(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Warning: Destructor is not virtual to keep this class as lightweight as
    // possible. Keep this in mind if inheriting from this class.
    //-------------------------------------------------------------------------

    // Summary:
    //  Destructor.
    //--------------
    ~CSceResourceListsDocument();

private:
    //-- << Deactivated  Operators >>
    //-------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSceResourceListsDocument(IN const CSceResourceListsDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSceResourceListsDocument& operator=(IN const CSceResourceListsDocument& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:
    // ECom unknown for the iXML document used to hold the Resource List
    // information.
    IEComUnknown* m_pXmlDocumentUnknown;
    // The resource list document.
    IXmlDocument*   m_pXmlDocument;
    // The subAllocator used by the document.
    CSubAllocator*  m_pAllocator;
    unsigned int m_uBlockSize;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCERESOURCELISTSDOCUMENT_H

//M5T_INTERNAL_USE_END
