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
#ifndef MXG_CSCEPIDFPERSON_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCEPIDFPERSON_H
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

#ifndef MXG_SCEPIDFTYPESCONVERTER_H
#include "SceXml/ScePidfTypesConverter.h"
#endif

#ifndef MXG_CSCEXMLHELPER_H
#include "SceXml/CSceXmlHelper.h"
#endif

#ifndef MXG_CSCEXMLPRIVATEHELPER_H
#include "SceXml/CSceXmlPrivateHelper.h"
#endif

#ifndef MXG_CXMLELEMENT_H
#include "Xml/CXmlElement.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CTime;

//==============================================================================
//== Class: CScePidfPerson
//========================================
//<GROUP TID_PACKAGE_SCEXML_CLASSES>
//
// Summary:
//  This class represents a PIDF person.
//
// Description:
//  As per RFC 4479, the CScePidfPerson represents a PIDF person, which "models
//  the human user and their states that are relevant to presence systems."
//
//  This class allows to represent status information about a person, like his
//  mood, the place he is in and what he is doing at the moment.
//
//  The user of this class should include a single person per PIDF publication.
//  As stated in RFC 4479: "In other words, the person component models a single
//  human being, and includes characteristics and statuses that are related to
//  the communication states for a single human being." 
//
//  It is however possible to receive multiple person representation when
//  receiving PIDF information from a peer when the presence system has
//  ambiguous information.
//
//  The CScePidfPerson is a simple wrapper that can be created from a
//  CXmlElement returned by a Pidf Document. It makes it easy to access all
//  standardized elements of the PIDF person element.
//
//  The CScePidfPerson is a light weigth class, it can be created on the stack
//  (as a local variable) without any problem. It however shares a pointer to a
//  CXmlElement that is owned by a Pidf Document. Deleting the associated
//  CScePidfDocument, calling Cleanup on it or deleting parent XML elements of
//  this person element will render this object invalid.
//
// Location:
//   SceXml/CScePidfPerson.h
//
// See Also:
//   IScePidfDocument, CScePidfDevice, CScePidfService
//
//==============================================================================
class CScePidfPerson
{
//-- Friend Declarations.
//M5T_INTERNAL_USE_BEGIN
    friend class CSceUaImppInitializer;
//M5T_INTERNAL_USE_END


//-- New types within class namespace

//M5T_INTERNAL_USE_BEGIN

protected:
    // Summary:
    //  Function pointer to a method specifying how to convert an element name
    //  to a 32 bit bitset.
    typedef uint32_t (*mxt_PFNElementToBitset32)(IN const char* pszName, IN const char* pszNamespaceUri);

    // Summary:
    //  Function pointer to a method specifying how to convert an element name
    //  to a 64 bit bitset.
    typedef uint64_t (*mxt_PFNElementToBitset64)(IN const char* pszName, IN const char* pszNamespaceUri);

    // Summary:
    //  Function pointer to a method specifying how to convert a 32 bit bitset
    //  value to a SPidfData.
    typedef const SPidfData* (*mxt_PFNBitsetToSPidfData32)(IN uint32_t uBitset);

    // Summary:
    //  Function pointer to a method specifying how to convert a 64 bit bitset
    //  value to a SPidfData.
    typedef const SPidfData* (*mxt_PFNBitsetToSPidfData64)(IN uint64_t uBitset);

    //M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CScePidfPerson(IN CXmlElement& rPersonElement);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CScePidfPerson();

    //-- << id Attribute - RFC 4479>>
    // The id attribute is automatically created by the CScePidfDocument when
    // the person element is created. It is a unique unique string used to
    // identify this element.

    // Summary:
    //  Gets the id associated to this person.
    mxt_result GetId(OUT const char*& rpszPersonId);


    //-- << Timestamp element - RFC 4479>>

    // Summary:
    //  Sets the timestamp corresponding to the last time this person
    //  information was changed.
    CXmlElement* SetTimestamp(IN const CTime& pTimestamp);

    // Summary:
    //  Gets the timestamp corresponding to the last time this person
    //  information was changed.
    CXmlElement* GetTimestamp(OUT CTime* pTime);


    //-- << Note element - RFC 4479>>

