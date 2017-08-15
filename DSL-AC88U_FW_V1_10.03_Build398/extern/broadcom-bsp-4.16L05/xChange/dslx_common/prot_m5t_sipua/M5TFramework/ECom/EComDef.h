//==SDOC========================================================================
//==============================================================================
//
//        Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ECOMDEF_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ECOMDEF_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_ECOM_ENABLE_SUPPORT
#endif

#if defined(MXD_ECOM_ENABLE_SUPPORT)

#ifndef MXG_CSHAREDPTR_H
#include "Basic/CSharedPtr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
class IEComUnknown;
class CEComUnknown;

//==============================================================================
//====  PRIVATE TYPE DEFINITIONS  ==============================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
/* PRIVATE */
/* PRIVATE */ // Internal representation of iid and clsid.
/* PRIVATE */ struct SEComGuid
/* PRIVATE */ {
/* PRIVATE */     unsigned int m_nLength;
/* PRIVATE */     const char* m_pszValue;
/* PRIVATE */
/* PRIVATE */     // Possibly add yy:mm:dd:hh:mm. Needed in the macro as well.
/* PRIVATE */ };
/* PRIVATE */
//M5T_INTERNAL_USE_END

//==============================================================================
//====  PUBLIC TYPE DEFINITIONS  ===============================================
//==============================================================================

//<GROUP ECOM_TYPES>
//
//  Summary:
//      CreateEComInstance function prototype used by the ECOM mechanism to
//      create a new instance of an ECOM class.
//--------------------------------------------------------------------------
typedef mxt_result (*mxt_pfnCreateEComInstance)(IN IEComUnknown* pOuterIEComUnknown,
                                                OUT CEComUnknown** ppCEComUnknown);
//<GROUP ECOM_TYPES>
//
//  Summary:
//      ECOM interface identifier basic data type. The IID is contained in a
//      variable declared with the macro MX_DECLARE_ECOM_IID in each interface
//      include file.
//-----------------------------------------------------------------------------
typedef const SEComGuid* mxt_iid;

//<GROUP ECOM_TYPES>
//
//  Summary:
//      ECOM class identifier basic data type. The CLISD is contained in a
//      variable declared with the macro MX_DECLARE_ECOM_CLSID in each package
//      EComCLSID.h include file.
//-----------------------------------------------------------------------------
typedef const SEComGuid* mxt_clsid;

//==============================================================================
//====  PRIVATE FUNCTION PROTOTYPE  ============================================
//==============================================================================

//==============================================================================
//====  PRIVATE TYPE DEFINITIONS  ==============================================
//==============================================================================


//==============================================================================
//====  PRIVATE MACROS  ========================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
/* PRIVATE */ #define MX_MAKESTRING(iidString) #iidString
//M5T_INTERNAL_USE_END

//==============================================================================
//====  PRIVATE INLINE FUNCTIONS  ==============================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
/* PRIVATE */
/* PRIVATE */ // The abstract comparison method for the guids
/* PRIVATE */ inline
/* PRIVATE */ bool IsEqualSEComGuid( IN const SEComGuid* pstEComGuid1,
/* PRIVATE */                        IN const SEComGuid* pstEComGuid2 )
/* PRIVATE */ {
/* PRIVATE */     return ( pstEComGuid1->m_nLength != pstEComGuid2->m_nLength ) ?
/* PRIVATE */         false :
/* PRIVATE */         memcmp( pstEComGuid1->m_pszValue,
/* PRIVATE */                 pstEComGuid2->m_pszValue,
/* PRIVATE */                 pstEComGuid1->m_nLength ) == 0;
/* PRIVATE */ }
/* PRIVATE */
/* PRIVATE */ //================================================================
/* PRIVATE */ //==
/* PRIVATE */ //==  IsEqualEComCLSID
/* PRIVATE */ //==
/* PRIVATE */ //================================================================
/* PRIVATE */ //
/* PRIVATE */ //  Description:
/* PRIVATE */ //      This method is a simple utility that compares two class
/* PRIVATE */ //      identifiers and tells whether or not they are equal. This
/* PRIVATE */ //      method is only used internally by the CreateEComInstance.
/* PRIVATE */ //
/* PRIVATE */ //================================================================
/* PRIVATE */ inline
/* PRIVATE */ bool IsEqualEComCLSID(IN const mxt_clsid clsid1, IN const mxt_clsid clsid2 )
/* PRIVATE */ {
/* PRIVATE */     return IsEqualSEComGuid( clsid1, clsid2);
/* PRIVATE */ }
/* PRIVATE */
//M5T_INTERNAL_USE_END

