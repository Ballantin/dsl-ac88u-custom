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
#ifndef MXG_CSCESIPCALLERPREFSSORTER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCESIPCALLERPREFSSORTER_H

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

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSceSipFeatureSet;
class CGenParamList;
class CSipHeader;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceSipCallerPrefsSorter
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Description:
//  The CSceSipCallerPrefsSorter implements the caller preferences sorting
//  algorithm defined in RFC 3841 section 7.2. Thus this class properly manages
//  the Reject-Contact and Accept-Contact headers, along with the UA
//  capabilities found in Contact headers, but does not handle the
//  Request-Disposition header.
//
//  It takes in input any number of Contact headers (through AddContacts),
//  the caller preferences (through SetCallerPrefsHeaders) and yields a list of
//  contacts, sorted according to the caller preferences mechanism.
//
//  SortContacts is used to execute the sorting process, while GetSortedContacts
//  provides access to the sorted list of contacts.
//
// Location:
//   SceCoreComponents/CSceSipCallerPrefsSorter.h
//
// See Also:
//   CSceSipFeatureSet
//
//==============================================================================
class CSceSipCallerPrefsSorter
{
//-- Friend Declarations


//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceSipCallerPrefsSorter();

    // Summary:
    //  Destructor.
    virtual ~CSceSipCallerPrefsSorter();

    // Summary:
    //  Adds one or more contact to be sorted.
    mxt_result AddContacts(IN TO CSipHeader* pContacts);

    // Summary:
    //  Configures the object with the caller preferences found in the packet.
    mxt_result SetCallerPrefsHeaders(IN const CSipPacket& rPacket);

    // Summary:
    //  Sorts the contacts according to the caller preferences and the
    //  capabilities found in the contacts.
    mxt_result SortContacts();

    // Summary:
    //  Gets the sorted contacts.
    mxt_result GetSortedContacts(OUT GO CVector<CSipHeader*>& rvecpContact);


//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceSipCallerPrefsSorter(IN const CSceSipCallerPrefsSorter& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceSipCallerPrefsSorter& operator=(IN const CSceSipCallerPrefsSorter& rSrc);

    //-- In order to compile under MSVC6, the EmptyVector method can unfortunately not be
    //-- templated.

    // This helper empties a CVector< const CSipHeader* > without leaks.
    void EmptyVector(INOUT CVector<const CSipHeader*>& rvecpToEmpty);

    // This helper empties a CVector< const CSceSipFeatureSet* > without leaks.
    void EmptyVector(INOUT CVector<const CSceSipFeatureSet*>& rvecpToEmpty);

    // This helper creates a new CSceSipFeatureSet and returns it along with the
    // ownership of the pointer.
    CSceSipFeatureSet* GO CreateFeatureSet(IN const CGenParamList& rParamList);

    // This structure is used to keep all the information while doing
    // caller preference sorting processing.
    struct SContactInformation
    {
        const CSipHeader*                 m_pHdrContact;
        CSceSipFeatureSet*                m_pContactFeatureSet;
        CVector<const CSceSipFeatureSet*> m_vecpAcceptContactFeatureSet;
        CVector<unsigned int>             m_vecuAcceptContactMatchingScores;
        unsigned int                      m_uCallerPreferenceScore;
        unsigned int                      m_uQValue;
    };

    // This helper is used to sort the target set by q value and preference score.
    static int CompareSContactInformation(IN SContactInformation* const & rpstFirstElement,
                                          IN SContactInformation* const & rpstSecondElement,
                                          IN mxt_opaque opq);

//-- Hidden Data Members
protected:
private:

    // These are the contacts we have received through AddContacts. The
    // contacts were unchained. We have the ownership of these contacts.
    CVector<const CSipHeader*>    m_vecpContactsInput;

    // These are the feature sets created from Reject-Contact headers.
    CVector<const CSceSipFeatureSet*> m_vecpRejectFeatureSets;

    // These are the feature sets created from Accept-Contact headers.
    CVector<const CSceSipFeatureSet*> m_vecpAcceptFeatureSets;

    // This is the implicit feature set created when no Accept-Contact or Reject-Contact
    // headers are found.
    CSceSipFeatureSet* m_pImplicitAcceptFeatureSet;

    // These are the contacts once they are sorted. Note that some contacts
    // that were present in m_vContactInput may have been discarded and not be
    // included in m_vContactSorted.
    CVector<const CSipHeader*>     m_vecpContactsSorted;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCESIPCALLERPREFSSORTER_H

