//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CUAIMPPPRESENCEPUBLISHER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUAIMPPPRESENCEPUBLISHER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_ISCEPUBLISHERMGR_H
#include "SceCoreComponents/IScePublisherMgr.h"
#endif

#ifndef MXG_IUAIMPPPRESENCEPUBLISHER_H
#include "SceUaImpp/IUaImppPresencePublisher.h"
#endif

#ifndef MXG_IUAIMPPPRESENCEPUBLISHERCONFIG_H
#include "SceUaImpp/IUaImppPresencePublisherConfig.h"
#endif

#ifndef MXG_ISCEXMLDOCUMENT_H
#include "SceXml/ISceXmlDocument.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSceXmlPatchOperations;
class IUaImppPresencePublisherConfig;
class IUaImppPresencePublisherMgr;
class IScePublisher;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CUaImppPresencePublisher
//==============================================================================
//
// Description:
//   This class implements the IUaImppPresencePublisher interface.
//
//   It is responsible for client PUBLISH requests for presence.
//
// Location:
//   SceUaImpp/CUaImppPresencePublisher.h
//
// See Also:
//   IUaImppPresencePublisher
//
//==============================================================================
class CUaImppPresencePublisher : private CEComDelegatingUnknown,
                                 protected IUaImppPresencePublisher,
                                 protected IScePublisherMgr
{
//-- Friend Declarations
friend class CSceUaImppInitializer;

//-- New types within class namespace
public:
protected:
private:

    // Description:
    //  These are the publisher states.
    enum EState
    {
        // Description:
        //  Steady state. This is the initial state, the publisher is initialized
        //  but no PUBLISH has been sent yet. The publisher remains in this state
        //  until the first call to PublishPresenceState() made by the application.
        eSTATE_INITIAL   = 0,

        // Description:
        //  This is a steady state, the publication is still active on the server.
        eSTATE_PUBLISHED,

        // Description:
        //  Final state. The publication has been terminated.
        eSTATE_TERMINATED
    };

//-- Published Interface
public:

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CUaImppPresencePublisher(IN IEComUnknown* pOuterIEComUnknown);

    // Summary:
    //  Destructor.
    virtual ~CUaImppPresencePublisher();


    //-- << Initialization mechanism >>
    static mxt_result InitializeCUaImppPresencePublisher();

    static void FinalizeCUaImppPresencePublisher();


    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    virtual mxt_result InitializeInstance();

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    //-- << IUaImppPresencePublisher >>
    virtual mxt_result SetManager(IN IUaImppPresencePublisherMgr* pMgr);

    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual mxt_result GetPidfDocument(INOUT IScePidfDocument** ppDocument);

    virtual mxt_result PublishPresenceState(bool bForceFullState);

    virtual mxt_result Terminate();

    virtual mxt_result Abort();

    //-- << IScePublisherMgr >>
    virtual void EvPublicationStatus(IN IScePublisher* pPublisher,
                                     IN EPublicationStatus eStatus);
    
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CUaImppPresencePublisher(IN const CUaImppPresencePublisher& rSrc);

    // Summary:
    //  Assignment Operator.
    CUaImppPresencePublisher& operator=(IN const CUaImppPresencePublisher& rSrc);


    //-- << Helper methods >>

    // Summary:
    //  Serializes the document to be sent in the Message Body.
    mxt_result SerializePidfDocument(IN bool bSendPidfFull,
                                     OUT CSipMessageBody*& rpMessageBody);

    // Summary:
    //  Transforms the patch document into a "pidf-diff" document.
    mxt_result UpdatePidfDiff(INOUT IXmlDocument*& rpPatchDocument);

    // Summary:
    //  Configures the publisher component.
    void ConfigurePublisher(IN ISceUserConfig* pUserConfig);

//-- Hidden Data Members
protected:
private:

    // The manager of this object.
    IUaImppPresencePublisherMgr* m_pMgr;

    // The publisher used by this component.
    IScePublisher* m_pPublisher;

    // Publisher state.
    EState m_eState;

    // The user's PIDF document.
    IScePidfDocument* m_pPidfDocument;

    // The patchOperation object acting as the PatchMgr of the document.
    CSceXmlPatchOperations* m_pXmlPatchOperation;

    // The publication mode.
    IUaImppPresencePublisherConfig::EPublicationMode m_ePublicationMode;

    // The size of the last sent "pidf-full" document.
    unsigned int m_uLastSentFullDocumentSize;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CUAIMPPPRESENCEPUBLISHER_H

//M5T_INTERNAL_USE_END
