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
#ifndef MXG_CSCECALLERPREFERENCES_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCECALLERPREFERENCES_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_SIPREQUESTDISPOSITIONDIRECTIVE_H
#include "SceSipTypes/SipRequestDispositionDirective.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSceSipFeatureSet;
class CSipHeader;
class CSipPacket;

//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceCallerPreferences
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This class represents SIP caller preferences.
//
// Description:
//  This class represents SIP caller preferences. Caller preferences can be
//  expressed when sending a request, in which case servers supporting this
//  extension will use the preferences to properly route the request to the best
//  matching target.
//
//  Caller preferences can also be used when receiving a request in order to
//  know how to treat this request. As an example, caller preferences can
//  indicate the language that the caller wants to use, and this can be
//  indicated to the endpoint receiving the call. Another example is the type of
//  media that the caller wants to establish, which can be used as a hint to
//  know which media sessions to set-up.
//
// Location:
//   SceCoreComponents/CSceCallerPreferences.h
//
// See Also:
//   CSceSipFeatureSet
//
//==============================================================================
class CSceCallerPreferences
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceCallerPreferences();

    // Summary:
    //  Destructor.
    virtual ~CSceCallerPreferences();

    // Summary:
    //  Copy Constructor.
    CSceCallerPreferences(IN const CSceCallerPreferences& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceCallerPreferences& operator=(IN const CSceCallerPreferences& rSrc);

    // Summary:
    //  Adds an Accept-Contact header.
    mxt_result AddAcceptContact(IN TO const CSceSipFeatureSet* pFeatureSet);

    // Summary:
    //  Adds a Reject-Contact header.
    mxt_result AddRejectContact(IN TO const CSceSipFeatureSet* pFeatureSet);

    // Summary:
    //  Provides access to the Accept-Contact headers.
    const CVector<const CSceSipFeatureSet*>& GetAcceptContact() const;

    // Summary:
    //  Provides access to the Reject-Contact headers.
    const CVector<const CSceSipFeatureSet*>& GetRejectContact() const;

    // Summary:
    //  Sets value associated with the Request-Disposition header.
    void SetRequestDisposition(IN uint32_t uBitSetDisposition);

    // Summary:
    //  Gets the value of the Request-Disposition header.
    uint32_t GetRequestDisposition() const;

    // Summary:
    //  Indicates if a given Directive is set in a bitset.
    bool IsSetInBitSet(IN ERequestDispositionDirective eDirective,
                       IN uint32_t uBitSet = static_cast<uint32_t>(-1));

    // Summary:
    //  Imports the caller preferences, if any, from the packet.
    mxt_result ImportPreferences(IN const CSipPacket& rPacket);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:

    // Summary:
    //  Copies the rvecpSourcesFeatureSets contents into rvecpDestinationFeatureSets
    //  with regards to pointer ownership issues.
    void CopyVector(IN const CVector<const CSceSipFeatureSet*>& rvecpSourcesFeatureSets,
                    OUT CVector<const CSceSipFeatureSet*>& rvecpDestinationFeatureSets);

    // Summary:
    //  Empties the provided vector without memory leaks.
    void EmptyVector(INOUT CVector<const CSceSipFeatureSet*>& rvecpFeatureSets);

    // Summary:
    //  Imports Accept or Reject contacts.
    mxt_result ImportAcceptRejectContactsHelper(IN const CSipHeader& rHeader); 

//-- Hidden Data Members
protected:
private:

    // The request disposition bitset.
    uint32_t m_uRequestDispositionBitSet;

    // The vector of Accept-Contact headers's feature set.
    CVector<const CSceSipFeatureSet*> m_vecpAcceptContactSets;

    // The vector of Reject-Contact headers's feature set.
    CVector<const CSceSipFeatureSet*> m_vecpRejectContactSets;

//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetAcceptContact
//==
//==============================================================================
//
// Returns:
//     The Accept-Contact feature sets.
//
// Description:
//     This method returns the feature sets that must be included in a request
//     as an Accept-Contact header.
//
// Warning:
//     This method returns a reference to the internal container. The caller
//     MUST NOT delete the content of that vector since the
//     CSceCallerPreferences object keeps the content's ownership.
//
// See also:
//     AddAcceptContact
//
//==============================================================================
inline const CVector<const CSceSipFeatureSet*>& CSceCallerPreferences::GetAcceptContact() const
{
    return m_vecpAcceptContactSets;
}

//==============================================================================
//==
//==  GetRejectContact
//==
//==============================================================================
//
// Returns:
//     The Reject-Contact feature sets.
//
// Description:
//     This method returns the feature sets that must be included in a request
//     as a Reject-Contact header.
//
// Warning:
//     This method returns a reference to the internal container. The caller
//     MUST NOT delete the content of that vector since the
//     CSceCallerPreferences object keeps the content's ownership.
//
// See also:
//     AddRejectContact
//
//==============================================================================
inline const CVector<const CSceSipFeatureSet*>& CSceCallerPreferences::GetRejectContact() const
{
    return m_vecpRejectContactSets;
}

//==============================================================================
//==
//==  GetRequestDisposition
//==
//==============================================================================
//
// Returns:
//     The current value of the request disposition bit set.
//
// Description:
//     This method returns the current value of the request disposition bitset.
//
// See also:
//     ERequestDispositionDirective
//
//==============================================================================
inline uint32_t CSceCallerPreferences::GetRequestDisposition() const
{
    // No traces because it is inline.
    return m_uRequestDispositionBitSet;
}



MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSCECALLERPREFERENCES_H

