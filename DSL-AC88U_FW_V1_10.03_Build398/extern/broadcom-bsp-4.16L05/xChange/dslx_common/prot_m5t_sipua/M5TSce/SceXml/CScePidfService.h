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
#ifndef MXG_CSCEPIDFSERVICE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEPIDFSERVICE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSCEXMLHELPER_H
#include "SceXml/CSceXmlHelper.h"
#endif

#ifndef MXG_CSCEXMLPRIVATEHELPER_H
#include "SceXml/CSceXmlPrivateHelper.h"
#endif

#ifndef MXG_SCEPIDFDATA_H
#include "SceXml/ScePidfData.h"
#endif

#ifndef MXG_SCEPIDFTYPESCONVERTER_H
#include "SceXml/ScePidfTypesConverter.h"
#endif

#ifndef MXG_CTIME_H
#include "Time/CTime.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class IUri;
class IXmlDocument;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CScePidfService
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Summary:
//  This class represents a PIDF service.
//
// Description:
//  The CScePidfService represents a service at the PIDF level. According to RFC
//  4479, "A service models a form of communication that can be used to interact
//  with the user."
//
//  RFC 4479 does not clearly define what is a service, but states that anything
//  can be a service as long as it exhibits a set of key properties:
//
//      - Characteristics: From RFC 4479, "[Characteristics] helps differentiate
//        the services from each other and give the presence watcher the context
//        in which to make a choice." Characteristics are not yet supported.
//      - Reach Information: "... provides the instructions for the
//        recipient of a document on how to correctly contact that service."
//        The reach information is set through the Contact element.
//      - Status Information: "Status represents generally dynamic information
//        about the availability of communications using that service. This is
//        in contrast to characteristics, which describe fairly static
//        properties of the various services." The status information is set
//        through the basic element which is under the status element.
//      - Relative Information: "Each service is also associated with a
//        priority, which represents the preference that the user has for usage
//        of one service over another." In this context, the relative priority
//        is encoded as part of the Contact as a q-value.
//
//  The PIDF service is encoded as a "tuple" element defined in RFC 3863.
//
//  The CScePidfService is a simple wrapper that can be created from a
//  CXmlElement returned by the CScePidfDocument. It makes it easy to access all
//  standardized elements of the PIDF service element.
//
//  The CScePidfService is a light weigth class, it can be created on the stack
//  (as a local variable) without any problem. It however shares a pointer to a
//  CXmlElement that is owned by a CScePidfDocument. Deleting the associated
//  CScePidfDocument, calling Cleanup on it or deleting parent XML elements of
//  this service element will render this object invalid.
//
// Location:
//   SceXml/CScePidfService.h
//
// See Also:
//   CScePidfDocument, CScePidfDevice, CScePidfPerson
//
//==============================================================================
class CScePidfService
{
//-- Friend Declarations.
//M5T_INTERNAL_USE_BEGIN
    friend class CSceUaImppInitializer;
//M5T_INTERNAL_USE_END


//-- New type.
public:

//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CScePidfService(IN CXmlElement& rServiceElement);

    // Summary:
    //  Destructor.
    ~CScePidfService();

    //-- << id Attribute - RFC 3863 >>
    // The id attribute is automatically created by the CScePidfDocument when
    // the person element is created. It is a unique unique string used to
    // identify this element.

    // Summary:
    //  Gets the id associated to this service.
    mxt_result GetId(OUT const char*& rpszServiceId);

    //-- << Contact element - RFC 3863 >>

    // Summary:
    //  Sets the Contact associated with this service.
    CXmlElement* SetContact(IN const IUri* pUri, IN const unsigned int* puPriority);
    
    // Summary:
    //  Sets the Contact associated with this service.
    CXmlElement* SetContact(IN const char* pszUri, IN const unsigned int* puPriority);

    // Summary:
    //  Gets the Contact associated with this service.
    CXmlElement* GetContact(OUT const char** ppszUri);

    // Summary:
    //  Gets the Contact's priority.
    mxt_result GetContactPriority(IN const CXmlElement* pContactElement,
                                  OUT unsigned int& ruPriority);

    //-- << Status + Basic elements (combined) - RFC 3863 >>

    // Summary:
    //  Sets the basic status of this service.
    CXmlElement* SetBasicStatus(IN EPidfBasicStatus eBasicStatus);

    // Summary:
    //  Gets the basic status of this service.
    CXmlElement* GetBasicStatus(OUT EPidfBasicStatus* peBasicStatus);

    //-- << Timestamp element - RFC 3863 >>

    // Summary:
    //  Sets the timestamp corresponding to the last time this service
    //  information was changed.
    CXmlElement* SetTimestamp(IN const CTime& rTimestamp);

    // Summary:
    //  Gets the timestamp corresponding to the last time this service
    //  information was changed.
    CXmlElement* GetTimestamp(OUT CTime* pTime);


    //-- << Note element - RFC 3863 >>