    // Summary:
    //  Gets a pointer to all Note elements held by this Person.
    mxt_result GetPersonNotes(OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the Person.
    CXmlElement* AddPersonNote(IN const char* pszUtf8EncodedNote,
                               IN const char* pszLanguageTag);

    //-- << Activities - RFC 4480 >>

    // Summary:
    //  Gets a pointer to all activities elements held by this Person.
    mxt_result GetActivities(OUT CVector<CXmlElement*>& rvpActivity);

    // Summary:
    //  Gets the value of a specific \<activities\> element.
    mxt_result GetActivity(IN CXmlElement* pActivityElement, OUT uint32_t& ruActivitiesBitSet);

    // Summary:
    //  Gets all other elements held by an activities element.
    mxt_result GetActivityOtherInfo(IN CXmlElement* pActivityElement, 
                                    OUT CVector<CXmlElement*>& rvpActivityOther);

    // Summary:
    //  Adds activities information to the person.
    CXmlElement* AddActivity(IN uint32_t bsActivities);

    // Summary:
    //  Adds an other element to the specified activities element.
    CXmlElement* AddActivityOtherInfo(IN CXmlElement* pActivityElement,
                                      IN const char* pszOtherInfo,
                                      IN const char* pszLang);

    // Summary:
    //  Updates the activity-describing elements of an activities element.
    mxt_result UpdateActivity(IN CXmlElement* pActivityElement,
                                IN uint32_t uActivitiesBitSet);

    // Summary:
    //  Gets a pointer to all Note elements held by the Activity element.
    mxt_result GetActivityNotes(IN CXmlElement* pActivityElement, 
                                OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the Activity element.
    CXmlElement* AddActivityNote(IN CXmlElement* pActivityElement,
                                 IN const char* pszUtf8EncodedNote,
                                 IN const char* pszLanguageTag);
    
    //-- << Class - RFC 4480 >>

    // Summary:
    //  Sets the class element of this person.
    CXmlElement* SetClass(IN const char* pszClass);

    // Summary:
    //  Gets the class element of this person.
    CXmlElement* GetClass(OUT const char** ppszClass) const;

    //-- << Mood - RFC 4480 >>

    // Summary:
    //  Gets all mood elements held by this person element.
    mxt_result GetMoods(OUT CVector<CXmlElement*>& rvpMood);

    // Summary:
    //  Gets the mood enumeration element of a mood element.
    mxt_result GetMood(IN CXmlElement* pMoodElement, OUT uint64_t& ruMoodsBitSet);

    // Summary:
    //  Gets all other elements held by a mood element.
    mxt_result GetMoodOtherInfo(IN CXmlElement* pMoodElement, 
                                OUT CVector<CXmlElement*>& rvpMoodOther);

    // Summary:
    //  Adds mood information to the presentity.
    CXmlElement* AddMood(IN uint64_t uMoodsBitSet);

    // Summary:
    //   Adds an other mood enumeration element to the specified mood element.
    CXmlElement* AddMoodOtherInfo(IN CXmlElement* pMoodElement,
                                  IN const char* pszOtherInfo,
                                  IN const char* pszLang);

    // Summary:
    //   Updates the mood enumeration elements of a mood element.
    mxt_result UpdateMood(IN CXmlElement* pMoodElement,
                          IN uint32_t uMoodsBitSet);

    // Summary:
    //  Gets a pointer to all Note elements held by the Mood element.
    mxt_result GetMoodNotes(IN CXmlElement* pMoodElement, 
                            OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the Mood element.
    CXmlElement* AddMoodNote(IN CXmlElement* pMoodElement,
                             IN const char* pszUtf8EncodedNote,
                             IN const char* pszLanguageTag);

    //-- << Place-Is - RFC 4480 >>

    // Summary:
    //  Gets all place-is elements held by this person element.
    mxt_result GetPlaceIs(OUT CVector<CXmlElement*>& rvpPlaceIs);

    // Summary:
    //  Gets all major media type attributes associated with this place-is.
    mxt_result GetPlaceIs(IN CXmlElement* pPlaceIsElement, 
                          OUT EPidfPlaceIsAudio* peAudioInfo,
                          OUT EPidfPlaceIsVideo* peVideoInfo,
                          OUT EPidfPlaceIsText* peTextInfo);

    // Summary:
    //  Adds place-is information to the presentity.
    CXmlElement* AddPlaceIs(IN EPidfPlaceIsAudio* peAudioInfo,
                            IN EPidfPlaceIsVideo* peVideoInfo,
                            IN EPidfPlaceIsText* peTextInfo);

    // Summary:
    //  Updates the place-is major media type attributes of a place-is element.
    mxt_result UpdatePlaceIs(IN CXmlElement* pPlaceIsElement,
                             IN EPidfPlaceIsAudio* peAudioInfo,
                             IN EPidfPlaceIsVideo* peVideoInfo,
                             IN EPidfPlaceIsText* peTextInfo);

    // Summary:
    //  Gets a pointer to all Note elements held by the PlaceIs element.
    mxt_result GetPlaceIsNotes(IN CXmlElement* pPlaceIsElement, 
                               OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the PlaceIs element.
    CXmlElement* AddPlaceIsNote(IN CXmlElement* pPlaceIsElement,
                                IN const char* pszUtf8EncodedNote,
                                IN const char* pszLanguageTag);


    //-- << Place-Type - RFC 4480 + 4589 >>
    
    // RFC 4589 details the different place-types we should support.

    // Summary:
    //  Gets all place-type elements held by this person element.
    mxt_result GetPlaceTypes(OUT CVector<CXmlElement*>& rvpPlaceType);

    // Summary:
    //  Gets the type of place element of a place-type element.
    mxt_result GetPlaceType(IN CXmlElement* pPlaceTypeElement, OUT uint64_t& ruPlaceTypesBitSet);

    // Summary:
    //  Adds type of place information to the presentity.
    CXmlElement* AddPlaceType(IN const uint64_t& ruPlaceTypesBitSet);

    // Summary:
    //  Gets all other elements held by pPlaceTypeElement.
    mxt_result GetPlaceTypeOtherInfo(IN CXmlElement* pPlaceTypeElement, 
                                     OUT CVector<CXmlElement*>& rvecpPlaceTypeOther);

    // Summary:
    //  Sets the \<other\> place-type element, using the rpid namespace, that
    //  allows free form text to describe the place of the person.
    CXmlElement* AddPlaceTypeOtherInfo(IN CXmlElement* pPlaceTypeElement,
                                       IN const char* pszOtherInfo,
                                       IN const char* pszLang);

    // Summary:
    //  Gets all other elements held by pPlaceTypeElement.
    mxt_result GetPlaceTypeOtherInfo(IN CXmlElement* pPlaceTypeElement, 
                                     OUT CXmlElement*& rpPlaceTypeOther);

    // Summary:
    //  Sets the \<other\> place-type element, using the rpid namespace, that
    //  allows free form text to describe the place of the person.
    CXmlElement* SetPlaceTypeOtherInfo(IN CXmlElement* pPlaceTypeElement,
                                       IN const char* pszOtherInfo,
                                       IN const char* pszLang);

    // Summary:
    //  Updates the type of place elements of a place-type element.
    mxt_result UpdatePlaceType(IN CXmlElement* pPlaceTypeElement,
                               IN const uint64_t& ruPlaceTypeBitSet);

    // Summary:
    //  Gets a pointer to all Note elements held by the PlaceType element.
    mxt_result GetPlaceTypeNotes(IN CXmlElement* pPlaceTypeElement, 
                                 OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the PlaceType element.
    CXmlElement* AddPlaceTypeNote(IN CXmlElement* pPlaceTypeElement,
                                  IN const char* pszUtf8EncodedNote,
                                  IN const char* pszLanguageTag);

    //-- << Privacy - RFC 4480 >>

    // Summary:
    //  Gets all privacy elements held by this person element.
    mxt_result GetPrivacies(OUT CVector<CXmlElement*>& rvpPrivacy);
    
    // Summary:
    //  Gets the private forms of communication element of a privacy element.
    mxt_result GetPrivacy(IN CXmlElement* pPrivacyElement, 
                          OUT uint32_t& ruPrivacyBitSet);

    // Summary:
    //  Adds private forms of communication to the presentity.
    CXmlElement* AddPrivacy(IN uint32_t uPrivacyBitSet);
    
    // Summary:
    //  Updates the private forms of communication elements of a privacy
    //  element.
    mxt_result UpdatePrivacy(IN CXmlElement* pPrivacyElement, 
                             IN uint32_t uPrivacyBitSet);

    // Summary:
    //  Gets a pointer to all Note elements held by the Privacy element.
    mxt_result GetPrivacyNotes(IN CXmlElement* pPrivacyElement, 
                               OUT CVector<CXmlElement*>& rvpNotes);

    // Summary:
    //  Adds a note to the Privacy element.
    CXmlElement* AddPrivacyNote(IN CXmlElement* pPrivacyElement,
                                IN const char* pszUtf8EncodedNote,
                                IN const char* pszLanguageTag);

    //-- << Sphere - RFC 4480 >>

    // Summary:
    //  Gets all sphere elements held by this person element.
    mxt_result GetSpheres(OUT CVector<CXmlElement*>& rvpSphere);

    // Summary:
    //  Gets all major media type attributes associated with this sphere.
    mxt_result GetSphere(IN CXmlElement* pSphereElement, 
                         OUT EPidfSphere& reSphere);

    // Summary:
    //  Adds current state and role to the presentity.
    CXmlElement* AddSphere(IN EPidfSphere eSphere);

    // Summary:
    //  Updates the sphere current state and role element of a sphere element.
    mxt_result UpdateSphere(IN CXmlElement* pSphereElement,
                            IN EPidfSphere eSphere);


    //-- << Status Icon - RFC 4480 >>

    // Summary:
    //  Gets all status-icon elements held by this person element.
    mxt_result GetStatusIcons(OUT CVector<CXmlElement*>& rvpStatusIcon);

    // Summary:
    //  Adds a status icon to the presentity.
    CXmlElement* AddStatusIcon(IN const char* pszStatusIconUri);

    // Summary:
    //  Updates the icon of a status-icon element.
    mxt_result UpdateStatusIcon(IN CXmlElement* pStatusIconElement,
                                IN const char* pszStatusIconUri);

    
    //-- << Time Offset - RFC 4480 >>

    // Summary:
    //  Gets all time-offset elements held by this person element.
    mxt_result GetTimeOffsets(OUT CVector<CXmlElement*>& rvpTimeOffset);

    // Summary:
    //  Gets the number of minutes of offset from UTC at the person's current
    //      location.
    mxt_result GetTimeOffset(IN CXmlElement* pTimeOffsetElement, 
                             OUT int& rnTimeOffsetMin);

    // Summary:
    //  Adds a time-offset to the presentity.
    CXmlElement* AddTimeOffset(IN int nTimeOffsetMin,
                               IN const char* pszTimeOffsetDescription = NULL);

    // Summary:
    //  Updates the number of minute of offset of a status-icon element.
    mxt_result UpdateTimeOffset(IN CXmlElement* pTimeOffsetElement,
                                IN int nTimeOffsetMin,
                                IN const char* pszTimeOffsetDescription = NULL);

    // Summary:
    //  Gets the description information associated with a time-offset
    //  element.
    mxt_result GetTimeOffsetDescription(IN CXmlElement* pTimeOffsetElement,
                                        OUT const char** ppszTimeOffsetDescription);

    
    //-- << User Input - RFC 4480 >>

    // Summary:
    //  Sets the user input information for this person.
    CXmlElement* SetUserInput(IN EPidfUserInputValue eInputState,
                              IN unsigned int* pIdleThreshold = NULL,
                              IN const CTime* pLastInputTime = NULL);
  
    // Summary:
    //  Returns the user-input information assigned to this person.
    CXmlElement* GetUserInput(OUT EPidfUserInputValue* peInputState);

    // Summary:
    //  Gets the idle-threshold information associated with a user-input
    //  element.
    mxt_result GetUserInputIdleThreshold(IN CXmlElement* pUserInputElement,
                                         OUT unsigned int& ruIdleThreshold);

    // Summary:
    //  Gets the last input time information associated with a user-input
    //  element.
    mxt_result GetUserInputLastInputTime(IN CXmlElement* pUserInputElement,
                                         OUT CTime& rLastInputTime);

    //-- << Card element - RFC 4482 >>

    // Summary:
    //  Sets the business card for this person.
    CXmlElement* SetCard(IN const char* pszCardUri);

    // Summary:
    //  Returns the card information assigned to this person.
    CXmlElement* GetCard(OUT const char** ppszCardUri);

    //-- << DisplayName element - RFC 4482 >>

    // Summary:
    //  Adds a display name for this person.
    CXmlElement* AddDisplayName(IN const char* pszDisplayName,
                                IN const char* pszLanguageTag = NULL);

    // Summary:
    //  Returns the display-name informations assigned to this person.
    mxt_result GetDisplayNames(OUT CVector<CXmlElement*>& rvpDisplayNames);

    //-- << Homepage element - RFC 4482 >>

    // Summary:
    //  Sets the general information (typically a web home page) about the
    //  person.
    CXmlElement* SetHomepage(IN const char* pszHomepageUri);

    // Summary:
    //  Returns the homepage information assigned to this person.
    CXmlElement* GetHomepage(OUT const char** ppszHomepageUri);

    //-- << Icon element - RFC 4482 >>

    // Summary:
    //  Sets the image (icon) representing the person.
    CXmlElement* SetIcon(IN const char* pszIconUri);

    // Summary:
    //  Returns the image (icon) representing the person.
    CXmlElement* GetIcon(OUT const char** ppszIconUri);

    //-- << Map element - RFC 4482 >>

    // Summary:
    //  Sets the map related to the person.
    CXmlElement* SetMap(IN const char* pszMapUri);

    // Summary:
    //  Returns the map related to the person.
    CXmlElement* GetMap(OUT const char** ppszMapUri);

    //-- << Sound element - RFC 4482 >>

    // Summary:
    //  Sets the sound related to the person.
    CXmlElement* SetSound(IN const char* pszSoundUri);

    // Summary:
    //  Returns the sound related to the person.
    CXmlElement* GetSound(OUT const char** ppszSoundUri);

    //-- <OMA extension: overriding-willingness>

    // Summary:
    //  Sets the overrinding willingness the person.
    CXmlElement* SetOverridingWillingness(IN EPidfBasicStatus eValue);

    // Summary:
    //  Returns whether or not the person is willing to accept communications.
    CXmlElement* GetOverridingWillingness(OUT EPidfBasicStatus* peValue);

//M5T_INTERNAL_USE_BEGIN

//-- Hidden Methods
//-------------------
protected:

    //-- << Initialization mechanism >>
    // Summary:
    //  Initializes the CScePidfPerson.
    static mxt_result InitializeCScePidfPerson();

    // Summary:
    //  Finalizes the CScePidfPerson.
    static void FinalizeCScePidfPerson();

    // Summary:
    //  Adds an \<other\> to pParentElement.
    CXmlElement* AddOtherInfoHelper(IN CXmlElement* pParentElement,
                                    IN const char* pszOtherInfo,
                                    IN const char* pszLang);

    // Summary:
    //  Creates and returns a bitset representing the childs of pParentElement.
    uint32_t GetBitsetElementHelper32(IN CXmlElement* pParentElement, IN mxt_PFNElementToBitset32 pfnFunction);

    // Summary:
    //  Creates and returns a bitset representing the childs of pParentElement.
    uint64_t GetBitsetElementHelper64(IN CXmlElement* pParentElement, IN mxt_PFNElementToBitset64 pfnFunction);

    // Summary:
    //  Creates and returns a bitset representing the childs of pParentElement.
    void AddBitsetElementHelper(IN CXmlElement* pElement,
                                IN uint32_t uBitSet,
                                IN mxt_PFNBitsetToSPidfData32 pfnFunction,
                                IN CVector<CXmlElement::SIdentificationInfo>* pvecpElementOrder,
                                IN uint32_t uUnknown,
                                IN uint32_t uOther);

    // Summary:
    //  Creates and returns a bitset representing the childs of pParentElement.
    void AddBitsetElementHelper(IN CXmlElement* pElement,
                                IN uint64_t uBitSet,
                                IN mxt_PFNBitsetToSPidfData64 pfnFunction,
                                IN CVector<CXmlElement::SIdentificationInfo>* pvecpElementOrder,
                                IN uint64_t uUnknown,
                                IN uint64_t uOther,
                                IN const char* pszNamespace = g_pszXML_NAMESPACE_RPID,
                                IN const char* pszNamespacePrefix = g_pszXML_NAMESPACE_RPID_PREFIX);


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CScePidfPerson(IN const CScePidfPerson& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CScePidfPerson& operator=(IN const CScePidfPerson& rSrc);

//-- Hidden Data Members
//------------------------
protected:
private:

    // The internal person XML element.
    CXmlElement* m_pPersonXmlElement;

    // Vector representing the sequence for the <person> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecPersonSequence;

    // Vector representing the sequence <note> followed by <other>. This
    // sequence is used for the <activities>, <mood>, <place-is>, <place-type>
    // and <privacy> elements.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecNoteOtherSequence;

    // Vector representing the second sequence for the <place-is> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecPlaceIsSequence;

    // Vector representing the second sequence for the <privacy> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecPrivacySequence;

    // Vector representing the second sequence for the <overriding-willingness> element.
    static CVector<CXmlElement::SIdentificationInfo>* m_pvecOverridingWillingnessSequence;

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
//    rpszPersonId:
//      Pointer reference to a string whch is filled with the id of this person.
//
//  Returns:
//      resS_OK if the attribute has been found, an error otherwise.
//
//  Description:
//      Gets the id attribute of this person element.
//
//==============================================================================
inline mxt_result CScePidfPerson::GetId(const char*& rpszPersonId)
{
    return m_pPersonXmlElement->GetAttribute(NULL, g_pszXML_ID, rpszPersonId);
}

//==============================================================================
//==
//==  GetPersonNotes
//==
//==============================================================================
//
//  Parameters:
//    rvpNotes:
//      Vector provided by the application, which is filled with all notes
//      elements that this person contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all notes elements held by this person element.
//
//      The UTF-8 encoded note text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the note text is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      AddPersonNote, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline mxt_result CScePidfPerson::GetPersonNotes(OUT CVector<CXmlElement*>& rvpNotes)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_PIDFDATAMODEL,
                                         g_pszXML_NOTE,
                                         OUT rvpNotes);
}

//==============================================================================
//==
//==  GetActivities
//==
//==============================================================================
//
//  Parameters:
//    rvpActivity:
//      Vector provided by the application, which is filled with all
//      activities elements that this person contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all activities elements held by this person element. As defined in
//      RFC 4480:
//
// <CODE>
//     The <activities> element describes what the person is currently
//     doing, expressed as an enumeration of activity-describing elements.
// </CODE>
//
//      More information about each activities can be retreived using
//      GetActivity.
//
//      The caller can also use CSceXmlHelper::GetFrom and
//      CSceXmlHelper::GetUntil on the returned CXmlElements to find out the
//      validity period of each activities.
//
//  See Also:
//      AddActivity
//
//==============================================================================
inline mxt_result CScePidfPerson::GetActivities(OUT CVector<CXmlElement*>& rvpActivity)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_ACTIVITIES,
                                         OUT rvpActivity);
}

//==============================================================================
//==
//==  GetActivityOtherInfo
//==
//==============================================================================
//
//  Parameters:
//    pActivityElement:
//      Pointer to the activities element from which to get its
//      activity-describing  \<other\> element. Cannot be NULL. This is usually
//      an element retreived after calling GetActivities.
//
//    rvpActivityOther:
//      Vector provided by the application, which is filled with all
//      other elements that pActivityElement contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets the \<other\> activities held by pActivityElement. 
//
//      The UTF-8 encoded activity text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the activity is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      GetActivities, uPIDFACT_APPOINTMENT
//
//==============================================================================
inline mxt_result CScePidfPerson::GetActivityOtherInfo(IN CXmlElement* pActivityElement, 
                                                       OUT CVector<CXmlElement*>& rvpActivityOther)
{
    return CSceXmlHelper::GetElementFrom(pActivityElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_OTHER,
                                         OUT rvpActivityOther);
}

//==============================================================================
//==
//==  AddActivityOtherInfo
//==
//==============================================================================
//
//  Parameters:
//    pActivityElement:
//      Pointer to the activities element in which to add an other element.
//      Cannot be NULL. This is usually an element retreived after calling
//      GetActivities.
//
//    pszOtherInfo:
//      Pointer to the UTF-8 encoded note. Cannot be NULL.
//
//    pszLang:
//      Pointer to the language tag that represents the language of this
//      activity. Can be NULL, in which case no tag is set.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information. Will
//      return NULL if pszOtherInfo or pszLang are NULL.
//
//  Description:
//      Adds an \<other\> activity element that allows free form text to
//      describe a specific activity not defined in the enumerated activities.
//
//      Note that the XML schema for representing activities allows either to
//      specify that the activity is unknown OR to specify a list of zero or
//      more activities. This method thus verifies if the \<unknown\> activity
//      is included as a children of pActivityElement. If it is present, it is
//      automatically removed.
//
//  See Also:
//      GetActivityOtherInfo, uPIDFACT_APPOINTMENT
//
//==============================================================================
inline CXmlElement* CScePidfPerson::AddActivityOtherInfo(IN CXmlElement* pActivityElement,
                                                         IN const char* pszOtherInfo,
                                                         IN const char* pszLang)
{
    return AddOtherInfoHelper(pActivityElement, pszOtherInfo, pszLang);
}

//==============================================================================
//==
//==  GetActivityNotes
//==
//==============================================================================
//
//  Parameters:
//    pActivityElement:
//      Pointer to the activities element from which to get the notes. Cannot be
//      NULL. This is usually an element retreived after calling GetActivities.
//
//    rvpNotes:
//      Vector provided by the application, which is filled with all notes
//      elements that pActivityElement contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all notes elements held by pActivityElement.
//
//      The UTF-8 encoded note text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the note text is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      AddActivityNote, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline mxt_result CScePidfPerson::GetActivityNotes(IN CXmlElement* pActivityElement, 
                                                   OUT CVector<CXmlElement*>& rvpNotes)
{
    return CSceXmlHelper::GetElementFrom(pActivityElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_NOTE,
                                         OUT rvpNotes);
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
//      Sets the class information for this person. From RFC 4480:
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
inline CXmlElement* CScePidfPerson::SetClass(IN const char* pszClass)
{
    return m_pPersonXmlElement->SetChildElement(g_pszXML_NAMESPACE_RPID,
                                                g_pszXML_NAMESPACE_RPID_PREFIX,
                                                g_pszXML_CLASS,
                                                pszClass,
                                                m_pvecPersonSequence);
}

//==============================================================================
//==
//==  GetMoods
//==
//==============================================================================
//
//  Parameters:
//    rvpMood:
//      Vector provided by the application, which is filled with all
//      mood elements that this person contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all mood elements held by this person element. As defined in
//      RFC 4480:
//
// <CODE>
//     The <mood> element describes the mood of the presentity.
// </CODE>
//
//      More information about each mood can be retreived using
//      GetMood.
//
//      The caller can also use CSceXmlHelper::GetFrom and
//      CSceXmlHelper::GetUntil on the returned CXmlElements to find out the
//      validity period of each mood.
//
//  See Also:
//      AddMood
//
//==============================================================================
inline mxt_result CScePidfPerson::GetMoods(OUT CVector<CXmlElement*>& rvpMood)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_MOOD,
                                         OUT rvpMood);
}

//==============================================================================
//==
//==  GetMoodOtherInfo
//==
//==============================================================================
//
//  Parameters:
//    pMoodElement:
//      Pointer to the mood element from which to get it's mood enumeration
//      \<other\> element. Cannot be NULL. This is usually an element retreived
//      after calling GetMoods.
//
//    rvpMoodOther:
//      Vector provided by the application, which is filled with all
//      other elements that pMoodElement contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets the \<other\> moods held by pActivityElement.
//
//      The UTF-8 encoded mood text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the mood is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      GetMoods
//
//==============================================================================
inline mxt_result CScePidfPerson::GetMoodOtherInfo(IN CXmlElement* pMoodElement, 
                                                   OUT CVector<CXmlElement*>& rvpMoodOther)
{
    return CSceXmlHelper::GetElementFrom(pMoodElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_OTHER,
                                         OUT rvpMoodOther);
}

//==============================================================================
//==
//==  AddMoodOtherInfo
//==
//==============================================================================
//
//  Parameters:
//    pMoodElement:
//      Pointer to the mood element in which to add an other element.
//      Cannot be NULL. This is usually an element retreived after calling
//      GetMoods.
//
//    pszOtherInfo:
//      Pointer to the UTF-8 encoded note. Cannot be NULL.
//
//    pszLang:
//      Pointer to the language tag that represents the language of this mood.
//      Can be NULL, in which case no tag is set.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information. Will
//      return NULL if pszOtherInfo or pszLang are NULL.
//
//  Description:
//      Adds an \<other\> mood element that allows free form text to
//      describe a specific mood not defined enumerated moods.
//
//      Note that the XML schema for representing moods allows either to
//      specify that the mood is unknown OR to specify a list of zero or
//      more moods. This method thus verifies if the \<unknown\> mood
//      is included as a children of pMoodElement. If it is present, it is
//      automatically removed.
//
//  See Also:
//      GetMoodOtherInfo
//
//==============================================================================
inline CXmlElement* CScePidfPerson::AddMoodOtherInfo(IN CXmlElement* pMoodElement,
                                                     IN const char* pszOtherInfo,
                                                     IN const char* pszLang)
{
    return AddOtherInfoHelper(pMoodElement, pszOtherInfo, pszLang);
}

//==============================================================================
//==
//==  GetMoodNotes
//==
//==============================================================================
//
//  Parameters:
//    pMoodElement:
//      Pointer to the mood element from which to get the notes. Cannot be
//      NULL. This is usually an element retreived after calling GetMoods.
//
//    rvpNotes:
//      Vector provided by the application, which is filled with all notes
//      elements that pMoodElement contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all notes elements held by pMoodElement.
//
//      The UTF-8 encoded note text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the note text is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      AddMoodNote, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline mxt_result CScePidfPerson::GetMoodNotes(IN CXmlElement* pMoodElement, 
                                            OUT CVector<CXmlElement*>& rvpNotes)
{
    return CSceXmlHelper::GetElementFrom(pMoodElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_NOTE,
                                         OUT rvpNotes);
}

