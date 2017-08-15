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
#ifndef MXG_CSCEPRESENCEAUTHORIZATIONDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEPRESENCEAUTHORIZATIONDOCUMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEPRESENCEAUTHORIZATIONDOCUMENT_H
#include "SceXml/IScePresenceAuthorizationDocument.h"
#endif

#ifndef MXG_ISCEXMLDOCUMENT_H
#include "SceXml/ISceXmlDocument.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;
class CSipMessageBody;
class CSubAllocator;
class CXmlElement;
class IXmlDocument;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CScePresenceAuthorizationDocument
//============================================
//
// Description:
//  The CScePresenceAuthorizationDocument class defines the API needed to
//  create and manipulate a presence authorization document, as defined in RFCs
//  4745 and 5025.
//
// Location:
//   SceXml/CScePresenceAuthorizationDocument.h
//
// See Also:
//   CScePresenceAuthorizationRule
//
//==============================================================================
class CScePresenceAuthorizationDocument : protected CEComDelegatingUnknown,
                                          private IScePresenceAuthorizationDocument,
                                          private ISceXmlDocument
{
//-- Friend Declarations.
    friend class CSceUaImppInitializer;


//-- Hidden Methods
//-------------------
private:
    //-- << ECom Mechanism >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);
    virtual mxt_result InitializeInstance();

    // Summary:
    //  Adds a rule to the document.
    virtual CXmlElement* AddRule();

    // Summary:
    //  Gets the rules held by the document.
    virtual mxt_result GetRules(OUT CVector<CXmlElement*>& rvpRule);

    // Summary:
    //  Finds and parses the PIDF found in the MIME object.
    virtual mxt_result Parse(IN const CMimeObject& rMimeObject);

    // Summary:
    //  Serializes the PIDF into a MIME object.
    virtual void Serialize(OUT GO CMimeObject*& rpMimeObject);

    // Summary:
    //  Finds and parses the authorization document found in the CSipMessageBody object.
    virtual mxt_result Parse(IN const CSipMessageBody& rMessageBody);

    // Summary:
    //  Serializes the authorization document into a CSipMessageBody object.    
    virtual void Serialize(OUT GO CSipMessageBody*& rpMessageBody);

    // Summary:
    //  Parses the authorization document from a pointer to the actual XML data.    
    virtual mxt_result Parse(INOUT const char*& rpcPos, IN unsigned int uLength = 0);    

    // Summary:
    //  Serializes the authorization document in a blob.
    virtual void Serialize(INOUT CBlob& rBlob);

    virtual mxt_result VerifyDocumentCleanup();

    // Summary:
    //  Verifies if the authorization document can optimize the memory it uses.
    bool IsCleanupRequired(OUT unsigned int& ruNumBytesSaved);
    
    // Summary:
    //  Optimizes the memory used by the authorization document.
    mxt_result Cleanup();

private:

    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CScePresenceAuthorizationDocument.
    static mxt_result InitializeCScePresenceAuthorizationDocument();

    // Summary:
    //  Finalizes the CScePresenceAuthorizationDocument.
    static void FinalizeCScePresenceAuthorizationDocument();

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CScePresenceAuthorizationDocument(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    //--------------
    ~CScePresenceAuthorizationDocument();

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CScePresenceAuthorizationDocument(IN const CScePresenceAuthorizationDocument& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CScePresenceAuthorizationDocument& operator=(IN const CScePresenceAuthorizationDocument& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

    // ECom unknown for the iXML document used to hold the PIDF information.
    IEComUnknown* m_pXmlDocumentUnknown;

    // XML document used to hold the auth-policy information.
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

#endif //-- #ifndef MXG_CSCEPRESENCEAUTHORIZATIONDOCUMENT_H

//M5T_INTERNAL_USE_END