    // Summary:
    //  Gets a pointer to all Note elements held by this service.
    mxt_result GetServiceNotes(OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the service.
    CXmlElement* AddServiceNote(IN const char* pszUtf8EncodedNote,
                                IN const char* pszLanguageTag);

    //-- << DeviceID element - RFC 4480 >>

    // Summary:
    //  Gets all DeviceID elements held by this service element.
    mxt_result GetDeviceIDs(OUT CVector<CXmlElement*>& rvpDeviceID);
    
    // Summary:
    //  Gets the ID held by a DeviceID element.
    mxt_result GetDeviceID(IN const CXmlElement* pDeviceID, 
                           OUT const char*& rpszDeviceID);

    // Summary:
    //  Add a DeviceID to the service element.
    CXmlElement* AddDeviceID(IN const char* pszDeviceID);
    
    // Summary:
    //  Update a DeviceID from the service element.
    mxt_result UpdateDeviceID(INOUT CXmlElement* pDeviceID, 
                              IN const char* pszDeviceID);

    //-- << Class element - RFC 4480 >>

    // Summary:
    //  Sets the class element of this service.
    CXmlElement* SetClass(IN const char* pszClass);

    // Summary:
    //  Gets the class element of this service.
    CXmlElement* GetClass(OUT const char** ppszClass);

    //-- << Privacy element - RFC 4480 >>

    // Summary:
    //  Gets all privacy elements held by this service element.
    mxt_result GetPrivacies(OUT CVector<CXmlElement*>& rvpPrivacy);
    
    // Summary:
    //  Gets the private forms of communication element of a privacy element.
    mxt_result GetPrivacy(IN const CXmlElement* pPrivacyElement, 
                          OUT uint32_t& rbsPrivacy);

    // Summary:
    //  Adds private forms of communication to the presentity.
    CXmlElement* AddPrivacy(IN uint32_t bsPrivacy);

    // Summary:
    //  Updates the private forms of communication elements of a privacy
    //  element.
    mxt_result UpdatePrivacy(INOUT CXmlElement* pPrivacyElement, 
                             IN uint32_t bsPrivacy);

    // Summary:
    //  Gets a pointer to all Note elements held by the Privacy element.
    mxt_result GetPrivacyNotes(IN CXmlElement* pPrivacyElement, 
                               OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the Privacy element.
    CXmlElement* AddPrivacyNote(INOUT CXmlElement* pPrivacyElement,
                                IN const char* pszUtf8EncodedNote,
                                IN const char* pszLanguageTag);


    //-- << Relationship element - RFC 4480 >>

    // Summary:
    //  Sets the Relationship element of this service.
    CXmlElement* SetRelationship(IN EPidfRelationship eRelationship,
                                 IN const char* pszOtherValue = NULL,
                                 IN const char* pszOtherLang = NULL);

    // Summary:
    //  Gets the Relationship element of this service.
    CXmlElement* GetRelationship(OUT EPidfRelationship* peRelationship,
                                 OUT const char** ppszOtherValue,
                                 OUT const char** ppszOtherLang);

    // Summary:
    //  Gets a pointer to all Note elements held by the Relationship element.
    mxt_result GetRelationshipNotes(OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the Relationship element.
    CXmlElement* AddRelationshipNote(IN const char* pszUtf8EncodedNote,
                                     IN const char* pszLanguageTag);


    //-- << Service-Class element - RFC 4480 >>

    // Summary:
    //  Sets the ServiceClass element of this service.
    CXmlElement* SetServiceClass(IN EPidfServiceClass eServiceClass);

    // Summary:
    //  Gets the ServiceClass element of this service.
    CXmlElement* GetServiceClass(OUT EPidfServiceClass* peServiceClass);


    // Summary:
    //  Gets a pointer to all Note elements held by the ServiceClass element.
    mxt_result GetServiceClassNotes(OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the ServiceClass element.
    CXmlElement* AddServiceClassNote(IN const char* pszUtf8EncodedNote,
                                     IN const char* pszLanguageTag);


    //-- << Status Icon element - RFC 4480 >>

    // Summary:
    //  Gets all status-icon elements held by this service element.
    mxt_result GetStatusIcons(OUT CVector<CXmlElement*>& rvpStatusIcon);

    // Summary:
    //  Adds a status icon to the presentity.
    CXmlElement* AddStatusIcon(IN const char* pszStatusIconUri);
    
    // Summary:
    //  Updates the icon of a status-icon element.
    mxt_result UpdateStatusIcon(INOUT CXmlElement* pStatusIconElement,
                                IN const char* pszStatusIconUri);


    //-- << Timed-Status element - RFC 4481>>

    // Summary:
    //  Gets all timed-status elements held by this Service.
    mxt_result GetTimedStatuses(OUT CVector<CXmlElement*>& rvpTimedStatus);

    // Summary:
    //  Gets the basic status of a timed-status element
    mxt_result GetTimedStatus(IN const CXmlElement* pTimedStatusElement,
                              OUT EPidfBasicStatus& reStatus);
    
    // Summary:
    //  Add timed-status element to this service.
    CXmlElement* AddTimedStatus(IN EPidfBasicStatus eStatus,
                                IN const CTime* pFrom,
                                IN const CTime* pUntil = NULL);
    
    // Summary:
    //  Updates the basic status of a timed-status element.
    mxt_result UpdateTimedStatus(INOUT CXmlElement* pTimedStatusElement,
                                 IN EPidfBasicStatus eStatus);

    // Summary:
    //  Gets the Note element held by the TimedStatus element.
    CXmlElement* GetTimedStatusNote(IN CXmlElement* pTimedStatusElement);


    // Summary:
    //  Adds a note to the TimedStatus element.
    CXmlElement* SetTimedStatusNote(INOUT CXmlElement* pTimedStatusElement,
                                    IN const char* pszUtf8EncodedNote,
                                    IN const char* pszLanguageTag);


    //-- << User Input element - RFC 4480 >>

    // Summary:
    //  Sets the user input information for this person.
    CXmlElement* SetUserInput(IN EPidfUserInputValue eInputValue,
                              IN const unsigned int* puIdleThreshold = NULL,
                              IN const CTime* pLastInputTime = NULL);
  
    // Summary:
    //  Returns the user-input information assigned to this person.
    CXmlElement* GetUserInput(OUT EPidfUserInputValue* peInputValue);

    // Summary:
    //  Gets the idle-threshold information associated with a user-input
    //  element.
    mxt_result GetUserInputIdleThreshold(IN const CXmlElement* pUserInputElement,
                                         OUT unsigned int& ruIdleThreshold);

    // Summary:
    //  Gets the last input time information associated with a user-input
    //  element.
    mxt_result GetUserInputLastInputTime(IN const CXmlElement* pUserInputElement,
                                         OUT CTime& rLastInputTime);


    //-- << Card element - RFC 4482 >>

    // Summary:
    //  Sets the business card for this service.
    CXmlElement* SetCard(IN const char* pszCardUri);

    // Summary:
    //  Returns the card information assigned to this service.
    CXmlElement* GetCard(OUT const char** ppszCardUri);

    //-- << DisplayName element - RFC 4482 >>

    // Summary:
    //  Sets the display name for this service.
    CXmlElement* SetDisplayName(IN const char* pszDisplayName,
                                IN const char* pszLanguageTag = NULL);

    // Summary:
    //  Returns the display-name information assigned to this service.
    CXmlElement* GetDisplayName(OUT const char** ppszDisplayName);

    //-- << Homepage element - RFC 4482 >>

    // Summary:
    //  Sets the general information (typically a web home page) about the
    //  service.
    CXmlElement* SetHomepage(IN const char* pszHomepageUri);

    // Summary:
    //  Returns the homepage information assigned to this service.
    CXmlElement* GetHomepage(OUT const char** ppszHomepageUri);

    //-- << Icon element - RFC 4482 >>

    // Summary:
    //  Sets the image (icon) representing the service.
    CXmlElement* SetIcon(IN const char* pszIconUri);

    // Summary:
    //  Returns the image (icon) representing the service.
    CXmlElement* GetIcon(OUT const char** ppszIconUri);

    //-- << Map element - RFC 4482 >>

    // Summary:
    //  Sets the map related to the service.
    CXmlElement* SetMap(IN const char* pszMapUri);

    // Summary:
    //  Returns the map related to the service.
    CXmlElement* GetMap(OUT const char** ppszMapUri);

    //-- << Sound element - RFC 4482 >>

    // Summary:
    //  Sets the sound related to the service.
    CXmlElement* SetSound(IN const char* pszSoundUri);

    // Summary:
    //  Returns the sound related to the service.
    CXmlElement* GetSound(OUT const char** ppszSoundUri);


    //-- << servcaps/application - RFC 5196 >>

    // Summary:
    //  Sets if the service support application.
    CXmlElement* SetServCapsApplication(IN bool bStatus);

    // Summary:
    //  Returns if the service support application.
    CXmlElement* GetServCapsApplication(OUT bool* pbStatus);

    //-- << servcaps/audio - RFC 5196 >>

    // Summary:
    //  Sets if the service support audio.
    CXmlElement* SetServCapsAudio(IN bool bStatus);

    // Summary:
    //  Returns if the service support audio.
    CXmlElement* GetServCapsAudio(OUT bool* pbStatus);

    //-- << servcaps/automata - RFC 5196 >>

    // Summary:
    //  Sets if the service support an automata.
    CXmlElement* SetServCapsAutomata(IN bool bStatus);

    // Summary:
    //  Returns if the service support an automata.
    CXmlElement* GetServCapsAutomata(OUT bool* pbStatus);

    //-- << servcaps/control - RFC 5196 >>

    // Summary:
    //  Sets if the service support control.
    CXmlElement* SetServCapsControl(IN bool bStatus);    

    // Summary:
    //  Returns if the service support control.
    CXmlElement* GetServCapsControl(OUT bool* pbStatus);

    //-- << servcaps/data - RFC 5196 >>

    // Summary:
    //  Sets if the service support data.
    CXmlElement* SetServCapsData(IN bool bStatus);

    // Summary:
    //  Returns if the service support data.
    CXmlElement* GetServCapsData(OUT bool* pbStatus);

    //-- << servcaps/isfocus - RFC 5196 >>

    // Summary:
    //  Sets if the service support conference.
    CXmlElement* SetServCapsIsfocus(IN bool bStatus);  

    // Summary:
    //  Returns if the service support conference.
    CXmlElement* GetServCapsIsfocus(OUT bool* pbStatus);

    //-- << servcaps/message - RFC 5196 >>

    // Summary:
    //  Sets if the service support message.
    CXmlElement* SetServCapsMessage(IN bool bStatus);

    // Summary:
    //  Returns if the service support message.
    CXmlElement* GetServCapsMessage(OUT bool* pbStatus);

    //-- << servcaps/text - RFC 5196 >>

    // Summary:
    //  Sets if the service support text.
    CXmlElement* SetServCapsText(IN bool bStatus);   

    // Summary:
    //  Returns if the service support text.
    CXmlElement* GetServCapsText(OUT bool* pbStatus);

    //-- << servcaps/video - RFC 5196 >>

    // Summary:
    //  Sets if the service support video.
    CXmlElement* SetServCapsVideo(IN bool bStatus);

    // Summary:
    //  Returns if the service support video.
    CXmlElement* GetServCapsVideo(OUT bool* pbStatus);

    //-- << servcaps/description - RFC 5196 >>

    // Summary:
    //  Gets all description elements held by the servcap element under this
    //  service element.
    mxt_result GetServCapsDescriptions(OUT CVector<CXmlElement*>& rvpDescription);
    
    // Summary:
    //  Add a new description element to the servcap element under this service
    //  element.
    CXmlElement* AddServCapsDescription(IN const char* pszDescription,
                                        IN const char* pszLang = NULL);

    //-- << servcaps/type - RFC 5196 >>

    // Summary:
    //  Sets the MIME media content type of this service.
    CXmlElement* SetServCapsType(IN uint32_t bsMediaTypes);

    // Summary:
    //  Gets the MIME media content type of this service.
    CXmlElement* GetServCapsType(OUT uint32_t* pbsMediaTypes) const;

    //-- << servcaps/duplex/supported - RFC 5196 >>

    // Summary
    //  Sets the supported communications by this service.
    CXmlElement* SetServCapsSupportedDuplex(IN uint32_t bsCaps);

    // Summary
    //  Gets the supported communications by this service.
    CXmlElement* GetServCapsSupportedDuplex(OUT uint32_t* pbsCaps);

    //-- << servcaps/duplex/unsupported - RFC 5196 >>

    // Summary
    //  Sets the unsupported communications by this service.
    CXmlElement* SetServCapsUnsupportedDuplex(IN uint32_t bsCaps);

    // Summary
    //  Gets the unsupported communications by this service.
    CXmlElement* GetServCapsUnsupportedDuplex(OUT uint32_t* pbsCaps);

    //-- << servcaps/class/supported - RFC 5196 >>

    // Summary
    //  Sets the supported class setting by this service.
    CXmlElement* SetServCapsSupportedClass(IN uint32_t bsCaps);

    // Summary
    //  Gets the supported class setting by this service.
    CXmlElement* GetServCapsSupportedClass(OUT uint32_t* pbsCaps);

    //-- << servcaps/class/unsupported - RFC 5196 >>

    // Summary
    //  Sets the unsupported class setting by this service.
    CXmlElement* SetServCapsUnsupportedClass(IN uint32_t bsCaps);

     // Summary
    //  Gets the unsupported class setting by this service.
    CXmlElement* GetServCapsUnsupportedClass(OUT uint32_t* pbsCaps);

    //-- << servcaps/event-packages/supported - RFC 5196 >>

    // Summary
    //  Sets the supported event-packages by this service.
    CXmlElement* SetServCapsSupportedEventPackages(IN uint32_t bsCaps);

    // Summary
    //  Gets the supported event-packages by this service.
    CXmlElement* GetServCapsSupportedEventPackages(OUT uint32_t* pbsCaps);
    
    //-- << servcaps/event-packages/unsupported - RFC 5196 >>

    // Summary
    //  Sets the unsupported event-packages by this service.
    CXmlElement* SetServCapsUnsupportedEventPackages(IN uint32_t bsCaps);

    // Summary
    //  Gets the unsupported event-packages by this service.
    CXmlElement* GetServCapsUnsupportedEventPackages(OUT uint32_t* pbsCaps);
    
    //-- << servcaps/methods/supported - RFC 5196 >>

    // Summary
    //  Sets the supported SIP methods by this service.
    CXmlElement* SetServCapsSupportedMethods(IN uint32_t bsCaps);

    // Summary
    //  Gets the supported SIP methods by this service.
    CXmlElement* GetServCapsSupportedMethods(OUT uint32_t* pbsCaps);

    //-- << servcaps/methods/unsupported - RFC 5196 >>

    // Summary
    //  Sets the unsupported SIP methods by this service.
    CXmlElement* SetServCapsUnsupportedMethods(IN uint32_t bsCaps);

    // Summary
    //  Gets the unsupported SIP methods by this service.
    CXmlElement* GetServCapsUnsupportedMethods(OUT uint32_t* pbsCaps);

    //-- << servcaps/extensions/supported - RFC 5196 >>

    // Summary
    //  Sets the supported SIP extensions by this service.
    CXmlElement* SetServCapsSupportedExtensions(IN uint32_t bsCaps);

    // Summary
    //  Gets the supported SIP extensions by this service.
    CXmlElement* GetServCapsSupportedExtensions(OUT uint32_t* pbsCaps);

    //-- << servcaps/extensions/unsupported - RFC 5196 >>

    // Summary
    //  Sets the unsupported SIP extensions by this service.
    CXmlElement* SetServCapsUnsupportedExtensions(IN uint32_t bsCaps);

    // Summary
    //  Gets the unsupported SIP extensions by this service.
    CXmlElement* GetServCapsUnsupportedExtensions(OUT uint32_t* pbsCaps);

    //-- << servcaps/actor/supported - RFC 5196 >>

    // Summary
    //  Sets the supported type of entity by this service.
    CXmlElement* SetServCapsSupportedActor(IN uint32_t bsCaps);

    // Summary
    //  Gets the supported type of entity by this service.
    CXmlElement* GetServCapsSupportedActor(OUT uint32_t* pbsCaps);

    //-- << servcaps/actor/unsupported - RFC 5196 >>

    // Summary
    //  Sets the unsupported type of entity by this service.
    CXmlElement* SetServCapsUnsupportedActor(IN uint32_t bsCaps);

    // Summary
    //  Gets the unsupported type of entity by this service.
    CXmlElement* GetServCapsUnsupportedActor(OUT uint32_t* pbsCaps);

    //-- << servcaps/priority - RFC 5196 >>

    // Summary:
    //  Gets all supported elements held by the priority element from the servcaps
    //  element of this service.
    mxt_result GetServCapsSupportedPriorities(OUT CVector<CXmlElement*>& rvpElems);

    // Summary:
    //  Gets all notsupported elements held by the priority element from the servcaps
    //  element of this service.
    mxt_result GetServCapsUnsupportedPriorities(OUT CVector<CXmlElement*>& rvpElems);
    
    // Summary:
    //  Adds a new supported elements to the priority element of the servcaps
    //  element of this service.
    CXmlElement* AddServCapsSupportedPriority(IN EPidfServCapsPriority ePriorityOp,
                                              IN unsigned int uValue1, 
                                              IN unsigned int uValue2 = 0);

    // Summary:
    //  Adds a new notsupported elements to the priority element of the servcaps
    //  element of this service.
    CXmlElement* AddServCapsUnsupportedPriority(IN EPidfServCapsPriority ePriorityOp,
                                                IN unsigned int uValue1, 
                                                IN unsigned int uValue2 = 0);

    // Summary:
    //  Updates information of one element under the priority element of the
    //  servcaps element of this service.
    mxt_result UpdateServCapsPriority(INOUT CXmlElement* pElement,
                                      IN unsigned int uValue1,
                                      IN unsigned int uValue2 = 0);

    // Summary:
    //  Gets information about one element under the priority element of the
    //  servcaps element of this service.
    mxt_result GetServCapsPriority(IN const CXmlElement* pElement,
                                   OUT EPidfServCapsPriority& rePriority,
                                   OUT unsigned int& ruValue1,
                                   OUT unsigned int& ruValue2);
    
    //-- << servcaps/schemes - RFC 5196 >>

    // Summary:
    //  Gets all supported s elements held by the scheme element from the servcaps
    //  element of this service.
    mxt_result GetServCapsSupportedSchemes(OUT CVector<CXmlElement*>& rvpElems);

    // Summary:
    //  Gets all notsupported s elements held by the scheme element from the
    //  servcaps element of this service.
    mxt_result GetServCapsUnsupportedSchemes(OUT CVector<CXmlElement*>& rvpElems);

    // Summary:
    //  Adds a new supported s element to the scheme element of the servcaps
    //  element of this service.
    CXmlElement* AddServCapsSupportedScheme(IN const char* pszScheme);

    // Summary:
    //  Adds a new notsupported s element to the scheme element of the servcaps
    //  element of this service.
    CXmlElement* AddServCapsUnsupportedScheme(IN const char* pszScheme);

    //-- << servcaps/languages - RFC 5196 >>

    // Summary:
    //  Gets all supported l elements held by the language element from the servcaps
    //  element of this service.
    mxt_result GetServCapsSupportedLanguageElems(OUT CVector<CXmlElement*>& rvpElems);

    // Summary:
    //  Gets all notsupported l elements held by the language element from the
    //  servcaps element of this service.
    mxt_result GetServCapsUnsupportedLanguageElems(OUT CVector<CXmlElement*>& rvpElems);

    // Summary:
    //  Adds a new supported l element to the language element of the servcaps
    //  element of this service.
    CXmlElement* AddServCapsSupportedLanguage(IN const char* pszLanguage);

    // Summary:
    //  Adds a new notsupported l element to the language element of the servcaps
    //  element of this service.
    CXmlElement* AddServCapsUnsupportedLanguage(IN const char* pszLanguage);

    //-- <OMA extension: service-description>

    // Summary
    //  Sets the description of this service.
    CXmlElement* SetServiceDescription(IN const char* pszServiceId,
                                       IN const char* pszVersion,
                                       IN const char* pszDescription);

    // Summary
    //  Gets the description of this service.
    CXmlElement* GetServiceDescription(OUT const char** ppszServiceId,
                                       OUT const char** ppszVersion,
                                       OUT const char** ppszDescription);

    //-- <OMA extension: willingness>

    // Summary
    //  Sets the willingness of this service.
    CXmlElement* SetWillingness(IN EPidfBasicStatus eValue);

    // Summary
    //  Gets the willingness of this service.
    CXmlElement* GetWillingness(OUT EPidfBasicStatus* peValue);
    
    //-- <OMA extension: session-participation>

    // Summary
    //  Sets the session-participation of this service.
    CXmlElement* SetSessionParticipation(IN EPidfBasicStatus eValue);

    // Summary
    //  Gets the session-participation of this service.
    CXmlElement* GetSessionParticipation(OUT EPidfBasicStatus* peValue);

    //-- <OMA extension: registration-state>

    // Summary
    //  Sets the registration-state of this service.
    CXmlElement* SetRegistrationState(IN EPidfOmaActiveTerminated eState);

    // Summary
    //  Gets the registration-state of this service.
    CXmlElement* GetRegistrationState(OUT EPidfOmaActiveTerminated* peState);

    //-- <OMA extension: barring-state>

    // Summary
    //  Sets the barring-state of this service.
    CXmlElement* SetBarringState(IN EPidfOmaActiveTerminated eState);

    // Summary
    //  Gets the barring-state of this service.
    CXmlElement* GetBarringState(OUT EPidfOmaActiveTerminated* peState);

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods.
protected:
    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CScePidfService.
    static mxt_result InitializeCScePidfService();

    // Summary:
    //  Finalizes the CScePidfService.
    static void FinalizeCScePidfService();

    // Summary:
    //  Adds elements defined by the bitset to a Privacy element.
    void AddPrivacyElementHelper(INOUT CXmlElement* pPrivacy,
                                 IN uint32_t uPrivacyTypesBitSet);

    // Summary:
    //  Sets elements defined by the bitset to the pszElement element.
    CXmlElement* SetServCapsElementHelper(IN const char* pszElement,
                                          IN const char* pszSupport,
                                          IN uint32_t uBitSet,
                                          IN uint32_t uMask,
                                          IN const SPidfData* pfnGetPidfFunction(uint32_t),
                                          IN const CVector<CXmlElement::SIdentificationInfo>* pvecElementSequence);

    // Summary:
    //  Gets elements defined by the pszElement element and assign the
    //  corresponding bitset to puBitSet.
    CXmlElement* GetServCapsElementHelper(IN const char* pszElement,
                                          IN const char* pszSupport,
                                          OUT uint32_t* puBitSet,
                                          IN uint32_t pfnGetPidfFunction(const char* const,
                                                                         const char* const));

    // Summary:
    //  Sets if the defined element is supported.
    CXmlElement* SetServCapsHelper(IN const char* pszElement,
                                   IN bool bStatus);

    // Summary:
    //  Gets if the defined element is supported.
    CXmlElement* GetServCapsHelper(IN const char* pszElement,
                                   OUT bool* pbStatus);

    // Summary:
    //  Adds priority rule to the supported/notsupported priority element under
    //  the servcaps element of this service.
    CXmlElement* AddServCapsPriorityHelper(IN const char* pszName,
                                           IN EPidfServCapsPriority ePriorityOp, 
                                           IN unsigned int uValue1, 
                                           IN unsigned int uValue2);

    // Summary:
    //  Gets all elements held by the specified priority child element.
    mxt_result GetServCapsPriorityHelper(const char* pszPriorityChildElementName,
                                         OUT CVector<CXmlElement*>& rvpElems);

    // Summary:
    //  Adds URI schemes to the specified scheme child element.
    CXmlElement* AddServCapsSupportedSchemeHelper(IN const char* pszName,
                                                  IN const char* pszScheme);

    // Summary:
    //  Appends all child elements of pParent into rvpElems.
    mxt_result GetAllChildElements(IN CXmlElement* pParent,
                                   OUT CVector<CXmlElement*>& rvpElems) const;

    // Summary:
    //  Tries to find the element qualified with pszNamespace and pszName and
    //  then gets its value.
    CXmlElement* GetElementHelper(IN const char* pszNamespace,
                                  IN const char* pszName,
                                  OUT const char** ppszValue);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CScePidfService(IN const CScePidfService& rSrc);

    // Summary:
    //  Assignment Operator.
    CScePidfService& operator=(IN const CScePidfService& rSrc);


//-- Hidden Data Members.
protected:
private:

    // XML element this class is representing.
    CXmlElement* m_pServiceXmlElement;

    // Vector representing the sequence for the \<tuple\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecServiceSequence;

    // Vector representing the sequence for the \<status\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecStatusSequence;

    // Vector representing the sequence \<note\> followed by \<other\>. This
    // sequence is used for the \<privacy\>, \<relationship\>, \<service-class\>
    // elements.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecNoteOtherSequence;

    // Vector representing the second sequence for the \<privacy\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecPrivacySequence;

    // Vector representing the sequence for the \<timed-status\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecTimedStatusSequence;

    // Vector representing the sequence for the \<servcaps\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecServCapsSequence;

    // Vector representing the sequence \<supported\> followed by
    // \<notsupported\>. This sequence is used for the \<duplex\>, \<class\>,
    // \<event-packages\>, \<methods\>, \<extensions\>, \<actor\>, \<priority\>,
    // \<schemes\> and \<languages\> elements.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecSupportedNotsupportedSequence;

    // Vector representing the second sequence for the \<duplex\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecDuplexSequence;

    // Vector representing the second sequence for the \<class\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecClassSequence;

    // Vector representing the second sequence for the \<event-packages\>
    // element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecEventPackagesSequence;

    // Vector representing the second sequence for the \<methods\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecMethodsSequence;

    // Vector representing the second sequence for the \<extensions\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecExtensionsSequence;

    // Vector representing the second sequence for the \<actor\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecActorSequence;

    // Vector representing the second sequence for the \<priority\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecPrioritySequence;

    // Vector representing the sequence for the \<service-description\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecServiceDescriptionSequence;

    // Vector representing the sequence for the \<willingness\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecWillingnessSequence;

    // Vector representing the sequence for the \<session-participation\> element.
    static CVector<CXmlElement::SIdentificationInfo>* ms_pvecSessionParticipationSequence;

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
//    rpszServiceId:
//      Pointer reference to a string which is filled with the id of this
//      service.
//
//  Returns:
//      resS_OK if the attribute has been found, an error otherwise.
//
//  Description:
//      Gets the id attribute of this service element.
//
//==============================================================================
inline mxt_result CScePidfService::GetId(const char*& rpszServiceId)
{
    return m_pServiceXmlElement->GetAttribute(NULL, g_pszXML_ID, rpszServiceId);
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
//      Sets the class information for this service. From RFC 4480:
//
//   <CODE>
//      The \<class\> element describes the class of the service, device, or
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
inline CXmlElement* CScePidfService::SetClass(IN const char* pszClass)
{
    return m_pServiceXmlElement->SetChildElement(g_pszXML_NAMESPACE_RPID,
                                                 g_pszXML_NAMESPACE_RPID_PREFIX,
                                                 g_pszXML_CLASS,
                                                 pszClass,
                                                 ms_pvecServiceSequence);
}

//==============================================================================
//==
//==  GetClass
//==
//==============================================================================
//
//  Parameters:
//    ppszClass:
//      Double const char pointer, which is set to point to the internal buffer
//      that holds the class string. 
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the class information assigned to this service.
//
//      See SetClass for more information about the class element.
//
//  See Also:
//      SetClass
//
//==============================================================================
inline CXmlElement* CScePidfService::GetClass(OUT const char** ppszClass)
{
    return GetElementHelper(g_pszXML_NAMESPACE_RPID,
                            g_pszXML_CLASS,
                            ppszClass);
}

//==============================================================================
//==
//==  GetPrivacies
//==
//==============================================================================
//
//  Parameters:
//    rvpPrivacy:
//      Vector provided by the application, which is filled with all
//      privacy elements that this service contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all privacy elements held by this service element. As defined in
//      RFC 4480:
//
// <CODE>
//     The \<privacy\> element indicates which types of communication third
//     parties in the vicinity of the presentity are unlikely to be able to
//     intercept accidentally or intentionally.
// </CODE>
//
//      More information about each privacy can be retreived using
//      GetPrivacy.
//
//  See Also:
//      GetPrivacy, AddPrivacy
//
//==============================================================================
inline mxt_result CScePidfService::GetPrivacies(OUT CVector<CXmlElement*>& rvpPrivacy)
{
    return CSceXmlHelper::GetElementFrom(m_pServiceXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_PRIVACY,
                                         OUT rvpPrivacy);
}

//==============================================================================
//==
//==  AddStatusIcon
//==
//==============================================================================
//
//  Parameters:
//    pszStatusIconUri:
//      String representing the Uri of the icon to set.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Adds a status icon to the presentity.
//
//      Note that there can be multiple status-icon element for the same service
//      as long as there are no overlaping from/until attributes. The
//      status-icon element is unique when no from/until attributes are present.
//
//  See Also:
//      GetStatusIcons, UpdateStatusIcon
//
//==============================================================================
inline CXmlElement* CScePidfService::AddStatusIcon(IN const char* pszStatusIconUri)
{
    return CSceXmlHelper::AddElement(INOUT m_pServiceXmlElement,
                                     g_pszXML_NAMESPACE_RPID,
                                     g_pszXML_NAMESPACE_RPID_PREFIX,
                                     g_pszXML_STATUS_ICON,
                                     pszStatusIconUri,
                                     ms_pvecServiceSequence,
                                     true);
}

//==============================================================================
//==
//==  GetTimedStatuses
//==
//==============================================================================
//
//  Parameters:
//    rvpTimedStatus:
//      Vector provided by the application, which is filled with all
//      timed-status elements that this service contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all timed-status elements held by this service element. As defined
//      in RFC 4481:
//
// <CODE>
//    The \<timed-status\> element is a child of the \<tuple\> element and MUST
//    NOT appear as a child of a PIDF \<status\> element or another
//    \<timed-status\> element.  More than one such element MAY appear within
//    a PIDF \<tuple\> element.
// </CODE>
//
//      More information about each timed-status can be retreived using
//      GetTimedStatus. Each timed-status element should have a from and an
//      until attribute, use CSceXmlHelper::GetFrom and CSceXmlHelper::GetUntil
//      to recover these.
//
//  See Also:
//      GetTimedStatus, AddTimedStatus, CSceXmlHelper::GetFrom,
//      CSceXmlHelper::GetUntil.
//
//==============================================================================
inline mxt_result CScePidfService::GetTimedStatuses(OUT CVector<CXmlElement*>& rvpTimedStatus)
{
    return CSceXmlHelper::GetElementFrom(m_pServiceXmlElement,
                                         g_pszXML_NAMESPACE_TIMED_STATUS,
                                         g_pszXML_TIMED_STATUS,
                                         OUT rvpTimedStatus);
}

//==============================================================================
//==
//==  SetCard
//==
//==============================================================================
//
//  Parameters:
//    pszCardUri:
//      The URI pointing to a business card to set.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the business card for this service. From RFC 4482:
//
//   <CODE>
//      The \<card\> element includes a URI pointing to a business card, e.g.,
//      in LDAP Data Interchange Format [15] (LDIF) or vCard [14] format.
//   <CODE>
//
//      This element should only be used when the service has a relationship.
//      From RFC 4482:
//
//   <CODE>
//      The \<tuple\> element is only extended with Contact Information for the
//      Presence Information Data format (CIPID) elements if the information
//      describes a service referring to another person that is marked by an
//      RPID \<relationship\> element with a value other than 'self'.
//   <CODE>
//
//  See Also:
//      GetCard
//
//==============================================================================
inline CXmlElement* CScePidfService::SetCard(IN const char* pszCardUri)
{
    return m_pServiceXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                 g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                 g_pszXML_CARD,
                                                 pszCardUri,
                                                 ms_pvecServiceSequence);
}

//==============================================================================
//==
//==  GetCard
//==
//==============================================================================
//
//  Parameters:
//    ppszCardUri:
//      Double const char pointer, which is set to point to this service's
//      business card. 
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns the card information assigned to this service.
//
//      See SetCard for more information about the card element.
//
//  See Also:
//      SetCard
//
//==============================================================================
inline CXmlElement* CScePidfService::GetCard(OUT const char** ppszCardUri)
{
    return GetElementHelper(g_pszXML_NAMESPACE_CIPID,
                            g_pszXML_CARD,
                            OUT ppszCardUri);
}

//==============================================================================
//==
//==  SetUserInput
//==
//==============================================================================
//
//  Parameters:
//    eInputValue:
//      The input value to set.
//
//    puIdleThreshold:
//      Pointer to the idle threshold to set. Can be NULL, in which case no
//      such information is set in this element.
//
//    pLastInputTime:
//      Pointer to the last time the user has entered input information on his
//      service. Can be NULL, in which case no such information is set in this
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the user input information for this service. From RFC 4480:
//
//   <CODE>  
//      The \<user-input\> element records the user-input or usage state of the
//      service or device, based on human user input, e.g., keyboard,
//      pointing device, or voice.
//   <CODE>  
//
//  See Also:
//      GetUserInput, GetUserInputIdleThreshold, GetUserInputLastInputTime
//
//==============================================================================
inline CXmlElement* CScePidfService::SetUserInput(IN EPidfUserInputValue eInputValue,
                                                  IN const unsigned int* puIdleThreshold,
                                                  IN const CTime* pLastInputTime)
{
    return CSceXmlPrivateHelper::SetUserInputHelper(m_pServiceXmlElement,
                                                    eInputValue,
                                                    puIdleThreshold,
                                                    pLastInputTime,
                                                    ms_pvecServiceSequence);
}

//==============================================================================
//==
//==  GetUserInput
//==
//==============================================================================
//
//  Parameters:
//    peInputValue:
//      Valid pointer to an EPidfUserInputValue, where the user input value
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
//      Returns the user-input information assigned to this service.
//
//      See SetUserInput for more information about the user-input element.
//
//  See Also:
//      SetUserInput, GetUserInputIdleThreshold, GetUserInputLastInputTime
//
//==============================================================================
inline CXmlElement* CScePidfService::GetUserInput(OUT EPidfUserInputValue* peInputValue)
{
    return CSceXmlPrivateHelper::GetUserInputHelper(m_pServiceXmlElement, OUT peInputValue);
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
inline mxt_result CScePidfService::GetUserInputIdleThreshold(IN const CXmlElement* pUserInputElement,
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
//    rpLastInputTime:
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
inline mxt_result CScePidfService::GetUserInputLastInputTime(IN const CXmlElement* pUserInputElement,
                                                             OUT CTime& rLastInputTime)
{
    return CSceXmlPrivateHelper::GetUserInputLastInputTimeHelper(pUserInputElement, OUT rLastInputTime);
}

//==============================================================================
//==
//==  GetDisplayName
//==
//==============================================================================
//
//  Parameters:
//    ppszDisplayName:
//      Double const char pointer, which is set to point to this service's
//      display name. 
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//      
//      Can be NULL, in which case the name is not returned.
//
//    pszLanguageTag:
//      Double const char pointer, which is set to point to the lang attribute
//      of the service's display name.
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//      
//      Can be NULL, in which case the language is not returned.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns the display-name information assigned to this service. A
//      display-name element can have a xml:lang attribute which can be accessed
//      using CSceXmlHelper::GetLanguage.
//
//      See SetDisplayName for more information about the display-name element.
//
//  See Also:
//      SetDisplayName, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline CXmlElement* CScePidfService::GetDisplayName(OUT const char** ppszDisplayName)
{
    return GetElementHelper(g_pszXML_NAMESPACE_CIPID,
                            g_pszXML_DISPLAY_NAME,
                            OUT ppszDisplayName);
}

//==============================================================================
//==
//==  SetHomepage
//==
//==============================================================================
//
//  Parameters:
//    pszHomepageUri:
//      The URI pointing to general information to set.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the general information (typically a web home page) about the
//      service. From RFC 4482:
//
//   <CODE>
//      The \<homepage\> element provides a URI pointing to general information
//      about the tuple or person, typically a web home page.
//   <CODE>
//
//      This element should only be used when the service has a relationship.
//      From RFC 4482:
//
//   <CODE>
//      The \<tuple\> element is only extended with Contact Information for the
//      Presence Information Data format (CIPID) elements if the information
//      describes a service referring to another person that is marked by an
//      RPID \<relationship\> element with a value other than 'self'.
//   <CODE>
//
//  See Also:
//      GetHomepage
//
//==============================================================================
inline CXmlElement* CScePidfService::SetHomepage(IN const char* pszHomepageUri)
{
    return m_pServiceXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                 g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                 g_pszXML_HOMEPAGE,
                                                 pszHomepageUri,
                                                 ms_pvecServiceSequence);
}

//==============================================================================
//==
//==  GetHomepage
//==
//==============================================================================
//
//  Parameters:
//    ppszHomepageUri:
//      Double const char pointer, which is set to point to this service's
//      general information. 
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns the homepage information assigned to this service.
//
//      See SetHomepage for more information about the homepage element.
//
//  See Also:
//      SetHomepage
//
//==============================================================================
inline CXmlElement* CScePidfService::GetHomepage(OUT const char** ppszHomepageUri)
{
    return GetElementHelper(g_pszXML_NAMESPACE_CIPID,
                            g_pszXML_HOMEPAGE,
                            OUT ppszHomepageUri);
}

//==============================================================================
//==
//==  SetIcon
//==
//==============================================================================
//
//  Parameters:
//    pszIconUri:
//      The URI pointing to an image (icon) representing the service.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the image (icon) representing the service. From RFC 4482:
//
//   <CODE>
//      The \<icon\> element provides a URI pointing to an image (icon)
//      representing the tuple or person.
//   <CODE>
//
//      This element should only be used when the service has a relationship.
//      From RFC 4482:
//
//   <CODE>
//      The \<tuple\> element is only extended with Contact Information for the
//      Presence Information Data format (CIPID) elements if the information
//      describes a service referring to another person that is marked by an
//      RPID \<relationship\> element with a value other than 'self'.
//   <CODE>
//
//  See Also:
//      GetIcon
//
//==============================================================================
inline CXmlElement* CScePidfService::SetIcon(IN const char* pszIconUri)
{
    return m_pServiceXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                 g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                 g_pszXML_ICON,
                                                 pszIconUri,
                                                 ms_pvecServiceSequence);
}

//==============================================================================
//==
//==  GetIcon
//==
//==============================================================================
//
//  Parameters:
//    ppszIconUri:
//      Double const char pointer, which is set to point to this service's
//      image (icon). 
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns the image (icon) representing the service.
//
//      See SetIcon for more information about the icon element.
//
//  See Also:
//      SetIcon
//
//==============================================================================
inline CXmlElement* CScePidfService::GetIcon(OUT const char** ppszIconUri)
{
    return GetElementHelper(g_pszXML_NAMESPACE_CIPID,
                            g_pszXML_ICON,
                            OUT ppszIconUri);
}

//==============================================================================
//==
//==  SetMap
//==
//==============================================================================
//
//  Parameters:
//    pszMapUri:
//      The URI pointing to a map related to the service.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the map related to the service. From RFC 4482:
//
//   <CODE>
//      The \<map\> element provides a URI pointing to a map related to the
//      tuple or person.
//   <CODE>
//
//      This element should only be used when the service has a relationship.
//      From RFC 4482:
//
//   <CODE>
//      The \<tuple\> element is only extended with Contact Information for the
//      Presence Information Data format (CIPID) elements if the information
//      describes a service referring to another person that is marked by an
//      RPID \<relationship\> element with a value other than 'self'.
//   <CODE>
//
//  See Also:
//      GetMap
//
//==============================================================================
inline CXmlElement* CScePidfService::SetMap(IN const char* pszMapUri)
{
    return m_pServiceXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                 g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                 g_pszXML_MAP,
                                                 pszMapUri,
                                                 ms_pvecServiceSequence);
}

//==============================================================================
//==
//==  GetMap
//==
//==============================================================================
//
//  Parameters:
//    ppszMapUri:
//      Double const char pointer, which is set to point to the map related to
//      the service. 
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns the map related to the service.
//
//      See SetMap for more information about the icon element.
//
//  See Also:
//      SetMap
//
//==============================================================================
inline CXmlElement* CScePidfService::GetMap(OUT const char** ppszMapUri)
{
    return GetElementHelper(g_pszXML_NAMESPACE_CIPID,
                            g_pszXML_MAP,
                            OUT ppszMapUri);
}

//==============================================================================
//==
//==  SetSound
//==
//==============================================================================
//
//  Parameters:
//    pszSoundUri:
//      The URI pointing to a sound related to the service.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the sound related to the service. From RFC 4482:
//
//   <CODE>
//      The \<sound\> element provides a URI pointing to a sound related to the
//      tuple or person.
//   <CODE>
//
//      This element should only be used when the service has a relationship.
//      From RFC 4482:
//
//   <CODE>
//      The \<tuple\> element is only extended with Contact Information for the
//      Presence Information Data format (CIPID) elements if the information
//      describes a service referring to another person that is marked by an
//      RPID \<relationship\> element with a value other than 'self'.
//   <CODE>
//
//  See Also:
//      GetSound
//
//==============================================================================
inline CXmlElement* CScePidfService::SetSound(IN const char* pszSoundUri)
{
    return m_pServiceXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                 g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                 g_pszXML_SOUND,
                                                 pszSoundUri,
                                                 ms_pvecServiceSequence);
}

//==============================================================================
//==
//==  GetSound
//==
//==============================================================================
//
//  Parameters:
//    ppszSoundUri:
//      Double const char pointer, which is set to point to the sound related to
//      the service. 
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns the sound related to the service.
//
//      See SetSound for more information about the sound element.
//
//  See Also:
//      SetSound
//
//==============================================================================
inline CXmlElement* CScePidfService::GetSound(OUT const char** ppszSoundUri)
{
    return GetElementHelper(g_pszXML_NAMESPACE_CIPID,
                            g_pszXML_SOUND,
                            OUT ppszSoundUri);
}

//==============================================================================
//==
//==  SetServCapsApplication
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support application.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support application. From RFC 5196:
//
//   <CODE>
//      The \<application\> element indicates that the service supports
//      application as a streaming media type as defined in [RFC3840].
//
//      The \<application\> element is a boolean type and does not have any
//      attributes.  The value 'true' indicates that service supports
//      application media type, and the value 'false' indicates that service
//      does not support application media type.
//   <CODE>
//
//    See Also:
//      GetServCapsApplication
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsApplication(IN bool bStatus)
{
    return SetServCapsHelper(g_pszXML_APPLICATION, bStatus);
}

//==============================================================================
//==
//==  GetServCapsApplication
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support application.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support application.
//
//      See SetServCapsApplication for more information about the application
//      element.
//
//  See Also:
//      SetServCapsApplication
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsApplication(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_APPLICATION, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsAudio
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support audio.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support audio. From RFC 5196:
//
//   <CODE>
//      The \<audio\> element indicates that the service supports audio as a
//      streaming media type as defined in [RFC3840].
//
//      The \<audio\> element is a boolean type and does not have any
//      attributes.  The value 'true' indicates that service supports audio
//      media type, and the value 'false' indicates that service does not
//      support audio media type.
//   <CODE>
//
//    See Also:
//      GetServCapsAudio
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsAudio(IN bool bStatus)
{
    return SetServCapsHelper(g_pszXML_AUDIO, bStatus);
}

//==============================================================================
//==
//==  GetServCapsAudio
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support audio.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support audio.
//
//      See SetServCapsAudio for more information about the audio element.
//
//  See Also:
//      SetServCapsAudio
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsAudio(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_AUDIO, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsAutomata
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support an automata.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support an automata. From RFC 5196:
//
//   <CODE>
//      The \<automata\> element indicates whether the service represents an
//      automaton (such as a voicemail server, conference server, or
//      recording device) or a human as defined in [RFC3840].
//
//      The \<automata\> element is a boolean type and does not have any
//      attributes.  The value 'true' indicates that the service represents
//      an automaton, and the value 'false' indicates that it represents a
//      human.
//   <CODE>
//
//    See Also:
//      GetServCapsAutomata
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsAutomata(IN bool bStatus)
{
    return SetServCapsHelper(g_pszXML_AUTOMATA, bStatus);
}

//==============================================================================
//==
//==  GetServCapsAutomata
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support an automata.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support an automata.
//
//      See SetServCapsAutomata for more information about the automata element.
//
//  See Also:
//      SetServCapsAutomata
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsAutomata(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_AUTOMATA, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsControl
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support control.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support control. From RFC 5196:
//
//   <CODE>
//      The \<control\> element indicates that the service supports control as
//      a streaming media type as defined in [RFC3840].
//
//      The \<control\> element is a boolean type and does not have any
//      attributes.  The value 'true' indicates that service supports control
//      media type, and the value 'false' indicates that service does not
//      support control media type.
//   <CODE>
//
//    See Also:
//      GetServCapsControl
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsControl(IN bool bStatus)   
{
    return SetServCapsHelper(g_pszXML_CONTROL, bStatus);
}

//==============================================================================
//==
//==  GetServCapsControl
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support control.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support control.
//
//      See SetServCapsControl for more information about the control element.
//
//  See Also:
//      SetServCapsControl
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsControl(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_CONTROL, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsData
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support data.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support data. From RFC 5196:
//
//   <CODE>
//      The \<data\> element indicates that the service supports data as a
//      streaming media type as defined in [RFC3840].
//
//      The \<data\> element is a boolean type and does not have any
//      attributes.  The value 'true' indicates that service supports data
//      media type, and the value 'false' indicates that service does not
//      support data media type.
//   <CODE>
//
//    See Also:
//      GetServCapsData
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsData(IN bool bStatus)
{
    return SetServCapsHelper(g_pszXML_DATA, bStatus);
}

//==============================================================================
//==
//==  GetServCapsData
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support data.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support data.
//
//      See SetServCapsData for more information about the data element.
//
//  See Also:
//      SetServCapsData
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsData(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_DATA, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsIsfocus
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support conference.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support conferecne. From RFC 5196:
//
//   <CODE>
//      The \<isfocus\> element indicates that the service is a conference
//      server, also known as a focus as defined in [RFC3840].
//
//      The \<isfocus\> element is of boolean type and does not have any
//      attributes.  The value 'true' indicates that service is a conference
//      server and the value 'false' indicates that service does not support
//      conferencing.
//   <CODE>
//
//    See Also:
//      GetServCapsIsfocus
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsIsfocus(IN bool bStatus)
{
    return SetServCapsHelper(g_pszXML_ISFOCUS, bStatus);
}

//==============================================================================
//==
//==  GetServCapsIsfocus
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support conference.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support conference.
//
//      See SetServCapsIsfocus for more information about the IsFocus element.
//
//  See Also:
//      SetServCapsIsfocus
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsIsfocus(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_ISFOCUS, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsMessage
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support message.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support message. From RFC 5196:
//
//   <CODE>
//      The \<message\> element indicates that the service supports messaging
//      as a streaming media type as defined in [RFC4569].
//
//      The \<message\> element is a boolean type and does not have any
//      attributes.  The value 'true' indicates that service supports message
//      media type, and the value 'false' indicates that service does not
//      support message media type.
//   <CODE>
//
//    See Also:
//      GetServCapsMessage
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsMessage(IN bool bStatus)
{
    return SetServCapsHelper(g_pszXML_MESSAGE, bStatus);
}

//==============================================================================
//==
//==  GetServCapsMessage
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support message.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support message.
//
//      See SetServCapsMessage for more information about the message element.
//
//  See Also:
//      SetServCapsMessage
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsMessage(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_MESSAGE, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsText
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support text.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support text. From RFC 5196:
//
//   <CODE>
//      The \<text\> element indicates that the service supports text as a
//      streaming media type as defined in [RFC3840].
//
//      The \<text\> element is a boolean type and does not have any
//      attributes.  The value 'true' indicates that service supports text
//      media type, and the value 'false' indicates that service does not
//      support text media type.
//   <CODE>
//
//    See Also:
//      GetServCapsText
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsText(IN bool bStatus)
{
    return SetServCapsHelper(g_pszXML_TEXT, bStatus);
}

//==============================================================================
//==
//==  GetServCapsText
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support text.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support text.
//
//      See SetServCapsText for more information about the text element.
//
//  See Also:
//      SetServCapsText
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsText(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_TEXT, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsVideo
//==
//==============================================================================
//
//  Parameters:
//    bStatus:
//      Boolean indicating if the service support video.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets if this service support video. From RFC 5196:
//
//   <CODE>
//      The \<video\> element indicates that the service supports video as a
//      streaming media type as defined in [RFC3840].
//
//      The \<video\> element is a boolean type and does not have any
//      attributes.  The value 'true' indicates that service supports video
//      media type, and the value 'false' indicates that service does not
//      support video media type.
//   <CODE>
//
//    See Also:
//      GetServCapsVideo
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsVideo(IN bool bStatus)
{
    return SetServCapsHelper(g_pszXML_VIDEO, bStatus);
}

//==============================================================================
//==
//==  GetServCapsVideo
//==
//==============================================================================
//
//  Parameters:
//    pbStatus:
//      Pointer to a boolean, which is set to point to the value indicating if
//      this service support video.
//      
//      Can be NULL, in which case only the CXmlElement is returned if the
//      element exists.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Returns if the service support video.
//
//      See SetServCapsVideo for more information about the video element.
//
//  See Also:
//      SetServCapsVideo
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsVideo(OUT bool* pbStatus)
{
    return GetServCapsHelper(g_pszXML_VIDEO, OUT pbStatus);
}

//==============================================================================
//==
//==  SetServCapsSupportedDuplex
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the duplex element to set under the supported element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the communication duplex supported by this service. From RFC 5196:
//
//   <CODE>
//      The \<duplex\> element lists whether a communications service can
//      simultaneously send and receive media ("full"), alternate between
//      sending and receiving ("half"), only receive ("receive-only"), or
//      only send ("send-only") as defined in [RFC3840].
//
//      The \<duplex\> element can contain two elements: \<supported\> and
//      \<notsupported\>.  Duplex modes that are supported by the service can
//      be listed under the \<supported\> element, and duplex modes that are
//      not supported by the service can be listed under the \<notsupported\>
//      element.
//   <CODE>
//
//  See Also:
//      GetServCapsSupportedDuplex, SetServCapsUnsupportedDuplex
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsSupportedDuplex(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_DUPLEX,
                                    g_pszXML_SUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSDUPLEX_FULL,
                                    GetPidfServCapsDuplex,
                                    ms_pvecDuplexSequence);
}

//==============================================================================
//==
//==  GetServCapsSupportedDuplex
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the supported duplex element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the supported duplex element from the servcaps element.
//
//      See SetServCapsSupportedDuplex for more information about the duplex
//      element.
//
//  See Also:
//      SetServCapsSupportedDuplex
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsSupportedDuplex(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_DUPLEX,
                                    g_pszXML_SUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsDuplex);
}

//==============================================================================
//==
//==  SetServCapsUnsupportedDuplex
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the duplex element to set under the \<notsupported\>
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the communication duplex not supported by this service.
//
//      See SetServCapsSupportedDuplex for more information about the duplex
//      element.
//
//  See Also:
//      GetServCapsUnsupportedDuplex, SetServCapsSupportedDuplex
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsUnsupportedDuplex(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_DUPLEX,
                                    g_pszXML_NOTSUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSDUPLEX_FULL,
                                    GetPidfServCapsDuplex,
                                    ms_pvecDuplexSequence);
}

//==============================================================================
//==
//==  GetServCapsUnsupportedDuplex
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the \<notsupported\> duplex
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the \<notsupported\> duplex element from the servcaps element.
//
//      See SetServCapsSupportedDuplex for more information about the duplex
//      element.
//
//  See Also:
//      SetServCapsSupportedDuplex, SetServCapsUnsupportedDuplex
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsUnsupportedDuplex(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_DUPLEX,
                                    g_pszXML_NOTSUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsDuplex);
}

//==============================================================================
//==
//==  SetServCapsSupportedClass
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the class element to set under the supported element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the class setting supported by this service. From RFC 5196:
//
//   <CODE>
//      The \<class\> element indicates the setting, business or personal, in
//      which a communications service is used as defined in [RFC3840].
//
//      The \<class\> element can contain two elements: \<supported\> and
//      \<notsupported\>.  Classes that are supported by the service can be
//      listed under the \<supported\> element, and classes that are not
//      supported by the service can be listed under the \<notsupported\>
//      element.
//   <CODE>
//
//  See Also:
//      GetServCapsSupportedClass, SetServCapsUnsupportedClass
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsSupportedClass(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_CLASS,
                                    g_pszXML_SUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSCLASS_BUSINESS,
                                    GetPidfServCapsClass,
                                    ms_pvecClassSequence);
}

//==============================================================================
//==
//==  GetServCapsSupportedClass
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the supported class element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the supported class element from the servcaps element.
//
//      See SetServCapsSupportedClass for more information about the class
//      element.
//
//  See Also:
//      SetServCapsSupportedClass
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsSupportedClass(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_CLASS,
                                    g_pszXML_SUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsClass);
}

//==============================================================================
//==
//==  SetServCapsUnsupportedClass
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the class element to set under the \<notsupported\>
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the class not supported by this service.
//
//      See SetServCapsSupportedClass for more information about the class
//      element.
//
//  See Also:
//      GetServCapsUnsupportedClass, SetServCapsSupportedClass
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsUnsupportedClass(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_CLASS,
                                    g_pszXML_NOTSUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSCLASS_BUSINESS,
                                    GetPidfServCapsClass,
                                    ms_pvecClassSequence);
}

//==============================================================================
//==
//==  GetServCapsUnsupportedClass
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the \<notsupported\> class element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the \<notsupported\> class element from the servcaps element.
//
//      See SetServCapsSupportedClass for more information about the class
//      element.
//
//  See Also:
//      SetServCapsSupportedClass, SetServCapsUnsupportedClass
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsUnsupportedClass(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_CLASS,
                                    g_pszXML_NOTSUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsClass);
}

//==============================================================================
//==
//==  SetServCapsSupportedEventPackages
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the event-packages element to set under the supported
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the event-packages supported by this service. From RFC 5196:
//
//   <CODE>
//      The \<event-packages\> element lists the event packages supported by a
//      service.
//
//      The \<event-packages\> element can contain two elements: \<supported\>
//      and \<notsupported\>.  Event packages that are supported by the service
//      can be listed under the \<supported\> element, and event packages that
//      are not supported by the service can be listed under the
//      \<notsupported\> element
//   <CODE>
//
//  See Also:
//      GetServCapsSupportedEventPackages, SetServCapsUnsupportedEventPackages
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsSupportedEventPackages(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_EVENT_PACKAGES,
                                    g_pszXML_SUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSEVENTPKG_CONFERENCE,
                                    GetPidfServCapsEventPkg,
                                    ms_pvecEventPackagesSequence);
}

//==============================================================================
//==
//==  GetServCapsSupportedEventPackages
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the supported event-packages element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the supported event-packages element from the servcaps element.
//
//      See SetServCapsSupportedEventPackages for more information about the
//      event-packages element.
//
//  See Also:
//      SetServCapsSupportedEventPackages
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsSupportedEventPackages(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_EVENT_PACKAGES,
                                    g_pszXML_SUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsEventPkg);
}

//==============================================================================
//==
//==  SetServCapsSupportedEventPackages
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the event-packages element to set under the
//      \<notsupported\> element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the event-packages not supported by this service.
//
//      See SetServCapsSupportedEventPackages for more information about the
//      event-packages element.
//
//  See Also:
//      GetServCapsUnsupportedEventPackages, SetServCapsSupportedEventPackages
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsUnsupportedEventPackages(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_EVENT_PACKAGES,
                                    g_pszXML_NOTSUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSEVENTPKG_CONFERENCE,
                                    GetPidfServCapsEventPkg,
                                    ms_pvecEventPackagesSequence);
}

