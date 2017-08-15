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
#ifndef MXG_CSCEPIDFDOCUMENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEPIDFDOCUMENT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEPIDFDOCUMENT_H
#include "SceXml/IScePidfDocument.h"
#endif

#ifndef MXG_ISCEXMLDOCUMENT_H
#include "SceXml/ISceXmlDocument.h"
#endif

#ifndef MXG_UAIMPPXMLDICTIONARY_H
#include "SceXml/UaImppXmlDictionary.h"
#endif

#ifndef MXG_CTIME_H
#include "Time/CTime.h"
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

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CScePidfDocument
//========================================
//
// Summary:
//  This class implements the PIDF document.
//
// Description:
//  This class implements the IScePidfDocument interface which represents presence
//  information of a single user
//
// Location:
//   SceXml/CScePidfDocument.h
//
// See Also:
//   CScePidfService, CScePidfDevice, CScePidfPerson
//
//==============================================================================
class CScePidfDocument : protected CEComDelegatingUnknown,
                         private IScePidfDocument,
                         private ISceXmlDocument
{
//-- Friend Declarations.
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

    //-- << IScePidfDocument >>
    // Summary:
    //  Sets the entity associated with this document.
    virtual mxt_result SetEntity(IN IUri* pEntityUri);

    // Summary:
    //  Sets the entity associated with this document.
    virtual mxt_result SetEntity(IN const char* pszEntityUri);
    
    // Summary:
    //  Gets the entity associated with this document.
    virtual GO IUri* GetEntityUri();
    
    // Summary:
    //  Gets the entity associated with this document.
    virtual const char* GetEntity();

    // Summary:
    //  Gets a pointer to all Service elements held by this PIDF document.
    virtual mxt_result GetServices(OUT CVector<CXmlElement*>& rvpServices);
    
    // Summary:
    //  Adds a service (or a tuple) element to the PIDF document.
    virtual CXmlElement* AddService();

    // Summary:
    //  Gets a pointer to all Person elements held by this PIDF document.
    virtual mxt_result GetPersons(OUT CVector<CXmlElement*>& rvpPersons);

    // Summary:
    //  Adds a Person element to the PIDF document.
    virtual CXmlElement* AddPerson();

    // Summary:
    //  Gets a pointer to all Device elements held by this PIDF document.
    virtual mxt_result GetDevices(OUT CVector<CXmlElement*>& rvpDevices);

    // Summary:
    //  Adds a Device element to the PIDF document.
    virtual CXmlElement* AddDevice(IN const char* pszDeviceId);

    // Summary:
    //  Gets a pointer to all Note elements held by this PIDF document.
    virtual mxt_result GetNotes(OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the document.
    virtual CXmlElement* AddNote(IN const char* pszUtf8EncodedNote,
                                 IN const char* pszLanguageTag);

    // Summary:
    //  Serializes the XML document into a MIME object.
    virtual void SerializePidfFull(IN unsigned int uVersion, OUT GO CMimeObject*& rpMimeObject);

    // Summary:
    //  Serializes the XML document into a CSipMessageBody object.
    virtual void SerializePidfFull(IN unsigned int uVersion, OUT GO CSipMessageBody*& rpMessageBody);

    // Summary:
    //  Serializes the XML in a blob.
    virtual void SerializePidfFull(IN unsigned int uVersion, INOUT CBlob& rBlob);

    // Summary:
    //  Finds and parses the PIDF found in the MIME object.
    virtual mxt_result Parse(IN const CMimeObject& rMimeObject);

    // Summary:
    //  Serializes the PIDF into a MIME object.
    virtual void Serialize(OUT GO CMimeObject*& rpMimeObject);

    // Summary:
    //  Finds and parses the PIDF found in the CSipMessageBody object.
    virtual mxt_result Parse(IN const CSipMessageBody& rMessageBody);

    // Summary:
    //  Serializes the PIDF into a CSipMessageBody object.
    virtual void Serialize(OUT GO CSipMessageBody*& rpMessageBody);

    // Summary:
    //  Parses the PIDF from a pointer to the actual XML data.    
    virtual mxt_result Parse(INOUT const char*& rpcPos, IN unsigned int uLength = 0);    

    // Summary:
    //  Serializes the PIDF in a blob.
    virtual void Serialize(INOUT CBlob& pBlob);

    // Summary:
    //  Checks if the document and associated allocator need to be cleaned-up
    //  and cleans it them up if necessary.
    virtual mxt_result VerifyDocumentCleanup();

    // Summary:
    //  Verifies if the PIDF document can optimize the memory it uses.
    virtual bool IsCleanupRequired(OUT unsigned int& ruNumBytesSaved);
    
    // Summary:
    //  Optimizes the memory used by the PIDF document.
    virtual mxt_result Cleanup();

//-- Hidden Methods.
protected:
    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CScePidfDocument.
    static mxt_result InitializeCScePidfDocument();

    // Summary:
    //  Finalizes the CScePidfDocument
    static void FinalizeCScePidfDocument();

    //-- << Constructors / Destructors / Operators >>
    // Summary:
    //  Default Constructor.
    CScePidfDocument(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CScePidfDocument();

private:

    // Summary:
    //  Validates that the root name is pidf-full or presence.
    mxt_result ValidateParseHelper();

    // Summary:
    //  This method tries to find a PIDF message in the given CMimeObject.
    const CMimeObject* FindPidfMessage(IN const CMimeObject* pObject);

    // Summary:
    //  This method tries to find a PIDF message in the given CSipMessageBody
    //  object.
    const CSipMessageBody* FindPidfMessage(IN const CSipMessageBody* pObject);


//-- Hidden Data Members.
protected:
private:

    // ECom unknown for the iXML document used to hold the PIDF information.
    IEComUnknown* m_pXmlDocumentUnknown;

    // XML document used to hold the PIDF information.
    IXmlDocument* m_pXmlDocument;

    // Allocator used for the XML document.
    CSubAllocator* m_pAllocator;

    // The allocation block size.
    unsigned int m_uBlockSize;

    // Vector representing the sequence for the <presence> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecPresenceSequence;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEPIDFDOCUMENT_H

//M5T_INTERNAL_USE_END

