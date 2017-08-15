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
#ifndef MXG_CCPIMHEADER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCPIMHEADER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CTXTHEADER_H
#include "TextMessage/CTxtHeader.h"
#endif

#ifndef MXG_CPIMHEADERGRAMMAR_H
#include "SceUaImppCpim/CpimHeaderGrammar.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CCpimHeader
//========================================
//<GROUP TID_PACKAGE_UAIMPP_CLASSES>
//
// Summary:
//  This class represents a CPIM header.
//
// Description:
//  This class represents a CPIM header. It supports a set of CPIM headers, with
//  each header type mapped to a value defined in ECpimHeaderType. The following
//  CPIM headers are supported:
//      - From, mapped to eHDR_CPIM_FROM;
//      - To, mapped to eHDR_CPIM_TO;
//      - cc, mapped to eHDR_CPIM_CC;
//      - DateTime, mapped to eHDR_CPIM_DATE_TIME;
//      - Subject, mapped to eHDR_CPIM_SUBJECT;
//      - NS, mapped to eHDR_CPIM_NS;
//      - Require, mapped to eHDR_CPIM_REQUIRE;
//
// Location:
//   CpimMessage/CCpimHeader.h
//
//==============================================================================
class CCpimHeader : public CTxtHeader
{
//-- Friend Declarations.

//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CCpimHeader(IN ECpimHeaderType eHeaderType,
                IN IAllocator* pAllocator = NULL);

//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Constructor reserved for internal use only.
    CCpimHeader(IN const STxtHeaderDescriptor* pProtocolHeaders,
                IN IAllocator* pAllocator = NULL);
