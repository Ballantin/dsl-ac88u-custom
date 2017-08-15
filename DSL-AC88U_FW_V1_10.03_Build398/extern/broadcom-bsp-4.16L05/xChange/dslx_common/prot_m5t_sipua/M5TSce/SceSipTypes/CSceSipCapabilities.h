//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCESIPCAPABILITIES_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCESIPCAPABILITIES_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPEVENTTYPES_H
#include "SceSipTypes/SipEventTypes.h"
#endif

#ifndef MXG_SIPMEDIATYPES_H
#include "SceSipTypes/SipMediaTypes.h"
#endif

#ifndef MXG_SIPMETHODTYPES_H
#include "SceSipTypes/SipMethodTypes.h"
#endif

#ifndef MXG_SIPOPTIONTAGS_H
#include "SceSipTypes/SipOptionTags.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHeaderList;
class CSipHeader;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//== Class: CSceSipCapabilities
//====================================
//
// Summary:
//  The SceSipCapabilities uses the local capabilities and peer capabilities to
//  manage a list of commonly supported capabilities between the two endpoints.
//
// Description:
//  The SceSipCapabilities uses the local capabilities and peer
//  capabilities to manage a list of commonly supported capabilities between
//  the two endpoints.
//
//  This implementation assumes that a peer supports everything until the
//  appropriate SetPeerSupportedXyz methods are called.
//
// Location:
//   SceSipTypes/CSceSipCapabilities.h
//
//==============================================================================
class CSceSipCapabilities
{
    
    //-- New types within class namespace
public:
    // Description:
    //  This enum defines SIP headers types that GetCapabilitiesHeaders can
    //  return.
    enum ECapsType
    {
        // Accept headers.
        eCAPS_ACCEPT        = 0x00000001,

        // Allow headers.
        eCAPS_ALLOW         = 0x00000002,

        // Allow-Events header.
        eCAPS_ALLOW_EVENT   = 0x00000004,

        // Supported headers.
        eCAPS_SUPPORTED     = 0x00000008
    };

protected:
private:



//-- Published Interface.
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSceSipCapabilities();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSceSipCapabilities();


    // Summary:
    //  Configures the methods supported by the local UA.
    //----------------------------------------------------
    void SetSupportedMethods(IN uint32_t uMethods);

    // Summary:
    //  Configures the extensions supported by the local UA.
    //-------------------------------------------------------
    void SetSupportedExtensions(IN uint32_t uExtensions);

    // Summary:
    //  Configures the payloads supported by the local UA.
    //-----------------------------------------------------
    void SetSupportedPayloads(IN unsigned int uPayloads);

    // Summary:
    //  Configures the events supported by the local UA.
    //---------------------------------------------------
    void SetSupportedEvents(IN uint32_t uEvents);

    // Summary:
    //  Configures the methods supported by the peer UA.
    //---------------------------------------------------
    mxt_result SetPeerSupportedMethod(IN const CSipHeader* pAllowHeader);

    // Summary:
    //  Configures the extensions supported by the peer UA.
    //------------------------------------------------------
    mxt_result SetPeerSupportedExtension(IN const CSipHeader* pSupportedHeader);

    // Summary:
    //  Configures the extensions required by the peer UA.
    //-----------------------------------------------------
    mxt_result SetPeerRequiredExtension(IN const CSipHeader* pRequireHeader,
                                        OUT GO CSipHeader*& rpHdrUnsupported);

    // Summary:
    //  Verifies that the extensions required by the peer UA are supported
    //  locally.
    //---------------------------------------------------------------------
    mxt_result VerifyPeerRequiredExtensions(IN const CSipHeader* pRequireHeader,
                                            OUT GO CSipHeader*& rpHdrUnsupported,
                                            OUT uint32_t* puPeerRequiredExtensions = NULL) const;

    // Summary:
    //  Configures the payloads supported by the peer UA.
    //----------------------------------------------------
    mxt_result SetPeerSupportedPayload(IN const CSipHeader* pAcceptHeader);