//==============================================================================
//==
//==  GetPlaceIs
//==
//==============================================================================
//
//  Parameters:
//    rvpPlaceIs:
//      Vector provided by the application, which is filled with all
//      place-is elements that this person contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all place-is elements held by this person element. As defined in
//      RFC 4480:
//
// <CODE>
//     The <place-is> element describes properties of the place the person is
//     currently at.
// </CODE>
//
//      More information about each place-is can be retreived using
//      GetPlaceIs.
//
//      The caller can also use CSceXmlHelper::GetFrom and
//      CSceXmlHelper::GetUntil on the returned CXmlElements to find out the
//      validity period of each pace-is information.
//
//  See Also:
//      GetPlaceIs, AddPlaceIs
//
//==============================================================================
inline mxt_result CScePidfPerson::GetPlaceIs(OUT CVector<CXmlElement*>& rvpPlaceIs)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_PLACE_IS,
                                         OUT rvpPlaceIs);
}

//==============================================================================
//==
//==  GetPlaceIsNotes
//==
//==============================================================================
//
//  Parameters:
//    pPlaceIsElement:
//      Pointer to the placeis element from which to get the notes. Cannot be
//      NULL. This is usually an element retreived after calling GetPlaceIs.
//
//    rvpNotes:
//      Vector provided by the application, which is filled with all notes
//      elements that pPlaceIsElement contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all notes elements held by pPlaceIsElement.
//
//      The UTF-8 encoded note text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the note text is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      AddPlaceIsNote, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline mxt_result CScePidfPerson::GetPlaceIsNotes(IN CXmlElement* pPlaceIsElement, 
                                                  OUT CVector<CXmlElement*>& rvpNotes)
{
    return CSceXmlHelper::GetElementFrom(pPlaceIsElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_NOTE,
                                         OUT rvpNotes);
}

