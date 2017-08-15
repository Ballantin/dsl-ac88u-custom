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
#ifndef MXG_CSCEPIDFDEVICE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEPIDFDEVICE_H
//M5T_INTERNAL_USE_END

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

#ifndef MXG_SCEPIDFDATA_H
#include "SceXml/ScePidfData.h"
#endif

#ifndef MXG_CSCEXMLPRIVATEHELPER_H
#include "SceXml/CSceXmlPrivateHelper.h"
#endif

#ifndef MXG_CSCEXMLHELPER_H
#include "SceXml/CSceXmlHelper.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CTime;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CScePidfDevice
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Description:
//  As per RFC 4479, the CScePidfDevice represents a device, which "models
//  the physical environment in which services manifest themselves for users.
//  Devices have characteristics that are useful in allowing a user to make a
//  choice about which communications service to use."
//
//  A device element MUST be set with a deviceID element that uniquely
//  identifies the device, otherwise the generated XML may be considered as
//  invalid by an entity receiving it.
//
//  The CScePidfDevice is a simple wrapper that can be created from a CXmlElement
//  returned by a PIDF Document. It makes it easy to access all standardized
//  elements of the PIDF device element.
//
//  The CScePidfDevice is a light weigth class, it can be created on the stack
//  (as a local variable) without any problem. It however shares a pointer to a
//  CXmlElement that is owned by a PIDF Document. Deleting the associated PIDF
//  Document, calling Cleanup on it or deleting parent XML elements of this
//  device element will render this object invalid.
//
// Location:
//   SceXml/CScePidfDevice.h
//
// See Also:
//   IScePidfDocument, CScePidfPerson, CScePidfService
//
//==============================================================================
class CScePidfDevice
{
//-- Friend Declarations
//------------------------
//M5T_INTERNAL_USE_BEGIN
    friend class CSceUaImppInitializer;
//M5T_INTERNAL_USE_END


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    CScePidfDevice(IN CXmlElement& rDeviceElement);

    // Summary:
    //  Destructor.
    virtual ~CScePidfDevice();

    //-- << id Attribute - RFC 4479>>
    // The id attribute is automatically created by the CScePidfDocument when
    // the device element is created. It is a unique unique string used to
    // identify this element.

    // Summary:
    //  Gets the id associated to this device.
    mxt_result GetId(OUT const char*& rpszDeviceId);


    //-- << Timestamp element - RFC 4479>>

    // Summary:
    //  Sets the timestamp corresponding to the last time this device
    //  information was changed.
    CXmlElement* SetTimestamp(IN const CTime& rTimestamp);

    // Summary:
    //  Gets the timestamp corresponding to the last time this device
    //  information was changed.
    CXmlElement* GetTimestamp(OUT CTime* pTime);

    //-- << Note element - RFC 4479>>