//==============================================================================
//==
//==  GetServCapsUnsupportedEventPackages
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the \<notsupported\> event-packages element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the \<notsupported\> event-packages element from the servcaps element.
//
//      See SetServCapsSupportedEventPackages for more information about the
//      event-packages element.
//
//  See Also:
//      SetServCapsSupportedEventPackages, SetServCapsUnsupportedEventPackages
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsUnsupportedEventPackages(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_EVENT_PACKAGES,
                                    g_pszXML_NOTSUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsEventPkg);
}

//==============================================================================
//==
//==  SetServCapsSupportedMethods
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the methods element to set under the supported
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the methods supported by this service. From RFC 5196:
//
//   <CODE>
//      The \<methods\> element indicates the SIP methods supported by a
//      service.  In this case, "supported" means that the service can
//      receive requests with this method.  In that sense, it has the same
//      connotation as the Allow header field as defined in [RFC3840].
//
//      The \<methods\> element can contain two elements: \<supported\> and
//      \<notsupported\>.  Methods that are supported by the service can be
//      listed under the \<supported\> element, and methods that are not
//      supported by the service can be listed under the \<notsupported\>
//      element.
//   <CODE>
//
//  See Also:
//      GetServCapsSupportedMethods, SetServCapsUnsupportedMethods
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsSupportedMethods(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_METHODS,
                                    g_pszXML_SUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSMETHOD_ACK,
                                    GetPidfServCapsMethod,
                                    ms_pvecMethodsSequence);
}