//==============================================================================
//====  PUBLIC MACROS  =========================================================
//==============================================================================

//==============================================================================
//==
//==  MX_DECLARE_ECOM_IID
//==
//==============================================================================
//<GROUP ECOM_MACROS>
//
//  Summary:
//      Declares an ECOM interface identifier.
//
//  Parameters:
//      szIfaceName:
//        The name of the interface used in the class definition
//
//  Description:
//      Declares an ECOM interface identifier ("IID") in the .h file of an ECOM
//      interface based on the class name.
//
//      The name of the variable is formed as follows: IID_InterfaceName.
//      Its content is generated automatically and must be unique. Each
//      interface inheriting from IEComUnknown must have an IID assigned.
//
//  <CODE>
//
//      MX_DECLARE_ECOM_IID(ISomeInterface);
//
//      class ISomeInterface : public IEComUnknown
//      {
//      public:
//          MX_DECLARE_ECOM_GETIID(ISomeInterface);
//
//          ...
//      };
//
//  </CODE>
//
//  See also:
//      mxt_iid, CreateEComInstance, IEComUnknown::QueryIf, IEComUnknown
//
//==============================================================================
#define MX_DECLARE_ECOM_IID(szIfaceName)                                        \
    const SEComGuid IID_##szIfaceName[] = {{sizeof(MX_MAKESTRING(szIfaceName)), \
                                            MX_MAKESTRING(szIfaceName)          \
                                          }};

//==============================================================================
//==
//==  MX_DECLARE_ECOM_GETIID
//==
//==============================================================================
//<GROUP ECOM_MACROS>
//
//  Summary:
//      Declares a method GetIID.
//
//  Parameters:
//      szIfaceName:
//        The name of the interface used in the class definition
//
//  Description:
//      Declares a method GetIID. This method is required to support the
//      specialized versions of CreateEComInstance and QueryIf.
//
//  <CODE>
//
//      MX_DECLARE_ECOM_IID(ISomeInterface);
//
//      class ISomeInterface : public IEComUnknown
//      {
//      public:
//          MX_DECLARE_ECOM_GETIID(ISomeInterface);
//
//          ...
//      };
//
//  </CODE>
//
//  See also:
//      CreateEComInstance, IEComUnknown::QueryIf
//
//==============================================================================
#define MX_DECLARE_ECOM_GETIID(szIfaceName)                                     \
    mxt_iid GetIID() const { return IID_##szIfaceName; }

//==============================================================================
//==
//==  MX_DECLARE_ECOM_CLSID
//==
//==============================================================================
//<GROUP ECOM_MACROS>
//
//  Summary:
//      Declares an ECOM class identifier.
//
//  Parameters:
//      szClassName:
//        The name of the ECOM class used in the class identifier definition.
//
//  Description:
//      Declares an ECOM class identifier ("CLSID") in the EComCLSID.h file of
//      a package based on the class name.
//
//      The name of the variable is formed as follows: CLSID_ClassName.
//      Its content is generated automatically and must be unique. Each
//      ECOM inheriting from CEComUnknown or CEComDelegatingUnknown must have a
//      CLSID assigned.
//
//  See also:
//      mxt_clsid, CreateEComInstance, CEComUnknown, CEComDelegatingUnknown
//
//==============================================================================
#define MX_DECLARE_ECOM_CLSID( szClassName )                                     \
    const SEComGuid CLSID_##szClassName[] = {{sizeof(MX_MAKESTRING(szClassName)),\
                                              MX_MAKESTRING(szClassName)         \
                                            }};