    // Summary:
    //  Configures the events supported by the peer UA.
    //--------------------------------------------------
    mxt_result SetPeerSupportedEvent(IN const CSipHeader* pAllowEventHeader);

    // Summary:
    //  Adds methods supported by the local UA.
    //------------------------------------------
    void AddSupportedMethods(IN uint32_t uMethods);

    // Summary:
    //  Adds extensions supported by the local UA.
    //---------------------------------------------
    void AddSupportedExtensions(IN uint32_t uExtensions);

    // Summary:
    //  Adds payloads supported by the local UA.
    //-------------------------------------------
    void AddSupportedPayloads(IN unsigned int uPayloads);

    // Summary:
    //  Adds events supported by the local UA.
    //-----------------------------------------
    void AddSupportedEvents(IN uint32_t uEvents);

    // Summary:
    //  Removes methods supported by the local UA.
    //---------------------------------------------
    void RemoveSupportedMethods(IN uint32_t uMethods);

    // Summary:
    //  Remove extensions supported by the local UA.
    //-----------------------------------------------
    void RemoveSupportedExtensions(IN uint32_t uExtensions);

    // Summary:
    //  Removes payloads supported by the local UA.
    //----------------------------------------------
    void RemoveSupportedPayloads(IN unsigned int uPayloads);

    // Summary:
    //  Removes events supported by the local UA.
    //--------------------------------------------
    void RemoveSupportedEvents(IN uint32_t uEvents); 

    // Summary:
    //  Verifies if both endpoints support the specified method.
    //-----------------------------------------------------------
    bool IsSupported(IN ESipMethodType eMethod) const;

    // Summary:
    //  Verifies if both endpoints support the specified option tag.
    //---------------------------------------------------------------
    bool IsSupported(IN ESipOptionTag eTag) const;

    // Summary:
    //  Verifies if both endpoints support the specified media type.
    //---------------------------------------------------------------
    bool IsSupported(IN ESipMediaType eMedia) const;

    // Summary:
    //  Verifies if both endpoints support the specified event.
    //----------------------------------------------------------
    bool IsSupported(IN ESipEventType eEvent) const;

    // Summary:
    //  Verifies if both endpoints support the specified option tag with the
    //  peer requiring support.
    //-----------------------------------------------------------------------
    bool IsRequired(IN ESipOptionTag eTag) const;

    // Summary:
    //  Verifies if the local capabilities support the specified method.
    //-------------------------------------------------------------------
    bool IsSupportedLocal(IN ESipMethodType eMethod) const;

    // Summary:
    //  Verifies if the local capabilities support the specified extension.
    //----------------------------------------------------------------------
    bool IsSupportedLocal(IN ESipOptionTag eTag) const;

    // Summary:
    //  Verifies if the local capabilities support the specified payload type.
    //-------------------------------------------------------------------------
    bool IsSupportedLocal(IN ESipMediaType eMedia) const;

    // Summary:
    //  Verifies if the local capabilities support the specified event type.
    //-----------------------------------------------------------------------
    bool IsSupportedLocal(IN ESipEventType eEvent) const;

    // Summary:
    //  Generates Allow headers according to the local capabilities.
    //---------------------------------------------------------------
    GO CSipHeader* GetAllowHeaders() const;

    // Summary:
    //  Generates Allow-Events headers according to the local capabilities.
    //----------------------------------------------------------------------
    GO CSipHeader* GetAllowEventsHeaders() const;

    // Summary:
    //  Generates Supported headers according to the local capabilities.
    //-------------------------------------------------------------------
    GO CSipHeader* GetSupportedHeaders() const;

    // Summary:
    //  Generates Accept headers according to the local capabilities.
    //----------------------------------------------------------------
    GO CSipHeader* GetAcceptHeaders() const;

    // Summary:
    //  Gets the capabilities headers defined by ubsWhatCaps.
    void GetCapabilitiesHeaders(IN uint32_t ubsWhatCaps,
                                INOUT CHeaderList& rHeaderList) const;

