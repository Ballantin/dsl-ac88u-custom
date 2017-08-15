//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix Telecom, Inc.
//
//   Mediatrix Telecom, Inc. reserves all rights to this document as well as to
//   the Intellectual Property of the document and the technology and know-how
//   that it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix Telecom, Inc.
//
//   Mediatrix Telecom, Inc. reserves the right to revise this publication and
//   make changes at any time and without the obligation to notify any person
//   and/or entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CGENPARAMLIST_H
#define MXG_CGENPARAMLIST_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CGENERICPARAM_H
#include "SipParser/CGenericParam.h" // ECharSet
#endif


//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CGenParamList
//=========================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//      This class implements a list of SIP parameters, separated by a specified
//      character. The CGenParamList can handle any type of CGenericParam.
//
//      Warning: This class' algorithm uses the premise that all parameter names
//      are unique. The application should not add two parameters that have the
//      same name.
//
//  <CODE>
//      RFC 3261 ABNF
//          (param-list)   =  *( SEMI generic-param )
//          generic-param  =  token [ EQUAL gen-value ]
//          gen-value      =  token / host / quoted-string
//  </CODE>
//
// Example:
//   Fetching a parameter named "tag".
//
//   <CODE>
//      CGenParamList paramList;
//
//      // I already know that the first parameter in the list is "tag".
//      //---------------------------------------------------------------
//      CGenericParam* pTagParam1 = paramList[0];
//
//      if(pTagParam1 != NULL)
//      {
//          // The list contained at least one parameter.
//          //--------------------------------------------
//          if(pTagParam1->GetName() == "tag")
//          {
//              // Check the value.
//              //------------------
//              if(pTagParam1->GetValue() == "some tag")
//              {
//                  ...
//              }
//          }
//          else
//          {
//              // Not the param i'm looking for.
//              //--------------------------------
//          }
//      }
//      else
//      {
//          // tag param was not found
//      }
//
//
//      // I'd like the list to find the parameter by its name.
//      //------------------------------------------------------
//      CGenericParam* pTagParam2 = paramList["tag"];
//
//      if(pTagParam2 != NULL)
//      {
//          // tag param was found
//      }
//      else
//      {
//          // tag param was not found
//      }
//   </CODE>
//
// Location:
//   SipParser/CGenParamList.h
//
// See Also:
//   CGenericParam
//
//==============================================================================
class CGenParamList
{
//-- New types definition
//-----------------------
public:
    // Enum to use in parameter to Parse().
    //--------------------------------------
    enum EStartWithSeparator
    {
        // Parse begins with a parameter name.
        //-------------------------------------
        eNO_SEP,