//==============================================================================
//==
//==  GetServCapsSupportedMethods
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the supported methods element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the supported methods element from the servcaps element.
//
//      See SetServCapsSupportedMethods for more information about the methods
//      element.
//
//  See Also:
//      SetServCapsSupportedMethods
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsSupportedMethods(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_METHODS,
                                    g_pszXML_SUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsMethod);
}

//==============================================================================
//==
//==  SetServCapsUnsupportedMethods
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the methods element to set under the
//      \<notsupported\> element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the methods not supported by this service.
//
//      See SetServCapsSupportedMethods for more information about the methods
//      element.
//
//  See Also:
//      GetServCapsUnsupportedMethods, SetServCapsSupportedMethods
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsUnsupportedMethods(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_METHODS,
                                    g_pszXML_NOTSUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSMETHOD_ACK,
                                    GetPidfServCapsMethod,
                                    ms_pvecMethodsSequence);
}

//==============================================================================
//==
//==  GetServCapsUnsupportedMethods
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the \<notsupported\> methods element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the \<notsupported\> methods element from the servcaps element.
//
//      See SetServCapsSupportedMethods for more information about the methods
//      element.
//
//  See Also:
//      SetServCapsSupportedMethods, SetServCapsUnsupportedMethods
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsUnsupportedMethods(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_METHODS,
                                    g_pszXML_NOTSUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsMethod);
}