//==============================================================================
//==
//==  GetPlaceTypes
//==
//==============================================================================
//
//  Parameters:
//    rvpPlaceType:
//      Vector provided by the application, which is filled with all
//      place-type elements that this person contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all place-type elements held by this person element. As defined in
//      RFC 4480:
//
// <CODE>
//     The <place-type> element describes the type of place the person is
//     currently at.
// </CODE>
//
//      More information about each place-type can be retreived using
//      GetPlaceType.
//
//      The caller can also use CSceXmlHelper::GetFrom and
//      CSceXmlHelper::GetUntil on the returned CXmlElements to find out the
//      validity period of each place type information.
//
//  See Also:
//      AddPlaceType
//
//==============================================================================
inline mxt_result CScePidfPerson::GetPlaceTypes(OUT CVector<CXmlElement*>& rvpPlaceType)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_PLACE_TYPE,
                                         OUT rvpPlaceType);
}

//==============================================================================
//==
//==  GetPlaceTypeOtherInfo
//==
//==============================================================================
//
//  Parameters:
//    pPlaceTypeElement:
//      Pointer to the place-type element from which to get its \<other\>
//      element. Cannot be NULL. This is usually an element retreived after
//      calling GetPlaceTypes.
//
//    rpPlaceTypeOther:
//      Pointer provided by the application, which is pointed to the other
//      element that pPlaceTypeElement contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets the \<other\>, using the rpid namespace, place-types held by
//      pPlaceTypeElement.
//
//      Note that this method gets all the the <other> elements using the
//      location-type namespace (urn:ietf:params:xml:ns:location-type) as
//      defined in RFC4589.
//
//      The UTF-8 encoded type of place text can be accessed through the element
//      returned in parameter, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the place-type is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      GetPlaceTypes, AddPlaceTypeOtherInfo
//
//==============================================================================
inline mxt_result CScePidfPerson::GetPlaceTypeOtherInfo(IN CXmlElement* pPlaceTypeElement, 
                                                        OUT CVector<CXmlElement*>& rvecpPlaceTypeOther)
{
    return CSceXmlHelper::GetElementFrom(pPlaceTypeElement,
                                         g_pszXML_NAMESPACE_LOCATIONTYPE,
                                         g_pszXML_OTHER,
                                         OUT rvecpPlaceTypeOther);
}

