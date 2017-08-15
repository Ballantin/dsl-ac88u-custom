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
#ifndef MXG_SCEPIDFTYPESCONVERTER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCEPIDFTYPESCONVERTER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCEPIDFDATA_H
#include "SceXml/ScePidfData.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//====  PUBLIC METHODS  ========================================================
//==============================================================================

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the bool corresponding to the C-style string in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
bool GetBooleanValue(IN const char* const pszBooleanValue);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the C-style string corresponding to the boolean value in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const char* GetBooleanValue(IN bool bValue);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the enum corresponding to the C-style string in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfBasicStatus GetPidfBasicStatus(IN const char* const pszBasicStatus);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the C-style string corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const char* GetPidfBasicStatus(IN EPidfBasicStatus eBasicStatus);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfActivity(IN const char* const pszActivity,
                         IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfActivity(IN uint32_t uActivity);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint64_t GetPidfMood(IN const char* const pszMood,
                     IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfMood(IN uint64_t uMood);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
EPidfPlaceIsAudio GetPidfPlaceIsAudio(IN const char* const pszPlaceIsAudio,
                                      IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfPlaceIsAudio(IN EPidfPlaceIsAudio ePlaceIsAudio);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfPlaceIsVideo GetPidfPlaceIsVideo(IN const char* const pszPlaceIsVideo,
                                      IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfPlaceIsVideo(IN EPidfPlaceIsVideo ePlaceIsVideo);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfPlaceIsText GetPidfPlaceIsText(IN const char* const pszPlaceIsText,
                                    IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfPlaceIsText(IN EPidfPlaceIsText ePlaceIsText);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint64_t GetPidfPlaceType(IN const char* const pszPlaceType,
                          IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfPlaceType(IN uint64_t uPlaceType);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfRelationship GetPidfRelationship(IN const char* const pszRelationship,
                                      IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfRelationship(IN EPidfRelationship eRelationship);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfServiceClass GetPidfServiceClass(IN const char* const pszServiceClass,
                                      IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfServiceClass(IN EPidfServiceClass eServiceClass);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfSphere GetPidfSphere(IN const char* const pszSphere,
                          IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfSphere(IN EPidfSphere eSphere);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfUserInputValue GetPidfUserInputValue(IN const char* const pszUserInputState);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfUserInputValue(IN EPidfUserInputValue eUserInputState);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfServCapsDuplex(IN const char* const pszServCapsDuplex,
                               IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfServCapsDuplex(IN uint32_t uServCapsDuplex);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfServCapsClass(IN const char* const pszServCapsClass,
                              IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfServCapsClass(IN uint32_t uServCapsClass);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfServCapsEventPkg(IN const char* const pszServCapsEventPkg,
                                 IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfServCapsEventPkg(IN uint32_t uServCapsEventPkg);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfServCapsMethod(IN const char* const pszServCapsMethod,
                               IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfServCapsMethod(IN uint32_t uServCapsMethod);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfServCapsExtension(IN const char* const pszServCapsExtension,
                                  IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfServCapsExtension(IN uint32_t uServCapsExtension);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfServCapsActor(IN const char* const pszServCapsActor,
                              IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfServCapsActor(IN uint32_t uServCapsActor);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfServCapsPriority GetPidfServCapsPriority(IN const char* const pszServCapsPriority,
                                              IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfServCapsPriority(IN EPidfServCapsPriority eServCapsPriority);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfDevCapsMobility(IN const char* const pszDevCapsMobility,
                                IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfDevCapsMobility(IN uint32_t uDevCapsMobility);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
EPidfOmaActiveTerminated GetPidfOmaActiveTerminated(IN const char* const pszOmaActiveTerminated,
                                                    IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData& GetPidfOmaActiveTerminated(IN EPidfOmaActiveTerminated eOmaActiveTerminated);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the value corresponding to the C-style strings in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
uint32_t GetPidfOmaNetworkAvailability(IN const char* const pszOmaNetworkAvailability,
                                       IN const char* const pszNamespace);

// <GROUP TID_PACKAGE_SCEXML_FUNCTIONS>
//
// Summary:
//  Gets the SPidfData corresponding to the value passed in parameter.
//
// Location:
//   SceXml/ScePidfTypesConverter.h
const SPidfData* GetPidfOmaNetworkAvailability(IN uint32_t uOmaNetworkAvailability);


//M5T_INTERNAL_USE_BEGIN

//-- Internal helper methods.

// Summary:
//  Internal helper to get a linear enumeration value from a value in a
//  C-style string array.
int GetLinearEnumFromName(IN const char* const pszValueToSearch,
                          IN const char* const apszDataArray[],
                          IN unsigned int uSizeOfArray,
                          IN int nNotFoundValue);

// Summary:
//  Internal helper to get a linear enumeration value from a name value in a
//  SPidfData array.
int GetLinearEnumFromName(IN const char* const pszNameValueToSearch,
                          IN const SPidfData astPidfDataArray[],
                          IN unsigned int uSizeOfArray,
                          IN int nNotFoundValue);

// Summary:
//  Internal helper to get a linear enumeration value from a name value and a
//  namespace in a SPidfData array.
int GetLinearEnum(IN const char* const pszNameValueToSearch,
                  IN const char* const pszNamespace,
                  IN const SPidfData astPidfDataArray[],
                  IN unsigned int uSizeOfArray,
                  IN int nNotFoundValue);

// Summary:
//  Internal helper to get a bitmask uint32_t value from a name value and a
//  namespace in a SPidfData array.
uint32_t GetMaskEnum32(IN const char* const pszNameValueToSearch,
                       IN const char* const pszNamespace,
                       IN const SPidfData astPidfDataArray[],
                       IN uint32_t uLastBitmaskValue,
                       IN uint32_t uNotFoundValue);

// Summary:
//  Internal helper to get a bitmask uint64_t value from a name value and a
//  namespace in a SPidfData array.
uint64_t GetMaskEnum64(IN const char* const pszNameValueToSearch,
                       IN const char* const pszNamespace,
                       IN const SPidfData astPidfDataArray[],
                       IN const uint64_t& ruLastBitmaskValue,
                       IN const uint64_t& ruNotFoundValue);

// Summary:
//  Internal helper to get SPidfData from an array with a uint32_t bitmask
//  value.
const SPidfData* GetPidfDataFromMask32(IN uint32_t uBitmask,
                                       IN const SPidfData astPidfDataArray[],
                                       IN uint32_t uLastBitmaskValue);

// Summary:
//  Internal helper to get SPidfData from an array with a uint64_t bitmask
//  value.
const SPidfData* GetPidfDataFromMask64(IN const uint64_t& ruBitmask,
                                       IN const SPidfData astPidfDataArray[],
                                       IN const uint64_t& ruLastBitmaskValue);

//M5T_INTERNAL_USE_END


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetBooleanValue
//==
//==============================================================================
//
//  Parameters:
//      bValue:
//       The boolean value to get as a C-style string.
//
//  Returns:
//      - The C-style string corresponding to the boolean value.
//
//  Description:
//      Gets the C-style string corresponding to the specified boolean value.
//
//==============================================================================
inline const char* GetBooleanValue(IN bool bValue)
{
    return g_apszXMLBOOLEANVALUE[bValue?1:0];
}


//==============================================================================
//==
//==  GetPidfBasicStatus
//==
//==============================================================================
//
//  Parameters:
//      pszBasicStatus:
//       The basic status value as a C-style string.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The value corresponding to pszBasicStatus.
//      - ePIDFBASIC_OTHER_VALUE if no pre-defined value matches the parameters.
//
//  Description:
//      Returns the enum value corresponding to the value, described by 
//      pszBasicStatus, of the basic element of the status element.
//
//==============================================================================
inline EPidfBasicStatus GetPidfBasicStatus(IN const char* const pszBasicStatus)
{
    return static_cast<EPidfBasicStatus>(GetLinearEnumFromName(pszBasicStatus,
                                                               g_apszPIDFBASICSTATUS,
                                                               ePIDFBASIC_OTHER_VALUE,
                                                               ePIDFBASIC_OTHER_VALUE));
}


//==============================================================================
//==
//==  GetPidfBasicStatus
//==
//==============================================================================
//
//  Parameters:
//      eBasicStatus:
//       The basic status value to get.
//
//  Returns:
//      - The C-style string representing the specified value.
//
//  Description:
//      Gets the C-style string corresponding to the value of the basic element
//      in the status element.
//
//==============================================================================
inline const char* GetPidfBasicStatus(IN EPidfBasicStatus eBasicStatus)
{
    return g_apszPIDFBASICSTATUS[eBasicStatus];
}


//==============================================================================
//==
//==  GetPidfActivity
//==
//==============================================================================
//
//  Parameters:
//      pszActivity:
//       The name of the element, as a C-style string, in the activities
//       element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the activities element.
//
//==============================================================================
inline uint32_t GetPidfActivity(IN const char* const pszActivity,
                                IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszActivity,
                         pszNamespace,
                         g_stPIDFACTIVITIES,
                         uPIDFACT_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfActivity
//==
//==============================================================================
//
//  Parameters:
//      uActivity:
//       The activity element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uActivity is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the activity element.
//
//==============================================================================
inline const SPidfData* GetPidfActivity(IN uint32_t uActivity)
{
    return GetPidfDataFromMask32(uActivity,
                                 g_stPIDFACTIVITIES,
                                 uPIDFACT_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfMood
//==
//==============================================================================
//
//  Parameters:
//      pszMood:
//       The name of the element, as a C-style string, in the mood element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint64_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint64_t value corresponding to an element, described by the
//      C-style strings in parameter, in the mood element.
//
//==============================================================================
inline uint64_t GetPidfMood(IN const char* const pszMood,
                            IN const char* const pszNamespace)
{
    return GetMaskEnum64(pszMood,
                         pszNamespace,
                         g_stPIDFMOOD,
                         uPIDFMOOD_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfMood
//==
//==============================================================================
//
//  Parameters:
//      uMood:
//       The element in the mood element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uMood is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the mood.
//
//==============================================================================
inline const SPidfData* GetPidfMood(IN uint64_t uMood)
{
    return GetPidfDataFromMask64(uMood,
                                 g_stPIDFMOOD,
                                 uPIDFMOOD_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfPlaceIsAudio
//==
//==============================================================================
//
//  Parameters:
//      pszPlaceIsAudio:
//       The name of the element, as a C-style string, in the audio element of
//       the place-is element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFAUDIO_OTHER_NAME if no pre-defined name matches the parameters.
//
//  Description:
//      Gets the enum value corresponding to an element, described by the
//      C-style strings in parameter, in the audio element of a place-is
//      element.
//
//==============================================================================
inline EPidfPlaceIsAudio GetPidfPlaceIsAudio(IN const char* const pszPlaceIsAudio,
                                             IN const char* const pszNamespace)
{
    return static_cast<EPidfPlaceIsAudio>(GetLinearEnum(pszPlaceIsAudio,
                                                        pszNamespace,
                                                        g_stPIDFPLACEISAUDIO,
                                                        ePIDFAUDIO_OTHER_NAME,
                                                        ePIDFAUDIO_OTHER_NAME));
}


//==============================================================================
//==
//==  GetPidfPlaceIsAudio
//==
//==============================================================================
//
//  Parameters:
//      ePlaceIsAudio:
//       The name of the element in the audio element in the place-is element.
//
//  Returns:
//      - The SPidfData for the specified element.
//
//  Description:
//      Gets the SPidfData corresponding to the name of the element if the audio
//      element in a place-is element.
//
//==============================================================================
inline const SPidfData& GetPidfPlaceIsAudio(IN EPidfPlaceIsAudio ePlaceIsAudio)
{
    return g_stPIDFPLACEISAUDIO[ePlaceIsAudio];
}


//==============================================================================
//==
//==  GetPidfPlaceIsVideo
//==
//==============================================================================
//
//  Parameters:
//      pszPlaceIsVideo:
//       The name of the element, as a C-style string, in the video element of
//       the place-is element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFVIDEO_OTHER_NAME if no pre-defined name matches the parameters.
//
//  Description:
//      Gets the enum value corresponding to an element, described by the
//      C-style strings in parameter, in the video element of a place-is
//      element.
//
//==============================================================================
inline EPidfPlaceIsVideo GetPidfPlaceIsVideo(IN const char* const pszPlaceIsVideo,
                                             IN const char* const pszNamespace)
{
    return static_cast<EPidfPlaceIsVideo>(GetLinearEnum(pszPlaceIsVideo,
                                                        pszNamespace,
                                                        g_stPIDFPLACEISVIDEO,
                                                        ePIDFVIDEO_OTHER_NAME,
                                                        ePIDFVIDEO_OTHER_NAME));
}


//==============================================================================
//==
//==  GetPidfPlaceIsVideo
//==
//==============================================================================
//
//  Parameters:
//      ePlaceIsVideo:
//       The name of the element in the video element in the place-is element.
//
//  Returns:
//      - The SPidfData for the specified element.
//
//  Description:
//      Gets the SPidfData corresponding to the name of the element in the video
//      element in a place-is element.
//
//==============================================================================
inline const SPidfData& GetPidfPlaceIsVideo(IN EPidfPlaceIsVideo ePlaceIsVideo)
{
    return g_stPIDFPLACEISVIDEO[ePlaceIsVideo];
}


//==============================================================================
//==
//==  GetPidfPlaceIsText
//==
//==============================================================================
//
//  Parameters:
//      pszPlaceIsText:
//       The name of the element, as a C-style string, in the text element of
//       the place-is element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFTEXT_OTHER_NAME if no pre-defined name matches the parameters.
//
//  Description:
//      Gets the enum value corresponding to an element, described by the
//      C-style strings in parameter, in the text element of the place-is
//      element.
//
//==============================================================================
inline EPidfPlaceIsText GetPidfPlaceIsText(IN const char* const pszPlaceIsText,
                                           IN const char* const pszNamespace)
{
    return static_cast<EPidfPlaceIsText>(GetLinearEnum(pszPlaceIsText,
                                                       pszNamespace,
                                                       g_stPIDFPLACEISTEXT,
                                                       ePIDFTEXT_OTHER_NAME,
                                                       ePIDFTEXT_OTHER_NAME));
}


//==============================================================================
//==
//==  GetPidfPlaceIsText
//==
//==============================================================================
//
//  Parameters:
//      ePlaceIsText:
//       The name of the element in the text element in the place-is element.
//
//  Returns:
//      - The SPidfData for the specified element.
//
//  Description:
//      Gets the SPidfData corresponding to the name of the element in the text
//      element in a place-is element.
//
//==============================================================================
inline const SPidfData& GetPidfPlaceIsText(IN EPidfPlaceIsText ePlaceIsText)
{
    return g_stPIDFPLACEISTEXT[ePlaceIsText];
}


//==============================================================================
//==
//==  GetPidfPlaceType
//==
//==============================================================================
//
//  Parameters:
//      pszPlaceType:
//       The name of the element, as a C-style string, in the place-type
//       element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint64_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint64_t value corresponding to an element, described by the
//      C-style strings in parameter, in the place-type element.
//
//==============================================================================
inline uint64_t GetPidfPlaceType(IN const char* const pszPlaceType,
                                 IN const char* const pszNamespace)
{
    return GetMaskEnum64(pszPlaceType,
                         pszNamespace,
                         g_stPIDFPLACETYPE,
                         uPIDFPLACETYPE_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfPlaceType
//==
//==============================================================================
//
//  Parameters:
//      uPlaceType:
//       The element in the place-type element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uPlaceType is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the place-type
//      element.
//
//==============================================================================
inline const SPidfData* GetPidfPlaceType(IN uint64_t uPlaceType)
{
    return GetPidfDataFromMask64(uPlaceType,
                                 g_stPIDFPLACETYPE,
                                 uPIDFPLACETYPE_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfPrivacy
//==
//==============================================================================
//
//  Parameters:
//      pszPrivacy:
//       The name of the element in the privacy element as a C-style string.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in a privacy element.
//
//==============================================================================
inline uint32_t GetPidfPrivacy(IN const char* const pszPrivacy,
                               IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszPrivacy,
                         pszNamespace,
                         g_stPIDFPRIVACY,
                         uPIDFPRIVACY_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfPrivacy
//==
//==============================================================================
//
//  Parameters:
//      uPrivacy:
//       The element in the privacy element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uPrivacy is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the privacy element.
//
//==============================================================================
inline const SPidfData* GetPidfPrivacy(IN uint32_t uPrivacy)
{
    return GetPidfDataFromMask32(uPrivacy,
                                 g_stPIDFPRIVACY,
                                 uPIDFPRIVACY_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfRelationship
//==
//==============================================================================
//
//  Parameters:
//      pszRelationship:
//       The name of the element, as a C-style string, in the relationship
//       element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFRELATIONSHIP_OTHER_NAME if no pre-defined name matches the
//        parameters.
//
//  Description:
//      Gets the enum value corresponding to an element, described by the
//      C-style strings in parameter, in a relationship element.
//
//==============================================================================
inline EPidfRelationship GetPidfRelationship(IN const char* const pszRelationship,
                                             IN const char* const pszNamespace)
{
    return static_cast<EPidfRelationship>(GetLinearEnum(pszRelationship,
                                                        pszNamespace,
                                                        g_stPIDFRELATIONSHIP,
                                                        ePIDFRELATIONSHIP_OTHER_NAME,
                                                        ePIDFRELATIONSHIP_OTHER_NAME));
}


//==============================================================================
//==
//==  GetPidfRelationship
//==
//==============================================================================
//
//  Parameters:
//      eRelationship:
//       The name of the element in the relationship element.
//
//  Returns:
//      - The SPidfData for the specified element.
//
//  Description:
//      Gets the SPidfData corresponding to the name of the element in the
//      relationship element.
//
//==============================================================================
inline const SPidfData& GetPidfRelationship(IN EPidfRelationship eRelationship)
{
    return g_stPIDFRELATIONSHIP[eRelationship];
}


//==============================================================================
//==
//==  GetPidfServiceClass
//==
//==============================================================================
//
//  Parameters:
//      pszServiceClass:
//       The name of the element, as a C-style string, in the service-class
//       element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFSERVICECLASS_OTHER_NAME if no pre-defined name matches the
//        parameters.
//
//  Description:
//      Gets the enum value corresponding to an element, described by the
//      C-style strings in parameter, in the service-class element.
//
//==============================================================================
inline EPidfServiceClass GetPidfServiceClass(IN const char* const pszServiceClass,
                                             IN const char* const pszNamespace)
{
    return static_cast<EPidfServiceClass>(GetLinearEnum(pszServiceClass,
                                                        pszNamespace,
                                                        g_stPIDFSERVICECLASS,
                                                        ePIDFSERVICECLASS_OTHER_NAME,
                                                        ePIDFSERVICECLASS_OTHER_NAME));
}


//==============================================================================
//==
//==  GetPidfServiceClass
//==
//==============================================================================
//
//  Parameters:
//      eServiceClass:
//       The name of the element in the service-class element.
//
//  Returns:
//      - The SPidfData for the specified element.
//
//  Description:
//      Gets the SPidfData corresponding to the name of the element in the
//      service-class element.
//
//==============================================================================
inline const SPidfData& GetPidfServiceClass(IN EPidfServiceClass eServiceClass)
{
    return g_stPIDFSERVICECLASS[eServiceClass];
}


//==============================================================================
//==
//==  GetPidfSphere
//==
//==============================================================================
//
//  Parameters:
//      pszSphere:
//       The name of the element, as a C-style string, in the sphere element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFSPHERE_OTHER_NAME if no pre-defined name matches the parameters.
//
//  Description:
//      Gets the enum value corresponding an element, described by the
//      C-style strings in parameter, in the sphere element.
//
//==============================================================================
inline EPidfSphere GetPidfSphere(IN const char* const pszSphere,
                                 IN const char* const pszNamespace)
{
    return static_cast<EPidfSphere>(GetLinearEnum(pszSphere,
                                                  pszNamespace,
                                                  g_stPIDFSPHERE,
                                                  ePIDFSPHERE_OTHER_NAME,
                                                  ePIDFSPHERE_OTHER_NAME));
}


//==============================================================================
//==
//==  GetPidfSphere
//==
//==============================================================================
//
//  Parameters:
//      eSphere:
//       The name of the element in the sphere element.
//
//  Returns:
//      - The SPidfData for the specified element.
//
//  Description:
//      Gets the SPidfData corresponding to the name of the element in the
//      sphere element.
//
//==============================================================================
inline const SPidfData& GetPidfSphere(IN EPidfSphere eSphere)
{
    return g_stPIDFSPHERE[eSphere];
}


//==============================================================================
//==
//==  GetPidfUserInputValue
//==
//==============================================================================
//
//  Parameters:
//      pszUserInputState:
//       The value of the user-input element as a C-style string.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFUSERINPUT_OTHER_NAME if no pre-defined name matches the
//        parameters.
//
//  Description:
//      Gets the enum value corresponding an user-input element value described
//      by the C-style strings in parameter.
//
//==============================================================================
inline EPidfUserInputValue GetPidfUserInputValue(IN const char* const pszUserInputState)
{
    return static_cast<EPidfUserInputValue>(GetLinearEnum(pszUserInputState,
                                                          "",
                                                          g_stPIDFUSERINPUT,
                                                          ePIDFUSERINPUT_OTHER_NAME,
                                                          ePIDFUSERINPUT_OTHER_NAME));
}


//==============================================================================
//==
//==  GetPidfUserInputValue
//==
//==============================================================================
//
//  Parameters:
//      eUserInputState:
//       The value of the user-input element.
//
//  Returns:
//      - The SPidfData for the specified element.
//
//  Description:
//      Gets the SPidfData corresponding to the value of the user-input element.
//
//==============================================================================
inline const SPidfData& GetPidfUserInputValue(IN EPidfUserInputValue eUserInputState)
{
    return g_stPIDFUSERINPUT[eUserInputState];
}


//==============================================================================
//==
//==  GetPidfServCapsDuplex
//==
//==============================================================================
//
//  Parameters:
//      pszServCapsDuplex:
//       The name of the element, as a C-style string, in the duplex element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the duplex element.
//
//==============================================================================
inline uint32_t GetPidfServCapsDuplex(IN const char* const pszServCapsDuplex,
                                      IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszServCapsDuplex,
                         pszNamespace,
                         g_stPIDFSERVCAPSDUPLEX,
                         uPIDFSERVCAPSDUPLEX_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfServCapsDuplex
//==
//==============================================================================
//
//  Parameters:
//      uServCapsDuplex:
//       The element in the duplex element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uServCapsDuplex is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the duplex element.
//
//==============================================================================
inline const SPidfData* GetPidfServCapsDuplex(IN uint32_t uServCapsDuplex)
{
    return GetPidfDataFromMask32(uServCapsDuplex,
                                 g_stPIDFSERVCAPSDUPLEX,
                                 uPIDFSERVCAPSDUPLEX_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfServCapsClass
//==
//==============================================================================
//
//  Parameters:
//      pszServCapsDuplex:
//       The name of the element, as a C-style string, in the supported or
//       notsupported element of a class element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the supported or notsupported element
//      of a class element.
//
//==============================================================================
inline uint32_t GetPidfServCapsClass(IN const char* const pszServCapsClass,
                                     IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszServCapsClass,
                         pszNamespace,
                         g_stPIDFSERVCAPSCLASS,
                         uPIDFSERVCAPSCLASS_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfServCapsClass
//==
//==============================================================================
//
//  Parameters:
//      uServCapsClass:
//       The element in the in the supported or notsupported element of a class
//       element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uServCapsClass is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the supported or
//      notsupported element of a class element.
//
//==============================================================================
inline const SPidfData* GetPidfServCapsClass(IN uint32_t uServCapsClass)
{
    return GetPidfDataFromMask32(uServCapsClass,
                                 g_stPIDFSERVCAPSCLASS,
                                 uPIDFSERVCAPSCLASS_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfServCapsEventPkg
//==
//==============================================================================
//
//  Parameters:
//      pszServCapsEventPkg:
//       The name of the element, as a C-style string, in the supported or
//       notsupported element of an event-packagestype element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the supported or notsupported element
//      of an event-packagestype element.
//
//==============================================================================
inline uint32_t GetPidfServCapsEventPkg(IN const char* const pszServCapsEventPkg,
                                        IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszServCapsEventPkg,
                         pszNamespace,
                         g_stPIDFSERVCAPSEVENTPKG,
                         uPIDFSERVCAPSEVENTPKG_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfServCapsEventPkg
//==
//==============================================================================
//
//  Parameters:
//      uServCapsEventPkg:
//       The element in the in the supported or notsupported element of an
//       event-packagestype element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uServCapsEventPkg is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the supported or
//      notsupported element of an event-packagestype element.
//
//==============================================================================
inline const SPidfData* GetPidfServCapsEventPkg(IN uint32_t uServCapsEventPkg)
{
    return GetPidfDataFromMask32(uServCapsEventPkg,
                                 g_stPIDFSERVCAPSEVENTPKG,
                                 uPIDFSERVCAPSEVENTPKG_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfServCapsMethod
//==
//==============================================================================
//
//  Parameters:
//      pszServCapsMethod:
//       The name of the element, as a C-style string, in the supported or
//       notsupported element of a methods element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the supported or notsupported element
//      of a methods element.
//
//==============================================================================
inline uint32_t GetPidfServCapsMethod(IN const char* const pszServCapsMethod,
                                      IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszServCapsMethod,
                         pszNamespace,
                         g_stPIDFSERVCAPSMETHODS,
                         uPIDFSERVCAPSMETHOD_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfServCapsMethod
//==
//==============================================================================
//
//  Parameters:
//      uServCapsMethod:
//       The element in the in the supported or notsupported element of a
//       methods element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uServCapsMethod is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the supported or
//      notsupported element of a methods element.
//
//==============================================================================
inline const SPidfData* GetPidfServCapsMethod(IN uint32_t uServCapsMethod)
{
    return GetPidfDataFromMask32(uServCapsMethod,
                                 g_stPIDFSERVCAPSMETHODS,
                                 uPIDFSERVCAPSMETHOD_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfServCapsExtension
//==
//==============================================================================
//
//  Parameters:
//      pszServCapsExtension:
//       The name of the element, as a C-style string, in the supported or
//       notsupported element of an extensions element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the supported or notsupported element
//      of an extentions element.
//
//==============================================================================
inline uint32_t GetPidfServCapsExtension(IN const char* const pszServCapsExtension,
                                         IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszServCapsExtension,
                         pszNamespace,
                         g_stPIDFSERVCAPSEXT,
                         uPIDFSERVCAPSEXT_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfServCapsExtension
//==
//==============================================================================
//
//  Parameters:
//      uServCapsExtension:
//       The element in the in the supported or notsupported element of an
//       extensions element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uServCapsExtension is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the supported or
//      notsupported element of an extensions element.
//
//==============================================================================
inline const SPidfData* GetPidfServCapsExtension(IN uint32_t uServCapsExtension)

{
    return GetPidfDataFromMask32(uServCapsExtension,
                                 g_stPIDFSERVCAPSEXT,
                                 uPIDFSERVCAPSEXT_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfServCapsActor
//==
//==============================================================================
//
//  Parameters:
//      pszServCapsActor:
//       The name of the element, as a C-style string, in the supported or
//       notsupported element of an actor element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the supported or notsupported element
//      of an actor element.
//
//==============================================================================
inline uint32_t GetPidfServCapsActor(IN const char* const pszServCapsActor,
                                     IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszServCapsActor,
                         pszNamespace,
                         g_stPIDFSERVCAPSACTOR,
                         uPIDFSERVCAPSACTOR_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfServCapsActor
//==
//==============================================================================
//
//  Parameters:
//      uServCapsActor:
//       The element in the in the supported or notsupported element of an
//       actor element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uServCapsActor is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the supported or
//      notsupported element of an actor element.
//
//==============================================================================
inline const SPidfData* GetPidfServCapsActor(IN uint32_t uServCapsActor)

{
    return GetPidfDataFromMask32(uServCapsActor,
                                 g_stPIDFSERVCAPSACTOR,
                                 uPIDFSERVCAPSACTOR_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfServCapsPriority
//==
//==============================================================================
//
//  Parameters:
//      pszServCapsPriority:
//       The name of the element, as a C-style string, in the supported or
//       notsupported element of a priority element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFSERVCAPSPRIO_OTHER_NAME if no pre-defined name matches the
//        parameters.
//
//  Description:
//      Returns the enum value corresponding to an element, described by the
//      C-style strings in parameter, in the supported or notsupported element
//      of a priority element.
//
//==============================================================================
inline EPidfServCapsPriority GetPidfServCapsPriority(IN const char* const pszServCapsPriority,
                                                     IN const char* const pszNamespace)
{
    return static_cast<EPidfServCapsPriority>(GetLinearEnum(pszServCapsPriority,
                                                            pszNamespace,
                                                            g_stPIDFSERVCAPSPRIORITY,
                                                            ePIDFSERVCAPSPRIO_OTHER_NAME,
                                                            ePIDFSERVCAPSPRIO_OTHER_NAME));
}


//==============================================================================
//==
//==  GetPidfServCapsPriority
//==
//==============================================================================
//
//  Parameters:
//      eServCapsPriority:
//       The element in the in the supported or notsupported element of a
//       priority element.
//
//  Returns:
//      - The SPidfData for the specified element.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the supported or
//      notsupported element of a priority element.
//
//==============================================================================
inline const SPidfData& GetPidfServCapsPriority(IN EPidfServCapsPriority eServCapsPriority)
{
    return g_stPIDFSERVCAPSPRIORITY[eServCapsPriority];
}


//==============================================================================
//==
//==  GetPidfDevCapsMobility
//==
//==============================================================================
//
//  Parameters:
//      pszDevCapsMobility:
//       The name of the element, as a C-style string, in the supported or
//       notsupported element of a mobility element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the supported or notsupported element
//      of a mobility element.
//
//==============================================================================
inline uint32_t GetPidfDevCapsMobility(IN const char* const pszDevCapsMobility,
                                       IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszDevCapsMobility,
                         pszNamespace,
                         g_stPIDFDEVCAPSMOBILITY,
                         uPIDFDEVCAPSMOBILITY_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfDevCapsMobility
//==
//==============================================================================
//
//  Parameters:
//      uDevCapsMobility:
//       The element in the in the supported or notsupported element of a
//       mobility element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uDevCapsMobility is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the supported or
//      notsupported element of a mobility element.
//
//==============================================================================
inline const SPidfData* GetPidfDevCapsMobility(IN uint32_t uDevCapsMobility)

{
    return GetPidfDataFromMask32(uDevCapsMobility,
                                 g_stPIDFDEVCAPSMOBILITY,
                                 uPIDFDEVCAPSMOBILITY_LAST_VALUE);
}


//==============================================================================
//==
//==  GetPidfOmaActiveTerminated
//==
//==============================================================================
//
//  Parameters:
//      pszOmaActiveTerminated:
//       The value of an OMA element, as a C-style string.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the value is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The enum value corresponding to the parameters.
//      - ePIDFOMAACTIVETERMINATED_OTHER_VALUE if no pre-defined name matches
//        the parameters.
//
//  Description:
//      Returns the enum value corresponding to the value, described by the
//      C-style strings in parameter, of an element.
//
//==============================================================================
inline EPidfOmaActiveTerminated GetPidfOmaActiveTerminated(IN const char* const pszOmaActiveTerminated,
                                                           IN const char* const pszNamespace)
{
    return static_cast<EPidfOmaActiveTerminated>(GetLinearEnum(pszOmaActiveTerminated,
                                                               pszNamespace,
                                                               g_stPIDFOMAACTIVETERMINATED,
                                                               ePIDFOMAACTIVETERMINATED_OTHER_VALUE,
                                                               ePIDFOMAACTIVETERMINATED_OTHER_VALUE));
}


//==============================================================================
//==
//==  GetPidfOmaActiveTerminated
//==
//==============================================================================
//
//  Parameters:
//      eOmaActiveTerminated:
//       The value to get the SPidfData from.
//
//  Returns:
//      - The SPidfData for the specified value.
//
//  Description:
//      Gets the SPidfData corresponding to the specified value.
//
//==============================================================================
inline const SPidfData& GetPidfOmaActiveTerminated(IN EPidfOmaActiveTerminated eOmaActiveTerminated)
{
    return g_stPIDFOMAACTIVETERMINATED[eOmaActiveTerminated];
}


//==============================================================================
//==
//==  GetPidfOmaNetworkAvailability
//==
//==============================================================================
//
//  Parameters:
//      pszOmaNetworkAvailability:
//       The name of the element, as a C-style string, in the network element of
//       a network-availability element.
//
//       MUST NOT be NULL.
//
//      pszNamespace:
//       The namespace where the element is defined.
//
//       MUST NOT be NULL.
//
//  Returns:
//      - The uint32_t value corresponding to the parameters.
//      - 0 if no pre-defined name matches the parameters.
//
//  Description:
//      Returns the uint32_t value corresponding to an element, described by the
//      C-style strings in parameter, in the network element of a
//      network-availability element.
//
//==============================================================================
inline uint32_t GetPidfOmaNetworkAvailability(IN const char* const pszOmaNetworkAvailability,
                                              IN const char* const pszNamespace)
{
    return GetMaskEnum32(pszOmaNetworkAvailability,
                         pszNamespace,
                         g_stPIDFOMANETWORKAVAILABILITY,
                         uPIDFOMANETWORKAVAILABILITY_LAST_VALUE,
                         0);
}


//==============================================================================
//==
//==  GetPidfOmaNetworkAvailability
//==
//==============================================================================
//
//  Parameters:
//      uOmaNetworkAvailability:
//       The element in the network element of a network-availability element.
//
//  Returns:
//      - A pointer to the SPidfData for the specified element.
//      - NULL if uOmaNetworkAvailability is invalid.
//
//  Description:
//      Gets the SPidfData corresponding to the element in the network element
//      of a network-availability element.
//
//==============================================================================
inline const SPidfData* GetPidfOmaNetworkAvailability(IN uint32_t uOmaNetworkAvailability)
{
    return GetPidfDataFromMask32(uOmaNetworkAvailability,
                                 g_stPIDFOMANETWORKAVAILABILITY,
                                 uPIDFOMANETWORKAVAILABILITY_LAST_VALUE);
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SCEPIDFTYPESCONVERTER_H