//==============================================================================
//==
//==  SetServCapsSupportedExtensions
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the extensions element to set under the supported
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the extensions supported by this service. From RFC 5196:
//
//   <CODE>
//      The \<extensions\> element is a list of SIP extensions (each of which
//      is defined by an option-tag registered with IANA) that are understood
//      by the service.  Understood, in this context, means that the option
//      tag would be included in a Supported header field in a request as
//      defined in [RFC3840].
//
//      The \<extensions\> element can contain two elements: \<supported\> and
//      \<notsupported\>.  Extensions that are supported by the service can be
//      listed under the \<supported\> element, and extensions that are not
//      supported by the service can be listed under the \<notsupported\>
//      element.
//   <CODE>
//
//  See Also:
//      GetServCapsSupportedExtensions, SetServCapsUnsupportedExtensions
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsSupportedExtensions(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_EXTENSIONS,
                                    g_pszXML_SUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSEXT_REL100,
                                    GetPidfServCapsExtension,
                                    ms_pvecExtensionsSequence);
}

//==============================================================================
//==
//==  GetServCapsSupportedExtensions
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the supported extensions element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the supported extensions element from the servcaps element.
//
//      See SetServCapsSupportedExtensions for more information about the
//      extensions element.
//
//  See Also:
//      SetServCapsSupportedExtensions
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsSupportedExtensions(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_EXTENSIONS,
                                    g_pszXML_SUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsExtension);
}