//==============================================================================
//==
//==  GetPlaceTypeNotes
//==
//==============================================================================
//
//  Parameters:
//    pPlaceTypeElement:
//      Pointer to the place-type element from which to get the notes. Cannot be
//      NULL. This is usually an element retreived after calling GetPlaceTypes.
//
//    rvpNotes:
//      Vector provided by the application, which is filled with all notes
//      elements that pPlaceTypeElement contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all notes elements held by pPlaceTypeElement.
//
//      The UTF-8 encoded note text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the note text is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      AddPlaceTypeNote, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline mxt_result CScePidfPerson::GetPlaceTypeNotes(IN CXmlElement* pPlaceTypeElement, 
                                                    OUT CVector<CXmlElement*>& rvpNotes)
{
    return CSceXmlHelper::GetElementFrom(pPlaceTypeElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_NOTE,
                                         OUT rvpNotes);
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
//      privacy elements that this person contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all privacy elements held by this person element. As defined in
//      RFC 4480:
//
// <CODE>
//     The <privacy> element indicates which types of communication third
//     parties in the vicinity of the presentity are unlikely to be able to
//     intercept accidentally or intentionally.
// </CODE>
//
//      More information about each privacy can be retreived using
//      GetPrivacy.
//
//      The caller can also use CSceXmlHelper::GetFrom and
//      CSceXmlHelper::GetUntil on the returned CXmlElements to find out the
//      validity period of each privacy information.
//
//  See Also:
//      GetPrivacy, AddPrivacy
//
//==============================================================================
inline mxt_result CScePidfPerson::GetPrivacies(OUT CVector<CXmlElement*>& rvpPrivacy)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_PRIVACY,
                                         OUT rvpPrivacy);
}