        // Parse begins with a separator.
        //--------------------------------
        eWITH_SEP
    };

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CGenParamList();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CGenParamList(IN const CGenParamList& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CGenParamList();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CGenParamList& operator=(IN const CGenParamList& rSrc);

    // Summary:
    //  Comparison operator. Compares the parameter lists following RFC 3261
    //  rules. For SipUri parameters, the rules are those of section 19.1.4.
    //  For SIP headers, all parameters must be present and match.
    //-----------------------------------------------------------------------
    bool operator==(IN const CGenParamList& rSrc) const;

    // Summary:
    //  Comparison operator. Compares the parameter lists following RFC 3261
    //  rules. For SipUri parameters, the rules are those of section 19.1.4.
    //  For SIP headers, all parameters must be present and match.
    //-----------------------------------------------------------------------
    bool operator!=(IN const CGenParamList& rSrc) const;

    // Summary:
    //  Returns the parameter at uIndex.
    //-----------------------------------
    inline CGenericParam* operator[](IN unsigned int uIndex);

    // <COMBINE CGenParamList::[]@IN unsigned int>
    //------------------------------
    inline const CGenericParam* operator[](IN unsigned int uIndex) const;

    // Summary:
    //  Returns the parameter whose name matches.
    //--------------------------------------------
    inline CGenericParam* operator[](IN const CString& rstrName);

    // Summary:
    //  Returns the parameter whose name matches.
    //--------------------------------------------
    inline CGenericParam* operator[](IN const char* pszName);

    // <COMBINE CGenParamList::[]@IN const CString&>
    //-----------------------------------------------
    inline const CGenericParam* operator[](IN const CString& rstrName) const;

    // <COMBINE CGenParamList::[]@IN const char*>
    //--------------------------------------------
    inline const CGenericParam* operator[](IN const char* pszName) const;

    //-- << Public methods >>
    //--------------------------------------------

    // Summary:
    //  Adds a parameter at the end of the list. Does not check if the
    //  parameter name is already in the list.
    //-----------------------------------------------------------------
    unsigned int Append(IN TO CGenericParam* pParam);

    // Summary:
    //  Returns true if the list contains no parameter.
    //--------------------------------------------------
    bool IsEmpty() const;

    // Summary:
    //  Returns the number of parameters in the list.
    //------------------------------------------------
    unsigned int Length() const;

    // Summary:
    //  Parses the parameters list beginning at rpcPos. cSeparator is the
    //  character that separates the parameters.
    //--------------------------------------------------------------------
    mxt_result Parse( IN CGenericParam::ECharSet eCharSet,
                      INOUT const char*& rpcPos,
                      IN EStartWithSeparator eStartWithSep = eWITH_SEP,
                      IN char cSeparator = ';' );

    // Summary:
    //  Removes the parameter at the specified index.
    //------------------------------------------------
    bool Remove(IN unsigned int uIndex);

    // Summary:
    //  Removes all parameters of the same name from the list.
    //---------------------------------------------------------
    bool Remove(IN const CString& rstrName);

    // Summary:
    //  Replaces or Adds the parameter.
    //----------------------------------
    void Set(IN TO CGenericParam* pParam);

    // Summary:
    //  Inserts cSeparator after each parameter, except for the last parameter.
    //--------------------------------------------------------------------------
    void Serialize(INOUT CBlob& rBlob, IN char cSeparator = ';') const;

    // Summary:
    //  Resets this object.
    //----------------------
    void Reset();


//-- Hidden Methods
//-------------------
protected:
    bool CompareUriParamList( IN const CGenParamList& rL1,
                              IN const CGenParamList& rL2,
                              IN CToken::ECharSet eCharSet ) const;

    unsigned int FindIndex(IN const CString& rstrName) const;
    unsigned int FindIndex(IN const char* pszName) const;


private:
    // Summary:
    //  This is a helper method for operator[]. It returns the parameter at 
    //  index uIndex.
    //----------------------------------------------------------------------
    CGenericParam* InternalGetAt(IN unsigned int uIndex) const;

//-- Hidden Data Members
//------------------------
protected:

    // Contains the list of parameter pointers. The parameters are serialized
    // in sequential order.
    //------------------------------------------------------------------------
    CVector<CGenericParam*> m_vecpParam;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rSrc:
//      List against which to compare.
//
//  Returns:
//      True if the lists are not equivalent following RFC 3261 rules.
//
//  Description:
//      Inequality operator. See CGenParamList::operator==.
//
//  See Also:
//      operator==
//
//==EDOC========================================================================
inline bool CGenParamList::operator!=(IN const CGenParamList& rSrc) const
{
    return !(*this == rSrc);
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Zero-based index of the parameter to fetch.
//
//  Returns:
//      Pointer to the parameter at the given index. NULL if the index is
//      out-of-bounds.
//
//  Description:
//      This method provides array-style access to individual parameters in the
//      list.
//
//==EDOC========================================================================
inline CGenericParam* CGenParamList::operator[](IN unsigned int uIndex)
{
    return InternalGetAt(uIndex);
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      Zero-based index of the parameter to fetch.
//
//  Returns:
//      Pointer to the parameter at the given index. It is NULL if the index is
//      out-of-bounds.
//
//  Description:
//      This method provides array-style access to individual parameters in the
//      list.
//
//==EDOC========================================================================
inline const CGenericParam* CGenParamList::operator[](IN unsigned int uIndex) const
{
    return InternalGetAt(uIndex);
}


//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//    rstrName:
//      Name of the parameter to fetch. Case sensitivity depends on the type of
//      parameter this list is holding.
//
//  Returns:
//      Pointer to the first parameter that matches the name. It is NULL if the
//      name is not matched. Note that if two parameters have the same name,
//      only the first one is returned.
//
//  Description:
//      This method provides array-style access to individual parameters in the
//      list.
//
//==EDOC========================================================================
inline CGenericParam* CGenParamList::operator[](IN const CString& rstrName)
{
    return InternalGetAt(FindIndex(rstrName));
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//    pszName:
//      Name of the parameter to fetch. Case sensitivity depends on the type of
//      parameter this list is holding.
//
//  Returns:
//      Pointer to the first parameter that matches the name. It is NULL if the
//      name is not matched. Note that if two parameters have the same name,
//      only the first one is returned.
//
//  Description:
//      This method provides array-style access to individual parameters in the
//      list.
//
//==EDOC========================================================================
inline CGenericParam* CGenParamList::operator[](IN const char* pszName)
{
    return InternalGetAt(FindIndex(pszName));
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//    rstrName:
//      Name of the parameter to fetch. Case sensitivity depends on the type of
//      parameter this list is holding.
//
//  Returns:
//      Pointer to the first parameter that matches the name. It is NULL if the
//      name is not matched. Note that if two parameters have the same name,
//      only the first one is returned.
//
//  Description:
//      This method provides array-style access to individual parameters in the
//      list.
//
//==EDOC========================================================================
inline const CGenericParam* CGenParamList::operator[](IN const CString& rstrName) const
{
    return InternalGetAt(FindIndex(rstrName));
}

//==SDOC========================================================================
//==
//==  operator[]
//==
//==============================================================================
//
//  Parameters:
//    pszName:
//      Name of the parameter to fetch. Case sensitivity depends on the type of
//      parameter this list is holding.
//
//  Returns:
//      Pointer to the first parameter that matches the name. It is NULL if the
//      name is not matched. Note that if two parameters have the same name,
//      only the first one is returned.
//
//  Description:
//      This method provides array-style access to individual parameters in the
//      list.
//
//==EDOC========================================================================
inline const CGenericParam* CGenParamList::operator[](IN const char* pszName) const
{
    return InternalGetAt(FindIndex(pszName));
}

//==SDOC========================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True if the parameter list contains no parameter.
//
//  Description:
//      Returns true if the parameter list contains no parameter.
//
//==EDOC========================================================================
inline bool CGenParamList::IsEmpty() const
{
    return m_vecpParam.IsEmpty();
}

//==SDOC========================================================================
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
//==EDOC========================================================================
inline unsigned int CGenParamList::Length() const
{
    return m_vecpParam.GetSize();
}

//==SDOC========================================================================
//==
//==  FindIndex
//==
//==============================================================================
//
//  Parameters:
//    rstrName:
//      Name of the parameter to find.
//
//  Returns:
//      Zero-based index of the parameter, Length() if not found.
//
//  Description:
//      Finds a parameter in the list by using its name. Only returns the first
//      parameter index if multiple parameters have the same name.
//
//==EDOC========================================================================
inline unsigned int CGenParamList::FindIndex(IN const CString& rstrName) const
{
    return FindIndex(rstrName.CStr());
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CGENPARAMLIST_H