//==============================================================================
//==
//==  SetServCapsUnsupportedExtensions
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the extensions element to set under the
//      \<notsupported\> element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the extensions not supported by this service.
//
//      See SetServCapsSupportedExtensions for more information about the
//      extensions element.
//
//  See Also:
//      GetServCapsUnsupportedExtensions, SetServCapsSupportedExtensions
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsUnsupportedExtensions(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_EXTENSIONS,
                                    g_pszXML_NOTSUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSEXT_REL100,
                                    GetPidfServCapsExtension,
                                    ms_pvecExtensionsSequence);
}

//==============================================================================
//==
//==  GetServCapsUnsupportedExtensions
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the \<notsupported\> extensions
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the \<notsupported\> extensions element from the servcaps element.
//
//      See SetServCapsSupportedExtensions for more information about the
//      extensions element.
//
//  See Also:
//      SetServCapsSupportedExtensions, SetServCapsUnsupportedExtensions
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsUnsupportedExtensions(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_EXTENSIONS,
                                    g_pszXML_NOTSUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsExtension);
}

//==============================================================================
//==
//==  SetServCapsSupportedActor
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the actor element to set under the supported
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the actor supported by this service. From RFC 5196:
//
//   <CODE>
//      The \<actor\> element indicates the type of entity that is available at
//      this URI as defined in [RFC3840].
//
//      The \<actor\> element can contain two elements: \<supported\> and
//      \<notsupported\>.  Actor types that are supported by the service can be
//      listed under the \<supported\> element, and actor types that are not
//      supported by the service can be listed under the \<notsupported\>
//      element.
//   <CODE>
//
//  See Also:
//      GetServCapsSupportedActor, SetServCapsUnsupportedActor
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsSupportedActor(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_ACTOR,
                                    g_pszXML_SUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSACTOR_ATTENDANT,
                                    GetPidfServCapsActor,
                                    ms_pvecActorSequence);
}