//==============================================================================
//==
//==  MX_DECLARE_DELEGATING_IECOMUNKNOWN
//==
//==============================================================================
//<GROUP ECOM_MACROS>
//
//  Summary:
//      Provides the unique implementation of the IEComUnknown interface.
//
//  Description:
//      Provides the unique implementation of the IEComUnknown interface.
//      As the macro shows, all calls to QueryIf are delegated to
//      the IEComUnknown owner for this ECOM.
//
//  See also:
//      CEComDelegatingUnknown
//
//==============================================================================
#define MX_DECLARE_DELEGATING_IECOMUNKNOWN                                      \
    template<class _Type> mxt_result QueryIf(OUT _Type** ppInterface)           \
    {                                                                           \
        if (ppInterface != NULL)                                                \
        {                                                                       \
            return QueryIf((*ppInterface)->GetIID(),                            \
                           reinterpret_cast<void**>(ppInterface));              \
        }                                                                       \
        return resFE_INVALID_ARGUMENT;                                          \
    }                                                                           \
                                                                                \
    template<class _Type> mxt_result QueryIf(OUT CSharedPtr<_Type>& rInterface) \
    {                                                                           \
        _Type** ppInterface = static_cast<_Type**>(rInterface);                 \
        return QueryIf((*ppInterface)->GetIID(),                                \
                       reinterpret_cast<void**>(ppInterface));                  \
    }                                                                           \
                                                                                \
    virtual mxt_result QueryIf(IN mxt_iid iidRequested, OUT void** ppInterface) \
    {                                                                           \
        return GetOwnerIEComUnknown()->QueryIf(iidRequested, ppInterface);      \
    }                                                                           \
                                                                                \
    virtual unsigned int AddIfRef()                                             \
    {                                                                           \
        return GetOwnerIEComUnknown()->AddIfRef();                              \
    }                                                                           \
                                                                                \
    virtual unsigned int ReleaseIfRef()                                         \
    {                                                                           \
        return GetOwnerIEComUnknown()->ReleaseIfRef();                          \
    }

//==============================================================================
//==
//==  MX_DECLARE_IECOMUNKNOWN
//==
//==============================================================================
//<GROUP ECOM_MACROS>
//
//  Summary:
//      Provides the unique implementation of the IEComUnknown interface.
//
//  Description:
//      Provides the unique implementation of the IEComUnknown interface.
//      As the macro shows, all calls to QueryIf are delegated to the ECOM
//      object itself.
//
//  Note:
//      Using a unique macro with the same implementation for
//      MX_DECLARE_IECOMUNKNOWN and MX_DECLARE_DELEGATING_IECOMUNKNOWN would
//      have been possible. The distinction was created for performance to avoid
//      the cost of dereferencing the IEComUnknown owner when the ECOM object
//      can not be aggregated.
//
//  See also:
//      CEComUnknown
//
//==============================================================================
#define MX_DECLARE_IECOMUNKNOWN                                                 \
    template<class _Type> mxt_result QueryIf(OUT CSharedPtr<_Type>& rInterface) \
    {                                                                           \
        _Type** ppInterface = static_cast<_Type**>(rInterface);                 \
        return  NonDelegatingQueryIf((*ppInterface)->GetIID(),                  \
                                     reinterpret_cast<void**>(ppInterface));    \
    }                                                                           \
                                                                                \
    template<class _Type> mxt_result QueryIf(OUT _Type** ppInterface)           \
    {                                                                           \
        if (ppInterface != NULL)                                                \
        {                                                                       \
            return NonDelegatingQueryIf((*ppInterface)->GetIID(),               \
                                        reinterpret_cast<void**>(ppInterface)); \
        }                                                                       \
        return resFE_INVALID_ARGUMENT;                                          \
    }                                                                           \
                                                                                \
    virtual mxt_result QueryIf(IN mxt_iid iidRequested, OUT void** ppInterface) \
    {                                                                           \
        return NonDelegatingQueryIf(iidRequested, ppInterface);                 \
    }                                                                           \
                                                                                \
    virtual unsigned int AddIfRef()                                             \
    {                                                                           \
        return NonDelegatingAddIfRef();                                         \
    }                                                                           \
                                                                                \
    virtual unsigned int ReleaseIfRef()                                         \
    {                                                                           \
        return NonDelegatingReleaseIfRef();                                     \
    }

//==============================================================================
//====  PUBLIC FUNCTIONS PROTOTYPE  ============================================
//==============================================================================