//M5T_INTERNAL_USE_END

    // Summary:
    //  Copy Constructor.
    CCpimHeader(IN const CCpimHeader& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CCpimHeader();

    // Summary:
    //  Assignment Operator.
    CCpimHeader& operator=(IN const CCpimHeader& rSrc);

    // Summary:
    //  Gets the header type.
    ECpimHeaderType GetHeaderType() const;

    // Summary:
    //  Provides access to the From header.
    const CTxtNameAddr& GetFrom() const;

    // <COMBINE CCpimHeader::GetFrom@const>
    CTxtNameAddr& GetFrom();

    // Summary:
    //  Provides access to the To header.
    const CTxtNameAddr& GetTo() const;

    // <COMBINE CCpimHeader::GetTo@const>
    CTxtNameAddr& GetTo();

    // Summary:
    //  Provides access to the cc header.
    const CTxtNameAddr& GetCc() const;

    // <COMBINE CCpimHeader::GetCc@const>
    CTxtNameAddr& GetCc();

    // Summary:
    //  Provides access to the DateTime header.
    const CTxtToken& GetDateTime() const;

    // <COMBINE CCpimHeader::GetDateTime@const>
    CTxtToken& GetDateTime();

    // Summary:
    //  Provides access to the Subject header.
    const CTxtToken& GetSubject() const;

    // <COMBINE CCpimHeader::GetSubject@const>
    CTxtToken& GetSubject();

    // Summary:
    //  Provides access to the NS header.
    const CTxtNameAddr& GetNs() const;

    // <COMBINE CCpimHeader::GetNs@const>
    CTxtNameAddr& GetNs();

    // Summary:
    //  Provides access to the Require header.
    const CTxtToken& GetRequire() const;

    // <COMBINE CCpimHeader::GetRequire@const>
    CTxtToken& GetRequire();

    // Summary:
    //  Returns a pointer to the chained header at the given 0-based index.
    CCpimHeader* GetNextCpimHeader(IN unsigned int uIndex = 0);

    // <COMBINE CCpimHeader::GetNextCpimHeader@IN unsigned int>
    const CCpimHeader* GetNextCpimHeader(IN unsigned int uIndex = 0) const;

    //-- << Parameters management >>

//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Creates an empty parameter list.
    virtual CTxtParameterList* CreateNewParameterList(IN bool bStartWithSeparator = true,
                                                      IN char cSeparator = g_cSEMI);


//-- Hidden Methods.
protected:
private:

    // Summary:
    //  Creates a new header of the same type as the current one.
    virtual GO CTxtHeader* CreateNewHeader();


//-- Hidden Data Members.
protected:
private:
    // The type of header.
    ECpimHeaderType m_eHeaderType;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetHeaderType
//==
//==============================================================================
//
//  Returns:
//      The header's type.
//
//  Description:
//      Gets the header type.
//
//==============================================================================
inline ECpimHeaderType CCpimHeader::GetHeaderType() const
{
    return m_eHeaderType;
}

//==============================================================================
//==
//==  GetFrom
//==
//==============================================================================
//
//  Returns:
//      The From URI.
//
//  Description:
//      Provides access to the From header's URI.
//
//==============================================================================
inline const CTxtNameAddr& CCpimHeader::GetFrom() const
{
    return GetNameAddrForm().m_nameAddr;
}

//-- <COMBINE CCpimHeader::GetFrom@const>
inline CTxtNameAddr& CCpimHeader::GetFrom()
{
    return GetNameAddrForm().m_nameAddr;
}

//==============================================================================
//==
//==  GetTo
//==
//==============================================================================
//
//  Returns:
//      The To URI.
//
//  Description:
//      Provides access to the To header's URI.
//
//==============================================================================
inline const CTxtNameAddr& CCpimHeader::GetTo() const
{
    return GetNameAddrForm().m_nameAddr;
}

//-- <COMBINE CCpimHeader::GetTo@const>
inline CTxtNameAddr& CCpimHeader::GetTo()
{
    return GetNameAddrForm().m_nameAddr;
}

//==============================================================================
//==
//==  GetCc
//==
//==============================================================================
//
//  Returns:
//      The cc URI.
//
//  Description:
//      Provides access to the cc header's URI.
//
//==============================================================================
inline const CTxtNameAddr& CCpimHeader::GetCc() const
{
    return GetNameAddrForm().m_nameAddr;
}

//-- <COMBINE CCpimHeader::GetCc@const>
inline CTxtNameAddr& CCpimHeader::GetCc()
{
    return GetNameAddrForm().m_nameAddr;
}

//==============================================================================
//==
//==  GetDateTime
//==
//==============================================================================
//
//  Returns:
//      The date and time.
//
//  Description:
//      Provides access to the DateTime header's value.
//
//==============================================================================
inline const CTxtToken& CCpimHeader::GetDateTime() const
{
    return GetOneTokenForm().m_tokFirst;
}

//-- <COMBINE CCpimHeader::GetDateTime@const>
inline CTxtToken& CCpimHeader::GetDateTime()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetSubject
//==
//==============================================================================
//
//  Returns:
//      The Subject.
//
//  Description:
//      Provides access to the Subject header's value.
//
//==============================================================================
inline const CTxtToken& CCpimHeader::GetSubject() const
{
    return GetOneTokenForm().m_tokFirst;
}

//-- <COMBINE CCpimHeader::GetSubject@const>
inline CTxtToken& CCpimHeader::GetSubject()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetNs
//==
//==============================================================================
//
//  Returns:
//      The NS URI.
//
//  Description:
//      Provides access to the NS header's URI.
//
//==============================================================================
inline const CTxtNameAddr& CCpimHeader::GetNs() const
{
    return GetNameAddrForm().m_nameAddr;
}

//-- <COMBINE CCpimHeader::GetNs@const>
inline CTxtNameAddr& CCpimHeader::GetNs()
{
    return GetNameAddrForm().m_nameAddr;
}

//==============================================================================
//==
//==  GetRequire
//==
//==============================================================================
//
//  Returns:
//      The Required headers.
//
//  Description:
//      Provides access to the Require header's value.
//
//==============================================================================
inline const CTxtToken& CCpimHeader::GetRequire() const
{
    return GetOneTokenForm().m_tokFirst;
}

//-- <COMBINE CCpimHeader::GetRequire@const>
inline CTxtToken& CCpimHeader::GetRequire()
{
    return GetOneTokenForm().m_tokFirst;
}

//==============================================================================
//==
//==  GetNextCpimHeader
//==
//==============================================================================
//
//  Parameters:
//      uIndex:
//          0-based index of the header to find in the chain.
//
//  Returns:
//      Pointer to the header at the specified index, or NULL if out of range.
//
//  Description:
//      This method finds the header at the specified index in the chain and
//      returns it. This method accesses the list of next headers, which does
//      not include the current instance of CCpimHeader.
//
//==============================================================================
inline CCpimHeader* CCpimHeader::GetNextCpimHeader(IN unsigned int uIndex)
{
    return static_cast<CCpimHeader*>(GetNextHeader(uIndex));
}

//-- <COMBINE CTxtHeader::GetNextCpimHeader@IN unsigned int>
inline const CCpimHeader* CCpimHeader::GetNextCpimHeader(IN unsigned int uIndex) const
{
    return static_cast<const CCpimHeader*>(GetNextHeader(uIndex));
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CCPIMHEADER_H