//==============================================================================
//==
//==  GetServCapsSupportedActor
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the supported actor element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the supported actor element from the servcaps element.
//
//      See SetServCapsSupportedActor for more information about the actor
//      element.
//
//  See Also:
//      SetServCapsSupportedActor
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsSupportedActor(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_ACTOR,
                                    g_pszXML_SUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsActor);
}

//==============================================================================
//==
//==  SetServCapsUnsupportedActor
//==
//==============================================================================
//
//  Parameters:
//    bsCaps:
//      Bitset representing the actor element to set under the \<notsupported\>
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the actor not supported by this service.
//
//      See SetServCapsSupportedActor for more information about the actor
//      element.
//
//  See Also:
//      GetServCapsUnsupportedActor, SetServCapsSupportedActor
//
//==============================================================================
inline CXmlElement* CScePidfService::SetServCapsUnsupportedActor(IN uint32_t bsCaps)
{
    return SetServCapsElementHelper(g_pszXML_ACTOR,
                                    g_pszXML_NOTSUPPORTED,
                                    bsCaps,
                                    uPIDFSERVCAPSACTOR_ATTENDANT,
                                    GetPidfServCapsActor,
                                    ms_pvecActorSequence);
}

//==============================================================================
//==
//==  GetServCapsUnsupportedActor
//==
//==============================================================================
//
//  Parameters:
//    pbsCaps:
//      Bitset, which is set to represent the \<notsupported\> actor element.
//
//  Returns:
//      Pointer to the CXmlElement used to store this information. Will return
//      NULL if no CXmlElement currently exists to store the data.
//
//  Description:
//      Gets the \<notsupported\> actor element from the servcaps element.
//
//      See SetServCapsSupportedActor for more information about the actor
//      element.
//
//  See Also:
//      SetServCapsSupportedActor, SetServCapsUnsupportedActor
//
//==============================================================================
inline CXmlElement* CScePidfService::GetServCapsUnsupportedActor(OUT uint32_t* pbsCaps)
{
    return GetServCapsElementHelper(g_pszXML_ACTOR,
                                    g_pszXML_NOTSUPPORTED,
                                    pbsCaps,
                                    GetPidfServCapsActor);
}