//==============================================================================
//==
//==  CreateEComInstance
//==
//==============================================================================
//<GROUP ECOM_FUNCTIONS>
//
//  Summary:
//      Creates an ECOM class instance.
//
//  Parameters:
//      clsid:
//          The class identifier (CLSID) of the ECOM class to create.
//
//      pOuterIEComUnknown:
//          The pointer to an outer ECOM class if the ECOM class to create is
//          being aggregated. If non null, use GetOwnerIEComUnknown to assign
//          argument.
//
//      iidRequested:
//          The interface identifier (IID) of the interface to retrieve.
//
//      ppInterface:
//          The pointer of the interface pointer variable to store the
//          requested interface.
//
//  Returns:
//      * resS_OK: The creation succeeded. ppInterface now contains a
//                 pointer to the requested interface.
//
//      * resFE_INVALID_ARGUMENT: The creation failed. The supplied ppInterface
//                                is invalid.
//
//      * resFE_MITOSFW_ECOM_NOINTERFACE: The creation failed. No interface has
//                                        been found corresponding to the
//                                        iidRequested.
//                                        In this condition, NULL is assigned to
//                                        the content of ppInterface
//
//      * resFE_MITOSFW_ECOM_CLSIDNOTAVAILABLE: The creation failed. The class
//                                              identifier corresponding to
//                                              clsid does not exist.
//
//      * Other(s): A result code that can be returned by the
//                  CEComUnknown::InitializeInstance of the ECOM class
//                  associated with the clsid if it was overriden.
//
//  Description:
//      This method creates in memory an instance of ECOM class associated
//      with clsid. Usually, IID_IEComUnknown is assigned to iidRequested for
//      the call, but it is not an obligation as long as it is supported by the
//      class. The interface identified by iidRequested is assigned to
//      ppInterface if supported by the ECOM class. The interface returned
//      through ppInterface is the first reference and currently the unique one
//      on the ECOM class.
//
//      When the ECOM class to create is not being aggregated,
//      ppOuterIEComUnknown must be assigned NULL. Otherwise, it is called
//      within another ECOM class and ppOuterIEComUnknown MUST be assigned with
//      the ECOM class's GetOwnerIEComUnknown method. In that case, iidRequested
//      MUST be assigned with IID_IEComUnknown.
//
//  See also:
//      IEComUnknown, mxt_clsid, mxt_iid, MX_DECLARE_ECOM_CLSID,
//      MX_DECLARE_ECOM_IID
//
//==============================================================================
mxt_result CreateEComInstance(IN const mxt_clsid clsid,
                              IN IEComUnknown* pOuterIEComUnknown,
                              IN const mxt_iid iidRequested,
                              OUT void** ppInterface);

//-- This forward declaration is required for the function to appear in the
//-- API Ref document. Doc-o-matic has a hard time with templated inline global
//-- functions.
template<class _Type> inline mxt_result CreateEComInstance(IN const mxt_clsid clsid,
                              IN IEComUnknown* pOuterIEComUnknown,
                              OUT _Type** ppInterface);

//==============================================================================
//==
//==  CreateEComInstance
//==
//==============================================================================
//<GROUP ECOM_FUNCTIONS>
//
//  Summary:
//      Creates an ECOM class instance.
//
//  Parameters:
//      clsid:
//          The class identifier (CLSID) of the ECOM class to create.
//
//      pOuterIEComUnknown:
//          The pointer to an outer ECOM class if the ECOM class to create is
//          being aggregated. If non null, use GetOwnerIEComUnknown to assign
//          argument.
//
//      ppInterface:
//          The pointer of the interface pointer variable to store the
//          requested interface.
//
//  Returns:
//      * resS_OK: The creation succeeded. ppInterface now contains a
//                 pointer to the requested interface.
//
//      * resFE_INVALID_ARGUMENT: The creation failed. The supplied ppInterface
//                                is invalid.
//
//      * resFE_MITOSFW_ECOM_NOINTERFACE: The creation failed. No interface has
//                                        been found corresponding to the
//                                        iidRequested.
//                                        In this condition, NULL is assigned to
//                                        the content of ppInterface
//
//      * resFE_MITOSFW_ECOM_CLSIDNOTAVAILABLE: The creation failed. The class
//                                              identifier corresponding to
//                                              clsid does not exist.
//
//      * Other(s): A result code that can be returned by the
//                  CEComUnknown::InitializeInstance of the ECOM class
//                  associated with the clsid if it was overriden.
//
//  Description:
//      This is a templated version of the original CreateEComInstance method.
//      It is designed to ease code readability.
//
//      This method creates in memory an instance of ECOM class associated
//      with clsid. The interface is retrieved from the ppInterface parameter
//      and is returned if supported by the ECOM class. The interface returned
//      through ppInterface is the first reference and currently the unique one
//      on the ECOM class.
//
//      When the ECOM class to create is not being aggregated,
//      ppOuterIEComUnknown must be assigned NULL. Otherwise, it is called
//      within another ECOM class and ppOuterIEComUnknown must be assigned with
//      the class's GetOwnerIEComUnknown method. In that case, ppInterface MUST
//      be of type IEComUnknown.
//
//  See also:
//      IEComUnknown, mxt_clsid, MX_DECLARE_ECOM_CLSID, MX_DECLARE_ECOM_GETIID,
//      MX_DECLARE_ECOM_IID
//
//==============================================================================
template<class _Type>
inline mxt_result CreateEComInstance(IN const mxt_clsid clsid,
                                     IN IEComUnknown* pOuterIEComUnknown,
                                     OUT _Type** ppInterface)
{
    return CreateEComInstance(clsid,
                              pOuterIEComUnknown,
                              (*ppInterface)->GetIID(),
                              reinterpret_cast<void**>(ppInterface));
}