    // Summary:
    //  Sets one or more note to the device.
    mxt_result GetDeviceNotes(OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Gets the notes associated with this device.
    CXmlElement* AddDeviceNote(IN const char* pszUtf8EncodedNote,
                               IN const char* pszLanguageTag);


    //-- << Device ID element - RFC 4479>>

    // Summary:
    //  Sets the Device ID associated with this device.
    CXmlElement* SetDeviceId(IN const char* pszDeviceId);

    // Summary:
    //  Gets the Device ID associated with this device.
    CXmlElement* GetDeviceId(OUT const char** ppszDeviceId);

    //-- << Class - RFC 4480 >>

    // Summary:
    //  Sets the class element of this device.
    CXmlElement* SetClass(IN const char* pszClass);

    // Summary:
    //  Gets the class element of this device.
    CXmlElement* GetClass(OUT const char** ppszClass);

    //-- << User Input - RFC 4480 >>

    // Summary:
    //  Sets the user-input information for this device.    
    CXmlElement* SetUserInput(IN EPidfUserInputValue eInputState,
                              IN unsigned int* pIdleThreshold = NULL,
                              IN const CTime* pLastInputTime = NULL);
  
    // Summary:
    //  Gets the user imput element from this device.
    CXmlElement* GetUserInput(OUT EPidfUserInputValue* peInputState);

    // Summary:
    //  Gets the user-input threshold information.
    mxt_result GetUserInputIdleThreshold(IN CXmlElement* pUserInputElement,
                                         OUT unsigned int& ruIdleThreshold);

    // Summary:
    //  Gets the last user-input time.
    mxt_result GetUserInputLastInputTime(IN CXmlElement* pUserInputElement,
                                         OUT CTime& rLastInputTime);


    //-- << devcaps/mobility/supported - RFC 5196 >>

    // Summary:
    //  Sets whether the device is fixed or mobile.
    CXmlElement* SetDevCapsSupportedMobility(IN uint32_t bsCaps);

    // Summary:
    //  Gets whether the device is fixed or mobile.    
    CXmlElement* GetDevCapsSupportedMobility(OUT uint32_t* pbsCaps);

    //-- << devcaps/mobility/unsupported - RFC 5196 >>

    // Summary:
    //  Sets the mobility type that is not supported by this device.
    CXmlElement* SetDevCapsUnsupportedMobility(IN uint32_t bsCaps);

    // Summary:
    //  Gets the mobility type that is not supported by this device.
    CXmlElement* GetDevCapsUnsupportedMobility(OUT uint32_t* pbsCaps);
    
    //-- << devcaps/description - RFC 5196 >>

    // Summary:
    //  Gets the textual description of the device, used for capabilities.    
    mxt_result GetDevCapsDescriptions(OUT CVector<CXmlElement*>& rvpDescriptions);

    // Summary:
    //  Provides a textual description of the device, used for capabilities.    
    CXmlElement* AddDevCapsDescription(IN const char* pszUtf8EncodedDescription,
                                       IN const char* pszLanguageTag);

    //-- <OMA extension: network-availability>

    // Summary:
    //  Gets all \<network\> elements from the \<network-availability\> element.    
    mxt_result GetNetworkAvailabilities(OUT CVector<CXmlElement*>& rvpNetwork);
    
    // Summary:
    //  Gets the value of a specific \<network\> element.
    mxt_result GetNetworkAvailability(IN CXmlElement* pNetworkElement, 
                                      OUT const char*& rpszNetworkId,
                                      OUT EPidfOmaActiveTerminated& rAvailability) const;

    // Summary:
    //  Adds a \<network\> element to the list of network-availability.   
    CXmlElement* AddNetworkAvailability(IN const char* pszNetworkId,
                                        IN EPidfOmaActiveTerminated eAvailability);
    
    // Summary:
    //  Updates a \<network\> element.   
    mxt_result UpdateNetworkAvailability(IN CXmlElement* pNetworkElement,
                                         IN EPidfOmaActiveTerminated eAvailability);

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:
    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CScePidfDevice.
    static mxt_result InitializeCScePidfDevice();

    // Summary:
    //  Finalizes the CScePidfDevice.
    static void FinalizeCScePidfDevice();

private:

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CScePidfDevice(IN const CScePidfDevice& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CScePidfDevice& operator=(IN const CScePidfDevice& rSrc);


//-- Hidden Data Members
//-----------------------
protected:
private:

    // The internal device XML element.
    CXmlElement& m_rDeviceXmlElement;

    // Vector representing the sequence for the <device> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecDeviceSequence;

    // Vector representing the sequence for the <devcaps> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecDevcapsSequence;

    // Vector representing the sequence for the <mobility> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecMobilitySequence;

    // Vector representing the sequence for the <supported> and <notsupported> elements.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecSupportedNotSupportedSequence;

    // Vector representing the sequence for the <network-availability> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecNetworkAvailabilitySequence;

    // Vector representing the sequence for the <network> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecNetworkSequence;

    //M5T_INTERNAL_USE_END

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetId
//==
//==============================================================================
//
//  Parameters:
//    rpszDeviceId:
//      Pointer reference to a string whch is filled with the id of this device.
//
//  Returns:
//      resS_OK if the attribute has been found, an error otherwise.
//
//  Description:
//      Gets the id attribute of this device element.
//
//==============================================================================
inline mxt_result CScePidfDevice::GetId(const char*& rpszDeviceId)
{
    return m_rDeviceXmlElement.GetAttribute(NULL, g_pszXML_ID, rpszDeviceId);
}

//==============================================================================
//==
//==  GetDeviceNotes
//==
//==============================================================================
//
//  Parameters:
//    rvpNotes:
//      Vector provided by the caller, which is filled with all notes
//      elements that this device contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all notes elements held by this device element.
//
//      The UTF-8 encoded note text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the note text is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      AddDeviceNote, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline mxt_result CScePidfDevice::GetDeviceNotes(OUT CVector<CXmlElement*>& rvpNotes)
{
    return CSceXmlHelper::GetElementFrom(&m_rDeviceXmlElement,
                                         g_pszXML_NAMESPACE_PIDFDATAMODEL,
                                         g_pszXML_NOTE,
                                         OUT rvpNotes);
}

//==============================================================================
//==
//==  AddDeviceNote
//==
//==============================================================================
//
//  Parameters:
//    pszUtf8EncodedNote:
//      Pointer to the UTF-8 encoded note. Can be NULL, in which case an empty
//      note element is created.
//
//    pszLanguageTag:
//      Pointer to the language tag that represents the language of this note.
//      Can be NULL, in which case no tag is set.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information. Will
//      return NULL if pszUtf8EncodedNote is NULL.
//
//  Description:
//      Adds a note to the Device element.
//
//      Each note found at the same level should have the same meaning, but
//      each should use a different language.
//
//  See Also:
//      GetDeviceNotes
//
//==============================================================================
inline CXmlElement* CScePidfDevice::AddDeviceNote(IN const char* pszUtf8EncodedNote,
                                                  IN const char* pszLanguageTag)
{
    CXmlElement* pNoteElement = CSceXmlHelper::AddElement(&m_rDeviceXmlElement,
                                                          g_pszXML_NAMESPACE_PIDFDATAMODEL,
                                                          g_pszXML_NAMESPACE_PIDFDATAMODEL_PREFIX,
                                                          g_pszXML_NOTE,
                                                          pszUtf8EncodedNote,
                                                          m_pvecDeviceSequence);

    if (pNoteElement != NULL)
    {
        pNoteElement->AppendAttribute(g_pszXML_NAMESPACE_XML, g_pszXML_LANG, pszLanguageTag);
    }

    return pNoteElement;
}

//==============================================================================
//==
//==  AddDeviceId
//==
//==============================================================================
//
//  Parameters:
//    pszDeviceId:
//      Pointer to the device ID to set. Must not be NULL.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information. Will
//      return NULL if the parameter is NULL.
//
//  Description:
//      Sets the deviceID element of the device. As specified in RFC 4479:
//
//  <CODE>
//   Devices are identified with a device ID.  A device ID is a URI that
//   is a globally and temporally unique identifier for the device.  In
//   particular, a device ID is a URN.  The URN has to be unique across
//   all other devices for a particular presentity.
//  </CODE>
//
//      See RFC 4479 for further information about the uniqueness requirements
//      of the deviceID element.
//
//  See Also:
//      GetDeviceId
//
//==============================================================================
inline CXmlElement* CScePidfDevice::SetDeviceId(IN const char* pszDeviceId)
{
    return m_rDeviceXmlElement.SetChildElement(g_pszXML_NAMESPACE_PIDFDATAMODEL,
                                               g_pszXML_NAMESPACE_PIDFDATAMODEL_PREFIX,
                                               g_pszXML_DEVICEID,
                                               pszDeviceId,
                                               m_pvecDeviceSequence);
}

//==============================================================================
//==
//==  SetClass
//==
//==============================================================================
//
//  Parameters:
//    pszClass:
//      Pointer to the class string. Must not be NULL.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information. Will
//      return NULL if the parameter is NULL.
//
//  Description:
//      Sets the class information for this device. From RFC 4480:
//
//   <CODE>
//      The <class> element describes the class of the service, device, or
//      person.  Multiple elements can have the same class name within a
//      presence document, but each person, service, or device can only have
//      one class label.  The naming of classes is left to the presentity.
//      The presentity can use this information to group similar services,
//      devices, or person elements or to convey information that the
//      presence agent can use for filtering or authorization.  This
//      information is not generally presented to the watcher user interface.
//   </CODE>
//
//  See Also:
//      GetClass
//
//==============================================================================
inline CXmlElement* CScePidfDevice::SetClass(IN const char* pszClass)
{
    return m_rDeviceXmlElement.SetChildElement(g_pszXML_NAMESPACE_RPID,
                                               g_pszXML_NAMESPACE_RPID_PREFIX,
                                               g_pszXML_CLASS,
                                               pszClass,
                                               m_pvecDeviceSequence);
}

//==============================================================================
//==
//==  SetUserInput
//==
//==============================================================================
//
//  Parameters:
//    eInputState:
//      The input state to set.
//
//    pIdleThreshold:
//      Pointer to the idle threshold to set. Can be NULL, in which case no
//      such information is set in this element.
//
//    pLastInputTime:
//      Pointer to the last time the user has entered input information on his
//      device. Can be NULL, in which case no such information is set in this
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the user input information for this device. From RFC 4480:
//
//   <CODE>  
//      The <user-input> element records the user-input or usage state of the
//      service or device, based on human user input, e.g., keyboard,
//      pointing device, or voice. 
//   <CODE>  
//
//  See Also:
//      GetUserInput, GetUserInputIdleThreshold, GetUserInputLastInputTime
//
//==============================================================================
inline CXmlElement* CScePidfDevice::SetUserInput(IN EPidfUserInputValue eInputState,
                                                 IN unsigned int* pIdleThreshold,
                                                 IN const CTime* pLastInputTime)
{
    return CSceXmlPrivateHelper::SetUserInputHelper(&m_rDeviceXmlElement,
                                                    eInputState,
                                                    pIdleThreshold,
                                                    pLastInputTime,
                                                    m_pvecDeviceSequence);
}

//==============================================================================
//==
//==  GetUserInput
//==
//==============================================================================
//
//  Parameters:
//    peInputState:
//      Valid pointer to an EPidfUserInputState, where the user input state
//      will be stored.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns the user-input information assigned to this device.
//
//      See SetUserInput for more information about the user-input element.
//
//  See Also:
//      SetUserInput, GetUserInputIdleThreshold, GetUserInputLastInputTime
//
//==============================================================================
inline CXmlElement* CScePidfDevice::GetUserInput(OUT EPidfUserInputValue* peInputState)
{
    return CSceXmlPrivateHelper::GetUserInputHelper(&m_rDeviceXmlElement, OUT peInputState);
}

//==============================================================================
//==
//==  GetUserInputIdleThreshold
//==
//==============================================================================
//
//  Parameters:
//    pUserInputElement:
//      Pointer to the user-input element from which the idle threshold must be
//      fetched. This pointer can be retreived through GetUserInput. Cannot be
//      NULL.
//
//    ruIdleThreshold:
//      Reference where the idle threshold will be stored by this method.
//
//  Returns:
//      resS_OK upon success, resFE_INVALID_ARGUMENT if pUserInputElement is
//      NULL, and resFE_FAIL if the idle threshold is not set in the element.
//
//  Description:
//      Gets the idle-threshold information associated with a user-input
//      element.
//
//  See Also:
//      GetUserInput
//
//==============================================================================
inline mxt_result CScePidfDevice::GetUserInputIdleThreshold(IN CXmlElement* pUserInputElement,
                                                            OUT unsigned int& ruIdleThreshold)
{
    return CSceXmlPrivateHelper::GetUserInputIdleThresholdHelper(pUserInputElement, OUT ruIdleThreshold);
}

//==============================================================================
//==
//==  GetUserInputLastInputTime
//==
//==============================================================================
//
//  Parameters:
//    pUserInputElement:
//      Pointer to the user-input element from which the last imput time must
//      be fetched. This pointer can be retreived through GetUserInput. Cannot
//      be NULL.
//
//    rLastInputTime:
//      Reference to a time object that will be set with the last input time
//      if the method succeds.
//
//  Returns:
//      resS_OK upon success, resFE_INVALID_ARGUMENT if pUserInputElement is
//      NULL, and resFE_FAIL if the last input time is not set in the element.
//
//  Description:
//      Gets the last input time information associated with a user-input
//      element.
//
//  See Also:
//      GetUserInput
//
//==============================================================================
inline mxt_result CScePidfDevice::GetUserInputLastInputTime(IN CXmlElement* pUserInputElement,
                                                            OUT CTime& rLastInputTime)
{
    return CSceXmlPrivateHelper::GetUserInputLastInputTimeHelper(pUserInputElement, OUT rLastInputTime);
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEPIDFDEVICE_H