    // Summary:
    //  Generates Require headers according to the parameter.
    //--------------------------------------------------------
    static GO CSipHeader* GetRequireHeaders(IN uint32_t uExtensions);


//-- Hidden Methods.
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSceSipCapabilities(IN const CSceSipCapabilities& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSceSipCapabilities& operator=(IN const CSceSipCapabilities& rSrc);


//-- Hidden Data Members.
//------------------------
protected:
private:

    uint32_t    m_uPeerMethods;
    uint32_t    m_uPeerExtensions;
    uint32_t    m_uPeerPayloads;
    uint32_t    m_uPeerEvents;
    uint32_t    m_uPeerRequiredExtensions;

    uint32_t    m_uLocalMethods;
    uint32_t    m_uLocalExtensions;
    uint32_t    m_uLocalPayloads;
    uint32_t    m_uLocalEvents;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  SetSupportedMethods
//==
//==============================================================================
//
//  Parameters:
//    uMethods:
//      Methods to support according to ESipMethodTypes enumeration.
//
//  Description:
//      This method sets the method supported by the local host.
//
//  See Also:
//      GetSupportedMethods, ESipMethodTypes
//
//==============================================================================
inline void CSceSipCapabilities::SetSupportedMethods(IN uint32_t uMethods)
{
    m_uLocalMethods = uMethods;
}

//==============================================================================
//==
//==  SetSupportedExtensions
//==
//==============================================================================
//
//  Parameters:
//    uExtensions:
//      Extensions to support according to ESipOptionTag enumeration.
//
//  Description:
//      This method sets the extensions supported by the local host.
//
//  See Also:
//      GetSupportedExtensions, ESipOptionTag
//
//==============================================================================
inline void CSceSipCapabilities::SetSupportedExtensions(IN uint32_t uExtensions)
{
    m_uLocalExtensions = uExtensions & ~eOPTIONTAG_LAST;
}

//==============================================================================
//==
//==  SetSupportedPayloads
//==
//==============================================================================
//
//  Parameters:
//    uPayloads:
//      Payloads to support according to ESipMediaTypes enumeration.
//
//  Description:
//      This method sets the payloads supported by the local host.
//
//  See Also:
//      GetSupportedPayloads, ESipMediaTypes
//
//==============================================================================
inline void CSceSipCapabilities::SetSupportedPayloads(IN unsigned int uPayloads)
{
    m_uLocalPayloads = uPayloads;
}

//==============================================================================
//==
//==  SetSupportedEvents
//==
//==============================================================================
//
//  Parameters:
//    uEvents:
//      Events to support according to ESipEventTypes enumeration.
//
//  Description:
//      This method sets the events supported by the local host.
//
//  See Also:
//      GetSupportedEvents, ESipEventTypes
//
//==============================================================================
inline void CSceSipCapabilities::SetSupportedEvents(IN uint32_t uEvents)
{
    m_uLocalEvents = uEvents;
}

//==========================================================================
//==
//==  AddSupportedMethods
//==
//==========================================================================
//
//  Parameters:
//    uMethods:
//      Methods to add according to ESipMethodType enumeration.
//
//  Description:
//      This method adds the methods supported by the local host.
//
//  See Also:
//      ESipMethodType, RemoveSupportedMethods 
//
//==========================================================================
inline void CSceSipCapabilities::AddSupportedMethods(IN uint32_t uMethods)
{
    m_uLocalMethods |= uMethods;
}

//==========================================================================
//==
//==  AddSupportedExtensions
//==
//==========================================================================
//
//  Parameters:
//    uExtensions:
//      Extensions to add according to ESipOptionTag enumeration.
//
//  Description:
//      This method adds the extensions supported by the local host.
//
//  See Also:
//      ESipOptionTag, RemoveSupportedExtensions 
//
//==========================================================================
inline void CSceSipCapabilities::AddSupportedExtensions(IN uint32_t uExtensions)
{
    m_uLocalExtensions |= uExtensions & ~eOPTIONTAG_LAST;
}

//==========================================================================
//==
//==  AddSupportedPayloads
//==
//==========================================================================
//
//  Parameters:
//    uPayloads:
//      Payloads to add according to ESipMediaType enumeration.
//
//  Description:
//      This method adds the payloads supported by the local host.
//
//  See Also:
//      ESipMediaType, RemoveSupportedPayloads
//
//==========================================================================
inline void CSceSipCapabilities::AddSupportedPayloads(IN unsigned int uPayloads)
{
    m_uLocalPayloads |= uPayloads;
}

//==========================================================================
//==
//==  AddSupportedEvents
//==
//==========================================================================
//
//  Parameters:
//    uEvents:
//      Events to add according to ESipEventType enumeration.
//
//  Description:
//      This method adds the events supported by the local host.
//
//  See Also:
//      ESipEventType, RemoveSupportedEvents
//
//==========================================================================
inline void CSceSipCapabilities::AddSupportedEvents(IN uint32_t uEvents)
{
    m_uLocalEvents |= uEvents;
}

//==========================================================================
//==
//==  RemoveSupportedMethods
//==
//==========================================================================
//
//  Parameters:
//    uMethods:
//      Methods to remove according to ESipMethodType enumeration.
//
//  Description:
//      This method removes the methods supported by the local host.
//
//  See Also:
//      ESipMethodType, AddSupportedMethods 
//
//==========================================================================
inline void CSceSipCapabilities::RemoveSupportedMethods(IN uint32_t uMethods)
{
    m_uLocalMethods &= ~uMethods;
}

//==========================================================================
//==
//==  RemoveSupportedExtensions
//==
//==========================================================================
//
//  Parameters:
//    uExtensions:
//      Extensions to remove according to ESipOptionTag enumeration.
//
//  Description:
//      This method removes the extensions supported by the local host.
//
//  See Also:
//      ESipOptionTag, AddSupportedExtensions 
//
//==========================================================================
inline void CSceSipCapabilities::RemoveSupportedExtensions(IN uint32_t uExtensions)
{
    m_uLocalExtensions &= ~uExtensions;
}

//==========================================================================
//==
//==  RemoveSupportedPayloads
//==
//==========================================================================
//
//  Parameters:
//    uPayloads:
//      Payloads to remove according to ESipMediaType enumeration.
//
//  Description:
//      This method removes the payloads supported by the local host.
//
//  See Also:
//      ESipMediaType, AddSupportedPayloads
//
//==========================================================================
inline void CSceSipCapabilities::RemoveSupportedPayloads(IN unsigned int uPayloads)
{
    m_uLocalPayloads &= ~uPayloads;
}

//==========================================================================
//==
//==  RemoveSupportedEvents
//==
//==========================================================================
//
//  Parameters:
//    uEvents:
//      Events to remove according to ESipEventType enumeration.
//
//  Description:
//      This method removes some events supported by the local host.
//
//  See Also:
//      ESipEventType, AddSupportedEvents
//
//==========================================================================
inline void CSceSipCapabilities::RemoveSupportedEvents(IN uint32_t uEvents)
{
    m_uLocalEvents &= ~uEvents;
}

//==============================================================================
//==
//==  IsSupported
//==
//==============================================================================
//
//  Parameters:
//    eMethod:
//      Method to check the support of.
//
//  Returns:
//      True if the method is supported, false otherwise.
//
//  Description:
//      This method checks if the method is supported by both the local
//      and remote hosts.
//
//==============================================================================
inline bool CSceSipCapabilities::IsSupported(IN ESipMethodType eMethod) const
{
    return ((m_uLocalMethods & m_uPeerMethods) & eMethod) > 0;
}

//==============================================================================
//==
//==  IsSupported
//==
//==============================================================================
//
//  Parameters:
//    eTag:
//      Extension to check the support of.
//
//  Returns:
//      True if the extension is supported, false otherwise.
//
//  Description:
//      This method checks if the extension is supported by both the local
//      and remote hosts.
//
//==============================================================================
inline bool CSceSipCapabilities::IsSupported(IN ESipOptionTag eTag) const
{
    return ((m_uLocalExtensions & m_uPeerExtensions) & eTag) > 0;
}

//==============================================================================
//==
//==  IsSupported
//==
//==============================================================================
//
//  Parameters:
//    eMedia:
//      Payload to check the support of.
//
//  Returns:
//      True if the payload is supported, false otherwise.
//
//  Description:
//      This method checks if the payload is supported by both the local
//      and remote hosts.
//
//==============================================================================
inline bool CSceSipCapabilities::IsSupported(IN ESipMediaType eMedia) const
{
    return ((m_uLocalPayloads & m_uPeerPayloads) & eMedia) > 0;
}

//==============================================================================
//==
//==  IsSupported
//==
//==============================================================================
//
//  Parameters:
//    eEvent:
//      Event to check the support of.
//
//  Returns:
//      True if the event is supported, false otherwise.
//
//  Description:
//      This method checks if the event is supported by both the local
//      and remote hosts.
//
//==============================================================================
inline bool CSceSipCapabilities::IsSupported(IN ESipEventType eEvent) const
{
    return ((m_uLocalEvents & m_uPeerEvents) & eEvent) > 0;
}

//==============================================================================
//==
//==  IsRequired
//==
//==============================================================================
//
//  Parameters:
//    eTag:
//      Extension to check the requirement of.
//
//  Returns:
//      True if the extension is required, false otherwise.
//
//  Description:
//      This method checks if the extension is required by the remote host.
//
//==============================================================================
inline bool CSceSipCapabilities::IsRequired(IN ESipOptionTag eTag) const
{
    return (m_uPeerRequiredExtensions & eTag) > 0;
}

//==============================================================================
//==
//==  IsSupportedLocal
//==
//==============================================================================
//
//  Parameters:
//    eMethod:
//      Method to check the support of.
//
//  Returns:
//      True if the method is supported, false otherwise.
//
//  Description:
//      This method checks if the method is supported by local host only.
//
//==============================================================================
inline bool CSceSipCapabilities::IsSupportedLocal(IN ESipMethodType eMethod) const
{
    return (m_uLocalMethods & eMethod) > 0;
}

//==============================================================================
//==
//==  IsSupportedLocal
//==
//==============================================================================
//
//  Parameters:
//    eTag:
//      Extension to check the support of.
//
//  Returns:
//      True if the extension is supported, false otherwise.
//
//  Description:
//      This method checks if the extension is supported by local host only.
//
//==============================================================================
inline bool CSceSipCapabilities::IsSupportedLocal(IN ESipOptionTag eTag) const
{
    return (m_uLocalExtensions & eTag) > 0;
}

//==============================================================================
//==
//==  IsSupportedLocal
//==
//==============================================================================
//
//  Parameters:
//    eMedia:
//      Payload to check the support of.
//
//  Returns:
//      True if the payload is supported, false otherwise.
//
//  Description:
//      This method checks if the payload is supported by local host only.
//
//==============================================================================
inline bool CSceSipCapabilities::IsSupportedLocal(IN ESipMediaType eMedia) const
{
    return (m_uLocalPayloads & eMedia) > 0;
}

//==============================================================================
//==
//==  IsSupportedLocal
//==
//==============================================================================
//
//  Parameters:
//    eEvent:
//      Event to check the support of.
//
//  Returns:
//      True if the event is supported, false otherwise.
//
//  Description:
//      This method checks if the event is supported by local host only.
//
//==============================================================================
inline bool CSceSipCapabilities::IsSupportedLocal(IN ESipEventType eEvent) const
{
    return (m_uLocalEvents & eEvent) > 0;
}


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCESIPCAPABILITIES_H