//==============================================================================
//==
//==  CreateEComInstance
//==
//==============================================================================
//<GROUP ECOM_FUNCTIONS>
//
//  Summary:
//      Creates an ECOM class instance.
//
//  Parameters:
//      clsid:
//          The class identifier (CLSID) of the ECOM class to create.
//
//      pOuterIEComUnknown:
//          The pointer to an outer ECOM class if the ECOM class to create is
//          being aggregated. If non null, use GetOwnerIEComUnknown to assign
//          argument.
//
//      rInterface:
//          A reference to a shared pointer to store the requested interface.
//
//  Returns:
//      * resS_OK: The creation succeeded. ppInterface now contains a
//                 pointer to the requested interface.
//
//      * resFE_INVALID_ARGUMENT: The creation failed. The supplied ppInterface
//                                is invalid.
//
//      * resFE_MITOSFW_ECOM_NOINTERFACE: The creation failed. No interface has
//                                        been found corresponding to the
//                                        iidRequested.
//                                        In this condition, NULL is assigned to
//                                        the content of ppInterface
//
//      * resFE_MITOSFW_ECOM_CLSIDNOTAVAILABLE: The creation failed. The class
//                                              identifier corresponding to
//                                              clsid does not exist.
//
//      * Other(s): A result code that can be returned by the
//                  CEComUnknown::InitializeInstance of the ECOM class
//                  associated with the clsid if it was overridden.
//
//  Description:
//      This is a templated version of the original CreateEComInstance method.
//      It is designed to ease code readability.
//
//      This method creates in memory an instance of ECOM class associated
//      with clsid. The interface is retrieved from the rInterface parameter
//      and is returned if supported by the ECOM class. The interface returned
//      through rInterface is the first reference and currently the unique one
//      on the ECOM class.
//
//      When the ECOM class to create is not being aggregated,
//      ppOuterIEComUnknown must be assigned NULL. Otherwise, it is called
//      within another ECOM class and ppOuterIEComUnknown must be assigned with
//      the class's GetOwnerIEComUnknown method. In that case, ppInterface MUST
//      be of type IEComUnknown.
//
//  See also:
//      IEComUnknown, mxt_clsid, MX_DECLARE_ECOM_CLSID, MX_DECLARE_ECOM_GETIID,
//      MX_DECLARE_ECOM_IID
//
//==============================================================================
template<class _Type>
inline mxt_result CreateEComInstance(IN const mxt_clsid clsid,
                                     IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CSharedPtr<_Type>& rInterface)
{
    return CreateEComInstance(clsid, pOuterIEComUnknown, static_cast<_Type**>(rInterface));
}

//M5T_INTERNAL_USE_BEGIN
//==============================================================================
//==
//==  FinalizeECom
//==
//==============================================================================
//
//  Summary:
//      Finalizes the ECOM mechanism.
//
//  Returns:
//      * resS_OK: The finalization successful.
//
//  Description:
//      Finalizes the ECOM mechanism. This method must be called when the ECOM
//      mechanism is no longer used. An equivalent call to InitializeECom must
//      have been performed previously.
//
//==============================================================================
//void FinalizeECom();

