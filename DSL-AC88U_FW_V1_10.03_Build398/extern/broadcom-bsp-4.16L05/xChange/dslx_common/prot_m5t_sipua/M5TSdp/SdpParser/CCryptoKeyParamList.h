//==============================================================================
//==============================================================================
//
//   Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CCRYPTOKEYPARAMLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCRYPTOKEYPARAMLIST_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


//-- Interface Realized and/or Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif


MX_NAMESPACE_START(MXD_GNS)



//-- Forward Declarations
class CBlob;
class CCryptoKeyParam;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CCryptoKeyParamList
//=========================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//      This class implements a list of key parameters.
//
// Description:
//      This class implements a list of key parameters, separated by a specified
//      character (;).
//
//  <CODE>
//      draft-ietf-mmusic-sdescriptions-09 ABNF
//         key-params       = key-param *(";" key-param)
//  </CODE>
//
// Location:
//   SdpParser/CCryptoKeyParamList.h
//
// See Also:
//   CCryptoKeyParam
//
//==============================================================================
class CCryptoKeyParamList : public CSdpParser
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>
    // Summary: Default constructor.
    CCryptoKeyParamList();

    // Summary: Copy constructor.
    CCryptoKeyParamList(IN const CCryptoKeyParamList& rSrc);

    // Summary: Destructor.
    virtual ~CCryptoKeyParamList();

    //  Summary: Assignment operator.
    CCryptoKeyParamList& operator=(IN const CCryptoKeyParamList& rFrom);

    // Summary: Comparison operator.
    bool operator==(IN const CCryptoKeyParamList& rFrom) const;

    // Summary:
    //  Returns the parameter at uIndex.
    CCryptoKeyParam* operator[](IN uint32_t uIndex);
    // <COMBINE CCryptoKeyParamList::[]>
    const CCryptoKeyParam* operator[](IN uint32_t uIndex) const;

    //-- << Public methods >>

    // Summary:
    //  Adds a parameter at the end of the list. Does not check if the
    //  parameter name is already in the list.
    uint32_t Append(IN TO CCryptoKeyParam* pParam);

    // Summary:
    //  Returns true if the list contains no parameters.
    bool IsEmpty() const;

    // Summary:
    //  Returns the number of parameters into the list.
    uint32_t Length() const;

    // Summary:
    //  Parses the parameters list beginning at rpszStartPosition.
    EParserResult Parse( INOUT const char*& rpszStartPosition, OUT mxt_result& rres);

    // Summary:
    //  Inserts cSeparator after each parameter, except for last parameter.
    void Serialize(INOUT CBlob& rBlob, IN char cSeparator = ';') const;

    // Summary:
    //  Resets this object.
    void Reset();

    // Summary:
    //  Returns true if data members are valid
    bool Validate();

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:


//-- Hidden Data Members
protected:

    // Contains the list of parameter pointers. The parameters are serialized
    // in sequential order.
    CVector<CCryptoKeyParam*> m_vpParam;

private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True if the param list contains no parameters.
//
//  Description:
//      Returns true if the param list contains no parameters.
//
//==============================================================================
inline bool CCryptoKeyParamList::IsEmpty() const
{
    return m_vpParam.IsEmpty();
}

//==============================================================================
//==
//==  Length
//==
//==============================================================================
//
//  Returns:
//      The number of parameters in the list.
//
//  Description:
//      Returns the number of parameters in the list.
//
//==============================================================================
inline uint32_t CCryptoKeyParamList::Length() const
{
    return m_vpParam.GetSize();
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CCRYPTOKEYPARAMLIST_H