//==============================================================================
//==
//==  GetPrivacyNotes
//==
//==============================================================================
//
//  Parameters:
//    pPrivacyElement:
//      Pointer to the privacy element from which to get the notes. Cannot be
//      NULL. This is usually an element retreived after calling GetPrivacy.
//
//    rvpNotes:
//      Vector provided by the application, which is filled with all notes
//      elements that pPrivacyElement contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all notes elements held by pPrivacyElement.
//
//      The UTF-8 encoded note text can be accessed through the elements
//      returned in the vector, using <C>CXmlElement::GetValue()</C>.
//
//      The language used in the note text is specified in the "lang"
//      attribute. This language information can easily be accessed using
//      <C>CSceXmlHelper::GetLanguage()</C>
//
//  See Also:
//      AddPrivacyNote, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline mxt_result CScePidfPerson::GetPrivacyNotes(IN CXmlElement* pPrivacyElement, 
                                                  OUT CVector<CXmlElement*>& rvpNotes)
{
    return CSceXmlHelper::GetElementFrom(pPrivacyElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_NOTE,
                                         OUT rvpNotes);
}

//==============================================================================
//==
//==  GetSpheres
//==
//==============================================================================
//
//  Parameters:
//    rvpSphere:
//      Vector provided by the application, which is filled with all
//      sphere elements that this person contains.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all sphere elements held by this person element. As defined in
//      RFC 4480:
//
// <CODE>
//     The <sphere> element designates the current state and role that the
//     person plays.
// </CODE>
//
//      More information about each sphere can be retreived using GetSphere.
//
//      The caller can also use CSceXmlHelper::GetFrom and
//      CSceXmlHelper::GetUntil on the returned CXmlElements to find out the
//      validity period of each sphere information.
//
//  See Also:
//      GetSphere, AddSphere
//
//==============================================================================
inline mxt_result CScePidfPerson::GetSpheres(OUT CVector<CXmlElement*>& rvpSphere)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_SPHERE,
                                         OUT rvpSphere);
}

