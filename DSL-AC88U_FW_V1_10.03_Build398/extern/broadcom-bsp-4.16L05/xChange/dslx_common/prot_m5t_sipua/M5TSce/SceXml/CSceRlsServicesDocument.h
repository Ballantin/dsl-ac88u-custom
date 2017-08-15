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
#ifndef MXG_CSCERLSSERVICESDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCERLSSERVICESDOCUMENT_H

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

#ifndef MXG_ISCERLSSERVICESDOCUMENT_H
#include "SceXml/ISceRlsServicesDocument.h"
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
//== Class: CSceRlsServicesDocument
//========================================
//
// Description:
//  This class implements a RLS Service document. This document is used to
//  define services (identified by an URI) provided by a Resource List Server
//  (RLS).
//
// See Also:
//   CSceResourceList
//
//==EDOC========================================================================
class CSceRlsServicesDocument : protected CEComDelegatingUnknown,
                                private ISceRlsServicesDocument,
                                private ISceXmlDocument
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    // Summary:
    //  Initializes the CSceRlsServicesDocument.
    static mxt_result InitializeCSceRlsServicesDocument();

    // Summary:
    //  Finalizes the CSceRlsServicesDocument
    static void FinalizeCSceRlsServicesDocument();

    // Summary:
    //  Adds a service that identifies its resources through a XCAP reference
    //  to a resource list.
    CXmlElement* AddService(IN const char* pszServiceUri,
                            IN const char* pszListReferenceUri,
                            IN uint32_t uSipEventBitset);

    // Summary:
    //  Adds a service that identifies its resources through the inclusion of a
    //  resource list.
    CXmlElement* AddService(IN const char* pszServiceUri,
                            IN uint32_t uSipEventBitset);

    // Summary:
    //  Gets the service elements found in this document.
    mxt_result GetServices(OUT CVector<CXmlElement*>& rvpService);


    // Summaty:
    //  Gets Service iformation about the resource list.
    EServiceInfo GetServiceInfo(IN CXmlElement* pServiceElement);

    // Summary:
    //  Gets the events supported by this service element.
    mxt_result GetServiceEvents(IN CXmlElement* pServiceElement,
                                OUT uint32_t& rSipEventBitset);

    // Summary:
    //  Gets the URI associated with thi service.
    mxt_result GetServiceUri(IN CXmlElement* pServiceElement,
                             OUT const char*& rpszServiceUri);

    // Summary:
    //  Gets the reference URI that points to the resource list assigned to
    //  this service.
    mxt_result GetServiceResourceListReference(IN CXmlElement* pServiceElement,
                                               OUT const char*& rpszListReferenceUri);

    // Summary:
    //  Gets the resource list held by the service element.
    CXmlElement* GetServiceResourceList(IN CXmlElement* pServiceElement);

    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result InitializeInstance();

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

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
    CSceRlsServicesDocument(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    //--------------
    ~CSceRlsServicesDocument();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSceRlsServicesDocument(IN const CSceRlsServicesDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSceRlsServicesDocument& operator=(IN const CSceRlsServicesDocument& rSrc);

    // Summary:
    //  Add a service element to the document.
    CXmlElement* AddServiceHelper(IN const char* pszServiceUri,
                                  IN const char* pszListReferenceUri,
                                  IN uint32_t uSipEventBitset);

    // Summary:
    //  Add the service types specified by uSipEventBitset to the <service>
    //  element pointed by pService.
    CXmlElement* AddPackagesHelper(IN CXmlElement*& pService,
                                   IN uint32_t uSipEventBitset);

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

#endif //-- #ifndef MXG_CSCERLSSERVICESDOCUMENT_H

//M5T_INTERNAL_USE_END