//==============================================================================
//==
//==  GetServCapsSupportedPriorities
//==
//==============================================================================
//
//  Parameters:
//    rvpElems:
//      Vector provided by the application, which is filled with all element
//      under the supported priority elements that this service contains under
//      servcaps element.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all elements held by the supported priority element under the servcaps
//      element of this service. As defined in RFC 5196:
//
// <CODE>
//    The \<priority\> element indicates the call priorities the service is
//    willing to handle as defined in [RFC3840].
//
//    The \<priority\> element can contain two elements: \<supported\> and
//    \<notsupported\>.  Priority values that are supported by the service
//    can be listed under the \<supported\> element, and priority values that
//    are not supported by the service can be listed under the
//    \<notsupported\> element.
// </CODE>
//
//      More information about each priority can be retrieved using
//      GetServCapsPriority.
//
//  See Also:
//      GetServCapsPriority, AddServCapsSupportedPriority, UpdateServCapsPriority
//      GetServCapsUnsupportedPriorities
//
//==============================================================================
inline mxt_result CScePidfService::GetServCapsSupportedPriorities(OUT CVector<CXmlElement*>& rvpElems)
{
    return GetServCapsPriorityHelper(g_pszXML_SUPPORTED, OUT rvpElems);
}

//==============================================================================
//==
//==  GetServCapsUnsupportedPriorities
//==
//==============================================================================
//
//  Parameters:
//    rvpElems:
//      Vector provided by the application, which is filled with all element
//      under the notsupported priority elements that this service contains under
//      servcaps element.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all elements held by the notsupported priority element under the
//      servcaps element of this service.
//
//      More information about each priority can be retrieved using
//      GetServCapsPriority.
//
//      See GetServCapsSupportedPriorities for more information about the priority
//      element.
//
//  See Also:
//      GetServCapsPriority, AddServCapsUnsupportedPriority, UpdateServCapsPriority
//      GetServCapsSupportedPriorities
//
//==============================================================================
inline mxt_result CScePidfService::GetServCapsUnsupportedPriorities(OUT CVector<CXmlElement*>& rvpElems)
{
    return GetServCapsPriorityHelper(g_pszXML_NOTSUPPORTED, OUT rvpElems);
}

//==============================================================================
//==
//==  AddServCapsSupportedScheme
//==
//==============================================================================
//
//  Parameters:
//    pszScheme:
//      Pointer to a string which specifies a URI schemes to add under the
//      supported scheme of this service.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Adds URI schemes to the supported scheme element under the servcaps
//      element of this service.
//
//      See GetServCapsSupportedSchemes for more information about the scheme
//      and s element.
//
//  See Also:
//      GetServCapsSupportedSchemes
//
//==============================================================================
inline CXmlElement* CScePidfService::AddServCapsSupportedScheme(IN const char* pszScheme)
{
    return AddServCapsSupportedSchemeHelper(g_pszXML_SUPPORTED, pszScheme);
}

//==============================================================================
//==
//==  AddServCapsUnsupportedScheme
//==
//==============================================================================
//
//  Parameters:
//    pszScheme:
//      Pointer to a string which specifies a URI schemes to add under the
//      notsupported scheme of this service.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Adds URI schemes to the notsupported scheme element under the servcaps
//      element of this service.
//
//      See GetServCapsSupportedSchemes for more information about the scheme
//      and s element.
//
//  See Also:
//      GetServCapsSupportedSchemes, GetServCapsUnsupportedSchemes
//
//==============================================================================
inline CXmlElement* CScePidfService::AddServCapsUnsupportedScheme(IN const char* pszScheme)
{
    return AddServCapsSupportedSchemeHelper(g_pszXML_NOTSUPPORTED, pszScheme);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEPIDFSERVICE_H