//==============================================================================
//==
//==  GetStatusIcons
//==
//==============================================================================
//
//  Parameters:
//    rvpStatusIcon:
//      Vector provided by the application, which is filled with all
//      status-icon elements.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all status-icon elements held by this person element. As defined in
//      RFC 4480:
//
// <CODE>
//     The <status-icon> element includes a URI pointing to an image (icon)
//     representing the current status of the person or service.
// </CODE>
//
//      More information about each status-icon can be retreived using
//      CXmlElement::GetValue.
//
//      The caller can also use CSceXmlHelper::GetFrom and
//      CSceXmlHelper::GetUntil on the returned CXmlElements to find out the
//      validity period of each status-icon information.
//
//  See Also:
//      CXmlElement::GetValue, AddStatusIcons
//
//==============================================================================
inline mxt_result CScePidfPerson::GetStatusIcons(OUT CVector<CXmlElement*>& rvpStatusIcon)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_STATUS_ICON,
                                         OUT rvpStatusIcon);
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
//      The "from" and "until" attributes can be assigned to the returned
//      element using CSceXmlHelper::SetFrom and CSceXmlHelper::SetUntil. These
//      attributes are used to assign a validity period to the status-icon
//      element and MUST be used if more than one such element is included in
//      the person element. Moreover, the values of "from" and "until" of two
//      status-icon elements must not overlap.
//
//  See Also:
//      GetStatusIcons, GetStatusIcon, UpdateStatusIcon
//
//==============================================================================
inline CXmlElement* CScePidfPerson::AddStatusIcon(IN const char* pszStatusIconUri)
{
    return CSceXmlHelper::AddElement(m_pPersonXmlElement,
                                     g_pszXML_NAMESPACE_RPID,
                                     g_pszXML_NAMESPACE_RPID_PREFIX,
                                     g_pszXML_STATUS_ICON,
                                     pszStatusIconUri,
                                     m_pvecPersonSequence,
                                     true);
}

//==============================================================================
//==
//==  GetTimeOffsets
//==
//==============================================================================
//
//  Parameters:
//    rvpTimeOffsets:
//      Vector provided by the application, which is filled with all
//      time-offset elements.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Gets all time-offset elements held by this person element. As defined in
//      RFC 4480:
//
// <CODE>
//     The <time-offset> element describes the number of minutes of offset
//     from UTC at the person's current location.
// </CODE>
//
//      More information about each time-offset can be retreived using
//      CScePidfPerson::GetTimeOffset.
//
//      The caller can also use CSceXmlHelper::GetFrom and
//      CSceXmlHelper::GetUntil on the returned CXmlElements to find out the
//      validity period of each time-offset information.
//
//  See Also:
//      CScePidfPerson::GetTimeOffset, AddTimeOffset
//
//==============================================================================
inline mxt_result CScePidfPerson::GetTimeOffsets(OUT CVector<CXmlElement*>& rvpTimeOffset)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_RPID,
                                         g_pszXML_TIME_OFFSET,
                                         OUT rvpTimeOffset);
}