//==============================================================================
//==
//==  InitializeECom
//==
//==============================================================================
//
//  Summary:
//      Initializes the ECOM mechanism.
//
//  Returns:
//      * resS_OK: The initialization was successful.
//
//  Description:
//      Initializes the ECOM mechanism. This method must be called before the
//      ECOM mechanism may be used. An equivalent call to FinalizeECom must be
//      be performed when the ECOM mechanism is no longer used.
//
//==============================================================================
//mxt_result InitializeECom();
//M5T_INTERNAL_USE_END

//==============================================================================
//==
//==  IsEqualECom
//==
//==============================================================================
//<GROUP ECOM_FUNCTIONS>
//
//  Summary:
//      Compares equality of two interfaces.
//
//  Parameters:
//      pIEComIf1:
//          First interface.
//
//      pIEComIf2:
//          Second interface.
//
//  Returns:
//      * true: If pIEComIf1 is implemented by the same ECOM as pIEComIf2.
//
//      * false: If pIEComIf1 is not implemented by the same ECOM as pIEComIf2.
//
//  Description:
//      This method is a simple utility that compares two interfaces and tells
//      whether or not they are implemented by the same ECOM class.
//
//==============================================================================
bool IsEqualECom(IN IEComUnknown* pIEComIf1, IN IEComUnknown* pIEComIf2);

//==============================================================================
//==
//==  RegisterECom
//==
//==============================================================================
//<GROUP ECOM_FUNCTIONS>
//
//  Summary:
//      Registers an ECOM class.
//
//  Parameters:
//      clsid:
//          The class identifier (CLSID).
//
//      pfnCreateEComInstance:
//          A function that serves to create a new instance.
//
//  Returns:
//      * resS_OK: The registration succeeded.
//
//      * resFE_INVALID_ARGUMENT: The registration failed.
//
//      * resFE_INVALID_STATE: The unregistration failed - Already Exists.
//
//  Description:
//      This method must be called to register an ECOM class before it can be
//      created with CreateEComInstance.
//
//==============================================================================
mxt_result RegisterECom(IN const mxt_clsid clsid,
                        IN mxt_pfnCreateEComInstance pfnCreateEComInstance);

//==============================================================================
//==
//==  UnregisterECom
//==
//==============================================================================
//<GROUP ECOM_FUNCTIONS>
//
//  Summary:
//      Unregisters an ECOM class.
//
//  Parameters:
//      clsid:
//          The class identifier (CLSID).
//
//  Returns:
//      * resS_OK: The unregistration succeeded.
//
//      * resFE_INVALID_ARGUMENT: The unregistration failed.
//
//      * resFE_INVALID_STATE: The unregistration failed - Not Found.
//
//  Description:
//      This method may be called to unregister an ECOM class. It is no longer
//      instantiable from CreateEComInstance.
//
//==============================================================================
mxt_result UnregisterECom(IN const mxt_clsid clsid);

//==============================================================================
//====  PUBLIC INLINE FUNCTIONS  ===============================================
//==============================================================================

//-- This forward declaration is required for the function to appear in the
//-- API Ref document. Doc-o-matic has a hard time with inline global functions.
inline bool IsEqualEComIID(IN const mxt_iid iid1, IN const mxt_iid iid2);

//==============================================================================
//==
//==  IsEqualEComIID
//==
//==============================================================================
//<GROUP ECOM_FUNCTIONS>
//
//  Summary:
//      Compares equality of two interfaces identifier.
//
//  Parameters:
//      iid1:
//          First interface identifier.
//
//      iid2:
//          Second interface identifier.
//
//  Returns:
//      * true: If iid1 is equal to iid2.
//
//      * false: If iid1 is not equal to iid2.
//
//  Description:
//      This method is a simple utility that compares two interfaces identifier
//      and tells whether or not they are equal. It is really useful for
//      overriding the QueryIf method.
//
//  See also:
//      IEComUnknown, CEComUnknown
//
//==============================================================================
inline bool IsEqualEComIID(IN const mxt_iid iid1, IN const mxt_iid iid2)
{
    return IsEqualSEComGuid(iid1, iid2);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // #ifndef MXG_ECOMDEF_H