//==============================================================================
//==
//==  GetTimeOffsetDescription
//==
//==============================================================================
//
//  Parameters:
//    pTimeOffsetElement:
//      Pointer to the time-offset element from which the idle threshold must be
//      fetched. This pointer can be retreived through GetTimeOffsets. Cannot be
//      NULL.
//
//    ppszTimeOffsetDescription:
//      Double const char pointer, which is set to point to the time-offset
//      description.
//      
//      The pointer returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//
//  Returns:
//      resS_OK upon success, resFE_INVALID_ARGUMENT if pTimeOffsetElement is
//      NULL, and resFE_FAIL if the idle threshold is not set in the element.
//
//  Description:
//      Gets the description information associated with a time-offset
//      element.
//
//  See Also:
//      GetTimeOffsets
//
//==============================================================================
inline mxt_result CScePidfPerson::GetTimeOffsetDescription(IN CXmlElement* pTimeOffsetElement,
                                                           OUT const char** ppszTimeOffsetDescription)
{
    return pTimeOffsetElement->GetAttribute(NULL,
                                            g_pszXML_DESCRIPTION,
                                            *ppszTimeOffsetDescription);
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
//      person. Can be NULL, in which case no such information is set in this
//      element.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the user input information for this person. From RFC 4480:
//
//   <CODE>  
//      The <user-input> element records the user-input or usage state of the
//      service or device, based on human user input, e.g., keyboard,
//      pointing device, or voice. If contained in a <person> element, it
//      summarizes any user input activity across all services and devices
//      operated by the presentity.
//   <CODE>  
//
//  See Also:
//      GetUserInput, GetUserInputIdleThreshold, GetUserInputLastInputTime
//
//==============================================================================
inline CXmlElement* CScePidfPerson::SetUserInput(IN EPidfUserInputValue eInputState,
                                                 IN unsigned int* pIdleThreshold,
                                                 IN const CTime* pLastInputTime)
{
    return CSceXmlPrivateHelper::SetUserInputHelper(m_pPersonXmlElement,
                                                    eInputState,
                                                    pIdleThreshold,
                                                    pLastInputTime,
                                                    m_pvecPersonSequence);
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
//      Returns the user-input information assigned to this person.
//
//      See SetUserInput for more information about the user-input element.
//
//  See Also:
//      SetUserInput, GetUserInputIdleThreshold, GetUserInputLastInputTime
//
//==============================================================================
inline CXmlElement* CScePidfPerson::GetUserInput(OUT EPidfUserInputValue* peInputState)
{
    return CSceXmlPrivateHelper::GetUserInputHelper(m_pPersonXmlElement,
                                                    OUT peInputState);
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
inline mxt_result CScePidfPerson::GetUserInputIdleThreshold(IN CXmlElement* pUserInputElement,
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
inline mxt_result CScePidfPerson::GetUserInputLastInputTime(IN CXmlElement* pUserInputElement,
                                                            OUT CTime& rLastInputTime)
{
    return CSceXmlPrivateHelper::GetUserInputLastInputTimeHelper(pUserInputElement, OUT rLastInputTime);
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
//      Sets the business card for this person. From RFC 4482:
//
//   <CODE>  
//      The <card> element includes a URI pointing to a business card, e.g.,
//      in LDAP Data Interchange Format [15] (LDIF) or vCard [14] format.
//   <CODE>  
//
//  See Also:
//      GetCard
//
//==============================================================================
inline CXmlElement* CScePidfPerson::SetCard(IN const char* pszCardUri)
{
    return m_pPersonXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                g_pszXML_CARD,
                                                pszCardUri,
                                                m_pvecPersonSequence);

}

//==============================================================================
//==
//==  GetDisplayNames
//==
//==============================================================================
//
//  Parameters:
//    rvpDisplayNames:
//      Vector which is filled with every <display-name> elements that this
//      <person> contains.
//      
//      The pointers returned through this parameter must be considered as
//      temporary and can become invalid any time this element or the PIDF
//      document is modified. The user should copy the string it points to if it
//      wants to keep this information for any length of time.
//
//  Returns:
//      resS_OK upon success, an error otherwise.
//
//  Description:
//      Returns the display-name informations assigned to this person.
//
//      For more information abot each <display-name> element used
//      CXmlElement::GetValue to access the display name and use
//      CSceXmlHelper::GetLanguage for the language.
//
//  See Also:
//      SetDisplayName, CXmlElement::GetValue, CSceXmlHelper::GetLanguage
//
//==============================================================================
inline mxt_result CScePidfPerson::GetDisplayNames(OUT CVector<CXmlElement*>& rvpDisplayNames)
{
    return CSceXmlHelper::GetElementFrom(m_pPersonXmlElement,
                                         g_pszXML_NAMESPACE_CIPID,
                                         g_pszXML_DISPLAY_NAME,
                                         OUT rvpDisplayNames);
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
//      person. From RFC 4482:
//
//   <CODE>
//      The <homepage> element provides a URI pointing to general information
//      about the tuple or person, typically a web home page.
//   <CODE>
//
//  See Also:
//      GetHomepage
//
//==============================================================================
inline CXmlElement* CScePidfPerson::SetHomepage(IN const char* pszHomepageUri)
{

    return m_pPersonXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                g_pszXML_HOMEPAGE,
                                                pszHomepageUri,
                                                m_pvecPersonSequence);
}

//==============================================================================
//==
//==  SetIcon
//==
//==============================================================================
//
//  Parameters:
//    pszIconUri:
//      The URI pointing to an image (icon) representing the person.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the image (icon) representing the person. From RFC 4482:
//
//   <CODE>
//      The <icon> element provides a URI pointing to an image (icon)
//      representing the tuple or person.
//   <CODE>
//
//  See Also:
//      GetIcon
//
//==============================================================================
inline CXmlElement* CScePidfPerson::SetIcon(IN const char* pszIconUri)
{
    return m_pPersonXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                g_pszXML_ICON,
                                                pszIconUri,
                                                m_pvecPersonSequence);
}

//==============================================================================
//==
//==  SetMap
//==
//==============================================================================
//
//  Parameters:
//    pszMapUri:
//      The URI pointing to a map related to the person.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the map related to the person. From RFC 4482:
//
//   <CODE>
//      The <map> element provides a URI pointing to a map related to the
//      tuple or person.
//   <CODE>
//
//  See Also:
//      GetMap
//
//==============================================================================
inline CXmlElement* CScePidfPerson::SetMap(IN const char* pszMapUri)
{
    return m_pPersonXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                g_pszXML_MAP,
                                                pszMapUri,
                                                m_pvecPersonSequence);
}

//==============================================================================
//==
//==  SetSound
//==
//==============================================================================
//
//  Parameters:
//    pszSoundUri:
//      The URI pointing to a sound related to the person.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the sound related to the person. From RFC 4482:
//
//   <CODE>
//      The <sound> element provides a URI pointing to a sound related to the
//      tuple or person.
//   <CODE>
//
//  See Also:
//      GetSound
//
//==============================================================================
inline CXmlElement* CScePidfPerson::SetSound(IN const char* pszSoundUri)
{
    return m_pPersonXmlElement->SetChildElement(g_pszXML_NAMESPACE_CIPID,
                                                g_pszXML_NAMESPACE_CIPID_PREFIX,
                                                g_pszXML_SOUND,
                                                pszSoundUri,
                                                m_pvecPersonSequence);
}

//==============================================================================
//==
//==  SetOverridingWillingness
//==
//==============================================================================
//
//  Parameters:
//    eValue:
//      The overriding willingness for this person.
//
//  Returns:
//      Pointer to the CXmlElement used/created to store this information.
//
//  Description:
//      Sets the overrinding willingness the person. From the
//      OMA-DDS-Presence_SIMPLE-V1_0-20080627-A document:
//
//   <CODE>
//      The "Overriding Willingness" provides an indication, set by the
//      Presentity that takes precedence over "Application-specific willingness"
//      settings. For example, when an "Overriding Willingness" element is
//      present, a positive setting indicates that the user is willing to accept
//      communications for all available communications types, while a negative
//      setting indicates that the user is not willing to accept any
//      communications.
//   <CODE>
//
//  See Also:
//      GetOverridingWillingness
//
//==============================================================================
inline CXmlElement* CScePidfPerson::SetOverridingWillingness(IN EPidfBasicStatus eValue)
{
    return m_pPersonXmlElement->SetChildElement(g_pszXML_NAMESPACE_OMAPRES,
                                                g_pszXML_NAMESPACE_OMAPRES_PREFIX,
                                                g_pszXML_OVERRIDING_WILLINGNESS,
                                                GetPidfBasicStatus(eValue),
                                                m_pvecPersonSequence);
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCEPIDFPERSON_H

